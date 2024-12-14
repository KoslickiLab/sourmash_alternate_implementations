#include "MultiSketchIndex.h"

MultiSketchIndex::MultiSketchIndex(int num_of_indices) {
    // Constructor
    this->num_of_indices = num_of_indices;
    multiple_sketch_indices = std::vector<std::unordered_map<hash_t, std::vector<int>>>(num_of_indices);
    mutexes = std::vector<std::mutex>(num_of_indices);
}


MultiSketchIndex::MultiSketchIndex() {
    // Constructor
    this->num_of_indices = 4096;
    multiple_sketch_indices = std::vector<std::unordered_map<hash_t, std::vector<int>>>(num_of_indices);
    mutexes = std::vector<std::mutex>(num_of_indices);
}


MultiSketchIndex::~MultiSketchIndex() {
    // Destructor
}


void MultiSketchIndex::add_hash(hash_t hash_value, std::vector<int> sketch_indices) {
    // Add the hash value to the index
    int idx_of_hash = index_of_hash(hash_value);
    mutexes[idx_of_hash].lock();
    multiple_sketch_indices[idx_of_hash][hash_value] = sketch_indices;
    mutexes[idx_of_hash].unlock();
}


void MultiSketchIndex::add_hash(hash_t hash_value, int sketch_index) {
    // Add the hash value to the index
    int idx_of_hash = index_of_hash(hash_value);
    mutexes[idx_of_hash].lock();
    if (!hash_exists(hash_value)) {
        multiple_sketch_indices[idx_of_hash][hash_value] = std::vector<int>();
    }
    multiple_sketch_indices[idx_of_hash][hash_value].push_back(sketch_index);
    mutexes[idx_of_hash].unlock();
}






const std::vector<int>& MultiSketchIndex::get_sketch_indices(hash_t hash_value) const {
    // Get the sketch indices for the hash value
    int idx_of_hash = index_of_hash(hash_value);
    if (hash_exists(hash_value)) {
        return multiple_sketch_indices[idx_of_hash].at(hash_value);
    } else {
        return empty_vector;
    }
}



void MultiSketchIndex::remove_hash(hash_t hash_value, int sketch_index) {
    // Remove the hash value from the index
    int idx_of_hash = index_of_hash(hash_value);
    mutexes[idx_of_hash].lock();
    if (hash_exists(hash_value)) {
        std::vector<int> &sketch_indices = multiple_sketch_indices[idx_of_hash][hash_value];
        // Remove the sketch index from the vector
        size_t index_to_remove = 0;
        for (size_t i = 0; i < sketch_indices.size(); i++) {
            if (sketch_indices[i] == sketch_index) {
                index_to_remove = i;
                break;
            }
        }
        sketch_indices.erase(sketch_indices.begin() + index_to_remove);
        if (sketch_indices.size() == 0) {
            multiple_sketch_indices[idx_of_hash].erase(hash_value);
        }
    }
    mutexes[idx_of_hash].unlock();
}



std::vector<int> MultiSketchIndex::remove_hash(hash_t hash_value) {
    // Remove the hash value from the index
    int idx_of_hash = index_of_hash(hash_value);
    std::vector<int> sketch_indices;
    mutexes[idx_of_hash].lock();
    if (hash_exists(hash_value)) {
        sketch_indices = multiple_sketch_indices[idx_of_hash][hash_value];
        multiple_sketch_indices[idx_of_hash].erase(hash_value);
    } else {
        sketch_indices = empty_vector;
    }
    mutexes[idx_of_hash].unlock();
    return sketch_indices;
}



void MultiSketchIndex::write_one_chunk(std::string filename, int start_index, int end_index) {
    // Write one chunk of the index to a file
    std::ofstream file(filename, std::ios::binary);
    for (int i = start_index; i < end_index; i++) {
        for (auto const& [hash_value, sketch_indices] : multiple_sketch_indices[i]) {
            // write hash_value
            file.write(reinterpret_cast<const char*>(&hash_value), sizeof(hash_t));
            // write number of sketch indices
            int num_sketch_indices = sketch_indices.size();
            file.write(reinterpret_cast<const char*>(&num_sketch_indices), sizeof(int));
            // write sketch indices
            for (size_t j = 0; j < num_sketch_indices; j++) {
                file.write(reinterpret_cast<const char*>(&sketch_indices[j]), sizeof(int));
            }
        }
    }
}



bool MultiSketchIndex::write_to_file(std::string directory_name, 
                                    int num_threads, 
                                    std::vector<SketchInfo> info_of_sketches,
                                    bool force_write) {
    // check if the directory exists, if not then create it
    struct stat info;
    if (stat(directory_name.c_str(), &info) != 0) {
        std::cout << "Error: Directory does not exist. Creating." << std::endl;
        std::string command = "mkdir -p " + directory_name;
        bool success = system(command.c_str());
        if (!success) {
            std::cout << "Error: Could not create directory." << std::endl;
            return false;
        }
    }

    // directory exists, now check if its empty
    DIR* dir = opendir(directory_name.c_str());
    struct dirent* ent;
    bool is_empty = true;
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
            is_empty = false;
            break;
        }
    }

    if (!is_empty) {
        std::cout << "Error: Directory is not empty." << std::endl;
        std::cout << "Continue anyway? (type y/n): ";
        if (force_write) {
            std::cout << "writing anyway (force)." << std::endl;
        } else {
            char response;
            std::cin >> response;
            if (response != 'y') {
                return false;
            }
        }
    }

    std::vector<std::string> files_written;
    std::vector<std::thread> threads;
    int chunk_size = num_of_indices / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start_index = i * chunk_size;
        int end_index = (i == num_threads - 1) ? num_of_indices : (i + 1) * chunk_size;
        std::string filename_this_thread = directory_name + "/chunk_" + std::to_string(i);
        threads.push_back(std::thread(&MultiSketchIndex::write_one_chunk, 
                            this, 
                            filename_this_thread, 
                            start_index, 
                            end_index));
        files_written.push_back(filename_this_thread);
    }

    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    std::string filename = directory_name + "/summary";
    std::ofstream output_file(filename);
    // num of files
    output_file << files_written.size() << std::endl;
    // individual file names (of the chunks)
    for (int i = 0; i < num_threads; i++) {
        output_file << files_written[i] << std::endl;
    }
    // num of genomes/sketches
    output_file << info_of_sketches.size() << std::endl;
    
    // next, one by one, write the info of the sketches
    for (int i = 0; i < info_of_sketches.size(); i++) {
        output_file << info_of_sketches[i].get_str_representation() << std::endl;
    }

    return true;
}




void MultiSketchIndex::load_one_chunk(std::string filename) {
    // Load one chunk of the index from a file
    std::ifstream input_file(filename, std::ios::binary);
    hash_t hash_value;
    int num_sketch_indices;
    while (input_file.read(reinterpret_cast<char*>(&hash_value), sizeof(hash_t))) {
        input_file.read(reinterpret_cast<char*>(&num_sketch_indices), sizeof(int));
        std::vector<int> sketch_indices;
        for (int i = 0; i < num_sketch_indices; i++) {
            int sketch_index;
            input_file.read(reinterpret_cast<char*>(&sketch_index), sizeof(int));
            sketch_indices.push_back(sketch_index);
        }
        add_hash(hash_value, sketch_indices);
    }
}




std::vector<SketchInfo> MultiSketchIndex::load_from_file(std::string directory_name){
    // Load an index from a file
    std::string summary_filename = directory_name + "/summary";
    std::ifstream summary_file(summary_filename);   

    if (!summary_file.is_open()) {
        std::cout << "Error: Could not open summary file." << std::endl;
        exit(1);
    }

    std::vector<std::string> files_to_read;
    // first read the number of files
    int num_files;
    summary_file >> num_files;

    // read the individual file names
    for (int i = 0; i < num_files; i++) {
        std::string filename;
        summary_file >> filename;
        files_to_read.push_back(filename);
    }

    // read the number of genomes
    int num_genomes;
    summary_file >> num_genomes;
    std::vector<SketchInfo> info_of_sketches;
    for (int i = 0; i < num_genomes; i++) {
        SketchInfo info;
        std::string file_path;
        std::string name;
        std::string md5;
        int ksize;
        hash_t max_hash;
        int seed;
        int sketch_size;

        // read one line as file path
        summary_file >> std::ws;
        std::getline(summary_file, file_path);

        // read one line as name
        summary_file >> std::ws;
        std::getline(summary_file, name);

        // read the rest of the info
        summary_file >> md5 >> ksize >> max_hash >> seed >> sketch_size;

        info = SketchInfo(file_path, name, md5, ksize, max_hash, seed, sketch_size);
        info_of_sketches.push_back(info);
    }

    summary_file.close();

    // now load the individual files
    std::vector<std::thread> threads;
    for (int i = 0; i < num_files; i++) {
        threads.push_back(std::thread(&MultiSketchIndex::load_one_chunk, this, files_to_read[i]));
    }

    // wait for all the threads to finish
    for (int i = 0; i < num_files; i++) {
        threads[i].join();
    }

    return info_of_sketches;

}


bool MultiSketchIndex::hash_exists(hash_t hash_value) const {
    int idx_of_hash = index_of_hash(hash_value);
    return multiple_sketch_indices[idx_of_hash].find(hash_value) != multiple_sketch_indices[idx_of_hash].end();
}
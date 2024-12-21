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
                                    bool store_archive,
                                    bool force_write) {
    // check if the directory exists, if not then create it
    struct stat info;
    if (stat(directory_name.c_str(), &info) != 0) {
        std::cout << "Error: Directory does not exist. Creating." << std::endl;
        std::string command = "mkdir -p " + directory_name;
        int ret_code = system(command.c_str());
        if (ret_code != 0) {
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
        std::cout << "Warning: Directory is not empty." << std::endl;
        if (!force_write) {
            std::cout << "Please specify --force-write to overwrite the existing index." << std::endl;
            return false;
        }
        std::cout << "Overwriting the existing index." << std::endl;
    }

    std::vector<std::string> files_written;
    std::vector<std::thread> threads;
    int chunk_size = num_of_indices / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start_index = i * chunk_size;
        int end_index = (i == num_threads - 1) ? num_of_indices : (i + 1) * chunk_size;
        std::string filename_only_no_path = "/chunk_" + std::to_string(i);
        std::string filename_this_thread = directory_name + filename_only_no_path;
        threads.push_back(std::thread(&MultiSketchIndex::write_one_chunk, 
                            this, 
                            filename_this_thread, 
                            start_index, 
                            end_index));
        files_written.push_back(filename_only_no_path);
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

    output_file.close();

    if (store_archive) {
        std::cout << "Storing archive..." << std::endl;
        std::string archive_name = directory_name + ".tar.gz";
        std::string command = "tar -czf " + archive_name + " -C " + directory_name + " .";
        std::cout << command << std::endl;
        if (system(command.c_str()) != 0) {
            std::cout << "Error storing archive." << std::endl;
            return false;
        }
        std::cout << "Archive stored to " << archive_name << std::endl;
    }

    return true;
}




void MultiSketchIndex::load_one_chunk(std::vector<std::string> chunk_filenames,
                                        int start_index, int end_index) {
    // Load the chunk files from start_index to end_index-1
    for (int i = start_index; i < end_index; i++) {
        std::ifstream file(chunk_filenames[i], std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file " << chunk_filenames[i] << std::endl;
            exit(1);
        }
        while (file.peek() != EOF) {
            hash_t hash_value;
            file.read(reinterpret_cast<char*>(&hash_value), sizeof(hash_t));
            int num_sketch_indices;
            file.read(reinterpret_cast<char*>(&num_sketch_indices), sizeof(int));
            std::vector<int> sketch_indices;
            for (int j = 0; j < num_sketch_indices; j++) {
                int sketch_index;
                file.read(reinterpret_cast<char*>(&sketch_index), sizeof(int));
                sketch_indices.push_back(sketch_index);
            }
            add_hash(hash_value, sketch_indices);
        }
    }
}




std::vector<SketchInfo> MultiSketchIndex::load_from_file(std::string index_name,
                                                            int num_threads) {

    // check if the index_name is a tar.gz archive, if yes, exyract it
    std::string directory_name = extract_if_tar_gz(index_name);

    // now read the index summary file and get all the info
    auto all_info = get_sketch_info_from_file(directory_name);
    std::vector<SketchInfo> info_of_sketches = std::get<0>(all_info);
    std::vector<std::string> files_to_read = std::get<1>(all_info);
    int num_references = info_of_sketches.size();
    int num_chunk_files = files_to_read.size();

    // now load the individual files
    num_threads = std::min(num_threads, num_chunk_files);
    std::vector<std::thread> threads;
    int num_chunks_this_thread = num_chunk_files / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start_index = i * num_chunks_this_thread;
        int end_index = (i == num_threads - 1) ? num_chunk_files : (i + 1) * num_chunks_this_thread;
        threads.push_back(std::thread(&MultiSketchIndex::load_one_chunk, 
                            this, 
                            files_to_read, 
                            start_index, 
                            end_index));
    }

    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    return info_of_sketches;

}


bool MultiSketchIndex::hash_exists(hash_t hash_value) const {
    int idx_of_hash = index_of_hash(hash_value);
    return multiple_sketch_indices[idx_of_hash].find(hash_value) != multiple_sketch_indices[idx_of_hash].end();
}



std::tuple< std::vector<SketchInfo>,
            std::vector<std::string>
                >         
            get_sketch_info_from_file(std::string index_directory_name) {
    // Load an index from a file
    std::string summary_filename = index_directory_name + "/summary";
    std::ifstream summary_file(summary_filename);   

    if (!summary_file.is_open()) {
        std::cout << "Error: Could not open summary file." << std::endl;
        std::cout << "Please check if the index is present in the directory." << std::endl;
        exit(1);
    }
    
    // Get the sketch info from a file
    std::vector<std::string> files_to_read;
    // first read the number of files
    int num_files;
    summary_file >> num_files;

    // read the individual file names
    for (int i = 0; i < num_files; i++) {
        std::string filename;
        summary_file >> filename;
        std::string filename_with_path = index_directory_name + '/' + filename;
        files_to_read.push_back(filename_with_path);
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

    return std::make_tuple( info_of_sketches, 
                            files_to_read);
}





std::string extract_if_tar_gz(std::string index_name) {
    // check if index_name is a tar.gz archive
    bool tar_gz = false;
    if (index_name.size() >= 7) {
        if (index_name.substr(index_name.size() - 7) == ".tar.gz") {
            tar_gz = true;
        }
    }

    // if not a tar.gz archive, return the index name as is
    if (!tar_gz) {
        return index_name;
    }

    // if a tar.gz archive, extract the desired directory name
    std::string directory_name = index_name.substr(0, index_name.size() - 7);
    std::cout << "Need to extract the tar.gz archive to " << directory_name << std::endl;
    
    // check if the directory exists and non-empty. if so, do not extract
    struct stat info;
    bool directory_exists = stat(directory_name.c_str(), &info) == 0;
    bool non_empty = false;
    if (directory_exists) {
        DIR* dir = opendir(directory_name.c_str());
        struct dirent* ent;
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                non_empty = true;
                break;
            }
        }
    }

    bool do_not_extract = directory_exists && non_empty;
    if (do_not_extract) {
        std::cout << "Warning: Unarchived directory " << directory_name << " already exists and is not empty." << std::endl;
        std::cout << "Warning: Assuming this directory as is, is indeed the index." << std::endl;
        return directory_name;
    }

    // create the directory
    std::cout << "Creating directory " << directory_name << std::endl;
    std::string command = "mkdir -p " + directory_name;
    std::cout << command << std::endl;
    int ret_code = system(command.c_str());
    if (ret_code != 0) {
        std::cout << "Error: Could not create directory." << std::endl;
        exit(1);
    }

    // extract the tar.gz archive
    std::cout << "Extracting the tar.gz archive to " << directory_name << std::endl;
    command = "tar -xzf " + index_name + " -C " + directory_name;
    std::cout << command << std::endl;
    ret_code = system(command.c_str());
    if (ret_code != 0) {
        std::cout << "Error: Could not extract the tar.gz archive." << std::endl;
        exit(1);
    }
    std::cout << "Extracted the tar.gz archive to " << directory_name << " successfully" << std::endl;
    return directory_name;     

}
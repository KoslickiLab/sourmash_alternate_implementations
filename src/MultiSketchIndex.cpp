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






const std::vector<int>& MultiSketchIndex::get_sketch_indices(hash_t hash_value) {
    // Get the sketch indices for the hash value
    int idx_of_hash = index_of_hash(hash_value);
    if (hash_exists(hash_value)) {
        return multiple_sketch_indices[idx_of_hash][hash_value];
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



bool MultiSketchIndex::write_to_file(std::string filename) {
    // Write the index to a file
    std::ofstream output_file(filename, std::ios::binary);
    if (!output_file.is_open()) {
        return false;
    }
    for (int i = 0; i < num_of_indices; i++) {
        for (auto const& [hash_value, sketch_indices] : multiple_sketch_indices[i]) {
            // write hash_value
            output_file.write(reinterpret_cast<const char*>(&hash_value), sizeof(hash_t));
            // write number of sketch indices
            size_t num_sketch_indices = sketch_indices.size();
            output_file.write(reinterpret_cast<const char*>(&num_sketch_indices), sizeof(size_t));
            // write sketch indices
            for (size_t j = 0; j < num_sketch_indices; j++) {
                output_file.write(reinterpret_cast<const char*>(&sketch_indices[j]), sizeof(int));
            }
        }
    }
    output_file.close();
    return true;
}


MultiSketchIndex::MultiSketchIndex(std::string filename) {
    // Constructor
    this->num_of_indices = 4096;
    multiple_sketch_indices = std::vector<std::unordered_map<hash_t, std::vector<int>>>(num_of_indices);
    mutexes = std::vector<std::mutex>(num_of_indices);
    std::ifstream input_file(filename, std::ios::binary);
    if (!input_file.is_open()) {
        return;
    }
    while (input_file.peek() != EOF) {
        hash_t hash_value;
        input_file.read(reinterpret_cast<char*>(&hash_value), sizeof(hash_t));
        size_t num_sketch_indices;
        input_file.read(reinterpret_cast<char*>(&num_sketch_indices), sizeof(size_t));
        std::vector<int> sketch_indices(num_sketch_indices);
        for (size_t i = 0; i < num_sketch_indices; i++) {
            input_file.read(reinterpret_cast<char*>(&sketch_indices[i]), sizeof(int));
        }
        add_hash(hash_value, sketch_indices);
    }
    input_file.close();
}
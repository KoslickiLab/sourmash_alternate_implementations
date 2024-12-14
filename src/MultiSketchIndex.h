#ifndef SKETCHINDEX_H
#define SKETCHINDEX_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>

#include <fstream>

#include "SketchInfo.h"


#ifndef HASH_T
#define HASH_T
typedef unsigned long long int hash_t;
#endif


/**
 * @brief MultiSketchIndex class, which is used to store the index of many sketches.
 * 
 */
class MultiSketchIndex {
    public:
        MultiSketchIndex(int num_of_indices);
        MultiSketchIndex();
        ~MultiSketchIndex();

        /**
         * @brief Add a hash value to the index.
         * 
         * @param hash_value The hash value to add.
         * @param sketch_index The index of the sketch in which this hash value appears.
         */
        void add_hash(hash_t hash_value, int sketch_index);
        


        /**
         * @brief Add a hash value to the index.
         * 
         * @param hash_value The hash value to add.
         * @param sketch_indices Indices of the sketches in which this hash value appears.
         */
        void add_hash(hash_t hash_value, std::vector<int> sketch_indices);


        /**
         * @brief Remove a hash value from the index.
         * 
         * @param hash_value The hash value to remove.
         * @param sketch_index The index of the sketch in which this hash value appears.
         */
        void remove_hash(hash_t hash_value, int sketch_index);


        /**
         * @brief Remove a hash value from the index.
         * 
         * @param hash_value The hash value to remove.
         * @param sketch_index The index of the sketch in which this hash value appears.
         */
        std::vector<int> remove_hash(hash_t hash_value);



        /**
         * @brief Get the sketch indices for a hash value.
         * 
         * @param hash_value The hash value to get the sketch indices for.
         * @return const std::vector<int>& The sketch indices in which the hash value appears.
         */
        const std::vector<int>& get_sketch_indices(hash_t hash_value) const;


        /**
         * @brief Check if a hash value exists in the index.
         * 
         * @param hash_value The hash value to check.
         * @return true If the hash value exists in the index.
         * @return false If the hash value does not exist in the index.
         */
        bool hash_exists(hash_t hash_value) const;


        /**
         * @brief Get the size of the index.
         * 
         * @return size_t The size of the index.
         */
        const size_t size() {
            size_t total_size = 0;
            for (int i = 0; i < num_of_indices; i++) {
                total_size += multiple_sketch_indices[i].size();
            }
            return total_size;
        }

        /**
         * @brief write index to file. Assumptions: the info are provided
         * correctly
         * 
         * @param directory_name where the index will be written
         * @param num_threads number of threads to use
         * @param info_of_sketches info of all the sketches
         * @return true if the index is written successfully
         * @return false if the index is not written successfully
         */
        bool write_to_file(std::string directory_name, 
                                    int num_threads, 
                                    std::vector<SketchInfo> info_of_sketches,
                                    bool force_write);

        /**
         * @brief load an index from a file.
         * 
         * @param directory_name the directory where the index is stored.
         * @return std::pair<std::vector<std::string>, std::vector<size_t>> genome names vector and sketch sizes vector
         */
        std::vector<SketchInfo> load_from_file(std::string directory_name);



        void show_index_stats() {
            std::cout << "Number of indices: " << num_of_indices << std::endl;
            std::cout << "Index size: " << size() << std::endl;
            //show size of the first 10 indices
            for (int i = 0; i < 10; i++) {
                std::cout << "Index " << i << " size: " << multiple_sketch_indices[i].size() << std::endl;
            }
        }


        bool operator==(const MultiSketchIndex &other) {
            // iterate over all the hash values
            for (int i = 0; i < num_of_indices; i++) {
                for (auto const& [hash_value, sketch_indices] : multiple_sketch_indices[i]) {
                    if (!other.hash_exists(hash_value)) {
                        return false;
                    }

                    const std::vector<int> &other_sketch_indices = other.get_sketch_indices(hash_value);
                    if (sketch_indices.size() != other_sketch_indices.size()) {
                        return false;
                    }

                    // create a sorted version of the sketch indices
                    std::vector<int> sketch_indices_ = sketch_indices;
                    std::sort(sketch_indices_.begin(), sketch_indices_.end());
                    std::vector<int> other_sketch_indices_ = other_sketch_indices;
                    std::sort(other_sketch_indices_.begin(), other_sketch_indices_.end());

                    // check if set of sketch indices are the same
                    for (int j = 0; j < sketch_indices_.size(); j++) {
                        if (sketch_indices_[j] != other_sketch_indices_[j]) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }


        std::vector<hash_t> get_all_hashes() {
            std::vector<hash_t> all_hashes;
            for (int i = 0; i < num_of_indices; i++) {
                for (auto const& [hash_value, sketch_indices] : multiple_sketch_indices[i]) {
                    all_hashes.push_back(hash_value);
                }
            }
            return all_hashes;
        }

        
    private:
        std::vector<std::unordered_map<hash_t, std::vector<int>>> multiple_sketch_indices;
        std::vector<std::mutex>mutexes;
        int num_of_indices;

        /**
         * @brief given a hash value, get the index of the hash_table where this hash value should be stored.
         * 
         * @param hash_value 
         * @return int 
         */
        int index_of_hash(hash_t hash_value) const {
            return hash_value % num_of_indices;
        }

        const std::vector<int> empty_vector;

        /**
         * @brief helper function to write one chunk of the index to a file.
         * 
         * @param file 
         * @param start_index 
         * @param end_index 
         * @return true 
         * @return false 
         */
        void write_one_chunk(std::string filename, int start_index, int end_index);


        /**
         * @brief load the contents of this file into the hash index
         * 
         * @param file 
         */
        void load_one_chunk(std::string filename);
        
};

#endif
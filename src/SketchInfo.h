#ifndef HASH_T
#define HASH_T
typedef unsigned long long int hash_t;
#endif

#ifndef SKETCHINFO_H
#define SKETCHINFO_H

#include <string>
#include <iostream>


/**
 * @brief Info of a sketch, does not contain the actual min-hashes
 * 
 */
class SketchInfo {
    
public:

    std::string file_path;
    std::string name;
    std::string md5;
    int ksize;
    hash_t max_hash;
    int seed;

    SketchInfo(std::string file_path, std::string name, std::string md5, int ksize, hash_t max_hash, int seed) {
        this->file_path = file_path;
        this->name = name;
        this->md5 = md5;
        this->ksize = ksize;
        this->max_hash = max_hash;
        this->seed = seed;
    }

    SketchInfo() {
        this->file_path = "";
        this->name = "";
        this->md5 = "";
        this->ksize = 0;
        this->max_hash = 0;
        this->seed = 0;
    }

    // copy constructor
    SketchInfo(const SketchInfo& info) {
        this->file_path = info.file_path;
        this->name = info.name;
        this->md5 = info.md5;
        this->ksize = info.ksize;
        this->max_hash = info.max_hash;
        this->seed = info.seed;
    }

    // destructor
    ~SketchInfo() {}

    void show() {
        std::cout << "file_path: " << this->file_path << std::endl;
        std::cout << "name: " << this->name << std::endl;
        std::cout << "md5: " << this->md5 << std::endl;
        std::cout << "ksize: " << this->ksize << std::endl;
        std::cout << "max_hash: " << this->max_hash << std::endl;
        std::cout << "seed: " << this->seed << std::endl;
    }

    bool operator==(const SketchInfo &other) {
        return this->file_path == other.file_path &&
               this->name == other.name &&
               this->md5 == other.md5 &&
               this->ksize == other.ksize &&
               this->max_hash == other.max_hash &&
               this->seed == other.seed;
    }

    bool operator!=(const SketchInfo &other) {
        return !(*this == other);
    }

    /**
     * @brief Get the str representation object
     * 
     * @return std::string 
     */
    std::string get_str_representation() {
        return this->file_path + "\n" + this->name + "\n" + this->md5 + "\n" + std::to_string(this->ksize) + "\n" + std::to_string(this->max_hash) + "\n" + std::to_string(this->seed);
    }

};

#endif
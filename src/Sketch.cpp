# include "Sketch.h"

void Sketch::show() {
    
    this->info.show();
    std::cout << "Hashes: ";
    for (hash_t hash : this->hashes) {
        std::cout << hash << " ";
    }
    std::cout << std::endl;

}

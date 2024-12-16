#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include "argparse.hpp"
#include "json.hpp"
#include "utils.h"
#include "MultiSketchIndex.h"

using namespace std;
using json = nlohmann::json;

struct Arguments {
    string filelist_sketches;
    string index_directory_name;
    int number_of_threads;
    int num_hashtables;
    bool force_write;
};


typedef Arguments Arguments;



void parse_args(int argc, char** argv, Arguments &arguments) {

    argparse::ArgumentParser parser("index");

    parser.add_argument("filelist_sketches")
        .help("The path to the file containing the paths to the sketches")
        .required()
        .store_into(arguments.filelist_sketches);

    parser.add_argument("index_directory_name")
        .help("The directory where the index will be stored)")
        .required()
        .store_into(arguments.index_directory_name);

    parser.add_argument("-t", "--threads")
        .help("The number of threads to use")
        .scan<'i', int>()
        .default_value(1)
        .store_into(arguments.number_of_threads);
    
    parser.add_argument("-n", "--num-hashtables")
        .help("The number of hash tables to use")
        .scan<'i', int>()
        .default_value(4096)
        .store_into(arguments.num_hashtables);

    parser.add_argument("-f", "--force")
        .help("Force write the index to file")
        .default_value(false)
        .implicit_value(true)
        .store_into(arguments.force_write);

    try {
        parser.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        std::cout << parser;
        exit(1);
    }

}



void show_arguments(Arguments &arguments) {
    cout << "*********************************" << endl;
    cout << "* " << endl;
    cout << "*  filelist_sketches: " << arguments.filelist_sketches << endl;
    cout << "*  index_directory_name: " << arguments.index_directory_name << endl;
    cout << "*  number_of_threads: " << arguments.number_of_threads << endl;
    cout << "*  num_hashtables: " << arguments.num_hashtables << endl;
    cout << "*  force_write: " << arguments.force_write << endl;
    cout << "* " << endl;
    cout << "*********************************" << endl;
}



int main(int argc, char** argv) {

    Arguments arguments;
    parse_args(argc, argv, arguments);
    show_arguments(arguments);

    cout << "Reading sketch list..." << endl;
    vector<string> sketch_paths;
    bool success = get_sketch_paths(arguments.filelist_sketches, sketch_paths);
    if (!success) {
        cout << "Error reading the sketch paths." << endl;
        exit(1);
    }
    cout << "There are " << sketch_paths.size() << " sketches to read." << endl;
    cout << "Reading using " << arguments.number_of_threads << " threads..." << endl;

    vector<Sketch> sketches;
    vector<int> empty_sketch_ids;
    read_sketches(sketch_paths, sketches, empty_sketch_ids, arguments.number_of_threads);
    cout << "Reading complete." << endl;
    cout << "There are " << empty_sketch_ids.size() << " empty sketches. Now building index.." << endl;

    MultiSketchIndex multi_sketch_index(arguments.num_hashtables);
    compute_index_from_sketches(sketches, multi_sketch_index, arguments.number_of_threads);
    cout << "Index built." << endl;
    cout << "Some index stats:" << endl;
    multi_sketch_index.show_index_stats();


    cout << "Writing index to file..." << endl;
    vector<SketchInfo> info_of_sketches;
    for (auto &sketch : sketches) {
        info_of_sketches.push_back(sketch.info);
    }

    bool success = multi_sketch_index.write_to_file(arguments.index_directory_name, 
                                            arguments.number_of_threads, 
                                            info_of_sketches,
                                            arguments.force_write);
    if (!success) {
        cout << "Error writing index to file." << endl;
        exit(1);
    }
    cout << "Index written to file." << endl;

    // make the program exit faster using exit(0)
    exit(0);

    /*
    
    // following code is for testing the load_from_file function
    cout << "Loading index from file..." << endl;
    MultiSketchIndex loaded_index(arguments.num_hashtables);
    auto loaded_sketch_info = loaded_index.load_from_file(arguments.index_directory_name);

    int num_sketches = sketches.size();
    int num_loaded_sketches = loaded_sketch_info.size();

    // check if the number of sketches is the same
    if (num_sketches != num_loaded_sketches) {
        cout << "Error: The number of sketches is not the same." << endl;
        cout << "Original: " << num_sketches << endl;
        cout << "Loaded: " << num_loaded_sketches << endl;
        exit(1);
    }

    // now assert that the sketch info are the same
    for (int i = 0; i < num_sketches; i++) {
        if (sketches[i].info != loaded_sketch_info[i]) {
            cout << "Error: The sketch info is not the same." << endl;
            // show the sketch info
            cout << "Original:" << endl;
            sketches[i].info.show();
            cout << "Loaded:" << endl;
            loaded_sketch_info[i].show();
            exit(1);
        }
    }

    // finally, check if the index is the same
    if (multi_sketch_index == loaded_index) {
        cout << "Index loaded successfully." << endl;
    } else {
        cout << "Error: The loaded index is not the same as the original one." << endl;
        exit(1);
    }

    cout << "All tests passed." << endl;
    
    exit(0);

    */

}
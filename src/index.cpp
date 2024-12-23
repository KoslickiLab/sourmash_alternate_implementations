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
    bool store_archive;
    string archive_name;
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
        .help("The directory where the index will be stored (must be a directory)")
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

    parser.add_argument("-s", "--store-archive")
        .help("Store a tar.gz archive of the index")
        .default_value(false)
        .implicit_value(true)
        .store_into(arguments.store_archive);

    parser.add_argument("-a", "--archive-name")
        .help("The name of the archive (will be discarded if store-archive is false)")
        .default_value("index.tar.gz")
        .store_into(arguments.archive_name);

    parser.add_argument("-f", "--force-write")
        .help("Force write the index to the directory")
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
    cout << "*  store_archive: " << arguments.store_archive << endl;
    cout << "*  archive_name: " << (arguments.store_archive ? arguments.archive_name : "N/A") << endl;
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
    bool success_read_filelist = get_sketch_paths(arguments.filelist_sketches, sketch_paths);
    if (!success_read_filelist) {
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
    //cout << "Some index stats:" << endl;
    //multi_sketch_index.show_index_stats();


    cout << "Writing index to file..." << endl;
    vector<SketchInfo> info_of_sketches;
    for (auto &sketch : sketches) {
        info_of_sketches.push_back(sketch.info);
    }

    bool success = multi_sketch_index.write_to_file(arguments.index_directory_name, 
                                            arguments.number_of_threads, 
                                            info_of_sketches,
                                            arguments.store_archive,
                                            arguments.archive_name,
                                            arguments.force_write);
    if (!success) {
        cout << "Error writing index to file." << endl;
        exit(1);
    }
    cout << "Index written successfully." << endl;

    exit(0);

}
#include <iostream>
#include <vector>

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

    parser.parse_args(argc, argv);

}



void show_arguments(Arguments &arguments) {
    cout << "*********************************" << endl;
    cout << "*" << endl;
    cout << "* filelist_sketches: " << arguments.filelist_sketches << endl;
    cout << "* index_directory_name: " << arguments.index_directory_name << endl;
    cout << "* number_of_threads: " << arguments.number_of_threads << endl;
    cout << "* num_hashtables: " << arguments.num_hashtables << endl;
    cout << "*" << endl;
    cout << "*********************************" << endl;
}



int main(int argc, char** argv) {

    Arguments arguments;
    parse_args(argc, argv, arguments);
    show_arguments(arguments);

    return 0;

}
#include "argparse.hpp"
#include "json.hpp"
#include "utils.h"
#include "MultiSketchIndex.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <thread>
#include <mutex>
#include <cmath>
#include <limits>
#include <iomanip>
#include <sstream>
#include <random>
#include <exception>

#include <sys/resource.h>
#include <cstdlib>

using namespace std;
using json = nlohmann::json;


struct Arguments {
    string query_path;
    string ref_index_name;
    string output_filename;
    int threshold_bp;
};


typedef Arguments Arguments;
typedef unsigned long long int hash_t;



void do_prefetch(Arguments& args) {
    // data structures
    Sketch query_sketch; 
    string ref_index_name;

    // Read the query sketch and the reference sketches
    cout << "Reading query sketch.." << endl;
    query_sketch = read_min_hashes(args.query_path);

    // build a map of the hash_values in the query sketch for fast presence/absence lookup
    unordered_set<hash_t> query_sketch_set;
    for (hash_t hash_value : query_sketch.hashes) {
        query_sketch_set.insert(hash_value);
    }

    cout << "Now reading reference index and searching matches against the query.." << endl;
    string index_directory_name = extract_if_tar_gz(args.ref_index_name);
    auto all_info = get_sketch_info_from_file(index_directory_name);
    vector<SketchInfo> info_of_references = get<0>(all_info);
    vector<string> chunk_filenames = get<1>(all_info);
    int num_references = info_of_references.size();
    
    size_t* num_intersection_values = new size_t[num_references];
    for (size_t i = 0; i < num_references; i++) {
        num_intersection_values[i] = 0;
    }

    for (string chunk_file_name : chunk_filenames) {
        std::ifstream file(chunk_file_name, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file " << chunk_file_name << std::endl;
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
            // if the hash value is in the query sketch, increment the number of intersections
            if (query_sketch_set.find(hash_value) != query_sketch_set.end()) {
                for (int sketch_index : sketch_indices) {
                    num_intersection_values[sketch_index]++;
                }
            }
        }
    }
    
    // now sort the ref sketches based on the number of intersections
    vector<tuple<int, size_t>> ref_id_num_intersections;
    for (int i = 0; i < num_references; i++) {
        ref_id_num_intersections.push_back(make_tuple(i, num_intersection_values[i]));
    }

    sort(ref_id_num_intersections.begin(), ref_id_num_intersections.end(), 
        [](const tuple<int, size_t>& a, const tuple<int, size_t>& b) {
            return get<1>(a) > get<1>(b);
        });

    // write the results to the output file
    ofstream outfile(args.output_filename);
    outfile << "match_id,match_name,match_md5,num_intersections,containment_query_ref,containment_ref_query,max_containment,jaccard" << endl;
    for (auto ref_id_num_intersection : ref_id_num_intersections) {
        
        int ref_id = get<0>(ref_id_num_intersection);
        size_t num_intersection = get<1>(ref_id_num_intersection);
        
        if (num_intersection < args.threshold_bp || num_intersection == 0) {
            break;
        }
        if (query_sketch.size() == 0 || info_of_references[ref_id].sketch_size == 0) {
            continue;
        }
        
        string match_name = info_of_references[ref_id].name;
        string match_md5 = info_of_references[ref_id].md5;
        double containment_query_ref = 1.0 * num_intersection / query_sketch.size();
        double containment_ref_query = 1.0 * num_intersection / info_of_references[ref_id].sketch_size;
        double max_containment = max(containment_query_ref, containment_ref_query);
        double jaccard = 1.0 * num_intersection / (query_sketch.size() + info_of_references[ref_id].sketch_size - num_intersection);
        
        outfile << ref_id << "," << match_name << "," << match_md5 << "," << num_intersection << "," << containment_query_ref << "," << containment_ref_query << "," << max_containment << "," << jaccard << endl;
    }

    outfile.close();
    cout << "Results written to " << args.output_filename << endl;
    cout << "Cleaning up and exiting... (may take some time)" << endl;
    
}



void parse_args(int argc, char** argv, Arguments &arguments) {
    argparse::ArgumentParser parser("prefetch: find all matching refs for a query sketch");

    parser.add_argument("query_path")
        .help("The path to the query sketch")
        .required()
        .store_into(arguments.query_path);

    parser.add_argument("ref_index_name")
        .help("The name of the index of the target references")
        .required()
        .store_into(arguments.ref_index_name);

    parser.add_argument("output_filename")
        .help("The path to the output file")
        .required()
        .store_into(arguments.output_filename);
    
    parser.add_argument("-b", "--threshold-bp")
        .help("The threshold in base pairs")
        .scan<'i', int>()
        .default_value(50)
        .store_into(arguments.threshold_bp);

    try {
        parser.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cout << err.what() << std::endl;
        std::cout << parser;
        exit(1);
    }
}



void show_args(Arguments &args) {
    cout << "**************************************" << endl;
    cout << "*" << endl;
    cout << "*   Query path: " << args.query_path << endl;
    cout << "*   Reference index name: " << args.ref_index_name << endl;
    cout << "*   Output filename: " << args.output_filename << endl;
    cout << "*   Threshold in base pairs: " << args.threshold_bp << endl;
    cout << "*" << endl;
    cout << "**************************************" << endl;
} 







int main(int argc, char** argv) {

    Arguments arguments;
    parse_args(argc, argv, arguments);
    show_args(arguments);
    do_prefetch(arguments);    

    exit(0);

}
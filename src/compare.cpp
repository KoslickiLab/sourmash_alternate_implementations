/*
Load sketches present in a filelist,
compute the index from the sketches,
compute all v all containment values,
write the results to a file.
*/

#include <iostream>
#include <vector>
#include <cstdlib>

#include "argparse.hpp"
#include "json.hpp"
#include "utils.h"
#include "MultiSketchIndex.h"

using namespace std;
using json = nlohmann::json;

struct Arguments {
    string filelist_queries;
    string ref_index_dir;
    string working_dir;
    string output_filename;
    double containment_threshold;
    int number_of_threads;
    int num_hashtables;
    int num_passes;
    int ksize;
};


typedef Arguments Arguments;



void do_compare(Arguments& args) {
    // data structures
    vector<string> query_sketch_paths;
    vector<string> target_sketch_paths;
    vector<Sketch> query_sketches;
    vector<Sketch> target_sketches;
    vector<int> empty_sketch_ids;
    MultiSketchIndex target_sketch_index(args.num_hashtables);

    // Read the sketches
    auto read_start = chrono::high_resolution_clock::now();
    cout << "Reading all sketches using " << args.number_of_threads << " threads" << endl;
    get_sketch_paths(args.filelist_queries, query_sketch_paths);
    cout << "Number of query sketches to read: " << query_sketch_paths.size() << endl;
    read_sketches(query_sketch_paths, 
                    query_sketches, 
                    empty_sketch_ids, 
                    args.number_of_threads);
    auto read_end = chrono::high_resolution_clock::now();
    auto read_duration = chrono::duration_cast<chrono::seconds>(read_end - read_start);
    cout << "Query sketches reading completed in " << read_duration.count() << " seconds." << endl;

    cout << "Reading the target index..." << endl;
    auto target_start = chrono::high_resolution_clock::now();
    vector<SketchInfo> info_of_target_sketches = target_sketch_index.load_from_file(args.ref_index_dir);
    auto target_end = chrono::high_resolution_clock::now();
    auto target_duration = chrono::duration_cast<chrono::seconds>(target_end - target_start);
    cout << "Target index loaded in " << target_duration.count() << " seconds." << endl;


    // build the index for the query sketches
    cout << "Building index for the query sketches..." << endl;
    auto build_start = chrono::high_resolution_clock::now();
    MultiSketchIndex multi_sketch_index_temp(args.num_hashtables);
    compute_index_from_sketches(query_sketches, multi_sketch_index_temp, args.number_of_threads);

    // check that this index is the same as the target index
    if (multi_sketch_index_temp == target_sketch_index) {
        cout << "The index built from the query sketches is the same as the target index." << endl;
    } else {
        cerr << "The index built from the query sketches is not the same as the target index." << endl;
        exit(1);
    }


    // Compute all v all containment values
    cout << "Computing all v all containment values..." << endl;
    vector<vector<int>> similars;
    auto start_compute = chrono::high_resolution_clock::now();
    compute_intersection_matrix(query_sketches, 
                                info_of_target_sketches, 
                                target_sketch_index, 
                                args.working_dir, 
                                similars, 
                                args.containment_threshold, 
                                args.num_passes,
                                args.number_of_threads,
                                args.ksize);
    auto end_compute = chrono::high_resolution_clock::now();
    auto duration_compute = chrono::duration_cast<chrono::seconds>(end_compute - start_compute);
    cout << "Containment values computed in " << duration_compute.count() << " seconds." << endl;

    // Write the results to a file
    cout << "Writing the results to " << args.output_filename << endl;
    // combine the files in out_dir/a_xyz.txt to args.output_filename, where 0 <= a < args.num_passes,
    // and 0 <= xyz < args.number_of_threads
    vector<string> files_to_combine;
    for (int i = 0; i < args.num_passes; i++) {
        for (int j = 0; j < args.number_of_threads; j++) {
            string pass_id_str = to_string(i);
            string thread_id_str = to_string(j);
            while (thread_id_str.size() < 3) {
                thread_id_str = "0" + thread_id_str;
            }
            string filename = args.working_dir + "/" + pass_id_str + "_" + thread_id_str + ".txt";
            files_to_combine.push_back(filename);
        }
    }

    // write the header in the output file
    ofstream output_file(args.output_filename);
    output_file << "query_id, query_name, query_md5, match_id, match_name, match_md5, jaccard, containment_query_in_match, containment_match_in_query, max_containment, max_containment_ani" << endl;
    output_file.close();

    // combining command: cat 
    string combine_command = "cat ";
    for (string filename : files_to_combine) {
        combine_command += filename + " ";
    }
    combine_command += " >> " + args.output_filename;
    // call the system command and check if it is successful
    if (system(combine_command.c_str()) != 0) {
        cerr << "Error in combining the files." << endl;
        exit(1);
    }
    cout << "Results written to " << args.output_filename << endl;

    cout << "Removing the individual files in the working directory..." << endl;
    // remove the individual files in the working directory
    for (string filename : files_to_combine) {
        if (remove(filename.c_str()) != 0) {
            cerr << "Error in removing the file " << filename << endl;
        }
    }
    cout << "Individual files removed." << endl;

    // Clean up
    cout << "Cleaning up and exiting... (may take some time)" << endl;
    exit(0);

}



void parse_args(int argc, char** argv, Arguments &arguments) {

    argparse::ArgumentParser parser("compare");

    parser.add_argument("filelist_queries")
        .help("The path to the file containing the paths to the query sketches")
        .required()
        .store_into(arguments.filelist_queries);

    parser.add_argument("filelist_targets")
        .help("The directory where the index is already stored)")
        .required()
        .store_into(arguments.ref_index_dir);

    parser.add_argument("working_dir")
        .help("The directory where smaller files will be stored")
        .required()
        .store_into(arguments.working_dir);

    parser.add_argument("output_filename")
        .help("The path to the output file")
        .required()
        .store_into(arguments.output_filename);

    parser.add_argument("-c", "--containment-threshold")
        .help("The containment threshold above which outputs are written")
        .scan<'f', double>()
        .default_value(0.5)
        .store_into(arguments.containment_threshold);

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

    parser.add_argument("-p", "--num-passes")
        .help("The number of passes to use")
        .scan<'i', int>()
        .default_value(1)
        .store_into(arguments.num_passes);

    parser.add_argument("-k", "--ksize")
        .help("The k-mer size")
        .scan<'i', int>()
        .default_value(31)
        .store_into(arguments.ksize);
    
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
    cout << "*   Query filelist: " << args.filelist_queries << endl;
    cout << "*   Targets index directory: " << args.ref_index_dir << endl;
    cout << "*   Working directory: " << args.working_dir << endl;
    cout << "*   Output filename: " << args.output_filename << endl;
    cout << "*   Containment threshold: " << args.containment_threshold << endl;
    cout << "*   Number of threads: " << args.number_of_threads << endl;
    cout << "*   Number of hash tables: " << args.num_hashtables << endl;
    cout << "*   Number of passes: " << args.num_passes << endl;
    cout << "*   K-mer size: " << args.ksize << endl;
    cout << "*" << endl;
    cout << "**************************************" << endl;
}



int main( int argc, char** argv ) {

    Arguments arguments;
    parse_args(argc, argv, arguments);
    show_args(arguments);
    do_compare(arguments);

    return 0;

}
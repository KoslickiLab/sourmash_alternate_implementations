from utils import run_command_and_record_time_memory
import os

if __name__ == '__main__':
    data_dir = "/scratch/mbr5797/fast_index_compare_data"
    index_names = ["index_10k", "index_20k", "index_30k", "index_40k", "index_50k"]
    benchmark_filename = "benchmark/benchmark_results_compare_one_v_many.csv"
    filesizes = [10000, 20000, 30000, 40000, 50000]
    working_dir = "/scratch/mbr5797/fast_index_compare_data/temp_working_dir"
    query_name = "/scratch/mbr5797/fast_index_compare_data/fecal_ref.sig"
    query_filelist_name = "filelist_1"
    
    query_size_ref_size_to_metrics = {}
    
    for ref_index_name, ref_filesize in list(zip(index_names, filesizes)):
        print(f"Processing {query_name} against {ref_index_name}")
        
        ref_index_path = os.path.join(data_dir, ref_index_name)
        output_filename = f"compare_1_v_{ref_filesize}_results.csv"
        output_filename = os.path.join(data_dir, output_filename)
        query_filelist_path = os.path.join(data_dir, query_filelist_name)
        
        command = f"bin/compare {query_filelist_path} {ref_index_path} {working_dir} {output_filename} -t 1 -c 0.00 -k 51"
        print(command)
        
        metrics = run_command_and_record_time_memory(command)
        if metrics:
            query_size_ref_size_to_metrics[ref_filesize] = metrics
        else:
            print(f"Error processing {query_filelist_path} and {ref_index_name}")
        
    # write the results to output file
    with open(benchmark_filename, 'w') as f:
        print('Writing to: ' + benchmark_filename)
        f.write("ref_filesize,cpu_time,wall_clock_time,peak_memory_usage\n")
        for ref_filesize, metrics in query_size_ref_size_to_metrics.items():
            f.write(f"{ref_filesize},{metrics['cpu_time']},{metrics['wall_clock_time']},{metrics['peak_memory_usage']}\n")

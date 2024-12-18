from utils import run_command_and_record_time_memory
import os

if __name__ == '__main__':
    data_dir = "/scratch/mbr5797/fast_index_compare_data"
    filelists = ["filelist_10k", "filelist_20k", "filelist_30k", "filelist_40k", "filelist_50k"]
    index_names = ["index_10k", "index_20k", "index_30k", "index_40k", "index_50k"]
    benchmark_filename = "benchmark/benchmark_results_compare_many_v_many.csv"
    filesizes = [10000, 20000, 30000, 40000, 50000]
    working_dir = "/scratch/mbr5797/fast_index_compare_data/temp_working_dir"
    
    query_size_ref_size_to_metrics = {}
    
    for query_filelist, query_filesize in list(zip(filelists, filesizes)):
        for ref_index_name, ref_filesize in list(zip(index_names, filesizes)):
            print(f"Processing {query_filelist} and {ref_index_name}")
            
            query_filelist_path = os.path.join(data_dir, query_filelist)
            ref_index_path = os.path.join(data_dir, ref_index_name)
            output_filename = f"compare_{query_filesize}_v_{ref_filesize}_results.csv"
            output_filename = os.path.join(data_dir, output_filename)
            
            # command = bin/compare filelist_queries ref_index working_dir output_filename -t 128 -c 0.0 -k 51
            command = f"bin/compare {query_filelist_path} {ref_index_path} {working_dir} {output_filename} -t 128 -c 0.01 -k 51"
            print(command)
            
            metrics = run_command_and_record_time_memory(command)
            if metrics:
                query_size_ref_size_to_metrics[(query_filesize, ref_filesize)] = metrics
            else:
                print(f"Error processing {query_filelist} and {ref_index_name}")
        
    # write the results to output file
    with open(benchmark_filename, 'w') as f:
        print('Writing to: ' + benchmark_filename)
        f.write("query_filesize,ref_filesize,cpu_time,wall_clock_time,peak_memory_usage\n")
        for (query_filesize, ref_filesize), metrics in query_size_ref_size_to_metrics.items():
            f.write(f"{query_filesize},{ref_filesize},{metrics['cpu_time']},{metrics['wall_clock_time']},{metrics['peak_memory_usage']}\n")

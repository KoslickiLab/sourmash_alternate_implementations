from utils import run_command_and_record_time_memory
import os

if __name__ == '__main__':
    data_dir = "/scratch/mbr5797/fast_index_compare_data"
    ref_filelists = ["filelist_10k", "filelist_20k", "filelist_30k", "filelist_40k", "filelist_50k"]
    benchmark_filename = "benchmark/benchmark_results_multisearch_one_v_many.csv"
    ref_filesizes = [10000, 20000, 30000, 40000, 50000]
    query_filelist = "filelist_1"
    
    query_size_ref_size_to_metrics = {}
    
    for ref_filelist, ref_filesize in list(zip(ref_filelists, ref_filesizes)):
        print(f"Processing {query_filelist} vs {ref_filelist}")
        
        query_filelist_path = os.path.join(data_dir, query_filelist)
        ref_filelist_path = os.path.join(data_dir, ref_filelist)
        output_filename = f"multisearch_1_v_{ref_filesize}_results.csv"
        output_filename = os.path.join(data_dir, output_filename)
        
        command = f"sourmash scripts multisearch -o {output_filename} -c 1 {query_filelist_path} {ref_filelist_path} -k 51 -t 0.0"
        print(command)
        
        metrics = run_command_and_record_time_memory(command)
        if metrics:
            query_size_ref_size_to_metrics[ref_filesize] = metrics
        else:
            print(f"Error processing one vs {ref_filesize}")
        
    # write the results to output file
    with open(benchmark_filename, 'w') as f:
        print('Writing to: ' + benchmark_filename)
        f.write("ref_filesize,cpu_time,wall_clock_time,peak_memory_usage\n")
        for ref_filesize, metrics in query_size_ref_size_to_metrics.items():
            f.write(f"{ref_filesize},{metrics['cpu_time']},{metrics['wall_clock_time']},{metrics['peak_memory_usage']}\n")

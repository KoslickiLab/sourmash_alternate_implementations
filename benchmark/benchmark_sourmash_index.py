import os
from utils import run_command_and_record_time_memory

if __name__ == "__main__":
    data_dir = "/scratch/mbr5797/fast_index_compare_data"
    filelists = ["filelist_10k", "filelist_20k", "filelist_30k", "filelist_40k", "filelist_50k"]
    index_names = ["sourmash_sbt_10k", "sourmash_sbt_20k", "sourmash_sbt_30k", "sourmash_sbt_40k", "sourmash_sbt_50k"]
    output_filename = "benchmark/benchmark_results_sourmash_index.csv"
    filesizes = [10000, 20000, 30000, 40000, 50000]
        
    filesize_to_metrics = {}
        
    for filelist, index_name, filesize in zip(filelists, index_names, filesizes):
        print(f"Processing {filelist}")
        filelist_path = os.path.join(data_dir, filelist)
        index_path = os.path.join(data_dir, index_name)
        
        # command = sourmash index /scratch/mbr5797/fast_index_compare_data/sourmash_sbt_10k --from-file /scratch/mbr5797/fast_index_compare_data/filelist_10k
        command = f"sourmash index {index_path} --from-file {filelist_path}"
        print(command)
        
        metrics = run_command_and_record_time_memory(command)
        if metrics:
            filesize_to_metrics[filesize] = metrics
        else:
            print(f"Error processing {filelist}")
            
    print(filesize_to_metrics)
    
    # Write results to CSV, headers should have no spaces or brackets
    with open(output_filename, 'w') as f:
        f.write("filesize,cpu_time,wall_clock_time,peak_memory_usage\n")
        for filesize, metrics in filesize_to_metrics.items():
            f.write(f"{filesize},{metrics['cpu_time']},{metrics['wall_clock_time']},{metrics['peak_memory_usage']}\n")
        
        
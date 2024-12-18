import os
from utils import run_command_and_record_time_memory

if __name__ == "__main__":
    data_dir = "/scratch/mbr5797/fast_index_compare_data"
    filelists = ["filelist_10k", "filelist_20k", "filelist_30k", "filelist_40k", "filelist_50k"]
    index_names = ["index_10k", "index_20k", "index_30k", "index_40k", "index_50k"]
    output_filename = "index_benchmark_results.csv"
    filesizes = [10000, 20000, 30000, 40000, 50000]
        
    filesize_to_metrics = {}
        
    for filelist, index_name, filesize in zip(filelists, index_names, filesizes):
        print(f"Processing {filelist}")
        filelist_path = os.path.join(data_dir, filelist)
        index_path = os.path.join(data_dir, index_name)
        
        # command = bin/index filelist index_10k -t 128 -f
        command = f"bin/index {filelist_path} {index_path} -t 128 -f"
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
        
        
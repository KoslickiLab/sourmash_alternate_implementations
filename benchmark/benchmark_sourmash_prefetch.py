import os
from utils import run_command_and_record_time_memory

if __name__ == "__main__":
    data_dir = "/scratch/mbr5797/fast_index_compare_data"
    index_names = ["sourmash_sbt_10k.sbt.zip", "sourmash_sbt_20k.sbt.zip", "sourmash_sbt_30k.sbt.zip", "sourmash_sbt_40k.sbt.zip", "sourmash_sbt_50k.sbt.zip"]
    benchmark_filename = "benchmark/benchmark_results_sourmash_prefetch.csv"
    ref_sizes = [10000, 20000, 30000, 40000, 50000]
    
    # this is the query sketch in filelist_1, used in the other benchmarks
    query_sketch_name = "/scratch/mbr5797/fast_index_compare_data/signatures_gtdb_rs207_genomic/718882c2dd33e2711f95f8ed2a413fde.sig"
    
    ref_size_to_metrics = {}
    
    for index_name, ref_size in zip(index_names, ref_sizes):
        print(f"Processing {query_sketch_name} vs {index_name}")
        
        index_path = os.path.join(data_dir, index_name)
        output_filename = f"sourmash_prefetch_results_against_{ref_size}.csv"
        output_filename = os.path.join(data_dir, output_filename)
        
        # cmd: sourmash prefetch query_filename sbt_name --threshold_bp 0 -o output_filename
        command = f"sourmash prefetch {query_sketch_name} {index_path} --threshold-bp 0 -o {output_filename}"
        print(command)
        
        metrics = run_command_and_record_time_memory(command)
        if metrics:
            ref_size_to_metrics[ref_size] = metrics
        else:
            print(f"Error processing {query_sketch_name} vs {index_name}")
            
    print(ref_size_to_metrics)
    
    with open(benchmark_filename, "w") as f:
        print('Writing to: ' + benchmark_filename)
        f.write("ref_filesize,cpu_time,wall_clock_time,peak_memory_usage\n")
        for ref_size, metrics in ref_size_to_metrics.items():
            f.write(f"{ref_size},{metrics['cpu_time']},{metrics['wall_clock_time']},{metrics['peak_memory_usage']}\n")
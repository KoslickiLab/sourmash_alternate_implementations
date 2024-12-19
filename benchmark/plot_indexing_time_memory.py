import pandas as pd 
import matplotlib.pyplot as plt

# set arial as the default font
plt.rcParams['font.family'] = 'Arial'

# Load the data
df_sourmash_index = pd.read_csv('benchmark_results_sourmash_index.csv')
df_cpp_index_one_thread = pd.read_csv('benchmark_results_index_1_thread.csv')
df_cpp_index_128_threads = pd.read_csv('benchmark_results_index_128_threads.csv')

# col names: filesize,cpu_time,wall_clock_time,peak_memory_usage

# plot wall-clock time against file size
plt.plot(df_sourmash_index['filesize'], df_sourmash_index['wall_clock_time'], label='sourmash index', marker='o')
plt.plot(df_cpp_index_one_thread['filesize'], df_cpp_index_one_thread['wall_clock_time'], label='cpp index (1 thread)', marker='o')
plt.plot(df_cpp_index_128_threads['filesize'], df_cpp_index_128_threads['wall_clock_time'], label='cpp index (128 threads)', marker='o')

plt.xlabel('Number of references')
plt.ylabel('Wall-clock time (s) to build index')
plt.title('Wall-clock time to build indices for different number of references')
plt.xticks(df_sourmash_index['filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_indexing_wall_clock_time.pdf')

# plot cpu time against file size
plt.clf()
plt.plot(df_sourmash_index['filesize'], df_sourmash_index['cpu_time'], label='sourmash index', marker='o')
plt.plot(df_cpp_index_one_thread['filesize'], df_cpp_index_one_thread['cpu_time'], label='cpp index (1 thread)', marker='o')
plt.plot(df_cpp_index_128_threads['filesize'], df_cpp_index_128_threads['cpu_time'], label='cpp index (128 threads)', marker='o')

plt.xlabel('Number of references')
plt.ylabel('CPU time (s) to build index')
plt.title('CPU time to build indices for different number of references')
plt.xticks(df_sourmash_index['filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_indexing_cpu_time.pdf')

# plot peak memory usage against file size
plt.clf()
plt.plot(df_sourmash_index['filesize'], df_sourmash_index['peak_memory_usage'], label='sourmash index', marker='o')
plt.plot(df_cpp_index_one_thread['filesize'], df_cpp_index_one_thread['peak_memory_usage'], label='cpp index (1 thread)', marker='o')
plt.plot(df_cpp_index_128_threads['filesize'], df_cpp_index_128_threads['peak_memory_usage'], label='cpp index (128 threads)', marker='o')

plt.xlabel('Number of references')
plt.ylabel('Peak memory usage (GB) to build index')
plt.title('Peak memory usage to build indices for different number of references')
plt.xticks(df_sourmash_index['filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_indexing_peak_memory_usage.pdf')

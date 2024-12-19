import pandas as pd 
import matplotlib.pyplot as plt

# set arial as the default font
plt.rcParams['font.family'] = 'Arial'

# Load the data
df_compare = pd.read_csv('benchmark_results_compare_one_v_many.csv')
df_multisearch = pd.read_csv('benchmark_results_multisearch_one_v_many.csv')
df_prefetch_lm = pd.read_csv('benchmark_results_this_prefetch.csv')

# col names: ref_filesize,cpu_time,wall_clock_time,peak_memory_usage

# plot wall-clock time against reference file size
plt.plot(df_compare['ref_filesize'], df_compare['wall_clock_time'], label='compare (cpp) (1 thread)', marker='o')
plt.plot(df_multisearch['ref_filesize'], df_multisearch['wall_clock_time'], label='multisearch (sourmash) (1 thread)', marker='o')
plt.plot(df_prefetch_lm['ref_filesize'], df_prefetch_lm['wall_clock_time'], label='prefetch-lm (cpp) (1 thread)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('Wall-clock time (s)')
plt.title('Wall-clock time for one query')
plt.xticks(df_compare['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_one_v_many_wall_clock_time.pdf')

# plot peak memory usage against reference file size
plt.clf()
plt.plot(df_compare['ref_filesize'], df_compare['peak_memory_usage'], label='compare (cpp) (1 thread)', marker='o')
plt.plot(df_multisearch['ref_filesize'], df_multisearch['peak_memory_usage'], label='multisearch (sourmash) (1 thread)', marker='o')
plt.plot(df_prefetch_lm['ref_filesize'], df_prefetch_lm['peak_memory_usage'], label='prefetch-lm (cpp) (1 thread)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('Peak memory usage (GB)')
plt.title('Peak memory usage for one query')
plt.xticks(df_compare['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)

plt.savefig('plots/benchmark_results_compare_one_v_many_peak_memory_usage.pdf')

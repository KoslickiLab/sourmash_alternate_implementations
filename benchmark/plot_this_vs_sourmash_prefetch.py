import pandas as pd 
import matplotlib.pyplot as plt

# set arial as the default font
plt.rcParams['font.family'] = 'Arial'

# Load the data
df_this_prefetch = pd.read_csv('benchmark_results_this_prefetch.csv')
df_sourmash_prefetch = pd.read_csv('benchmark_results_sourmash_prefetch.csv')

# col names: ref_filesize,cpu_time,wall_clock_time,peak_memory_usage

# plot wall-clock time against reference file size
plt.plot(df_this_prefetch['ref_filesize'], df_this_prefetch['wall_clock_time'], label='cpp prefetch (1 thread)', marker='o')
plt.plot(df_sourmash_prefetch['ref_filesize'], df_sourmash_prefetch['wall_clock_time'], label='sourmash prefetch (1 thread)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('Wall-clock time (s)')
plt.title('Wall-clock time for one query when loading an index')
plt.xticks(df_this_prefetch['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_prefetch_wall_clock_time.pdf')


# plot peak memory usage against reference file size
plt.clf()
plt.plot(df_this_prefetch['ref_filesize'], df_this_prefetch['peak_memory_usage'], label='cpp prefetch (1 thread)', marker='o')
plt.plot(df_sourmash_prefetch['ref_filesize'], df_sourmash_prefetch['peak_memory_usage'], label='sourmash prefetch (1 thread)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('Peak memory usage (GB)')
plt.title('Peak memory usage for one query when loading an index')
plt.xticks(df_this_prefetch['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_prefetch_peak_memory_usage.pdf')
import pandas as pd 
import matplotlib.pyplot as plt

# set arial as the default font
plt.rcParams['font.family'] = 'Arial'

# Load the data
df_compare = pd.read_csv('benchmark_results_compare_many_v_many.csv')
df_multisearch = pd.read_csv('benchmark_results_multisearch_many_v_many.csv')

# col names: query_filesize,ref_filesize,cpu_time,wall_clock_time,peak_memory_usage

# plot wall-clock time for query file size = 50000, use different reference file sizes in x-axis
df_compare_50000 = df_compare[df_compare['query_filesize'] == 50000]
df_multisearch_50000 = df_multisearch[df_multisearch['query_filesize'] == 50000]

plt.plot(df_compare_50000['ref_filesize'], df_compare_50000['wall_clock_time'], label='compare (cpp)', marker='o')
plt.plot(df_multisearch_50000['ref_filesize'], df_multisearch_50000['wall_clock_time'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('Wall-clock time (s)')
plt.title('Wall-clock time for 50k queries')
plt.xticks(df_compare_50000['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_wall_clock_time.pdf')


# plot peak memory usage for query file size = 50000, use different reference file sizes in x-axis
plt.clf()
plt.plot(df_compare_50000['ref_filesize'], df_compare_50000['peak_memory_usage'], label='compare (cpp)', marker='o')
plt.plot(df_multisearch_50000['ref_filesize'], df_multisearch_50000['peak_memory_usage'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('Peak memory usage (GB)')
plt.title('Peak memory usage for 50k queries')
plt.xticks(df_compare_50000['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_peak_memory_usage.pdf')


# plot wall-clock time for ref file size = 50000, use different query file sizes in x-axis
plt.clf()
df_compare_50000 = df_compare[df_compare['ref_filesize'] == 50000]
df_multisearch_50000 = df_multisearch[df_multisearch['ref_filesize'] == 50000]

plt.plot(df_compare_50000['query_filesize'], df_compare_50000['wall_clock_time'], label='compare (cpp)', marker='o')
plt.plot(df_multisearch_50000['query_filesize'], df_multisearch_50000['wall_clock_time'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Query list size')
plt.ylabel('Wall-clock time (s)')
plt.title('Wall-clock time for 50k references')
plt.xticks(df_compare_50000['query_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_wall_clock_time_query.pdf')


# plot peak memory usage for ref file size = 50000, use different query file sizes in x-axis
plt.clf()
plt.plot(df_compare_50000['query_filesize'], df_compare_50000['peak_memory_usage'], label='compare (cpp)', marker='o')
plt.plot(df_multisearch_50000['query_filesize'], df_multisearch_50000['peak_memory_usage'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Query list size')
plt.ylabel('Peak memory usage (GB)')
plt.title('Peak memory usage for 50k references')
plt.xticks(df_compare_50000['query_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_peak_memory_usage_query.pdf')

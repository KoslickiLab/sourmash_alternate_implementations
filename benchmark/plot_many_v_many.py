import pandas as pd 
import matplotlib.pyplot as plt

# set arial as the default font
plt.rcParams['font.family'] = 'Arial'

# Load the data
df_compare = pd.read_csv('benchmark_results_compare_many_v_many.csv')
df_compare_without_index = pd.read_csv('benchmark_results_compare_many_v_many_no_index.csv')
df_multisearch = pd.read_csv('benchmark_results_multisearch_many_v_many.csv')

# drop rows with query_filesize > 30000, ref_filesize > 30000
df_compare = df_compare[(df_compare['query_filesize'] <= 30000) & (df_compare['ref_filesize'] <= 30000)]
df_compare_without_index = df_compare_without_index[(df_compare_without_index['query_filesize'] <= 30000) & (df_compare_without_index['ref_filesize'] <= 30000)]
df_multisearch = df_multisearch[(df_multisearch['query_filesize'] <= 30000) & (df_multisearch['ref_filesize'] <= 30000)]

# col names: query_filesize,ref_filesize,cpu_time,wall_clock_time,peak_memory_usage

# plot wall-clock time for query file size = 30000, use different reference file sizes in x-axis
df_compare_30000 = df_compare[df_compare['query_filesize'] == 30000]
df_compare_without_index_30000 = df_compare_without_index[df_compare_without_index['query_filesize'] == 30000]
df_multisearch_30000 = df_multisearch[df_multisearch['query_filesize'] == 30000]

plt.plot(df_compare_30000['ref_filesize'], df_compare_30000['wall_clock_time'], label='compare (cpp) by building index', marker='o')
plt.plot(df_compare_without_index_30000['ref_filesize'], df_compare_without_index_30000['wall_clock_time'], label='compare (cpp) by loading index', marker='o')
plt.plot(df_multisearch_30000['ref_filesize'], df_multisearch_30000['wall_clock_time'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('Wall-clock time (s)')
plt.title('Wall-clock time for 30k queries')
plt.xticks(df_compare_30000['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_wall_clock_time.pdf')


# plot CPU time for query file size = 30000, use different reference file sizes in x-axis
plt.clf()
plt.plot(df_compare_30000['ref_filesize'], df_compare_30000['cpu_time'], label='compare (cpp) by building index', marker='o')
plt.plot(df_compare_without_index_30000['ref_filesize'], df_compare_without_index_30000['cpu_time'], label='compare (cpp) by loading index', marker='o')
plt.plot(df_multisearch_30000['ref_filesize'], df_multisearch_30000['cpu_time'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('CPU time (s)')
plt.title('CPU time for 30k queries')
plt.xticks(df_compare_30000['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_cpu_time.pdf')


# plot peak memory usage for query file size = 30000, use different reference file sizes in x-axis
plt.clf()
plt.plot(df_compare_30000['ref_filesize'], df_compare_30000['peak_memory_usage'], label='compare (cpp) by building index', marker='o')
plt.plot(df_compare_without_index_30000['ref_filesize'], df_compare_without_index_30000['peak_memory_usage'], label='compare (cpp) by loading index', marker='o')
plt.plot(df_multisearch_30000['ref_filesize'], df_multisearch_30000['peak_memory_usage'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Reference list size')
plt.ylabel('Peak memory usage (GB)')
plt.title('Peak memory usage for 30k queries')
plt.xticks(df_compare_30000['ref_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_peak_memory_usage.pdf')


# plot wall-clock time for ref file size = 30000, use different query file sizes in x-axis
plt.clf()
df_compare_30000 = df_compare[df_compare['ref_filesize'] == 30000]
df_compare_without_index_30000 = df_compare_without_index[df_compare_without_index['ref_filesize'] == 30000]
df_multisearch_30000 = df_multisearch[df_multisearch['ref_filesize'] == 30000]

plt.plot(df_compare_30000['query_filesize'], df_compare_30000['wall_clock_time'], label='compare (cpp) by building index', marker='o')
plt.plot(df_compare_without_index_30000['query_filesize'], df_compare_without_index_30000['wall_clock_time'], label='compare (cpp) by loading index', marker='o')
plt.plot(df_multisearch_30000['query_filesize'], df_multisearch_30000['wall_clock_time'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Query list size')
plt.ylabel('Wall-clock time (s)')
plt.title('Wall-clock time for 30k references')
plt.xticks(df_compare_30000['query_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_wall_clock_time_query.pdf')


# plot peak memory usage for ref file size = 30000, use different query file sizes in x-axis
plt.clf()
plt.plot(df_compare_30000['query_filesize'], df_compare_30000['peak_memory_usage'], label='compare (cpp) by building index', marker='o')
plt.plot(df_compare_without_index_30000['query_filesize'], df_compare_without_index_30000['peak_memory_usage'], label='compare (cpp) by loading index', marker='o')
plt.plot(df_multisearch_30000['query_filesize'], df_multisearch_30000['peak_memory_usage'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Query list size')
plt.ylabel('Peak memory usage (GB)')
plt.title('Peak memory usage for 30k references')
plt.xticks(df_compare_30000['query_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_peak_memory_usage_query.pdf')


# plot CPU time for ref file size = 30000, use different query file sizes in x-axis
plt.clf()
plt.plot(df_compare_30000['query_filesize'], df_compare_30000['cpu_time'], label='compare (cpp) by building index', marker='o')
plt.plot(df_compare_without_index_30000['query_filesize'], df_compare_without_index_30000['cpu_time'], label='compare (cpp) by loading index', marker='o')
plt.plot(df_multisearch_30000['query_filesize'], df_multisearch_30000['cpu_time'], label='multisearch (sourmash)', marker='o')

plt.xlabel('Query list size')
plt.ylabel('CPU time (s)')
plt.title('CPU time for 30k references')
plt.xticks(df_compare_30000['query_filesize'])
plt.legend()
plt.grid(linestyle='--', alpha=0.3)
plt.savefig('plots/benchmark_results_compare_many_v_many_cpu_time_query.pdf')

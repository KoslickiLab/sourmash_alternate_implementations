import argparse
import pandas as pd
import multiprocessing

def parse_args():
    # arguments: multisearch_file, by_index_file, genome_names
    parser = argparse.ArgumentParser(description='Test against multisearch')
    parser.add_argument('multisearch_file', type=argparse.FileType('r'))
    parser.add_argument('by_index_file', type=argparse.FileType('r'))
    parser.add_argument('--num_threads', type=int, default=1)
    
    return parser.parse_args()


def read_multisearch_output(multisearch_file):
    # a csv file with columns: query_name,query_md5,match_name,match_md5,containment,max_containment,jaccard,intersect_hashes
    df = pd.read_csv(multisearch_file, sep=',')
    return df


def read_by_index_output(by_index_file):
    # a csv file with columns: query_id, query_name, query_md5, match_id, match_name, match_md5, jaccard, containment_query_in_match, containment_match_in_query, max_containment, max_containment_ani
    df = pd.read_csv(by_index_file, sep=',')
    return df


def test_one_chunk(df_multisearch, df_by_index, query_genome_names, genome_names_start_index, genome_names_end_index):    
    
    # iterate over all rows of the by_index file
    for i in range(genome_names_start_index, genome_names_end_index):
        name_query_genome = query_genome_names[i]
        multisearch_results_with_this_query = df_multisearch[df_multisearch['query_name'] == name_query_genome]
        by_index_results_with_this_query = df_by_index[df_by_index['query_name'] == name_query_genome]
        
        # create a dictionary of the by_index_results, keyed by match_name
        by_index_results_dict = {}
        for index, row in by_index_results_with_this_query.iterrows():
            match_name = row['match_name']
            jaccard = row['jaccard']
            max_containment = row['max_containment']
            by_index_results_dict[match_name] = (jaccard, max_containment)
            
        # create a dictionary of the multisearch_results, keyed by match_name
        multisearch_results_dict = {}
        for index, row in multisearch_results_with_this_query.iterrows():
            match_name = row['match_name']
            jaccard = row['jaccard']
            max_containment = row['max_containment']
            multisearch_results_dict[match_name] = (jaccard, max_containment)
            
        # iterate over all these rows of the multisearch file
        for index, row in multisearch_results_with_this_query.iterrows():
            match_name = row['match_name']
            
            jaccard_multisearch = row['jaccard']
            max_containment_multisearch = row['max_containment']
            
            if match_name not in by_index_results_dict:
                # print all information
                print('Result found in multisearch but not in by_index: query_name =', name_query_genome, ', match_name =', match_name, ', jaccard =', jaccard_multisearch, ', containment =', max_containment_multisearch)
                continue
            
            # get the jaccard and containment from the by_index results
            jaccard_by_index, max_containment_by_index = by_index_results_dict[match_name]
            
            # compare the jaccard and containment values
            assert abs(jaccard_multisearch - jaccard_by_index) < 1e-4
            assert abs(max_containment_multisearch - max_containment_by_index) < 1e-4
            
        # iterate over all rows of the by_index file
        for index, row in by_index_results_with_this_query.iterrows():
            match_name = row['match_name']
            
            jaccard_by_index = row['jaccard']
            max_containment_by_index = row['max_containment']
            
            if match_name not in multisearch_results_dict:
                # print all information
                print('Result found in by_index but not in multisearch: query_name =', name_query_genome, ', match_name =', match_name, ', jaccard =', jaccard_by_index, ', containment =', containment_by_index, ', query_id =', i, ', match_id =', match_id)
                continue
            
            jaccard_multisearch, max_containment_multisearch = multisearch_results_dict[match_name]
            
            # compare the jaccard and containment values
            assert abs(jaccard_multisearch - jaccard_by_index) < 1e-4
            assert abs(max_containment_multisearch - max_containment_by_index) < 1e-4



def test_against_multisearch(multisearch_file, by_index_file, num_threads):
    df_multisearch = read_multisearch_output(multisearch_file)
    df_by_index = read_by_index_output(by_index_file)
    
    all_query_names = df_multisearch['query_name'].unique()
    
    num_query_genomes = len(all_query_names)
    chunk_size = num_query_genomes // num_threads
    process_list = []
    for i in range(num_threads):
        start_index = i * chunk_size
        end_index = start_index + chunk_size
        if i == num_threads - 1:
            end_index = num_query_genomes
        # run test_one_chunk in parallel using multiprocessing
        p = multiprocessing.Process(target=test_one_chunk, args=(df_multisearch, df_by_index, all_query_names, start_index, end_index))
        p.start()
        process_list.append(p)
    
    for p in process_list:
        p.join()
    
                
    print('All tests passed!')
    
    
def main():
    args = parse_args()
    print('Testing for the following arguments...')
    # show the arguments
    print(args)
    test_against_multisearch(args.multisearch_file, args.by_index_file, args.num_threads)
    
if __name__ == '__main__':
    main()
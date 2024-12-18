import argparse
import pandas as pd

# arguments: sourmash prefetch filename, our prefetch filename
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('sourmash_prefetch')
    parser.add_argument('our_prefetch')
    return parser.parse_args()

def main():
    args = parse_args()
    
    # read data as pandas dataframes
    sourmash_prefetch = pd.read_csv(args.sourmash_prefetch)
    our_prefetch = pd.read_csv(args.our_prefetch)
    
    # in our_prefetch, change match_md5 to match_md5[:8]
    our_prefetch['match_md5'] = our_prefetch['match_md5'].apply(lambda x: x[:8])
    
    # remove spaces from column names
    sourmash_prefetch.columns = sourmash_prefetch.columns.str.strip()
    our_prefetch.columns = our_prefetch.columns.str.strip()
    
    # sourmash prefetch column names
    # intersect_bp,jaccard,max_containment,f_query_match,f_match_query,match_filename,match_name,match_md5,match_bp,query_filename,query_name,query_md5,query_bp,ksize,moltype,scaled,query_n_hashes,query_abundance,query_containment_ani,match_containment_ani,average_containment_ani,max_containment_ani,potential_false_negative
    
    # our prefetch column names
    # match_id,match_name,match_md5,num_intersections,containment_query_ref,containment_ref_query,max_containment,jaccard
    
    tests_passed = True
    
    # md5 set of matches for both results
    sourmash_md5_set = set(sourmash_prefetch['match_md5'])
    our_md5_set = set(our_prefetch['match_md5'])
    
    # check if the md5 sets are the same
    if sourmash_md5_set == our_md5_set:
        print('MD5 sets are the same')
    else:
        print('MD5 sets are different')
        tests_passed = False
        
    # find the md5s that are in sourmash but not in our results
    for md5 in sourmash_md5_set:
        if md5 not in our_md5_set:
            print(f'MD5 {md5} is in sourmash but not in our results')
        
    # find the md5s that are in our results but not in sourmash
    for md5 in our_md5_set:
        if md5 not in sourmash_md5_set:
            print(f'MD5 {md5} is in our results but not in sourmash')
            
    # for the common md5s, compare intersect_bp==num_intersections, jaccard==jaccard, max_containment==max_containment
    
    # find the common md5s
    common_md5s = sourmash_md5_set.intersection(our_md5_set)
    print(f'Number of common md5s: {len(common_md5s)}')
    
    for md5 in common_md5s:
        sourmash_row = sourmash_prefetch[sourmash_prefetch['match_md5'] == md5]
        our_row = our_prefetch[our_prefetch['match_md5'] == md5]
        
        sourmash_intersect_bp = sourmash_row['intersect_bp'].values[0]/1000
        our_num_intersections = our_row['num_intersections'].values[0]
        assert abs(sourmash_intersect_bp - our_num_intersections) < 0.0001
        
        sourmash_jaccard = sourmash_row['jaccard'].values[0]
        our_jaccard = our_row['jaccard'].values[0]
        assert abs(sourmash_jaccard - our_jaccard) < 0.0001
        
        sourmash_max_containment = sourmash_row['max_containment'].values[0]
        our_max_containment = our_row['max_containment'].values[0]
        assert abs(sourmash_max_containment - our_max_containment) < 0.0001
        
    print("For the common md5s, intersect_bp==num_intersections, jaccard==jaccard, max_containment==max_containment")
        
    if tests_passed:
        print('All tests passed')
    else:
        print('Some tests failed')
        
    if not tests_passed:
        exit(1)
        
if __name__ == '__main__':
    main()
        
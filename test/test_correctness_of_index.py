import argparse
import json

def parse_args():
    parser = argparse.ArgumentParser(description='Test correctness of index.py')
    parser.add_argument('--filelist', type=str, help='Input sketch lists')
    parser.add_argument('--index_directory', type=str, help='The directory of index files')
    return parser.parse_args()


def main():
    args = parse_args()
    filelist = args.filelist
    index_directory = args.index_directory

    index_to_build = {}

    # each line in filelist is a sketch file, which is a json file
    with open(filelist, 'r') as f_list:
        id_of_this_sketch = 0
        for line in f_list:
            sketch_file = line.strip()
            with open(sketch_file, 'r') as f_sketch:
                sketch = json.load(f_sketch)
                min_hashes = sketch[0]['signatures'][0]['mins']
                for min_hash in min_hashes:
                    if min_hash not in index_to_build:
                        index_to_build[min_hash] = []
                    index_to_build[min_hash].append(id_of_this_sketch)
            id_of_this_sketch += 1

    # build index
    built_index = index_to_build

    # load index
    loaded_index = {}
    # open index_directory/summary
    summary_filename = 'summary'
    with open(f'{index_directory}/{summary_filename}', 'r') as f_summary:
        # open the file summary, this is a simple text file
        # first line is the number of individaul files = n
        # then follows n lines, each line is a separate text file
        num_files = int(f_summary.readline().strip())
        for i in range(num_files):
            line = f_summary.readline().strip()
            with open(line, 'rb') as f_index:
                # each line in this file is a binary file, the data is as follows:
                # 64 bits: minhash value
                # 32 bits: number of sketches that have this minhash value = m
                # m * 32 bits: the ids of the sketches that have this minhash value
                while True:
                    min_hash = f_index.read(8)
                    if not min_hash:
                        break
                    min_hash = int.from_bytes(min_hash, byteorder='big')
                    num_sketches = int.from_bytes(f_index.read(4), byteorder='big')
                    for i in range(num_sketches):
                        sketch_id = int.from_bytes(f_index.read(4), byteorder='big')
                        if min_hash not in loaded_index:
                            loaded_index[min_hash] = []
                        loaded_index[min_hash].append(sketch_id)

    # compare built index and loaded index
    assert built_index == loaded_index


if __name__ == '__main__':
    main()
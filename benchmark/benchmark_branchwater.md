# Multisearch

Following results are on gtdb_rs207, sketches are computed using k = 51, obtained from here: https://farm.cse.ucdavis.edu/~ctbrown/sourmash-db/. Random number of files are selected from all the sketch files using bash command `shuf -n`. Running using 128 threads.

### 10k vs 10k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_10k_v_10k -c 128 filelist_10k filelist_10k -k 51"
        User time (seconds): 5857.33
        System time (seconds): 41.45
        Percent of CPU this job got: 12213%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:48.29
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 1471596
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1056536
        Voluntary context switches: 1290466
        Involuntary context switches: 186113
        Swaps: 0
        File system inputs: 0
        File system outputs: 952256
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 10k vs 20k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_10k_v_20k -c 128 filelist_10k filelist_20k -k 51"
        User time (seconds): 10000.54
        System time (seconds): 63.48
        Percent of CPU this job got: 12345%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 1:21.51
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 2084208
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 10
        Minor (reclaiming a frame) page faults: 1262159
        Voluntary context switches: 2880364
        Involuntary context switches: 13649
        Swaps: 0
        File system inputs: 0
        File system outputs: 1884896
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 10k vs 30k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_10k_v_30k -c 128 filelist_10k filelist_30k -k 51"
        User time (seconds): 16481.60
        System time (seconds): 77.57
        Percent of CPU this job got: 12516%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 2:12.29
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 2703144
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 6
        Minor (reclaiming a frame) page faults: 1760314
        Voluntary context switches: 2988700
        Involuntary context switches: 223234
        Swaps: 0
        File system inputs: 0
        File system outputs: 2825896
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 20k vs 10k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_20k_v_10k -c 128 filelist_20k filelist_10k -k 51"
        User time (seconds): 11138.36
        System time (seconds): 67.50
        Percent of CPU this job got: 12417%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 1:30.24
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 2086636
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 7
        Minor (reclaiming a frame) page faults: 1444073
        Voluntary context switches: 2629121
        Involuntary context switches: 14391
        Swaps: 0
        File system inputs: 0
        File system outputs: 1883344
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 20k vs 20k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_20k_v_20k -c 128 filelist_20k filelist_20k -k 51"
        User time (seconds): 21470.71
        System time (seconds): 111.75
        Percent of CPU this job got: 11984%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 3:00.09
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 2694800
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 1
        Minor (reclaiming a frame) page faults: 2137409
        Voluntary context switches: 5384777
        Involuntary context switches: 22524
        Swaps: 0
        File system inputs: 0
        File system outputs: 3753096
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 20k vs 30k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_20k_v_30k -c 128 filelist_20k filelist_30k -k 51"
        User time (seconds): 32201.64
        System time (seconds): 124.67
        Percent of CPU this job got: 12624%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 4:16.06
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 3314284
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 6
        Minor (reclaiming a frame) page faults: 2647700
        Voluntary context switches: 5596632
        Involuntary context switches: 36227
        Swaps: 0
        File system inputs: 0
        File system outputs: 5619168
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 30k vs 10k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_30k_v_10k -c 128 filelist_30k filelist_10k -k 51"
        User time (seconds): 15960.98
        System time (seconds): 108.19
        Percent of CPU this job got: 12394%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 2:09.65
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 2714240
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 10
        Minor (reclaiming a frame) page faults: 2851528
        Voluntary context switches: 4206549
        Involuntary context switches: 18889
        Swaps: 0
        File system inputs: 0
        File system outputs: 2823520
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 30k vs 20k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_30k_v_20k -c 128 filelist_30k filelist_20k -k 51"
        User time (seconds): 32489.22
        System time (seconds): 1853.57
        Percent of CPU this job got: 12583%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 4:32.92
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 3316032
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 140
        Minor (reclaiming a frame) page faults: 22618805
        Voluntary context switches: 6633070
        Involuntary context switches: 39753
        Swaps: 0
        File system inputs: 0
        File system outputs: 5618888
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 30k vs 30k
```
Command being timed: "sourmash scripts multisearch -o multisearch_output_30k_v_30k -c 128 filelist_30k filelist_30k -k 51"
        User time (seconds): 49813.51
        System time (seconds): 227.40
        Percent of CPU this job got: 12524%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 6:39.55
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 3923500
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 7
        Minor (reclaiming a frame) page faults: 3532498
        Voluntary context switches: 12505579
        Involuntary context switches: 166652
        Swaps: 0
        File system inputs: 0
        File system outputs: 8433712
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```
# Compare

Following results are on gtdb_rs207, sketches are computed using k = 51, obtained from here: https://farm.cse.ucdavis.edu/~ctbrown/sourmash-db/. Random number of files are selected from all the sketch files using bash command `shuf -n`. Running using 128 threads.

## Many vs many

### 10k vs 10k
```
Command being timed: "compare filelist_10k filelist_10k temp_working_dir compare_output_10k_v_10k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 143.81
        System time (seconds): 60.66
        Percent of CPU this job got: 1626%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:12.57
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 2532424
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 823630
        Voluntary context switches: 1226459
        Involuntary context switches: 424
        Swaps: 0
        File system inputs: 8
        File system outputs: 1836328
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 10k vs 20k
```
Command being timed: "compare filelist_10k filelist_20k temp_working_dir compare_output_10k_v_20k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 250.93
        System time (seconds): 129.08
        Percent of CPU this job got: 1558%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:24.37
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 4541844
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1609095
        Voluntary context switches: 2331173
        Involuntary context switches: 748
        Swaps: 0
        File system inputs: 0
        File system outputs: 3641856
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 10k vs 30k
```
Command being timed: "compare filelist_10k filelist_30k temp_working_dir compare_output_10k_v_30k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 369.73
        System time (seconds): 176.07
        Percent of CPU this job got: 1480%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:36.85
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 6875008
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 2305524
        Voluntary context switches: 3383151
        Involuntary context switches: 950
        Swaps: 0
        File system inputs: 0
        File system outputs: 5463056
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 20k vs 10k
```
Command being timed: "compare filelist_20k filelist_10k temp_working_dir compare_output_20k_v_10k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 204.18
        System time (seconds): 98.15
        Percent of CPU this job got: 2070%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:14.60
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 3230060
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1041766
        Voluntary context switches: 1250285
        Involuntary context switches: 836
        Swaps: 0
        File system inputs: 0
        File system outputs: 3638920
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 20k vs 20k
```
Command being timed: "compare filelist_20k filelist_20k temp_working_dir compare_output_20k_v_20k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 362.24
        System time (seconds): 185.28
        Percent of CPU this job got: 2029%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:26.98
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 5638892
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1838705
        Voluntary context switches: 2308089
        Involuntary context switches: 1167
        Swaps: 0
        File system inputs: 50920
        File system outputs: 7267856
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 20k vs 30k
```
Command being timed: "compare filelist_20k filelist_30k temp_working_dir compare_output_20k_v_30k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 506.49
        System time (seconds): 310.48
        Percent of CPU this job got: 1871%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:43.64
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 8352676
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 3086938
        Voluntary context switches: 3506398
        Involuntary context switches: 1734
        Swaps: 0
        File system inputs: 0
        File system outputs: 10886936
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 30k vs 10k
```
Command being timed: "compare filelist_30k filelist_10k temp_working_dir compare_output_30k_v_10k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 256.92
        System time (seconds): 146.69
        Percent of CPU this job got: 2463%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:16.38
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 3930456
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1264155
        Voluntary context switches: 1281040
        Involuntary context switches: 837
        Swaps: 0
        File system inputs: 0
        File system outputs: 5458488
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 30k vs 20k
```
Command being timed: "compare filelist_30k filelist_20k temp_working_dir compare_output_30k_v_20k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 457.10
        System time (seconds): 279.56
        Percent of CPU this job got: 2349%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:31.35
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 6738796
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 2315843
        Voluntary context switches: 2354927
        Involuntary context switches: 1413
        Swaps: 0
        File system inputs: 0
        File system outputs: 10886472
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 30k vs 30k
```
Command being timed: "compare filelist_30k filelist_30k temp_working_dir compare_output_30k_v_30k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 649.32
        System time (seconds): 394.24
        Percent of CPU this job got: 2208%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:47.25
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 9827744
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 3428622
        Voluntary context switches: 3456783
        Involuntary context switches: 2029
        Swaps: 0
        File system inputs: 0
        File system outputs: 16350720
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```



## One vs many

### 1 vs 10k
```
Command being timed: "compare filelist_1 filelist_10k temp_working_dir compare_output_1_v_10k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 86.86
        System time (seconds): 28.05
        Percent of CPU this job got: 866%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:13.25
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 1848688
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 684809
        Voluntary context switches: 1216669
        Involuntary context switches: 203
        Swaps: 0
        File system inputs: 0
        File system outputs: 8
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 1 vs 20k

```
Command being timed: "compare filelist_1 filelist_20k temp_working_dir compare_output_1_v_20k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 169.37
        System time (seconds): 53.80
        Percent of CPU this job got: 887%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:25.14
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 3461548
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1633529
        Voluntary context switches: 2279784
        Involuntary context switches: 441
        Swaps: 0
        File system inputs: 0
        File system outputs: 8
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

### 1 vs 30k

```
Command being timed: "compare filelist_1 filelist_30k temp_working_dir compare_output_1_v_30k -t 128 -n 2000 -k 51 -c 0.01"
        User time (seconds): 270.73
        System time (seconds): 82.29
        Percent of CPU this job got: 1003%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:35.17
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 5408896
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 1953033
        Voluntary context switches: 3372865
        Involuntary context switches: 790
        Swaps: 0
        File system inputs: 0
        File system outputs: 8
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```
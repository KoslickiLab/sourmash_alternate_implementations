# smash_alternate_implementations

An alternate implementation of the tools available in sourmash.

# Building
```
make
```

# Available tools
1. prefetch
1. compare
1. gather

# Usages
All tool usages are available using `--help` flag.


## `compare` output format
1. Index of query
1. Name of query
1. MD5 of query
1. Size of the query sketch
1. Index of match
1. Name of match
1. MD5 of match
1. Size of the matched reference sketch
1. Jaccard
1. Containment(query, target)
1. Containment(target, query)


# Testing
Test scripts are available in the `test` directory. These tests are written in python.
To install the dependencies required by the test scripts, do the following:
```
conda create --name <env_name> --file test/requirements.txt
```
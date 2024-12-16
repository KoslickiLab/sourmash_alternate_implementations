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
2. Name of query
3. MD5 of query
4. Index of match
5. Name of match
6. MD5 of match
7. Jaccard
8. Containment(query, target)
9. Containment(target, query)


# Testing
Test scripts are available in the `test` directory. These tests are written in python.
To install the dependencies required by the test scripts, do the following:
```
conda create --name <env_name> --file test/requirements.txt
```
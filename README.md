# Cerberus: A Formal Approach to Secure and Efficient Enclave Memory Sharing

This is the artifacts of the paper 

## Directory Structure

* `AbstractPlatform`: A formal model for Cerberus extended from TAP
* `Common`: Some common definitions for the formal model
* `Apps`: Implemented/Modified applications
* `KeystoneImplementation`: Cerberus implementation on top of Keystone (diff.txt includes the diff from the latest master branch)

## How to Read the Code

All implementatios are incremental to the latest [Keystone](https://github.com/keystone-enclave/keystone).
Git logs have been removed to anonymize the submission.
Thus, see diff.txt in each of the components to see the difference between the latest master and our implementation

## How to Test

* Follow [docs](https://docs.keystone-enclave.org) to setup initial Keystone repo.
* Replace each of the components (sdk, sm/src, keystone-driver, keystone-runtime) with the ones that are provided in this repo

keystone-runtime is pulled from git repo, so we changed the URL in the [macro](https://github.com/anonymous1721/TAPC/tree/main/KeystoneImplementation/sdk/macros.cmake). This will download the correct version of Eyrie runtime.
Make sure to remove `build/examples` directory entirely before re-compiling the apps with the modified runtime.

### Sqlite

For sqlite, all source code / CMakeLists.txt are included in the directory.
Thus, simply copy the sqlite directory to `sdk/examples`, and add `add_subdirectory(sqlite)` to `sdk/examples/CMakelists.txt`.

Run the following command in `build` directory (of the Keystone repo)

```
make examples; cp examples/sqlite/sqlite-test.ke overlay/root; make image
```

This will make the `sqlite-test.ke` package available when you boot the machine

----
# Origin Keystone Readme:
# Keystone: An Open-Source Secure Enclave Framework for RISC-V Processors

![Documentation Status](https://readthedocs.org/projects/keystone-enclave/badge/)
[![Build Status](https://travis-ci.org/keystone-enclave/keystone.svg?branch=master)](https://travis-ci.org/keystone-enclave/keystone/)

Visit [Project Website](https://keystone-enclave.org) for more information.

`master` branch is for public releases.
`dev` branch is for development use (up-to-date but may not fully documented until merged into `master`).

# Documentation

See [docs](http://docs.keystone-enclave.org) for getting started.

# Contributing

See CONTRIBUTING.md

# Citation

If you want to cite the project, please use the following bibtex:

```
@inproceedings{lee2019keystone,
    title={Keystone: An Open Framework for Architecting Trusted Execution Environments},
    author={Dayeol Lee and David Kohlbrenner and Shweta Shinde and Krste Asanovic and Dawn Song},
    year={2020},
    booktitle = {Proceedings of the Fifteenth European Conference on Computer Systems},
    series = {EuroSys’20}
}
```

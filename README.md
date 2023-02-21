# 信息系统安全作业2

# Cerberus: A Formal Approach to Secure and Efficient Enclave Memory Sharing

* 参考 [docs](https://docs.keystone-enclave.org) 搭建 Keystone 
* 替换为Cerberus的组件 (sdk, sm/src, keystone-driver, keystone-runtime) ，
* ps: 本repo中已完成替换


```
make examples; cp examples/sqlite/sqlite-test.ke overlay/root; make image
```


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

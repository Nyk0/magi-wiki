# Welcome to Magi

Welcome to the Magi wiki. Here you will find documentation related to the University of Sorbonne Paris North research support department. Let’s start our journey with the presentation of HPC resources. In an HPC cluster all resources are gathered in
different partitions. A partition is a set of homogeneous compute nodes (ie computers that run your code). Here are the resources available on Magi for HPC part:

| Partition name | Number of nodes | Processor | GPU | Memory | Network |
| -- | -- | -- | -- | -- | -- |
| COMPUTE | 51 | 2 x Intel(R) Xeon(R) CPU E5-2650 v3 @ 2.30GHz (40 cores) | x | 64 GB | Infiniband |
| MISC-56c | 15 | 2 x Intel(R) Xeon(R) Gold 5120 CPU @ 2.20GHz (56 cores) | x | 96 GB | Ethernet 1 GB |
| SMP-80c | 1 | 2 x Intel(R) Xeon(R) CPU E7-4820 v3 @ 1.90GHz (80 cores) | x | 512 GB | Ethernet 1 GB |
| SMP-128c | 2 | 2 x AMD EPYC 7502 32-Core Processor (128 cores) | x | 256 GB | Ethernet 1 GB |
| SMP-256c | 2 | 2 x AMD EPYC 9554 64-Core Processor (256 cores) | x | 1,6 TB | Ethernet 1 GB |
| GPU-A100 | 1 | 2 x AMD EPYC 7513 32-Core Processor (128 cores) | 10 x NVIDIA A100 80 GB  | 512 GB | Ethernet 1 GB |
| FORMATION | 2 | 2 x Intel(R) Xeon(R) Bronze 3204 CPU @ 1.90GHz (6 cores) | x | 48 GB | Ethernet 1 GB |

We have three flavors of each partition:
* Regular means that your job can run forever;
* Short means that your job can run for 3 days, but you get an higher priority than Regular;
* Very short means that your job can run for 3 hours, but you get an higher priority than Short.

To invoke a partition flavor you will use the partition name for regular (eg COMPUTE), the partition name with the suffix SHORT for short (eg COMPUTE-SHORT) and the partition name with the suffix VERYSHORT for very short (eg COMPUTE-VERYSHORT).

## SLURM quickstart howto

We use SLURM (Simple Linux Utility for Resource Management) as the job scheduler for our HPC cluster. In this section, we will introduce SLURM basic commands.

### sinfo


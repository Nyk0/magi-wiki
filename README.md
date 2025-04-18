# Welcome to Magi

Welcome to the Magi wiki. Here you will find documentation related to the University of Sorbonne Paris North research support department. Let’s start our journey with the presentation of HPC resources. In an HPC cluster all resources are gathered in
different partitions. A partition is a set of homogeneous compute nodes (ie computers that run your code). Here are the resources available on Magi for HPC part:

| Partition name | Number of nodes | Processor | Arch | GPU | Memory | Network |
| -- | -- | -- | -- | -- | -- |
| COMPUTE | 51 | 2 x Intel(R) Xeon(R) CPU E5-2650 v3 @ 2.30GHz (40 cores) | haswell | x | 64 GB | Infiniband |
| MISC-56c | 15 | 2 x Intel(R) Xeon(R) Gold 5120 CPU @ 2.20GHz (56 cores) | skylake | x | 96 GB | Ethernet 1 GB |
| SMP-80c | 1 | 2 x Intel(R) Xeon(R) CPU E7-4820 v3 @ 1.90GHz (80 cores) | westmere | x | 512 GB | Ethernet 1 GB |
| SMP-128c | 2 | 2 x AMD EPYC 7502 32-Core Processor (128 cores) | znver2 | x | 256 GB | Ethernet 1 GB |
| SMP-256c | 2 | 2 x AMD EPYC 9554 64-Core Processor (256 cores) | znver4 | 1,6 TB | Ethernet 1 GB |
| GPU-A100 | 1 | 2 x AMD EPYC 7513 32-Core Processor (128 cores) | znver3 | 10 x NVIDIA A100 80 GB  | 512 GB | Ethernet 1 GB |
| FORMATION | 2 | 2 x Intel(R) Xeon(R) Bronze 3204 CPU @ 1.90GHz (6 cores) | cascadelake | x | 48 GB | Ethernet 1 GB |

We have three flavors of each partition:
* Regular means that your job can run forever;
* Short means that your job can run for 3 days, but you get an higher priority than Regular;
* Very short means that your job can run for 3 hours, but you get an higher priority than Short.

To invoke a partition flavor you will use the partition name for regular (eg COMPUTE), the partition name with the suffix SHORT for short (eg COMPUTE-SHORT) and the partition name with the suffix VERYSHORT for very short (eg COMPUTE-VERYSHORT).

## Cite Magi in your publications

This is how to cite Magi in your publications:

*The experiments in this work were carried out on Magi, the experimental platform of University of Sorbonne Paris North (USPN) dedicated to research. This platform offers researchers at the institution High-Performance Computing (HPC), cloud and storage services.*

## SLURM quickstart howto

We use SLURM (Simple Linux Utility for Resource Management) as the job scheduler for our HPC cluster. In this section, we will introduce SLURM basic commands.

### sinfo

sinfo is used to view partition and node information for a system running Slurm. 

```console
nicolas.greneche@frontend:~$ sinfo
PARTITION          AVAIL  TIMELIMIT  NODES  STATE NODELIST
COMPUTE*              up   infinite      1 drain* magi77
COMPUTE*              up   infinite     12    mix magi[50-54,69-73,79,83]
COMPUTE*              up   infinite     28  alloc magi[46-49,55-68,74-76,78,80-82,84-86]
COMPUTE-SHORT         up 3-00:00:00      1 drain* magi77
COMPUTE-SHORT         up 3-00:00:00     21    mix magi[50-54,69-73,79,83,88-93,96-98]
COMPUTE-SHORT         up 3-00:00:00     29  alloc magi[46-49,55-68,74-76,78,80-82,84-87]
COMPUTE-VERYSHORT     up    3:00:00      1 drain* magi77
COMPUTE-VERYSHORT     up    3:00:00     21    mix magi[50-54,69-73,79,83,88-93,96-98]
COMPUTE-VERYSHORT     up    3:00:00     29  alloc magi[46-49,55-68,74-76,78,80-82,84-87]
SMP-128c              up   infinite      2   idle magi[166-167]
SMP-128c-SHORT        up 3-00:00:00      2   idle magi[166-167]
SMP-128c-VERYSHORT    up    3:00:00      2   idle magi[166-167]
```

Columns explanation:

* PARTITION name of the partition;
* AVAIL partition is up or down;
* TIMELIMIT walltime of job (maximum time to live);
* NODES number of nodes of a partition in a given state;
* STATE state of the node:
    * drain: node is down for an administrative reason;
    * alloc: the whole node is allocated for a job;
    * mix: the node is busy running multiple jobs;
    * idle: the node is free, waiting for jobs.
* NODELIST: the list of nodes of this partition in this state.

### srun

Run a parallel job on cluster managed by Slurm. If necessary, srun will first create a resource allocation in which to run the parallel job.

```console
nicolas.greneche@frontend:~$ srun -p SMP-128c -N 2 hostname
magi166
magi167
```
We ran one task "hostname", a command that only displays the hostname of the current node and exit on the SMP-128c partition.

### sbatch

Submit a batch script to Slurm. The first step is to write a submission script. The following script runs the “hostname” command the same way as above in the srun section.

```console
nicolas.greneche@frontend:~$ cat hostname.slurm
#!/bin/bash

#SBATCH --job-name=hello
#SBATCH --output=hostname.out
#SBATCH --error=hostname.err
#SBATCH --mail-type=end
#SBATCH --mail-user=you@you.tld
#SBATCH --nodes=2
#SBATCH --partition=SMP-128c

srun hostname
```

The shebang indicates that a submission script is a bash script. However, it takes special comments prefixed with #SBATCH. These comments are parsed by sbatch command. Here are some explanations about the example script:

* --job-name: the name of the job in the queue;
* --output: the file containing standard outputs of your job;
* --error: the file containing error outputs of your job;
* --mail-type: condition to send email to the submitter;
* --mail-user: mail address of submitter;
* --nodes: number of nodes required for the job;
* --partition: name of the partition.

Then, we can use sbatch command to submit the script:

```console
nicolas.greneche@frontend:~$ sbatch hostname.slurm
Submitted batch job 9788
```

This command returns a Jobid (9788), the identifier of your job. Wen can now check the outputs:

```console
nicolas.greneche@frontend:~$ cat hostname.out
magi167
magi166
```

### squeue

The command squeue is used to view job and job step information for jobs managed by Slurm.

```console
nicolas.greneche@frontend:~$ squeue
             JOBID PARTITION     NAME     USER ST       TIME  NODES NODELIST(REASON)
              9777 COMPUTE-S mag-Zr30   xxxxxx PD       0:00      4 (Priority)
              9778 COMPUTE-S ZrCuFe54   xxxxxx PD       0:00      4 (Priority)
              9755 COMPUTE-S Ti_50K_1 xxxxxxxx  R    1:06:28      4 magi[79,96-98]
              9753 COMPUTE-S Ti_50K_8 xxxxxxxx  R    3:06:56      4 magi[88-91]
```

Columns explanation:

* JOBID: identifier of the job;
* PARTITION: name of the partition running the job (--partition);
* NAME: name of the job (--jobname);
* USER: submitter of the job;
* ST: state of the job:
    * PD: pending, job is waiting for resources;
    * R: running, job is running;
* TIME: human time elapsed;
* NODES: number of nodes involved in the job's computation;
* NODELIST(REASON): name list of nodes involved in the job's computation or reason for pending if job in PD state;

### scancel

The command scancel is used to signal or cancel jobs, job arrays or job steps.

```console
nicolas.greneche@frontend:~$ scancel 9788
```

Just invoke scancel followed by the jobid to cancel a job.

## Contact

If any problem occurs, feel free to reach us at: dsi-recherche -AT- univ-paris13 -DOT- fr





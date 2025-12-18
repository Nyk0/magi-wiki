# Uing OpenMPI on Magi

## 1. Introduction: MPI and Slurm Concepts

### What is MPI?

MPI (Message Passing Interface) is a standardized programming model designed for parallel computing on distributed-memory systems. An MPI program is typically composed of:
- **One MPI application** (a single executable)
- **Multiple MPI processes** (often called *workers* or *ranks*)

Each MPI process:
- Has its own memory space
- Is identified by a **rank** (an integer from `0` to `N-1`)
- Communicates with other processes via explicit message passing

The total number of MPI processes is usually referred to as the **world size**.

### MPI Workers vs Slurm Tasks

When running MPI on a Slurm-managed cluster, it is crucial to understand the mapping between MPI concepts and Slurm concepts:

| Concept           | MPI                | Slurm           |
|-------------------|--------------------|-----------------|
| Parallel unit     | MPI process (rank) | Task            |
| Total parallelism | World size         | --ntasks        |
| Placement         | MPI runtime        | Slurm scheduler |

> [!NOTE]
**One Slurm task = one MPI process**


Slurm is responsible for:
- Allocating nodes and CPU resources
- Spawning tasks on allocated nodes

The MPI runtime (OpenMPI):
- Discovers the Slurm allocation
- Assigns ranks to tasks
- Sets up communication channels

Because of this tight integration, **you should not manually start MPI processes with **`**mpirun**`** on a Slurm cluster**. Instead, Slurm should be used as the launcher.

## 2. Loading the OpenMPI Module

On most HPC clusters, OpenMPI is provided through the environment module system.

To load OpenMPI:

```bash
nicolas.greneche@frontend:~$ module load /softs/modules/openmpi/<arch>-<openmpi_version>-ucx-<ucx_version>
```
> [!NOTE]
UCX (Unified Communication X) is a high-performance communication framework used by OpenMPI to efficiently exploit modern interconnects such as InfiniBand, Omni-Path, and high-speed Ethernet. It provides low-latency, high-bandwidth communication and is automatically selected by OpenMPI when available.

To verify that the correct binaries are used:

```bash
nicolas.greneche@frontend:~$ which mpirun
/softs/openmpi-<arch>-<openmpi_version>-ucx-<ucx_version>/bin/mpirun

nicolas.greneche@frontend:~$ which mpicc
/softs/openmpi-<arch>-<openmpi_version>-ucx-<ucx_version>/bin/mpicc
```

You should see paths pointing to the module installation directory.

## 3. Compiling a Basic MPI Program

You can find some MPI codes [here](https://github.com/Nyk0/magi-wiki/tree/main/MPI/examples).

Compile the program using the MPI compiler wrapper:

```bash
nicolas.greneche@frontend:~/mpi-test$ mpicc -o comm_mpi_test comm_mpi_test.c
```

The MPI wrapper automatically adds the correct include paths and libraries. You can test on the frontend node:

```bash
nicolas.greneche@frontend:~/mpi-test$ mpirun -np 2 ./comm_mpi_test
=== MPI Sanity Check ===
World size : 2 processes
[P2P] RANK 0 sends "PING" to RANK 1
[P2P] RANK 0 received "PONG" from RANK 1
[BCAST] RANK 0 broadcasts value 42
[BCAST] RANK 0 received value 42
[REDUCE] sum of ranks+1 = 3 (expected = 3) to OK
[ALLREDUCE] RANK 0 sees global sum = 3 (expected = 3) → OK
[ALLGATHER] RANK 0 received : 0 1
=== End of MPI Sanity Check ===
[P2P] RANK 1 received "PING" from RANK 0
[P2P] RANK 1 sends back "PONG" to RANK 0
[BCAST] RANK 1 received value 42
[ALLREDUCE] RANK 1 sees global sum = 3 (expected = 3) → OK
[ALLGATHER] RANK 1 received : 0 1
```

> [!CAUTION]
Do not exceed 2 workers (-np 2). The frontend node is not designed for heavy computation.

## 4. Interactive Execution with `srun`

Interactive execution is useful for testing and debugging.

```bash
nicolas.greneche@frontend:~/mpi-test$ srun --partition=MISC-56c-VERYSHORT --nodes=2 --ntasks-per-node=2 ./comm_mpi_test
=== MPI Sanity Check ===
World size : 4 processes
[P2P] RANK 0 sends "PING" to RANK 1
[P2P] RANK 1 received "PING" from RANK 0
[P2P] RANK 1 sends back "PONG" to RANK 0
[P2P] RANK 0 received "PONG" from RANK 1
[BCAST] RANK 0 broadcasts value 42
[BCAST] RANK 0 received value 42
[BCAST] RANK 1 received value 42
[BCAST] RANK 2 received value 42
[BCAST] RANK 3 received value 42
[REDUCE] sum of ranks+1 = 10 (expected = 10) to OK
[ALLREDUCE] RANK 0 sees global sum = 10 (expected = 10) → OK
[ALLREDUCE] RANK 2 sees global sum = 10 (expected = 10) → OK
[ALLREDUCE] RANK 1 sees global sum = 10 (expected = 10) → OK
[ALLREDUCE] RANK 3 sees global sum = 10 (expected = 10) → OK
[ALLGATHER] RANK 0 received : 0 1 2 3
[ALLGATHER] RANK 2 received : 0 1 2 3
[ALLGATHER] RANK 1 received : 0 1 2 3
[ALLGATHER] RANK 3 received : 0 1 2 3
=== End of MPI Sanity Check ===
```

This allocates:
- 2 nodes (--nodes=2)
- 2 MPI tasks per node (--ntasks-per-node=2)
- From MISC-56c-VERYSHORT partition (--partition=MISC-56c-VERYSHORT)
- Total of **4** MPI processes

Once the allocation is active, Slurm will:
- Launch one task per MPI process
- Bind tasks to CPUs according to the cluster policy

OpenMPI will:
- Detect the Slurm environment
- Assign ranks automatically

## 5. Batch Submission Script Example

Below is a minimal Slurm submission script for an MPI job.

```bash
nicolas.greneche@frontend:~/mpi-test$ cat run.slurm
#!/bin/bash

#SBATCH --job-name=comm_mpi_test
#SBATCH --output=comm_mpi_test.out
#SBATCH --error=comm_mpi_test.err
#SBATCH --partition=MISC-56c-VERYSHORT
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2

module load /softs/modules/openmpi/<arch>-<openmpi_version>-ucx-<ucx_version>

srun ./comm_mpi_test
```

> [!NOTE]
You may recognize the ressources parameters of the previous section.

Then, submit it:

```bash
nicolas.greneche@frontend:~/mpi-test$ sbatch run.slurm
Submitted batch job 51101
nicolas.greneche@frontend:~/mpi-test$
```

And check outputs:

```bash
nicolas.greneche@frontend:~/mpi-test$ cat comm_mpi_test.out
=== MPI Sanity Check ===
World size : 4 processes
[P2P] RANK 0 sends "PING" to RANK 1
[P2P] RANK 1 received "PING" from RANK 0
[P2P] RANK 1 sends back "PONG" to RANK 0
[P2P] RANK 0 received "PONG" from RANK 1
[BCAST] RANK 0 broadcasts value 42
[BCAST] RANK 0 received value 42
[BCAST] RANK 1 received value 42
[BCAST] RANK 2 received value 42
[BCAST] RANK 3 received value 42
[REDUCE] sum of ranks+1 = 10 (expected = 10) to OK
[ALLREDUCE] RANK 0 sees global sum = 10 (expected = 10) → OK
[ALLREDUCE] RANK 1 sees global sum = 10 (expected = 10) → OK
[ALLREDUCE] RANK 2 sees global sum = 10 (expected = 10) → OK
[ALLREDUCE] RANK 3 sees global sum = 10 (expected = 10) → OK
[ALLGATHER] RANK 0 received : 0 1 2 3
[ALLGATHER] RANK 2 received : 0 1 2 3
[ALLGATHER] RANK 1 received : 0 1 2 3
[ALLGATHER] RANK 3 received : 0 1 2 3
=== End of MPI Sanity Check ===
```

### Notes
- `--ntasks` or `--ntasks-per-node` controls the number of MPI processes
- `srun` is the **recommended launcher** for OpenMPI on Slurm
- No `mpirun` or `mpiexec` is needed (and should generally be avoided)

## Caveats

### Working with nodes embedding large number of cores

You may experience errors like:

```bash
[magi173:2375229:0:2375229] mm_ep.c:458 Assertion ucs_arbiter_group_is_empty(&ep->arb_group)' failed
```

The observed crashes are caused by a UCX internal race condition in the shared-memory transports (mm/scopy), not by the application or MPI semantics themselves. Hyper-Threading is not the root cause, but it increases concurrency and therefore the likelihood of triggering this latent UCX bug, especially during collective operations that mix intra-node shared memory and inter-node InfiniBand communication. A stable mitigation is to keep UCX enabled while disabling the problematic shared-memory transports and explicitly selecting safe transports (e.g. UCX_TLS=self,rc,tcp), which preserves InfiniBand RDMA performance.

In a nutshell, you may add the following environement variables in your submission script:

```bash
nicolas.greneche@frontend:~/mpi-test$ cat run.slurm
#!/bin/bash

#SBATCH --job-name=comm_mpi_test
#SBATCH --output=comm_mpi_test.out
#SBATCH --error=comm_mpi_test.err
#SBATCH --partition=MISC-56c-VERYSHORT
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2

export OMPI_MCA_pml=ucx
export UCX_TLS=self,rc,tcp
unset UCX_NET_DEVICES

module load /softs/modules/openmpi/<arch>-<openmpi_version>-ucx-<ucx_version>

srun ./comm_mpi_test
```

This lets UCX selects the Infiniband card port, wich is safe on Magi. Alternatively, you can pinpoint the network interface:

```bash
nicolas.greneche@frontend:~/mpi-test$ cat run.slurm
#!/bin/bash

#SBATCH --job-name=comm_mpi_test
#SBATCH --output=comm_mpi_test.out
#SBATCH --error=comm_mpi_test.err
#SBATCH --partition=MISC-56c-VERYSHORT
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=2

export OMPI_MCA_pml=ucx
export UCX_TLS=self,rc,tcp
export UCX_NET_DEVICES=mlx5_0:1

module load /softs/modules/openmpi/<arch>-<openmpi_version>-ucx-<ucx_version>

srun ./comm_mpi_test
```

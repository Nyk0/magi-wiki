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

### Example MPI Program (C)

Create a file named `hello_mpi.c`:
#include <mpi.h>#include <stdio.h>int main(int argc, char **argv){    int rank, size;    MPI_Init(&argc, &argv);    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    MPI_Comm_size(MPI_COMM_WORLD, &size);    printf("Hello from rank %d out of %d\n", rank, size);    MPI_Finalize();    return 0;}
### Compilation

Compile the program using the MPI compiler wrapper:
mpicc hello_mpi.c -o hello_mpi
The MPI wrapper automatically adds the correct include paths and libraries.

## 4. Interactive Execution with `srun`

Interactive execution is useful for testing and debugging.

### Request an Interactive Allocation
salloc --nodes=2 --ntasks-per-node=4 --partition=compute
This allocates:
- 
2 nodes

- 
4 MPI tasks per node

- 
Total of 8 MPI processes

### Run the MPI Program

Once the allocation is active:
srun ./hello_mpi
Slurm will:
- 
Launch one task per MPI process

- 
Bind tasks to CPUs according to the cluster policy

OpenMPI will:
- 
Detect the Slurm environment

- 
Assign ranks automatically

You should see output similar to:
Hello from rank 0 out of 8Hello from rank 1 out of 8...
## 5. Batch Submission Script Example

Below is a minimal Slurm submission script for an MPI job.
#!/bin/bash#SBATCH --job-name=mpi-hello#SBATCH --nodes=2#SBATCH --ntasks-per-node=4#SBATCH --time=00:05:00#SBATCH --partition=compute#SBATCH --output=mpi-%j.out#SBATCH --error=mpi-%j.errmodule purgemodule load openmpi/5.0.0srun ./hello_mpi
### Submission

Submit the job using:
sbatch mpi_job.sh
### Notes
- 
`--ntasks` or `--ntasks-per-node` controls the number of MPI processes

- 
`srun` is the **recommended launcher** for OpenMPI on Slurm

- 
No `mpirun` or `mpiexec` is needed (and should generally be avoided)

## 6. Summary
- 
MPI programs consist of multiple parallel processes (workers/ranks)

- 
Slurm tasks map directly to MPI processes

- 
Slurm handles resource allocation and process launching

- 
OpenMPI integrates natively with Slurm

- 
Use `srun` for both interactive and batch MPI execution

This model ensures correct placement, scalability, and accounting on the cluster.

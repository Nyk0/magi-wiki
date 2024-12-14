# Using GPU on Magi

First, you must load cuda module. This will add environment variable to access cuda compiler and librairies:

```console
nicolas.greneche@frontend:~$ module load /softs/modules/cuda/XX_version
```

Then, you can compile your cuda code:

```console
nicolas.greneche@frontend:~$ nvcc -o simple simple.cu
```

This program only displays allocated GPU cards. Let's book 2 GPU cards:

```console
nicolas.greneche@frontend:~/recettes/cuda$ srun --partition=GPU-A100 --gres=gpu:2 ./simple
Device Number: 0
  Device name: NVIDIA A100 80GB PCIe
  Memory Clock Rate (KHz): 1512000
  Memory Bus Width (bits): 5120
  Peak Memory Bandwidth (GB/s): 1935.360000

Device Number: 1
  Device name: NVIDIA A100 80GB PCIe
  Memory Clock Rate (KHz): 1512000
  Memory Bus Width (bits): 5120
  Peak Memory Bandwidth (GB/s): 1935.360000
```

In this command, we use 2 arguments with srun:
* --partition=GPU-A100: we run the job on GPU capable partition;
* --gres=gpu:2: we book 2 GPU cards.

We can write a sumbmission script:

```console
nicolas.greneche@frontend:~$ cat gpu.slurm
#!/bin/bash
#SBATCH --job-name=gpu
#SBATCH --output=gpu.out
#SBATCH --error=gpu.err
#SBATCH --gres=gpu:2
#SBATCH --partition=GPU-A100

module load /softs/modules/cuda/XX_version

srun ./simple
```

And submit it:

```console
nicolas.greneche@frontend:~$ sbatch gpu.slurm
Submitted batch job 9790
```

And check the outputs:

```console
nicolas.greneche@frontend:~$ cat gpu.out
Device Number: 0
  Device name: NVIDIA A100 80GB PCIe
  Memory Clock Rate (KHz): 1512000
  Memory Bus Width (bits): 5120
  Peak Memory Bandwidth (GB/s): 1935.360000

Device Number: 1
  Device name: NVIDIA A100 80GB PCIe
  Memory Clock Rate (KHz): 1512000
  Memory Bus Width (bits): 5120
  Peak Memory Bandwidth (GB/s): 1935.360000

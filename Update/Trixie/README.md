# Testing Procedure for the New Magi Environment Based on Debian Trixie (13)

This document explains how to test the new version of Magi based on Debian Trixie (13).

To begin, connect to the temporary frontend of the new environment. You can disable strict host key checking for this connection so that you do not run into SSH fingerprint issues later when the new node replaces the current one:

```bash
ssh -o StrictHostKeyChecking=no -p 2822 <login>@magi-tmp.univ-paris13.fr
```

## 1. Make Sure You Are Using the New SLURM Instance

The first step is to verify that you are connected to the new SLURM environment.

Check which module is currently loaded:

```bash
nicolas.greneche@frontend:~$ module list
Currently Loaded Modulefiles:
 1) /softs/modules/slurm/default
```

In this example, the loaded module is still the old SLURM module (`/softs/modules/slurm/default`). For the purpose of testing, unload it:

```bash
nicolas.greneche@frontend:~$ module unload /softs/modules/slurm/default
```

Then verify that no SLURM module remains loaded:

```bash
nicolas.greneche@frontend:~$ module list
No Modulefiles Currently Loaded.
```

If you have other modules loaded, you can unload them in the same way.

Now load the new SLURM module:

```bash
nicolas.greneche@frontend:~$ module load /softs/trixie/modules/x86-64/slurm/default
```

Check again:

```bash
nicolas.greneche@frontend:~$ module list
Currently Loaded Modulefiles:
 1) /softs/trixie/modules/x86-64/slurm/default
```

You can also verify which `srun` binary is being used:

```bash
nicolas.greneche@frontend:~$ which srun
/softs/trixie/x86-64/slurm/slurm-latest/bin/srun
```

## 2. New Software and Module Layout

As you can see, the software and module layout has changed compared to the previous version of Magi.

The CPU architecture is now explicitly included in software paths so that multiple builds of the same application can coexist for different processor families.

This change reflects the way Magi has been funded and expanded over time: hardware has been acquired incrementally, resulting in a fairly heterogeneous infrastructure.

To make this easier to handle, Magi now provides an environment variable set by the SLURM job prolog that indicates the CPU architecture associated with the selected partition.

For example:

```bash
nicolas.greneche@frontend:~$ srun -p COMPUTE /bin/bash -lc 'echo "$MAGI_CPU_ARCH"'
haswell
```

```bash
nicolas.greneche@frontend:~$ srun -p FORMATION /bin/bash -lc 'echo "$MAGI_CPU_ARCH"'
cascadelake
```

This variable allows you to write more generic job submission scripts without hardcoding the target architecture.

## 3. InfiniBand Device Information

The InfiniBand device name is also passed directly into the user environment through the UCX-related environment variable:

```bash
nicolas.greneche@frontend:~$ srun -p FORMATION /bin/bash -lc 'echo "$UCX_NET_DEVICES"'
mlx4_0:1
```

## 4. Software Paths

You can now explore the software tree available on Magi.

The standard software path format is:

```bash
/softs/<distrib>/<arch>/<software>/<version>
```

For example:

```bash
/softs/trixie/cascadelake/vasp/6.5.1
```

This path refers to VASP version 6.5.1 built for Intel Cascadelake on Debian Trixie.

This is exactly why `MAGI_CPU_ARCH` is useful: it allows your submission scripts to remain generic while still selecting the appropriate software build for the target partition.

## 5. Module Paths

Modules follow a similar layout:

```bash
/softs/<distrib>/modules/<arch>/<software>/<version>
```

For example:

```bash
/softs/trixie/modules/znver2/vasp/6.5.1
```

This module corresponds to VASP version 6.5.1 for Debian Trixie, compiled for AMD znver2.

## 6. GPU Testing

Please note that GPU testing is not available at this stage.

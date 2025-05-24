# Using Netlib Blas, Lapack and ScaLapack on Magi

The Blas, Lapack, and Scalapack libraries are available on Magi, compiled for each of the processor architectures available on the different partitions. You'll find them in /softs with the following naming scheme:

````{verbatim}
<library>-<architecture>-<version>[-suffix]
````

For instance, Lapack version 3.12.1 compiled for haswell (-march=haswell) is *lapack-haswell-3.12.1*.

With scalapack, the MPI compilation chain is also provided. For instance *scalapack-haswell-2.2.2-openmpi-5.0.3-ucx-1.16.0* means that it’s scalapack compiled for Intel haswell architecture version 2.2.2. The suffix *openmpi-5.0.3-ucx-1.16.0* means that openmpi version 5.0.3 compiled with ucx-1.16.0 was used.

Feel free to explore /softs to see which Lapack and Scalapack are available.

> [!NOTE]
> You can find which partition runs which CPU architecture [here](https://github.com/Nyk0/magi-wiki/blob/main/README.md#welcome-to-magi).

> [!NOTE]
>In the following examples, you can notice that *-Wl,-t* are used in gcc. It's only to enlight that gcc used our custom instance of Netlib librairies.

## Compile and execute a matrix multiplication (C = A * B) with custom Blas

```console
nicolas.greneche@frontend:~$ gcc blas.c -L/softs/lapack-haswell-3.12.1/lib \
      -lblas -lm -lgfortran -o blas -Wl,-t 2>&1 | grep blas
/softs/lapack-haswell-3.12.1/lib/libblas.a
```

```console
nicolas.greneche@frontend:~$ ./blas
Result matrix C:
23.000000 34.000000
31.000000 46.000000
```

## Compile and execute solving Ax = b (using dgesv) with custom Lapack

```console
nicolas.greneche@frontend:~$ gcc lapack.c -L/softs/lapack-cascadelake-3.12.1/lib \
      \textcolor{black}{-llapack -lblas -lm -lgfortran -o lapack -Wl,-t 2>&1 | grep lapack}
/softs/lapack-cascadelake-3.12.1/lib/liblapack.a
/softs/lapack-cascadelake-3.12.1/lib/libblas.a
```

```console
nicolas.greneche@frontend:~$ ./lapack
Solution x:
2.200000
3.600000
```

## Compile and execute distributed Ax = b (using pdgesv) with custom ScaLapack

```console
nicolas.greneche@frontend:~$ module load /softs/modules/openmpi/cascadelake-5.0.3-ucx-1.16.0
```

```console
nicolas.greneche@frontend:~$ mpicc scalapack.c \
      -L/softs/scalapack-cascadelake-2.2.2-openmpi-5.0.3-ucx-1.16.0/lib -lscalapack \
      -L/softs/lapack-cascadelake-3.12.1/lib -llapack -lblas -lm -lgfortran \
      -o scalapack -Wl,-t 2>&1 | grep lapack
/softs/scalapack-cascadelake-2.2.2-openmpi-5.0.3-ucx-1.16.0/lib/libscalapack.a
/softs/lapack-cascadelake-3.12.1/lib/liblapack.a
/softs/lapack-cascadelake-3.12.1/lib/libblas.a
```

```console
nicolas.greneche@frontend:~$ mpirun -n 1 ./scalapack
Solution x:
x[0] = 2.200000
x[1] = 3.600000
```

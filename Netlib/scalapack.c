#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// BLACS & ScaLAPACK symbols (Fortran routines)
extern void blacs_pinfo_(int *mypnum, int *nprocs);
extern void blacs_get_(int *icontxt, int *what, int *val);
extern void blacs_gridinit_(int *icontxt, const char *order, int *nprow, int *npcol);
extern void blacs_gridinfo_(int *icontxt, int *nprow, int *npcol, int *myrow, int *mycol);
extern void blacs_gridexit_(int *icontxt);
extern void blacs_exit_(int *error_code);
extern void descinit_(int *desc, const int *m, const int *n, const int *mb, const int *nb,
                      const int *irsrc, const int *icsrc, const int *ictxt, const int *lld, int *info);

extern void pdgesv_(int *n, int *nrhs,
                    double *A, int *ia, int *ja, int *descA, int *ipiv,
                    double *B, int *ib, int *jb, int *descB, int *info);

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int myid, nprocs;
    blacs_pinfo_(&myid, &nprocs);

    int ictxt;
    blacs_get_(&ictxt, &ictxt, &ictxt);

    int nprow = 1, npcol = 1;
    blacs_gridinit_(&ictxt, "Row", &nprow, &npcol);

    int myrow, mycol;
    blacs_gridinfo_(&ictxt, &nprow, &npcol, &myrow, &mycol);

    // Matrix size
    int n = 2, nrhs = 1;
    int nb = 2; // block size
    int ia = 1, ja = 1, ib = 1, jb = 1;
    int lld = n;
    int descA[9], descB[9], info;

    // Matrix A (column-major)
    double A[4] = {2.0, 1.0,
                   1.0, 3.0};
    double B[2] = {8.0, 13.0};
    int ipiv[2];

    // Initialize descriptors
    descinit_(descA, &n, &n, &nb, &nb, &myrow, &mycol, &ictxt, &lld, &info);
    descinit_(descB, &n, &nrhs, &nb, &nb, &myrow, &mycol, &ictxt, &lld, &info);

    // Solve A * X = B
    pdgesv_(&n, &nrhs, A, &ia, &ja, descA, ipiv,
            B, &ib, &jb, descB, &info);

    if (myid == 0) {
        if (info == 0) {
            printf("Solution x:\n");
            for (int i = 0; i < n; ++i) {
                printf("x[%d] = %f\n", i, B[i]);
            }
        } else {
            printf("pdgesv failed with info = %d\n", info);
        }
    }

    blacs_gridexit_(&ictxt);
    int zero = 0;
    blacs_exit_(&zero);

    MPI_Finalize();
    return 0;
}

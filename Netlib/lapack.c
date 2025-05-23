#include <stdio.h>

extern void dgesv_(const int *n, const int *nrhs, double *A, const int *lda,
                   int *ipiv, double *B, const int *ldb, int *info);

int main() {
    int n = 2, nrhs = 1, lda = 2, ldb = 2, info;
    double A[4] = {2, 1,
                   1, 3};  // Column-major
    double B[2] = {8, 13};
    int ipiv[2];

    dgesv_(&n, &nrhs, A, &lda, ipiv, B, &ldb, &info);

    if (info == 0) {
        printf("Solution x:\n");
        for (int i = 0; i < n; i++) {
            printf("%f\n", B[i]);
        }
    } else {
        printf("dgesv failed with info = %d\n", info);
    }

    return 0;
}

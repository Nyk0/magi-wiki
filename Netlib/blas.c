#include <stdio.h>

extern void dgemm_(const char *transa, const char *transb, const int *m, const int *n,
                   const int *k, const double *alpha, const double *A, const int *lda,
                   const double *B, const int *ldb, const double *beta,
                   double *C, const int *ldc);

int main() {
    int m = 2, n = 2, k = 2;
    double A[4] = {1, 2,
                   3, 4};
    double B[4] = {5, 6,
                   7, 8};
    double C[4] = {0, 0,
                   0, 0};
    double alpha = 1.0, beta = 0.0;

    dgemm_("N", "N", &m, &n, &k, &alpha, A, &m, B, &k, &beta, C, &m);

    printf("Result matrix C:\n");
    for (int i = 0; i < 4; i++) {
        printf("%f ", C[i]);
        if ((i + 1) % 2 == 0) printf("\n");
    }
    return 0;
}

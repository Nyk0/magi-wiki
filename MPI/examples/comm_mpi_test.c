#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void check(int err, const char *where) {
    if (err != MPI_SUCCESS) {
        char errstr[MPI_MAX_ERROR_STRING];
        int len = 0;
        MPI_Error_string(err, errstr, &len);
        fprintf(stderr, "[MPI ERROR] at %s: %s\n", where, errstr);
        MPI_Abort(MPI_COMM_WORLD, err);
    }
}

int main(int argc, char *argv[]) {
    int err = MPI_Init(&argc, &argv);
    check(err, "MPI_Init");

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("=== MPI Sanity Check ===\n");
        printf("World size : %d processes\n", size);
        fflush(stdout);
    }

    /* ----------------------------------------------------
     * 1) Point to point communication (ping-pong 0 <-> 1)
     * ---------------------------------------------------- */
    if (size >= 2) {
        char msg[64];
        MPI_Status status;

        if (rank == 0) {
            strcpy(msg, "PING");
            printf("[P2P] RANK 0 envoie \"%s\" → RANK 1\n", msg);
            fflush(stdout);
            err = MPI_Send(msg, (int)strlen(msg) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            check(err, "MPI_Send ping");

            err = MPI_Recv(msg, 64, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
            check(err, "MPI_Recv pong");
            printf("[P2P] RANK 0 a reçu \"%s\" de RANK 1\n", msg);
            fflush(stdout);
        } else if (rank == 1) {
            err = MPI_Recv(msg, 64, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
            check(err, "MPI_Recv ping");
            printf("[P2P] RANK 1 received \"%s\" from RANK 0\n", msg);
            fflush(stdout);

            strcpy(msg, "PONG");
            printf("[P2P] RANK 1 sends back \"%s\" to RANK 0\n", msg);
            fflush(stdout);
            err = MPI_Send(msg, (int)strlen(msg) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            check(err, "MPI_Send pong");
        }
    } else if (rank == 0) {
        printf("[WARN] P2P test sauté (size < 2)\n");
    }
  
    MPI_Barrier(MPI_COMM_WORLD);

    /* ----------------------------------------------------
     * 2) Bcast test : rank 0 broadcasts a value to all
     * ---------------------------------------------------- */
    int bcast_value = 0;
    if (rank == 0) {
        bcast_value = 42;
        printf("[BCAST] RANK 0 broadcasts value %d\n", bcast_value);
        fflush(stdout);
    }

    err = MPI_Bcast(&bcast_value, 1, MPI_INT, 0, MPI_COMM_WORLD);
    check(err, "MPI_Bcast");

    printf("[BCAST] RANK %d received value %d\n", rank, bcast_value);
    fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);

    /* ----------------------------------------------------
     * 3) Test Reduce (add) to rank 0
     * ---------------------------------------------------- */
    int reduce_send = rank + 1;   // par ex. 1,2,3,...,size
    int reduce_sum  = 0;

    err = MPI_Reduce(&reduce_send, &reduce_sum, 1, MPI_INT,
                     MPI_SUM, 0, MPI_COMM_WORLD);
    check(err, "MPI_Reduce");

    if (rank == 0) {
        int expected = size * (size + 1) / 2;
        printf("[REDUCE] sum of ranks+1 = %d (expected = %d) to %s\n",
               reduce_sum, expected,
               (reduce_sum == expected) ? "OK" : "MISMATCH");
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    /* ----------------------------------------------------
     * 4) Test Allreduce (global sum)
     * ---------------------------------------------------- */
    int allreduce_send = rank + 1;
    int allreduce_sum  = 0;

    err = MPI_Allreduce(&allreduce_send, &allreduce_sum, 1, MPI_INT,
                        MPI_SUM, MPI_COMM_WORLD);
    check(err, "MPI_Allreduce");

    int expected = size * (size + 1) / 2;
    printf("[ALLREDUCE] RANK %d sees global sum = %d (expected = %d) → %s\n",
           rank, allreduce_sum, expected,
           (allreduce_sum == expected) ? "OK" : "MISMATCH");
    fflush(stdout);

    MPI_Barrier(MPI_COMM_WORLD);

    /* ----------------------------------------------------
     * 5) Test Allgather : each rank sends a value
     *    and receives the whole array
     * ---------------------------------------------------- */
    int allgather_send = rank;
    int *allgather_recv = malloc(size * sizeof(int));
    if (!allgather_recv) {
        fprintf(stderr, "[RANK %d] Error malloc\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    err = MPI_Allgather(&allgather_send, 1, MPI_INT,
                        allgather_recv, 1, MPI_INT,
                        MPI_COMM_WORLD);
    check(err, "MPI_Allgather");

    printf("[ALLGATHER] RANK %d received : ", rank);
    for (int i = 0; i < size; i++) {
        printf("%d ", allgather_recv[i]);
    }
    printf("\n");
    fflush(stdout);

    free(allgather_recv);

    if (rank == 0) {
        printf("=== End of MPI Sanity Check ===\n");
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}

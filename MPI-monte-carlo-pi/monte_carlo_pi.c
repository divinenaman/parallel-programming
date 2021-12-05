#include <stdio.h>
#include <stdlib.h>
#include "/home/zorin/openMPI/include/mpi.h"
#include <math.h>
#define SEED 35791246
#define N 10000

int main(int argc, char *argv[])
{
    int rank, size;
    long num, num_per_p, sum = 0;
    double x, y, pi;
    MPI_Init(&argc, &argv);               //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(SEED + size);

    if (rank == 0)
    {
        num = atoi(argv[1]);
        MPI_Bcast(&num, 1, MPI_LONG, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&num, 1,
                 MPI_LONG, MPI_ANY_SOURCE, 0,
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    num_per_p = num / size;
    long sum_p = 0;
    for (long i = 0; i < num_per_p; i++)
    {
        x = (rand() * RAND_MAX) / (double)RAND_MAX;
        y = (rand() * RAND_MAX) / (double)RAND_MAX;
        if ((x * x + y * y) <= 1.0)
            sum_p++;
    }

    MPI_Reduce(&sum_p, &sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        pi = (4 * sum) / (double)num;
        printf("PI VALUE: %.16f\n", pi);
    }

    MPI_Finalize();
    return 0;
}
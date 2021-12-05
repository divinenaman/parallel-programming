#include <stdio.h>
#include <stdlib.h>
#include "/home/zorin/openMPI/include/mpi.h"
#include <math.h>
#define N 10

int checkPrime(int n) {
    int isPrime = 0;
    for(int i = 2; i <= n/2; i++) {
        isPrime = isPrime + (n%i == 0);
    }
    if (isPrime == 0) return 1;
    else return 0;
}

int main(int argc, char *argv[])
{
    int rank, size;
    
    MPI_Init(&argc, &argv);               //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        int num = 2, r = 0, isPrime, choice = 1, total = 0;
        int primes[N];
        while(total < N) {
            r = (r+1)%size;
            if (r != 0) {
                MPI_Send(&choice, 1, MPI_INT, r, 1, MPI_COMM_WORLD);
                MPI_Send(&num, 1, MPI_INT, r, 0, MPI_COMM_WORLD);
                MPI_Recv(&isPrime, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (isPrime) primes[total++] = num;
                num++;
            }
        }
        choice = 0;
        for(int i = 1; i < rank; i++) {
            MPI_Send(&choice, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }

        for (int i = 0; i < N; i++) {
            printf("%d\n",primes[i]);
        }

        MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);

    } else {
        int num, isPrime, choice;
        do {
            MPI_Recv(&choice, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (choice) {
                MPI_Recv(&num, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                isPrime = checkPrime(num);
                MPI_Send(&isPrime, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        } while(choice);
    }

    MPI_Finalize();
    return 0;
}    
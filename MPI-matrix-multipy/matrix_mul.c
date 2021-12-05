#include <stdio.h>
#include <stdlib.h>
#include "/home/zorin/openMPI/include/mpi.h"
#include <math.h>
#define N 10

int main(int argc, char *argv[])
{
    int rank, size;
    int A[N][N], B[N][N], C[N][N];
    MPI_Init(&argc, &argv);               //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(1);

    if (rank == 0 && size > 1) {
        int num = 2, r = 0, isPrime, choice = 1, total = 0, i, j, k;
        int primes[N];
        
        for(i = 0; i < N; i++) {
            for(j = 0; j < N; j++) {
                A[i][j] = 3*(j+1);
                B[i][j] = 4*(j+1);
            }
        }

        for (i = 0; i < N; i++ ) {
            int temp[N];
            for (j = 0; j < N; j++) {
                for (k = 0; k < N; k++) {
                    temp[k] = B[k][j];     
                }
                int ans;
                do {
                    r = (r+1)%size;
                } while(r==0);
                MPI_Send(&A[i][0], N, MPI_INT, r, 0, MPI_COMM_WORLD);
                MPI_Send(temp, N, MPI_INT, r, 1, MPI_COMM_WORLD);
                MPI_Recv(&ans, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                C[i][j] = ans;
            }
        }

        for (int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                printf("%d\t",C[i][j]);
            }
            printf("\n");
        }

        MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);

    } else if(size > 1) {
        int t1[N], t2[N];
        do {
            MPI_Recv(t1, N, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(t2, N, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int ans = 0;
            for (int i = 0; i < N; i ++) {
                ans += t1[i]*t2[i];    
            }
            MPI_Send(&ans, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } while(1);
    }

    MPI_Finalize();
    return 0;
}    
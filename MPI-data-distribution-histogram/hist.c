#include <stdio.h>
#include <stdlib.h>
#include "/home/zorin/openMPI/include/mpi.h"
#include <math.h>
#define N 10

double find_max(double *A, int n) {
    double m = 0;
    for(int i = 0; i < n; i++) {
        if(*(A+i) > m) m = *(A+i);
    }
    return m;
}

int main(int argc, char *argv[])
{
    double A[N];

    int rank, size, i, j;
    double maxx;

    MPI_Init(&argc, &argv);               //Start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(1);

    if (rank == 0) {
        for(i = 0; i < N; i++) {
            A[i] = rand()%1000/(double)100.21;
        }
        int r = 0;
        maxx = find_max(A, N);
        double bin_size = 1;
        
        for(double k = maxx; k >= bin_size; k-=bin_size) {
            int isInRange = 0;
            double low = k-bin_size;
            printf("%.3f-%.3f |",low, k);
            for(j = 0; j < N; j++) {
                
                do {
                    r = (r+1)%size;
                } while(r==0);

                MPI_Send(&low, 1, MPI_DOUBLE, r, 0, MPI_COMM_WORLD);
                MPI_Send(&k, 1, MPI_DOUBLE, r, 0, MPI_COMM_WORLD);
                MPI_Send(&A[j], 1, MPI_DOUBLE, r, 0, MPI_COMM_WORLD);
                MPI_Recv(&isInRange, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                if (isInRange) printf("* ");
                else printf("  ");
            }
            printf("\n");            
        }
        printf("Array        ");    
        for(j = 0; j < N; j++) {
            printf("%0.1f,",A[j]);
        }
        printf("\n");

        MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);        
    } else {
        double high, low, num;
        int isInRange;
        while(1) {
            MPI_Recv(&low, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&high, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);    
            MPI_Recv(&num, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 

            if (num <= high && num >= low) isInRange = 1;
            else isInRange = 0;

            MPI_Send(&isInRange, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
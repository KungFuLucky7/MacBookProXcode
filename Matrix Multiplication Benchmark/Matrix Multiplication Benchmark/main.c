//
//  main.c
//  Matrix Multiplication Benchmark
//
//  created by Terry Chun Wong on 2/28/14.
//  copyright () 2014 San Francisco State University. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NMIN 50
#define NMAX 500
#define STEP 50

double **a, **b, **c;

double millisec(void)
{ return clock()/(double) (CLOCKS_PER_SEC/1000); } // Run time

void allocateMatrix(int n) {
    // Allocate memory for matrices
    a = malloc(n * sizeof(*a));
    b = malloc(n * sizeof(*b));
    c = malloc(n * sizeof(*c));
    for (int i = 0; i < n; i++) {
        a[i]=malloc(n * sizeof(*(a[i])));
        b[i]=malloc(n * sizeof(*(b[i])));
        c[i]=malloc(n * sizeof(*(c[i])));
    }
}

void deleteMatrix(int n) {
    // De-Allocate memory to prevent memory leak
    for (int i = 0; i < n; i++) {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }
    free(a);
    free(b);
    free(c);
}

/*
 starttime = millisec();
 for(m=0; m<M; m++) // Repeat M times
 for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
 for(j=0; j<n; j++)
 for(k=0; k<n; k++)
 c[i][j] += a[i][k]*b[k][j]; // Remove comment to make
 // c[i][j] += a[i][k]*b[j][k]; // 8 permutations
 // c[i][j] += a[k][i]*b[k][j];
 // c[i][j] += a[k][i]*b[j][k];
 // c[j][i] += a[i][k]*b[k][j];
 // c[j][i] += a[i][k]*b[j][k];
 // c[j][i] += a[k][i]*b[k][j];
 // c[j][i] += a[k][i]*b[j][k];
 stoptime = millisec();
 RUN_TIME[row][0] = (stoptime-starttime)/M;
 */

int main(void)
{
    int i, j, k, M, m, n, nmin, nmax, step, row, v;
    double starttime, stoptime;
    double Tijk[8], Tjik[8], Tikj[8], Tkij[8], Tjki[8], Tkji[8];
    
    nmin = NMIN;
    nmax = NMAX;
    step = STEP;
    printf("nmin: %d\tnmax: %d\tstep: %d\n", nmin, nmax, step);
    
    n = nmin;
    // Initialize a[][] and b[][] // Typical size from 100*100 to 500*500
    allocateMatrix(n);
    
    // Define arbitrary initial values of matrices a[][] and b[][]
    for(i=0; i<n; i++) {
        for(j=0; j<n; j++) {
            a[i][j] = rand();
            b[i][j] = rand();
        }
    }
    
    for(n=nmin; n<=nmax; n+=step)
    {
        printf("==================================================================================\n");
        printf("n=%-7d\t%-7s\t\t%-7s\t\t%-7s\t\t%-7s\t\t%-7s\t\t%-7s\n", n, "Tijk", "Tjik", "Tikj", "Tkij", "Tjki", "Tji");
        printf("==================================================================================\n");
        allocateMatrix(n);
        M =(nmax*nmax*nmax)/(n*n*n);
        for(i=0; i<n; i++) // Matrix [][] initialization
            for(j=0; j<n; j++) c[i][j]=0.0;
        row = 0;
        
        /* Permutation 1: c[i][j] += a[i][k]*b[k][j]; */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        c[i][j] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
     
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        c[i][j] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        c[i][j] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        c[i][j] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        c[i][j] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        c[i][j] += a[i][k]*b[k][j]; // Remove comment to make
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        row++;
        
        /* Permutation 2: c[i][j] += a[i][k]*b[j][k]; */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        c[i][j] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        c[i][j] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        c[i][j] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        c[i][j] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        c[i][j] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        c[i][j] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        row++;
        
        /* Permutation 3: c[i][j] += a[k][i]*b[k][j]; */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        c[i][j] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        c[i][j] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        c[i][j] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        c[i][j] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        c[i][j] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        c[i][j] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        row++;
        
        /* Permutation 4: c[i][j] += a[k][i]*b[j][k]; */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        c[i][j] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        c[i][j] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        c[i][j] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        c[i][j] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        c[i][j] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        c[i][j] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        row++;
        
        /* Permutation 5: c[j][i] += a[i][k]*b[k][j]; */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                       c[j][i] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                       c[j][i] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                       c[j][i] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                       c[j][i] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                       c[j][i] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                       c[j][i] += a[i][k]*b[k][j];
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        row++;
        
        /* Permutation 6: c[j][i] += a[i][k]*b[j][k]; */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                       c[j][i] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                       c[j][i] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                       c[j][i] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                       c[j][i] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                       c[j][i] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                       c[j][i] += a[i][k]*b[j][k];
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        row++;
        
        /* Permutation 7: c[j][i] += a[k][i]*b[k][j]; */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                       c[j][i] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                       c[j][i] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                       c[j][i] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                       c[j][i] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                       c[j][i] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                       c[j][i] += a[k][i]*b[k][j];
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        row++;
        
        /* Permutation 8: c[j][i] += a[k][i]*b[j][k]; */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                       c[j][i] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                       c[j][i] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                       c[j][i] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                       c[j][i] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                       c[j][i] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                       c[j][i] += a[k][i]*b[j][k];
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = c[n-1][n-1]; // Verification
        
        for(i=0; i<8; i++) {
            printf("perm: %-5d\t%-7f\t%-7f\t%-7f\t%-7f\t%-7f\t%-7f\n", i+1, Tijk[i], Tjik[i], Tikj[i], Tkij[i], Tjki[i], Tkji[i]);
        }
        deleteMatrix(n);
    }
    
    printf("End of program!\n");
    
    return 0;
}


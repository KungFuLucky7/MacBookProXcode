//
//  main.c
//  Matrix Multiplication Benchmark Pointers
//
//  Created by Terry Chun Wong on 3/6/14.
//  Copyright (c) 2014 San Francisco State University. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NMIN 50
#define NMAX 500
#define STEP 50
#define PERMUTATIONS 48

double *a, *b, *c;

double millisec(void)
{ return clock()/(double) (CLOCKS_PER_SEC/1000); } // Run time

// Find the mininum of runtimes
double min(double *RunTime) {
    double min = RunTime[0];
    int n = PERMUTATIONS;
    
    for (int i = 1; i < n; i++) {
        if (RunTime[i] < min)
            min = RunTime[i];
    }
    
    return min;
}

// Find the maximum of runtimes
double max(double *RunTime) {
    double max = RunTime[0];
    int n = PERMUTATIONS;
    
    for (int i = 1; i < n; i++) {
        if (RunTime[i] > max)
            max = RunTime[i];
    }
    
    return max;
}

// Find the geometric mean of R(n)s
double geometric_mean(double *R, int n) {
    double Rave, product=R[0];
    
    for (int i = 1; i < n; i++)
        product *= R[i];
    Rave = pow((product),(1./n));
    
    return Rave;
}

void allocateMatrix(int n) {
    // Allocate memory for matrices
    a = malloc(n*n * sizeof(a));
    b = malloc(n*n * sizeof(b));
    c = malloc(n*n * sizeof(c));
}

void deleteMatrix(int n) {
    // De-Allocate memory to prevent memory leak
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
 *(c+i*n+j) += *(a+i*n+k) * *(b+k*n+j); // Remove comment to make
 // *(c+i*n+j) += *(a+i*n+k) * *(b+j*n+k); // 8 permutations
 // *(c+i*n+j) += *(a+k*n+i) * *(b+k*n+j);
 // *(c+i*n+j) += *(a+k*n+i) * *(b+j*n+k);
 // *(c+j*n+i) += *(a+i*n+k) * *(b+k*n+j);
 // *(c+j*n+i) += *(a+i*n+k) * *(b+j*n+k);
 // *(c+j*n+i) += *(a+k*n+i) * *(b+k*n+j);
 // *(c+j*n+i) += *(a+k*n+i) * *(b+j*n+k);
 stoptime = millisec();
 RUN_TIME[row][0] = (stoptime-starttime)/M;
 */

int main(void)
{
    int i, j, k, M, m, n, nmin, nmax, step, row, index;
    double starttime, stoptime, Tmin, Tmax, *R, Q, Rave, Qave, v;
    double Tijk[8], Tjik[8], Tikj[8], Tkij[8], Tjki[8], Tkji[8], RUN_TIME[PERMUTATIONS];
    
    nmin = NMIN;
    nmax = NMAX;
    step = STEP;
    printf("nmin: %d\tnmax: %d\tstep: %d\n", nmin, nmax, step);
    
    n = nmin;
    R = malloc((nmax/nmin) * sizeof(*R));
    index = 0;
    for(n=nmin; n<=nmax; n+=step)
    {
        printf("==================================================================================\n");
        printf("n=%-7d\t%-7s\t\t%-7s\t\t%-7s\t\t%-7s\t\t%-7s\t\t%-7s\n", n, "Tijk", "Tjik", "Tikj", "Tkij", "Tjki", "Tkji");
        printf("==================================================================================\n");
        // Initialize a[][] and b[][] // Typical size from 100*100 to 500*500
        allocateMatrix(n);
        
        // Define arbitrary initial values of matrices a[][] and b[][]
        for(i=0; i<n; i++) {
            for(j=0; j<n; j++) {
                *(a+i*n+j) = rand();
                *(b+i*n+j) = rand();
            }
        }
        M =(nmax*nmax*nmax)/(n*n*n);
        for(i=0; i<n; i++) // Matrix [][] initialization
            for(j=0; j<n; j++) *(c+i*n+j)=0.0;
        row = 0;
        
        /* Permutation 1: *(c+i*n+j) += *(a+i*n+k) * *(b+k*n+j); */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        *(c+i*n+j)+= *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+k*n+j); // Remove comment to make
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        row++;
        
        /* Permutation 2: *(c+i*n+j) += *(a+i*n+k) * *(b+j*n+k); */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        *(c+i*n+j) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        row++;
        
        /* Permutation 3: *(c+i*n+j) += *(a+k*n+i) * *(b+k*n+j); */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        row++;
        
        /* Permutation 4: *(c+i*n+j) += *(a+k*n+i) * *(b+j*n+k); */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        *(c+i*n+j) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        row++;
        
        /* Permutation 5: *(c+j*n+i) += *(a+i*n+k) * *(b+k*n+j); */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+k*n+j);
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        row++;
        
        /* Permutation 6: *(c+j*n+i) += *(a+i*n+k) * *(b+j*n+k); */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        *(c+j*n+i) += *(a+i*n+k) * *(b+j*n+k);
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        row++;
        
        /* Permutation 7: *(c+j*n+i) += *(a+k*n+i) * *(b+k*n+j); */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+k*n+j);
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        row++;
        
        /* Permutation 8: *(c+j*n+i) += *(a+k*n+i) * *(b+j*n+k); */
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // Basic i-j-k form (first of 6 permutations)
                for(j=0; j<n; j++)
                    for(k=0; k<n; k++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tijk[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-i-k form
                for(i=0; i<n; i++)
                    for(k=0; k<n; k++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tjik[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(i=0; i<n; i++) // i-k-j form
                for(k=0; k<n; k++)
                    for(j=0; j<n; j++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tikj[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // k-i-j form
                for(i=0; i<n; i++)
                    for(j=0; j<n; j++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tkij[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(j=0; j<n; j++) // j-k-i form
                for(k=0; k<n; k++)
                    for(i=0; i<n; i++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tjki[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        starttime = millisec();
        for(m=0; m<M; m++) // Repeat M times
            for(k=0; k<n; k++) // Final k-j-i form (last of 6 permutations)
                for(j=0; j<n; j++)
                    for(i=0; i<n; i++)
                        *(c+j*n+i) += *(a+k*n+i) * *(b+j*n+k);
        stoptime = millisec();
        Tkji[row] = (stoptime-starttime)/M;
        v = *(c+(n-1)*(n-1)); // Verification
        
        for(i=0; i<8; i++) {
            printf("perm: %-5d\t%-7f\t%-7f\t%-7f\t%-7f\t%-7f\t%-7f\n", i+1, Tijk[i], Tjik[i], Tikj[i], Tkij[i], Tjki[i], Tkji[i]);
        }
        
        i = 0;
        for (j=0; j<8; j++)
            RUN_TIME[i++] = Tijk[j];
        for (j=0; j<8; j++)
            RUN_TIME[i++] = Tjik[j];
        for (j=0; j<8; j++)
            RUN_TIME[i++] = Tikj[j];
        for (j=0; j<8; j++)
            RUN_TIME[i++] = Tkij[j];
        for (j=0; j<8; j++)
            RUN_TIME[i++] = Tjki[j];
        for (j=0; j<8; j++)
            RUN_TIME[i++] = Tkji[j];
        
        Tmin = min(RUN_TIME);
        Tmax = max(RUN_TIME);
        R[index] = Tmax/Tmin;
        Q = 100 * (R[index]-1)/(R[index]+1);
        printf("\nTmin: %f ms\tTmax: %f ms\tR(n): %f\tQ(n): %f%%\n", Tmin, Tmax, R[index], Q);
        index++;
        deleteMatrix(n);
    }
    
    printf("\n**********************************************************************************\n");
    Rave = geometric_mean(R, index);
    Qave = 100 * (Rave-1)/(Rave+1);
    printf("\nRave: %f\tQave: %f%%\n", Rave, Qave);
    printf("End of the Matrix Multiplication Benchmark program!\n");
    
    return 0;
}


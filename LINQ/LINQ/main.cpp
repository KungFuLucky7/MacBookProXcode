//
//  main.cpp
//  LINQ
//
//  Created by Terry Chun Wong on 3/26/14.
//  Copyright (c) 2014 San Francisco State University. All rights reserved.
//  Matrix inversion reference:
//  http://www.programming-techniques.com/2011/09/numerical-methods-inverse-of-nxn-matrix.html
//

#include <iostream>
using namespace std;

#define MMAX 2000

double** LINQ(double **m1) {
    int i, j, k, n;
    float ratio, a;
    
    n = MMAX/2;
    for(i = 0; i < n; i++){
        for(j = n; j < 2*n; j++){
            if(i==(j-n))
                m1[i][j] = 1.0;
            else
                m1[i][j] = 0.0;
        }
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(i!=j){
                ratio = m1[j][i]/m1[i][i];
                for(k = 0; k < 2*n; k++){
                    m1[j][k] -= ratio * m1[i][k];
                }
            }
        }
    }
    for(i = 0; i < n; i++){
        a = m1[i][i];
        for(j = 0; j < 2*n; j++){
            m1[i][j] /= a;
        }
    }
    for(i = 0; i < n; i++){
        for(j = n, k = 0; j < 2*n; j++, k++)
            m1[i][k] = m1[i][j];
    }
    
    /*printf("\nThe inverted matrix is: \n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%.4f", m1[i][j]);
            printf("\t");
        }
        printf("\n");
    }*/
    
    return m1;
}

int main() {
    int i, j, n;
    double **m1, **m2;
    
    m1 = new double*[MMAX];
    for(int i = 0; i < MMAX; ++i)
        m1[i] = new double[MMAX*2];
    
    //printf("\nThe original matrix is: \n");
    n = MMAX/2;
    for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			if (i != j)
			{
				m1[i][j] = 1.0001f;
			}
			else {
				m1[i][j] = 2.0001f;
			}
			//printf("%.4f", m1[i][j]);
            //printf("\t");
		}
		//printf("\n");
	}

    // Invert the matrix twice to get the original
    m2 = LINQ(m1);
    m2 = LINQ(m2);
    //printf("\nThe original matrix is: \n");
    for(i = 0; i < n; i++){
        for(j = n; j < 2*n; j++){
            //printf("%.4f", m2[i][j]);
            //printf("\t");
        }
        //printf("\n");
    }
    
    printf("End of program!\n");
    
    return 0;
}


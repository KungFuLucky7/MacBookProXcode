//
//  main.cpp
//  QSORT
//
//  Created by Terry Chun Wong on 3/26/14.
//  Copyright (c) 2014 San Francisco State University. All rights reserved.
//  Quicksort reference:
//  http://p2p.wrox.com/
//

#include <iostream>
using namespace std;

#define MMAX 5000000
int m[MMAX];

// Quicksort function
void QSORT(int a[], int left, int right)
{
	int pivot, l_saved, r_saved;
	
	l_saved = left;
	r_saved = right;
	pivot = a[left];
	while (left < right)
	{
		while ((a[right] >= pivot) && (left < right))
			right--;
		if (left != right)
		{
			a[left] = a[right];
			left++;
		}
		while ((a[left] <= pivot) && (left < right))
			left++;
		if (left != right)
		{
			a[right] = a[left];
			right--;
		}
	}
	a[left] = pivot;
	pivot = left;
	left = l_saved;
	right = r_saved;
	if (left < pivot)
		QSORT(a, left, pivot-1);
	if (right > pivot)
		QSORT(a, pivot+1, right);
}

int main()
{
    srand((unsigned)time(NULL));
    for(int i = 0; i < MMAX; i++) {
        m[i] = rand();
    }
    QSORT(m, 0, MMAX-1);

    printf("End of program!\n");
    
    return 0;
}


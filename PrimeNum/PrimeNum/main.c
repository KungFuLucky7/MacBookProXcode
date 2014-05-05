//
//  main.c
//  PrimeNum
//
//  Created by Terry Chun Wong on 5/4/14.
//  Copyright (c) 2014 San Francisco State University. All rights reserved.
//

#include <stdio.h>
typedef int bool;
enum { false, true };

int main(int argc, const char * argv[])
{
    int i = 0, n = 0, counter = 0, number = 2;
    bool isPrime = true;
    
    while (n <= 0) {
        printf("Please enter the number of sequencial prime numbers you want to generate:\n");
        scanf("%d", &n);
    }

    while (counter < n) {
        isPrime = true;
        for (i = 2; i*i <= number; i++) {
            if (number % i == 0)
                isPrime = false;
        }
        if (isPrime) {
            printf("%d\n", number);
            counter++;
        }
        number++;
    }
    
    return 0;
}


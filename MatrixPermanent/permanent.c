/**
 * File: permanent.c
 * Author: Benjamin Mayes <bdm8233@rit.edu>
 * Description: 
 *  Input: Given input on standard in in the following format:
 *      #                   (a number representing the number of rows and columns of a matrix)
 *      # # # ... #         (n integers representing the first row of the matrix (first of n rows))
 *      .
 *      .
 *      .
 *      # # # ... #         (a total of n-1 additional rows of n numbers)
 *  
 *   Output: The permanent of the given matrix.
 *
 * Things of note that this program contains:
 * - Lexicographic permuation generation.
 *
 * Possible extensions:
 * - Accept NxM matrices and use permutations of size min(N,M).
 *  - This counts the number of maximum matchings of a bipartite graph with
 *    N vertices in partition and M in the other.
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int calc_perm(int** matrix, int n, int* perm) {
    int i,k,l; // iteration/index variables
    int sum = 0; // the permanent itself

    do {
        // caculate the product for the current permutation
        int prod = 1;
        for( i = 0; i < n && prod > 0; ++i ) {
            prod *= matrix[i][perm[i]];
        }
        sum += prod;

        // generate the next permutation
        // 1. find largest k s.t.: perm[k] < perm[k+1]
        k = -1;
        for( i = 0; i < n-1; ++i ) {
            if(perm[i] < perm[i+1]) {
                k = i;
            }
        }
        // non-negative means such a k, and therefore another permutation
        // exists...
        if(k >= 0) {
            // 2. find largest l s.t. perm[k] < perm[l]
            int l = k + 1;
            for(i = l + 1; i < n; ++i){
                if( perm[k] < perm[i]) {
                    l = i;
                }
            }
            // 3. swap perm[k] and perm[l]
            int tmp = perm[k];
            perm[k] = perm[l];
            perm[l] = tmp; 
            int first = k + 1;
            // 4. reverse perm[k+1]..perm[n-1].
            for( i = first; i < (n-first)/2 + first; ++i ) {
                int tmp = perm[n-(i-first) - 1];
                perm[n-(i-first) - 1] = perm[i];
                perm[i] = tmp;
            }
        } else {
            // no more permutations exist.
            break;
        }
    } while(1);

    return sum;
}

int main() {
    int i,j; // iteration variables
    int n; // number of rows/columns
    int** mat; // an nxn matrix

    // read in the number of rows and columns of the matrix
    scanf("%d", &n);
    mat = malloc(n*(sizeof(int*)));
    
    // read in the NxN elements
    for( i = 0; i < n; ++i ) {
        mat[i] = malloc(n*sizeof(int));
        for( j = 0; j < n; ++j ) {
            scanf("%d",&mat[i][j]);
        }
    }

    // calculate the initial permutation (0, 1, 2, ..., n - 1)
    int perm[n];
    for( i = 0; i < n; ++i ) {
        perm[i] = i;
    }

    // calculate the permanent
    int permanent = calc_perm(mat, n, perm);
    printf("Permanent: %d\n", permanent);

    // cleanup memory
    for( i = 0; i < n; ++i ) {
        free(mat[i]);
    }
    free(mat);
}

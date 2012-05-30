#include <stdio.h>
#include <strings.h>
int select( int* nums, int size, int k );
int bubblesort( int* nums, int size );
void partition( int* nums, int size, int pivot_index, int j[2] );

int main() {
    // determine the number of input elements to read
    int n;
    scanf( "%d", &n );

    // read in the elements from standard input
    int nums[n];
    for( int i = 0; i < n; ++i ) {
        scanf( "%d", &nums[i]);
    }
    
    // mhalf is the median of the numbers
    int mhalf = select( nums, n, n/2 );

    // mthird and mtwothird are the "medians" that occur at the first and
    // second third positions of the array
    int mthird = select( nums, n, n/3 );
    int mtwothird = select( nums, n, 2*n/3 );

    // count corresponding to number of mhalf elements fount
    int c1 = 0;
    // counts corresponding to number of mthird and mtwothird elements found
    int c2 = 0;
    int c3 = 0;

    // count the incidence of mhalf, mthird, and mtwothird elements
    for( int i = 0; i < n; ++i ) {
        int num = nums[i];
        if( num == mhalf ) {
            ++c1;
        }
        if( num == mthird ) {
            ++c2;
        }
        if( num == mtwothird ) {
            ++c3;
        }
    }

    // if the count of the median element is > n/2 then it is a majority
    // element
    if( c1 > n/2 ) {
        printf( "YES\n" );
    } else {
        printf( "NO\n" );
    }

    // if the count of the third and two-third "median" elements are greater
    // than n/3 then we want to print "YES"
    if( c2 > n/3 || c3 > n/3 ) {
        printf( "YES\n" );
    } else {
        printf( "NO\n" );
    }
   
#ifdef DEBUG
    for( int i = 0; i < n; ++i ) {
        printf( "%d ", nums[i] );
    }
    printf( "\n" );
#endif
    return 0;
}

int select( int* nums, int size, int k ) {
    // base case for select to prevent infinite recursion
    if( size == 1 ) {
        return nums[0];
    }

    // size of B is n/5 + 1 if 5 does not divide n evenly
    int bsize = size/5;
    if( size % 5 ) {
        bsize++;
    }
    
    // obtain the medians of the n/5 groups
    int b[bsize];
    int i;
    for( i = 0; i < size / 5; ++i ) {
        b[i] = bubblesort( &nums[5*i], 5 );
    }
    // if there is a remaining group then find the median of that
    if( i < bsize ) {
        b[i] = bubblesort( &nums[5*i], size % 5 );
    }

    // obtain the median of B
    int pivot = select( b, bsize, bsize / 2 );
    
    // find the first index of the pivot
    int pivot_index = 0;
    for( i = 0; i < size; ++i ) {
        if( pivot == nums[i] ) {
            pivot_index = i;
            break;
        }
    }
    // perform an in-place partitioning of the set with the selected pivot.
    int j[2];
    partition( nums, size, pivot_index, j );
 
    // recurse to a smaller chunk of the array to find k
    if( k < j[0] ) {
        return select( nums, j[0], k );
    } else if( k >= j[0] && k <= j[1] ) {
        return pivot;
    } else if( k > j[1] ) {
        return select( &nums[j[1]+1], size-(j[1]+1), k - j[1] - 1 );
    }
    return 0;
}

// simple swap routine
inline int swap( int* i1, int* i2 ) {
    int tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}

// modified bubblesort that returns the median for convenience
int bubblesort( int* nums, int size ) {
    for( int i = 0; i < size - 1; ++i ) {
        for( int j = 0; j < size - 1; ++j ) {
            if( nums[j] > nums[j+1] ) {
                swap( &nums[j], &nums[j+1] );
            }
        }
    }

    // return the median (greater element if list is of even size)
    return (nums[size/2]);
}

// performs an in-place partition on the given array
void partition( int* nums, int size, int pivot_index, int j[2] ) {
    // the index of the first and last pivot in the pivot subarray
    int j1 = size-1;
    int j2 = size-1;
    
    // move pivot to the end of the list
    int pivot = nums[pivot_index];
    swap( &nums[j1], &nums[pivot_index] );

    // iterate through the entire array, keeping elements < pivot in place,
    // and moving elements >= to the pivot to their correct position
    for( int i = 0; i < j1 ; ++i ) {
        // element > pivot should be to the right of the pivot
        if( nums[i] > pivot ) {
            // swap last pivot with element to the left of first pivot
            swap( &nums[j2], &nums[j1-1] );
            if( j1 - i  > 1 ) {
                // if the first swap didn't move the element to its spot
                // then move the correct element to its spot.
                swap( &nums[i], &nums[j2] );
            }

            // the window of the pivot elements moved left 1 position
            --j2;
            --j1;
            // the current index now contains an unchecked element
            --i;
        } else if( nums[i] == pivot ) {
            // elements equal to the pivot should be placed to the left
            // of the first pivot
            swap( &nums[i], &nums[j1-1] );
            // the window of pivot elements grew to the left by 1
            --j1;
            // the current index now contains an unchecked element
            --i;
        }
    }
    // "return" the first and last pivot indices
    j[0] = j1;
    j[1] = j2;
}

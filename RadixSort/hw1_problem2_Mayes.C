#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef     struct _node    node;

struct _node {
    unsigned int num;
    node* next;
};


int main() {
    // Read in the number of elements
    unsigned int n, i, j;
    scanf( "%u", &n );

    // initialize needed data structures
    node* bucket1[10];
    node* bucket2[10];
    node** buckets[2] = { bucket1, bucket2 };
    node* current_ends[10];
    // clear out the data structures 
    memset( bucket1, 0, sizeof(bucket1) );
    memset( bucket2, 0, sizeof(bucket2) );
    memset( current_ends, 0, sizeof(current_ends) );

    // Read in the elements
    node list[n];
    int max_len = 0;
    for( i = 0; i < n; ++i ) {
        node** current_buckets = buckets[0];
        scanf( "%u", &(list[i].num) );
        int len = log10(list[i].num); 
        if( len > max_len ) max_len = len;
        list[i].next = 0;
        
        // add the number to the bucket
        int digit = list[i].num % 10;
        if( current_ends[digit] ) {
            // end of the current bucket
            current_ends[digit]->next = &list[i];
            current_ends[digit] = &list[i];
        } else {
            // beginning of current bucket
            current_buckets[digit] = &list[i];
            current_ends[digit] = &list[i];
        }
    }

    // iterate for each digit and perform the radix sort
    for( i = 1; i <= max_len; ++i ) {
        node** current_buckets = buckets[i%2];
        node** prev_buckets = buckets[(i-1)%2];
        memset( current_ends, 0, sizeof(current_ends) );
        // for each possible base 10 digit iterate over the nodes in that
        // bucket
        for( j = 0; j < 10; ++j ) {
            node* m = prev_buckets[j];
            while( m ) {
                // extract the digit
                int power = pow(10,i);
                int digit = (m->num/power) % 10;
                // place the digit at the end of the linked list if there
                // are elements in the list or at the beginning if not.
                if( current_ends[digit] ) {
                    current_ends[digit]->next = m;
                    current_ends[digit] = m;
                } else {
                    current_buckets[digit] = m;
                    current_ends[digit] = m;
                }
                node* next = m->next;
                m->next = 0;
                m = next;
            }
        } 
    }

    // iterate through the sorted bucket and print the sorted integer
    for( i = 0; i < 10; ++i ) {
        // we only want to check the bucket if there is a last element
        // in that given bucket
        node* m = current_ends[i] ? buckets[max_len%2][i] : 0;
        while( m ) {
            printf( "%d ", m->num );
            m = m->next;
        }
    }
    printf( "\n" );

}

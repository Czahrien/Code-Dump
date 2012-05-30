// Author: Benjamin Mayes

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// the type of element that the heart consists of
typedef struct _heart_entry heart_entry;
struct _heart_entry {
    unsigned int h;
    int paths;
};
// the adjacency matrix
unsigned int** adjacency_matrix;

//adds an edge to the adjacency matrix
void new_edge( int u, int v, int w ) {
    adjacency_matrix[u][v] = w;
}

int main() {
    // read in the number of vertices and edges
    int n, m;
    scanf("%d %d", &n, &m );

    //create the adjacency matrix
    adjacency_matrix = (unsigned int**)malloc((n+1)*sizeof(unsigned int*));
    for( int i = 0; i <= n; ++i ) {
        adjacency_matrix[i] = (unsigned int*)calloc((n+1),sizeof(unsigned int));
    }
    int u,v,w;
    for( int i = 0; i < m; ++i ) {
        scanf( "%d %d %d", &u, &v, &w );
        new_edge( u, v, w );
    }

    // fill in the remaining parts of the adjacency matrix with
    // a value meanining "INFINITY"
    for( int i = 1; i <= n; ++i ) {
        for( int j = 1; j <= n; ++j ) {
            if( !adjacency_matrix[i][j] ) {
                adjacency_matrix[i][j] = INT_MAX;
            }
        }
    }

    // create the 3d heart
    heart_entry*** heart = (heart_entry***)calloc((n+1),sizeof(heart_entry**));
    for( int i = 0; i <= n; ++i ) {
        heart[i] = (heart_entry**)calloc((n+1),sizeof(heart_entry*));
        for( int j = 0; j <= n; ++j ) {
            heart[i][j] = (heart_entry*)calloc((n+1),sizeof(heart_entry));
        }
    }

    // fill in the first level of the heart
    for( int i = 1; i <= n; ++i ) {
        for( int j = 1; j <= n; ++j ) {
            heart[i][j][0].h = adjacency_matrix[i][j];
            if( heart[i][j][0].h < INT_MAX ){
                heart[i][j][0].paths = 1;
            }
        }
    }

    // The modified Floyd-Warshall
    for( int k = 1; k <= n; ++k ) {
        for( int i = 1; i <= n; ++i ) {
            for( int j = 1; j <= n; ++j ) {
                if( i != j ) {
                    unsigned int if_not_k = heart[i][j][k-1].h;
                    // the number of paths if k is not chosen does not change
                    unsigned int if_not_k_path = heart[i][j][k-1].paths;
                    unsigned int if_k = heart[i][k][k-1].h + heart[k][j][k-1].h;
                    // choosing k means we want to multiply the number of 
                    // paths of the two subpaths
                    unsigned int if_k_path = heart[i][k][k-1].paths * heart[k][j][k-1].paths;
                    // handle infinity cases
                    if( if_not_k > INT_MAX ) {
                        if_not_k = INT_MAX;   
                        if_not_k_path = 0;
                    }
                    if ( if_k > INT_MAX ) {
                        if_k = INT_MAX;
                        if_k_path = 0;
                    }
                    // when equal, we want to add the number of paths
                    if( if_k == if_not_k ) {
                        heart[i][j][k].h = if_k;
                        // we want to combine the number of paths
                        heart[i][j][k].paths = if_not_k_path + if_k_path;
                    // choosing k is better than not choosing k
                    } else if( if_k < if_not_k ) {
                        heart[i][j][k].h = if_k;
                        heart[i][j][k].paths = if_k_path;
                    // it is better to not choose k
                    } else {
                        heart[i][j][k].h = if_not_k;
                        heart[i][j][k].paths = if_not_k_path;
                    }
                } 
            }
        }
    }

    // print out the table of distances and path counts
    for( int i = 1; i <= n; ++i ) {
        for( int j = 1; j <= n; ++j ) {
            if( heart[i][j][n].h < INT_MAX ) {
                if( i != j ) {
                    printf( "%d/%d ", heart[i][j][n].h, heart[i][j][n].paths );
                } else {
                    printf( "0/1 " );
                }
            } else {
                printf( "inf/0 ");
            }
        }
        printf( "\n" );
    }

    //cleanup memory
    for( int i = 0; i <= n; ++i ) {
        for( int j = 0; j <= n; ++j ) {
            free(heart[i][j]);
        }
        free(heart[i]);
        free(adjacency_matrix[i]);
    }
    free(heart);
    free(adjacency_matrix);
    return 0;
}

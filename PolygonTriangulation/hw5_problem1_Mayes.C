// Author: Benjamin Mayes
// Algorithms HW #5 Problem 1 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))

// a data structure to represent a point in space
struct _point {
    double x;
    double y;
};
typedef struct _point point;

// a simple squaring algorithm
double square( double d ) {
    return d*d;
}

// a euclidean distance algorithm
double distance( point* p1, point* p2 ) {
    return sqrt( square(p1->x - p2->x) + square(p1->y - p2->y) );
}

int main() {
    // read in values from standard input
    int n;
    scanf("%d",&n);
    point* points = (point*)malloc(n*sizeof(point));
    for( int i = 0; i < n; ++i ) {
        scanf("%lf %lf", &(points[i].x), &(points[i].y) );
    }
    
    // convenience array storing the distance between all two points
    double** distances = (double**)malloc(n*sizeof(double));
    for( int i = 0; i < n; ++i ) {
        distances[i] = (double*)malloc(n*sizeof(double));
        for( int j = 0; j < n; ++j ) {
            distances[i][j] = distance(&points[i], &points[j]);
        }
    }

    // obtain the perimeter of the figure
    double perimeter = distances[n-1][0];
    for( int i = 0; i < n-1; ++i ) {
        perimeter += distances[i][i+1];
    }

    // create the 2D heart
    // We have n vertices, the table will have (n-1)*(n-1) vertices
    // however, since 1 indexing is easier for this algorithm we will still
    // allocate n^2 space.
    double** heart = (double**)malloc(n*sizeof(double*));
    for( int i = 0; i < n; ++i ) {
        heart[i] = (double*)calloc(n,sizeof(double));
    }

    // fill in the heart of the algorithm
    int m = n - 1;
    for( int d = 1; d <= m; ++d ) {
        for( int L = 1; L <= m - d; ++L ) {
            int R = L+d;
            heart[L][R] = DBL_MAX;

            // find the minimum weight of two subpolygons with the addition of 
            // a new triangle to connect them
            for( int k = L; k <= R - 1; ++k ) {
                double tmp = heart[L][k] + heart[k+1][R]
                    + distances[(L-1)][k]
                    + distances[k][R]
                    + distances[(L-1)][R];
                if( heart[L][R] > tmp ) heart[L][R] = tmp;
            }
        }
    }
    
    #ifdef DEBUG
    for( int i = 1; i < n; ++i ) {
        for( int j = 1; j < n; ++j ) {
            printf( "%f ", heart[i][j] );
        }
        printf( "\n" );
    }
    #endif

    // what we have is the sum of all the perimeters of the triangles that make
    // up the triangulation. We want to subtract out the perimeter of the
    // polygon and divide the remaining quantity by two since all the shared
    // edges of the triangle were double counted by the algorithm.
    printf( "%d\n", (int)((heart[1][m]-perimeter)/2));

    //cleanup allocated memory
    for( int i = 0; i < n; ++i ) {
        free(distances[i]);
        free(heart[i]);
    }
    free(distances);
    free(heart);
    free(points);

    return 0;
}

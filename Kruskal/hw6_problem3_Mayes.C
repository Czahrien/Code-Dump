#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mergesort.h"
typedef struct _edge edge;
typedef struct _vertex vertex;

// a structure representing an edge
struct _edge {
    int u;
    int v;
    double w;
    int f;
};

// a structure representing a vertex
struct _vertex {
    int boss;
    int size;
    int* set;
};

// an edge comparison function for the mergesort routine
int edge_cmp( const void* v1, const void* v2 ) {
    edge* e1 = (edge*)v1;
    edge* e2 = (edge*)v2;
    // e1 is less than e2 if:
    // e1 is in f or 
    // e2 is not in f and e1 has a weight less than or equal to e2's weight
    return e1->f || !e2->f && e1->w <= e2->w;
}

// merges two sets for the union-find data structure
void merge( vertex* v1, vertex* v2 ) {
    int* newset = (int*)malloc((v1->size+v2->size)*sizeof(int));
    int i1 = 0;
    int i2 = 0;
    int si = 0;
    while( i1 < v1->size && i2 < v2->size ) {
        if( v1->set[i1] <= v1->set[i2] ) {
            newset[si++] = v1->set[i1++];
        } else {
            newset[si++] = v2->set[i2++];
        }
    }
    while( i1 < v1->size ) {
        newset[si++] = v1->set[i1++];
    }
    while( i2 < v2->size ) {
        newset[si++] = v2->set[i2++];
    }
    v1->size += v2->size;
    free(v1->set);
    v1->set = newset;
}

// takes the union of the sets of vertices u and v
void unionize( vertex* vertices, int n, int u, int v ) {
    int boss_v = vertices[v].boss;
    int boss_u = vertices[u].boss;
    //we want to select the larger set of the sets of the bosses of u and v 
    if( vertices[boss_u].size > vertices[boss_v].size ) {
        // union the two sets
        merge( &vertices[boss_u], &vertices[boss_v] );
        // set the bosses of the elements of the set[boss[v]] to boss[u]
        for( int i = 0; i < vertices[boss_v].size; ++i ) {
            vertices[vertices[boss_v].set[i]].boss = vertices[u].boss;
        }
    } else {
        // merge the two sets
        merge( &vertices[boss_v], &vertices[boss_u] );
        // set the bosses of the elements of the set[boss[u]] to boss[v]
        for( int i = 0; i < vertices[boss_u].size; ++i ) {
            vertices[vertices[boss_u].set[i]].boss = vertices[v].boss;
        }
        
    }
}

// finds the set that vertex v belongs to
int find( vertex* vertices, int v ) {
    return vertices[v].boss;
}

int main() {
    // read in the number of vertices and edges
    int n, m;
    scanf( "%d %d", &n, &m );

    // initialize the union-find data structure
    vertex* vertices = (vertex*)calloc(n,sizeof(vertex));
    for( int i = 0; i < n; ++i ) {
        vertices[i].boss = i;
        vertices[i].size = 1;
        vertices[i].set = (int*)malloc(sizeof(int));
        vertices[i].set[0] = i;
    }

    // read in and initialize the edge list
    edge* edges = (edge*)calloc(m, sizeof(edge));
    int* T = (int*)calloc(m, sizeof(int));
    for( int i = 0; i < m; ++i ) {
        edge* e = &edges[i];
        scanf( "%d %d %lf %d", &e->u, &e->v, &e->w, &e->f );
    }

    // mergesort the edges
    mrgsrt(edges, m, sizeof(edge), edge_cmp);

#ifdef DEBUG
    for( int i = 0; i < m; ++i ) {
        printf( "%d<->%d: %f (%d)\n", edges[i].u, edges[i].v, 
                edges[i].w, edges[i].f );
    }
#endif

    int tindex = 0;
    // start kruskal's algorithm
    for( int i = 0; i < m; ++i ) {
        edge* e = &edges[i];
        // check to see if the addition of the current edge produces a cycle
        if( find(vertices,e->u) == find(vertices,e->v) ) {
            if( e->f ) {
                //if this edge is in f than we have a problem...
                printf( "-1\n" );
                break;
            }
        } else {
            // add the edge to the chosen edges list
            T[tindex++] = i;
            unionize(vertices,n,e->u,e->v);
        }
    }

    if( tindex == n - 1 ) {
        // determine the total weight of all the added edges
        double weight = 0;
        for( int i = 0; i < tindex; ++i ) {
            weight += edges[T[i]].w;
        }   
        // print the weight
        printf( "%d\n", (int)weight );
    }
end:
    // cleanup memory!
    for( int i = 0; i < n; ++i ) {
        free(vertices[i].set);
    }
    free(vertices);
    free(edges);
    free(T);
    return 0;
}

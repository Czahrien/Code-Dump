// Author: Benjamin Mayes

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// the vertex and edge data structures
typedef struct _vertex vertex;
typedef struct _edge edge;
struct _vertex {
    int weight;  // the weight of a given vertex
    int in_h;    // whether or not the vertex is (or was) in h
    edge* first; // the first edge of the adjacency list
    edge* last;  // the last edge of the adjacency list
};
struct _edge {
    int v; // the destination of the directed edge
    int w; // the weight of the directed edge
    edge* next; // the next edge in the adjacency list
};

// the graph itself
vertex* the_graph;

// adds edge (u,v) to the graph with the weight of vertex v
void new_edge( int u, int v ) {
    vertex* src = &the_graph[u];
    edge* e = (edge*)malloc(sizeof(edge));
    e->v = v;
    e->w = the_graph[v].weight;
    e->next = 0;
    if( src->first ) {
       src->last->next = e;
       src->last = e;
    } else {
        src->first = e;
        src->last = e;
    }
}

int main() {
    // read in # of vertices and # of edges
    int n, m;
    scanf("%d %d", &n, &m );
    the_graph = (vertex*)calloc(n+1,sizeof(vertex));

    // read in the vertex weights
    for( int i = 1; i <=n; ++i ) {
        scanf( "%d", &(the_graph[i].weight) );
    }

    // read in the edges and add them to the graph
    int u,v;
    for( int i = 0; i < m; ++i ) {
        scanf( "%d %d", &u, &v );
        new_edge( u, v );
        new_edge( v, u );
    }

    // read in the source vertex
    int source;
    scanf( "%d", &source );

    // create the h and distance arrays
    int* h = (int*)calloc(n+1,sizeof(int));
    int h_length = 0;
    unsigned int* dist = (unsigned int*)malloc((n+1)*sizeof(unsigned int));
    for( int i = 0; i <=n; ++i ) {
        dist[i] = UINT_MAX;
    }
    dist[source] = 0;
    
    //Dijkstra's Algorithm!
    for( int i = 0; i < n; ++i ) {
        int min_vertex;
        if( i ) {
            // on all iterations but the first we take an element from
            // h
            int min_index = 1;
            min_vertex = h[1];
            // extract the minimum vertex from h
            for( int j = 1; j <= h_length; ++j ) {
                if( dist[h[j]] < dist[min_vertex] ) {
                    min_vertex = h[j];
                    min_index = j;
                }
            }
            for( int j = min_index; j < h_length; ++j ) {
                h[j] = h[j+1];
            }
            --h_length;
        } else {
            // on the first iteration we want to start with the source
            min_vertex = source;
        }

        //for each of the minimum distance vertex's neighbors we want to
        //update the distance and add to h if it is not already in h.
        edge* n = the_graph[min_vertex].first;
        while( n ) {
            if( dist[n->v] > dist[min_vertex] + n->w ) {
                dist[n->v] = dist[min_vertex] + n->w;
            }
            if(!the_graph[n->v].in_h) {
                the_graph[n->v].in_h = 1;
                h[++h_length] = n->v;
            }
            n = n->next;
        }
    }

    //print out the path weights with the source vertex's weight added in
    //to obtain the desired weights
    for( int i = 1; i <= n; ++i ) {
        printf( "%d ", dist[i]+the_graph[source].weight ); 
    }
    printf( "\n" );

    // clean up memory
    free(h);
    free(dist);
    for( int i = 0; i <= n; ++i ) {
        edge* e = the_graph[i].first;
        while( e ) {
            edge* next = e->next;
            free(e);
            e = next;
        }
    }
    free(the_graph);
    return 0;
}

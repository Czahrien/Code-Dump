//Author: Benjamin Mayes
//Algorithms HW #5 Problem 2

#include <stdio.h>
#include <stdlib.h>

// link_nodes are nodes used for a linked-list
typedef struct _link_node link_node;
struct _link_node {
    int v;
    link_node* next;
};

// graph_vertices wrap a graph vertex and contains a linked list of all the
// neighbors of a current graph
typedef struct _graph_vertex graph_vertex;
struct _graph_vertex {
    int marked;
    link_node* first;
    link_node* last;
};

// constructs a link_node on the heap wrapping a graph vertex
link_node* new_node( int v ) {
    link_node* n = (link_node*)calloc(1,sizeof(link_node));
    if( !n ) {
        perror( "malloc" );
        exit(1);
    }
    n->v = v;
    return n;
}

void add_to_linked_list( graph_vertex* g, link_node* l ) {
    if( g->first ) {
        g->last->next = l;
        g->last = l;
    } else {
        g->first = l;
        g->last = l;
    }
}
int* topo_sort = 0;

void dfs( graph_vertex* nodes, int n ) {
    static int topo_index = 0;
    nodes[n].marked = 1;
    link_node* neighbors = nodes[n].first;
    while( neighbors ) {
        if( !nodes[neighbors->v].marked ) {
            dfs( nodes, neighbors->v );
        }
        neighbors = neighbors->next;
    }
    topo_sort[topo_index++] = n;
}

int main() {
    int n, m;
    scanf( "%d %d", &n, &m );
    // the array of nodes to mark;
    graph_vertex* nodes = (graph_vertex*)calloc( n, sizeof(graph_vertex) );
    if( !nodes ) {
        perror( "malloc" ); 
        exit(1);
    }

    for( int i = 0; i < m; ++i ) {
        int n1, n2;
        scanf("%d %d", &n1, &n2 );
        // allocate a new node on the heap
        link_node* ln = new_node(n2);

        // add the linked nodes to the linked lists
        graph_vertex* v = &nodes[n1];
        add_to_linked_list( v, ln );
    }

    // create space for the topological sort to be stored
    topo_sort = (int*)calloc(n, sizeof(int));

    // perform a topological sort on the graph:
    for( int i = 0; i < n; ++i ) {
        // connected component!
        if( !nodes[i].marked ) {
            // do a topological sorting DFS
            dfs(nodes, i);
        } 
    }

    // construct the heart and run the dynamic algorithm
    int* heart = (int*)calloc(n,sizeof(int));
    for( int i = 0; i < n; ++i ) {
        link_node* l = nodes[topo_sort[i]].first;
        if( l ) {
            int max = 0;
            while( l ) {
                if( max <= heart[l->v] ) 
                    max = 1 + heart[l->v];
                l = l->next;
            }
            heart[topo_sort[i]] = max;
        }
    }

    // find the max value in the heart and print it out
    int max = 0;
    for( int i = 0; i < n; ++i ) {
        if( max < heart[i] ) {
            max = heart[i];
        }
    }
    printf( "%d\n", max );

    // Cleanup memory
    for( int i = 0; i < n; ++i ) {
        link_node* n = nodes[i].first;
        while( n ) {
            link_node* next = n->next;
            free(n);
            n = next;
        }
    }
    free(nodes);
    free(topo_sort);
    free(heart);
}

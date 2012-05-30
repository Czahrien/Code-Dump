//Author: Benjamin Mayes
//Algorithms HW #5 Problem 2

#include <stdio.h>
#include <stdlib.h>

// link_nodes are nodes used for a linked-list
typedef struct _link_node link_node;
struct _link_node {
    int v;
    int parent_v;
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
        // allocate nodes on the heap
        link_node* ln1 = new_node(n2);
        link_node* ln2 = new_node(n1);
        
        // add the linked nodes to the linked lists
        graph_vertex* v = &nodes[n1];
        if(v->first){
            //case 1: new node isn't first
            v->last->next = ln1;
            v->last = ln1;
        } else {
            //case 2: new node is first
            v->first = ln1;
            v->last = ln1;
        }
        v = &nodes[n2];
        if(v->first) {
            //case 1: new node isn't first
            v->last->next = ln2;
            v->last = ln2;
        } else {
            //case 2: new node is first
            v->first = ln2;
            v->last = ln2;
        }    
    }
#ifdef DEBUG
    for( int i = 0; i < n; ++i ) {
        printf("%d:", i);
        link_node* n = nodes[i].first;
        while( n ) {
            printf(" %d", n->v );
            n = n->next;
        }
        printf("\n");
    }
#endif

    for( int i = 0; i < n; ++i ) {
        // if the node is not marked then it is part of an uncounted
        // connected component!
        if( !nodes[i].marked ) {
            nodes[i].marked = 1;
            link_node* first = new_node(i);
            first->parent_v = -1;
            link_node* last = first;
            // loop while there are remaining parts to the current component
            while( first ) {
                // add all of first's neighbors that are unmarked
                link_node* n = nodes[first->v].first;
                while( n ) {
                    // do not add marked nodes
                    if( !nodes[n->v].marked ) {
                        // node is added, mark it.
                        nodes[n->v].marked = 1;
                        link_node* tmp = new_node(n->v);
                        tmp->parent_v = first->v;
                        last->next = tmp;
                        last = tmp;
                    } else {
                        if( n->v != first->parent_v )
                            nodes[n->v].marked++;
                    }
                    n = n->next;
                }
                link_node* next = first->next;
                free(first);
                first = next;
            }
        } 

    }

    int cyclic = 0;
    for( int i = 0; i < n; ++i ) {
        if( nodes[i].marked > 1 ) {
            cyclic = 1;
            break;
        }
    }

    if( cyclic ) {
        printf( "YES\n" );
    } else {
        printf( "NO\n" );
    }

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
}

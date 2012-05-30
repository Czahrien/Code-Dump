//Author: Benjamin Mayes
//Algorithms HW #5 Problem 3

#include <stdio.h>
#include <stdlib.h>

typedef struct _link_node link_node;
typedef struct _graph_vertex graph_vertex;
// a node for a linked list, stores both a row and a column of the maze
struct _link_node {
    int r;
    int c;
    link_node* next;
};

// a vertex which wraps values in the maze
struct _graph_vertex {
    int v;
    int dist; //-1 is unmarked, >= 0 is marked
    link_node* first;
    link_node* last;
};

// creates a new link_node
link_node* new_node( int r, int c ) {
    link_node* n = (link_node*)calloc(1,sizeof(link_node));
    if( !n ) {
        perror( "malloc" );
        exit(1);
    }
    n->r = r;
    n->c = c;
    return n;
}

// adds a link_node to the linked list starting at a vertex
void add_node( graph_vertex* g, link_node* l ) {
    if( g->first ) {
        g->last->next = l;
        g->last = l;
    } else {
        g->first = l;
        g->last = l;
    }
}

int main() {
    int n, m;
    scanf( "%d %d", &m, &n );
    // the array of vertices and their adjacencies;
    graph_vertex** nodes = (graph_vertex**)calloc( m+2, sizeof(graph_vertex*) );
    // create the 2nd dimension of arrays in nodes
    for( int i = 0; i < m+2; ++i ) {
        nodes[i] = (graph_vertex*)calloc( n+2, sizeof(graph_vertex));
        if( !nodes[i] ) {
            perror( "malloc" ); 
            exit(1);
        }
    }

    // remember the start location as the BFS will start from there
    int startr,startc;

    for( int i = 1; i <= m; ++i ) {
        for( int j = 1; j <= n; ++j ) {
            int num;
            scanf( "%d", &num );
            if( num == 0 || num == 1 ) {
                num = !num;
            }
            nodes[i][j].v = num;
            if( num == 2 ) {
                startr = i;
                startc = j;
            }
        }
    }

    // start at the upper left-hand corner of the graph and add
    // edges down and to the right of the current index
    for( int i = 1; i <= m; ++i ){
        for( int j = 1; j <= n; ++j ) {
            if( nodes[i][j].v ) {
                if( nodes[i+1][j].v){
                    add_node( &nodes[i][j], new_node(i+1,j) );
                    add_node( &nodes[i+1][j], new_node(i,j) );
                }
                if( nodes[i][j+1].v){
                    add_node( &nodes[i][j], new_node(i,j+1) ); 
                    add_node( &nodes[i][j+1], new_node(i,j) );
                }
            }
        }
    }
    
    link_node* first = new_node( startr, startc );
    link_node* last = first;
    //nodes[startr][startc].dist = 0;
    while( first && nodes[first->r][first->c].v != 3 ) {
        //add all of start's unmarked neighbors to the queue:
        link_node* tmp = nodes[first->r][first->c].first;
        while(tmp) {
            if( !nodes[tmp->r][tmp->c].dist ) {
                // set the distance from the start point to the previous
                // node's + 1
                nodes[tmp->r][tmp->c].dist = 
                    nodes[first->r][first->c].dist + 1;
                last->next = new_node(tmp->r,tmp->c);
                last = last->next;
            }
            tmp = tmp->next;
        }
        tmp = first->next;
        free(first);
        first = tmp;
    }

    if( first ) {
        printf( "%d\n", nodes[first->r][first->c].dist );
    } else {
        printf( "No path found.\n" );
    }
    // Cleanup memory
    while( first ) {
        link_node* tmp = first->next;
        free(first);
        first = tmp;
    }
    for( int i = 0; i < m + 2; ++i ) {
        for( int j = 0; j < n + 2; ++j ) {
            link_node* n = nodes[i][j].first;
            while( n ) {
                link_node* next = n->next;
                free(n);
                n = next;
            }
        }
        free(nodes[i]);
    }
    free(nodes);
}

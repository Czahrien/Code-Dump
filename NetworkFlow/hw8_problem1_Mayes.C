#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// typedefs
typedef struct _graph graph;
typedef struct _vertex vertex;
typedef struct _edge edge;
typedef struct _linked_list linked_list;
typedef struct _linked_node linked_node;

// a graph
struct _graph {
    int num_vertices;       // the number of vertices
    int num_edges;          // the number of edges
    int source;             // the source vertex
    int sink;               // the sink vertex
    vertex* vertices;       // vertices in the graph
    int* edge_flow;         // flow values for the edges
    edge** edges;           // master edge list
};

// a vertex
struct _vertex {
    int parent;     // parent vertex for BFS
    int distance;   // parent distance for BFS
    int e;          // edge for BFS backtracking
    int marked;     // mark for BFS
    edge* first;    // first edge
    edge* last;     // final edge
};

// an edge
struct _edge {
    int capacity;     // capacity of the edge
    int num;          // edge number
    int v;            // destination vertex
    int forward;
    edge* next;
    edge* reverse; // the reverse edge
};

// a struct representing a linked list
struct _linked_list {
    linked_node* first;
    linked_node* last;
};

// a node of a linked list
struct _linked_node {
    int vertex;
    linked_node* next;
};

// the graph EK is being performed on
graph* the_graph;

// adds an edge to the graph
void add_edge( int src, int dst ) {
    vertex* u = &(the_graph->vertices[src]);
    vertex* v = &(the_graph->vertices[dst]);    

    // add the forward edge
    edge* ef = (edge*)malloc(sizeof(edge));
    ef->capacity = 1;
    ef->forward = 1;
    ef->num = the_graph->num_edges++;
    ef->v = dst;
    ef->next = 0;
    the_graph->edges[ef->num] = ef;
    if( u->first ) {
        u->last->next = ef;
        u->last = ef;
    } else {
        u->first = ef;
        u->last = ef;
    }

    // backward edge    
    edge* er = (edge*)malloc(sizeof(edge));
    er->capacity = 0;
    er->forward = 0;
    er->num = the_graph->num_edges++;
    er->v = src;
    er->next = 0;
    the_graph->edges[er->num] = er;
    if( v->first ) {
        v->last->next = er;
        v->last = er;
    } else {
        v->first = er;
        v->last = er;
    }
    // connect the two edges;
    ef->reverse = er;
    er->reverse = ef;
}

// transforms the chessboard into a flow graph
graph* transform( int** board, int rows, int cols ) {
    the_graph = (graph*)malloc(sizeof(graph));
    the_graph->num_vertices = rows*cols+2;
    the_graph->num_edges = 1;
    the_graph->source = the_graph->num_vertices-1;
    the_graph->sink = the_graph->num_vertices-2;
    the_graph->edge_flow = (int*)calloc(6*rows*cols,sizeof(int));
    the_graph->edges = (edge**)calloc(6*rows*cols,sizeof(edge*));
    the_graph->vertices = (vertex*)calloc(the_graph->num_vertices,sizeof(vertex));
    for( int i = 0; i < rows; ++i ) {
        for( int j = 0; j < cols; ++j ) {
            // check if the current vertex can be occupied
            if( !board[i][j] ) {
                // white squares start from the upper left-hand corner
                if( (i + j) % 2 == 0 ) {
                    // white square
                    add_edge(the_graph->source, i*cols+j );
                    // connect to row above
                    if( i > 0 && !board[i-1][j] ) {
                        add_edge(i*cols + j, (i-1)*cols+j); 
                    }
                    // connect to row below
                    if( i < rows - 1 && !board[i+1][j] ) {
                        add_edge(i*cols + j, (i+1)*cols+j); 
                    }
                    // connect to row to the left
                    if( j > 0 && !board[i][j-1] ) {
                        add_edge(i*cols + j, i*cols+j-1); 
                    }
                    // connect to row to the right
                    if( j < cols - 1 && !board[i][j+1] ) {
                        add_edge(i*cols + j, i*cols+j+1); 
                    }

                } else {
                    //black square
                    add_edge( i*cols+j, the_graph->sink ); 
                }
            }
        }
    }
}

// enqueues an int in a linked list
void enqueue( linked_list* ll, int v ) {
    linked_node* n = (linked_node*)malloc(sizeof(linked_node));
    n->vertex = v;
    n->next = 0;
    if( ll->last ) {
        ll->last->next = n;
        ll->last = n;
    } else {
        ll->first = n;
        ll->last = n;
    }
}

// dequeues an int from a linked list
int dequeue(linked_list* ll ) {
    if( !ll->first ) return -1;
    int v = ll->first->vertex;
    linked_node* next = ll->first->next;
    free(ll->first);
    if( ll->first == ll->last ) {
        ll->first = 0;
        ll->last = 0;
    } else {
        ll->first = next;
    }
    return v;
}

// finds an augmenting path in the_graph, stores it in path, and returns
// the size of the path if it exists, 0 otherwise.
int bfs(graph* the_graph, int* path) {
    //reset the needed variables
    for( int i = 0; i < the_graph->num_vertices; ++i ) {
        the_graph->vertices[i].distance = 0;
        the_graph->vertices[i].parent = 0;
        the_graph->vertices[i].marked = 0;
        the_graph->vertices[i].e = 0;
    }
    // create a linked list and initialize the queue to contain the source
    linked_list ll = {0,0};
    enqueue(&ll, the_graph->source);
    int v = 0;
    // loop while there are still vertices to look at and we have not reached
    // the sink
    while( ll.first && v != the_graph->sink ) {
        v = dequeue(&ll);
        edge* e = the_graph->vertices[v].first;
        while( e ) {
            // we only want to follow and edge if it has a capacity and the
            // vertex on the other end of it is unmarked
            if( e->capacity && !the_graph->vertices[e->v].marked ) {
                // mark the vertex
                the_graph->vertices[e->v].marked = 1;
                // store information for backtracking
                the_graph->vertices[e->v].parent = v;
                the_graph->vertices[e->v].distance =
                    the_graph->vertices[v].distance + 1;
                the_graph->vertices[e->v].e = e->num;
                enqueue(&ll, e->v);
            }
            e = e->next;
        }
    }
    if( v != the_graph->sink ) {
        // no path exists
        return 0;
    } else {
        // obtain the length of the path
        int len = the_graph->vertices[v].distance;
        // reconstruct the path as a list of edges
        int i = the_graph->vertices[v].distance - 1;
        int j = v;
        while( i >= 0 ) {
            path[i] = the_graph->vertices[j].e;
            j = the_graph->vertices[j].parent;
            --i;
        }
        while( dequeue(&ll) != -1 );
        return len;
    }
}

// edmonds-karp network flow algorithm
int edmonds_karp(graph* the_graph) {
    int* path = (int*)calloc(the_graph->num_vertices,sizeof(int));
    int vertices;
    // while there is an augmenting path
    while( (vertices = bfs(the_graph,path)) ) {
        // find the minimum capacity along the path
        int d = INT_MAX;
        for( int i = 0; i < vertices; ++i ) {
            int e = path[i];
            if( the_graph->edges[e]->capacity < d ) {
                d = the_graph->edges[e]->capacity;
            }
        }
        // follow the path and apply the flow accordingly
        for( int i = 0; i < vertices; ++i ) {
            int e = path[i];
            if( the_graph->edges[e]->forward ) {
                the_graph->edge_flow[e] += d;
                the_graph->edges[e]->capacity -= d;
                the_graph->edges[e]->reverse->capacity += d;
            } else {
                the_graph->edge_flow[the_graph->edges[e]->reverse->num] -= d;
                the_graph->edges[e]->capacity += d;
                the_graph->edges[e]->reverse->capacity -= d;
            }
        }
    }
    free(path);
    // obtain the flow value by measuring the flow leaving the source
    int flow = 0;
    edge* e = the_graph->vertices[the_graph->source].first; 
    while( e ) {
        flow += the_graph->edge_flow[e->num];
        e = e->next;
    }
    return flow;
}

int main() {
    // read in the input
    int n, m;
    scanf( "%d %d", &n, &m ); 
    int** board = (int**)malloc(n*sizeof(int*));
    for( int i = 0; i < n; ++i ) {
        board[i] = (int*)malloc(m*sizeof(int));
    }
    int num_spaces = 0;
    for( int i = 0; i < n; ++i ) {
        for( int j = 0; j < m; ++j ) {
            scanf("%d", &board[i][j] );
            if( !board[i][j] )
                ++num_spaces;
        }
    }
    // transform the board into a graph
    transform( board, n, m );
    // perform edmonds-karp on the graph
    int max_flow = edmonds_karp(the_graph);
    // check the max_flow
    if( max_flow * 2 == num_spaces ) {
        printf( "YES\n" );
    } else {
        printf( "NO\n" );
    }
    //cleanup memory
    for( int i = 0; i < n; ++i ) {
        free(board[i]);
    }
    free(the_graph->vertices);
    free(the_graph->edge_flow);
    for( int i = 0; i < the_graph->num_edges; ++i ) {
        if( the_graph->edges[i] ) {
            free(the_graph->edges[i]);
        }
    }
    free(the_graph->edges);
    free(board);
}

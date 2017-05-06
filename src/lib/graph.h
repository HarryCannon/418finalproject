/*
 * NOTE: Code adapted and modified from 15-122
 */

#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph_edge edge;
struct graph_edge {
	unsigned int a;
	unsigned int b;
};

typedef struct territory_node territory;
struct territory_node {
	unsigned int owner;
	unsigned int numTroops;
};

typedef struct graph_header graph;
struct graph_header {
	unsigned int size;
	territory *territories;
	unsigned int **topology;
};

typedef unsigned int vertex;
typedef struct graph_header* graph_t;

graph_t graph_new(unsigned int size, unsigned int numEdges, edge* edges);

void graph_free(graph_t G);

unsigned int graph_size(graph_t G);

void graph_getneighbors(graph_t G, vertex v, vertex* neighbors);

#endif

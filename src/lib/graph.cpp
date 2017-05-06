#include "graph.h"
#include <stdlib.h>

/*
 * Vars
 * size: the number of unique nodes
 * numEdges: number of connections between territories
 * edges: a struct of two unsigned ints used to draw the graph
 *
 * Note: since the topology of the graph does not change, we store the 
 * neighbors in heap-allocated fixed size arrays
 */

typedef struct graph_header graph;
graph* graph_new(unsigned int size, unsigned int numEdges, edge* edges) {
	graph* G = (graph*)malloc(sizeof(graph));
	G->size = size;

	/* Making the topology */

	int* vertexNeighbors = (int*)calloc(size,sizeof(int));

	// Get the count of each vertex
	for (int i = 0; i < numEdges; i++) {
		edge e = edges[i];
		unsigned int a = e.a;
		unsigned int b = e.b;
		vertexNeighbors[a]++;
		vertexNeighbors[b]++;
	}
	
	G->topology = (unsigned int**)malloc(size * sizeof(unsigned int*));
	
	// Make the adj-array for each vertex, reset neighbors which we will use to 
	// index into each array
	for (int i = 0; i < size; i++) {
		G->topology[i] = (unsigned int*)malloc(vertexNeighbors[i] * sizeof(unsigned int));
		vertexNeighbors[i] = 0;
	}
	
	// Using vertexNeighbors as an index, populate the adj-arrays
	for (int i = 0; i < numEdges; i++) {
		unsigned int a = edges[i].a;
		unsigned int b = edges[i].b;
		int indexA = vertexNeighbors[a];
		int indexB = vertexNeighbors[b];

		G->topology[a][indexA] = b;
		G->topology[b][indexB] = a;

		vertexNeighbors[a]++;
		vertexNeighbors[b]++;
	}

	/* Making the territories */
	G->territories = (territory*)calloc(size, sizeof(territory));
	return G;
}

void graph_free(graph* G) {
	free(G->territories);
	for (int i = 0; i < G->size; i++) {
		free(G->topology[i]);
	}
	free(G->topology);
	free(G);
}

unsigned int graph_size(graph* G) {
	return G->size;
}

void graph_getneighbors(graph* G, vertex v, vertex* neighbors) {

}

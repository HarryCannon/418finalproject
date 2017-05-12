#include "graph.h"
#include <stdlib.h>
#include <iostream>
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
	G->territorySizes = (unsigned int*)calloc(size,sizeof(unsigned int));
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
	
	G->topology = (vertex**)malloc(size * sizeof(vertex*));
	
	// Make the adj-array for each vertex, reset neighbors which we will use to 
	// index into each array
	for (int i = 0; i < size; i++) {
		G->topology[i] = (vertex*)malloc(vertexNeighbors[i] *sizeof(vertex));
		G->territorySizes[i] = vertexNeighbors[i];
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
	free(vertexNeighbors);

	/* Making the territories */
	G->territories = (territory*)calloc(size, sizeof(territory));
	for (int i = 0; i < size; i++) {
		G->territories[i].index = i;
	}
	return G;
}

void graph_free(graph* G) {
	free(G->territories);
	free(G->territorySizes);
	for (int i = 0; i < G->size; i++) {
		free(G->topology[i]);
	}
	free(G->topology);
	free(G);
}

unsigned int graph_size(graph* G) {
	return G->size;
}

vertex* graph_getneighbors(graph* G, vertex v) {
	return G->topology[v];
}

void graph_print(graph_t G) {
	for (unsigned int i = 0; i < G->size; i++) {
		graph_print_territory(G, i);
	}
}

void graph_printgrid(graph_t G, int length, int width) {
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			int node = i*length + j;
			territory terr = G->territories[node];
			if (terr.owner == 0) {
				std::cout << " " << terr.numTroops << " ";
			}
			else 
			{
				std::cout << "-" << terr.numTroops << " ";
			}
		}
		std::cout << std::endl;
	}
}

void graph_print_territory(graph_t G, vertex v) {
	vertex* V = G->topology[v];
	territory T = G->territories[v];
	std::cout << "Vertex: " << T.index << " Owned by: " << T.owner << " Troops: " << T.numTroops << std::endl;
	std::cout << "Neighbors: ";
	for (int i = 0; i < G->territorySizes[v]; i++) {
		std::cout << G->topology[v][i] << " ";
	}
	std::cout << std::endl << std::endl;;
}

void graph_modify_troops(territory* T, vertex v, int numTroops) {
	T[v].numTroops += numTroops;
}

void graph_transfer_troops(territory* T, vertex src, vertex dest, int numTroops) {
	T[src].numTroops -= numTroops;
	T[dest].numTroops += numTroops;
}

void graph_applymove(territory* T, move* m) {
	vertex atk = m->attacker;
	vertex def = m->defender;
	T[atk].numTroops = m->attackersLeft;
	T[def].numTroops = m->defendersLeft;
	if (m->success) {
		std::cout << "\tgraph_applymove: successful attack\n";
		T[def].owner = T[atk].owner;
	}
}

void graph_setowner(territory* T, vertex v, unsigned int newOwner) {
	T[v].owner = newOwner;
}

unsigned int graph_getowner(territory* T, vertex v) {
	return T[v].owner;
}

graph_t graph_copy(graph_t G) {
	graph_t ret = (graph*)malloc(sizeof(graph));
	ret->size = G->size;
	ret->territorySizes = (unsigned int*)calloc(G->size,sizeof(unsigned int));
	ret->topology = (vertex**)malloc(G->size * sizeof(vertex*));
	ret->territories = (territory*)calloc(G->size, sizeof(territory));
	for (int i = 0; i < G->size; i++) {
		ret->territories[i] = G->territories[i];
		ret->territorySizes[i] = G->territorySizes[i];
		ret->topology[i] = (vertex*)malloc(G->territorySizes[i] *sizeof(vertex));
		for (int j = 0; j < G->territorySizes[i]; j++) {
			ret->topology[i][j] = G->topology[i][j];
		}
	}
	return ret;
}

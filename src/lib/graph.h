#include <cstddef>
#ifndef GRAPH_H
#define GRAPH_H

typedef unsigned int vertex;

typedef struct graph_edge edge;
struct graph_edge {
	unsigned int a;
	unsigned int b;
};

typedef struct territory_node territory;
struct territory_node {
	vertex index;
	unsigned int owner;
	unsigned int numTroops;
};

typedef struct graph_header graph;
struct graph_header {
	unsigned int size;
	unsigned int *territorySizes;
	territory *territories;
	vertex **topology;
};

// If success == true, then we change the owner of the defender
typedef struct single_move move;
struct single_move {
	vertex attacker;
	vertex defender;
	unsigned int attackersLeft;
	unsigned int defendersLeft;
	bool success;
};


typedef struct graph_header* graph_t;

graph_t graph_new(unsigned int size, unsigned int numEdges, edge* edges);

void graph_free(graph_t G);

unsigned int graph_size(graph_t G);

vertex* graph_getneighbors(graph_t G, vertex v); 

void graph_print(graph_t G); //Prints all vertices

void graph_printgrid(graph_t G, int length, int width);

void graph_print_territory(graph_t G, vertex v); //Prints all relevant info for a vertex

void graph_modify_troops(territory* T, vertex v, int numTroops);

void graph_transfer_troops(territory* T, vertex src, vertex dest, int numTroops);

void graph_applymove(territory* T, move* m);

void graph_setowner(territory* T, vertex v, unsigned int newOwner);

unsigned int graph_getowner(territory* T, vertex v);

graph_t graph_copy(graph_t G);

#endif

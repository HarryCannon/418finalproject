
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

typedef struct graph_header* graph_t;

graph_t graph_new(unsigned int size, unsigned int numEdges, edge* edges);

void graph_free(graph_t G);

unsigned int graph_size(graph_t G);

vertex* graph_getneighbors(graph_t G, vertex v); //TODO: add filter here?

/* int heuristic(graph_t G, territory* node, unsigned int player);

territory*** graph_makemovespace(graph_t G, int depth, int maxMoves);

void graph_freemovespace(graph_t G, int depth, int maxMoves);

int graph_getmoves(graph_t G, territory* node, int currDepth, int*** moveSpace, unsigned int player, int turn);

int alphabeta(territory* node, int currDepth, int maxDepth, int alpha, int beta, unsigned int player, int turn);

territory* graph_makemove(graph_t G, move m, int depth, unsigned int player, int turn);
*/

#endif

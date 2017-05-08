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

/*
int heuristic(graph_t G, territory* node, unsigned int player) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (node[i].owner == player) {
			count++;
		}
	}
	if (count == 0) {
		count = INT_MIN;
	}
	if (count == size) {
		count == INT_MAX;
	}
	return count;
}

territory*** graph_makemovespace(graph* G, int depth, int maxMoves) {
	territory*** moveSpace = (territory***)calloc(depth,sizeof(territory**));
	for (int i = 0; i < depth; i++) {
		moveSpace[i] = (territory**)calloc(maxMoves,sizeof(territory*));
		for (int j = 0; j < maxMoves; j++) {
			moveSpace[i][j] = (territory*)calloc(size,sizeof(territory));
		}
	}
}

void graph_freemovespace(graph* G, int depth, int maxMoves, territory*** moveSpace) {
	for (int i = 0; i < depth; i++) {
		for (int j = 0; j < maxMoves; j++) {
			free(moveSpace[i][j]);
		}
		free(moveSpace[i]);
	}
	free(moveSpace);
}

//TODO improve this lol
int graph_getmoves(graph_t G, territory* node, int currDepth, int*** moveSpace, unsigned int player, int turn) {
	//If reinforcing, for now you just put all of your troops on one territory.
	if (turn % 4 < 2) {
		int count = heuristic(G, node, player);
		int reinforcement = count/2;
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < size; j++) {
				moveSpace[currDepth][i][j] = node[j];
			}
		}
		int index = 0;
		for (int i = 0; i < size; i++) {
			if (node[j].owner == player) {
				moveSpace[currDepth][index][j].numTroops += reinforcement;
				index++;
			}
		}
		return count;
	}
	//If attacking, for now you can choose a territory and a country to attack with it. 
	else {
		int count = 0;
		for (int i = 0; i < size; i++) {
			if (node[i].owner == player) {
				vertex* neighbors = graph_getneighbors(G, i);
				for (int j = 0; j < territorySizes[i]; j++) {
					if (node[j].owner == (1 - player) && node[j].numTroops > 1) {
						for (int k = 0; k < size; k++) {
							moveSpace[currDepth][count][k] = node[k];
						}
						int att = node[i].numTroops;
						int def = node[j].numTroops;
						if (att > def + 1) {
							node[j].owner = player;
							node[i].numTroops = 1;
							node[j].numTroops = att - (def + 1);
						}
						if (att == (def + 1) {
							node[i].numTroops = 2;
							node[j].numTroops = 1;
						}
						if (att < def + 1) {
							node[i].numTroops = 1;
							node[j].numTroops = def - (att - 1);
						}
						count++;
					}
				}
			}
		}
	}
}

int alphabeta(graph_t G, territory*** moveSpace, territory* node, int currDepth, int maxDepth, int alpha, int beta, unsigned int player, int turn)
{
	bool maximizingPlayer = ((maxDepth - currDepth) % 2) == 0;
	int heuristic = heuristic(G, node, player);

	if (depth == 0 || heuristic == INT_MAX || heuristic == INT_MIN) {
		return heuristic(G, node, player);
	}

	if (maximizingPlayer) {
		int v = INT_MIN;
		int numMoves = graph_getmoves(G, node, currDepth - 1, moveSpace, player, turn);
		for (int i = 0; i < numMoves; i++) {
			int newv = alphabeta(G, moveSpace, moveSpace[currDepth - 1][i], currDepth - 1, maxDepth, alpha, beta, player, turn + 1)
			if (newv > v) {
				v = newv;
				if (currDepth == maxDepth) {
					for (int j = 0; j < size; j++) {
						node[i] = moveSpace[currDepth - 1][i][j];
					}
				}
			}
			alpha = max(alpha, v)
			if (beta <= alpha) {
				break;
			}
		}
		return v;
	} else {
		int v = INT_MAX;
		int numMoves = graph_getmoves(G, currDepth - 1, moveSpace, 1-player, turn);
		for (int i = 0; i < numMoves; i++) {
			v = min(v, alphabeta(G, moveSpace, moveSpace[currDepth - 1][i], currDepth - 1, maxDepth, alpha, beta, player, turn + 1));
			alpha = min(alpha, v)
			if (beta <= alpha) {
				break;
			}
		}
		return v;
	}
}

territory* graph_makemove(graph_t G, move m, int depth, unsigned int player, int turn) {
	territory*** moveSpace = graph_makemovespace(G, depth, maxMoves);
	alphabeta(G, moveSpace, territories, depth, dpeth, INT_MIN, INT_MAX, player, turn);
}
*/
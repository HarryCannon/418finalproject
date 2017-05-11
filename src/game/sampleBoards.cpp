#include "sampleBoards.h"
#include <stdlib.h>

graph_t ring_graph(unsigned int size) {
	edge* E = (edge*)malloc(sizeof(struct graph_edge) * size);
	for (unsigned int i = 0; i < size; i++) {
		E[i].a = i;
		E[i].b = (i+1)%size;
	}
	return graph_new(size, size, E);
}

graph_t grid_graph(unsigned int length, unsigned int width) {
	unsigned int size = length * width;
	edge* E = (edge*)malloc(sizeof(struct graph_edge) * size);

	int edgeI = 0;
	for (unsigned int i = 0; i < length; i++) {
		for (unsigned int j = 0; j < width; j++) {
			// for each node (i,j) 
			// draw the edges to the node below it, and to the right of it
			// if they exist

			int currI = i*length + j;
			int bottomI = (i+1)*length + j;
			int rightI = i*length + j + 1;
			if (i < length-1) {
				E[edgeI].a = currI;
				E[edgeI].b = bottomI;
				edgeI++;
			}
			if (j < width-1) {
				E[edgeI].a = currI;
				E[edgeI].b = rightI;
				edgeI++;
			}
		}
	}


	return graph_new(size, size, E);
}

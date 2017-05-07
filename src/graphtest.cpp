#include "lib/graph.h"
#include <stdlib.h>
int main() {
	// Make a ring graph of size 4
	
	edge* E = (edge*)malloc(sizeof(struct graph_edge) * 4);
	for (int i = 0; i < 4; i++) {
		E[i].a = i;
		E[i].b = (i + 1)%4;
	}

	graph_t G = graph_new(4, 4, E);

	return 0;
}

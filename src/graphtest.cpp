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

	graph_setowner(G->territories, 0, 1);
	graph_setowner(G->territories, 1, 1);

	for (int i = 0; i < 4; i++) {
		graph_modify_troops(G->territories, i, 5);
	}

	graph_print(G);

	return 0;
}

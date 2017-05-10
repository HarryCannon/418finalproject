#include "lib/graph.h"
#include "game/game.h"
#include "game/player.h"
#include "ai/randomAI.h"
#include <stdlib.h>
#include <iostream>

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

	// Board is setup
	// Setup players
	player *p1 = new player();
	p1->isCPU = true;
	p1->attacking_move = &attack_random_ai;
	
	player *p2 = new player();
	p2->isCPU = true;
	p2->attacking_move = &attack_random_ai;
	// Begin game

	player *P = (player*)malloc(sizeof(player) * 2);
	P[0] = *p1;
	P[1] = *p2;

	game_info *game = new game_info();
	game->numPlayers = 2;
	game->players = P;
	game->board = G;
	game->turnCount = 0;
	game->playerTurn = 0;
	game->attackingTurn = true;

	int winner = run_game(game);	

	std::cout << "The winner is " << winner << std::endl;

	return 0;
}

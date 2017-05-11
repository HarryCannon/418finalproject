#include "lib/graph.h"
#include "game/game.h"
#include "game/player.h"
#include "ai/randomAI.h"
#include <stdlib.h>
#include <iostream>
#include "game/sampleBoards.h"

int main() {
	// Make a ring graph of size 4

	graph_t G = ring_graph(4);

	graph_setowner(G->territories, 2, 1);
	graph_setowner(G->territories, 3, 1);

	for (int i = 0; i < 4; i++) {
		graph_modify_troops(G->territories, i, 5);
	}

	graph_print(G);

	// Board is setup
	// Setup players
	player *p1 = new player();
	p1->isCPU = true;
	p1->attacking_move = &attack_random_ai;
	p1->defending_move = &defend_random_ai;
	
	player *p2 = new player();
	p2->isCPU = true;
	p2->attacking_move = &attack_random_ai;
	p2->defending_move = &defend_random_ai;
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

	int winner = run_game(game,10);	

	std::cout << "The winner is " << winner << std::endl;

	return 0;
}

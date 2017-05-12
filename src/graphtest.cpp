#include "lib/graph.h"
#include "game/game.h"
#include "lib/dice.h"
#include "game/player.h"
#include "ai/randomAI.h"
#include "ai/aistuff.h"
#include <stdlib.h>
#include <iostream>
#include "game/sampleBoards.h"
#include <time.h>

int main() {
	//precompute dice stuff
	for (int i = 1; i < 21; i++) {
		for (int j = 1; j < 21; j++) {
			printf("i: %d, j: %d", i, j);
			getProbabilityOfSuccess(i, j);
		}
	}
	int temp = 0;
	getBattleEndstates(10, 1, .5, &temp);
	// Make a ring graph of size 4
	srand(time(NULL));
	int length = 4;
	int width = 4;

	graph_t G = grid_graph(length, width);

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			int node = i*width + j;
			if ((i + j) % 2 == 0) { 
				graph_setowner(G->territories, node, 1);
			}
			graph_modify_troops(G->territories, node, (rand() % 15) + 1);
		}
	}

	graph_print(G);

	// Board is setup
	// Setup players
	player *p1 = new player();
	p1->isCPU = true;
	p1->attacking_move = &attack_parallel_ai;
	p1->defending_move = &defend_sequential_ai;
	
	player *p2 = new player();
	p2->isCPU = true;
	p2->attacking_move = &attack_sequential_ai;
	p2->defending_move = &defend_sequential_ai;
	// Begin game

	player *P = (player*)malloc(sizeof(player) * 2);
	P[0] = *p1;
	P[1] = *p2;
	/*

	player *p3 = new player();
	p3->isCPU = true;
	p3->attacking_move = &attack_parallel_ai;
	p3->defending_move = &defend_sequential_ai;
	
	player *p4 = new player();
	p4->isCPU = true;
	p4->attacking_move = &attack_parallel_ai;
	p4->defending_move = &defend_sequential_ai;
	// Begin game

	player *P2 = (player*)malloc(sizeof(player) * 2);
	P2[0] = *p3;
	P2[1] = *p4;
	*/

	game_info *game1 = new game_info();
	game1->numPlayers = 2;
	game1->players = P;
	game1->board = G;
	game1->turnCount = 0;
	game1->playerTurn = 0;
	game1->attackingTurn = true;

	//game_info *game2 = copy_game(game1);
	//game2->players = P2;

	int winner = run_game(game1);	

	std::cout << "The winner is " << winner << std::endl;

	return 0;
}

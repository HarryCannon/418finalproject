#include "game.h"
#include <stdlib.h>
#include <iostream>

bool execute_move(game_info* game) {
	int currP = game->playerTurn;
	game->turnCount++;
	game->playerTurn = (game->playerTurn + 1) % game->numPlayers;
	player p = game->players[currP];

	if (p.isCPU) {
		if (game->attackingTurn) {
			(p.make_attacking_move)(game);
			// Check if any players are out
			int* playerArr = (int*)calloc(sizeof(int), game->numPlayers);
			for (int i = 0; i < graph_size(game->board); i++) {
				playerArr[graph_getowner(game->board->territories,i)]++;
			}
			for (int i = 0; i < game->numPlayers; i++) {
				if (playerArr[i] == 0) {
					std::cout << "Player " << i << " is out." << std::endl;
					return false; // TODO: support multiple players
				}
			}
		}
		else {
			(p.make_defending_move)(game);
		}
	}
	else {
		//TODO: Read line from input, parse, and then execute relevant moves
	}
}

#include "game.h"
#include <stdlib.h>
#include <iostream>
#include <string>

bool execute_move(game_info* game) {
	int currP = game->playerTurn;
	std::cout << "\tPLAYER " << currP << "'s TURN\n";
	player p = game->players[currP];

	if (p.isCPU) {
		if (game->attackingTurn) {
			(p.attacking_move)(game);
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
			(p.defending_move)(game);
		}
	}
	else {
		//TODO: Read line from input, parse, and then execute relevant moves
		// aka, have humans the ability to play
	}
	game->playerTurn = (game->playerTurn + 1) % game->numPlayers;
	game->turnCount++;
}

int run_game(game_info* game) {

	bool done = false;

	while (!done) {
		std::string temp;
		std::getline(std::cin, temp); // So we can "step through" a game
		if (temp == "q") return -1;
		done = execute_move(game);
		graph_print(game->board);
	}

	return 0;
}

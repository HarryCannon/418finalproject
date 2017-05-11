#include "game.h"
#include <stdlib.h>
#include <iostream>
#include <string>

bool execute_move(game_info* game) {
	int currP = game->playerTurn;
	std::cout << "PLAYER " << currP << "'s TURN\n";
	player p = game->players[currP];

	if (p.isCPU) {
		if (game->attackingTurn) {
			(p.attacking_move)(game);
			// Check if any players are out
			int* playerArr = (int*)calloc(sizeof(int), game->numPlayers);
			std::cout << "Owners:";
			for (int i = 0; i < graph_size(game->board); i++) {
				int owner = graph_getowner(game->board->territories, i);
				std::cout << " " << owner;
				playerArr[owner] = playerArr[owner] + 1;
			}
			std::cout << std::endl;
			for (int i = 0; i < game->numPlayers; i++) {
				if (playerArr[i] == 0) {
					std::cout << "Player " << i << " is out." << std::endl;
					return true; // TODO: support multiple players
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
	int t = game->playerTurn + 1;
	if (t == game->numPlayers) {
		std::cout << "Switching attack/def turns.\n";
		game->attackingTurn = !game->attackingTurn;
	}
	game->playerTurn = (game->playerTurn + 1) % game->numPlayers;
	game->turnCount++;
	return false;
}

int run_game(game_info* game, int granularity) {

	bool done = false;
	while (!done) {
		std::string temp = "";
		if (game->turnCount % granularity == 0) {
			std::getline(std::cin, temp); // So we can "step through" a game
		}
		if (temp == "q") return -1;
		done = execute_move(game);
		if (game->turnCount % granularity == 0) {
			graph_print(game->board);
		}
	}

	return game->playerTurn;
}

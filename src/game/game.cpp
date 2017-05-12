#include "game.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>

game_info* copy_game(game_info* game) {
	game_info* ret = new game_info;
	ret->numPlayers = game->numPlayers;
	ret->players = NULL;
	ret->board = graph_copy(game->board);
	ret->turnCount = game->turnCount;
	ret->attackingTurn = game->attackingTurn;
	return ret;
}

bool execute_move(game_info* game) {
	int currP = game->playerTurn;
	std::cout << "\tPLAYER " << currP << "'s TURN\n";
	std::cout << "attacking turn: " << game->attackingTurn << "turnCount : " << game->turnCount;
	player p = game->players[currP];

	if (p.isCPU) {
		if (game->attackingTurn) {
			(p.attacking_move)(game);
			/*
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
			*/
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
	if (game->playerTurn == 0) {
		game->attackingTurn = !game->attackingTurn;
	}
	if (game->turnCount > 100) {
		return true;
	}
	return false;
}

int run_game(game_info* game1, game_info* game2) {

	bool done = false;

	while (!done) {
		std::string temp;
		std::getline(std::cin, temp); // So we can "step through" a game
		if (temp == "q") return -1;
		auto t1 = std::chrono::high_resolution_clock::now();
		done = execute_move(game2);
		auto t2 = std::chrono::high_resolution_clock::now();
		int time1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << std::endl << time1<< " milliseconds." << std:: endl;
		t1 = std::chrono::high_resolution_clock::now();
		done = execute_move(game1);
		t2 = std::chrono::high_resolution_clock::now();
		int time2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << std::endl << time2<< " milliseconds." << std::endl;
		std::cout << std::endl << "speedup: " <<  ((double)time2/(double)time1) << std::endl;
		//graph_printgrid(game1->board, 3, 3);
	}

	return 0;
}

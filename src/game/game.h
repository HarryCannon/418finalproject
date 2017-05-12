#include "player.h"
#include "../lib/graph.h"
/*
 * File detailing a game of risk
 */

#ifndef RISK_GAME_H
#define RISK_GAME_H

typedef struct risk_game game_info;
struct risk_game {
	int numPlayers;
	player* players;
	graph* board;
	int turnCount;
	int playerTurn;
	bool attackingTurn;
};

game_info* copy_game(game_info* game);

// Return true if the game is over, false otherwise
// Modifies the game_info struct in place
bool execute_move(game_info* game);

int run_game(game_info* game);
#endif 

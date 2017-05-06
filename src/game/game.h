#include "player.h"
#include "board.h"
/*
 * File detailing a game of risk
 */

#ifndef RISK_GAME_H
#define RISK_GAME_H
struct risk_game {
	int numPlayers;
	player* players;
	game_board* board;
	int turnCount;
	int playerTurn;
};
#endif 

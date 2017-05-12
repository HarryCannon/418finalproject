#include "../lib/dice.h"
#include "../game/game.h"
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include "aistuff.h"
#include "omp.h"
#include <chrono>

#define MAXBATTLES 1
#define TURNDEPTH 8

typedef struct battleStruct battle;
struct battleStruct {
	vertex attacker;
	vertex defender;
};

int heuristic(game_info* game);
std::vector<battle> getBattles(game_info* game);
endstate* getEndstates(int attackers, int defenders, int*);
void applyReinforcementStrategy(game_info* game);

void advance_turn(game_info* game) {
	game->turnCount++;
	game->playerTurn = (game->playerTurn + 1) % 2;
	if (game->playerTurn == 0) {
		game->attackingTurn = !game->attackingTurn;
	}
}

int heuristic(game_info* game, unsigned int player) {
	int count = 0;
	for (int i = 0; i < game->board->size; i++) {
		if (game->board->territories[i].owner == player) {
			count++;
		}
	}
	return count;
}

std::vector<battle> getBattles(game_info* game) {
	std::vector<battle> ret;
	battle c;
	c.attacker = -1;
	c.defender = -1;
	ret.push_back(c);

	for (int i = 0; i < game->board->size; i++) {
		territory terr = game->board->territories[i];
		if (terr.owner == game->playerTurn && terr.numTroops > 1) {
			vertex* neighbors = graph_getneighbors(game->board, terr.index);
			for (int j = 0; j < game->board->territorySizes[terr.index]; j++) {
				territory neighbor = game->board->territories[neighbors[j]];
				if (neighbor.owner != game->playerTurn && getProbabilityOfSuccess(terr.numTroops, neighbor.numTroops) > 0.5) {
					battle b;
					b.attacker = i;
					b.defender = neighbors[j];
					ret.push_back(b);
				}
			}
		}
	}

	return ret;
}

endstate* getEndstates(int attackers, int defenders, int* size) {
	endstate* ret = getBattleEndstates(attackers, defenders, 0.5, size);
	return ret;
}

void applyReinforcementStrategy(game_info* game) {
	//printf("Entering reinforcement\n");
	int numTerritories = heuristic(game, game->playerTurn);
	int troops = ((numTerritories / 2) < 3) ? 3 : (numTerritories / 2);
	std::vector<vertex> border;

	for (int i = 0; i < game->board->size; i++) {
		territory terr = game->board->territories[i];
		if (terr.owner == game->playerTurn) {
			vertex* neighbors = graph_getneighbors(game->board, terr.index);
			for (int j = 0; j < game->board->territorySizes[terr.index]; j++) {
				territory neighbor = game->board->territories[neighbors[j]];
				if (neighbor.owner != game->playerTurn) {
					border.push_back(i);
					break;
				}
			}
		}
	}

	if (border.size() == 0) {
		//printf("Border size is zero, thinking\n");
		return;
	}
	int perTerritory = troops / border.size();
	int remainder = troops % border.size();

	for (int i = 0; i < border.size(); i++) {
		int count = perTerritory;
		if (remainder > 0) {
			count++;
			remainder--;
		}
		int borderTroops = game->board->territories[border[i]].numTroops;
		int delta = ((20 - borderTroops) < count) ? (20 - borderTroops) : count;
		graph_modify_troops(game->board->territories, border[i], delta);
	}
	//printf("Leaving reinforcement\n");
}

double alphabeta(game_info* game, double alpha, double beta, battle* b, int battleCount, int maxBattles, int maxTurn, unsigned int player, battle* rootBattle = NULL) {
	//printf("turn: %d, attackingTurn: %d, playerTurn: %d, alpha: %f, beta: %f, battleCount: %d\n", game->turnCount, game->attackingTurn, game->playerTurn, alpha, beta, battleCount);
	int count = heuristic(game, player);

	//When at a terminal node, just return the heuristic
	if (count == 0 || count == game->board->size || game->turnCount >= maxTurn) {
		return (double)count;
	}

	//If reinforcing turn just do the strat
	if (!game->attackingTurn) {
		//Save current board so we can undo changes
		int turn = game->turnCount;
		territory* copy = new territory[game->board->size];
		for (int i = 0; i < game->board->size; i++) {
			copy[i] = game->board->territories[i];
		}

		applyReinforcementStrategy(game);
		advance_turn(game);
		applyReinforcementStrategy(game);
		advance_turn(game);
		double ret = alphabeta(game, alpha, beta, NULL, 0, maxBattles, maxTurn, player);

		//Undo reinforcement strategy
		game->turnCount = turn;
		for (int i = 0; i < game->board->size; i++) {
			game->board->territories[i] = copy[i];
		}
		delete copy;
		return ret;
	} 
	//Attacking turn
	else {
		//Need to find best battle
		if (b == NULL) {
			if (battleCount == maxBattles) {
				int turn = game->turnCount;
				int playerTurn = game->playerTurn;
				advance_turn(game);

				double ret = alphabeta(game, alpha, beta, NULL, 0, maxBattles, maxTurn, player);

				game->turnCount = turn;
				game->playerTurn = playerTurn;
				game->attackingTurn = true;
				return ret;
			}
			std::vector<battle> battles = getBattles(game);
			//maximizing
			if (game->playerTurn == player) {
				double v = 0.0;
				double newV = 0.0;
				for (int i = 0; i < battles.size(); i++) {
					if (battles[i].attacker == -1) {
						int turn = game->turnCount;
						int playerTurn = game->playerTurn;
						advance_turn(game);

						newV = alphabeta(game, alpha, beta, NULL, 0, maxBattles, maxTurn, player);

						game->turnCount = turn;
						game->playerTurn = playerTurn;
						game->attackingTurn = true;
					}
					else {
						newV = alphabeta(game, alpha, beta, &battles[i], battleCount+1, maxBattles, maxTurn, player);
					}
					//printf("value of battle attacker: %d, defender: %d, is %f\n", battles[i].attacker, battles[i].defender, newV);
					if (newV > v) {
						v = newV;
						if (rootBattle != NULL) {
							rootBattle->attacker = battles[i].attacker;
							rootBattle->defender = battles[i].defender;
						}
					}
					if (v > alpha) {
						alpha = v;
					}
					if (beta <= alpha) {
						break;
					}
				}
				return v;
			}
			//minimizing
			else {
				double v = (double)game->board->size;
				double newV = 0.0;
				for (int i = 0; i < battles.size(); i++) {
					if (battles[i].attacker == -1) {
						int turn = game->turnCount;
						int playerTurn = game->playerTurn;
						advance_turn(game);

						newV = alphabeta(game, alpha, beta, NULL, 0, maxBattles, maxTurn, player);

						game->turnCount = turn;
						game->playerTurn = playerTurn;
						game->attackingTurn = true;
					}
					else {
						newV = alphabeta(game, alpha, beta, &battles[i], battleCount+1, maxBattles, maxTurn, player);
					}
					if (newV < v) {
						v = newV;
						if (rootBattle != NULL) {
							rootBattle->attacker = battles[i].attacker;
							rootBattle->defender = battles[i].defender;
						}
					}
					if (v < beta) {
						beta = v;
					}
					if (beta <= alpha) {
						break;
					}
				}
				return v;
			}
		}
		//Need to make weighted sum of battle outcomes
		else {
			double v = 0.0;
			int numEndstates = 0;
			int attackers = game->board->territories[b->attacker].numTroops;
			int defenders = game->board->territories[b->defender].numTroops;
			unsigned int defOwner = game->board->territories[b->defender].owner;
			endstate* endstates = getEndstates(attackers, defenders, &numEndstates);
			for (int i = 0; i < numEndstates; i++) {
				int atk = endstates[i].attackers;
				int def = endstates[i].defenders;
				if (def == 0) {
					def = atk - 1;
					atk = 1;
					game->board->territories[b->defender].owner = game->playerTurn;
				}
				game->board->territories[b->attacker].numTroops = atk;
				game->board->territories[b->defender].numTroops = def;
				double newV = alphabeta(game, alpha, beta, NULL, battleCount, maxBattles, maxTurn, player);
				//Undo battle outcome
				game->board->territories[b->attacker].numTroops = attackers;
				game->board->territories[b->defender].numTroops = defenders;
				game->board->territories[b->defender].owner = defOwner;
				v += newV * endstates[i].probability;
			}
			return v;
		}
	}
}

double parallelalphabeta(game_info* game, double alpha, double beta, battle* b, int battleCount, int maxBattles, int maxTurn, unsigned int player, battle* rootBattle = NULL) {
	//printf("turn: %d, attackingTurn: %d, playerTurn: %d, alpha: %f, beta: %f, battleCount: %d\n", game->turnCount, game->attackingTurn, game->playerTurn, alpha, beta, battleCount);
	int count = heuristic(game, player);

	//When at a terminal node, just return the heuristic
	if (count == 0 || count == game->board->size || game->turnCount >= maxTurn) {
		return (double)count;
	}

	std::vector<battle> battles = getBattles(game);
	std::vector<game_info*> games;
	for (int i = 0; i < battles.size(); i++) {
		games.push_back(copy_game(game));
	}
	//maximizing
	double v = 0.0;
	//bool done = false;
	#pragma omp parallel for reduction(max: v)
	for (int i = 0; i < battles.size(); i++) {
		double newV = 0.0;
		if (battles[i].attacker == -1) {
			int turn = games[i]->turnCount;
			int playerTurn = games[i]->playerTurn;
			advance_turn(games[i]);

			newV = alphabeta(games[i], 0.0, beta, NULL, 0, maxBattles, maxTurn, player);
	
			games[i]->turnCount = turn;
			games[i]->playerTurn = playerTurn;
			games[i]->attackingTurn = true;
		}
		else {
			newV = alphabeta(games[i], 0.0, beta, &battles[i], battleCount+1, maxBattles, maxTurn, player);
		}
		//printf("value of battle attacker: %d, defender: %d, is %f\n", battles[i].attacker, battles[i].defender, newV);
		if (newV > v) {
			v = newV;
			if (rootBattle != NULL) {
				rootBattle->attacker = battles[i].attacker;
				rootBattle->defender = battles[i].defender;
			}
		}
	}
	for (int i = 0; i < battles.size(); i++) {
		graph_free(games[i]->board);
		delete games[i];
	}
	return v;
}

void attack_sequential_ai(game_info* game) {
	battle b;
	b.attacker = -1;
	b.defender = -1;

	for (int i = 0; i < MAXBATTLES; i++) {
		//double alphabeta(game_info* game, double alpha, double beta, battle* b, int battleCount, int maxBattles, int maxTurn, unsigned int player, battle* rootBattle = NULL)
		double v = alphabeta(game, 0.0, (double)game->board->size, NULL, i, MAXBATTLES, game->turnCount + TURNDEPTH, game->playerTurn, &b);
		printf("Left alpha beta, chose battle: attacker: %d, defender: %d, with value %f\n", b.attacker, b.defender, v);
		if (b.attacker == -1) {
			break;
		}
		move *m = multipleAttacks(game->board->territories[b.attacker].numTroops, game->board->territories[b.defender].numTroops, 0.5);
		m->attacker = b.attacker;
		m->defender = b.defender;
		graph_applymove(game->board->territories, m);
	}
}

void attack_parallel_ai(game_info* game) {
	battle b;
	b.attacker = -1;
	b.defender = -1;

	for (int i = 0; i < MAXBATTLES; i++) {
		//double alphabeta(game_info* game, double alpha, double beta, battle* b, int battleCount, int maxBattles, int maxTurn, unsigned int player, battle* rootBattle = NULL)
		double v = parallelalphabeta(game, 0.0, (double)game->board->size, NULL, i, MAXBATTLES, game->turnCount + TURNDEPTH, game->playerTurn, &b);
		printf("Left alpha beta, chose battle: attacker: %d, defender: %d, with value %f\n", b.attacker, b.defender, v);
		if (b.attacker == -1) {
			break;
		}
		move *m = multipleAttacks(game->board->territories[b.attacker].numTroops, game->board->territories[b.defender].numTroops, 0.5);
		m->attacker = b.attacker;
		m->defender = b.defender;
		graph_applymove(game->board->territories, m);
	}
}

void defend_sequential_ai(game_info* game) {
	applyReinforcementStrategy(game);
}

/* int heuristic(graph_t G, territory* node, unsigned int player);

territory*** graph_makemovespace(graph_t G, int depth, int maxMoves);

void graph_freemovespace(graph_t G, int depth, int maxMoves);

int graph_getmoves(graph_t G, territory* node, int currDepth, int*** moveSpace, unsigned int player, int turn);

int alphabeta(territory* node, int currDepth, int maxDepth, int alpha, int beta, unsigned int player, int turn);

territory* graph_makemove(graph_t G, move m, int depth, unsigned int player, int turn);
*/



/*
int heuristic(graph_t G, territory* node, unsigned int player) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (node[i].owner == player) {
			count++;
		}
	}
	if (count == 0) {
		count = INT_MIN;
	}
	if (count == size) {
		count == INT_MAX;
	}
	return count;
}

territory*** graph_makemovespace(graph* G, int depth, int maxMoves) {
	territory*** moveSpace = (territory***)calloc(depth,sizeof(territory**));
	for (int i = 0; i < depth; i++) {
		moveSpace[i] = (territory**)calloc(maxMoves,sizeof(territory*));
		for (int j = 0; j < maxMoves; j++) {
			moveSpace[i][j] = (territory*)calloc(size,sizeof(territory));
		}
	}
}

void graph_freemovespace(graph* G, int depth, int maxMoves, territory*** moveSpace) {
	for (int i = 0; i < depth; i++) {
		for (int j = 0; j < maxMoves; j++) {
			free(moveSpace[i][j]);
		}
		free(moveSpace[i]);
	}
	free(moveSpace);
}

//TODO improve this lol
int graph_getmoves(graph_t G, territory* node, int currDepth, int*** moveSpace, unsigned int player, int turn) {
	//If reinforcing, for now you just put all of your troops on one territory.
	if (turn % 4 < 2) {
		int count = heuristic(G, node, player);
		int reinforcement = count/2;
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < size; j++) {
				moveSpace[currDepth][i][j] = node[j];
			}
		}
		int index = 0;
		for (int i = 0; i < size; i++) {
			if (node[j].owner == player) {
				moveSpace[currDepth][index][j].numTroops += reinforcement;
				index++;
			}
		}
		return count;
	}
	//If attacking, for now you can choose a territory and a country to attack with it. 
	else {
		int count = 0;
		for (int i = 0; i < size; i++) {
			if (node[i].owner == player) {
				vertex* neighbors = graph_getneighbors(G, i);
				for (int j = 0; j < territorySizes[i]; j++) {
					if (node[j].owner == (1 - player) && node[j].numTroops > 1) {
						for (int k = 0; k < size; k++) {
							moveSpace[currDepth][count][k] = node[k];
						}
						int att = node[i].numTroops;
						int def = node[j].numTroops;
						if (att > def + 1) {
							node[j].owner = player;
							node[i].numTroops = 1;
							node[j].numTroops = att - (def + 1);
						}
						if (att == (def + 1) {
							node[i].numTroops = 2;
							node[j].numTroops = 1;
						}
						if (att < def + 1) {
							node[i].numTroops = 1;
							node[j].numTroops = def - (att - 1);
						}
						count++;
					}
				}
			}
		}
	}
}

int alphabeta(graph_t G, territory*** moveSpace, territory* node, int currDepth, int maxDepth, int alpha, int beta, unsigned int player, int turn)
{
	bool maximizingPlayer = ((maxDepth - currDepth) % 2) == 0;
	int heuristic = heuristic(G, node, player);

	if (depth == 0 || heuristic == INT_MAX || heuristic == INT_MIN) {
		return heuristic(G, node, player);
	}

	if (maximizingPlayer) {
		int v = INT_MIN;
		int numMoves = graph_getmoves(G, node, currDepth - 1, moveSpace, player, turn);
		for (int i = 0; i < numMoves; i++) {
			int newv = alphabeta(G, moveSpace, moveSpace[currDepth - 1][i], currDepth - 1, maxDepth, alpha, beta, player, turn + 1)
			if (newv > v) {
				v = newv;
				if (currDepth == maxDepth) {
					for (int j = 0; j < size; j++) {
						node[i] = moveSpace[currDepth - 1][i][j];
					}
				}
			}
			alpha = max(alpha, v)
			if (beta <= alpha) {
				break;
			}
		}
		return v;
	} else {
		int v = INT_MAX;
		int numMoves = graph_getmoves(G, currDepth - 1, moveSpace, 1-player, turn);
		for (int i = 0; i < numMoves; i++) {
			v = min(v, alphabeta(G, moveSpace, moveSpace[currDepth - 1][i], currDepth - 1, maxDepth, alpha, beta, player, turn + 1));
			alpha = min(alpha, v)
			if (beta <= alpha) {
				break;
			}
		}
		return v;
	}
}

territory* graph_makemove(graph_t G, move m, int depth, unsigned int player, int turn) {
	territory*** moveSpace = graph_makemovespace(G, depth, maxMoves);
	alphabeta(G, moveSpace, territories, depth, dpeth, INT_MIN, INT_MAX, player, turn);
}
*/
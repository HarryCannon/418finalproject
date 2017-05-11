#include "randomAI.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "../lib/dice.h"

void attack_random_ai(game_info* game) {
	int me = game->playerTurn;
	graph* Board = game->board;
	territory* T = Board->territories;
		
	int numFrontierNodes = 0;
	std::vector<vertex> frontier;
		
	// Get all of my nodes on the "frontier"
	for (vertex i = 0; i < graph_size(Board); i++) {
		if (graph_getowner(T, i) == me) {
			std::cout << "My node: " << i << " ";
			for (vertex j = 0; j < Board->territorySizes[i]; j++) {
				vertex neighborNode = Board->topology[i][j];
				if (graph_getowner(T, neighborNode) != me) {
					//My node is adj to an enemy node
					frontier.push_back(i);
					break;
				}
			}
		}
	}
	std::cout << std::endl;
	// Got my frontier
	std::cout << "I got " << frontier.size() << " elems in frontier" << std::endl;
	srand((unsigned)time(0)); 
	int r = rand() % frontier.size();
	int nodeToAttack = frontier[r];
	int nodeToDefend;
	for (int i = 0; i < Board->territorySizes[nodeToAttack]; i++) {
		int potentialDef = Board->topology[nodeToAttack][i];
		if (graph_getowner(T,potentialDef) != me) {
			nodeToDefend = potentialDef;
			break;
		}
	}
	std::cout << "I pick " << nodeToAttack << " randomly, to attack node: " << nodeToDefend << std::endl;
	
	// Attack until there is 1 attacker, or 0 defenders
	// TODO: Maybe instead of 0, randint from [0,1]?
	move *m = multipleAttacks(T[nodeToAttack].numTroops,T[nodeToDefend].numTroops, 0);
	m->attacker = nodeToAttack;
	m->defender = nodeToDefend;
	graph_applymove(T, m);
	// Done!
}

void defend_random_ai(game_info* game) {
	int me = game->playerTurn;
	int numTerrs = 0;
	std::vector<vertex> myTerrs;
	// Count how many territories I have, divide by 2, then	
	for (int i = 0; i < graph_size(game->board); i++) {
		if (graph_getowner(game->board->territories, i) == me) {
			numTerrs++;
			myTerrs.push_back(i);
		}
	}
	int numReinforcements = (numTerrs/2 > 3) ? numTerrs/2 : 3;

	// For each reinforcement, place it randomly
	srand((unsigned)time(0)); 
	for (int i = 0; i < numReinforcements; i++) {
		int r = rand() % myTerrs.size();
		graph_modify_troops(game->board->territories, myTerrs[r], 1);
		std::cout << "Placing 1 troop on " << myTerrs[r] << "\n";
	}

}

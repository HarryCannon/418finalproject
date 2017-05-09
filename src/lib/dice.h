#include "graph."

#ifndef DICE_H
#define DICE_H

double getProbabilityOfSuccess(int attackers, int defenders);

// 1 <= numDef <= 3
// 1 <= numAtk <= 3
// 0 <= @returnVal <= numDef
int singleAttack(int numAtk, int numDef);

// given numAtk attackers, numDef defenders, attacker keeps attacking
// until the probability of success is less than the specified threshold
// threshold is in range [0,1]
// returns a move struct, defined in graph.h
move *multipleAttacks(int numAtk, int numDef, double threshold);

#endif

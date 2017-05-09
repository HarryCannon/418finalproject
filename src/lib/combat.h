#include "graph.h"

#ifndef COMBAT_H
#define COMBAT_H
typedef unsigned int troopCount;

troopCount single_attack(troopCount attackers, troopCount defenders);

troopCount single_encounter(troopCount attackers, troopCount defenders, double threshold);

#endif

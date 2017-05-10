#include "dice.h"
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <iostream>

double recurse(double** winprobs, double*** probs, int attackers, int defenders) {
    if (defenders == 0) {
        return 1.0;
    }
    if (attackers == 0) {
        return 0.0;
    }
    if (winprobs[attackers][defenders] > -.5) {
        return winprobs[attackers][defenders];
    }
    int def_dice = (defenders > 3) ? 3 : defenders;
    int atk_dice = (attackers > 3) ? 3 : attackers;
    int min = (def_dice < atk_dice) ? def_dice : atk_dice;

    double ret = 0.0;
    for (int i = 0; i < 1 + min; i++) {
        ret += probs[def_dice - 1][atk_dice - 1][i] * recurse(winprobs, probs, attackers + i - min, defenders - i);
    }
    if (winprobs[attackers][defenders] < -.5) {
        winprobs[attackers][defenders] = ret;
    }
    return ret;
}
double getProbabilityOfSuccess(int attackers, int defenders) {
    //I was too lazy to write the code to compute this
    double*** probs = new double**[3];
    for (int i = 0; i < 3; i++) {
        probs[i] = new double*[3];
        for (int j = 0; j < 3; j++) {
            probs[i][j] = new double[4];
        }
    }

    probs[0][0][0] = 0.58333333333333340;
    probs[0][0][1] = 0.41666666666666670;

    probs[0][1][0] = 0.42129629629629630;
    probs[0][1][1] = 0.57870370370370370;

    probs[0][2][0] = 0.34027777777777780;
    probs[0][2][1] = 0.65972222222222220;

    probs[1][0][0] = 0.74537037037037030;
    probs[1][0][1] = 0.25462962962962965;

    probs[1][1][0] = 0.44830246913580246;
    probs[1][1][1] = 0.32407407407407407;
    probs[1][1][2] = 0.22762345679012347;

    probs[1][2][0] = 0.29256687242798350;
    probs[1][2][1] = 0.33577674897119340;
    probs[1][2][2] = 0.37165637860082307;

    probs[2][0][0] = 0.82638888888888880;
    probs[2][0][1] = 0.17361111111111110;

    probs[2][1][0] = 0.61934156378600820;
    probs[2][1][1] = 0.25475823045267487;
    probs[2][1][2] = 0.12590020576131689;

    probs[2][2][0] = 0.38303755144032920;
    probs[2][2][1] = 0.26466049382716050;
    probs[2][2][2] = 0.21469907407407407;
    probs[2][2][3] = 0.13760288065843620;

    double** winprobs = new double*[attackers];
    for (int i = 0; i < attackers; i++) {
        winprobs[i] = new double[defenders + 1];
    }

    for (int i = 0; i < attackers; i++) {
        for (int j = 0; j < defenders + 1; j++) {
            winprobs[i][j] = -1.0;
        }
    }

    double ret = recurse(winprobs, probs, attackers - 1, defenders);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            delete [] probs[i][j];
        }
        delete [] probs[i];
    }
    delete [] probs;

    for (int i = 0; i < attackers; i++) {
        delete [] winprobs[i];
    }
    delete [] winprobs;

    return ret;
}

// 1 <= numDef <= 3
// 1 <= numAtk <= 3
// 0 <= @returnVal <= numDef
int singleAttack(int numAtk, int numDef) {
    std::vector<int> atk;
    std::vector<int> def;

    for (int i = 0; i < numAtk; i++) {
        atk.push_back(rand() % 6);
    }
    for (int i = 0; i < numDef; i++) {
        def.push_back(rand() % 6);
    }
    std::sort(atk.begin(), atk.end());
    std::sort(def.begin(), def.end());

    int ret = 0;
    
    for (int i = 0; i < ((numAtk < numDef) ? numAtk : numDef); i++) {
        if (atk[numAtk - 1 - i] > def[numDef - 1 - i]) {
            ret++;
        }
    }

    return ret;
}

// given numAtk attackers, numDef defenders, attacker keeps attacking
// until the probability of success is less than the specified threshold
// threshold is in range [0,1]
// returns a move struct, defined in graph.h
move *multipleAttacks(int numAtk, int numDef, double threshold) {
    move* result = new move();
    int attackers = numAtk;
    int defenders = numDef;

    while (attackers > 1 && defenders > 0 && getProbabilityOfSuccess(attackers, defenders) > threshold) {
        int def_dice = (defenders > 3) ? 3 : defenders;
        int atk_dice = ((attackers - 1) > 3) ? 3 : (attackers - 1);
        int min = (def_dice < atk_dice) ? def_dice : atk_dice;
        int result = singleAttack(atk_dice, def_dice);
        defenders -= result;
        attackers -= (min - result);
    }
    if (defenders == 0) {
        result->success = true;
				result->attackersLeft = attackers - 1;
				result->defendersLeft = 1;
    } else {
        result->success = false;
				result->attackersLeft = attackers;
				result->defendersLeft = defenders;
    }
		std::cout << "multipleAttacks (atk,def) left: (" << result->attackersLeft << "," << result->defendersLeft << ")\n"; 
		return result;
}

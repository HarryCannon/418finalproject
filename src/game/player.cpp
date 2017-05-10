#include "player.h"
#include <stdlib.h>
player *new_player(bool isCPU) {
	player *p = (player*)calloc(1, sizeof(player));
	p->isCPU = isCPU;
	return p;
}

void set_player_attacking_move(player* p, void (*atkFn)(game_info*)) {
	p->attacking_move = atkFn;
}

void set_player_defending_move(player* p, void (*defFn)(game_info*)) {
	p->defending_move = defFn;
}

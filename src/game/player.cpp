#include "player.h"
#include <stdlib.h>
player *new_player(bool isCPU) {
	player *p = (player*)calloc(1, sizeof(player));
	p->isCPU = isCPU;
	return p;
}

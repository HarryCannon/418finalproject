
#ifndef PLAYER_H
#define PLAYER_H
typedef struct risk_game game_info;

struct risk_player {
	bool isCPU;
	void (*attacking_move)(game_info*);
	void (*defending_move)(game_info*);
	// Enter any optional "sliders" fields here
};

typedef struct risk_player player;

struct risk_player* new_player(bool isCPU);
void set_player_attacking_move(player* p, void (*atkFn)(game_info*));
void set_player_defending_move(player* p, void (*defFn)(game_info*));


#endif

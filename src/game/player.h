
#ifndef PLAYER_H
#define PLAYER_H
typedef struct risk_game game_info;

struct risk_player {
	bool isCPU;
	void (*make_attacking_move)(game_info*);
	void (*make_defending_move)(game_info*);
	// TODO: Enter any "sliders" fields here
};

typedef struct risk_player player;

struct risk_player* new_player(bool isCPU);

#endif

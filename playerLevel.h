#ifndef PLAYERLEVEL_H
#define PLAYERLEVEL_H

#include "map.h"
#include <stdio.h>

//struce holds player's id and player's level
typedef struct player_level_t *PlayerLevel;

// creataes a new PlayerLevel Struct
PlayerLevel createPlayerLevel(int player_id, double level);

//gets PlayerLevel struct, allocates and returns a new copy of it
PlayerLevel copyPlayerLevel(PlayerLevel player_level);

//frees PlayerLevel struct
void destroyPlayerLevel(PlayerLevel player_level);

//functions for comparing between two PlayerLevel structs
//compares first by levels, if levels are even , compares by players id's
int comparePlayerLevel(PlayerLevel player_level1, PlayerLevel player_level2);

int getPlayerLevelId(PlayerLevel player);

double getPlayerLevelLevel(PlayerLevel player);


#endif
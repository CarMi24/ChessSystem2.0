#include "playerLevel.h"
#include <stdlib.h>

struct player_level_t
{
    int id;
    double level;
};

PlayerLevel createPlayerLevel(int player_id, double level)
{
    PlayerLevel player_level = malloc(sizeof(*player_level));
    if (player_level == NULL)
    {
        return NULL;
    }
    player_level->id = player_id;
    player_level->level = level;
    return player_level;
}

PlayerLevel copyPlayerLevel(PlayerLevel player_level)
{
    PlayerLevel copy_player_level = malloc(sizeof(*copy_player_level));
    if (copy_player_level == NULL)
    {
        return NULL;
    }
    copy_player_level->id = player_level->id;
    copy_player_level->level = player_level->level;
    return copy_player_level;
}

void destroyPlayerLevel(PlayerLevel player_level)
{
    free(player_level);
}

int comparePlayerLevel(PlayerLevel player_level1, PlayerLevel player_level2)
{
    if (player_level1->level != player_level2->level)
    {
        return ((player_level1->level > player_level2->level) ? -1 : 1);
    }
    return ((player_level1->id > player_level2->id) ? 1 : -1);
}

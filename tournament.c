#include "tournament.h"
#include <stdio.h>
#include <string.h>


typedef struct Tournament_t
{
    int tournament_id;
    char* tournament_location;
    Map tournament_games;
    int max_games_per_player;
    int tournament_winner_id;
    int winner_id;
    int longest_game_time;
    bool is_closed;

};


static MapKeyElement copyGameKey(MapKeyElement game_key)
{
    if(game_key==NULL)
    {
        return NULL;
    }
    int *copy = (int*)malloc(sizeof(game_key));
    if(copy==NULL)
    {
        return NULL;
    }
    *copy = *(int*)game_key;
    return copy;
}

static MapDataElement copyGameData(MapDataElement game)
{
    if(game==NULL)
    {
        return NULL;
    }
    Game *clone = malloc(sizeof(clone));
    if(clone == NULL)
    {
        return NULL;
    }
    *clone = copyGame(*(Game*)game);
    return clone;
}

static void freeGameKey(MapKeyElement game_key)
{
    free(game_key);
}

static void freeGameData(MapDataElement game)
{
    destroyGame(*(Game*)game);
    free(game);
}

static int compareKeyGame(MapKeyElement game1, MapKeyElement game2)
{
    return *(int*)game1 - *(int*)game2;
}

static void updateLongestGameTime(Tournament tournament,int new_time)
{
    if(tournament->longest_game_time < new_time)
    {
        tournament->longest_game_time = new_time;
    }
}

Tournament createTournament(int tournament_id, char* location, int max_games_per_player)
{
    Tournament new_tournament = malloc(sizeof(new_tournament));
    if(new_tournament == NULL)
    {
        return NULL;
    }
    new_tournament->tournament_id = tournament_id;
    new_tournament->tournament_location = strcopy(location);
}




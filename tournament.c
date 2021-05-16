#include "tournament.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct Tournament_t
{
    int tournament_id;
    char* tournament_location;
    Map tournament_games;
    int max_games_per_player;
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
    Game clone = malloc(sizeof(clone));
    if(clone == NULL)
    {
        return NULL;
    }
    clone = copyGame((Game)game);
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
    new_tournament->tournament_location = malloc(strlen(location) + 1);
    if(new_tournament->tournament_location == NULL)
    {
        free(new_tournament);
        return NULL;
    }
    strcpy(new_tournament->tournament_location,location);
    new_tournament->tournament_games = mapCreate(copyGameData,copyGameKey,freeGameData,freeGameKey,compareKeyGame);
    if(new_tournament->tournament_games == NULL)
    {
        free(new_tournament->tournament_location);
        free(new_tournament);
        return NULL;
    }
    new_tournament->winner_id = 0;
    new_tournament->max_games_per_player = max_games_per_player;
    new_tournament->is_closed = false;
    new_tournament->longest_game_time = 0;

    return new_tournament;
}

void destroyTournament(Tournament tournament)
{
    destroyMap(tournament->tournament_games);
    free(tournament->tournament_location);
    free(tournament);
}

bool checkIfGameExists(Tournament tournament, Game game)
{
    assert(tournament!=NULL && game != NULL);
    Game tmp = NULL;
    MAP_FOREACH(int*,key,tournament->tournament_games)
    {
        tmp = mapGet(tournament->tournament_games,key);
        if(compareGames(tmp,game))
        {
            return true;
        }
       freeGameKey(key);
    }
    return false;
}

TournamentResult addGameToTournament(Tournament tournament, Game game)
{
    if(tournament == NULL || game == NULL)
    {
        return TOURNAMENT_NULL_ARGUMENT;
    }
    int key = mapGetSize(tournament->tournament_games) + 1;
    if(mapPut(tournament->tournament_games,&key,game)!=MAP_SUCCESS)
    {
        return TOURNAMENT_ERROR;
    }
    destroyGame(game);
    return TOURNAMENT_SUCCESS;
}

TournamentResult removePlayerFromTournament(Tournament tournament, int player_id)
{
    if(tournament == NULL)
    {
        return TOURNAMENT_NULL_ARGUMENT;
    }
    Game tmp;
    MAP_FOREACH(int*,key,tournament->tournament_games)
    {
        tmp = mapGet(tournament->tournament_games,key);
        removePlayer(tmp,player_id);
        freeGameKey(key);
    }
    return TOURNAMENT_SUCCESS;
}

TournamentResult endTournament(Tournament tournament, int winner_id)
{
    if(tournament == NULL)
    {
        return TOURNAMENT_NULL_ARGUMENT;
    }
    tournament->is_closed = true;
    tournament->winner_id = winner_id;
    return TOURNAMENT_SUCCESS;
}

int getTournamentLongestTime(Tournament tournament)
{
    assert(tournament!=NULL);
    return tournament->longest_game_time;
}

double calculateAverageTournamentGameTime(Tournament tournament)
{
    Game tmp;
    double avg = 0.0;
    MAP_FOREACH(int*,key,tournament->tournament_games)
    {
       tmp = mapGet(tournament->tournament_games,key);
       avg += getGameTime(tmp);
       freeGameKey(key);
    }
    avg /= mapGetSize(tournament->tournament_games);
    return avg;
}

bool getTournamentStatus(Tournament tournament)
{
    return tournament->is_closed;
}







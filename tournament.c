#include "tournament.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
typedef struct Tournament_t
{
    int tournament_id;
    char *tournament_location;
    Map tournament_games;
    int max_games_per_player;
    int winner_id;
    int longest_game_time;
    int total_players;
    bool is_closed;
};

static MapKeyElement copyGameKey(MapKeyElement game_key)
{
    if (game_key == NULL)
    {
        return NULL;
    }
    int *copy = (int *)malloc(sizeof(game_key));
    if (copy == NULL)
    {
        return NULL;
    }
    *copy = *(int *)game_key;
    return copy;
}

static MapDataElement copyGameData(MapDataElement game)
{
    if (game == NULL)
    {
        return NULL;
    }
    Game clone = copyGame((Game)game);
    return clone;
}

static void freeGameKey(MapKeyElement game_key)
{
    free(game_key);
}

static void freeGameData(MapDataElement game)
{
    destroyGame((Game)game);
}

static int compareKeyGame(MapKeyElement game1, MapKeyElement game2)
{
    return *(int *)game1 - *(int *)game2;
}

static void updateLongestGameTime(Tournament tournament, int new_time)
{
    if (tournament->longest_game_time < new_time)
    {
        tournament->longest_game_time = new_time;
    }
}

static int absoluteValue(int num)
{
    return num>0 ? num : (num)*(-1);
}

static void updateTournamentTotalPlayers(Tournament tournament, int player_1,int player_2)
{
    int players_to_add = 2;
    Game tmp_game = NULL;
    MAP_FOREACH(int*,key,tournament->tournament_games)
    {
        tmp_game = mapGet(tournament->tournament_games,key);
        if(absoluteValue(getFirstPlayerId(tmp_game))==player_1)
        {
            players_to_add--;
        }
        if(absoluteValue(getSecondPlayerId(tmp_game))==player_2)
        {
            players_to_add--;
        }
        free(key);
    }
    players_to_add = (players_to_add < 0) ? 0 : players_to_add;
    tournament->total_players += players_to_add;
}

Tournament createTournament(int tournament_id, char *location, int max_games_per_player)
{
    Tournament new_tournament = malloc(sizeof(new_tournament));
    if (new_tournament == NULL)
    {
        return NULL;
    }
    new_tournament->tournament_id = tournament_id;
    new_tournament->tournament_location = malloc(strlen(location) + 1);
    if (new_tournament->tournament_location == NULL)
    {
        free(new_tournament);
        return NULL;
    }
    strcpy(new_tournament->tournament_location, location);
    new_tournament->tournament_games = mapCreate(copyGameData, copyGameKey, freeGameData, freeGameKey, compareKeyGame);
    if (new_tournament->tournament_games == NULL)
    {
        free(new_tournament->tournament_location);
        free(new_tournament);
        return NULL;
    }
    new_tournament->winner_id = 0;
    new_tournament->max_games_per_player = max_games_per_player;
    new_tournament->is_closed = false;
    new_tournament->longest_game_time = 0;
    new_tournament->total_players = 0;

    return new_tournament;
}

void destroyTournament(Tournament tournament)
{
    destroyMap(tournament->tournament_games);
    free(tournament->tournament_location);
    free(tournament);
}

Tournament copyTournament(Tournament tournament)
{
    assert(tournament != NULL);
    Tournament copy_tournament = malloc(sizeof(*copy_tournament));
    if (copy_tournament == NULL)
    {
        return NULL;
    }
    copy_tournament->tournament_id = tournament->tournament_id;
    copy_tournament->tournament_location = tournament->tournament_location;
    copy_tournament->max_games_per_player = tournament->max_games_per_player;
    copy_tournament->winner_id = tournament->winner_id;
    copy_tournament->is_closed = tournament->is_closed;
    Map tournament_games_map_copy = mapCopy(tournament->tournament_games);
    if (tournament_games_map_copy == NULL)
    {
        free(copy_tournament);
        return NULL;
    }
    copy_tournament->tournament_games = tournament_games_map_copy;
    return copy_tournament;
}

bool checkIfGameInTournament(Tournament tournament, Game game)
{
    assert(tournament != NULL && game != NULL);

    Game tmp = NULL;
    MAP_FOREACH(int *, key, tournament->tournament_games)
    {
        tmp = mapGet(tournament->tournament_games, key);
        if (compareGames(tmp, game))
        {
            return true;
        }
        freeGameKey(key);
    }
    return false;
}

void addGameToTournament(Tournament tournament, Game game)
{
    assert(tournament != NULL && game != NULL);
    int key = mapGetSize(tournament->tournament_games) + 1;
    /*update total players in tournament*/
    assert(mapPut(tournament->tournament_games, &key, game) == MAP_SUCCESS);
    int player_1 = getFirstPlayerId(game);
    int player_2 = getSecondPlayerId(game);
    updateTournamentTotalPlayers(tournament,player_1,player_2);
}

void removePlayerFromTournament(Tournament tournament, int player_id)
{
    assert(tournament != NULL);

    Game tmp;
    MAP_FOREACH(int *, key, tournament->tournament_games)
    {
        tmp = mapGet(tournament->tournament_games, key);
        removePlayerFromGame(tmp, player_id);
        freeGameKey(key);
    }
}

void endTournament(Tournament tournament, int winner_id)
{
    assert(tournament != NULL);
    tournament->is_closed = true;
    tournament->winner_id = winner_id;
}

int getTournamentLongestTime(Tournament tournament)
{
    assert(tournament != NULL);
    return tournament->longest_game_time;
}

double calculateAverageTournamentGameTime(Tournament tournament)
{
    Game tmp;
    double avg = 0.0;
    MAP_FOREACH(int *, key, tournament->tournament_games)
    {
        tmp = mapGet(tournament->tournament_games, key);
        avg += getGameTime(tmp);
        freeGameKey(key);
    }
    avg /= mapGetSize(tournament->tournament_games);
    return avg;
}

int getTournamentId(Tournament tournament)
{
    return tournament->tournament_id;
}
char *getTournamentLocation(Tournament tournament)
{
    return tournament->tournament_location;
}
Map getTournamentGamesMap(Tournament tournament)
{
    return tournament->tournament_games;
}
int getTournamentMaxGamesPerPlayer(Tournament tournament)
{
    return tournament->max_games_per_player;
}
int getTournamentWinnerId(Tournament tournament)
{
    return tournament->winner_id;
}
int getTournamentLongestGameTime(Tournament tournament)
{
    return tournament->longest_game_time;
}
bool getTournamentStatus(Tournament tournament)
{
    return tournament->is_closed;
}
int getTournamentTotalPlayers(Tournament tournament)
{
    return tournament->total_players;
}



int getPlayerTotalPlayTimeInTournament(Tournament tournament, Player player)
{
    int total_game_time_in_tournament = 0;
    MAP_FOREACH(int *, key, tournament->tournament_games)
    {
        Game game = mapGet(tournament->tournament_games, key);
        int player_id = getPlayerId(player);
        if (player_id == getFirstPlayerId(game) || player_id == getSecondPlayerId(game))
        {
            total_game_time_in_tournament += getGamePlayTime(game);
        }
        freeGameKey(key);
    }
    return total_game_time_in_tournament;
}

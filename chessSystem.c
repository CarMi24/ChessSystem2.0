
#include "chessSystem.h"

#include <string.h>
#include <stdlib.h>

#define SMALL_A 'a'
#define SMALL_z 'z'
#define CAPITAL_A 'A'
#define CAPITAL_Z 'Z'
#define BACKSPACE ' '

struct chess_system_t
{
    Map tournaments; /* KEY = tournament_id, DATA = tournament */
    Map players;     /* KEY = player_id, DATA = player */
    int total_games_in_system;
};

/** Functions for the tournaments map**/

/**
 * TournamentKey is type of integer pointer - the id of the tournament.
 * TournamentData is type of Tournament - all tournaments that are taking place in the system.
*/
static void freeKeyInt(MapKeyElement n)
{
    free(n);
}
// static void freeDataTournament(MapDataElement tournament_data)
// {
//     destroyTournament(tournament_data);
// }
static MapKeyElement copyKeyInt(MapKeyElement n)
{
    if (n == NULL)
    {
        return NULL;
    }
    int *copy = (int *)malloc(sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    *copy = *(int *)n;
    return copy;
}
// static MapDataElement copyElementData(MapDataElement tournament_data)**************************************
// {
//     if (tournament_data == NULL)
//     {
//         return NULL;
//     }
//     Tournament copy = malloc(sizeof(*copy));
//     if (copy == NULL)
//     {
//         return NULL;
//     }
//     return copy;
// }
// static MapDataElement copyElementData(MapDataElement tournament_data)
// {
//     if (tournament_data == NULL)
//     {
//         return NULL;
//     }
//     Tournament copy = copyTournament(tournament_data);
//     if (copy == NULL)
//     {
//         return NULL;
//     }
//     return copy;
// }
static int compareKeyInt(MapKeyElement n1, MapKeyElement n2)
{
    return (*(int *)n1 - *(int *)n2);
}

/** Functions for the players map**/
/**
 * player_key is type of integer pointer - the id of the tournament.
 * tournament_data is type of Tournament - all tournaments that are taking place in the system.
*/
// static void freeKeyPlayer(MapKeyElement player_key)
// {
//     free(player_key);
// }
// static void freeDataPlayer(MapDataElement player_data)
// {
//     destroyPlayer(player_data);
// }
// static MapKeyElement copyKeyPlayer(MapKeyElement player_key)
// {
//     if (player_key == NULL)
//     {
//         return NULL;
//     }
//     int *copy = (int *)malloc(sizeof(*copy));
//     if (copy == NULL)
//     {
//         return NULL;
//     }
//     *copy = *(int *)player_key;
//     return copy;
// }
// static MapDataElement copyDataPlayer(MapDataElement player_data)
// {
//     if (player_data == NULL)
//     {
//         return NULL;
//     }
//     Player copy = copyPlayer(player_data);
//     if (copy == NULL)
//     {
//         return NULL;
//     }
//     return copy;
// }
// static int compareKeyPlayer(MapKeyElement player_key1, MapKeyElement player_key2)
// {
//     return (*(int *)player_key1 - *(int *)player_key1);
// }

/** Static Functions for convinience **/

/**
 * returns true if an integer is positive.
 */
static bool isPositive(int number)
{
    return (number > 0 ? true : false);
}

/**
 * Check if location is valid
 */
static bool isLocationValid(const char *location)
{
    if (!(location[0] >= CAPITAL_A && location[0] <= CAPITAL_Z))
    {
        return false;
    }
    int len = strlen(location);
    for (int i = 1; i < len; i++)
    {
        if ((!(location[i] >= SMALL_A && location[i] <= SMALL_z)) && location[i] != BACKSPACE)
        {
            return false;
        }
    }
    return true;
}

/**
 * check each player games played in the tournament in comparison to the max allowed
 * returns true if both under the limit otherwise false
 */
static bool checkExceededGames(ChessSystem chess, int player1_id, int player2_id, int tournament_id, int max_games_per_player)
{
    if (mapContains(chess->players, &player1_id))
    {
        if (getGamesPlayedInTournament((Player)mapGet(chess->players, &player1_id), tournament_id) >= max_games_per_player)
        {
            return false;
        }
    }
    if (mapContains(chess->players, &player2_id))
    {
        if (getGamesPlayedInTournament((Player)mapGet(chess->players, &player2_id), tournament_id) >= max_games_per_player)
        {
            return false;
        }
    }
    return true;
}

// /**
//  * calculates and return the ID of the player who won the tournament.
//  */

// static int calculateTournamentWinner(Map players, int tournament_id);

// /**
//  * Iterates the players map and sums the players who played in a given tournament
//  * returns the number of players
//  */
// static int calculateNumOfPlayersInTournament(Map players, int tournament_id)
// {
// }

ChessSystem chessCreate()
{
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    Map tournaments_map = mapCreate((copyMapDataElements)copyTournament, copyKeyInt, (freeMapDataElements)destroyTournament, freeKeyInt, compareKeyInt);
    if (tournaments_map == NULL)
    {
        return NULL;
    }
    chess_system->tournaments = tournaments_map;
    Map players_map = mapCreate((copyMapDataElements)copyPlayer, copyKeyInt, (freeMapDataElements)destroyPlayer, freeKeyInt, compareKeyInt);
    if (tournaments_map == NULL)
    {
        mapDestroy(tournaments_map);
        return NULL;
    }
    chess_system->players = players_map;
    chess_system->total_games_in_system = 0;
    return chess_system;
}

void chessDestroy(ChessSystem chess)
{
    if (chess == NULL)
    {
        return;
    }
    mapDestroy(chess->tournaments);
    mapDestroy(chess->players);
    free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char *tournament_location)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (!isPositive(tournament_id))
    {
        return CHESS_INVALID_ID;
    }
    if (mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if (!(isLocationValid(tournament_location)))
    {
        return CHESS_INVALID_LOCATION;
    }
    if (!isPositive(max_games_per_player))
    {
        return CHESS_INVALID_MAX_GAMES;
    }
    Tournament new_tournament = createTournament(tournament_id, tournament_location, max_games_per_player);
    if (new_tournament == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult map_result = mapPut(chess->tournaments, &tournament_id, new_tournament);
    if (map_result == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if ((!isPositive(first_player)) || (!isPositive(second_player)) ||
        (!isPositive(tournament_id)) || (first_player == second_player))
    {
        return CHESS_INVALID_ID;
    }
    if (!(mapContains(chess->tournaments, &tournament_id)))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Tournament tmp_tournament = (Tournament)mapGet(chess->tournaments, &tournament_id);
    if (getTournamentIsClosed(tmp_tournament) == true)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    Game game_to_add = createGame(tournament_id, first_player, second_player, winner, play_time);
    if (game_to_add == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    if (checkIfGameExists((Tournament)mapGet(chess->tournaments, &tournament_id), game_to_add))
    {
        destroyGame(game_to_add);
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if (!(isPositive(play_time)))
    {
        destroyGame(game_to_add);
        return CHESS_INVALID_PLAY_TIME;
    }
    int max_games_per_player = getTournamentMaxGamesPerPlayer((Tournament)mapGet(chess->tournaments, &tournament_id));
    if (!checkExceededGames(chess, first_player, second_player, tournament_id, max_games_per_player))
    {
        destroyGame(game_to_add);
        return CHESS_EXCEEDED_GAMES;
    }
    addGameToTournament((Tournament)mapGet(chess->tournaments, &tournament_id), game_to_add); //return value?*********
    createPlayer(first_player);
    createPlayer(second_player);
    //after adding the game update each players new stats
    Player player1 = (Player)mapGet(chess->players, &first_player);
    Player player2 = (Player)mapGet(chess->players, &second_player);
    if (winner == FIRST)
    {
        updatePlayerTournamentStats(player1, tournament_id, WIN, play_time);
        updatePlayerTournamentStats(player2, tournament_id, LOSE, play_time);
    }
    else if (winner == SECOND)
    {
        updatePlayerTournamentStats(player1, tournament_id, LOSE, play_time);
        updatePlayerTournamentStats(player2, tournament_id, WIN, play_time);
    }
    else
    {
        updatePlayerTournamentStats(player1, tournament_id, DRAW, play_time);
        updatePlayerTournamentStats(player2, tournament_id, DRAW, play_time);
    }
    return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (!isPositive(tournament_id))
    {
        return CHESS_INVALID_ID;
    }
    if (!(mapContains(chess->tournaments, &tournament_id)))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    //updates players stats, removes games from tournament, removes tournament
    MAP_FOREACH(int *, playerKey, chess->players)
    {
        Player player = mapGet(chess->players, playerKey);
        if (isPlayerPlayingInTournament(player, tournament_id))
        {
            Tournament tournament_to_delete = (Tournament)mapGet(chess->tournaments, &tournament_id); // get tournament to get the relevant play_time (needed to update player's TournamentStats)
            int total_play_time_in_tournament = getPlayerTotalPlayTimeInTournament(tournament_to_delete, player);
            removeTournamentUpdateStats(player, tournament_id, total_play_time_in_tournament); // update relevant stats and removes TournamentsStats from player
            mapDestroy(getTournamentGamesMap(tournament_to_delete));                           // remove all games from tournament's game map
        }
        mapRemove(chess->tournaments, &tournament_id); // remove tournament from tournament maps
        freeKeyInt(playerKey);
    }
    return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (!isPositive(player_id))
    {
        return CHESS_INVALID_ID;
    }
    if (!(mapContains(chess->players, &player_id)))
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    Player player = mapGet(chess->players, &player_id);
    Map player_tournament_map = getPlayerTournamentsMap(player);
    MAP_FOREACH(int *, key, player_tournament_map)
    {
        Tournament tournament = mapGet(chess->tournaments, key);
        removePlayerFromTournament(tournament, player_id);
        freeKeyInt(key);
    }
    mapRemove(chess->players, &player_id);
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id);

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult *chess_result);

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE *file);

ChessResult chessSaveTournamentStatistics(ChessSystem chess, char *path_file);
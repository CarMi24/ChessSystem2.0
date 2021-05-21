#include "chessSystem.h"

#include <string.h>
#include <stdlib.h>

#define LOWER_A 'a'
#define LOWER_Z 'z'
#define UPPER_A 'A'
#define UPPER_Z 'Z'
#define SPACE ' '

struct chess_system_t
{
    Map tournaments; /* KEY = tournament_id, DATA = tournament */
    Map players;     /* KEY = player_id, DATA = player */
};

/** Functions for the tournaments map**/

/**
 * Tournament's Key is type of integer pointer - the id of the tournament.
 * Tournament's Data is type of Tournament
*/
static void freeKeyInt(MapKeyElement n)
{
    free(n);
}

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

static int compareKeyInt(MapKeyElement n1, MapKeyElement n2)
{
    return (*(int *)n1 - *(int *)n2);
}

/**
 * returns true if an integer is positive.
 */
static bool isPositive(int number)
{
    return number > 0;
}

/**
 * Check if location is valid
 */
static bool isLocationValid(const char *location)
{
    if (!(location[0] >= UPPER_A && location[0] <= UPPER_Z))
    {
        return false;
    }
    int len = strlen(location);
    for (int i = 1; i < len; i++)
    {
        if ((!(location[i] >= LOWER_A && location[i] <= LOWER_Z)) && location[i] != SPACE)
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

    Player player_1 = (Player)mapGet(chess->players, &player1_id);
    Player player_2 = (Player)mapGet(chess->players, &player2_id);

    if (player_1 != NULL && getTotalGamesByTournament(player_1, tournament_id) >= max_games_per_player)
    {
        return true;
    }
    if (player_2 != NULL && getTotalGamesByTournament(player_2, tournament_id) >= max_games_per_player)
    {
        return true;
    }

    return false;
}
static void bubbleSort(int *levels, int n)
{
}

static bool checkIfGameExists(ChessSystem chess, Game game)
{
    Tournament tmp = NULL;
    MAP_FOREACH(int *, tournament_id, chess->tournaments)
    {
        tmp = mapGet(chess->tournaments, tournament_id);
        if (checkIfGameInTournament(tmp, game))
        {
            return true;
        }
        free(tournament_id);
    }
    return false;
}
/**
 * search for a player in players map of system.
 * if found, returns it, otherwise creates a new player.
 */
static Player getOrCreatePlayerInSystem(ChessSystem chess, int player_id)
{
    Player player = (Player)mapGet(chess->players, &player_id);
    if (player == NULL)
    {
        player = createPlayer(player_id);
    }

    return player;
}
/**
 * if the player does not exist in the system, it means its a new player,
 * put it in the players map and destroys it. (because mapPut creates a copy).
 */
static addNewPlayerToSystem(ChessSystem chess, int player_id, Player player)
{
    if (!mapContains(chess->players, player_id))
    {
        mapPut(chess->players, player_id, player);
        destroyPlayer(player);
    }
}
/**
 * get a valid new game properties, and updates the players in the chess system
 */

static bool updatePlayersMap(ChessSystem chess, int first_player, int second_player, Winner winner, int tournament_id, int play_time)
{
    Player player_1 = getOrCreatePlayerInSystem(chess, first_player);
    Player player_2 = getOrCreatePlayerInSystem(chess, second_player);
    if (player_1 != NULL && player_2 != NULL)
    {
        int index_outcome = winner == DRAW ? TIE : (winner == FIRST_PLAYER ? WIN : LOSE);
        updatePlayerTournamentStats(player_1, tournament_id, index_outcome, play_time);
        updatePlayerTournamentStats(player_2, tournament_id, index_outcome * -1, play_time);
        addNewPlayerToSystem(chess, first_player, player_1);
        addNewPlayerToSystem(chess, second_player, player_2);
        return true;
    }
    if (player_1 == NULL && player_2 == NULL)
    {
        return false;
    }
    if (player_2 != NULL)
    {
        if (!mapContains(chess->players, second_player))
        {
            free(player_2);
        }
        return false;
    }
    if (!mapContains(chess->players, first_player))
    {
        free(player_1);
    }
    return false;
}

static bool checkWinner(int *players_id, int n)
{
    int counter = 0;
    for (int i = 0; i < n; i++)
    {
        if (players_id[i] != 0)
        {
            counter++;
        }
        if (counter > 1)
        {
            return false;
        }
    }
    return true;
}

static int filterByScore(Map players, int *players_id, int n, int tournament_id)
{
    int max_score = -1;
    int id_winner = 0;
    Player tmp = NULL;
    int tmp_score;
    for (int i = 0; i < n; i++)
    {
        tmp = mapGet(players, &i);
        tmp_score = getScoreByTournament(tmp, tournament_id);
        if (tmp_score > max_score)
        {
            max_score = getScoreByTournament(tmp, tournament_id);
            id_winner = players_id[i];
        }
        else if (tmp_score < max_score)
        {
            players_id[i] = 0;
        }
    }
    return id_winner;
}

static int filterByLosses(Map players, int *players_id, int n, int tournament_id)
{
    Player tmp = NULL;
    int tmp_losses = 0;
    int id_winner = -1;
    int min_losses = getLossesByTournament(tmp, tournament_id);
    for (int i = 0; i < n; i++)
    {
        tmp = mapGet(players, &i);
        tmp_losses = getLossesByTournament(tmp, tournament_id);
        if (tmp_losses < min_losses)
        {
            min_losses = getLossesByTournament(tmp, tournament_id);
            id_winner = players_id[i];
        }
        else if (tmp_losses > min_losses)
        {
            players_id[i] = 0;
        }
    }
    return id_winner;
}

static int filterByWins(Map players, int *players_id, int n, int tournament_id)
{
    Player tmp = NULL;
    int tmp_wins = 0;
    int id_winner = -1;
    int max_wins = -1;
    for (int i = 0; i < n; i++)
    {
        tmp = mapGet(players, &i);
        tmp_wins = getWinsByTournament(tmp, tournament_id);
        if (tmp_wins > max_wins)
        {
            max_wins = getWinsByTournament(tmp, tournament_id);
            id_winner = players_id[i];
        }
        else if (tmp_wins < max_wins)
        {
            players_id[i] = 0;
        }
    }
    return id_winner;
}
static int minIdWinner(int *players_id, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (players_id[i] != 0)
        {
            return players_id[i];
        }
    }
    return -1;
}

static int calculateTournamentWinner(Map players, int tournament_id)
{
    int n = mapGetSize(players);
    int *players_id = (int *)malloc(sizeof(int) * n);
    int i = 0;
    MAP_FOREACH(int *, id, players)
    {
        players_id[i] = *id;
        i++;
        free(id);
    }
    int id_winner = filterByScore(players, players_id, n, tournament_id);
    if (checkWinner(players_id, n))
    {
        return id_winner;
    }
    id_winner = filterByLosses(players, players_id, n, tournament_id);
    if (checkWinner(players_id, n))
    {
        return id_winner;
    }
    id_winner = filterByWins(players, players_id, n, tournament_id);
    if (checkWinner(players_id, n))
    {
        return id_winner;
    }
    return minIdWinner(players_id, n);
}

static void removeTouranmentStatsFromPlayers(ChessSystem chess, Tournament tournament_to_delete)
{
    MAP_FOREACH(int *, playerKey, chess->players)
    {
        Player player = mapGet(chess->players, playerKey);
        int total_play_time_in_tournament = getPlayerTotalPlayTimeInTournament(tournament_to_delete, player);
        removeTournamentUpdateStats(player, getTournamentId(tournament_to_delete), total_play_time_in_tournament); // update relevant stats and removes TournamentsStats from player
        freeKeyInt(playerKey);
    }
}

static bool NoTournamentsEnded(Map tournaments_map)
{
    MAP_FOREACH(int *, tournamentKey, tournaments_map)
    {
        Tournament current_tournament = mapGet(tournaments_map, tournamentKey);
        if (getTournamentStatus(current_tournament))
        {
            freeKeyInt(tournamentKey);
            return false;
        }
        freeKeyInt(tournamentKey);
    }
    return true;
}

//updated by Ori marked (**)
ChessSystem chessCreate()
{
    ChessSystem chess_system = malloc(sizeof(*chess_system));
    if (chess_system == NULL)
    {
        return NULL;
    }
    chess_system->tournaments = mapCreate((copyMapDataElements)copyTournament, copyKeyInt, (freeMapDataElements)destroyTournament, freeKeyInt, compareKeyInt);
    if (chess_system->tournaments == NULL)
    {
        free(chess_system); //(**)
        return NULL;
    }
    chess_system->players = mapCreate((copyMapDataElements)copyPlayer, copyKeyInt, (freeMapDataElements)destroyPlayer, freeKeyInt, compareKeyInt);
    if (chess_system->players == NULL)
    {
        mapDestroy(chess_system->tournaments);
        free(chess_system); //(**)
        return NULL;
    }
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

//updated by Ori
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
    if (mapPut(chess->tournaments, &tournament_id, new_tournament) == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    free(new_tournament); //(**)
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
    Tournament tournament_to_add = (Tournament)mapGet(chess->tournaments, &tournament_id);
    if (getTournamentStatus(tournament_to_add))
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    Game new_game = createGame(tournament_id, first_player, second_player, winner, play_time);
    if (new_game == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    if (checkIfGameExists(chess, new_game)) //(**)
    {
        destroyGame(new_game);
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if (play_time < 0)
    {
        destroyGame(new_game);
        return CHESS_INVALID_PLAY_TIME;
    }
    int max_games_per_player = getTournamentMaxGamesPerPlayer(tournament_to_add);
    if (checkExceededGames(chess, first_player, second_player, tournament_id, max_games_per_player))
    {
        destroyGame(new_game);
        return CHESS_EXCEEDED_GAMES;
    }
    if (!updatePlayersMap(chess, first_player, second_player, winner, tournament_id, play_time))
    {
        return CHESS_OUT_OF_MEMORY;
    }
    addGameToTournament(tournament_to_add, new_game); //return value? it's void.
    updateLongestGameTime(tournament_to_add, play_time);
    destroyGame(new_game);
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
    Tournament tournament_to_delete = (Tournament)mapGet(chess->tournaments, &tournament_id);
    removeTouranmentStatsFromPlayers(chess, tournament_to_delete);
    mapRemove(chess->tournaments, &tournament_id); // remove tournament from tournament maps

    return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id) // not finished
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
    Player player = (Player)mapGet(chess->players, &player_id);
    Map player_tournament_map = getPlayerTournamentsMap(player);
    MAP_FOREACH(int *, key, player_tournament_map)
    {
        Tournament tournament = mapGet(chess->tournaments, key);
        removePlayerFromTournament(tournament, player_id);
        freeKeyInt(key);
    }
    mapRemove(chess->players, &player_id);
    player_tournament_map = NULL;
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (!isPositive(tournament_id))
    {
        return CHESS_INVALID_ID;
    }
    Tournament tournament_to_end = (Tournament)mapGet(chess->tournaments, &tournament_id);
    if (tournament_to_end == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (!getTournamentStatus(tournament_to_end))
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    if (mapGetSize(getTournamentGamesMap(tournament_to_end)) == 0)
    {
        return CHESS_NO_GAMES;
    }
    int winner_id = calculateTournamentWinner(chess->players, tournament_id);
    assert(winner_id != -1);
    endTournament(tournament_to_end, winner_id);
    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult *chess_result)
{
    if (chess == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return -1.0;
    }
    if (!isPositive(player_id))
    {
        *chess_result = CHESS_INVALID_ID;
        return -1.0;
    }
    Player player = mapGet(chess->players, &player_id);
    if (player == NULL)
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return -1.0;
    }

    return calculateAveragePlayTime(player);
}

//ChessResult chessSavePlayersLevels(ChessSystem chess, FILE *file);

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE *file) //WHEN DO YOU RETURN CHESS_AVE_FALIURE?
{
    if (chess == NULL || file == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    //creates a "PlayerLevel" Map, Key=struct of PlayerLevel, Data=player_id
    Map player_level_map = mapCreate((copyMapDataElements)copyKeyInt, (copyMapKeyElements)copyPlayerLevel,
                                     (freeMapDataElements)freeKeyInt, (freeMapKeyElements)destroyPlayerLevel,
                                     (compareMapKeyElements)comparePlayerLevel);
    if (player_level_map == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult map_result;
    MAP_FOREACH(int *, keyInt, chess->players) // run on players map, for each player create new PlayerLevel struct and add it to player_level_map
    {
        Player player = mapGet(chess->players, keyInt);
        PlayerLevel new_player_level = createPlayerLevel(*keyInt, getPlayerLevel(player));
        map_result = mapPut(player_level_map, new_player_level, keyInt);
        if (map_result == MAP_OUT_OF_MEMORY)
        {
            return CHESS_OUT_OF_MEMORY;
        }
        free(keyInt);
    }

    MAP_FOREACH(PlayerLevel, key, player_level_map) // run on PlayerLevelMap and print id and level of each player
    {
        if (fprintf(file, "%d %.2f\n", getPlayerLevelId(key), getPlayerLevelLevel(key)) < 0)
        {
            destroyPlayerLevel((PlayerLevel)key);
            return CHESS_SAVE_FAILURE;
        }
        destroyPlayerLevel((PlayerLevel)key);
    }
    mapDestroy(player_level_map);
    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics(ChessSystem chess, char *path_file)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (noTournamentsEnded(chess->tournaments))
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    FILE *file = fopen(path_file, 'w');
    if (file == NULL)
    {
        return CHESS_SAVE_FAILURE;
    }

    Tournament tournament = NULL;
    Map tournament_game_map = NULL;
    MAP_FOREACH(int *, tournamentKey, chess->tournaments)
    {
        if (getTournamentStatus(tournament))
        {
            tournament = mapGet(chess->tournaments, tournamentKey);
            tournament_game_map = getTournamentGamesMap(tournament);
            if (fprintf("%d\n%d\n%.2f\n%s\n%d\n%d\n",
                        getTournamentWinnerId(tournament),
                        getTournamentLongestGameTime(tournament),
                        calculateTournamentAverageGameTime(tournament),
                        getTournamentLocation(tournament),
                        mapGetSize(tournament_game_map),
                        getTournamentTotalPlayers(tournament)) < 0)
            {
                freeKeyInt(tournamentKey);
                fclose(file);
                return CHESS_SAVE_FAILURE;
            }
        }
        freeKeyInt(tournamentKey);
    }
    fclose(file);
    return CHESS_SUCCESS;
}
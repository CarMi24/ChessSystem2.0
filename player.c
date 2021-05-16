#include "player.h"
#include <stdlib.h>

struct Player_t
{
    int player_id;
    int total_wins;
    int total_losses;
    int total_draws;
    double level;
    int total_play_time;
    Map tournament_rates; // KeyElement = tournament_id, DataElement = TournamentStats (struct)
};

/** Functions for the tournaments map**/
static MapKeyElement copyKeyInt(MapKeyElement n)
{
    if (n == NULL)
    {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    *copy = *(int *)n;
    return copy;
}
/**
 * Alloc a new TournamentStat struct.
 * copies the stats from a give TournamentStat and returns the copy
 */
static TournamentStats copyStats(TournamentStats stats)
{
    TournamentStats clone = malloc(sizeof(*clone));
    clone->wins = stats->wins;
    clone->losses = stats->losses;
    clone->draws = stats->draws;
    clone->total_games_in_tournament = stats->total_games_in_tournament;
    return clone;
}
static MapDataElement coptDataTournamentStats(MapDataElement tournament_stats)
{
    if (tournament_stats == NULL)
    {
        return NULL;
    }
    TournamentStats copy = malloc(sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    copy = copyStats((TournamentStats)tournament_stats);
    return copy;
}

static void freeKeyInt(MapKeyElement n)
{
    free(n);
}

static void freeDataTournamentStats(MapDataElement tournament_stats)
{
    free(tournament_stats);
}

static int compareInts(MapKeyElement n1, MapKeyElement n2)
{
    return (*(int *)n1 - *(int *)n2);
}
/** Functions for the tournaments map**/

static void updateTotal(int index_outcome) //?
{
}

Player createPlayer(int player_id)
{
    Player player = malloc(sizeof(*player));
    player->player_id = player_id;
    player->total_wins = 0;
    player->total_losses = 0;
    player->total_draws = 0;
    player->total_play_time = 0;
    player->total_draws = 0;
    Map tournament_rates = mapCreate(copyKeyInt, coptDataTournamentStats, freeKeyInt, freeDataTournamentStats, compareInts);
    if (tournament_rates == NULL) // if map allocation failed or recieved null values
    {
        free(player);
        return NULL;
    }
    player->tournament_rates = tournament_rates;
}

void destroyPlayer(Player player)
{
    if (player == NULL)
    {
        return NULL;
    }
    mapDestroy(player->tournament_rates);
    free(player);
}

double calculateAveragePlayTime(Player player)
{
    return (((double)(player->total_play_time)) / (double)(player->total_wins + player->total_losses + player->total_draws));
}

/**/ void updatePlayerTournamentStats(Player player, int tournament_id, int index_outcome)
{
}

PlayerResult removeTournament(Player player, int tournament_id)
{
    mapRemove(player->tournament_rates, tournament_id);
}

Map getPlayerTournamentsMap(Player player)
{
    return player->tournament_rates;
}

double calculatePlayersLevel(Player player, int num_of_games)
{
    return ((double)((6 * player->total_wins) - (10 * player->total_losses) + (2 * player->total_draws)) / ((double)num_of_games));
}

int getGamesPlayedInTournament(Player player, int tournament_id)
{
    TournamentStats stats = mapGet(player->tournament_rates, tournament_id);
    if (stats == NULL)
    {
        return 0;
    }
    return stats->total_games_in_tournament;
}

/**
 * Calculate and returns the Player score in a given tournament
 * returns -1 if the player didnt play in the given tournament
 */
int getScoreByTournament(Player player, int tournament_id)
{
    TournamentStats stats = mapGet(player->tournament_rates, tournament_id);
    if (stats == NULL)
    {
        return -1;
    }
    return (2 * (stats->wins) + stats->draws);
}

/**
 * Gets 2 players with the same score in the same tournament.
 * return the id of the one should be considered the winner:
 * first by comparing wins, then losses, draws and finally ID.
 */
int compareSameTournamentScore(Player player1, Player player2)
{
}

/**
 * Calculates and return the players general level in the system
 */
double calculatePlayerLevel(Player player, int total_games)
{
}

int compareSameLevelPlayers(Player player1, Player player2)
{
    if (player1->player_id < player2->player_id)
    {
        return player1->player_id;
    }
    return player2->player_id;
}

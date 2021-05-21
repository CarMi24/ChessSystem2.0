
  
#include "player.h"
#include "stdbool.h"
#include <stdlib.h>
#include "assert.h"


#define NO_SCORE -1

#define TOTAL_GAMES ((player->total_wins) + (player->total_losses) + (player->total_draws))
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

struct TournamentStats_t
{
    int wins;
    int losses;
    int draws;
    int total_games_in_tournament;
};

static TournamentStats creatTournamentStats(int index_outcome)
{
    TournamentStats new_stats = (TournamentStats)malloc(sizeof(*new_stats));
    if(new_stats == NULL)
    {
        return NULL;
    }
    new_stats->draws = index_outcome == TIE ? 1:0;
    new_stats->losses = index_outcome == LOSE ? 1:0;
    new_stats->wins = index_outcome == WIN ? 1:0;
    new_stats->total_games_in_tournament = 1;
    return new_stats;
}

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
// static MapDataElement copyDataTournamentStats(MapDataElement tournament_stats)
// {
//     if (tournament_stats == NULL)
//     {
//         return NULL;
//     }
//     TournamentStats copy = malloc(sizeof(*copy));
//     if (copy == NULL)
//     {
//         return NULL;
//     }
//     copy = copyStats((TournamentStats)tournament_stats);
//     return copy;
// }

static void freeKeyInt(MapKeyElement n)
{
    free(n);
}

static void freeTournamentStats(TournamentStats tournament_stats)
{
    free(tournament_stats);
}

static int compareInts(MapKeyElement n1, MapKeyElement n2)
{
    return (*(int *)n1 - *(int *)n2);
}
/** Functions for the tournaments map**/

static void updateTotal(Player player, int index_outcome, int play_time)
{
   
    player->total_wins = index_outcome == WIN ?  player->total_wins+1: player->total_wins;
    player->total_draws = index_outcome == LOSE ?  player->total_losses+1 : player->total_losses; 
    player->total_losses = index_outcome == TIE ? player->total_draws+1 : player->total_draws;   
    player->total_play_time += play_time;
    player->level = calculatePlayerLevel(player);
}


static double calculatePlayerLevel(Player player)
{
    return ((double)((6 * player->total_wins) - (10 * player->total_losses) + (2 * player->total_draws)) / ((double)TOTAL_GAMES));
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
    player->level = 0.0;
    player->tournament_rates = mapCreate(copyKeyInt, (copyMapDataElements)copyStats, freeKeyInt, (freeMapDataElements)freeTournamentStats, compareInts);
    if (player->tournament_rates == NULL) // if map allocation failed or recieved null values
    {
        free(player);
        return NULL;
    }
    
    return player;
}

Player copyPlayer(Player player)
{
    assert(player!=NULL);
    
    Player copy_player = malloc(sizeof(*copy_player));
    if (copy_player == NULL)
    {
        return NULL;
    }
    copy_player->player_id = player->player_id;
    copy_player->total_wins = player->total_wins;
    copy_player->total_losses = player->total_losses;
    copy_player->total_draws = player->total_draws;
    copy_player->level = player->level;
    copy_player->total_play_time = player->total_play_time;
    copy_player->tournament_rates = mapCopy(player->tournament_rates);
    if (copy_player->tournament_rates == NULL)
    {
        free(copy_player);
        return NULL;
    }
    return copy_player;
}

void destroyPlayer(Player player)
{
    if (player == NULL)
    {
        return;
    }
    mapDestroy(player->tournament_rates);
    free(player);
}

double calculateAveragePlayTime(Player player)
{
    return (((double)(player->total_play_time)) / (double)TOTAL_GAMES);
}

// void updatePlayerTournamentStats(Player player, int tournament_id, int index_outcome, int play_time)
// {
//     TournamentStats stats_to_update = (TournamentStats)mapGet(player->tournament_rates, &tournament_id);
//     if (index_outcome == WIN)
//     {
//         stats_to_update->wins = stats_to_update->wins + 1;
//     }
//     if (index_outcome == DRAW)
//     {
//         stats_to_update->draws = stats_to_update->draws + 1;
//     }
//     if (index_outcome == LOSE)
//     {
//         stats_to_update->losses = stats_to_update->losses + 1;
//     }
//     stats_to_update->total_games_in_tournament = stats_to_update->total_games_in_tournament + 1;
//     updateTotal(player, index_outcome, play_time);
// }

bool updatePlayerTournamentStats(Player player, int tournament_id, int index_outcome, int play_time)
{
    TournamentStats to_update = (TournamentStats)mapGet(player->tournament_rates, &tournament_id);
    if(to_update==NULL)
    {
        TournamentStats temp_stats = creatTournamentStats(index_outcome);
        if(temp_stats == NULL)
        {
            return false;
        }
        mapPut(player->tournament_rates,&tournament_id,temp_stats);
        free(temp_stats);
        return true;
    }
    to_update->draws = index_outcome == TIE ? to_update->draws+1:to_update->draws;
    to_update->losses = index_outcome == LOSE ? to_update->losses+1:to_update->losses;
    to_update->wins = index_outcome == WIN ? to_update->wins+1:to_update->wins;
    to_update->total_games_in_tournament++;
    return true;
}

void removeTournamentUpdateStats(Player player, int tournament_id, int total_game_time_to_update)
{
    TournamentStats stats_to_update = mapGet(player->tournament_rates, &tournament_id);
    if(stats_to_update == NULL)
    {
        return;
    }
    player->total_wins = player->total_wins - stats_to_update->wins;
    player->total_losses = player->total_losses - stats_to_update->losses;
    player->total_draws = player->total_draws - stats_to_update->draws;
    player->total_play_time = player->total_play_time - total_game_time_to_update;
    player->level = calculatePlayerLevel(player);
    mapRemove(player->tournament_rates, &tournament_id);
}

Map getPlayerTournamentsMap(Player player)
{
    return player->tournament_rates;
}

int getPlayerId(Player player)
{
    return player->player_id;
}


/**
 * Calculate and returns the Player score in a given tournament
 * returns -1 if the player didnt play in the given tournament
 */
int getScoreByTournament(Player player, int tournament_id)
{
    TournamentStats stats = mapGet(player->tournament_rates, &tournament_id);
    if (stats == NULL)
    {
        return NO_SCORE;
    }
    return (WIN * (stats->wins) + stats->draws);
}

int getWinsByTournament(Player player, int tournament_id)
{
    TournamentStats tmp = (TournamentStats)mapGet(player->tournament_rates,&tournament_id);
    if(tmp == NULL)
    {
        return 0;
    }
    return tmp->wins;
}

int getLossesByTournament(Player player, int tournament_id)
{
    TournamentStats tmp = (TournamentStats)mapGet(player->tournament_rates,&tournament_id);
    if(tmp == NULL)
    {
        return 0;
    }
    return tmp->losses;
}

int getDrawsByTournament(Player player, int tournament_id)
{
    TournamentStats tmp = (TournamentStats)mapGet(player->tournament_rates,&tournament_id);
    if(tmp == NULL)
    {
        return 0;
    }
    return tmp->draws;
}

int getTotalGamesByTournament(Player player, int tournament_id)
{
    TournamentStats tmp = (TournamentStats)mapGet(player->tournament_rates,&tournament_id);
    if(tmp == NULL)
    {
        return 0;
    }
    return tmp->total_games_in_tournament;
}

double getPlayerLevel(Player player)
{
    return player->level;
}

bool checkIfPlayerInTournament(Player player, int tournament_id)
{
    if (mapContains(player->tournament_rates, &tournament_id))
    {
        return true;
    }
    return false;
}
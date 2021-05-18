#ifndef PLAYER_H
#define PLAYER_H


#include "map.h"
#include "game.h"

#define LOSE 0
#define DRAW 1
#define WIN 2


/** Type for defining the PlayerResult*/
typedef enum PlayerResult_t
{
    PLAYER_OUT_OF_MEMORY,
    PLAYER_NULL_ARGUMENT,
    PLAYER_ERROR,
    PLAYER_SUCCESS
}PlayerResult;

/** Type for defining the Player*/
typedef struct Player_t *Player;

typedef struct TournamentStats_t *TournamentStats;


Player createPlayer(int player_id);

/**
 * allocates and returns a new copy of a given player
 */
Player copyPlayer(Player player);

void destroyPlayer(Player player);

/**
 * Calculate and returns the average play time in all the player's games.
 */
double calculateAveragePlayTime(Player player);

/**
 * updates a new outcome of a game to a tournament stats - index_outcome - LOSE 0, DRAW 1, WIN 2
 * if tournament doesnt exist adds a new one with initial tournament stat.
 */
void updatePlayerTournamentStats(Player player, int tournament_id, int index_outcome, int play_time);

/**
 * Removes a tournament from the players tournaments maps.
 * Updates the player's stats (wins, losses, draws).
 *  @return -
 * PLAYER_NULL_ARGUMENT
 */
void removeTournamentUpdateStats(Player player, int tournament_id,int total_game_time_to_update);

/**
 * returns the players tournaments map.
 */
Map getPlayerTournamentsMap(Player player);

/**
 * returns player's id
 */
int getPlayerId(Player player);

/**
 * Calculate and returns the player general level.
 */
double calculatePlayersLevel(Player player, int num_of_games);

/**
 * returns 0 if never played in the tournament
 */
int getGamesPlayedInTournament(Player player, int tournament_id);

/**
 * Calculate and returns the Player score in a given tournament
 */
int getScoreByTournament(Player player, int tournament_id);

/**
 * Gets 2 players with the same score in the same tournament.
 * return the id of the one should be considered the winner:
 * first by comparing wins, then losses, draws and finally ID.
 */
int compareSameTournamentScore(Player player1, Player player2);

/**
 * Calculates and return the players general level in the system
 */
double calculatePlayerLevel(Player player, int total_games);

/**
 * Compare players with the same general level
 * returns - the player id with the smaller id
 */
int compareSameLevelPlayers(Player player1, Player player2);

/**
 * returns true if player plays in the given tournament, false otherwise
 */
bool isPlayerPlayingInTournament(Player player, int tournament_id);


#endif
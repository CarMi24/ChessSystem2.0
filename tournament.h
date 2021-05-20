#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "map.h"
#include "game.h"
#include "player.h"

#define NULL_ID -1

/** Type for defining the Tournament */
typedef enum TournamentResult_t
{
    TOURNAMENT_OUT_OF_MEMORY,
    TOURNAMENT_NULL_ARGUMENT,
    TOURNAMENT_ERROR,
    TOURNAMENT_SUCCESS

} TournamentResult;

//typedef struct Tournament_t *Tournament;

typedef struct Tournament_t *Tournament;

/**
* createTournament: Allocates a new empty tournament.
* @return
* 	NULL - if allocations failed.
* 	A new Tournament in case of success.
*/
Tournament createTournament(int tournament_id, const char *location, int max_games_per_player);

/**
 * allocates and returns a copy of a given tournament
 */
Tournament copyTournament(Tournament tournament);

/**
 * Get functions  NEED COPY OR BY VALUE?
 */
int getTournamentId(Tournament tournament);
char *getTournamentLocation(Tournament tournament);
Map getTournamentGamesMap(Tournament tournament);
int getTournamentMaxGamesPerPlayer(Tournament tournament);
int getTournamentWinnerId(Tournament tournament);
int getTournamentLongestGameTime(Tournament tournament);
bool getTournamentIsClosed(Tournament tournament);

void destroyTournament(Tournament tournament);

/**
 * Check if the game already taken place in the tournament
 */
bool checkIfGameExists(Tournament tournament, Game game);

/**
 * Adds a game to the games map.
 */
TournamentResult addGameToTournament(Tournament tournament, Game game);

/**
 * Iterates the games map and remove the player from each game he plays in.
 * @return -
 * TOURNAMENT_NULL_ARGUMENT
 * TOURNAMENT_ERROR
 */
TournamentResult removePlayerFromTournament(Tournament tournament, int player_id);

/**
 * Closes the tournament - Set the tournament is_closed value to true.
 * @return -
 * TOURNAMENT_NULL_ARGUMENT
 */
TournamentResult endTournament(Tournament tournament, int winner_id);

/**
 * returns the tournament longest play time.
 */
int getTournamentLongestGameTime(Tournament tournament);

/**
 * Calculate and returns the tournament average play time.
 */
double calculateAverageTournamentGameTime(Tournament tournament);

/**
 * returns the tournament status: false if open true if closed.
 */
bool getTournamentStatus(Tournament Tournament);

/**
 * prints the tournaments statistics to the a file located in the given path_file.
 * @return -
 * TOURNAMENT_NULL_ARGUMENT
 * TOURNAMENT_ERROR - if tournaments closed
 */
TournamentResult printTournamentStats(Tournament tournament, char *path_file);

/**
 * calculates and returns player's total play time in tournament
 */
int getPlayerTotalPlayTimeInTournament(Tournament tournament, Player player);
#endif

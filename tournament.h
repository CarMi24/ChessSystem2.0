#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "map.h"
#include "game.h"
#include "player.h"

#define NULL_ID -1


typedef struct Tournament_t *Tournament;


/** Functions for the games map**/

/**
 * GameKey is type of integer pointer - the number of the game in the tournament.
 * GameData is type of Game - games that are taken place in the tournament.
 * The key will be the tournament_id where the game takes place.
*/
static MapKeyElement copyGameKey(MapKeyElement game_key);
static MapDataElement copyGameData(MapDataElement game);
static void freeGameKey(MapKeyElement game_key);
static void freeGameData(MapDataElement game);
static int compareKeyGame(MapKeyElement game1, MapKeyElement game2);


/**
 * updates the longest game time if new_time is bigger.
 * otherwise does nothing. 
 */
static void updateLongestGameTime(Tournament tournament, int new_time);



/**
* createTournament: Allocates a new empty tournament.
* @return
* 	NULL - if allocations failed.
* 	A new Tournament in case of success.
*/
Tournament createTournament(int tournament_id, char* location, int max_games_per_player);


/**
 * Deallocates the tournament and free all memory allocated in tournament properties.
 */
void destroyTournament(Tournament tournament);


/**
* Allocates a new empty tournament.
* Copies the data of the given tournament to the new allocated one.
* @return
* 	NULL - if allocations failed.
* 	A clone of the given Tournament in case of success.
*/
Tournament copyTournament(Tournament tournament);


/**
 * Check if the game already taken place in the tournament
 */
bool checkIfGameInTournament(Tournament tournament, Game game);


/**
 * Adds a game to the games map.
 */
void addGameToTournament(Tournament tournament, Game game);

/**
 * Iterates the games map and remove the player from each game he plays in.
 */
void removePlayerFromTournament(Tournament tournament, int player_id);

/**
 * Closes the tournament - Set the tournament is_closed value to true.
 */
void endTournament(Tournament tournament, int winner_id);

/**
 * returns the tournament longest play time.
 */
int getTournamentLongestGameTime(Tournament tournament);

/**
 * Calculate and returns the tournament average play time.
 */
double calculateAverageTournamentGameTime(Tournament tournament);

/**
 * prints the tournaments statistics to the a file located in the given path_file.
 */
void printTournamentStats(Tournament tournament,char* path_file);



int getPlayerTotalPlayTimeInTournament(Tournament tournament, Player player);


/**
 * Get funcs for Tournament's properties
 */
int getTournamentId(Tournament tournament);
char *getTournamentLocation(Tournament tournament);
Map getTournamentGamesMap(Tournament tournament);
int getTournamentMaxGamesPerPlayer(Tournament tournament);
int getTournamentWinnerId(Tournament tournament);
int getTournamentLongestGameTime(Tournament tournament);
bool getTournamentStatus(Tournament tournament);

#endif
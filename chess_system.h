#ifndef CHESS_SYSTEM_H
#define CHESS_SYSTEM_H
#include "map.h"
#include "game.h"
#include "tournament.h"


/** Type for defining the ChessResult */
typedef enum ChessResult_t
{
    CHESS_OUT_OF_MEMORY,
    CHESS_NULL_ARGUMENT,
    CHESS_SUCCESS,
    CHESS_INVALID_ID,
    CHESS_TOURNAMENT_ALREADY_EXISTS,
    CHESS_TOURNAMENT_NOT_EXISTS,
    CHESS_TOURNAMENT_ENDED,
    CHESS_GAME_ALREADY_EXISTS,
    CHESS_INVALID_LOCATION,
    CHESS_INVALID_MAX_GAMES,
    CHESS_INVALID_PLAY_TIME,
    CHESS_EXCEEDED_GAMES

} ChessResult;

/** Type for defining the Chess System */
typedef struct ChessSystem_t
{
    Map tournaments;
    Map players;
} *ChessSystem;

/** Functions for the tournaments map**/

/**
 * TournamentKey is type of integer pointer - the id of the tournament.
 * TournamentData is type of Tournament - all tournaments that are taking place in the system.
*/
static void freeKeyTournament(MapKeyElement tournament_key);
static void freeDataTournament(MapDataElement tournament_data);
static MapKeyElement copyKeyTournament(MapKeyElement tournament_key);
static MapDataElement copyElementData(MapDataElement tournament_data);
static int compareKeyTournament(MapKeyElement tournament_key1, MapKeyElement tournament_key2);
/** Functions for the players map**/

/**
 * player_key is type of integer pointer - the id of the tournament.
 * tournament_data is type of Tournament - all tournaments that are taking place in the system.
*/
static void freeKeyPlayer(MapKeyElement player_key);
static void freeDataPlayer(MapDataElement player_data);
static MapKeyElement copyKeyPlayer(MapKeyElement player_key);
static MapDataElement copyDataPlayer(MapDataElement player_data);
static int compareKeyPlayer(MapKeyElement player_key1, MapKeyElement player_key2);

/** Static Functions for convinience **/


/**
 * returns if an integer is positive.
 */
static bool isPositive(int number);

/**
 * Check if location is valid
 */
static bool isLocationValid(const char* location);

/**
 * check each player games played in the tournament in comparison to the max allowed
 * returns true if both under the limit otherwise false
 */
static bool checkExceededGames(int player1_id, int player2_id,int tournament_id, int max_games_per_player);

/**
 * calculates and return the ID of the player who won the tournament.
 */

static int calculateTournamentWinner(Map players, int tournament_id);

/**
 * Iterates the players map and sums the players who played in a given tournament
 * returns the number of players
 */
static int calculateNumOfPlayersInTournament(Map players, int tournament_id);

/** Chess System Application **/

/**
 * Allocates a new empty ChessSystem.
 * Creates a map for tournaments and players
 */
ChessSystem chessCreate();

/**
 * Deallocates a given ChessSystem.
 */
void chessDestroy(ChessSystem chess);

/**
 * Adds a new tournament to the system
 * @param chess - the system to add the tournament to.
 * @param tournament_id - must be positive integer.
 * @param max_games_per_player - must be a positive integer.
 * @param tournament_location - must start with a Capital letter and english lowercase letters only.
 * @return -
 * CHESS_OUT_OF_MEMORY
 * CHESS_NULL_ARGUMENT
 * CHESS_INVALID_ID
 * CHESS_TOURNAMENT_ALREADY_EXISTS
 * CHESS_INVALID_LOCATION
 * CHESS_INVALID_MAX_GAMES
 * CHESS_SUCCESS
 */
ChessResult chessAddTournament(ChessSystem chess, int tournament_id,
                         int max_games_per_player, const char* tournament_location);


/**
 * Adds a new game to the system
 * @param chess - the system to add the game to.
 * @param tournament_id - must be positive integer.
 * @param first_player - must be positive integer.
 * @param second_player - must be positive integer.
 * @param winner - must be positive integer.
 * @param play_time - must be positive integer.
 * @return -
 * CHESS_OUT_OF_MEMORY
 * CHESS_NULL_ARGUMENT
 * CHESS_INVALID_ID - also if the players have the same ID.
 * CHESS_TOURNAMENT_NOT_EXISTS
 * CHESS_TOURNAMENT_ENDED
 * CHESS_GAME_ALREADY_EXISTS - in THIS tournament.
 * CHESS_INVALID_PLAY_TIME
 * CHESS_EXCEEDED_GAMES - if a player played the max games he can in this tournament.
 * CHESS_SUCCESS
 */
ChessResult chessAddGame(ChessSystem chess, int tournament_id,
                        int first_player, int second_player,
                        Winner winner, int play_time);

/**
 * Removes a tournament from the system.
 * Updates the players statistics accordingly.
 * @param chess - the system to add the game to.
 * @param tournament_id - must be positive integer.
 * @return -
 * CHESS_NULL_ARGUMENT
 * CHESS_INVALID_ID
 * CHESS_TOURNAMENT_NOT_EXISTS
 */
ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id);

/**
 * Removes a player from the system.
 * Updates the players statistics accordingly.
 * @param chess - the system to add the game to.
 * @param tournament_id - must be positive integer.
 * @return -
 * CHESS_NULL_ARGUMENT
 * CHESS_INVALID_ID
 * CHESS_TOURNAMENT_NOT_EXISTS
 */
ChessResult chessRemovePlayer(ChessSystem chess, int player_id);


#endif
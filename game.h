#ifndef GAME_H
#define GAME_H

#define PLAYER_1 1
#define PLAYER_2 2
typedef enum GameResult_t
{
    GAME_OUT_OF_MEMORY,
    GAME_NULL_ARGUMENT,
    GAME_ERROR

}GameResult;
typedef enum Winner_t
{
    FIRST=1,
    SECOND,
    DRAW
} Winner;

/** Type for defining the Game */
typedef struct Game_t
{
     int first_player;
    int second_player;
    Winner winner;
    int play_time;
    int tournament_id;
}
 *Game;

static void editGameWinner(Winner winner);
/**
 * Alloc new memory for a game and copy the data from the given one.
 * returns the clone
 */
Game copyGame(Game game);
/**
 * Alloc new memory for a game and sets the data recieved.
 * returns the game.
 */
Game createGame(int first_player, int second_player,
                Winner winner, int play_time);


/**
 * Dealloc all memory used by the game struct
 */
void destroyGame(Game game);

/**
 * Check if games have the same opponents as the other
 */
bool compareGames(Game game1, Game game2);

/**
 * Check and return if a player plays in a given game.
 */
bool isPlayerInGame(Game game, int player_id);

/**
 * Given a player_id - removes it from the given game.
 * then, updates the winner using editGameWinner static funct.
 * if player_id does not match any of the players does nothing.
 */
void removePlayer(Game game, int index_player);

/**
 * returns the game time of a game
 */
int getGameTime(Game game);

#endif
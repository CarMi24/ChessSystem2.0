#ifndef GAME_H
#define GAME_H







/** Type for defining the Game */
typedef struct Game_t *Game;

typedef enum Winner_t GameWinner;

static void editGameWinner(GameWinner winner);
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
                GameWinner winner, int play_time);


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
void removePlayerFromGame(Game game, int index_player);

/**
 * Get funcs for players id and playtime
 */
int getGamePlayTime(Game game);
int getFirstPlayerId(Game game);
int getSecondPlayerId(Game game);


#endif
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
typedef struct Game_t *Game;

static void editGameWinner(Winner winner);

Game copyGame(Game game);

Game createGame(int first_player, int second_player,
                Winner winner, int play_time);


void destroyGame(Game game);

bool compareGames(Game game1, Game game2);

/**
 * Check and return if a player plays in a given game.
 */
bool isPlayerInGame(Game game, int player_id);

void removePlayer(Game Game, int index_player);

#endif
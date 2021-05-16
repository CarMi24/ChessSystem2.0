#include "game.h"
#include <stdbool.h>
#include <stdlib.h>

struct Game_t
{
    int first_player;
    int second_player;
    Winner winner;
    int play_time;
    int tournament_id;
};

static void editGameWinner(Game game, Winner winner)
{
    game->winner = winner;
}

Game createGame(int tournament_id, int first_player, int second_player,
                Winner winner, int play_time, GameResult game_result)
{
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    {
        game_result = GAME_OUT_OF_MEMORY;
        return NULL;
    }
    game->tournament_id = tournament_id;
    game->first_player = first_player;
    game->second_player = second_player;
    game->winner = winner;
    game->play_time = play_time;
    return game;
}

void destroyGame(Game game)
{
    if (game == NULL)
    {
        return NULL;
    }
    free(game);
}

bool compareGames(Game game1, Game game2)
{
    if ((game1->first_player == game2->first_player) && (game1->second_player == game2->second_player))
    {
        return true;
    }
    return false;
}

bool isPlayerInGame(Game game, int player_id)
{
    return ((game->first_player == player_id) || (game->second_player == player_id) ? true : false);
}
void removePlayer(Game game, int player_id)
{
    if (game->first_player == player_id)
    {
        game->first_player = 0;
    }
    else
    {
        game->second_player = 0;
    }
}
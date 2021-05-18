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
                Winner winner, int play_time)
{
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    {
        return NULL;
    }
    game->tournament_id = tournament_id;
    game->first_player = first_player;
    game->second_player = second_player;
    game->winner = winner;
    game->play_time = play_time;
    return game;
}
Game copyGame(Game game)
{
    Game newGame = malloc(sizeof(*newGame));
    if (newGame == NULL)
    {
        return NULL;
    }

    newGame->first_player = game->second_player;
    newGame->second_player = game->second_player;
    newGame->winner = game->winner;
    newGame->play_time = game->play_time;
    newGame->tournament_id = game->tournament_id;
    return newGame;
}

void destroyGame(Game game)
{
    if (game == NULL)
    {
        return;
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
void removePlayerFromGame(Game game, int player_id)
{
    if (game->first_player == player_id)
    {
        game->first_player = 0;
        editGameWinner(game, SECOND);
    }
    else
    {
        game->second_player = 0;
        editGameWinner(game, FIRST);
    }
}
int getFirstPlayerId(Game game)
{
    return game->first_player;
}
int getSecondPlayerId(Game game)
{
    return game->second_player;
}
int getGamePlayTime(Game game)
{
    return game->play_time;
}

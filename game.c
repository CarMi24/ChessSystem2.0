#include "game.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct Game_t
{
    int first_player;
    int second_player;
    int winner;
    int play_time;
    int tournament_id;
};


static void editGameWinner(Game game, int winner)
{
    game->winner = winner;
}

Game createGame(int tournament_id, int first_player, int second_player,
                int winner, int play_time)
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
    Game new_game = malloc(sizeof(*new_game));
    if (new_game == NULL)
    {
        return NULL;
    }

    new_game->first_player = game->second_player;
    new_game->second_player = game->second_player;
    new_game->winner = game->winner;
    new_game->play_time = game->play_time;
    new_game->tournament_id = game->tournament_id;
    return new_game;
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
    return ((game->first_player == player_id) || (game->second_player == player_id));
}

void removePlayerFromGame(Game game, int player_id)
{
    assert(game!=NULL);

    if (game->first_player == player_id)
    {
        game->first_player *= -1;
        editGameWinner(game, SECOND_PLAYER);
    }
    else if(game->second_player == player_id)
    {
        game->second_player *= -1;
        editGameWinner(game, FIRST_PLAYER);
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
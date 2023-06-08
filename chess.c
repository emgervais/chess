#include "chess.h"

void test(void *param)
{
    t_game *game = param;
    if(mlx_is_key_down(game->mlx, MLX_KEY_Q))
    {
        fill_move(game, game->white);
        t_player *temp = game->white;
        t_move *temp2;

        while (temp)
        {
            temp2 = temp->moves;
            while(temp2->close)
                temp2 = temp2->next;
            temp = temp->next;
        }
    }
}
int main(int ac, char **av)
{
    if(ac != 2 || (*av[1] != '0' && *av[1] != '1'))
        return(printf("choose mode solo '0' or duo '1'\n"), 1);
    t_game *game = malloc(sizeof(t_game));
    game->img = malloc(sizeof(t_img));
    t_player *temp;
    t_move *temp2;

    game->gamemode = 0;
    if(*av[1] == '1')
        game->gamemode = 1;
    game->turn = 0;
    game->check = 0;
    game->locked = 0;
    game->pass = 0;
    game->castle = 0;
    game->saved = malloc(sizeof(t_move));
    game->saved->to = NULL;
    game->saved->from = NULL;
    init_game(game);
    mlx_cursor_hook(game->mlx, &mouse, game);
    mlx_mouse_hook(game->mlx, &click, game);
    mlx_loop_hook(game->mlx, &test, game);
    temp = game->white;
    //while(temp)
    //{
    //    temp2 = temp->moves;
    //    while(temp2->close)
    //    {
    //        printf("piece ; %d from %d to %d\n", temp->piece, temp2->from->square, temp2->to->square);
    //        temp2 = temp2->next;
    //    }
    //    temp = temp->next;
    //}
    mlx_loop(game->mlx);
    mlx_terminate(game->mlx);
}
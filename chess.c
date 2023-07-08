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
void init_g(t_game *game)
{
    game->turn = 0;
    game->check = 0;
    game->locked = 0;
    game->fd = 0;
    game->file = 0;
    game->pass = 0;
    game->castle = 0;
    game->turn_num = 1;
    game->gamemode = 0;
    game->log = NULL;
    game->img = malloc(sizeof(t_img));
    game->saved = malloc(sizeof(t_move));
    game->saved->to = NULL;
    game->saved->from = NULL;
    game->advancedopen = 0;
    game->midgame = 0;
}
int main(int ac, char **av)
{
    if(ac != 2 || (*av[1] != '0' && *av[1] != '1'))
        return(printf("choose mode solo '0' or duo '1'\n"), 1);
    t_game *game = malloc(sizeof(t_game));
    init_g(game);
    if(*av[1] == '1')
        game->gamemode = 1;
    init_game(game);
    mlx_cursor_hook(game->mlx, &mouse, game);
    mlx_mouse_hook(game->mlx, &click, game);
    mlx_loop_hook(game->mlx, &test, game);
    mlx_loop(game->mlx);
    mlx_terminate(game->mlx);//prmot sometime isnt updated white ppawn take in h6 if check after romotion crash
}
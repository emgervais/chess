#include "chess.h"

void mouse(double xpos, double ypos, void* param)
{
    t_game *game = param;
    if((xpos <= 800 && xpos >= 0) && (ypos <= 800 && ypos >= 0))
    {
        game->x = xpos;
        game->y = ypos;
    }
}

int is_turn(t_game *game)
{
    t_board *temp = game->board;
    t_player *temp2;
    if(game->turn == 0)
    {
        temp2 = game->white;
        while(temp->x - 10 != game->x / 100 * 100 || temp->y - 10 != game->y / 100 * 100)
            temp = temp->next;
        while(temp2 && temp2->pos != temp)
            temp2 = temp2->next;
        if(!temp2)
            return (0);
        else
            return (1);
    }
    else
    {
        temp2 = game->black;
        while(temp->x - 10 != game->x / 100 * 100 || temp->y - 10 != game->y / 100 * 100)
            temp = temp->next;
        while(temp2 && temp2->pos->square != temp->square)
            temp2 = temp2->next;
        if(!temp2)
            return (0);
        else
            return (1);
    }
}

int is_enemy(t_game *game, int turn, t_board *pos)
{
    t_player *player;
    if(turn == 0)
        player = game->black;
    else
        player = game->white;
    while(player && player->pos != pos)
        player = player->next;
    if(!player)
        return(0);
    player->pos = 0;
    player->piece = 0;
    return (1);
}

void update_pos(t_game *game, t_board *from, t_board *to, int turn)
{
    t_player *player;
    if(turn == 0)
        player = game->white;
    else
        player = game->black;
    while(player->pos != from)
        player = player->next;
    to->piece = from->piece;
    from->piece = 0;
    player->piece = to->piece;
    player->pos = to;
}
void click(mouse_key_t button, action_t action, modifier_key_t mods, void* param)
{
    t_game *game = param;
    t_board *temp = game->board;
    t_board *sel = game->board;
    t_player *player;

    if((game->x <= 800 && game->x >= 0) && (game->y <= 800 && game->y >= 0))
    {
        if(button == MLX_MOUSE_BUTTON_LEFT && action == 0 && game->selected && !is_turn(game))
        {
            while(temp->x - 10 != game->x / 100 * 100 || temp->y - 10 != game->y / 100 * 100)
                temp = temp->next;
            while(sel->square != game->selected)
                sel = sel->next;
            if(!is_valid(sel, temp, game, 1))
            {
                game->selected = 0;
                mlx_delete_image(game->mlx, game->select);
                if(!game->turn)
                {
                    if(is_enemy(game, 0, temp))
                        mlx_delete_image(game->mlx, temp->img);
                    mlx_delete_image(game->mlx, sel->img);
                    temp->img = mlx_new_image(game->mlx, 80, 80);
                    temp->img = mlx_texture_to_image(game->mlx, find_img(game->img, sel->piece));
                    mlx_image_to_window(game->mlx, temp->img, temp->x, temp->y);
                    update_pos(game, sel, temp, 0);
                    game->turn = 1;
                }
                else
                {
                    if(game->gamemode)
                    {
                        if(temp->img && is_enemy(game, 1, temp))
                            mlx_delete_image(game->mlx, temp->img);
                        mlx_delete_image(game->mlx, sel->img);
                        temp->img = mlx_new_image(game->mlx, 80, 80);
                        temp->img = mlx_texture_to_image(game->mlx, find_img(game->img, sel->piece));
                        mlx_image_to_window(game->mlx, temp->img, temp->x, temp->y);
                        update_pos(game, sel, temp, 1);
                        game->turn = 0;
                    }
                }
            } 
        }
        else if(button == MLX_MOUSE_BUTTON_LEFT && !action && is_turn(game))
        {
            t_board *temp = game->board;
            if(game->select)
                mlx_delete_image(game->mlx, game->select);
            game->select = mlx_new_image(game->mlx, 100, 100);
            game->select = mlx_texture_to_image(game->mlx, game->sel);
            mlx_image_to_window(game->mlx, game->select, game->x / 100 * 100, game->y / 100 * 100);
            while(temp->x != game->x / 100 * 100 + 10 || temp->y != game->y / 100 * 100 + 10)
                temp = temp->next;
            game->selected = temp->square;
        }
        if(!game->gamemode && game->turn == 1)
        {
            srand(time(NULL));
            fill_move(game, game->black, 1);
            apply_move(game, choose_move_rand(game->black, (rand() % (how_many_moves(game->black)))));
            clear_move(game->black);
            game->turn = 0;
        }
    }
}

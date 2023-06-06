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
        if(!temp2 || !temp2->piece)
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
        if(!temp2 || !temp2->piece)
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
    player->piece = 0;
    return (1);
}

t_board *find_king(t_game *game, int turn)
{
    t_player *temp = game->black;

    if(turn)
        temp = game->white;
    while(abs(temp->piece) != 6)
        temp = temp->next;
    return (temp->pos);
}
void update_pos(t_game *game, t_board *from, t_board *to, int turn)
{
    t_player *player = game->black;
    t_player *player2 = game->white;
    t_board *temp = to->up;
    if(!turn)
    {
        temp = to->down;
        player = game->white;
        player2 = game->black;
    }
    while(player->pos != from)
        player = player->next;
    if(game->pass)
    {
        while(player2->pos != temp)
            player2 = player2->next;
        player2->piece = 0;
        temp->piece = 0;
        game->pass = 0;
        mlx_delete_image(game->mlx, temp->img);
    }
    if((from->piece > 0 && to->piece < 0) || (from->piece < 0 && to->piece > 0))
    {
        while(player2->pos != to)
            player2 = player2->next;
    player2->piece = 0;
    }
    to->piece = from->piece;
    from->piece = 0;
    player->piece = to->piece;
    player->pos = to;
    player->m++;
}
void click(mouse_key_t button, action_t action, modifier_key_t mods, void* param)
{
    t_game *game = param;
    t_board *temp;
    t_board *sel;
    t_player *player;

    if((game->x <= 800 && game->x >= 0) && (game->y <= 800 && game->y >= 0))
    {
        if(button == MLX_MOUSE_BUTTON_LEFT && action == 0 && game->selected && !is_turn(game) && !game->locked)
        {
            temp = game->board;
            sel = game->board;
            while(temp->x - 10 != game->x / 100 * 100 || temp->y - 10 != game->y / 100 * 100)
                temp = temp->next;
            game->to = temp;
            while(sel->square != game->selected)
                sel = sel->next;
            game->from = sel;
            if(is_valid(sel, temp, game, 1))
            {
                mlx_delete_image(game->mlx, game->select);
                if(!game->turn)
                {
                    if(is_enemy(game, 0, temp))
                        mlx_delete_image(game->mlx, temp->img);
                    if(is_promo(game, sel, temp))
                    {
                        mlx_texture_t *text = mlx_load_png("./img/promo.png");
                        game->promo = mlx_texture_to_image(game->mlx, text);
                        mlx_image_to_window(game->mlx, game->promo, 0, 0);
                        game->locked = 1;
                        mlx_delete_texture(text);
                        return;
                    }
                    mlx_delete_image(game->mlx, sel->img);
                    temp->img = mlx_new_image(game->mlx, 80, 80);
                    temp->img = mlx_texture_to_image(game->mlx, find_img(game->img, sel->piece));
                    mlx_image_to_window(game->mlx, temp->img, temp->x, temp->y);
                    update_pos(game, sel, temp, 0);
                    game->turn = 1;
                    game->selected = 0;
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
                        game->selected = 0;
                        game->turn = 0;
                    }
                }
            } 
        }
        else if(button == MLX_MOUSE_BUTTON_LEFT && !action && is_turn(game) && !game->locked)
        {
            t_board *temp4 = game->board;
            if(game->select)
                mlx_delete_image(game->mlx, game->select);
            game->select = mlx_new_image(game->mlx, 100, 100);
            game->select = mlx_texture_to_image(game->mlx, game->sel);
            mlx_image_to_window(game->mlx, game->select, game->x / 100 * 100, game->y / 100 * 100);
            while(temp4->x != game->x / 100 * 100 + 10 || temp4->y != game->y / 100 * 100 + 10)
                temp4 = temp4->next;
            game->selected = temp4->square;
        }
        else if(game->locked && game->x / 100 * 100 == 0 && game->y / 100 * 100 == 0 && button == MLX_MOUSE_BUTTON_LEFT && action == 0)
        {
            init_promo(game, game->to);
            mlx_delete_image(game->mlx, game->promo);
            mlx_delete_image(game->mlx, game->from->img);//update pos

            game->locked = 0;
            game->turn = !game->turn;
        }
        if(!game->gamemode && game->turn == 1 && !game->locked)
        {
            srand(NULL);
            fill_move(game, game->black);
            apply_move(game, choose_move_rand(game->black, (rand() % (how_many_moves(game->black)))));
            clear_move(game->black);
            game->turn = 0;
        }
    }
}
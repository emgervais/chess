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
    int tower = -2;
    if(!turn)
    {
        tower = 2;
        temp = to->down;
        player = game->white;
        player2 = game->black;
    }
    t_player *hold = player;
    while(player->pos != from)
        player = player->next;
    if(pass_pawn(game, from, to))
    {
        while(player2->pos != temp)
            player2 = player2->next;
        player2->piece = 0;
        temp->piece = 0;
        mlx_delete_image(game->mlx, temp->img);
    }
    else if(pass_pawn(game, game->saved->from, game->saved->to))
    {
        int i = 0;
        while(game->passed[i] != game->saved->to)
            i++;
        game->passed[i] = NULL;
    }
    if(game->castle)
    {
        if(to == from->right->right)
        {
            while(hold->pos->square % 10 != 8 || hold->piece != tower)
                hold = hold->next;
            mlx_delete_image(game->mlx, hold->pos->img);
            hold->pos->piece = 0;
            hold->pos = to->left;
            hold->pos->img = mlx_texture_to_image(game->mlx, find_img(game->img, tower));
            mlx_image_to_window(game->mlx, hold->pos->img, hold->pos->x, hold->pos->y);
            hold->pos->piece = tower;
        }
        else
        {
            while(hold->pos->square % 10 != 1 || hold->piece != tower)
                hold = hold->next;
            mlx_delete_image(game->mlx, hold->pos->img);
            hold->pos->piece = 0;
            hold->pos = to->right;
            hold->pos->img = mlx_texture_to_image(game->mlx, find_img(game->img, tower));
            mlx_image_to_window(game->mlx, hold->pos->img, hold->pos->x, hold->pos->y);
            hold->pos->piece = tower;
        }
        game->castle = 0;
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
    game->saved->to = NULL;
    game->saved->from = NULL;
}
int checkmate(t_game *game, t_player *player)
{
    t_player *temp = player;
    t_move *moves;
    int i = 0;
    fill_move(game, player);
    while(temp)
    {
        while(!temp->piece)
            temp = temp->next;
        if(!temp)
            break;
        moves = temp->moves;
        while(moves->close)
        {
            i++;
            moves = moves->next;
        }
        temp = temp->next;
    }
    if(!i && is_attacked(game, find_king(game, game->turn), find_king(game, game->turn)->piece))
        return (1);
    else if(!i)
        return(1);
    clear_move(player);
    return (0);
}

int check_other(t_board *from, t_board *to, t_game *game)
{
    t_player *player = game->black;
    if(!game->turn)
        player = game->white;
    while(player)
    {
        if(player->piece == from->piece && player->pos != from && is_valid(player->pos, to, game, 0, 0))
                return(1);
        player = player->next;
    }
    return (0);
}
char *translate(t_board *from, t_board *to, int f, t_game *game, char *move)
{
    char letter[5] = {'R', 'N', 'B', 'Q', 'K'};
    char *num = ft_itoa(game->turn_num);
    int p = abs(from->piece);
    int l = 0;
    int i = 0;
        if(!f)
        {
            while(num[l])
                move[i++] = num[l++];
            move[i++] = '.';
        }
        if(p == 6 && (to == from->left->left || to == from->right->right))
        {
            move[i++] = 'O';
            move[i++] = '-';
            move[i++] = 'O';
            if(to == from->left->left)
            {
                move[i++] = '-';
                move[i++] = 'O';
            }
        }
        else
        {
            if(p != 1)
                move[i++] = letter[p - 2];
        //add place
            if(check_other(from, to, game))
                move[i++] = from->square % 10 + 96;
            if((from->piece > 0 && to->piece < 0) || (from->piece < 0 && to->piece > 0))
                move[i++] = 'x';
            move[i++] = to->square % 10 + 96;
            move[i++] = to->square / 10 + 48;
        }
        if(simulate_move2(game, from, to))
            move[i++] = '+';
        move[i++] = ' ';
        move[i++] = '\0';
    return (move);
}
char *strjoin(const char* s1, const char* s2)
{
    char* result = malloc(strlen(s1) + strlen(s2) + 1);

    if (result)
    {
        strcpy(result, s1);
        strcat(result, s2);
    }

    return (result);
}

void enter_log(t_board *from, t_board *to, t_game *game)
{
    if(!game->log)
    {
        game->log = malloc(sizeof(t_log));
        game->log->temp = malloc(sizeof(char) * 10);
        game->log->move = malloc(sizeof(char) * 1000);
        game->log->m = game->turn_num;
        translate(from, to, 0, game, game->log->move);
        return ;
    }
    int f = 0;
    if(game->turn_num == game->log->m)
        f = 1;
    game->log->move = strjoin(game->log->move, translate(from, to, f, game, game->log->temp));
    game->log->m = game->turn_num;
    printf("%s\n", game->log->move);
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
            if(is_valid(sel, temp, game, 1, 1))
            {
                mlx_delete_image(game->mlx, game->select);
                game->selected = 0;
                if(!game->turn)
                {
                    if(is_enemy(game, 0, temp))
                        mlx_delete_image(game->mlx, temp->img);
                    if(is_promo(game, sel, temp))
                    {
                        mlx_texture_t *text = mlx_load_png("./img/promo.png");
                        game->promo = mlx_texture_to_image(game->mlx, text);
                        mlx_image_to_window(game->mlx, game->promo, temp->x - 10, temp->y - 10); 
                        game->locked = 1;
                        mlx_delete_texture(text);
                        return;
                    }
                    mlx_delete_image(game->mlx, sel->img);
                    temp->img = mlx_new_image(game->mlx, 80, 80);
                    temp->img = mlx_texture_to_image(game->mlx, find_img(game->img, sel->piece));
                    mlx_image_to_window(game->mlx, temp->img, temp->x, temp->y);
                    enter_log(sel, temp, game);
                    update_pos(game, sel, temp, 0);
                    game->turn = 1;
                    if(checkmate(game, game->black))
                    {
                        printf("white won\n");
                        mlx_close_window(game->mlx);
                    }
                }
                else
                {
                    if(game->gamemode)
                    {
                        if(temp->img && is_enemy(game, 1, temp))
                            mlx_delete_image(game->mlx, temp->img);
                    if(is_promo(game, sel, temp))
                    {
                        mlx_texture_t *text = mlx_load_png("./img/promo.png");
                        game->promo = mlx_texture_to_image(game->mlx, text);
                        mlx_image_to_window(game->mlx, game->promo, temp->x - 10, temp->y - 10); 
                        game->locked = 1;
                        mlx_delete_texture(text);
                        return;
                    }
                        mlx_delete_image(game->mlx, sel->img);
                        temp->img = mlx_new_image(game->mlx, 80, 80);
                        temp->img = mlx_texture_to_image(game->mlx, find_img(game->img, sel->piece));
                        mlx_image_to_window(game->mlx, temp->img, temp->x, temp->y);
                        enter_log(sel, temp, game);
                        update_pos(game, sel, temp, 1);
                        game->selected = 0;
                        game->turn = 0;
                        if(checkmate(game, game->white))
                        {
                            printf("black won\n");
                            mlx_close_window(game->mlx);
                        }
                        game->turn_num++;
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
        else if(game->locked && game->x / 100 * 100 == game->to->x - 10 && game->y / 100 * 100 == game->to->y - 10 && button == MLX_MOUSE_BUTTON_LEFT && action == 0)
        {
            init_promo(game, game->to);
            mlx_delete_image(game->mlx, game->promo);
            mlx_delete_image(game->mlx, game->from->img);
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
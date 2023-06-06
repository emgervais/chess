#include "chess.h"
void init_promo(t_game *game, t_board *current)
{
    int i = 1;
    t_player *player = game->white;
    if(game->turn == 1)
    {
        player = game->white;
        i = -1;
    }
    if((game->x < 50 && game->x >= 0) && (game->y < 50 && game->y >= 0))
    {
        square(game, current, find_img(game->img, 4 * i));
        game->from->piece = 0;
        game->to->piece = 4 * i;
    }
    else if((game->x < 100 && game->x >= 50) && (game->y < 50 && game->y >= 0))
    {
        game->from->piece = 0;
        game->to->piece = 3 * i;
        square(game, current, find_img(game->img, 3 * i));
    }
    else if((game->x < 50 && game->x >= 0) && (game->y < 100 && game->y >= 50))
    {
        game->from->piece = 0;
        game->to->piece = 5 * i;
        square(game, current, find_img(game->img, 5 * i));
    }
    else if((game->x < 100 && game->x >= 50) && (game->y < 100 && game->y >= 50))
    {
        game->from->piece = 0;
        game->to->piece = 2 * i;
        square(game, current, find_img(game->img, 2 * i));
    }
    while(player->pos != game->from)
        player = player->next;
    player->pos = game->to;
    player->piece = game->to->piece;
}

int is_promo(t_game *game, t_board *from, t_board *to)
{
    if ((to->square / 10 == 8 || to->square / 10 == 1) && abs(from->piece) == 1)
        return (1);
    return (0);
}

int pawn_attack(t_game *game, t_board *from, t_board *to)
{
    if(from->piece > 0 && (from->diagone == to || from->diagonw == to) && to->piece < 0)
        return (1);
    else if(from->piece < 0 && (from->diagose == to || from->diagosw == to) && to->piece > 0)
        return (1);
    return (0);
}
int pawn(t_game *game, t_board *from, t_board *to)
{
    t_board *temp = from;
    t_player *player = game->black;

    if(from->piece > 0)
    {
        if(((to == temp->up->up && from->square / 10 == 2 && !temp->up->piece) || to == temp->up) && !to->piece)
            return (1);
        else if((to == temp->diagonw && temp->diagonw->piece < 0) || (to == temp->diagone && temp->diagone->piece < 0))
            return (1);
        else if(((to == temp->diagonw && temp->diagonw->piece == 0) || (to == temp->diagone && temp->diagone->piece == 0)) && from->square / 10 == 5 && to->down->piece == -1)
        {
            while(player->pos != to->down)
                player = player->next;
            if(player->m == 1)
            {
                game->pass = 1;
                return(1);
            }
        }
    }
    else
    {
        player = game->white;
        if(((to == temp->down->down && from->square / 10 == 7 && !temp->down->piece) || to == temp->down) && !to->piece)
            return (1);
        else if((to == temp->diagosw && temp->diagosw->piece > 0) || (to == temp->diagose && temp->diagose->piece > 0))
            return (1);
        else if(((to == temp->diagosw && temp->diagosw->piece == 0) || (to == temp->diagose && temp->diagose->piece == 0)) && from->square / 10 == 4 && to->up->piece == 1)
        {
            while(player->pos != to->up)
                player = player->next;
            printf("m : %d\n", player->m);
            if(player->m == 1)
            {
                game->pass = 1;
                return(1);
            }
        }
    }
    return (0);
}
int tower(t_game *game, t_board *from, t_board *to)
{
    t_board *temp = from;
    int i = 0;

    if(((from->piece < 0 && to->piece < 0) || (from->piece > 0 && to->piece > 0)))
        return (0);
    while(i < 4)
    {
        temp = from;
        if(!i)
            temp = temp->up;
        if(i == 1)
            temp = temp->right;
        if(i == 2)
            temp = temp->left;
        if(i == 3)
            temp = temp->down;
        while(temp && !temp->piece)
        {
            if(temp == to)
                return (1);
            if(!i)
                temp = temp->up;
            if(i == 1)
                temp = temp->right;
            if(i == 2)
                temp = temp->left;
            if(i == 3)
                temp = temp->down;
        }
        if(temp && temp == to)
            return (1);
        i++;
    }
    return (0);
}
int bishop(t_game *game, t_board *from, t_board *to)
{
    t_board *temp = from;
    int i = 0;

    if(((from->piece < 0 && to->piece < 0) || (from->piece > 0 && to->piece > 0)))
        return (0);
    while(i < 4)
    {
        temp = from;
        if(!i)
            temp = temp->diagone;
        if(i == 1)
            temp = temp->diagonw;
        if(i == 2)
            temp = temp->diagose;
        if(i == 3)
            temp = temp->diagosw;
        while(temp && !temp->piece)
        {
            if(temp == to)
                return (1);
            if(!i)
                temp = temp->diagone;
            if(i == 1)
                temp = temp->diagonw;
            if(i == 2)
                temp = temp->diagose;
            if(i == 3)
                temp = temp->diagosw;
        }
        if(temp && temp == to)
            return (1);
        i++;
    }
    return (0);
}
int knight(t_game *game, t_board *from, t_board *to)
{
    t_board *temp = from;

    if(((from->piece < 0 && to->piece < 0) || (from->piece > 0 && to->piece > 0)))
        return (0);
    if(from->square < 71)
    {
        temp = from->up->up;
        if(to == temp->right || to == temp->left)
            return (1);
    }
    if(from->square > 28)
    {
        temp = from->down->down;
        if(to == temp->right || to == temp->left)
            return (1);
    }
    if(from->square % 10 > 2)
    {
        temp = from->left->left;
        if(to == temp->up || to == temp->down)
            return (1);
    }
    if(from->square % 10 < 7)
    {
        temp = from->right->right;
        if(to == temp->up || to == temp->down)
            return (1);
    }
    return (0);
}
int king(t_game *game, t_board *from, t_board *to)
{
    t_board *temp = from;
    if(((from->piece < 0 && to->piece < 0) || (from->piece > 0 && to->piece > 0)))
        return (0);
    temp = from->up;
    if(temp != to)
        temp = from->down;
    if(temp != to)
        temp = from->right;
    if(temp != to)
        temp = from->left;
    if(temp != to)
        temp = from->diagone;
    if(temp != to)
        temp = from->diagonw;
    if(temp != to)
        temp = from->diagose;
    if(temp != to)
        temp = from->diagosw;
    if(temp == to)
        return (1);
    return (0);
}
int is_attacked(t_game *game, t_board *current, int piece)
{
    t_board *temp = game->board;
    int hold = current->piece;
    current->piece = piece;
    while(temp)
    {
        if(temp == current)
            temp = temp->next;
        if(!temp)
            break;
        if(abs(temp->piece) == 1)
        {
            if(pawn_attack(game, temp, current))
            {
                current->piece = hold;
                return (1);
            }
        }
        else if(temp->piece)
        {
            if (is_valid(temp, current, game, 0))
            {
                current->piece = hold;
                return (1);
            }
        }
        temp = temp->next;
    }
    current->piece = hold;
    return (0);
}

int simulate_move(t_game *game, t_board *from, t_board *to)
{
    int hold = to->piece;
    to->piece = from->piece;
    t_player *player = game->white;
    int turn = 1;
    if(game->turn)
    {
        player = game->black;
        turn = 0;
    }
    while(player->pos != from)
        player = player->next;
    player->pos = to;
    if(is_attacked(game, find_king(game, turn), find_king(game, turn)->piece))
    {
        to->piece = hold;
        player->pos = from;
        return (1);
    }
    player->pos = from;
    to->piece = hold;
    return (0);
}
int is_valid(t_board *from, t_board *to, t_game *game, int first)
{
    if((from->piece > 0 && to->piece > 0) || (from->piece < 0 && to->piece < 0))
        return (0);
    else if(first && abs(from->piece) == 6 && is_attacked(game, to, from->piece))
        return(0);
    else if(first && simulate_move(game, from, to))
        return (0);
    else if(abs(from->piece) == 1)
        return(pawn(game, from, to));
    else if(abs(from->piece) == 2)
        return(tower(game, from, to));
    else if(abs(from->piece) == 3)
        return(knight(game, from, to));
    else if (abs(from->piece) == 4)
        return(bishop(game, from, to));
    else if(abs(from->piece) == 5 && (tower(game, from, to) || bishop(game, from, to)))
        return (1);
    else 
        return(king(game, from, to));
}
t_board *find_square(t_game *game, int pos)
{
    t_board *temp = game->board;

    while (temp->square != pos)
        temp = temp->next;
    return (temp);
}
void fill_move(t_game *game, t_player *player)
{
    t_player *temp = player;
    t_board *temp2;
    t_move *temp3;

    while(temp)
    {
        if(temp->piece)
        {
            temp3 = temp->moves;
            temp2 = game->board;
            while(temp2)
            {
                if(temp2 == temp->pos)
                    temp2 = temp2->next;
                if(!temp2)
                    break;
                if(is_valid(temp->pos, temp2, game, 1))
                {
                    if(temp2->piece)
                        temp3->attack = 1;
                    temp3->close = 1;
                    temp3->from = temp->pos;
                    temp3->to = temp2;
                    temp3 = temp3->next;
                }
                temp2 = temp2->next;
            }
        }
        temp = temp->next;
    }
}

void clear_move(t_player *player)
{
    t_player *temp = player;
    t_move *temp2;

    while(temp)
    {
        if(!temp->piece)
            temp = temp->next;
        if(!temp)
            break;
        temp2 = temp->moves;
        while(temp2->close)
        {
            temp2->close = 0;
            temp2->attack = 0;
            temp2->check = 0;
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
}
int how_many_moves(t_player *player)
{
    t_player *temp = player;
    t_move *temp2;
    int count = 0;

    while(temp)
    {
        if(!temp->piece)
            temp = temp->next;
        if(!temp)
            break;
        temp2 = temp->moves;
        while(temp2->close)
        {
            count++;
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
    return (count);
}

t_move *choose_move_rand(t_player *player, int move)
{
    t_player *temp = player;
    t_move *temp2;

    while(temp)
    {
        if(!temp->piece)
            temp = temp->next;
        if(!temp)
            break;
        temp2 = temp->moves;
        while(temp2->close)
        {
            move--;
            if(!move)
                return(temp2);
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
}

void apply_move(t_game *game, t_move *move)
{
    if(move->to->img && is_enemy(game, 1, move->to))
        mlx_delete_image(game->mlx, move->to->img);
    mlx_delete_image(game->mlx, move->from->img);
    move->to->img = mlx_new_image(game->mlx, 80, 80);
    move->to->img = mlx_texture_to_image(game->mlx, find_img(game->img, move->from->piece));
    mlx_image_to_window(game->mlx, move->to->img, move->to->x, move->to->y);
    update_pos(game, move->from, move->to, 1);
}
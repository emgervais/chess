#include "chess.h"

int pawn(t_game *game, t_board *from, t_board *to)
{
    t_board *temp = from;

    if(game->turn == 0)
    {
        if(((to == temp->up->up && from->square / 10 == 2 && !temp->up->piece) || to == temp->up) && !to->piece)
            return (0);
        else if((to == temp->diagonw && temp->diagonw->piece < 0) || (to == temp->diagone && temp->diagone->piece < 0))
            return (0);
    }
    else
    {
        if(((to == temp->down->down && from->square / 10 == 7 && !temp->up->piece) || to == temp->down) && !to->piece)
            return (0);
        else if((to == temp->diagosw && temp->diagosw->piece > 0) || (to == temp->diagose && temp->diagose->piece > 0))
            return (0);
    }
    return (1);
}
int tower(t_game *game, t_board *from, t_board *to)
{
    t_board *temp;
    int i = 0;

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
                return (0);
            if(!i)
                temp = temp->up;
            if(i == 1)
                temp = temp->right;
            if(i == 2)
                temp = temp->left;
            if(i == 3)
                temp = temp->down;
        }
        if(temp && temp == to && ((from->piece < 0 && to->piece > 0) || (from->piece > 0 && to->piece < 0)))
            return (0);
        i++;
    }
    return (1);
}
int bishop(t_game *game, t_board *from, t_board *to)
{
    t_board *temp;

        int i = 0;

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
                return (0);
            if(!i)
                temp = temp->diagone;
            if(i == 1)
                temp = temp->diagonw;
            if(i == 2)
                temp = temp->diagose;
            if(i == 3)
                temp = temp->diagosw;
        }
        if(temp && temp == to && ((from->piece < 0 && to->piece > 0) || (from->piece > 0 && to->piece < 0)))
            return (0);
        i++;
    }
    return (1);
}
//int castle(t_game)
int knight(t_game *game, t_board *from, t_board *to)
{
    t_board *temp;

    if(from->square < 78)
    {
        temp = from->up->up;
        if(to == temp->right || to == temp->left && ((from->piece <= 0 && to->piece >= 0) || (from->piece >= 0 && to->piece <= 0)))
            return (0);
    }
    if(from->square > 28)
    {
        temp = from->down->down;
        if(to == temp->right || to == temp->left && ((from->piece <= 0 && to->piece >= 0) || (from->piece >= 0 && to->piece <= 0)))
            return (0);
    }
    if(from->square % 10 > 2)
    {
        temp = from->left->left;
        if(to == temp->up || to == temp->down && ((from->piece <= 0 && to->piece >= 0) || (from->piece >= 0 && to->piece <= 0)))
            return (0);
    }
    if(from->square % 10 < 7)
    {
        temp = from->right->right;
        if(to == temp->up || to == temp->down && ((from->piece <= 0 && to->piece >= 0) || (from->piece >= 0 && to->piece <= 0)))
            return (0);
    }
    return (1);
}
int king(t_game *game, t_board *from, t_board *to)
{
    t_board *temp;
    //if((to->square == 13 || to->square == 17) && game->turn == 0 && !castle)
    //    return (0);
    //else if((to->square == 83 || to->square == 87) && game->turn == 1 && !castle)
    //    return (0);
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
    if(temp == to && ((from->piece <= 0 && to->piece >= 0) || (from->piece >= 0 && to->piece <= 0)))
        return (0);
    return (1);
}
int is_valid(t_board *from, t_board *to, t_game *game)
{
    if((game->check && abs(from->piece) != 6)  || (from->piece > 0 && to->piece > 0) || (from->piece < 0 && to->piece < 0))
        return (1);
    else if(abs(from->piece) == 1)
        return(pawn(game, from, to));//check if piece is blocking for doublemove
    else if(abs(from->piece) == 2)
        return(tower(game, from, to));
    else if(abs(from->piece) == 3)
        return(knight(game, from, to));
    else if (abs(from->piece) == 4)
        return(bishop(game, from, to));
    else if(abs(from->piece) == 5 && (!tower(game, from, to) || !bishop(game, from, to)))
        return (0);
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
                if(!is_valid(temp->pos, temp2, game))
                {
                    if(temp2->piece)
                        temp3->attack = 1;
                    if(temp2->piece == 6 || temp2->piece == -6)
                        temp3->check = 1;
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
void algo(t_game *game)
{

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
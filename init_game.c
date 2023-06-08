#include "chess.h"

void init_board(t_game *game)
{
    int i = 0;
    int y = 8;
    int x = 2;
    int px = 110;
    int py = 10;
    t_board *temp;
    t_board *temp2;

    game->board = malloc(sizeof(t_board));
    game->board->next = NULL;
    game->board->square = 81;
    game->board->x = 10;
    game->board->y = 10;
    while(i++ < 63)
    {
        temp = malloc(sizeof(t_board));
        temp->next = NULL;
        temp->square = y * 10 + x;
        x++;
        temp->x = px;
        temp->y = py;
        px += 100;
        if(x == 9)
        {
            py += 100;
            px = 10;
            y--;
            x = 1;
        }
        temp2 = game->board;
        while(temp2->next)
            temp2 = temp2->next;
        temp2->next = temp;
    }
}
void init_line(t_game *game)
{
    t_board *current = game->board->next;
    t_board *old = game->board;

    old->left = NULL;
    while(current)
    {
        if(current->square % 10 != 1)
        {
            if(current->square % 10 == 8)
                current->right = NULL;
            old->right = current;
            current->left = old;
        }
        else
            current->left = NULL;
        current = current->next;
        old = old->next;
    }
}
void init_column(t_game *game)
{
    t_board *current = game->board;
    t_board *old;
    t_board *top = game->board;
    int i = top->square;

    while(top)
    {
        top->up = NULL;
        current = top;
        i = top->square;
        while(i > 10)
        {
            old = current;
            i -= 10;
            while(current->square != i && i > 10)
            {
                current = current->next;
            }
            if(i > 10)
            {
                current->up = old;
                old->down = current;
            }
        }
        current->down = NULL;
        top = top->right;
    }
}
void init_diago2(t_game *game)
{
    t_board *current;
    t_board *old;
    t_board *temp = game->board;
    int pos;
    while(temp->right->right)
        temp = temp->right;
    temp->right->diagose = NULL;
    temp->right->diagonw = NULL;
    while(temp)
    {
        temp->diagonw = NULL;
        old = temp;
        current = temp;
        pos = temp->square - 9;
        while((pos % 10 <= 8 && pos % 10 >= 1) && (pos / 10 <= 8 && pos / 10 >= 1))
        {
            while(current->square != pos)
                current = current->next;
            current->diagonw = old;
            old->diagose = current;
            old = current;
            pos -= 9;
        }
        old->diagose = NULL;
        if(!temp->down)
        {
            temp->diagonw = NULL;
            temp->diagose = NULL;
        }
        if(temp->left)
            temp = temp->left;
        else
            temp = temp->down;
    }
}
void init_diago1(t_game *game)
{
    t_board *current;
    t_board *old;
    t_board *temp = game->board->next;
    int pos;
    game->board->diagosw = NULL;
    game->board->diagone = NULL;
    while(temp)
    {
        temp->diagone = NULL;
        old = temp;
        current = temp;
        pos = temp->square - 11;
        while((pos % 10 <= 8 && pos % 10 >= 1) && (pos / 10 <= 8 && pos / 10 >= 1))
        {
            while(current->square != pos)
                current = current->next;
            current->diagone = old;
            old->diagosw = current;
            old = current;
            pos -= 11;
        }
        old->diagosw = NULL;
        if(!temp->down)
        {
            temp->diagone = NULL;
            temp->diagosw = NULL;
        }
        if(temp->right)
            temp = temp->right;
        else
            temp = temp->down;
    }
}
void init_piece(t_game *game)
{
    t_board *temp = game->board;
    int piece = -2;

    while(temp->square != 71)
    {
        temp->piece = piece--;
        if(temp->square % 10 == 5)
            piece += 1;
        if(temp->square % 10 > 4)
            piece += 2;
        temp = temp->next;
    }
    while(temp->square != 61)
    {
        temp->piece = -1;
        temp = temp->next;
    }
    piece = 2;
    while(temp->square != 21)
    {
        temp->piece = 0;
        temp = temp->next;
    }
    while(temp->square != 11)
    {
        temp->piece = 1;
        temp = temp->next;
    }
    while(temp)
    {
        temp->piece = piece++;
        if(temp->square % 10 == 5)
            piece -= 1;
        if(temp->square % 10 > 4)
            piece -= 2;
        temp = temp->next;
    }
}
void init_player(t_game *game)
{
    game->white = malloc(sizeof(t_player));
    game->black = malloc(sizeof(t_player));
    game->white->next = NULL;
    game->black->next = NULL;
    int i = 0;
    t_player *temp = game->black;
    t_player *temp2;
    t_board *board = game->board;
    temp->piece = board->piece;
    temp->pos = board;
    temp->m = 0;
    board = board->next;
    while(i++ < 15)
    {
        temp2 = malloc(sizeof(t_player));
        temp2->next = NULL;
        temp2->piece = board->piece;
        temp2->m = 0;
        temp2->pos = board;
        while(temp->next)
            temp = temp->next;
        temp->next = temp2;
        board = board->next;
    }
    while(board->square != 21)
        board = board->next;
    i = 0;
    temp = game->white;
    temp->piece = board->piece;
    temp->pos = board;
    temp->m = 0;
    board = board->next;
    while(i++ < 15)
    {
        temp2 = malloc(sizeof(t_player));
        temp2->next = NULL;
        temp2->piece = board->piece;
        temp2->pos = board;
        temp2->m = 0;
        while(temp->next)
            temp = temp->next;
        temp->next = temp2;
        board = board->next;
    }
}
void init_move(t_game *game)
{
    t_player *temp = game->white;
    t_move *movet;
    t_move *movet2;
    int i = 0;
    while(temp)
    {
        i = 0;
        movet = malloc(sizeof(t_move));
        movet->next = NULL;
        movet->close = 0;
        temp->moves = movet;
        while(i++ < 30)
        {
            movet2 = malloc(sizeof(t_move));
            movet2->next = NULL;
            movet2->close = 0;
            while(movet->next)
                movet=movet->next;
            movet->next = movet2;
        }
        temp = temp->next;
    }
    temp = game->black;
    while(temp)
    {
        i = 0;
        movet = malloc(sizeof(t_move));
        movet->next = NULL;
        movet->close = 0;
        temp->moves = movet;
        while(i++ < 30)
        {
            movet->close = 0;
            movet2 = malloc(sizeof(t_move));
            movet2->next = NULL;
            while(movet->next)
                movet=movet->next;
            movet->next = movet2;
        }
        temp = temp->next;
    }
}
mlx_texture_t *which_txt(int id)
{
    if(id == 2)
        return (mlx_load_png("./img/wtower.png"));
    if(id == 3)
        return(mlx_load_png("./img/wknight.png"));
    if (id == 4)
        return (mlx_load_png("./img/wbishop.png"));
    if(id == 6)
        return (mlx_load_png("./img/wking.png"));
    if(id == 5)
        return(mlx_load_png("./img/wqueen.png"));
    if(id == 1)
        return (mlx_load_png("./img/wpawn.png"));
    if (id == -1)
        return (mlx_load_png("./img/bpawn.png"));
    if(id == -2)
        return (mlx_load_png("./img/btower.png"));
    if (id == -3)
        return (mlx_load_png("./img/bknight.png"));
    if (id == -4)
        return (mlx_load_png("./img/bbishop.png"));
    if (id == -6)
        return (mlx_load_png("./img/bking.png"));
    if (id == -5)
        return (mlx_load_png("./img/bqueen.png"));
}
void init_textures(t_game *game)
{
    mlx_texture_t *temp = mlx_load_png("./img/board.png");
    game->b = mlx_new_image(game->mlx, 800, 800);
    game->b = mlx_texture_to_image(game->mlx, temp);
    game->sel = mlx_load_png("./img/select.png");
    game->img = malloc(sizeof(t_img));
    t_img *temp1 = game->img;
    t_img *temp2;
    temp1->next = NULL;
    temp1->id = 6;
    temp1->img = which_txt(6);
    int id = 6;
    while(id-- > -7)
    {
        if(id == 0)
            id--;
        temp2 = malloc(sizeof(t_img));
        temp2->next = NULL;
        temp2->id = id;
        temp2->img = which_txt(id);
        while(temp1->next)
            temp1 = temp1->next;
        temp1->next = temp2;
    }
}
void init_mlx(t_game *game)
{
    mlx_image_t *temp;
    game->mlx = mlx_init(800, 800, "Chess", true);
    init_textures(game);
    mlx_image_to_window(game->mlx, game->b, 0, 0);
}
void square(t_game *game, t_board *temp, mlx_texture_t *t)
{
    temp->img = mlx_new_image(game->mlx, 80, 80);
    temp->img = mlx_texture_to_image(game->mlx, t);
    mlx_image_to_window(game->mlx, temp->img, temp->x, temp->y);
}
mlx_texture_t *find_img(t_img *img, int id)
{
    t_img *temp = img;

    while(temp->id != id)
        temp = temp->next;
    return(temp->img);
}
void init_starting_pos(t_game *game)
{
    t_board *temp = game->board;

    game->selected = 0;
    square(game, temp, find_img(game->img, -2));
    temp = temp->next;
    square(game, temp, find_img(game->img, -3));
    temp = temp->next;
    square(game, temp, find_img(game->img, -4));
    temp = temp->next;
    square(game, temp, find_img(game->img, -5));
    temp = temp->next;
    square(game, temp, find_img(game->img, -6));
    temp = temp->next;
    square(game, temp, find_img(game->img, -4));
    temp = temp->next;
    square(game, temp, find_img(game->img, -3));
    temp = temp->next;
    square(game, temp, find_img(game->img, -2));
    temp = temp->next;
    while(temp->square / 10 == 7)
    {
        temp->img = mlx_new_image(game->mlx, 80, 80);
        temp->img = mlx_texture_to_image(game->mlx, find_img(game->img, -1));
        mlx_image_to_window(game->mlx, temp->img, temp->x, temp->y);
        game->black->value = 1;
        temp = temp->next;
    }
    while(temp->square / 10 != 2)
        temp = temp->next;
    while(temp->square / 10 == 2)
    {
        temp->img = mlx_new_image(game->mlx, 80, 80);
        temp->img = mlx_texture_to_image(game->mlx, find_img(game->img, 1));
        mlx_image_to_window(game->mlx, temp->img, temp->x, temp->y);
        game->white->value = 1;
        temp = temp->next;
    }
    square(game, temp, find_img(game->img, 2));
    temp = temp->next;
    square(game, temp, find_img(game->img, 3));
    temp = temp->next;
    square(game, temp, find_img(game->img, 4));
    temp = temp->next;
    square(game, temp, find_img(game->img, 5));
    temp = temp->next;
    square(game, temp, find_img(game->img, 6));
    temp = temp->next;
    square(game, temp, find_img(game->img, 4));
    temp = temp->next;
    square(game, temp, find_img(game->img, 3));
    temp = temp->next;
    square(game, temp, find_img(game->img, 2));
    temp = temp->next;
}
void init_pass_pawn(t_game *game)
{
    t_board *temp = game->board;
    int i = 0;
    while(temp->square / 10 != 6)
        temp = temp->next;
    while(temp->square / 10 == 6)
    {
        game->passed[i++] = temp;
        temp = temp->next;
    }
    while(temp->square / 10 != 3)
        temp = temp->next;
    while(temp->square / 10 == 3)
    {
        game->passed[i++] = temp;
        temp = temp->next;
    }
}
void init_game(t_game *game)
{
    init_board(game);
    init_line(game);
    init_column(game);
    init_diago1(game);
    init_diago2(game);
    init_piece(game);
    init_player(game);
    init_move(game);
    init_mlx(game);
    init_starting_pos(game);
    init_pass_pawn(game);
}
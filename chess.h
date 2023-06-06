#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./MLX42/include/MLX42/MLX42.h"

typedef struct s_board
{
    mlx_image_t *img;
    int square;
    int piece;
    int x;
    int y;
    struct s_board *next;
    struct s_board *up;
    struct s_board *down;
    struct s_board *left;
    struct s_board *right;
    struct s_board *diagone;
    struct s_board *diagosw;
    struct s_board *diagonw;
    struct s_board *diagose;
}   t_board;

typedef struct s_move
{
    int value;
    t_board *from;
    t_board *to;
    int check;
    int attack;
    int close;
    struct s_move *next;
} t_move;

typedef struct s_player
{
    t_board *pos;
    int piece;
    int value;
    int m;
    struct s_player *next;
    t_move *moves;

} t_player;

typedef struct s_img
{
    int id;
    mlx_texture_t *img;
    struct s_img *next;
} t_img;

typedef struct s_game
{
    int x;
    int y;
    mlx_image_t *b;
    mlx_image_t *select;
    mlx_texture_t *sel;
    t_board *board;
    t_board *to;
    t_board *from;
    t_player *white;
    t_player *black;
    t_img   *img;
    mlx_t *mlx;
    int turn;
    int selected;
    int check;
    int gamemode;
    int locked;
    int pass;
    mlx_image_t *promo;
}   t_game;

void init_game(t_game *game);
mlx_texture_t *find_img(t_img *img, int id);
void click(mouse_key_t button, action_t action, modifier_key_t mods, void* param);
void mouse(double xpos, double ypos, void* param);
int is_valid(t_board *from, t_board *to, t_game *game, int first);
void fill_move(t_game *game, t_player *player);
int how_many_moves(t_player *player);
t_move *choose_move_rand(t_player *player, int move);
void apply_move(t_game *game, t_move *move);
void clear_move(t_player *player);
int is_enemy(t_game *game, int turn, t_board *pos);
void update_pos(t_game *game, t_board *from, t_board *to, int turn);
int is_attacked(t_game *game, t_board *current, int piece);
t_board *find_king(t_game *game, int turn);
void init_promo(t_game *game, t_board *current);
int is_promo(t_game *game, t_board *from, t_board *to);
void square(t_game *game, t_board *temp, mlx_texture_t *t);
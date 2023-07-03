#include "chess.h"

char *find_move(t_game *game, char *line)
{
    int i = strlen(game->log->move);
    int l = 0;
    line += i;
    char *temp = malloc(sizeof(char) * 100);
    while(*line == ' ')
        line++;
    if(line[1] == '.')
        line + 2;
    while(line[l] != ' ')
    {
        temp[l] = line[l];
        l++;
    }
    temp[l] = '\0';
    return(temp);
}
t_move *untranslate(t_game *game, char *move)//add prom and castle
{
    char *letter = "RNBQK";
    int prec = 0;
    t_player *player = game->white;
    int i = 0;
    int piece;
    while(letter[i] != *move && letter[i])
        i++;
    piece = i + 2;
    if(!letter[i])
        piece = 1;
    if(piece != 1)
        move++;
    if(game->turn)
    {
        player = game->black;
        piece *= -1;
    }
    int colfrom = *move - 96;
    if(*move <= 'z' && *move >= 'a' && (move[1] >= '9' || move[1] <= '1'))
        prec = 1;
    move++;
    if(*move == 'x')
        move++;
    int colto = colfrom;
    if(*move <= 'z' && *move >= 'a')
    {
        colto = *move - 96;
        move++;
    }
    int line = *move - 48;
    int pos = line * 10 + colto;
    t_board *cas = game->board;
    t_move *rmv = malloc(sizeof(t_move));
    while(cas->square != pos)
        cas = cas->next;
    while(1)
    {
        while(player->piece != piece)
            player = player->next;
        if(prec && player->pos->square % 10 != colfrom)
            player = player->next;
        else if(is_valid(player->pos, cas, game, 1, 1))
        {
            rmv->from = player->pos;
            rmv->to = cas;
            break ;
        }
        player = player->next;
    }
    return (rmv);
}
t_move *check_open(t_game *game)
{
    int i = 0;
    char *temp;
    if(!game->fd)
    {
        game->openn = malloc(sizeof(char **) * 300);
        game->fd = open("./opening.txt", O_RDONLY);
        game->file = malloc(sizeof(char) * 20001);
        int l = read(game->fd, game->file, 20000);
        game->file[l] = 0;
        temp = game->file;
        while(1)
        {
            game->openn[i] = strstr(temp, game->log->move);
            if(!game->openn[i])
                break ;
            temp = game->openn[i];
            while(*temp != '\n')
                temp++;
            *temp = 0;
            temp++;
            i++;
        }
    }
    else
    {
        int p = 0;
        i = 0;
        while(game->openn[i])
        {
            if(strstr(game->openn[i], game->log->move))
                game->openn[p++] = game->openn[i];
            i++;
        }
        i = p;
    }
    printf("i = %d\n", i);
    int k = rand() % i;
    printf("chose line : %s\n", game->openn[k]);
    char *move = find_move(game, game->openn[k]);
    t_move *mov = untranslate(game, move);
    enter_log(mov->from, mov->to, game, 0);//check promote
    free(move);
    return(mov);
}
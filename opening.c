#include "chess.h"

char *find_move(t_game *game, char *line)
{
    int i = strlen(game->log->move);
    int l = 0;
    line += i;
    if(*line == '#')
        return (NULL);
    char *temp = malloc(sizeof(char) * 100);
    while(*line == ' ')
        line++;
    if(line[1] == '.')
        line += 2;
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
        close(game->fd);
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
        if(!p)
            return (NULL);
    }
    int k = rand() % i;
    printf("chose line : %s\n", game->openn[k]);
    char *move = find_move(game, game->openn[k]);
    if(!move)
    {
        game->advancedopen = 1;
        return (NULL);
    }
    t_move *mov = untranslate(game, move);//promote
    free(move);
    return(mov);
}

int past_valid(t_game *game)
{
    int i = strlen(game->log->move);
    char *temp = malloc(sizeof(char) * (i + 1));
    i -= 2;
    while(game->log->move[i] != ' ')
        i--;
    i++;
    int k = i;
    temp[i] = '\0';
    while(--i >= 0)
        temp[i] = game->log->move[i];
    i = 0;
    while(game->openn[i] && (!strstr(game->openn[i], temp) || game->openn[i][k] != '#'))
        i++;
    free(temp);
    if(!game->openn[i])
        return (0);
    game->openn[0] = game->openn[i];
    return (1);
}

struct DownloadData {
    FILE *file;
    bool downloadComplete;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    struct DownloadData *downloadData = (struct DownloadData *)userdata;
    size_t written = fwrite(ptr, size, nmemb, downloadData->file);
    return (written);
}

char *find_url(char *line)
{
    while(*line != '#')
        line++;
    line++;
    return(ft_strjoin("https://www.pgnmentor.com/openings/", line));
}

void init_advanced(t_game *game)
{
    char *url = find_url(game->openn[0]);
    FILE *file = fopen("./open/opening.zip", "wb");
    printf("%s\n", url);
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();

    struct DownloadData downloadData;
    downloadData.file = file;
    downloadData.downloadComplete = false;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &downloadData);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) 
        fprintf(stderr, "Failed to download file: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl);
    fclose(file);
    curl_global_cleanup();
    while (!downloadData.downloadComplete) 
    {
    }
}
#include <stdio.h>

char *find_url(char **openn)
{
    while(*openn[0] != '#')
        *openn++;
    *openn++;
    return(openn[0]);
}
void download(char **line)
{
    char *url = find_url(line);
    printf("%s\n", url);
    //curl_global_init(CURL_GLOBAL_DEFAULT);
    //CURL *curl = curl_easy_init();
    //curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/file.txt");
}

int main()
{
    char **line = {"1.e4 c5 2.Nf3 d6 3.d4 cxd4 4.Nxd4 Nf6 5.Nc3 g6 6.f4#SicilianDragonOther6.zip", NULL};

    download(line);
}
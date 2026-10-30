#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <basetsd.h>

typedef struct
{
    int user_id;
    int current_credits;
    int high_score;
    char name[100];
    int (*current_game)();
} Player;

typedef SSIZE_T ssize_t;

int current_game();
int get_player_data(int uid, Player *p, char datafile[]);
void fatal(char *message);

int main()
{
    char datafile[] = "player_data.txt";

    Player player;
    int fd;

    player.user_id = 1;
    strcpy(player.name, "Player");
    fd = open(datafile, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, _S_IREAD | _S_IWRITE);
    write(fd, &player, sizeof(player));

    player.user_id = 2;
    strcpy(player.name, "Second");
    write(fd, &player, sizeof(player));
    close(fd);

    Player found;
    if (get_player_data(99, &found, datafile) == 1)
    {
        printf("Found player: id=%d, name=%s, credits=%d\n",
               found.user_id, found.name, found.current_credits);
    }
    else
    {
        printf("Player not found\n");
    }

    return 0;
}

void fatal(char *message)
{
    char error_message[100];
    strcpy(error_message, "[!!] Fatal Error ");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
}

int get_player_data(int uid, Player *p, char datafile[])
{
    Player current_player;
    int fd;
    ssize_t bytes_read;

    fd = open(datafile, O_BINARY);
    if (fd < 0)
    {
        perror("Error opening file");
        return -1;
    }

    while ((bytes_read = read(fd, &current_player, sizeof(Player))) > 0)
    {
        if ((size_t)bytes_read < sizeof(Player))
        {
            break;
        }
        if (current_player.user_id == uid)
        {
            *p = current_player;
            close(fd);
            return 1;
        }
    }
    close(fd);

    return -1;
}
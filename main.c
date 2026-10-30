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
void *ec_malloc(unsigned int size);
void fatal(char *message);

int main()
{
    Player player;
    Player loaded_player;

    player.user_id = 1;
    player.current_credits = 100;
    player.high_score = 0;
    strcpy(player.name, "Player");

    // printf("The user id is: %d, the current credits num is: %d, the high score is: %d, the name is: %s",
    //        player.user_id, player.current_credits, player.high_score, player.name);

    int fd;

    char datafile[] = "player_data.txt";

    fd = open(datafile, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, _S_IREAD | _S_IWRITE);

    if (fd == -1)
        fatal("in main() while opening file");

    if (write(fd, &player, sizeof(player)) == -1)
        fatal("in main() while writing buffer to file");

    close(fd);

    fd = open(datafile, O_RDWR);
    read(fd, &loaded_player, sizeof(loaded_player));

    loaded_player.user_id = 2;

    // printf("The loaded user id is: %d, the current credits num is: %d, the high score is: %d, the name is: %s",
    //    loaded_player.user_id, loaded_player.current_credits, loaded_player.high_score, loaded_player.name);

    write(fd, &loaded_player, sizeof(loaded_player));

    get_player_data(1, &player, datafile);
    close(fd);

    return 0;
}

void *ec_malloc(unsigned int size)
{
    void *ptr;
    ptr = malloc(size);
    if (ptr == NULL)
        fatal("in ec_malloc() on memory allocation");
    return ptr;
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
    char line[sizeof(Player)];
    int fd;
    ssize_t bytes_read;

    fd = open(datafile, O_BINARY);
    if (fd < 0)
    {
        perror("Error opening file");
        return -1;
    }

    while ((bytes_read = read(fd, line, sizeof(line))) > 0)
    {
        if (bytes_read < sizeof(Player))
        {
            break;
        }
        Player *current_player = (Player *)line;
        printf("User id: %d\n", current_player->user_id);
    }
    close(fd);

    return 0;
}
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int current_game();
void *ec_malloc(unsigned int size);
void fatal(char *message);

typedef struct
{
    int user_id;
    int current_credits;
    int high_score;
    char name[100];
    int (*current_game)();
} Player;

int main()
{
    Player player;
    Player loaded_player;

    player.user_id = 1;
    player.current_credits = 100;
    player.high_score = 0;
    strcpy(player.name, "Player");

    printf("The user id is: %d, the current credits num is: %d, the high score is: %d, the name is: %s",
           player.user_id, player.current_credits, player.high_score, player.name);

    int fd;

    char datafile[] = "player_data.txt";

    fd = open(datafile, O_WRONLY | O_CREAT | O_TRUNC, _S_IREAD | _S_IWRITE);

    if (fd == -1)
        fatal("in main() while opening file");

    if (write(fd, &player, sizeof(player)) == -1)
        fatal("in main() while writing buffer to file");

    close(fd);

    fd = open(datafile, O_RDONLY);
    read(fd, &loaded_player, sizeof(loaded_player));
    close(fd);

    printf("The loaded user id is: %d, the current credits num is: %d, the high score is: %d, the name is: %s",
           loaded_player.user_id, loaded_player.current_credits, loaded_player.high_score, loaded_player.name);

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
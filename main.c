#include <stdio.h>

struct Player
{
    int userId;
    int currentCredits;
    int highScore;
    char name[100];
    void *function;
};

int main()
{
    struct Player player;
    player.userId = 1;
    player.currentCredits = 100;
    player.highScore = 0;
    strcpy("Player", player.name);
    player.function = NULL;

    printf("The user id is: %d, the current credits num is: %d, the high score is: %d, the name is: %s",
           player.userId, player.currentCredits, player.highScore, player.name);

    return 0;
}
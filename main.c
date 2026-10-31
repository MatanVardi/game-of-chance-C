#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <basetsd.h>

typedef struct Player
{
    int user_id;
    int current_credits;
    int high_score;
    char name[100];
    int (*current_game)(struct Player *);
} Player;

typedef SSIZE_T ssize_t;

int pick_a_number(Player *p);
int dealer_no_match(Player *p);
int find_the_ace(Player *p);
int get_player_data(int uid, Player *p, char datafile[]);
void fatal(char *message);

int main()
{
    char datafile[] = "player_data.txt";
    int fd;

    Player player;

    player.user_id = 1;
    player.current_credits = 2000;
    strcpy(player.name, "Player");
    fd = open(datafile, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, _S_IREAD | _S_IWRITE);
    write(fd, &player, sizeof(player));

    bool game_is_on = true;
    int choice;
    char menu_options[] =
        "1. Play pick a number\n"
        "2. Play no match dealer\n"
        "3. Play find the ace\n"
        "4. View high score\n"
        "5. Change username\n"
        "6. Reset account to 100 credits\n"
        "7. Quit\n";
    while (game_is_on)
    {
        printf("%s", menu_options);

        scanf("%d", &choice);

        if (choice > 7 || choice < 1)
        {
            printf("Select a different choice\n");
        }
        else if (choice == 7)
        {
            game_is_on = false;
        }
        else if (choice == 1)
        {
            player.current_game = pick_a_number;
            player.current_game(&player);
        }
        else if (choice == 2)
        {
            player.current_game = dealer_no_match;
            player.current_game(&player);
        }
        else if (choice == 3)
        {
            player.current_game = find_the_ace;
            player.current_game(&player);
        }

        else
        {
            printf("The mode is not created yet\n");
        }
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

int pick_a_number(Player *p)
{
    printf("Playing Pick a Number\n");
    if (p->current_credits < 10)
    {
        return -1;
    }
    p->current_credits -= 10;
    int random_num = (rand() % 20) + 1;
    int player_guess;

    printf("Type a number between 1-20\n");
    scanf("%d", &player_guess);

    if (player_guess == random_num)
    {
        p->current_credits += 100;
        printf("You won 100 credits\n");
    }
    else
    {
        printf("You lost!\n");
    }

    return 0;
}

int dealer_no_match(Player *p)
{
    printf("Playing Dealer no match\n");
    printf("Enter your wanted credits to bet on\n");
    int player_credits;
    int arr[16];
    int random_num;
    int arr_length = sizeof(arr) / sizeof(arr[0]);
    int found_pair = false;
    scanf("%d", &player_credits);
    if (player_credits > 0)
    {
        for (int i = 0; i < arr_length; i++)
        {
            random_num = (rand() % 100);
            arr[i] = random_num;
        }

        for (int index = 0; index < arr_length; index++)
        {
            for (int j = index + 1; j < arr_length; j++)
            {
                if (arr[index] == arr[j])
                {
                    found_pair = true;
                    p->current_credits -= player_credits;
                    printf("You lost\n");
                    return 0;
                }
            }
        }
        printf("You won\n");

        p->current_credits += player_credits;
    }
    else
    {
        return -1;
    }

    return 0;
}

int find_the_ace(Player *p)
{
    printf("Playing Find the Ace\n");
    int wager;
    printf("How many credits would you like to wager?\n");
    scanf("%d", &wager);

    if (wager <= 0 || wager > p->current_credits)
    {
        printf("Invalid wager.\n");
        return -1;
    }

    int ace = (rand() % 3);
    int choice;

    printf("Type a number between 1-3\n");
    scanf("%d", &choice);
    choice = choice - 1;

    int revealed;
    for (int i = 0; i < 3; i++)
    {
        if (i != ace && i != choice)
        {
            revealed = i;
            break;
        }
    }
    printf("Card %d is a Queen.\n", revealed + 1);

    int switch_card;
    printf("Do you want to switch your card (1 for yes, 0 for no)?\n");
    scanf("%d", &switch_card);

    if (switch_card == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (i != revealed && i != choice)
            {
                choice = i;
                break;
            }
        }
    }
    if (choice == ace)
    {
        printf("You found the ace\n");
        p->current_credits += wager;
    }
    else
    {
        printf("You did not find the ace\n");
        p->current_credits -= wager;
    }

    return 0;
}
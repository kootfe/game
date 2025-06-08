#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios org_termios;

void dis_raw()
{
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_termios);
}

void en_raw()
{
        printf("Enabled raw lol");
        tcgetattr(STDIN_FILENO, &org_termios);
        atexit(dis_raw);
        struct termios raw = org_termios;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

typedef struct {
    int x;
    int y;
} Player;

void loop(Player player);

int main()
{
    printf("\033[2J\033[H");
        en_raw();

    Player player;
    player.x = 5;
    player.y = 5;

    loop(player);
        while (1)
        {
                char c;
                ssize_t n = read(STDIN_FILENO, &c, 1);
        printf("\033[2J\033[H");

                if (n == -1) continue;

                if (c == 'q')
                {
                        break;
                }

                switch (c)
                {
                        case 'a':
                player.x -= 1;
                break;
                        default:
                                printf("Key: %c\n", c);
                }
        loop(player);
        }
        return  0;
}

void loop(Player player)
{
    printf("\rX: %d Y: %d\n", player.x, player.y);
}

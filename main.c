#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

struct termios org_termios;

void dis_raw()
{
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_termios);
}

void en_raw()
{
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
void cls()
{
	printf("\033[2J\033[H");
}
void loop(Player player);
void map(int px, int py, int width, int height);
void command();
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
                if (n == -1) continue;

		cls();
                if (c == 'q')
                {
                        break;
                }

                switch (c)
                {
                        case 'a':
		                player.x -= 1;
               			break;
			case 's':
				player.y += 1;
				break;
			case 'd':
				player.x += 1;
				break;
			case 'w':
				player.y -= 1;
				break;
			case ':':
				command();
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
    map(player.x, player.y, 20, 20);
}

void map(int px, int py, int width, int height)
{
    for(int h = 0; h < height; h++)
    {
        for(int w = 0; w < width; w++)
        {
            if(h == 0 || h == height - 1 || w == 0 || w == width - 1)
            {
                printf("# ");
            }
            else if (w == px && h == py)
            {
                printf("@ ");
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n"); // Newline after each row, NOT after each tile
    }
}

void command()
{
	dis_raw();
	char input[100];

	printf(":");
	fflush(stdout);
	if (fgets(input, sizeof(input), stdin) != NULL)
	{
		input[strcspn(input, "\n")] = '\0';
		if (strcmp(input, "help") == 0)
		{
			printf("This is help command\n");
		} else
		{
			printf("Your command is: %s\n", input);
		}
	}

	printf("press enter to continute...");

	while (getchar() != '\n');
	cls();
	en_raw();
}


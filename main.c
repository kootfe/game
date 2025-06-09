#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

struct termios org_termios;

typedef enum {
    TILE_FLOOR,
    TILE_WALL,
    PLAYER,
    ENEMY,
} Tile;

typedef struct {
    int x;
    int y;
} Player;

typedef struct {
    int x;
    int y;
} Ememy;

typedef struct {
    int width;
    int height;
    Tile **tiles; 
} MapV1;

MapV1 map;

void dis_raw() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_termios);
}

void en_raw() {
    tcgetattr(STDIN_FILENO, &org_termios);
    atexit(dis_raw);
    struct termios raw = org_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void cls() {
    printf("\033[2J\033[H");
}

void init_map(int width, int height) {
    map.width = width;
    map.height = height;
    map.tiles = malloc(height * sizeof(Tile *));
    if (!map.tiles) {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < height; i++) {
        map.tiles[i] = malloc(width * sizeof(Tile));
        if (!map.tiles[i]) {
            perror("malloc");
            exit(1);
        }
    }

    // Fill map with floor and walls on borders
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height -1 || x == 0 || x == width -1) {
                map.tiles[y][x] = TILE_WALL;
            } else {
                map.tiles[y][x] = TILE_FLOOR;
            }
        }
    }
}

void free_map() {
    for (int i = 0; i < map.height; i++) {
        free(map.tiles[i]);
    }
    free(map.tiles);
}

void map_gen(Player player) {
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            if (player.x == x && player.y == y) {
                printf("@ ");
            } else {
                switch (map.tiles[y][x]) {
                    case TILE_FLOOR: printf(". "); break;
                    case TILE_WALL: printf("# "); break;
                    case ENEMY: printf("E "); break;
                    default: printf("? "); break;
                }
            }
        }
        printf("\n");
    }
}

void command();

void loop(Player *player) {
    printf("X: %d Y: %d\n", player->x, player->y);
    map_gen(*player);
}

int main() {
    en_raw();
    cls();
    init_map(20, 20);

    Player player = {5,5};

    loop(&player);

    while (1) {
        char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n == -1) continue;

        cls();
        if (c == 'q') break;

        switch (c) {
            case 'a':
                if (player.x > 1 && map.tiles[player.y][player.x - 1] != TILE_WALL) player.x--;
                break;
            case 'd':
                if (player.x < map.width - 2 && map.tiles[player.y][player.x + 1] != TILE_WALL) player.x++;
                break;
            case 'w':
                if (player.y > 1 && map.tiles[player.y - 1][player.x] != TILE_WALL) player.y--;
                break;
            case 's':
                if (player.y < map.height - 2 && map.tiles[player.y + 1][player.x] != TILE_WALL) player.y++;
                break;
            case ':':
                command();
                break;
            default:
                printf("Key: %c\n", c);
                break;
        }
        loop(&player);
    }

    free_map();
    dis_raw();
    return 0;
}

void command() {
    dis_raw();
    char input[100];

    printf(":");
    fflush(stdout);
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "help") == 0) {
            printf("This is help command\n");
        } else if (strcmp("q", input) == 0) {
            exit(0);
        } else {
            printf("Your command is: %s\n", input);
        }
    }

    printf("press enter to continue...");
    while (getchar() != '\n');
    cls();
    en_raw();
}

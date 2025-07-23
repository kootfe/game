#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "includes/item.h"
#include "includes/enemy.h"
#include "includes/items.h"
#include "includes/player.h"
#include "includes/command.h"
#include "includes/map.h"
#include "includes/tetmin.h"
#include "includes/errors.h"
#include "includes/utils.h"

void loop(Player *player, Map *map, Gate *gate, Enemy *enemy);

int score = 0;
int main()
{
    Player player = {
        .hp = 20, .maxHp = 20,
        .x = 2, .y = 2,
        .maxInvSize = 20,
        .inv = NULL
    };

    Enemy enemy = {
        .x = 12, .y = 6,
        .hp = 20, .maxHp = 20
    };

    ItemStack *inv = malloc(sizeof(ItemStack) * player.maxInvSize);
    if (inv == NULL) {
        errf("[MALLOC FAIL] File: %s, Line: %d\n", __FILE__, __LINE__);
        return -1;
    }
    for (int i = 0; i < player.maxInvSize; i++) {
        inv[i] = EMPTYSLOT;
    }
    player.inv = inv;
    Map map = { 20, 20 };
    Gate gate = { 6, 7 };
    int shouldExit = 0;

    srand(time(NULL));
    en_raw();
    cls();
    loop(&player, &map, &gate, &enemy);
    while (! shouldExit) {
        char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n == -1) continue;

        cls();

        switch(c) {
            case 'q':
                shouldExit++;
                break;
            case 'a':
                move_player(&player, LEFT, map);
                break;
            case 'd':
                move_player(&player, RIGHT, map);
                break;
            case 's':
                move_player(&player, DOWN, map);
                break;
            case 'w':
                move_player(&player, UP, map);
                break;
            case ':':
                command();
                break;
            default:
                break;
        }

        loop(&player, &map, &gate, &enemy);
    }

}

void loop(Player *player, Map *map, Gate *gate, Enemy *enemy)
{
    printf("Score: %d\n", score);
    mv_en(enemy, *player);
    draw_map(*map, player, *gate, *enemy);
    if (player->x == gate->x && gate->y == player->y) {
        map_gen(map, player, gate, enemy);
        ++score; //optimization lol
        cls();
        loop(player, map, gate, enemy);
    } else if (enemy->x == player->x && player->y == enemy->y) {
        cls();
        printf("Gane over!\nPress any button to continue");
        getchar();
        exit(-1);
    }
}


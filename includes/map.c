#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "player.h"
#include "enemy.h"
#include "utils.h"

void map_gen(Map *map, Player *player, Gate *gate, Enemy *enemy)
{
    int x = randr(5, 25);
    int y = randr(5, 25);
    map->height = y;
    map->width = x;
    player->x = 2;
    player->y = 2;
    enemy->x = map->width-2;
    enemy->y = map->height-2;
    gate->x = randr(1, map->width-1);
    gate->y = randr(1, map->height-1);

}

void draw_map(Map map, Player *player, Gate gate, Enemy enemy)
{
    for (int h = 0; h < map.height; h++) {
        for (int w = 0; w < map.width; w++) {
            if (h == 0 || h == map.height -1 || w == 0 || w == map.width - 1) {
                printf("# ");
            } else if (w == player->x && h == player->y) {
                printf("@ ");
            } else if (w == gate.x && h == gate.y) {
                printf("G ");
            } else if (w == enemy.x && h == enemy.y) {
                printf("E ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

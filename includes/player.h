#ifndef PLAYER_H
#define PLAYER_H

typedef struct Map Map;
#include "item.h"

typedef enum Directions {
    UP, DOWN, LEFT, RIGHT,
} Directions;

typedef struct Player {
    int hp;
    int maxHp;
    int x;
    int y;
    int maxInvSize;
    ItemStack *inv;
} Player;

void move_player(Player *player, Directions direct, Map map);
void freePlayer(Player *player);

#endif

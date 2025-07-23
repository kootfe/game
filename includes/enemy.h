#ifndef ENEMY_H
#define ENEMY_H

typedef struct Enemy {
    int x, y, hp, maxHp;
} Enemy;

typedef struct Player Player;

void mv_en(Enemy *enemy, Player player);

#endif

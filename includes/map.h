#ifndef MAP_H
#define MAP_H

typedef struct Enemy Enemy; //i could just include it but forward dec feels safer normaly enemy.h dont depends to map.h

typedef struct Map {
    int width;
    int height;
} Map;

typedef struct Gate {
    int x;
    int y;
} Gate;

typedef struct Player Player;

void map_gen(Map *map, Player *player, Gate *gate, Enemy *enemy);
void draw_map(Map map, Player *player, Gate gate, Enemy enemy);

#endif

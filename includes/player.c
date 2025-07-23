#include "player.h"
#include "map.h"
#include <stdlib.h>


void move_player(Player *player, Directions direct, Map map)
{
    switch (direct) {
        case UP:
            if (player->y - 1 != 0) player->y--;
            break;
        case DOWN:
            if (player->y + 1 != map.height-1) player->y++;
            break;
        case RIGHT:
            if (player->x + 1 != map.width-1) player->x++;
            break;
        case LEFT:
            if (player->x - 1 != 0) player->x--;
            break;
        default:
            break;
    }
}

void freePlayer(Player *player)
{
   // for (int i = 0; i < player -> maxInvSize; i++) {
     //   free(player->inv[i]);
    //}
    free(player->inv);
}

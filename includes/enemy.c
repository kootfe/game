#include "enemy.h"
#include "player.h"
#include <stdlib.h>

void mv_en(Enemy *enemy, Player player)
{
    int res1 = rand() % 2;
    int res2 = (rand() % 100) < 25 ? 0 : 1; //this weirdly works dont ask me how
    if (res2) {
        if (res1) {
            player.x < enemy->x ? enemy->x-- : enemy->x++;
        } else {
            player.y < enemy->y ? enemy->y-- : enemy->y++;
        }
    }
}

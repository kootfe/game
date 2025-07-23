#include "item.h"
#include "player.h"

int itemcmp(const Item i1, const Item i2)
{
    if (i1.id == i2.id) return 0;
    return -1;
}

void addItemRandom(const Item item, ItemStack *inv, Player *player)
{
    int len = player->maxInvSize;
    int check = 0;
    
    for (int i = 0; i < len; i++) {
        if (inv[i].type != PLACEHOLDER && itemcmp(item, inv[i].item.item) == 0 && inv[i].count < inv[i].item.item.maxStack) {
            inv[i].count++;
            check++;
            break;
        }
    }

    if(!check) {
        for (int i = 0; i < len; i++) {
            if (inv[i].type == PLACEHOLDER) {
                ItemStack itemStackTemp = {.type = ITEM, .count = 1, .item = item };
                inv[i] = itemStackTemp;
                break;
            }
        }
    }
}

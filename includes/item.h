#ifndef ITEM_H
#define ITEM_H

typedef struct Item {
    int maxStack;
    int id;
    char* name;
} Item;

typedef struct ItemStack {
    enum {
        ITEM,
        SWORD,
        PLACEHOLDER,
    } type;
    union {
        Item item;
    } item;
    int count;
} ItemStack;

typedef struct Player Player;

int itemcmp(const Item i1, const Item i2);
void addItemRandom(const Item item, ItemStack *inv, Player *player);

#endif

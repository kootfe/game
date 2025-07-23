#ifndef ITEMS_H
#define ITEMS_H

#include "item.h"
//PLACEHOLDER ITEMS 
const Item AIR = { .id = 0, .maxStack = 1, .name = "Air" };
const ItemStack EMPTYSLOT = { .item = { .item = AIR }, .count = 1, .type = PLACEHOLDER };

//ITEMS
const Item GEM = { .id = 1, .maxStack = 99, .name = "Gem" };

#endif

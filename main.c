#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUFF_LEAK 99

struct termios org_termios;

void dis_raw()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_termios);
}

void en_raw()
{
    tcgetattr(STDIN_FILENO, &org_termios);
    atexit(dis_raw);
    struct termios raw = org_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

typedef struct {
    int maxStack;
    int id;
    char* name;
} Item;

typedef struct {
    int dmg;
    int durab;
    int knockback;
    Item base;
} SwordItem;

typedef struct {
    enum {
        ITEM,
        SWORD,
        PLACEHOLDER,
    } type;
    union {
        Item item;
        SwordItem sword;
    } item;
    int count;
} ItemStack;

typedef struct {
    int x;
    int y;
    Item testItemTemp;
    ItemStack* inv;
    int invSize;
} Player;

typedef struct {
    int x;
    int y;
} Enemy;

typedef struct {
    int width;
    int height;
} MapV1;

const Item AIR = {.id = 0, .maxStack = 0, .name = "Air"};
const ItemStack EMPTYSLOT = { .item = AIR, .count = 0, .type = PLACEHOLDER };
MapV1 map = { 20, 20 };
Player player = { 5, 5, {4, 2, "Test"} };

int str_calt(const char *str, const char target)
{
    int ret = 1;
    while (*str)
    {
        if (*str == target)
        {
            ret++;
        }
    str++;
    }
    return ret;
}

char **split(const char *str, const char target, int *err)
{
    int len = str_calt(str, target);
    char **arr = malloc(sizeof(char*) * (len + 1));
    int i = 0;
    char buff[255];
    int pos = 0;
    while (*str)
    {
        if (*str == target)
        {
            buff[i] = '\0';
            i = 0;
            arr[pos++] = strdup(buff);
        } else {
            buff[i++] = *str;
            if (i == 253) {
                *err = BUFF_LEAK;
               for (int j = 0; j < pos; j++)
                {
                    free(arr[j]);
                }
               free(arr);
                return NULL; //just fuck you im out sign lol
            }
        }
        str++;
    }

    if (i > 0)
    {
        buff[i] = '\0';
        arr[pos++] = strdup(buff);
    }

    arr[pos] = NULL;
    *err = 0;
    return arr;
}

void freecmd(char **arr)
{
    int i = 0;
    while (1)
    {
        if (arr[i] != NULL) {
            free(arr[i++]);
        } else {
            break;
        }
    }
    free(arr);
}


void cls()
{
    printf("\033[2J\033[H");
}

Enemy enemy = { 10, 10 };
void loop();
void map_gen(int px, int py);
void command();
void mv_en();

int main()
{
    printf("\033[2J\033[H");
    en_raw();
    srand(time(NULL));
    ItemStack *inv = malloc(sizeof(ItemStack) * player.invSize);
    for (int i = 0; i < player.invSize; i++)
    {
        inv[i] = EMPTYSLOT;
    }
    player.inv = inv;
    loop();
    while (1)
    {
        char c;
        ssize_t n = read(STDIN_FILENO, &c, 1);
        if (n == -1) continue;

        cls();
        if (c == 'q')
        {
            break;
        }

        switch (c)
        {
            case 'a':
                if (player.x != 1) player.x -= 1;
                break;
            case 's':
                if (player.y != map.height-1) player.y += 1;
                break;
            case 'd':
                if (player.x != map.width-1) player.x += 1;
                break;
            case 'w':
                if (player.y != 1) player.y -= 1;
                break;
            case ':':
                command();
                break;
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUFF_LEAK 99

struct termios org_termios;

void dis_raw()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_termios);
}

void en_raw()
{
    tcgetattr(STDIN_FILENO, &org_termios);
    atexit(dis_raw);
    struct termios raw = org_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

typedef struct {
    int maxStack;
    int id;
    char* name;
} Item;

typedef struct {
    int dmg;
    int durab;
    int knockback;
    Item base;
} SwordItem;

typedef struct {
    enum {
        ITEM,
        SWORD,
    } type;
    union {
        Item item;
        SwordItem sword;
    } item;
    int count;
} ItemStack;

typedef struct {
    int x;
    int y;
    Item testItemTemp;
    ItemStack* inv;
    int invSize;
    int 
} Player;

typedef struct {
    int x;
    int y;
} Enemy;

typedef struct {
    int width;
    int height;
} MapV1;

MapV1 map = { 20, 20 };

int str_calt(const char *str, const char target)
{
    int ret = 1;
    while (*str)
    {
        if (*str == target)
        {
            ret++;
        }
    str++;
    }
    return ret;
}

char **split(const char *str, const char target, int *err)
{
    int len = str_calt(str, target);
    char **arr = malloc(sizeof(char*) * (len + 1));
    int i = 0;
    char buff[255];
    int pos = 0;
    while (*str)
    {
        if (*str == target)
        {
            buff[i] = '\0';
            i = 0;
            arr[pos++] = strdup(buff);
        } else {
            buff[i++] = *str;
            if (i == 253) {
                *err = BUFF_LEAK;
               for (int j = 0; j < pos; j++)
                {
                    free(arr[j]);
                }
               free(arr);
                return NULL; //just fuck you im out sign lol
            }
        }
        str++;

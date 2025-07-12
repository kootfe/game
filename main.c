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
    int InvSize;
} Player;

typedef struct {
    Item item;
    int x;
    int y;
    int count;
} ItemEntity;
const Item AIR = { .id = 0, .maxStack = 1, .name = "Air" };
const ItemStack EMPTYSLOT = { .item = AIR, .count = 1, .type = PLACEHOLDER };

const Item GEM = { .id = 1, .maxStack = 99, .name = "Shiny Gem" };

typedef struct {
    int x;
    int y;
} Enemy;

typedef struct {
    int width;
    int height;
} MapV1;

typedef struct {
    int x;
    int y;
} Gate;

int itemcmp(const Item i1, const Item i2)
{
    if (i1.id == i2.id) return 0;
    return -1;
}

MapV1 map = { 20, 20 };
Gate gate = { 8, 15 };
Player player = { 5, 5, {4, 2, "Test"}, NULL, 20 };
int score = 0;
void freeGame(Player *player);
void giveMeMap();

void freePlayer(Player *play)
{
    free(play->inv);
}

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

void addItemRandom(const Item item, ItemStack *inv)
{
    int len = player.InvSize;
    int check = 0;
    for (int i = 0; i < len; i++)
    {
        if (inv[i].type != PLACEHOLDER && itemcmp(item, inv[i].item.item) == 0 && inv[i].count < inv[i].item.item.maxStack)
        {
            inv[i].count++;
            check = 1;
            break;
        }
    }
    if (!check)
    {
        for (int i = 0; i < len; i++) {
            if (inv[i].type == PLACEHOLDER)
            {
                ItemStack itemStackTemp = {.type=ITEM, .count = 1, .item = item};
                inv[i] = itemStackTemp;
                break;
            }
        }
    }
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

void printInv(ItemStack *inv);

int main()
{
    printf("\033[2J\033[H");
    en_raw();
    srand(time(NULL));
    ItemStack *inv = malloc(sizeof(ItemStack) * player.InvSize);
    for (int i = 0; i < player.InvSize; i++)
    {
        inv[i] = EMPTYSLOT;
    }
    player.inv = inv;
    addItemRandom(GEM, inv);
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
            case 'i':
                printInv(player.inv);
                break;
            default:  
                printf("Key: %c\n", c);  
        }  

        loop();  
    }  
    freePlayer(&player);  
    return  0;

}

//void loop(Player player)
void loop()
{
    mv_en(); // move enemy first

    if (enemy.x == player.x && enemy.y == player.y)  
    {  
        cls();  
        map_gen(player.x, player.y); // show the final map  
        printf("\nGame Over\n");  
        sleep(2); // short delay to show game over  
        exit(0);  
    } else if (player.x == gate.x && player.y == gate.y)
    {
        giveMeMap();
        ++score;
    }
    cls();  
    printf("MH: %d, MW: %d\n", map.height, map.width);
    map_gen(player.x, player.y);  
    printf("Score: %d", score);
}

void mv_en()
{
    int res1 = rand() % 2;
    int res2 = (rand() % 100) < 25 ? 0 : 1;
    if (res1) {
        if (res2) {
            if (player.x < enemy.x) {
                enemy.x--;
            } else {
                enemy.x++;
            }
        }
    } else {
        if (res2) {
            if (player.y < enemy.y) {
                enemy.y--;
            } else {
                enemy.y++;
            }
        }
    }
}
void giveMeMap()
{
    int x = rand() % (18) + 10;
    int y = rand() % 18 + 10;
    map.height = y;
    map.width = x;
    player.x = 2;
    player.y = 2;
    enemy.y = y-3;
    enemy.x = x-2;
    gate.y = rand() % (map.height - 1) + 2;
    gate.x = rand() % (map.width - 1) + 2;
}

void map_gen(int px, int py)
{
    for(int h = 0; h <= map. height; h++)
    {
        for(int w = 0; w <= map.width; w++)
        {
            if(h == 0 || h == map.height || w == 0 || w == map.width)
            {
                printf("# ");
            }
            else if (w == px && h == py)
            {
                printf("@ ");
            } else if (w == enemy.x && h == enemy.y) {
                printf("E ");
            }
            else if (w == gate.x && h == gate.y)
            {
                printf ("G ");
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void printInv(ItemStack *inv)
{
    for (int i = 0; i < player.InvSize; i++)
    {
        printf("Item: %s, Count: %d, ID: %d\n", inv[i].item.item.name, inv[i].count, inv[i].item.item.id);
    }
    printf("press enter to continute...");  

    while (getchar() != '\n');  
    cls();  
 
}

void command()
{
    dis_raw();
    char input[1024];
    char **arr = NULL;
    int err;
    printf(":");
    fflush(stdout);
    if (fgets(input, sizeof(input), stdin) != NULL)
    {
        input[strcspn(input, "\n")] = '\0';
        arr = split(input, ' ', &err);
        //printf("Command: %s\n", input);
    }

    if (strcmp("setp", arr[0]) == 0)  
    {  
        if (arr[1] == NULL || arr[2] == NULL) exit(-1);  
        player.x = atoi(arr[1]);  
        player.y = atoi(arr[2]); //not my probllem if they can use it...  
    } else if (strcmp("help", arr[0]) == 0)  
    {  
        printf("Read The Fucking Manual\n");  
    } else if (strcmp("inv", arr[0]) == 0) {
        printInv(player.inv);
    } else if (strcmp("give", arr[0]) == 0) {
        addItemRandom(GEM, player.inv);
    } else if (strcmp("map", arr[0]) == 0) {
        giveMeMap();
    }

    freecmd(arr);  
    printf("press enter to continute...");  

    while (getchar() != '\n');  
    cls();  
    en_raw();

}

void freeGame(Player *player)
{
    freePlayer(player);
}



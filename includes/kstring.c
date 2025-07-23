#include <string.h>
#include <stdlib.h>
#include "kstring.h"
#include "errors.h"

/**
 * Str Calt functions was born by mistake. Actual name str_calt but my muscle memory does str_calt so its this now...
 * What it does?
 * its simple takes a string... fot exemple 
 * char *str = "Hello World";
 * takes a target for exemple
 * char tr = ' ';
 * and calcualtes how many seperete words with spliting that so in our exemple
 * Hello World how splited from ' ' so its now 
 * { 'hello", "world" } we have 2 so we return 2.
 * the ret = 1 as defualt becouse that/
 */
int str_calt(const char *str, const char target)
{
    if (!str) 
    {
        errf("Hey... You know you should enter a text right?");
    }
    int ret = 1;
    while (*str) {
        if (*str == target) {
            ret++;
        }
        str++;
    }
    return ret;
}

/**
 * Split fincton takes a string, a target and a error variable
 * it does create a array and needs get freed by free_split() fincton for memory managment(optional if you want your gmae run on 1fps)
 * for exemple out string is "hello my names is Mr. Meatball" and tarhet is ' '
 * this func will return an allocated array as {"Hello', "my', "name", "is", "Mr.", "Meatball" };
 */
char **split(const char *str, const char target, int *err)
{
    int len = str_calt(str, target);
    int i = 0;
    char buff[SPLIT_BUFF_SIZE];
    int pos = 0;
    char **arr = malloc(sizeof(char*) * (len + 1));
    if (arr == NULL) {
        errf("[MALLOC FAIL]: File: %s Line: %d\n", __FILE__, __LINE__);
        *err = MALLOC_FAIL_ERR;
        return NULL;
    }

    while (*str) {
        if (*str == target)
        {
            buff[i] = '\0';
            i = 0;
            arr[pos++] = strdup(buff);
            if (arr[pos-1] == NULL) {
                *err = STRDUP_FAIL_ERR;
                errf("[STRDUP FAIL]: File: %s Line: %d\n", __FILE__, __LINE__);
                goto fail;
            }
        } else {
            buff[i++] = *str;
            if (i == SPLIT_BUFF_SIZE - 1) { // Just to be sure -1
                *err = BUFF_LEAK_ERR;
                goto fail;
                return NULL; //error managmetn ass returnung err the reason is that i return NULL
            }
        }
            str++;
    }

    if (i > 0) {
        buff[i] = '\0';
        arr[pos++] = strdup(buff);
        if (arr[pos-1] == NULL) {
            errf("[STRDUP FAIL]: File: %s Line: %d\n", __FILE__, __LINE__);
            *err = STRDUP_FAIL_ERR;
            goto fail;
        }
    }

    arr[pos] = NULL;
    *err = 0;
    return arr;

fail:
    for (int j = 0; j < pos; j++) {
        free(arr[j]);
    }
    free(arr);
    return NULL;
}

void free_split(char **arr)
{
    if (!arr) return;
    for (int i = 0; arr[i]; i++) {
        free(arr[i]);
    }
    free(arr);
}

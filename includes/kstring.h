#ifndef KSTRING_H
#define KSTRING_H

#define SPLIT_BUFF_SIZE 255

int str_calt(const char *str, const char target);
char **split(const char *str, const char target, int *err);
void free_split(char **arr);

#endif

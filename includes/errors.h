#ifndef ERRORS_H
#define ERRORS_H

#define BUFF_LEAK_ERR 99
#define MALLOC_FAIL_ERR 100
#define STRDUP_FAIL_ERR 101

void errf(const char *s, ...);

#endif

#include <stdio.h>
#include "errors.h"
#include "kstring.h"
#include "tetmin.h"
#include <string.h>
#include "utils.h"

void command()
{
    dis_raw();
    char input[1024]; //i dont want soo much ram eater so short commands
    int err;
    char **cmd;
    printf(":");
    fflush(stdout);
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        cmd = split(input, ' ', &err);
        err = 0; // to be sure...
    } else {
        errf("[USER ERROR] No input.");
    }

    if (err) {errf("error on split func"); return;}

    if (!cmd || !cmd[0]) {errf("Somethinf went wrong"); return;}

    if (strcmp("help", cmd[0]) == 0) {
        printf("Read The Fucking Manual\n");
    }

    free_split(cmd);
    printf("Press enter to continue...");
    while(getchar() != '\n');
    cls();

    en_raw();
}

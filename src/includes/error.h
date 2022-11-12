#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

void error(char *error_msg, int line)
{
    printf("야 개짖는 소리좀 안나게해라\n");
    printf("%s, %d Line\n", error_msg, line);
    exit(-1);
}

#endif
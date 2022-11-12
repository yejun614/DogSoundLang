#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

void error(char *error_msg, int line, char *error_function)
{
    printf("야 개짖는 소리좀 안나게해라\n");
    printf("%s, %d Line From %s\n", error_msg, line, error_function);
    exit(-1);
}

#endif
#ifndef SCRIPT_RUNNER_H
#define SCRIPT_RUNNER_H

#include <cstdio>
#include <cstdlib>
#include "global.h"
#include "parser.h"

void run_script() {
  char ch, buffer[REPL_CMD_LENGTH] = "";
  size_t line = 0, length = 0;

  FILE *fp = fopen(SCRIPT_PATH, "r");

  if (fp == NULL) {
    printf("[run_script][%s] 스크립트 파일을 열 수 없습니다.\n", SCRIPT_PATH);
    exit(1);
  }

  while(true) {
    line++;
    length = 0;

    while (true) {
      ch = (char)fgetc(fp);
      if (ch == EOF || ch == '\n') break;

      buffer[length++] = ch;
    }

    if (ch == EOF) break;
    if (length == 0) continue;

    buffer[length] = '\0';
    parser(buffer, line, "Main");
  }

  fclose(fp);
}


#endif
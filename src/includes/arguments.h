#ifndef ARGUMENTS_H
#include <cstring>
#include "global.h"
#define ARGUMENTS_H

void arg_help() {
  printf(" -s <SCRIPT_PATH> \t\t 스크립트 파일 위치\n");
}

char arg_parser(int argc, char** argv) {
  int running_mode = RUN_REPL;

  for (int arg = 0; arg < argc - 1; arg++) {
    if (strcmp(argv[arg], "-s") == 0) {
      strcpy(SCRIPT_PATH, argv[++arg]);
      running_mode = RUN_SCRIPT;
    }
  }

  return running_mode;
}

#endif
#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <cstring>
#include "global.h"

void arg_help() {
  printf(" -h               \t\t 사용법 출력\n");
  printf(" -v               \t\t 버전 출력\n");
  printf(" -s <SCRIPT_PATH> \t\t 스크립트 파일 위치\n");
  printf(" -E               \t\t C언어로 실행가능한 소스코드 출력");
}

char arg_parser(int argc, char** argv) {
  int running_mode = RUN_REPL;

  for (int arg = 0; arg < argc; arg++) {
    if (strcmp(argv[arg], "-h") == 0) {
      arg_help();
      running_mode = RUN_NOTHING;

    } else if (strcmp(argv[arg], "-v") == 0) {
      printf("개소리 (DogSoundLang)\n");
      printf("version 0.1\n");
      running_mode = RUN_NOTHING;

    } else if (strcmp(argv[arg], "-s") == 0) {
      strcpy(SCRIPT_PATH, argv[++arg]);
      running_mode = RUN_SCRIPT;
    } else if (strcmp(argv[arg], "-E") == 0) {
      running_mode = RUN_EMBED_C_SRC;
    }
  }

  return running_mode;
}

#endif
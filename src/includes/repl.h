#ifndef REPL_H
#define REPL_H

#include <cstring>

#include "global.h"
#include "parser.h"

/**
 * @brief 강아지 아스키 아트 출력
 */
void print_dog_ascii_art() {
  printf("\n    /\\\n");
  printf("   /  \\_____\n");
  printf("  /    \\   *|_____\n");
  printf("  \\        _______|\n");
  printf("  /        \\_____    개소리 (DogSoundLang)\n");
  printf("  /       ______/    version 0.1\n");
  printf("  /______/\n\n");

}

/**
 * @brief REPL 모드
 * @return program exit code (0: 정상 종료)
 */
int repl() {
  print_dog_ascii_art();

  while (true) {
    // Prompt
    printf("%s", REPL_PROMPT);

    char *cmd = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&cmd, &len, stdin);

    if (cmd[0] == '\n') {
      free(cmd);
      continue;
    }
    
    cmd[strlen(cmd) - 1] = '\0';
    
    if (strcmp(cmd, "그만해") == 0) {
      free(cmd);
      printf("뫙뫙\n");
      exit(0);
    }

    // 명령어 실행
    parser(cmd, 1, "Main");

    free(cmd);
    
    printf("\n");
  }

  return 0;
}

#endif

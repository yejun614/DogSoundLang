#ifndef REPL_H
#define REPL_H

#include "global.h"
#include "parser.h"

/**
 * @brief 강아지 아스키 아트 출력
 */
void print_dog_ascii_art() {
  printf("\n      / \\__\n");
  printf("     (    @\\___\n");
  printf("    /         O   개소리 (DogSoundLang)\n");
  printf("   /   (_____/    version 0.1\n");
  printf("  /_____/   U\n\n");
}

/**
 * @brief REPL 모드
 * @return program exit code (0: 정상 종료)
 */
int repl() {
  print_dog_ascii_art();

  char inputCmd[REPL_CMD_LENGTH] = "";

  while (true) {
    // Prompt
    printf("%s", REPL_PROMPT);

    // 명령어 입력
    if (!scanf("%99[^\n]%*c", inputCmd)) {
      // 입력 없음
      break;
    }

    // 명령어 실행
    parser(inputCmd, 1, "Main");
  }

  return 0;
}

#endif
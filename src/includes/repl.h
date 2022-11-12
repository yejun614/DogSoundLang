#ifndef REPL_H
#include "global.h"
#include "parser.h"
#define REPL_H

/**
 * @brief REPL 모드
 * @return program exit code (0: 정상 종료)
 */
int repl() {
  char inputCmd[REPL_CMD_LENGTH] = "";

  while (true) {
    // Prompt
    printf("%s", REPL_PROMPT);

    // 명령어 입력
    if (!scanf("%s", inputCmd)) {
      // 입력 없음
      break;
    }

    // 명령어 실행
    parser(inputCmd, 1);
  }

  return 0;
}

#endif
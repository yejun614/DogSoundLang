#include <cstdio>
#include "./includes/arguments.h"
#include "./includes/repl.h"
#include "./includes/script_runner.h"

#define DEBUG

int main(int argc, char** argv) {
  switch (arg_parser(argc, argv)) {
  case RUN_NOTHING:
    printf("아무것도 실행되지 않았습니다.\n");
    break;

  case RUN_REPL:
    repl();
    break;

  case RUN_SCRIPT:
    run_script();
    break;

  case RUN_EMBED_C_SRC:
    printf("아직 개발중인 기능입니다.");
    break;
  }

  return 0;
}

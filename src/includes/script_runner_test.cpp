#include <cstdio>
#include <cstring>
#include "global.h"
#include "script_runner.h"

int main() {
  strcpy(SCRIPT_PATH, "../../build/test/멍멍");
  run_script();

  return 0;
}

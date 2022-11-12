#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

#include <map>
#include <string>
#include <vector>
#include <utility>

using namespace std;

/**
 * @brief 함수 구조체
 */
typedef struct Function {
  size_t param_count;
  vector<string> code;
} Function;

/**
 * @brief 프로그램 함수를 저장
 */
map<string, Function> FUNCTIONS;

/**
 * @brief 프로그램 안에서 사용할 정수형 변수의 최대 개수
 */
#define TOTAL_VARIABLE_SIZE 1024

/**
 * @brief REPL 모드에서 입력되는 명령어 문자열 크기
 */
#define REPL_CMD_LENGTH 4096

/**
 * @brief REPL 모드에서 보이는 prompt 문자열 크기
 */
#define REPL_PROMPT_LENGTH 100

/**
 * @brief 실행할 때 REPL 모드로 할지 스크립트 실행 모드로 할지 결정
 */
#define RUN_NOTHING 0
#define RUN_REPL 1
#define RUN_SCRIPT 2
#define RUN_EMBED_C_SRC 3

/**
 * @brief REPL 모드에서 보이는 PROMPT 문자열
 */
char REPL_PROMPT[REPL_PROMPT_LENGTH] = " > ";

char FUNCTION_NAME[100] = "";

/**
 * @brief 스크립트 실행 파일 위치
 */
char SCRIPT_PATH[1024] = "";

/**
 * @brief 프로그램 정수형 변수
 */
int INT_VARS[TOTAL_VARIABLE_SIZE];

#endif
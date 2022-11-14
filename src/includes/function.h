#ifndef FUNCTION_H
#define FUNCTION_H

#include <cstring>
#include <string>
#include <algorithm>
#include "global.h"
#include "parser.h"
#include "error.h"

using namespace std;

void parser(char *s, int line, char *function_name);

// 마지막으로 선언된 함수의 이름
string LAST_DEFINED_FUNCTION;

/**
 * @brief 함수 정의
 * @param function_name 함수 이름
 * @param param_count 파라미터 개수
 */
void define_function(char *function_name, unsigned int param_count) {
  Function func;
  func.param_count = param_count;

  string function_name_str = function_name;
  auto it = FUNCTIONS.find(function_name);

  if (it == FUNCTIONS.end()) {
    Function func;
    func.param_count = param_count;

    FUNCTIONS.insert(make_pair(function_name, func));

  } else {
    it->second = func;
  }

  LAST_DEFINED_FUNCTION = function_name_str;
}

/**
 * @brief 마지막으로 정의된 함수 코드 내용 추가
 * @param function_name 함수 이름
 * @param param_count 파라미터 개수
 */
void append_code_function(char *line, int line_number) {
  string line_str = line;

  auto it = FUNCTIONS.find(LAST_DEFINED_FUNCTION);

  if (it == FUNCTIONS.end()) {
    error("함수가 정의되기 이전에 함수 내용이 입력되었습니다.", line_number, "Main");
    exit(1);
  }

  it->second.code.push_back(line_str);
}

/**
 * @brief 함수 정의
 * @param function_name 함수 이름
 * @param param_count 파라미터 개수
 */
void call_function(char *function_name, char args[][1024], int line_number) {
  string function_name_str = function_name;

  auto it = FUNCTIONS.find(function_name_str);

  if (it == FUNCTIONS.end()) {
    error("정의되지 않은 함수가 호출되었습니다.", line_number, "Main");
    exit(1);
  }

  vector<string> &code = it->second.code;
  string param_pattern = "", param_str;
  char line_c_str[REPL_CMD_LENGTH];

  line_number = 1;

  for (auto line = code.begin(); line != code.end(); line++) {
    string current = *line;

    for (int param = it->second.param_count-1; param >= 0; param--) {
      param_pattern = "개껌";
      for (int i = 0; i < param + 1; i++) {
        param_pattern += '!';
      }

      string::size_type pos = 0;
      param_str = args[param];

      while ((pos = current.find(param_pattern, pos)) != string::npos) {
        current.replace(pos, param_pattern.size(), param_str);
        pos += param_str.size();
      }
    }

    strcpy(line_c_str, current.c_str());
    parser(line_c_str, line_number++, function_name);
  }
}

#endif

#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <algorithm>
#include "global.h"

using namespace std;

string LAST_DEFINED_FUNCTION;

void define_function(char *function_name, uint param_count) {
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

  LAST_DEFINED_FUNCTION = function_name;
}

void append_code_function(char *line) {
  string line_str = line;

  auto it = FUNCTIONS
}

#endif
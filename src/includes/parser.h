#ifndef PARSER_H
#define PARSER_H

#define DEBUG

#include <stdio.h>
#include <string.h>

#include "global.h"
#include "error.h"
#include "function.h"

int function_checker = 0;
int loop_checker = 0;
int if_checker = 0;
char loop_condition_code[REPL_CMD_LENGTH] = "";
int loop_condition_left_index = 0;


int white_space(char *s)
{
  int index = 0;
  while (s[index] == ' ')
  {
    index++;
  }
  return index;
}

void m_int_parser(char *s, int line, char *function_name, int left){
  int index = 0;
  int value = 0;
  int flag = 0;
  while (strncmp(s, "망", 3) == 0) {
    index++;
    s += 3; //한글 1글자는 3Byte
  }
  while (s[0] == '!')
  {
    value++;
    s++;
  }
  if (s[0] == '?')
  {
    s++;
    flag = 1;
  }
  if(s[0] != NULL)
  {
    error("SyntaxError: invalid syntax", line, function_name);
  }
  if (flag){
    INT_VARS[index] = INT_VARS[index]-value-INT_VARS[left];
  }
  else {
    INT_VARS[index] = -value;
  }
  #ifndef DEBUG
    printf("[m_int_parser][망%d]INT_VARS[%d] = %d\n", flag, index, INT_VARS[index]);
  #endif
}

/**
 * @brief int형 변수를 선언하는 코드
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void
 */
void p_int_parser(char *s, int line, char *function_name, int left){
  int index = 0;
  int value = 0;
  int flag = 0;
  while (strncmp(s, "멍", 3) == 0) {
    index++;
    s += 3; //한글 1글자는 3Byte
  }
  while (s[0] == '!')
  {
    value++;
    s++;
  }
  if (s[0] == '?')
  {
    s++;
    flag = 1;
  }
  if(s[0] != NULL)
  {
    error("SyntaxError: invalid syntax", line, function_name);
  }
  if (flag){
    INT_VARS[index] = INT_VARS[index]+value+INT_VARS[left];
  }
  else {
    INT_VARS[index] = value;
  }
  #ifndef DEBUG
    printf("[p_int_parser][멍%d]INT_VARS[%d] = %d\n", flag, index, INT_VARS[index]);
  #endif
}

/**
 * @brief int형 변수의 문자열 인덱스를 가져오는 코드
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void
 */
int get_parser(char *s, int line, char *function_name){
  int index = 0;

  s += 3;

  while (s[0] == '!')
  {
    index++;
    s++;
  }

  #ifndef DEBUG
    printf("[get_parser][손]INT_VARS[%d] = %d\n", index, INT_VARS[index]);
  #endif
  if (s[0] == ' ' || s[0] == NULL)
    return index;
  
  error("SyntaxError: invalid syntax", line, function_name);

}

int compare_parser(char *s, int line, char *function_name, int left) {
  int right = 0, flag;
  s += 30;
  s += white_space(s);
  
  if(strncmp(s, "엄마?", 7) == 0) {
    flag = 0;
  }
  else if(strncmp(s, "아빠?", 7) == 0) {
    flag = 1;
  }
  else if(strncmp(s, "둘다?", 7) == 0) {
    flag = 2;
  }
  else {
    error("SyntaxError: for invalid condition", line, function_name);
  }
  s += 7;
  s += white_space(s);

  if(strncmp(s, "손", 3) != 0)
    error("SyntaxError: for invalid syntax", line, function_name);
  s+= 3;
  while(s[0] == '!')
  {
    s++;
    right++;
  }
  #ifndef DEBUG
    //printf("left : %d right : %d flag : %d\n", left, right, flag);
  #endif
  if (flag == 0) {
    return INT_VARS[left] > INT_VARS[right];
  }
  else if(flag == 1){
    return INT_VARS[left] < INT_VARS[right];
  }else if (flag == 2){
    return INT_VARS[left] == INT_VARS[right];
  }
}

/**
 * @brief int형 변수를 선언하는 코드
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @param[in] value 출력할 값
 * @param[in] format_string 포맷스트링
 * @return void
 */
void print_parser(char *s, int line, int value, char *format_string, char *function_name){

  int index = 0;

  s += 3*4; //한글 1글자는 3Byte
  if(s[0] != NULL)
  {
    
    error("SyntaxError: invalid syntax", line, function_name);
  }
  printf(format_string, value);
}


void make_function(char *s, int line, char *function_name) //return function_name, parameter2
{
  
  int value = 0;
  s += 3*2;
  s += white_space(s);
  
  int index = 0;
  while(s[0] != '(')
  {
    if(s[0] == ' ')
    {
      error("SyntaxError: invalid function name", line, function_name);
    }
    index++;
    s++;
  }
  
  if (index == 0)
    error("SyntaxError: function name not found", line, function_name);

  strncpy(FUNCTION_NAME, s-index, index);
  
  if(s[0] != '(')
    error("SyntaxError: invalid syntax", line, function_name);
  s++;
  while (s[0] != ')')
  {
    s += white_space(s);
    if (s[0] == NULL)
      error("SyntaxError: invalid syntax", line, function_name);
    else if (s[0] == ',')
    {
      s++;
    }
    else if( strncmp(s, "개껌", 6) == 0 )
    {
      s += 3*2;
      value++;
    }
  }
  #ifndef DEBUG
    printf("[make_function][개껌]%s(%d)\n", FUNCTION_NAME, value);
  #endif

  define_function(FUNCTION_NAME, value);
}

void function_call(char *s, int line, char *function_name)
{
  
  s += 3*3;
  s += white_space(s);

  char parameter[10][1024];
  int value = 0;

  int index = 0;
  while(s[0] != '(')
  {
    if(s[0] == ' ')
    {
      error("SyntaxError: invalid function name", line, function_name);
    }
    index++;
    s++;
  }
  
  if (index == 0)
    error("SyntaxError: function name not found", line, function_name);

  strncpy(FUNCTION_NAME, s-index, index);
  
  if(s[0] != '(')
    error("SyntaxError: invalid syntax", line, function_name);

  
  s++;
  if(s[0] == ')')
  {
    call_function(FUNCTION_NAME, parameter, line);
  }
  index = 1;
  value = 0;
  while (s[0] != ')')
  {
    s += white_space(s);
    
    if (s[0] == NULL)
      error("SyntaxError: invalid syntax", line, function_name);
    else if (s[0] == ',')
    {
      memset(parameter[index-1], 0, 1024);
      strncpy(parameter[index-1], s-value, value);
      s++;
      index++;
      value = 0;
    }
    else if( strncmp(s, "손", 3) == 0 )
    {
      
      s += 3*1;
      value = 3;
      while(s[0] == '!')
      {
        s++;
        value++;
      }
    }
    else if( s[0] == '!')
    {
      s ++;
      value++;
    }
    
  }
  memset(parameter[index-1], 0, 1024);
  strncpy(parameter[index-1], s-value, value);
  
  #ifndef DEBUG
    printf("[call_function][우쭈쭈]%s(", FUNCTION_NAME);
    for(int i = 0; i < index; i++)
    {
      if(i != 0)
        printf(", ");
      printf("%s", parameter[i]);
    }
    printf(")\n");
  #endif
  
  call_function(FUNCTION_NAME, parameter, line);
  
}

void loop_parser(char *s, int line, char *function_name) {
  s += 2*3;
  s += white_space(s);
  loop_condition_left_index = get_parser(s, line, function_name);
  s += 3;
  s += loop_condition_left_index;
  s += white_space(s);

  strcpy(loop_condition_code, s);
  // printf("loop_code : %d %s\n", loop_condition_left_index, loop_condition_code);
  // printf("%d\n", compare_parser(s, line, function_name, loop_condition_left_index));
  define_function("loop_temp_func", 0);

  #ifndef DEBUG
    printf("[loop_parser][돌아] loop_code : %s", loop_condition_code);
  #endif
  // while (compare_parser(s, line, function_name, left))
  // {

  // }

}

/**
 * @brief 문자열과 라인 번호를 받아 해당 라인에 맞는 행위 실행
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void 
 */
void parser(char *s, int line, char *function_name) {
  int i=0;
  int first_parameter = 0;

  s += white_space(s);
  
  if(if_checker){
    if (strncmp(s, "산책가자", 12) == 0)
    {
      if_checker = 0;
      s+= 3*4;
      return ;
    }
    if(if_checker == 2)
      return ;
  }
  
  if (function_checker)
  {
    if (strncmp(s, "산책가자", 12) == 0)
    {
      function_checker = 0;
      s+= 3*4;
      return ;
    }
    append_code_function(s, line);
    return ;
  }

  if (loop_checker)
  {
    if (strncmp(s, "산책가자", 12) == 0) {
    char args[1][1024] = {
          "",
        };
      while (compare_parser(
      loop_condition_code,
      line,
      function_name,
      loop_condition_left_index
    )) {
      loop_checker = 0;
      call_function("loop_temp_func", args, line);
    }
      return;
    }
    
    append_code_function(s, line);
    return;

  }


  
  if (strncmp(s, "손", 3) == 0) {
      first_parameter  = get_parser(s, line, function_name); //index
      s += first_parameter + 3; //3을 더해주는 이유는 손[3Byte] + !개수
      if (s[0] == ' ')
        s++;
      first_parameter = INT_VARS[first_parameter];
  }
  
  if (strncmp(s, "멍", 3) == 0) {
      p_int_parser(s, line, function_name, first_parameter);
  }else if(strncmp(s, "망", 3) == 0) {
      m_int_parser(s, line, function_name, first_parameter);
  }
  else if (strncmp(s, "크게짖어", 12) == 0) {
      print_parser(s, line, first_parameter, "%c", function_name);
  }
  else if (strncmp(s, "작게짖어", 12) == 0) {
      print_parser(s, line, first_parameter, "%d", function_name);
  }
  else if (strncmp(s, "개집", 6) == 0){
    function_checker = 1;
    make_function(s, line, function_name); //parameter
  }
  else if(strncmp(s, "우쭈쭈", 9) == 0) {
    function_call(s, line, function_name);
  }
  else if (strncmp(s, "엄마가좋아아빠가좋아", 30) == 0) {
    if_checker = 1 + compare_parser(s, line, function_name, first_parameter);
  }
  else if (strncmp(s, "돌아", 2) == 0) {
    loop_parser(s, line, function_name);
    loop_checker = 1;
  }
  else {
      error("SyntaxError: invalid syntax", line, function_name);
  }



}

#endif
#ifndef PARSER_H
#include <stdio.h>

#include "global.h"

#include "error.h"

#include <string.h>

#define PARSER_H

/**
 * @brief int형 변수를 선언하는 코드
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void
 */
void int_parser(char *s, int line){
  int index = 0;
  int value = 0;
  while (strncmp(s, "멍", 1) == 0) {
    index += 1;
    s += 3; //한글 1글자는 3Byte
  }
  while (s[0] == '!')
  {
    value++;
    s++;
  }
  if(s[0] != NULL)
  {
    error("SyntaxError: invalid syntax", line);
  }
  #ifndef DEBUG
    printf("[int_parser][멍]INT_VARS[%d] = %d\n", index, value);
  #endif
  INT_VARS[index] = value;
}

/**
 * @brief int형 변수의 문자열 인덱스를 가져오는 코드
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void
 */
int get_parser(char *s, int line){
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
  
  error("SyntaxError: invalid syntax", line);

}

void conpare_parser(char *s, int line) {
  int white_spacer =0;
  while(s[white_spacer] != ' ') 
    white_spacer++;

  if(strncmp(s+white_spacer+1, "엄마가좋아아빠가좋아", 10)) {
       int num1=0,num2=0;
    if ((strncmp(s[0], "손", 1) == 0) && ((strncmp(s+white_spacer+16, "손", 1) == 0))) {
      
      int index = 1;
      while(s[index] == '!') {
        index++;
        num1++;
      }
      index = (white_spacer+17);
      while(s[index] == '!') {
        index++;
        num2++;
      }
    }
       if(strncmp(s+white_spacer+12, "엄마", 2) == 0) 
          if(num1 > num2)
            printf("1");
       else if(strncmp(s+white_spacer+12, "아빠", 2) == 0)
          if(num1 < num2)
            printf("-1");
       else if(strncmp(s+white_spacer+12, "둘다", 2) == 0)
          if(num1 == num2)
            printf("0");
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
void print_parser(char *s, int line, int value, char *format_string){

  
  
  int index = 0;
  s += 3*4; //한글 1글자는 3Byte
  if(s[0] != NULL)
  {

    error("SyntaxError: invalid syntax", line);
  }
  printf(format_string, value);
}

/**
 * @brief 문자열과 라인 번호를 받아 해당 라인에 맞는 행위 실행
 * @param[in] s 한 라인의 문자열
 * @param[in] line 해당 라인의 넘버 수
 * @return void 
 */
void parser(char *s, int line) {
  int i=0;
  int first_parameter = 0;

  while (s[0] == ' ') //공백제거
  {
    if (s[0] == ' ') 
      s++;
  }

  if (strncmp(s, "손", 1) == 0) {
      first_parameter  = get_parser(s, line); //index
      s += first_parameter + 3; //3을 더해주는 이유는 손[3Byte] + !개수
      if (s[0] == ' ')
        s++;
      first_parameter = INT_VARS[first_parameter];
  }

  printf("%d : %s\n", line, s);
  if (strncmp(s, "멍", 1) == 0) {
      int_parser(s, line);
  }
  else if (strncmp(s, "크게짖어", 1) == 0) {
      print_parser(s, line, first_parameter, "%c");
  }
  else if (strncmp(s, "작게짖어", 1) == 0) {
      print_parser(s, line, first_parameter, "%d");
  }
  else if (strncmp(s, "엄마가좋아아빠가좋아", 1) == 0) {
    //pass
  }
  else {
      error("SyntaxError: invalid syntax", line);
  }



}

#endif
#ifndef CMD_BARK_H
#include "global.h"
#define CMD_BARK_H

/**
 * @brief 정수형 변수 값을 설정
 * @param index 변수 index 위치
 * @param value 변수 값
 */
void set_int_var(uint index, int value) {
  INT_VARS[index] = value;
}

/**
 * @brief 정수형 변수 값 가져오기
 * @param index 변수 index 위치
 * @return index 위치에 있는 변수 값
 */
int get_int_var(uint index) {
  return INT_VARS[index];
}

#endif
cmake_minimum_required(VERSION 3.11)

add_definitions(--std=c++17)

project(
  DogSoundLang
  VERSION 0.1
  DESCRIPTION "개소리 프로그래밍 언어"
  LANGUAGES CXX
)

file(GLOB SRC
  "*/*.h"
  "*/*.cpp"
)

add_executable(
  DogLang
  ${SRC}
)

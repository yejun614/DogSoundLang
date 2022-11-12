#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <utility>

using namespace std;

#define DEBUG

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


int function_checker = 0;
int loop_checker = 0;
int if_checker = 0;
char loop_condition_code[REPL_CMD_LENGTH] = "";
int loop_condition_left_index = 0;


void error(char *error_msg, int line, char *error_function);
void define_function(char *function_name, uint param_count);
void call_function(char *function_name, char args[][1024], int line_number);

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


void parser(char *s, int line, char *function_name);

// 마지막으로 선언된 함수의 이름
string LAST_DEFINED_FUNCTION;

/**
 * @brief 함수 정의
 * @param function_name 함수 이름
 * @param param_count 파라미터 개수
 */
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

void run_script() {
  char ch, buffer[REPL_CMD_LENGTH] = "";
  size_t line = 0, length = 0;

  FILE *fp = fopen(SCRIPT_PATH, "r");

  if (fp == NULL) {
    printf("[run_script][%s] 스크립트 파일을 열 수 없습니다.\n", SCRIPT_PATH);
    exit(1);
  }

  while(true) {
    line++;
    length = 0;

    while (true) {
      ch = (char)fgetc(fp);
      if (ch == EOF || ch == '\n') break;

      buffer[length++] = ch;
    }

    if (ch == EOF) break;
    if (length == 0) continue;

    buffer[length] = '\0';
    parser(buffer, line, "Main");
  }

  fclose(fp);
}

/**
 * @brief 강아지 아스키 아트 출력
 */
void print_dog_ascii_art() {
  printf("\n      / \\__\n");
  printf("     (    @\\___\n");
  printf("    /         O   개소리 (DogSoundLang)\n");
  printf("   /   (_____/    version 0.1\n");
  printf("  /_____/   U\n\n");
}

/**
 * @brief REPL 모드
 * @return program exit code (0: 정상 종료)
 */
int repl() {
  print_dog_ascii_art();

  char inputCmd[REPL_CMD_LENGTH] = "";

  while (true) {
    // Prompt
    printf("%s", REPL_PROMPT);

    // 명령어 입력
    if (!scanf("%99[^\n]%*c", inputCmd)) {
      // 입력 없음
      break;
    }

    // 명령어 실행
    parser(inputCmd, 1, "Main");
  }

  return 0;
}

void error(char *error_msg, int line, char *error_function)
{
    printf("야 개짖는 소리좀 안나게해라\n");
    printf("%s, %d Line From %s\n", error_msg, line, error_function);
    exit(-1);
}

void arg_help() {
  printf(" -h               \t\t 사용법 출력\n");
  printf(" -v               \t\t 버전 출력\n");
  printf(" -s <SCRIPT_PATH> \t\t 스크립트 파일 위치\n");
  printf(" -E               \t\t C언어로 실행가능한 소스코드 출력");
}

char arg_parser(int argc, char** argv) {
  int running_mode = RUN_REPL;

  for (int arg = 0; arg < argc; arg++) {
    if (strcmp(argv[arg], "-h") == 0) {
      arg_help();
      running_mode = RUN_NOTHING;

    } else if (strcmp(argv[arg], "-v") == 0) {
      printf("개소리 (DogSoundLang)\n");
      printf("version 0.1\n");
      running_mode = RUN_NOTHING;

    } else if (strcmp(argv[arg], "-s") == 0) {
      strcpy(SCRIPT_PATH, argv[++arg]);
      running_mode = RUN_SCRIPT;
    } else if (strcmp(argv[arg], "-E") == 0) {
      running_mode = RUN_EMBED_C_SRC;
    }
  }

  return running_mode;
}

int main(int argc, char** argv) {

  int line = 0;

  parser("개집 출력(개껌)", ++line, "Main");
  parser("  멍개껌!", ++line, "Main");
  parser("  손! 크게짖어", ++line, "Main");
  parser("산책가자", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)", ++line, "Main");
  parser("우쭈쭈 출력(!!!!!!!!!!)", ++line, "Main");

  return 0;
}

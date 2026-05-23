#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../headers/htod.h"
#include "../headers/functions.h"
//#include "../headers/include.h"

#define SIZE 256
#define MGC 4
#define TIMESTAMP 4
#define MGC_NUM "03F30D0A" //magic number for python 2.7.10

struct function {
    int fn_body[SIZE];
    int ln_of_fn_body;
    int fn_name[SIZE];
    int ln_of_name;
    int local_constants[SIZE];
    int ln_local_constants;
    int cnt_local_variables;
    int local_vars_names[SIZE];
    int module_name[SIZE];
} fun[SIZE];

int *const_array = 0;
int *dup_const_array[SIZE];
int *name_array;
int instruction_array[SIZE * 8];
int lenofinstr = 0, k = 0, l = 0;
int struct_index = 0;
int no_of_funs = 0;
int i = 0, counter = 0, len_const_array = 0;
char *var_array[SIZE];
int no_fns;

//-----------------------------------------
// external functions
int push(int);
void start_interpreter(int *, int *);
int htod(char *);
unsigned char *next_byte(unsigned char *, char *opcode);
void print_var_array(int);
unsigned char *create_var_array(char *, int, unsigned char *, int);

int main(int argc, char **argv) {
  printf("Python Bytecode(.pyc) Interpreter\nfor Python Version: 2.7.10 on MSX-DOS2 Ver. 0.1a\n");
  char trace_mode = 0;
  char *input_file;
  if (argc >= 2 && strcmp(argv[1], "-t") == 0) {
    trace_mode = 1;
  }
  if((trace_mode && argc < 3) || (!trace_mode && argc <2)){
    printf("Need compiled python file as cmd line argument\nProgram exited\n");
    exit(0);
  } else {
    input_file = trace_mode ? argv[2]: argv[1];
    FILE *pyc_pointer;
    size_t file_size, read_size;
    unsigned char *buffer;
    unsigned char hex_str[SIZE * 8];
    unsigned char *hex_str_p = hex_str;
    char opcode[3], values[9], timestamp[9], values1[9], values2[9], values3[9], values4[9], values5[9], values6[9], magic[9];
    char magic_of_this_version[9];
    int i, j;
    int start = 0, counter_local_const = 0;
    int len, check = 0, counter = 0, opcode_seventyfour = 0, counter_mgc = MGC, counter_ts = TIMESTAMP;
    
    size_t buffer_size = 0;
    size_t capacity = 1024; // 初期容量
    int c;

    if(trace_mode){
      printf("[TRACE] input=%s\n", input_file);
    }
    
    //    pyc_pointer = fopen(argv[1], "rb");
    pyc_pointer = fopen(input_file, "rb");
    if (!pyc_pointer) {
      printf("Error, opening file %s\n", input_file);
      exit(0);
    }
    // 動的にバッファを拡張しながら読み込む
    buffer = (unsigned char *)malloc(capacity);
    if (!buffer) {
      printf("Memory allocation failed\n");
      fclose(pyc_pointer);
      exit(0);
    }
    
    // 1バイトずつ読み込んでEOFを正確に検出
    while ((c = fgetc(pyc_pointer)) != EOF) {
      if (buffer_size >= capacity) {
        capacity *= 2;
        unsigned char *new_buffer = (unsigned char *)realloc(buffer, capacity);
        if (!new_buffer) {
	  printf("Memory allocation failed\n");
	  free(buffer);
	  fclose(pyc_pointer);
	  exit(0);
        }
        buffer = new_buffer;
      }
      buffer[buffer_size++] = (unsigned char)c;
    }
    
    fclose(pyc_pointer);
    file_size = buffer_size; // 実際に読み込んだサイズ
    if(trace_mode){
      printf("[TRACE] file size=%d\n",file_size);
    }
    
    // バイナリ→16進数文字列に変換
    hex_str_p = hex_str;
    for (i = 0; i < file_size; i++) {
      hex_str_p += sprintf((char *)hex_str_p, "%02X", buffer[i]);
    }
    *hex_str_p = '\0'; // ヌル終端
    
    free(buffer);
    
    // 以降はhex_strを使って元の処理を続行
    hex_str_p = hex_str;
    printf("\n");
    
    //copying magic to magic and comparing magic to 2.7.10
    i = counter_mgc;
    while (i > 0) {
      hex_str_p = next_byte(hex_str_p, opcode);
      if (i == counter_mgc)
	strcpy(magic, opcode);
      else
	strcat(magic, opcode);
      i--;
    }
    if (strcmp(magic, MGC_NUM) != 0) {
      printf("\nThis pyvm only supports version 2.7.10\n");
      exit(0);
    }
    
    //copying timestamp of pyc file to timestamp
    i = counter_ts;
    while (i > 0) {
      hex_str_p = next_byte(hex_str_p, opcode);
      if (i == counter_ts)
	strcpy(timestamp, opcode);
      else
	strcat(timestamp, opcode);
      i--;
    }
    
    //skipping mainfun attributes;
    i = 16;
    while (i > 0) {
      hex_str_p = next_byte(hex_str_p, opcode);
      i--;
    }
    
    hex_str_p = next_byte(hex_str_p, opcode);
    hex_str_p = next_byte(hex_str_p, opcode);
#ifdef DEBUG
    printf("hex_str_p = %s\n", hex_str_p);
    printf("opcode = %s\n", opcode);
#endif
    // getting the length of main instructions
    if (strcmp(opcode, "73") == 0 && start == 0) {
      start = 1;
#ifdef DEBUG
      printf("opcode=0x73 = TYPE_STRING. get next 4 bytes = length of instructions.\n");
#endif
      //get next 4 bytes = length of instructions.
      for (i = 4; i > 0; i--) {
	hex_str_p = next_byte(hex_str_p, opcode);
	if (i == 4)
	  strcpy(values, strrvs(opcode));
	else
	  strcat(values, strrvs(opcode));
      }
      lenofinstr = htod(strrvs(values));
      if(trace_mode){
	printf("[TRACE] main instruction length=%d\n", lenofinstr);
      }
#ifdef DEBUG
      printf("opcode = %s\n", opcode);
#endif
    }
    
    i = 0;
    j = 0;
    while (*hex_str_p) {
      hex_str_p = next_byte(hex_str_p, opcode);
      if (i < lenofinstr) {
	instruction_array[i] = htod(opcode);
#ifdef DEBUG
	printf("opcode = %s\n", opcode);
#endif
	if (strcmp(opcode, "84") == 0) // checking if MAKE FUNCTION present in the instruction. 0x84
	  no_fns++;
	no_of_funs = no_fns;
      } else if (strcmp(opcode, "73") == 0 && no_fns > 0) { // start of functions
	int function_body = 0, local_vars = 0, module_name = 0, fn_name = 0, counter = 0, lenofargmnts = 0;
	for (j = 4; j > 0; j--) {
	  hex_str_p = next_byte(hex_str_p, opcode);
	  if (j == 4)
	    strcpy(values2, strrvs(opcode));
	  else
	    strcat(values2, strrvs(opcode));
	}
	int lenofarg = htod(strrvs(values2));
	fun[struct_index].ln_of_fn_body = lenofarg; // assigning len of fn body [ASSIGN]
	if (trace_mode) {
	  printf("[TRACE] function[%d] name_length=%d name_bytes=", struct_index, lenofargmnts);
	}
	counter = lenofarg;
	while (counter > 0) { // saving fun_body onto struct
	  hex_str_p = next_byte(hex_str_p, opcode);
	  fun[struct_index].fn_body[k] = htod(opcode); // assigning function body
	  
	  k++;
	  counter--;
	}
	hex_str_p = next_byte(hex_str_p, opcode);
	while (strcmp(opcode, "74") != 0) { // for local variables / skiping till local vars
	  if (strcmp(opcode, "69") == 0) { // looking for local constants.
	    // creating local constants.
	    for (l = 4; l > 0; l--) {
	      hex_str_p = next_byte(hex_str_p, opcode);
	      if (l == 4)
		strcpy(values6, strrvs(opcode));
	      else
		strcat(values6, strrvs(opcode));
	    }
	    fun[struct_index].local_constants[counter_local_const] = htod(strrvs(values6));
	    counter_local_const++;
	  }
	  fun[struct_index].ln_local_constants = counter_local_const;
	  hex_str_p = next_byte(hex_str_p, opcode);
	  
	}
	int total_no_of_local_vars = 0;
	while (strcmp(opcode, "74") == 0) { // now at 74
	  for (l = 4; l > 0; l--) {
	    hex_str_p = next_byte(hex_str_p, opcode);
	    if (l == 4)
	      strcpy(values3, strrvs(opcode));
	    else
	      strcat(values3, strrvs(opcode));
	  }
	  lenofargmnts = htod(strrvs(values3));
	  l = 0;
	  while (lenofargmnts > 0) {
	    hex_str_p = next_byte(hex_str_p, opcode);
	    fun[struct_index].local_vars_names[l] = htod(opcode); // stores decimal of hex of local variable name;
	    l++;
	    lenofargmnts--;
	  }
	  total_no_of_local_vars++;
	  hex_str_p = next_byte(hex_str_p, opcode);
	}
	fun[struct_index].cnt_local_variables = total_no_of_local_vars;
	hex_str_p = next_byte(hex_str_p, opcode);
	while (strcmp(opcode, "73") != 0) { // skip until 73
	  hex_str_p = next_byte(hex_str_p, opcode);
	}
	l = 0;
	for (l = 4; l > 0; l--) {
	  hex_str_p = next_byte(hex_str_p, opcode);
	  if (l == 4)
	    strcpy(values4, strrvs(opcode));
	  else
	    strcat(values4, strrvs(opcode));
	}
	lenofargmnts = 0;
	lenofargmnts = htod(strrvs(values4));
	l = 0;
	while (lenofargmnts > 0) {
	  hex_str_p = next_byte(hex_str_p, opcode);
	  fun[struct_index].module_name[l] = htod(opcode); // storing module name .
	  l++;
	  lenofargmnts--;
	}
	hex_str_p = next_byte(hex_str_p, opcode);
	while (strcmp(opcode, "74") != 0) {
	  hex_str_p = next_byte(hex_str_p, opcode); //skip until 74;
	}
	l = 0;
	for (l = 4; l > 0; l--) {
	  hex_str_p = next_byte(hex_str_p, opcode);
	  if (l == 4)
	    strcpy(values5, strrvs(opcode));
	  else
	    strcat(values5, strrvs(opcode));
	}
	lenofargmnts = 0;
	lenofargmnts = htod(strrvs(values5));
	fun[struct_index].ln_of_name = lenofargmnts; // assigning len of fun name [ASSIGN]
	l = 0;
	while (lenofargmnts > 0) {
	  hex_str_p = next_byte(hex_str_p, opcode);
	  fun[struct_index].fn_name[l] = htod(opcode); // storing fun name
	  if (trace_mode) {
	    printf("%02X", fun[struct_index].fn_name[l]);
	    if (lenofargmnts > 1) printf(" ");
	  }
	  l++;
	  lenofargmnts--;
	}
	if (trace_mode) {
	  printf("\n");
	}
	no_fns--;
	struct_index++;
      } else if (strcmp(opcode, "69") == 0) {
	for (j = 4; j > 0; j--) {
	  hex_str_p = next_byte(hex_str_p, opcode);
	  if (j == 4)
	    strcpy(values1, strrvs(opcode));
	  else
	    strcat(values1, strrvs(opcode));
	}
	len_const_array = push(htod(strrvs(values1)));
      }
      i++;
    }
#ifdef DEBUG	
    printf("hex_str=%p, *hex_str_p=%02X\n",(void*)hex_str_p, (unsigned)*hex_str_p);
#endif
    start_interpreter(instruction_array, const_array);
  }
  return 0;
}

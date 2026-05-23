#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "../headers/include.h"
#include "../headers/htod.h"





extern int len_const_array;
extern int *const_array;


int stack_size;
int *stack;



// push elements to end of array max size of constants array = SIZE.. // use dynamic allocation to make it dynamic

int push(int a){
   int  *ptr;
  static int i=0;
#ifdef DEBUG_s
  printf("push: i=%d, value=%d, const_array[%d]=%d\n", i, a, i, a);  
#endif
  if(i==0){
    const_array =(int*)malloc(sizeof(int) * 1);
    if(const_array == 0){

      perror("error in malloc: push()");
      exit(1);
    }
    *const_array = a;
  }
  else{

    ptr = (int*)realloc(const_array,sizeof(int) * (i+1));
    if(ptr ==0){

      perror("error in realloc: push()");
      free(const_array);
      exit(1);
    }
    const_array = ptr;
    *(ptr + i) =a;
  }

  
  i= i+1;
  return i;
    
}


//reverses the string and returns the reversed string
char* strrvs(char *str){
  
    static char buf[16];
    size_t len, i;
    
    len = strlen(str);  
    if(len >= sizeof(buf)){
      len = sizeof(buf) - 1;
    }
    for (i = 0; i < len; ++i){
      buf[i] = str[len - 1 - i];
    }
    buf[len] = '\0';
    return buf;
}

//-----FUNCTIONS FOR STACK MANIPULATION------------
//------------------------------------------------------------------------

//pops up the element on the top of stack /last element in array

int pop(){

  int top;
  int index = stack_size -1;
  top = stack[index];
  stack_size = stack_size -1;
  return top;
  
  
}



// implemeted dynamic stack allocation.
void push_stack(int a){
  static int l =0;
  int  *ptr;
#ifdef DEBUG_s 
  printf("in stack: value=%d,",a);
#endif
  if(l==0){
#ifdef DEBUG_s
    printf("l==0 ,");
    printf("call malloc ,");
#endif
    stack =(int*)malloc(sizeof(int) * 1);
    if(stack == 0){

      perror("error in malloc: push_stack()");
      exit(1);
    }
#ifdef DEBUG_s
    printf("stack != 0 ,");
#endif
    *stack = a;
#ifdef DEBUG_s
    printf("*stack = a\n");
#endif
  }
  else{
#ifdef DEBUG_s
    printf("l=%d ,",l);
    printf("ptr=%x ,",ptr);
    printf("call realloc ,");
#endif
    ptr = (int*)realloc(stack,sizeof(int) * (l+1));
#ifdef DEBUG_s
    printf("ptr=%x ,",ptr);
#endif
    if(ptr ==0){

      perror("error in realloc : push_stack()");
      free(stack);
      exit(1);
    }
    stack = ptr;
#ifdef DEBUG_s
    printf("ptr != 0 ,");
#endif
    *(ptr + l) =a;
#ifdef DEBUG_s
    printf("*(ptr + l) =a\n");
#endif
  }
 
  l= l+1;
  stack_size = l;
  
    
}

//-----------------END OF STACK FUNCTIONS------------------
//------------------------------------------------------------------------

  

// functions for creating instruction set

unsigned char *next_byte(unsigned char *hex_str_p, char *opcode){

     opcode[0] = *hex_str_p;
     opcode[1] = *(hex_str_p +1);
     opcode[2] = '\0';
#ifdef DEBUG
     printf("%s\t",opcode);
#endif
     hex_str_p++;
     hex_str_p++;
     return hex_str_p;
     
  
}



//--------------FUNCTIONS FOR BASIC ARITHMATIC OPS---------
//--------------------------------------------------------------------------------


//sum

int sum(int a, int b){
  //printf("the sum is %d\n",a+b);
  return a+b;
  
}

//subtract

int sub(int a, int b){

  return a-b;
}

//multiplication

int mul(int a, int b){

  return a*b;
}

// division

int divi(int a, int b){

  return a/b;
}

// modulus operation

int modulo(int a, int b){

  return a%b;
}


//--------------END OF BASCIC ARITHMETIC OPS------
//-------------------------------------------------------------------


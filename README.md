# python-bytecode-interpreter

A bytecode interpreter for v 2.7.10

The interpreter is purely coded in C. The interpreter takes the compiled python ".pyc" file as input. This file which contains the bytecode is converted into hex and each of the instructions are interperted accordingly. This interpreter has two C files, one of it converts the bytecode to corresponding hex and the other one does the interpreting.

This interpreter supports basic arithmetic operations addition, substraction, division, multiplication and modulo operation on integers.
It also supports basic looping with while and simple functions with no recursive calls.

##### Instructions and their hex values 
Some of the Instructions and their equivalent hex values as in Python v 2.7.10

     LOAD_CONST : 64
     BINARY_SUM : 17
     JUMP_WHILE : 71
     PRINT_ITEM : 47

### Installation and Usage:

```
     make -f makefile.z88dk

```
A pyvm executable is created which takes .pyc files as command line input.

```
    ./pyvm *.pyc
	
```

Running Testcodes on MSXDOS2:

```
    A>pyvm testcodes/fac.pyc
    A>pyvm testcodes/loop.pyc
	
```

Running Tracemode option on MSXDOS2:

```
    A>pyvm -t testcodes/fac.pyc
    A>pyvm -t testcodes/loop.pyc
	
```

"testfile.py" is a python script for testing, it tests for factorial, even, loop and lmn .pyc files. This test file should be run only after make.

-----
1st release on April 1, 2026.

<img width="858" height="666" alt="image" src="https://github.com/user-attachments/assets/d0574ec2-da87-4d51-9794-d733f1c88cbe" />

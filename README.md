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
     make

```
A pyvm executable is created which takes .pyc files as command line input.

```
    ./pyvm *.pyc
	
```

Running Testcodes:

```
    ./pyvm testcodes/fac.pyc
    ./pyvm testcodes/loop.pyc
	
```

"testfile.py" is a python script for testing, it tests for factorial, even, loop and lmn .pyc files. This test file should be run only after make.

-----
<img width="852" height="666" alt="image" src="https://github.com/user-attachments/assets/b3221e34-62ed-4b9e-b64b-2041842033ed" />
<img width="857" height="676" alt="image" src="https://github.com/user-attachments/assets/b33ba41d-1899-4268-8b35-fe076dfcf4af" />

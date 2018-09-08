# EasyBonsai

This program will make it easier to write code in bonsai-assembler (code will be translated back to bonsai), it supports features like "creating/naming" functions and value allocation without much effort.

(BonsaiAssembler: https://bonsai.pinyto.de/assembler/)

## Installing

* Windows:
  Donwload and Compile.
* Linux/Mac:
  Download and Compile if your compiler supports regex.

### Getting Started

This program can be easily ran through your console, it works like this:

```
EasyBonsai.exe (Name of Text/Source File) (Optional Parameters: --strict,--hidelinenumbers,--simple ,--output:File)
```

**Parameters**

* strict:
Translator will abort if invalid code was found
* hidelinenumbers:
Line numbers will not be shown, obviously
* simple:
Only output will be shown
* output:
Translated code will be saved in stated outputfile.

## Features

### Variable Allocation:
```
//You will need a value x(the value to copy from), value y(the value to copy to) and a helper value.
[x=y#helpervalue]
```
In this example x will be set to 0.
```
[x=zero]
```

### Function System
```
0: tst 0
1: jmp x //<-- This will later jump straight to line 3
2: hlt
3: x: inc 1
4: dec 0
5: jmp 0
```
### EasyJumps
Allows us to jump a set amount of lines forwards or backwards.
```
0: tst 0
1: jmp +2 //Will jump to line 3
2: jmp +3 // Will jump to line 5
3: dec 0
4: inc 1
5: [...]
6: jmp -6 //You can also jump backwards! This will jump to line 0
```


## Built With

* [termcolor](https://github.com/ikalnytskyi/termcolor) - For color.

## Authors

* **Me** 
* **Moquo** (Python Port)

## Implementations

* Python
This project was ported to python by Moquo (https://gitlab.com/Moquo/EasyBonsai.py)

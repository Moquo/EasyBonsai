# EasyBonsai

This program will make it easier to write code in bonsai-assemble (code will be translated back to bonsai), it supports features like "creating/naming" functions and value allocation without much effort.

(BonsaiAssembler: https://bonsai.pinyto.de/assembler/)

## Installing

Download and Compile :D (This project should work for linux too but I'm not too sure about that because of the cppconlib library)

### Getting Started

This program can be easily ran through your console, it works like this:

```
EasyBonsai.exe (Name of Text/Source File)
```
This will translate the code of your source file to bonsai assembler code.


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

### Function like System
```
0: tst 0
1: jmp x //<-- This will later jump straight to line 3
2: hlt
3: x: inc 1
4: dec 0
5: jmp 0
```
### "JmpSkipping"
(With this code its pretty self explanatory)
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

* [cppconlib](https://archive.codeplex.com/?p=cppconlib) - For better looking console outputs.

## Authors

* **Me** 

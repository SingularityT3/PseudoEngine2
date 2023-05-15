# PseudoEngine 2

A pseudocode interpreter designed to be compatible with the Cambridge IGCSE and A level pseudocode [syntax](./Syntax.md)

![PseudoEngine2](https://user-images.githubusercontent.com/44658109/220901152-5a794d58-de15-4028-91b0-fbebee033a70.png)

## The REPL
The REPL stands for Read Execute Print Loop and is an interface where pseudocode statements can be entered. The statement is immediately executed after it is entered and the result is printed to the console(if any)

## Usage
- Run the following commands in terminal/powershell in the directory of the executable.

  Windows:
  ```
  PseudoEngine2.exe [filename]
  ```
  Linux/Unix:
  ```
  ./PseudoEngine2 [filename]
  ```
  NOTE: `PseudoEngine2` would be replaced by the name of the executable which may be something like `Pseudoengine2-v0.5.exe` if it is downloaded from the releases

  Filename is an optional arguement. If it is provided the program in the corresponding file is run otherwise the REPL is launched.

- Alternatively, double click the executable file if supported by the OS to directly start the REPL. It is also possible to run files from the REPL using the command `RUNFILE <filename>`.

## Building
Make sure cmake and a c++ compiler is installed, then clone the repository and inside its folder run the following commands:
```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
The executable will be generated inside the build folder

# AppAnvil
A GUI for the Linux Security Module AppArmor

## Dependencies
### Compile Time
* cmake
* g++ (or another equivalent c++ compiler)
* libgtkmm-3.0-dev
### Optional
A GUI builder
* glade 

Linters and Static Analysis tools used by the Makefile
* cppcheck
* clang-tidy

## Compilation Instructions
To generate a makefile, and build the project using that makefile:
```
$ cmake .
$ make
```
To run Linters and extra static analysis on source code
```
$ make ANALYZE
```

## Run
To run the resulting binary:
```
$ ./dist/appanvil
```

# AppAnvil
A GUI for the Linux Security Module AppArmor

## Dependencies
### Compile Time
* cmake
* g++ (or another equivalent c++ compiler)
* libgtkmm-3.0-dev
* libjsoncpp-dev
* apparmor-utils

### Optional
A GUI builder
* glade 

Linters and Static Analysis tools used by the Makefile
* cppcheck
* clang-tidy

## Compilation Instructions

To modify pkexec's behavior when authenticating calls to some AppArmor utilities.
```
sudo cp ./resources/com.github.jack-ullery.AppAnvil.pkexec.policy /usr/share/polkit-1/actions/
```

To generate a makefile, and build the project using that makefile:
```
$ cmake .
$ make
```
To generate a makefile, build, and run extra static analysis on the source code:
```
$ cmake -DANALYZE=TRUE .
$ make
```

## Run
To run the resulting binary:
```
$ ./dist/appanvil
```

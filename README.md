# AppAnvil
A graphical user interface for the AppArmor access control system

## What is AppArmor
AppArmor is a Linux kernel extension that supplements the normal access control system. It can restrict specific processes from accessing files and other system resources. When properly configured, it can protect the system from internal or external threats. By restricting vulnerable processes, AppArmor mitigates the damage security vulnerabilities can cause.

AppArmor only activates after a program’s actions are authorized by the normal file-system. This ensures that permissions are granted only if the process had access to that resource in the first place.

To configure AppArmor, users create and apply profiles that specify which resources an application should have access to. By default, applications cannot access resources, unless a profile grants them permission. 

Attempted breaches of policy are recorded in the system log. AppArmor can be configured to send desktop notifications when such a violation occurs. 
AppArmor is shipped with every Ubuntu, Debian, and SUSE operating system. It is a powerful tool that gives knowledgeable users control over their operating system. If made more accessible to the public, it could enhance privacy and security for users of Linux distributions.

## The AppAnvil Project
By default, AppArmor is not easy to configure, running silently in the background. Currently, it is only accessible through the command-line, and requires some specialized knowledge to configure. 

The AppAnvil project aims to create an intuitive graphical interface for monitoring and configuring AppArmor. In particular, we want it to be easy to monitor and deploy profiles, change a profile’s permissions, and to parse system logs.

Currently, this project is in-progress and not feature complete. There is still some work to do before it is ready for a general audience.

## Dependencies
### Run Time
* GTKmm
* JsonCpp
* libapparmor
* [libappanvil](https://github.com/jack-ullery/libappanvil)
* apparmor-utils

#### Install commands (Ubuntu)
```
sudo apt install libgtkmm-3.0-dev libjsoncpp-dev libapparmor-dev apparmor-utils
```

#### Install commands (OpenSUSE)
```
sudo zypper in gtkmm3-devel jsoncpp-devel libapparmor-devel apparmor-utils
```

### Compile Time
* CMake
* PkgConfig
* clang (or another equivalent c++ compiler)
* Bison
* Flex

#### Install commands (Ubuntu)
```
sudo apt install pkg-config cmake clang bison flex libfl-dev
```

#### Install commands (OpenSUSE)
```
sudo zypper in cmake clang bison flex
```

### Optional
A GUI builder
* glade 

Linters and Static Analysis tools used by the Makefile
* cppcheck
* clang-tidy

Code beautifier/formatter
* clang-format

Unit Tests
* googletest
* googlemock

Code Coverage
* lcov

#### Install commands (Ubuntu)
```
sudo apt install glade cppcheck clang-tidy clang-format libgtest-dev libgmock-dev lcov
```

#### Install commands (OpenSUSE)
```
sudo zypper in glade cppcheck clang gtest gmock lcov
```

# Compilation Instructions
### Prebuild
To download the libappanvil dependency
```
git submodule update --init --recursive
```

Before you build the executable, you must first generate the makefile by running:
```
cmake .
```
Alternatively, if you want to run optional linters and static analysis checks:
```
cmake -DANALYZE=TRUE .
```

### Build
After the makefile is generated, you can build and install the executables:
```
sudo make install
```
This builds the necessary executables, installs them, and applies recommended changes to PolicyKit.

### Package

```
git clean -fdx
git submodule foreach --recursive git clean -fdx
debuild -us -uc
```

### Test
To build and run the unit tests:
```
make appanvil_test libappanvil_test -j$(nproc)
make test
```
To generate a code coverage report:
```
make report
```
Afterwards, you can then view the report by opening `./report/index.html` inside a web browser.

### Automatically Format Code
To format code, without building it:
```
make FORMAT
```
### Uninstall Instructions

To uninstall AppAnvil binaries and pkexec policy from the system:
```
sudo make uninstall
```

# Run
To run the resulting binary:
```
appanvil
```
To run the unit tests (need to be built first):
```
./dist/test
```

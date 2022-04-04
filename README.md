# AppAnvil
A graphical user interface for the Linux Security Module AppArmor

## What is AppArmor
AppArmor is a Linux kernel extension that supplements the normal access control system. It can restrict specific processes from accessing files and other system resources. When properly configured, it can protect the system from internal or external threats. By restricting vulnerable processes, AppArmor mitigates the damage security vulnerabilities can cause.

AppArmor only activates after a program’s actions are authorized by the normal file-system. This ensures that permissions are granted only if the process had access to that resource in the first place.

To configure AppArmor, users create and apply profiles that specify which resources an application should have access to. By default, applications cannot access resources, unless a profile grants them permission. 

Every attempted breach of policy is recorded in the system log. AppArmor can be configured to send desktop notifications when such a violation occurs. 
AppArmor is shipped with every Ubuntu, Debian, and SUSE operating system. It is a powerful tool that gives knowledgeable users control over their operating system. If made more accessible to the public, it could enhance privacy and security for users of Linux Operating Systems.

## The AppAnvil Project
By default, AppArmor doesn’t do very much, running silently in the background. Currently, it is only accessible through the command-line, and requires some specialized knowledge to configure. 

The AppAnvil project aims to create an intuitive graphical interface for monitoring and configuring AppArmor. In particular, we want it to be easy to monitor and deploy profiles, change a profile’s permissions, and to parse system logs.

# Dependencies
## Compile Time
* cmake
* g++ (or another equivalent c++ compiler)
* GTKmm (libgtkmm-3.0-dev)
* JsonCpp (libjsoncpp-dev)
* apparmor-utils

## Optional
A GUI builder
* glade 

Linters and Static Analysis tools used by the Makefile
* cppcheck
* clang-tidy

Code beautifier/formatter
* clang-format

Unit Tests
* googletest (libgtest-dev)
* googlemock (libgmock-dev)

# Compilation Instructions

To generate a makefile, build, and install the project using that makefile:
```
$ cmake .
$ sudo make install
```
To generate a makefile, build, install, and run extra static analysis on the source code:
```
$ cmake -DANALYZE=TRUE .
$ sudo make install
```
To format code, without building it:
```
$ make FORMAT
```
# Uninstall Instructions

To uninstall AppAnvil binaries and pkexec policy from the system:
```
$ sudo make uninstall
```

# Run
To run the resulting binary:
```
$ appanvil
```
To run unit tests (need googletest & googlemock installed):
```
$ ./dist/appanvil_test
```

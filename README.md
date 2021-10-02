# Database project 1 - Database management system

## Dependencies
1. gcc 4.8+ | clang 3.5+
2. cmake 3.11+

## To install dependencies

### For Linux
Open terminal and enter the following commands: <br/>
``` bash
sudo apt-get update && sudo apt-get install cmake && sudo apt-get install g++
```
Cmake and g++ will then be installed. <br/><br/>

To check that cmake and gcc is installed: <br/>
`gcc --version` <br/>
`cmake --version` <br/>

You should be able to see the version that u have downloaded

### For Mac
Open terminal from spotlight and enter the following commands: <br/>
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" && brew install gcc && brew install cmake
```
Cmake and g++ will then be installed. <br/><br/>

To check that cmake and gcc is installed: <br/>
`gcc --version` <br/>
`cmake --version` <br/>

You should be able to see the version that u have downloaded

## Build project for Linux and Mac

``` bash
cmake -H. -Bbuild
cd build && make
```

## Run project
```
./build/bin/dbms
```

Run test:
```
./build/bin/dbms_test
```

## To install dependencies and build project on Windows
Before building the project, please change line 83 of the main.cpp file to the absolute path of the data folder. (This is because in windows, the parent dir of executable file might be in different folder due to the difference in ide) <br/>

Install a IDE of your choice and follow their respective guide for cmake and build the project:
### Clion
install: https://www.jetbrains.com/clion/ <br/>
guide: https://www.jetbrains.com/help/clion/creating-new-project-from-scratch.html

### Visual studio code
install: https://code.visualstudio.com/ <br/>
guide: https://computingonplains.wordpress.com/building-c-applications-with-cmake-and-visual-studio-code/

### Visual studio: 
install: https://visualstudio.microsoft.com/downloads/ <br/>
guide: https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-160





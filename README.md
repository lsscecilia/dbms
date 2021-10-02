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
Please refer to this guide:
https://www.stereolabs.com/docs/app-development/cpp/windows/#building-on-windows
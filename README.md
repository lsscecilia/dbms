# Database project 1 - Database management system

## Dependencies
1. gcc 4.8+ | clang 3.5+
2. cmake 3.11+
3. gtest

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
Please ensure that you have a active wifi connection as gtest module will be fetch automatically

``` bash
cmake -H. -Bbuild
cd build && make
```

## Run project
In the dbms directory:

```
./build/bin/dbms
```

Run test:
```
./build/bin/dbms_test
```

## To install dependencies and build project on Windows
Before building the project, please change line 83 of the main.cpp file to the absolute path of the data folder. (This is because in windows, the parent dir of executable file might be in different folder due to the difference in ide) <br/>

Ensure the following dependencies are installed:

* [CMake](https://cmake.org/download/)
* [MSYS2](https://www.msys2.org/) (After downloading the MSYS2 installer, follow all the steps in the website to install mingw64 and related dependencies.)

Perform the following steps:

1. Open the source code in [VSCode](https://code.visualstudio.com/).
1. Install the CMake tools extension in VSCode: [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools).
1. Set the CMake Tools environment variables:
    1. You need to open the `settings.json` by Ctrl + Shift + P and search-select `Preference: Open settings (JSON)`
    1. Add the lines below:
    ```
    "cmake.cmakePath": "C:\\msys64\\mingw64\\bin\\cmake.exe",
    "cmake.mingwSearchDirs": [
      "C:\\msys64\\mingw64\\bin"
   ],
   "cmake.generator": "MinGW Makefiles"
    ```
1. Click on the wrench icon located at the bottom bar and select `GCC 10.3.0 x86_64-w64-mingw32`, which is the latest version as of 2/10/2021.
1. Click on the `Build` button with a gear icon, located at the bottom bar of the VSCode window.
1. Once done, navigate to the executable in build/bin/dbms.exe and run the dbms.exe executable to start. The project will open in a command shell

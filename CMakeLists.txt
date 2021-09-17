cmake_minimum_required(VERSION 3.9)

#set project name
project(dbms VERSION "1.0")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE src/main.cpp)
set(CONFIGURE_FILE src/config.h.in)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "-g") 

#set executable
add_executable(${PROJECT_NAME} ${SOURCE}) 

include_directories( ${CMAKE_BINARY_DIR}/bin/ ) # Make sure it can be included...


#add lib
add_library(bplustree
  src/bplustree.cpp)

add_library(storage 
  src/storage.cpp
  src/record.cpp
  src/block.cpp)

target_link_libraries(
  dbms
  bplustree
  storage
)

# add_library(lib2
# src/lib2.cpp)

# #link lib
# target_link_libraries(
#     lib1
#     lib2
# )

# for source file to run, need to either add it in libaries or add it under line 9 (set(SOURCE src/main.cpp {file}.cpp))


#google test
include(FetchContent)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(main_project ON)
endif ()
option(project_build_tests "Build unit tests" ${main_project})

if (project_build_tests)
  find_package(GTest 1.10.0 QUIET)
  if (NOT GTest_FOUND)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG release-1.10.0)

    FetchContent_GetProperties(googletest)
    if (NOT googletest_POPULATED)
      FetchContent_Populate(googletest)
      add_subdirectory(
        ${googletest_SOURCE_DIR}
        ${googletest_BINARY_DIR}
        EXCLUDE_FROM_ALL)
      add_library(GTest::Main ALIAS gtest_main)
    endif ()
  endif ()
endif ()

if (project_build_tests)
  add_executable(dbms_test
    test/main_test.cpp)

  target_link_libraries(dbms_test
    GTest::Main
    bplustree
    storage)

  target_compile_definitions(dbms_test
    PRIVATE TEST_DATA="${PROJECT_SOURCE_DIR}/test/data/")
endif ()
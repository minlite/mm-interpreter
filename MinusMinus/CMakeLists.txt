cmake_minimum_required(VERSION 3.6)
project(MinusMinus)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Interpret.cpp
    Interpret.h
    main.cpp
    Stack.cpp
    Stack.h
    SymbolTable.cpp
    SymbolTable.h)

add_executable(mm ${SOURCE_FILES})
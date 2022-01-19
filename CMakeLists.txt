cmake_minimum_required(VERSION 3.10)
project(LSMTree)

set(CMAKE_CXX_STANDARD 14)

add_executable(LSMTree main.cpp src/Buffer.h src/Buffer.cpp src/types.h src/Run.h src/Run.cpp src/Level.h src/LSMTree.h src/LSMTree.cpp src/Merge.h src/Merge.cpp)

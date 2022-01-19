cmake_minimum_required(VERSION 3.10)
project(BloomFilterTest)

set(CMAKE_CXX_STANDARD 14)

add_executable(

        BloomFilterTest BloomFilterTest.cpp
        ../../src/MurmurHash3.h ../../src/MurmurHash3.cpp
        ../../src/BloomFilter.h ../../src/BloomFilter.cpp

        )
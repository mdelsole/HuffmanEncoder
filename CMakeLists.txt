cmake_minimum_required(VERSION 3.9)
project(COP3530_S19_Huffman_Algorithm_master)

set(CMAKE_CXX_STANDARD 11)

add_executable(COP3530_S19_Huffman_Algorithm_master
        catch.hpp
        catch_main.cpp
        huffman_tree.cpp
        huffman_tree.h
        tests.cpp)

cmake_minimum_required(VERSION 3.31)
project(CourseworkNew)

set(CMAKE_CXX_STANDARD 20)

add_executable(CourseworkNew main.cpp
        config.h
        EncodingKey.hpp
        EncodingKey.cpp
        DecodingKey.hpp
        DecodingKey.cpp
        ArrayList.hpp
        LinkedList.hpp
        HashMap.hpp
        fileHandling.hpp
        fileHandling.cpp
        encodingHandling.hpp
        encodingHandling.cpp
        consoleHandling.cpp
        consoleHandling.hpp
)

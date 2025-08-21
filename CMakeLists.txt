cmake_minimum_required(VERSION 3.31)
project(CourseworkNew)

set(CMAKE_CXX_STANDARD 20)

add_executable(CourseworkNew main.cpp
        ConsoleHelper.h
        CodeHelper.h
        FileHelper.h
        config.h
        EncryptionKey.h
        exceptions.h
)

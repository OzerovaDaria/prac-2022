cmake_minimum_required(VERSION 3.17)
project(Salnikov_task)

set(CMAKE_CXX_STANDARD 20)

add_executable(Salnikov_task
        Numbers/long_number.h
        Numbers/long_number.cpp
        Numbers/Rational.h
        Numbers/Rational.cpp
        Numbers/ComplexNumber.h
        Numbers/ComplexNumber.cpp
        parsing/parsing.h
        parsing/parsing.cpp
        Matrix/Vector.h
        Matrix/Vector.cpp
        Matrix/Matrix.h
        Matrix/Matrix.cpp
        main.cpp)

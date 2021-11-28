cmake_minimum_required(VERSION 3.20)
project(Semestralka C)

set(CMAKE_C_STANDARD 90)

add_executable(Semestralka main.c pgm.c pgm.h ccl.c ccl.h disj_set.c disj_set.h)

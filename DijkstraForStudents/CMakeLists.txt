cmake_minimum_required(VERSION 3.17)
project(DijkstraForStudents C)

set(CMAKE_C_STANDARD 99)

add_executable(DijkstraForStudents dijkstraClient.c dijkstra.c dijkstra.h)
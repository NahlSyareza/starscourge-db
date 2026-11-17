cmake_minimum_required(VERSION 3.10)
project(starscourge-db C)

include_directories(./)

add_executable(main main.c glintstone.c)

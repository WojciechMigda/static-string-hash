project( "static-string-hash" CXX )

cmake_minimum_required( VERSION 2.8.3 )

add_definitions( -O2 -ggdb -std=c++0x -Wall -pedantic )

add_executable( main main.cpp )

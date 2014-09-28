#!/bin/bash
make
./A2com < tests/test1 > testoutput/test1.out
./A2com < tests/test2 > testoutput/test2.out
./A2com < tests/test3 > testoutput/test3.out
./A2com < tests/test4 > testoutput/test4.out
./A2com < tests/test5 > testoutput/test5.out
./A2com < tests/test6 > testoutput/test6.out
./A2com < tests/testspaces > testoutput/testspaces.out

diff testoutput/test1.out testoutput/test1 

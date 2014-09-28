#!/bin/bash
make

testcases=( test1 test2 test3 test4 test5 test6 testspaces )

for test in "${testcases[@]}"
do
	./A2com < tests/$test > testoutput/$test.out
	
	echo "Diff $test"
	diff testoutput/$test.out testoutput/$test
done

#!/bin/bash
make

testcases=( test1 test2 test3 test4 test5 test6 testspaces )

for test in "${testcases[@]}"
do
	./A2com < tests/$test | sed 's/\(.*\)[A-Z][a-z]\{2\}.*[0-9]\{2\}:[0-9]\{2\}/\1Sep 28 12:34/g' > testoutput/$test.out
	
	echo "Diff $test"
	diff testoutput/$test.out testoutput/$test
done

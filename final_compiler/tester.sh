#!/bin/bash

make
compiled=$?
if [[ $compiled != 0 ]]; then
	echo "does not compile"
	exit 1
fi

echo "Compiles"

echo -n "Basic Test : "
./a.out test.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
	exit 1
else
	diff -w -B output.txt bout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
		exit 1
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi


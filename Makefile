all:
	g++ -o llrb-test llrb-test.cpp
	./llrb-test < test.in > test.out
	dot test.out | gvpr -c -ftree.g | neato -n -Tpng -o tree.png

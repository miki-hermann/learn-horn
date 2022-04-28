.PHONY: compile offline online horn-closure install clean scratch example

compile: offline online horn-closure example

offline: matrix+formula.o common.o offline.o
	g++ -O4 -o offline offline.o common.o matrix+formula.o

offline.o: matrix+formula.hpp common.hpp offline.cpp
	g++ -O4 -c -o offline.o offline.cpp

online: matrix+formula.o common.o online.o
	g++ -O4 -o online online.o common.o matrix+formula.o

online.o: matrix+formula.hpp common.hpp online.cpp
	g++ -O4 -c -o online.o online.cpp

horn-closure: matrix+formula.o common.o horn-closure.o
	g++ -O4 -o horn-closure-v1 horn-closure-v1.o common.o matrix+formula.o
	g++ -O4 -o horn-closure-v2 horn-closure-v2.o common.o matrix+formula.o

horn-closure.o: matrix+formula.hpp common.hpp horn-closure-v1.cpp  horn-closure-v2.cpp
	g++ -O4 -c -o horn-closure-v1.o horn-closure-v1.cpp
	g++ -O4 -c -o horn-closure-v2.o horn-closure-v2.cpp

common.o: common.hpp common.cpp
	g++ -O4 -c -o common.o common.cpp

matrix+formula.o: matrix+formula.hpp matrix+formula.cpp
	g++ -O4 -c -o matrix+formula.o matrix+formula.cpp

example: digit.cpp digit.csv.xz
	g++ -O4 -o digit digit.cpp

clean:
	rm -f *.o
	rm -f *~

scratch: clean
	rm -f offline online
	rm -f horn-closure-v1 horn-closure-v2
	rm -f digit digit[0-9].csv

install:
	sudo mkdir -p /usr/local/bin
	sudo cp -f offline online /usr/local/bin
	sudo cp -f horn-closure /usr/localbin
	sudo cp -f digit /usr/local/bin

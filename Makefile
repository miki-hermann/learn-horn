.PHONY: compile offline online horn-closure install clean scratch

compile: offline online horn-closure

offline: matrix+formula.o common.o offline.o
	g++ -O4 -o offline offline.o common.o matrix+formula.o

offline.o: matrix+formula.hpp common.hpp offline.cpp
	g++ -O4 -c -o offline.o offline.cpp

online: matrix+formula.o common.o online.o
	g++ -O4 -o online online.o common.o matrix+formula.o

online.o: matrix+formula.hpp common.hpp online.cpp
	g++ -O4 -c -o online.o online.cpp

horn-closure: matrix+formula.o common.o horn-closure.o
	g++ -O4 -o horn-closure horn-closure.o common.o matrix+formula.o

horn-closure.o: matrix+formula.hpp common.hpp horn-closure.cpp
	g++ -O4 -c -o horn-closure.o horn-closure.cpp

common.o: common.hpp common.cpp
	g++ -O4 -c -o common.o common.cpp

matrix+formula.o: matrix+formula.hpp matrix+formula.cpp
	g++ -O4 -c -o matrix+formula.o matrix+formula.cpp

clean:
	rm -f *.o
	rm -f *~

scratch: clean
	rm -f offline online hc-miki horn-closure
	rm -f train[0-9].csv

install:
	sudo mkdir -p /usr/local/bin
	sudo cp -f offline online /usr/local/bin

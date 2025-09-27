.PHONY: compile offline online \
	install clean scratch zip

# FLAGS := -O4 -march=native -flto -fopenmp -Wfatal-errors
FLAGS := -O4 -march=native -flto -Wfatal-errors
# CXX := g++
EXECUTABLES := /usr/local/bin
SUDO := sudo

compile: offline online

common.o: common.cpp common.hpp
	$(CXX) $(FLAGS) -c -o common.o common.cpp

offline.o: common.hpp offline.cpp
	$(CXX) $(FLAGS) -c -o offline.o offline.cpp

offline: common.o offline.o
	$(CXX) $(FLAGS) -o offline offline.o common.o

online.o: common.hpp online.cpp
	$(CXX) $(FLAGS) -c -o online.o online.cpp

online: common.o online.o
	$(CXX) $(FLAGS) -o online online.o common.o

clean:
	rm -f *.o
	rm -f *~

scratch: clean
	rm -f offline online
	rm -f *.zip

install:
	$(SUDO) mkdir -p $(EXECUTABLES)
	-$(SUDO) cp -f online $(EXECUTABLES)/offline-horn
	-$(SUDO) cp -f online $(EXECUTABLES)/online-horn

zip: common.cpp common.hpp offline.cpp online.cpp \
	*.csv test.sh README.md Makefile check-horn
	zip -r learn-horn.zip *.cpp *.hpp *.csv test.sh README.md Makefile check-horn

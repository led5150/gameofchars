# By  : Matt Champlin
# Makefile for gameofchars
#


CXX      = clang++
CXXFLAGS = -Wall -Wextra -std=c++11


gameofchars: gameofchars.cpp universe.cpp Organism.cpp termfuncs.cpp
		${CXX} ${CXXFLAGS} -o $@ $^


%.o: %.cpp $(shell echo *.h)
	${CXX} ${CXXFLAGS} -c $<


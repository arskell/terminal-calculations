all:
	g++ -Wall main.cpp parser.h parser.cpp parserExceptions.cpp parserExceptions.h
debug:
	g++ -Wall -g -DDEBUG main.cpp parser.h parser.cpp parserExceptions.cpp parserExceptions.h 
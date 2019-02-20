all:
	g++ main.cpp parser.h parser.cpp parserExceptions.cpp parserExceptions.h
debug:
	g++ -g -DDEBUG main.cpp parser.h parser.cpp parserExceptions.cpp parserExceptions.h 
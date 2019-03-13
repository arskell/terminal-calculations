all:
	g++ -Wall main.cpp fmt_def.h parser.h parser.cpp parserExceptions.cpp parserExceptions.h user_functions/functions.hpp user_functions/functions.cpp 
debug:
	g++ -Wall -g -DDEBUG main.cpp fmt_def.h parser.h parser.cpp parserExceptions.cpp parserExceptions.h user_functions/functions.hpp user_functions/functions.cpp 
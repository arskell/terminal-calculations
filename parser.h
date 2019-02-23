
#ifndef PARSER_H

#define PARSER_H


#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <map>
#include <list>
#include "parserExceptions.h"

#define OP_COUNT 4

namespace calc_prs{
	typedef double numeric_fmt;
	
	class parser{
	public:
		parser();
		~parser();
		void operator<<(std::string s);
		void erase_buf();
		std::string get_result();
		void process_buf();
	private:
		std::string buf;
		std::string workspace;
		std::map<std::string, numeric_fmt> __namespace;
		
		void __validation_checker(std::string &data);
		inline bool __s_check(const char c);
		
		void prc(std::string &data); //process data inside high priority parentheses
		std::pair<size_t, std::pair<size_t,size_t>> find_most_priority_exp(std::string &data);
		std::pair<size_t, size_t> find_most_priority_parentheses(std::string &data);
		numeric_fmt solve_expression(numeric_fmt &f, numeric_fmt &s, char oper);
		size_t near_operators(std::string &data, size_t mpos, bool forward = true);
		bool have_oper(std::string &data,const char* operator_list);
		//size_t test_add(size_t a, size_t b){return a+b;}
		std::list<std::pair<size_t, size_t>> get_variables_borders(std::string &data);
		std::pair<size_t, size_t> get_var_borders(std::string &data, size_t pos = 0);
		inline bool is_oper(const char c);
		inline bool is_name_char(const char c);
		const char operators[2][2] = {{'*','/'}, {'+','-'}};
		const char oper_list[4] = {'*','/','+','-'};
	
	};

}


#endif
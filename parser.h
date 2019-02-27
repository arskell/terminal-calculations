
#ifndef PARSER_H

#define PARSER_H


#include <string>
#include <utility>
#include <algorithm>
#include <map>
#include <list>
#include "parserExceptions.h"

#ifdef DEBUG
#include <iostream>
#endif

#define OP_COUNT 4
#define NUMERIC_FMT_TO_STRING_FUNCTION(str) atof(str)

namespace calc_prs{
	typedef double numeric_fmt;
	
	enum koid{                      /*kind of input data       */
			EXPRESSION,
			FUNCTION
		};
	
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
		void __prevalidation_valid_checker(std::string& data);
		inline bool __s_check(const char c);
		
		koid get_koid(std::string &data);
		
		std::string solve_input_data(std::string data);
		std::string dereference_all_vars(std::string data);
		void prc(std::string &data); //process data inside high priority parentheses
		std::pair<size_t, std::pair<size_t,size_t>> find_most_priority_exp(std::string &data);
		std::pair<size_t, size_t> find_most_priority_parentheses(std::string &data);
		numeric_fmt solve_bi_expression(numeric_fmt &f, numeric_fmt &s, char oper);
		size_t near_operators(std::string &data, size_t mpos, bool forward = true);
		bool have_oper(std::string &data,const char* operator_list);
		std::list<std::pair<size_t, size_t>> get_variables_borders(std::string &data);
		std::pair<size_t, size_t> get_var_borders(std::string &data, size_t pos = 0);
		inline bool is_oper(const char c);
		inline bool is_name_char(const char c);
		const char operators[2][2] = {{'*','/'}, {'+','-'}};
		const char oper_list[4] = {'*','/','+','-'};
	
	};
	
}


#endif

#ifndef PARSER_H

#define PARSER_H


#include <string>
#include <utility>
#include <algorithm>
#include <vector>
#include <map>
#include <list>

#include <math.h>
#include "parserExceptions.h"

#ifdef DEBUG
#include <iostream>
#endif

#define OP_COUNT 5+1 	/* must be an even number    */
#define NUMERIC_FMT_TO_STRING_FUNCTION(str) atof(str)

namespace calc_prs{
	typedef double numeric_fmt;
	
	enum koid{                      /*kind of input data       */
			EXPRESSION,
			FUNCTION_DEF
		};
	
	class parser{
	public:
		parser();
		~parser();
		void operator<<(std::string s);
		void erase_buf();
		std::string get_result();
		void process_buf();
		size_t length();
	private:
		struct function{
			std::vector<std::pair<std::string, numeric_fmt>> local_namespace;
			std::string expression;
		};
		
		std::string buf;
		std::string workspace;
		std::map<std::string, numeric_fmt> __namespace;
		std::map<std::string, function> __funcspace;
		
		void __validation_checker(std::string &data);
		void __prevalidation_valid_checker(std::string& data);
		inline bool __s_check(const char c);
		
		std::vector<std::pair<std::string, numeric_fmt>>::iterator find(std::vector<std::pair<std::string, numeric_fmt>>& vctr, std::string& wrd);
		koid get_koid(std::string &data);
		std::pair<size_t, size_t> get_function_borders(std::string&data);   /* need to call after calling the 'dereference_all_vars' ! 	*/
		std::pair<size_t, size_t> get_char_word(std::string &data, size_t pos);
		std::string solve_input_data(std::string data);
		std::string dereference_all_vars(std::string data, std::map<std::string, numeric_fmt> &nmspace, bool excep_throw = true);
		std::string dereference_all_vars(std::string data, std::vector<std::pair<std::string, numeric_fmt>> &nmspace, bool excep_throw = true);
		void prc(std::string &data); 									/*process the data inside high priority parentheses			*/
		std::pair<size_t, std::pair<size_t,size_t>> find_most_priority_exp(std::string &data);
		std::pair<size_t, size_t> find_most_priority_parentheses(std::string &data);
		numeric_fmt solve_bi_expression(numeric_fmt &f, numeric_fmt &s, char oper);
		size_t near_operators(std::string &data, size_t mpos, bool forward = true);
		bool have_oper(std::string &data,const char* operator_list);
		std::list<std::pair<size_t, size_t>> get_variables_borders(std::string &data);
		std::pair<size_t, size_t> get_var_borders(std::string &data, size_t pos = 0);
		inline bool is_oper(const char c);
		inline bool is_name_char(const char c);
		const char operators[3][2] = {{'^', '\0'}, {'*','/'}, {'+','-'}};
		const char oper_list[5] = {'^','*','/','+','-'};
		
	};
	
}


#endif
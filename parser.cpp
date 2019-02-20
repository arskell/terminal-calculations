#include"parser.h"

calc_prs::parser::parser(){
}

calc_prs::parser::~parser(){
	erase_buf();
}

void calc_prs::parser::operator<<(std::string s){
	buf += s;
}

void calc_prs::parser::erase_buf(){
	buf.erase();
}

void calc_prs::parser::process_buf(){
	prc(buf);
}

void calc_prs::parser::prc(std::string &data){
	numeric_fmt a,b;
	std::string b_str, l_str, r_str;
	while(have_oper(data, oper_list)){
		auto exp = find_most_priority_exp(data);
		a = atof(data.substr((exp.second.first==0)?exp.second.first:(exp.second.first + 1), exp.first - exp.second.first).c_str());
		b = atof(data.substr(exp.first + 1,exp.second.second- exp.first).c_str());
		#ifdef DEBUG
		std::cout<<exp.first<< " <"<< exp.second.first <<"," << exp.second.second<<">"<<std::endl;
		std::cout<<a<<std::endl;
		std::cout<<b<<std::endl;
		#endif
		b_str = std::to_string(solve_expression(a, b, data[exp.first]));
		if(exp.second.first != 0){
			l_str = data.substr(0, exp.second.first+1);//&
		}else{
			l_str = "";
		}
		if(exp.second.second != data.length()){
			r_str = data.substr(exp.second.second, data.length() - exp.second.second);
		}else{
			r_str = "";
		}
		data = l_str + b_str + r_str;
		#ifdef DEBUG
		std::cout<<data<<std::endl;
		std::cout<<std::endl;
		#endif
	}
}

std::pair<size_t, std::pair<size_t,size_t>> calc_prs::parser::find_most_priority_exp(std::string &data){
	std::pair<size_t, size_t> pr;
	size_t b, a, res;
	for(int i = 0; i < OP_COUNT/2; i++){ // OP_COUNT/2 pairs of operators 
		b = data.find(operators[i][0]);
		a = data.find(operators[i][1]);
		if(a != b){ // a OR b != npos
			res = (a>b)?b:a;
			if(res != std::string::npos){
				pr.first = near_operators(data, res - 1, false);
				pr.second = near_operators(data, res + 1, true);
				return {res, pr};
			}
		}
	}
	throw p_excep("std::pair<size_t, size_t> calc_prs::parser::find_most_priority_exp(std::string &data)");
}

size_t calc_prs::parser::near_operators(std::string &data, size_t mpos, bool forward){
	auto sz = data.length();
	for(int i = mpos; i != (forward?data.length():0); i+=(forward?1:(-1))){
		for(int j = 0; j < OP_COUNT; j++){
			if(data[i] == oper_list[j]){
				return i;
			}
		}
	}
	return forward?data.length():0;
	//throw p_excep("size_t calc_prs::parser::near_operators(std::string &data, size_t mpos, bool forward)");
}

calc_prs::numeric_fmt calc_prs::parser::solve_expression(calc_prs::numeric_fmt f, calc_prs::numeric_fmt s, char oper){
	switch (oper){
		case '*':
		return f * s;
		case '/':
		return f / s;
		case '+':
		return f + s;
		case '-':
		return f - s;
	}
	return -1;
}

bool calc_prs::parser::have_oper(std::string &data,const char* operator_list){
	for(size_t t = 0; t < OP_COUNT; t++){
		if (data.find(operator_list[t]) != std::string::npos) return true;
	}
	return false;
}

std::string calc_prs::parser::get_result(){
	return buf;
}


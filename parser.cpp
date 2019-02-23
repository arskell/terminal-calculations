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
	workspace.erase();
}

void calc_prs::parser::process_buf(){
	//TODO: valid checker
	auto count_open_p = std::count(buf.begin(), buf.end(), '(');
	auto count_close_p = std::count(buf.begin(),buf.end(), ')');
	if(count_close_p != count_open_p){
		throw fmt_error("Parenthesis is missing", (count_close_p > count_open_p)?0:buf.length());
	}
	std::string b_data;
	while(true){
		auto p = find_most_priority_parentheses(buf);
		if( (p.first == 0) && ( (p.second == buf.length()) || (p.second == buf.length() - 1))){
			prc(buf);
			return;
		}
		b_data = buf.substr(p.first + 1, p.second - p.first - 1);
		prc(b_data);
		buf =  buf.substr(0, p.first) + b_data +  buf.substr(p.second + 1, buf.length() - p.second);
	}
}

std::pair<size_t, size_t> calc_prs::parser::find_most_priority_parentheses(std::string &data){
	size_t op, cl;
	op = data.rfind('(');
	cl = data.find(')', op);
	if((op == std::string::npos) && (cl == std::string::npos)){
		return {0, data.length()};
	}
	if( (op == std::string::npos) ^ (cl == std::string::npos)){
		throw fmt_error("Parenthesis is missing", (cl==std::string::npos)?data.length():0);
	}
	return {op, cl};
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
				pr.first = near_operators(data, (res==0)?0:(res - 1), false);
				pr.second = near_operators(data, res + 1, true);
				if( (res == 0) && ( (data[res]=='-') || (data[res] == '+') )){
					res = near_operators(data, res + 1, true);
					pr.second = near_operators(data, pr.second + 1, true);
				}
				if( (res + 1) == pr.second ){
					pr.second = near_operators(data, res + 2, true);
				}
				return {res, pr};
			}
		}
	}
	throw p_excep("std::pair<size_t, size_t> calc_prs::parser::find_most_priority_exp(std::string &data)");
}

bool calc_prs::parser::is_oper(const char& c){
	for(size_t i(0); i < OP_COUNT; i++){
		if(c == oper_list[i]) return true;
	}
	return false;
}

size_t calc_prs::parser::near_operators(std::string &data, size_t mpos, bool forward){
	for(size_t i = mpos; i != (forward?data.length():0); i+=(forward?1:(-1))){
		if(is_oper(data[i])) return i;
	}
	return forward?data.length():0;
}

calc_prs::numeric_fmt calc_prs::parser::solve_expression(calc_prs::numeric_fmt& f, calc_prs::numeric_fmt& s, char oper){
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
	throw p_excep("ERROR: undefined operator \'" + std::string((char*)&oper) + "\'");
}

bool calc_prs::parser::have_oper(std::string &data,const char* operator_list){
	size_t opr;
	for(size_t t = 0; t < OP_COUNT; t++){
		opr = data.rfind(operator_list[t]);
		if (opr != std::string::npos){
			if((opr == 0) && (data[opr] == '-')){
				return false;
			}else{
				return true;
			}
		}
	}
	return false;
}

std::string calc_prs::parser::get_result(){
	return buf;
}


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
	__validation_checker(buf);
	
	std::string b_data;
	
	auto expr = buf.find('=');
	bool hcon = false;
	if(expr != std::string::npos){
		b_data = buf.substr(expr + 1, buf.length() - expr);
		hcon = true;
	}else{
		b_data = buf;
	}
	
	auto mvar = get_variables_borders(b_data);
	if(mvar.size() != 0){
		std::map<std::string, numeric_fmt>::iterator __nat;
		for(auto i = mvar.begin(); i != mvar.end(); i++){
			__nat = __namespace.find(b_data.substr((*i).first, (*i).second - (*i).first + 1)); //+1
			if( __nat == __namespace.end()){
				throw p_excep("ERROR: no such variable \'" + b_data.substr((*i).first, (*i).second - (*i).first + 1) + "\'");
			}
			b_data = b_data.substr(0, (*i).first) +
					 std::to_string((*__nat).second) + 
					 ( ((*i).second==(b_data.length()))?(""):b_data.substr((*i).second + 1, b_data.length() - (*i).first));
		}
	}
	std::string var_buf;
	while(true){
		auto p = find_most_priority_parentheses( b_data);
		if( (p.first == 0) && ( (p.second ==  b_data.length()) || (p.second ==  b_data.length() - 1))){
			prc( b_data);
			if(hcon){
				break;
			}else{
				buf = b_data;
				return;
			}
		}
		var_buf =  b_data.substr(p.first + 1, p.second - p.first - 1);
		prc(var_buf);
		 b_data =   b_data.substr(0, p.first) + var_buf +   b_data.substr(p.second + 1,  b_data.length() - p.second);
	}
	__namespace[buf.substr(0, expr)] = atof(b_data.c_str());
	buf.erase();
}

std::list<std::pair<size_t, size_t>> calc_prs::parser::get_variables_borders(std::string &data){
	std::list<std::pair<size_t, size_t>> lst;
	std::pair<size_t, size_t> b;
	while(true){
		b = get_var_borders(data, (lst.size()==0)?0:(lst.back().second + 1));
		if(b.first == std::string::npos) return lst;
		lst.insert(lst.end(), b);
		if(b.second == data.length()) return lst;
	}
}

std::pair<size_t, size_t> calc_prs::parser::get_var_borders(std::string& data, size_t pos){
	for(size_t i = pos; i < data.length(); i++){
		if(is_name_char(data[i])){
			return {i, near_operators(data, i, true) - 1};
		}
	}
	return {std::string::npos, std::string::npos};
}

inline bool calc_prs::parser::is_name_char(const char c){
	for(int l = 'a'; l < (int)'z'; l++){
		if(c == (char)l) return true;
	}
	for(int l = 'A'; l < (int)'Z'; l++){
		if(c == (char)l) return true;
	}
	return false;
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

inline bool calc_prs::parser::is_oper(const char c){
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
	throw p_excep("ERROR: undefined operator \'" + std::string(&oper, 1) + "\'");
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

inline bool calc_prs::parser::__s_check(const char ch){
	if((ch >= int('0')) && (ch <= int('9'))) return true;
	if((ch == '/') || (ch == '*') || (ch == '-') || (ch == '+')) return true;
	if( ch == '=') return true;
	if((ch == '(') || (ch == ')')) return true;
	return is_name_char(ch);
}

void calc_prs::parser::__validation_checker(std::string &data){
	auto count_open_p = std::count(data.begin(), data.end(), '(');
	auto count_close_p = std::count(data.begin(),data.end(), ')');
	if(count_close_p != count_open_p){
		throw fmt_error("Parenthesis is missing", (count_close_p > count_open_p)?0:data.length());
	}
	char ch;
	for(size_t i(0); i < data.length(); i++){
		ch = data[i];
		if( !__s_check(ch)) {
			throw fmt_error("unknown symbol \'" + std::string(&ch, 1) + "\'", i);
		}
	}
	
}






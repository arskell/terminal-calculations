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

std::string calc_prs::parser::solve_input_data(std::string data){
	std::string var_buf;
	while(true){
		auto p = find_most_priority_parentheses( data);
		if( (p.first == 0) && ( (p.second ==  data.length()) || (p.second ==  data.length() - 1))){
			prc(data);
			return data;
		}
		var_buf =  data.substr(p.first + 1, p.second - p.first - 1);
		prc(var_buf);
		data =   data.substr(0, p.first) + var_buf +   data.substr(p.second + 1,  data.length() - p.second);
	}
}

std::string calc_prs::parser::dereference_all_vars(std::string data,std::map<std::string, numeric_fmt> &nmspace, bool excep_throw){
	auto mvar = get_variables_borders(data);
	if(mvar.size() != 0){
		std::string name;
		std::string tmp;
		std::map<std::string, numeric_fmt>::iterator __nat;
		for(auto i = mvar.begin(); i != mvar.end(); i++){
			name = data.substr(i->first, i->second - i->first + 1);
			if(name.find('(') != std::string::npos) continue;
			__nat = nmspace.find(name); 
			if( __nat == nmspace.end()){
				if(excep_throw){
					throw p_excep("ERROR: no such variable \'" + data.substr((*i).first, (*i).second - (*i).first + 1) + "\'");
				}else{continue;}
			}
			tmp =  std::to_string((*__nat).second);
			data = data.substr(0, (*i).first) +
					 tmp + 
					 ( ((*i).second==(data.length()))?(""):data.substr((*i).second + 1, data.length() - (*i).first));
			for(auto j = i; j!= mvar.end(); j++){
				j->first = j->first + (tmp.length() - name.length());
				j->second = j->second + (tmp.length() - name.length());
			}
		}
	}
	return data;
}

std::string calc_prs::parser::dereference_all_vars(std::string data, std::vector<std::pair<std::string, numeric_fmt>> &nmspace, bool excep_throw){
	auto mvar = get_variables_borders(data);
	if(mvar.size() != 0){
		std::string name;
		std::string tmp;
		std::vector<std::pair<std::string, numeric_fmt>>::iterator __nat;
		for(auto i = mvar.begin(); i != mvar.end(); i++){
			name = data.substr(i->first, i->second - i->first + 1);
			if(name.find('(') != std::string::npos) continue;
			__nat = find(nmspace, name);
			if( __nat == nmspace.end()){
				if(excep_throw){
					throw p_excep("ERROR: no such variable \'" + data.substr((*i).first, (*i).second - (*i).first + 1) + "\'");
				}else{continue;}
			}
			tmp =  std::to_string((*__nat).second);
			data = data.substr(0, (*i).first) +
					 tmp + 
					 ( ((*i).second==(data.length()))?(""):data.substr((*i).second + 1, data.length() - (*i).first));
			for(auto j = i; j!= mvar.end(); j++){
				j->first = j->first + (tmp.length() - name.length());
				j->second = j->second + (tmp.length() - name.length());
			}
		}
	}
	return data;
}

void calc_prs::parser::process_buf(){
	__prevalidation_valid_checker(buf);
	for(size_t i = 0; i < buf.length(); i++){
		if(buf[i] == ' '){
			buf = buf.substr(0, i) + buf.substr(i + 1, buf.length() - i);
		}
	}
	__validation_checker(buf);
	if(buf == "namespace"){ 	/* temporary code word to debugging 		*/
		buf = "\n";
		for(auto i = __namespace.begin(); i != __namespace.end(); i++){
			buf += i->first + " == " + std::to_string(i->second) + "\n";
		}
		return;
	}
	
	if(buf == "funcspace"){ 	/* temporary code word to debugging 		*/
		buf = "\n";
		for(auto i = __funcspace.begin(); i != __funcspace.end(); i++){
			buf += i->first + " ::: ";
			for(auto j = i->second.local_namespace.begin(); j != i->second.local_namespace.end(); j++){
				buf += j->first + ", ";
			}
			buf+= ";\n";
		}
		return;
	}
	switch(get_koid(buf)){
		case EXPRESSION:
			{
				auto expr = buf.find('=');
				bool hcon = false;
				std::string varname;
				if(expr != std::string::npos){
					varname = buf.substr(0, expr);
					buf = buf.substr(expr + 1, buf.length() - expr);
					hcon = true;
				}
				buf = dereference_all_vars(buf, __namespace);
				auto high_priority_func_borders = get_function_borders(buf); 
				std::string workd;
				while(high_priority_func_borders.first != std::string::npos){
					workd = buf.substr(high_priority_func_borders.first, high_priority_func_borders.second - high_priority_func_borders.first);// without last ')'
					size_t offset = workd.find('(');
					std::string function_name = workd.substr(0, offset);
					std::string var_def = workd.substr(offset + 1, workd.length() - offset - 1);
					if(__funcspace.find(function_name) == __funcspace.end()) throw fmt_error("no such function \'" + function_name + "\'", high_priority_func_borders.first);
					function _fnc = __funcspace[function_name];
					workd = _fnc.expression;
					for(auto it = _fnc.local_namespace.begin(); it!=_fnc.local_namespace.end(); it++){
						size_t __tmp = 0;
						__tmp = var_def.find(',', __tmp);
						it->second = NUMERIC_FMT_TO_STRING_FUNCTION(solve_input_data(var_def.substr(0, __tmp)).c_str());
						var_def = var_def.substr(__tmp + 1, var_def.length() - __tmp - 1);
					}
					workd = dereference_all_vars(workd, _fnc.local_namespace, false);
					workd = dereference_all_vars(workd, __namespace);
					workd = solve_input_data(workd);
					buf = buf.substr(0, high_priority_func_borders.first) + workd + buf.substr(high_priority_func_borders.second +1,
																								buf.length() - high_priority_func_borders.second - 1);
					high_priority_func_borders = get_function_borders(buf);
				}
				buf = solve_input_data(buf);
				if(hcon){
					__namespace[varname] = NUMERIC_FMT_TO_STRING_FUNCTION(buf.c_str());
					buf.erase();
				}
			}
			break;
		case FUNCTION_DEF:
			{
				auto fd_opr = buf.find(':');
				std::string fdef = buf.substr(0, fd_opr - 1);
				function fnc;
				fnc.expression = buf.substr(fd_opr + 1, buf.length()-fd_opr);
				auto f_o = fdef.find('(');;
				auto b = fdef.find(',', f_o + 1);
				size_t s_o = (b == std::string::npos)?(fdef.length() - 1):(b-1);
				while(f_o != std::string::npos){
					f_o++;
					std::string var_name = fdef.substr(f_o, s_o - f_o + 1);
					if(!is_name_char(var_name[0])) throw fmt_error("unexpected symbol at arguments", f_o);
					//fnc.local_namespace[var_name] = 0;
					fnc.local_namespace.insert(fnc.local_namespace.end(), std::pair<std::string, numeric_fmt>(var_name, 0));
					fdef = fdef.substr(0, f_o - 1) + fdef.substr(s_o + 1, fdef.length() - s_o);
					f_o = fdef.find(',');
					b = fdef.find(',', f_o + 1);
					s_o = (b == std::string::npos)?(fdef.length()-1):(b-1);
				}
				__funcspace[fdef] = fnc;
				buf.erase();
			}
			break;
		default:
			break;
	}
}

std::vector<std::pair<std::string, calc_prs::numeric_fmt>>::iterator
calc_prs::parser::find(std::vector<std::pair<std::string, numeric_fmt>>& vctr, std::string& word){
	for(auto it = vctr.begin(); it != vctr.end(); it++){
		if(it->first == word) return it;
	}
	return vctr.end();
}

calc_prs::koid calc_prs::parser::get_koid(std::string &data){
	if(int c = std::count(data.begin(),data.end(), ':')){
		if(c == 1) return FUNCTION_DEF;
		if(c > 1) throw fmt_error("unexpected symbol \':\'", data.rfind(':'));
	}
	return EXPRESSION;
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

void calc_prs::parser::__prevalidation_valid_checker(std::string &data){
	for(size_t i = 0; i < data.length(); i++){
		if(data[i] == ' '){
			if( (i > 0) && (i < (data.length() + 1))){
				if(!is_oper(data[i-1]) && !is_oper(data[i+1])){	
					if( (data[i-1] != '=') && (data[i+1] != '='))throw fmt_error("Invalid syntax", i);
				}
			}
		}
	}
}

std::pair<size_t, size_t> calc_prs::parser::get_var_borders(std::string& data, size_t pos){
	/*size_t r;
	for(size_t i = pos; i < data.length(); i++){
		if(is_name_char(data[i])){
			r = near_operators(data, i, true) - 1;
			return {i, data[r]==')'?r-1:r};
		}
	}*/
	auto candidate = get_char_word(data, pos);
	//size_t i = candidate.second;
	while(candidate.second != std::string::npos){
		if(data[candidate.second+1] != '(') return candidate;
		candidate = get_char_word(data, candidate.second+1);
	}
	return {std::string::npos, std::string::npos};
}

inline bool calc_prs::parser::is_name_char(const char c){
	if( (c >= 'a') && ( c <= 'z')){
		return true;
	}
	if( (c >= 'A') && ( c <= 'Z')){
		return true;
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

std::pair<size_t, size_t> calc_prs::parser::get_function_borders(std::string& data){
	std::pair<size_t, size_t> prev = get_char_word(data, 0);
	std::pair<size_t, size_t> cor;
	if(prev.first == std::string::npos) return {std::string::npos, std::string::npos};
	while(prev.first!=std::string::npos){
		cor = prev;
		prev = get_char_word(data, cor.second + 1);
	}
	std::string buffer = data;
	size_t op_p = static_cast<size_t>(std::count(data.begin()+cor.first, data.end(), '('));
	size_t right_index = cor.second;
	{
		if(op_p != 1){
			for(size_t i = 0; i < op_p; i++){ 
				right_index = data.find(')', right_index+1);
			}
		}else{
			right_index = data.find(')', cor.second);
		}
	}
	return {cor.first, right_index};
}

std::pair<size_t, size_t> calc_prs::parser::get_char_word(std::string &data, size_t pos){
	size_t r = 0;
	for(size_t i = pos; i < data.length(); i++){
		if( is_name_char(data[i])){
			r = i;
			r++;
			while(is_name_char(data[r])){ r++;}
			return {i, r - 1};
		}
	}
	return {std::string::npos, std::string::npos};
}

void calc_prs::parser::prc(std::string &data){
	numeric_fmt a,b;
	std::string b_str, l_str, r_str;
	while(have_oper(data, oper_list)){
		auto exp = find_most_priority_exp(data);
		a = NUMERIC_FMT_TO_STRING_FUNCTION(data.substr((exp.second.first==0)?exp.second.first:(exp.second.first + 1), exp.first - exp.second.first).c_str());
		b = NUMERIC_FMT_TO_STRING_FUNCTION(data.substr(exp.first + 1,exp.second.second- exp.first).c_str());
		#ifdef DEBUG
		std::cout<<exp.first<< " <"<< exp.second.first <<"," << exp.second.second<<">"<<std::endl;
		std::cout<<a<<std::endl;
		std::cout<<b<<std::endl;
		#endif
		b_str = std::to_string(solve_bi_expression(a, b, data[exp.first]));
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

calc_prs::numeric_fmt calc_prs::parser::solve_bi_expression(calc_prs::numeric_fmt& f, calc_prs::numeric_fmt& s, char oper){
	switch (oper){
		case '*':
		return f * s;
		case '/':
		return f / s;
		case '+':
		return f + s;
		case '-':
		return f - s;
		case '^':
		return pow(f, s);
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
	if((ch == '/') || (ch == '*') || (ch == '-') || (ch == '+') || (ch == '^')) return true;
	if( ch == '=') return true;
	if( ch == ':') return true;
	if( ch == ',') return true;
	if( ch == '.') return true;
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
	switch(std::count(buf.begin(), buf.end(), '=')){
	case 0:
		break;
	case 1:
		if(!is_name_char(buf[0])) throw fmt_error("invalid expression", 0);
		break;
	default:
		throw fmt_error("unexpected symbol \'=\'", buf.rfind('=')); 
	}
	
}






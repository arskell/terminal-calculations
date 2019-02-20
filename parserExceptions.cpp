#include"parserExceptions.h"

calc_prs::p_excep::p_excep(std::string msg, std::string mtype){
	_msg = msg;
	_type = mtype;
}

std::string calc_prs::p_excep::get_msg(){
	return _msg;
}


calc_prs::fmt_error::fmt_error(std::string msg, size_t marker):p_excep::p_excep(msg, "fmt_error"){
	_mrk = marker;
}

std::string calc_prs::fmt_error::get_msg(){
	return std::string("ERROR: "+ _msg + " at " + std::to_string(_mrk));
}
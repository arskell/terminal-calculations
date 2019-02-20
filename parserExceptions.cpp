#include"parserExceptions.h"

calc_prs::p_excep::p_excep(std::string msg, std::string mtype){
	_mrk = std::string::npos;
	_msg = msg;
	_type = mtype;
}

size_t calc_prs::p_excep::get_marker_position(){
	return _mrk;
}

std::string calc_prs::p_excep::get_msg(){
	return _msg;
}


calc_prs::fmt_error::fmt_error(std::string msg, size_t marker):p_excep::p_excep(msg, "fmt_error"){
	_mrk = marker;
}

std::string calc_prs::fmt_error::get_msg(){
	return std::string("ERROR: "+ _msg + " at " + "position number " + std::to_string(_mrk));
}


calc_prs::too_big_number_exception::too_big_number_exception():p_excep::p_excep("Out of buffer", "too_big_number_exception"){
	
}

std::string calc_prs::too_big_number_exception::get_msg(){
	return std::string("ERROR: Out of buffer number");
}
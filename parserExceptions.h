
#ifndef PARSER_EXC_H

#define PARSER_EXC_H

#include<string>

namespace calc_prs{
	
	class p_excep{
	public:
		p_excep(std::string msg, std::string mtype = "base parse exception");
		virtual std::string get_msg();
	protected:
		std::string _msg;
		std::string _type;
	};
	
	
	class fmt_error:public p_excep{
	public:
		fmt_error(std::string msg, size_t marker);
		virtual std::string get_msg();
		size_t get_marker_position();
	private:
		size_t _mrk;
	};
	
	class too_big_number_exception: public p_excep{
	public:
		too_big_number_exception();
		virtual std::string get_msg();
	};
}
#endif
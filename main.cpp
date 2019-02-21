#include<iostream>
#include<string>
#include"parser.h"

void error_proc(calc_prs::p_excep &exc, calc_prs::parser &prs);

int main(int argc, char** argv){
	bool error = false;
	calc_prs::parser prs;
	if (argc > 1){
		prs<<argv[1];
		try{
			prs.process_buf();
		}catch(calc_prs::p_excep &exc){
			error_proc(exc, prs);
			error = true;
		}
		if(!error) std::cout<<prs.get_result()<<std::endl;
		return 0;
	}
	std::string inp;
	while(true){
		error = false;
		std::cout<<"<<< ";
		std::getline(std::cin, inp);
		if(inp[0] == 'q'){
			return 0;
		}
		prs<<inp;
		try{
			prs.process_buf();
		}catch(calc_prs::p_excep &exc){
			error_proc(exc, prs);
			error = true;
		}
		if(!error) std::cout<<">>> "<<prs.get_result()<<std::endl;
		prs.erase_buf();
		inp.erase();
	}
}

void error_proc(calc_prs::p_excep &exc, calc_prs::parser &prs){
	std::cout<< exc.get_msg() << ":" <<std::endl;
	if(exc.has_marker()){
		std::cout<<prs.get_result()<<std::endl;
		for(size_t i = 0; i < exc.get_marker_position(); i++){
		std::cout<<" ";
		}
		std::cout<<"^" <<std::endl;
	}
}

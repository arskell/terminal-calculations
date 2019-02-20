#include<iostream>
#include<string>
#include"parser.h"

int main(int argc, char** argv){
	calc_prs::parser prs;
	/*
	prs<<argv[(argc==1)?0:1];
	prs.process_buf();
	std::cout<<prs.get_result()<<std::endl;
	*/
	while(true){
		std::cout<<"<<< ";
		std::getline(std::cin, inp);
		if(inp[0] == 'q'){
			return 0;
		}
		prs<<inp;
		prs.process_buf();
		std::cout<<">>> "<<prs.get_result()<<std::endl;
		prs.erase_buf();
		inp.erase();
	}
}
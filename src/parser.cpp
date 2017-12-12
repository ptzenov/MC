/** 
 * file: parser.cpp
 * Input file parser and utility funcitons
 *
 **/
#include "parser.hpp"

#include <fstream>
#include <sstream>
#include <iostream>


std::string normalizeStr(std::string str){

	std::string result(str);

	result.erase(0,str.find_first_not_of(" \n\r\t"));
	result.erase(str.find_last_not_of(" \n\r\t")+1);
		
	return result;

}

std::vector<std::string> tokenize(std::string str, std::string DELIMITER){

	std::vector<std::string> v;
	int ctr_0 =0;
	bool tokenfound = false;

	for(size_t i = 0; i< str.length(); ++i){
		if(DELIMITER.find(str[i]) != std::string::npos){
			if (!tokenfound){
				v.push_back(str.substr(ctr_0,i-ctr_0));
				tokenfound = true;
			}
			ctr_0 = i+1;
		}else{tokenfound = false;}
	}
	v.push_back(str.substr(ctr_0));
	return v;
}


std::vector<std::string>  getOptionFromFile(char* filename, std::string option){

	std::ifstream file(filename);
	std::string line;
	
	if(file == nullptr)
	{
		std::cerr << "could not open file" << filename << " .Aborting \n";
		std::string msg (filename);
		throw std::ios_base::failure("Cannot open file! "+msg); // throwing and exception! 
	}

	std::vector<std::string> result;
	bool found  = false;

	// skip through the file until the first non commented or empty line
	if (file.is_open()){
		std::cout << "FILE OPENED\n";

		std::string DELIMITER = " =\n\t\r[]";

		while(file.good()){

			getline(file, line);
			while ((line.empty() || line.find("#") !=std::string::npos) && file.good())
				getline(file, line);

			//remove trailing and leding spaces.
			
			line = normalizeStr(line);
			// now parse line looking for your option ...
			//tokenize the string removing empty spaces
			std::vector<std::string> tokens = tokenize(line,DELIMITER);
			if(tokens.size() !=0)
				if( tokens[0].compare(option) == 0 ){
					found = true;
					for (size_t i=1; i<tokens.size();i++)
						result.push_back(tokens[i]);
					std::cout << "Option: " << option << " found. Value is:\n";
					for (size_t i=0; i<result.size();i++)
						std::cout << result[i] << " ";
					std::cout << "\n";
					break;
				}
		}
		file.close();
	}
	return result;
}


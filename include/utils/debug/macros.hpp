/**
 * file: utils/debug/macros.hpp
 * Defines some macros used for debugging 
 */
#ifndef _MACROS_HPP
#define _MACROS_HPP
#include <iostream>

#define MSG(X) std::cout<<"MSG: " X << std::endl<< \
	"\tFILE: " << __FILE__<< std::endl << "\tLINE: " << __LINE__<< std::endl;
		
#endif

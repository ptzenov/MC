/** 
 * file: parser.hpp
 *
 * some help routins for parsing input files
 */
#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>
#include <vector>


#define MAX_CHARS_PER_LINE 512


namespace MC
{

std::string normalizeStr(std::string str);

std::vector<std::string> tokenize(std::string str, std::string DELIMITER);

std::vector<std::string>  getOptionFromFile(char* filename, std::string option);

};

#endif


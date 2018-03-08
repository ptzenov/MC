#include <iterator>
#include <iostream>
#include <algorithm>

int main(int argc, char** argv)
{
	char vector[3] = {'I','a','m'};

	auto pritn_c = [&](char const & p){std::cout << p << " ";};
	std::for_each(std::begin(vector),std::end(vector),pritn_c); 

}



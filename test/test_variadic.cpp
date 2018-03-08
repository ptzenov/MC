#include <iterator>
#include <iostream>
#include <algorithm>
template<typename T>
void recurrent_fill(std::vector<T>& container)
{;}

template<typename T, typename... Targs>
void recurrent_fill(std::vector<T>& container , T val, Targs ... args)
{
	container.push_back(val);
	recurrent_fill(container,args...);
}


template<typename T>
void recurrent_insert_data(std::vector<T> & vec)
{
        return;
}

template<typename T, typename size_type, typename... Targs>
void recurrent_insert_data(std::vector<T>& vec,T val,size_type N, Targs... args)
{
	T dummy{};
        std::copy(std::begin(val),std::begin(val)+N,dummy.begin());
        vec.push_back(dummy);
        recurrent_copy_eigenvectors(vec,args...);
}












// must provide a base function! 
// when the param expansion does not work
void recurrent_print(const char* str)
{
	std::cout<< *str;
}

template<typename T, typename ... Targs>
void recurrent_print(const char* str, T val, Targs... vals)
{
	for(;*str != '\0';++str)
	{
		if (*str == '%'){
			std::cout << val; 
			recurrent_print(str+1,vals...);
			return;
		}
		std::cout<<*str;
	}	
}
template<typename T>
void print_v(std::vector<T> vec)
{
	for(auto const & v:vec)
	{std::cout << v << " ";}
	std::cout << std::endl;
}

int main(int argv, char** argc)
{
	const char* text = "Hello, my name is %. I am % years old and live in %."; 
	recurrent_print(text,"petar",30,"Germany"); 
	std::vector<double> test_fill;
	recurrent_fill(test_fill, 1.,2.,3.,4.,5.,6.);
	print_v(test_fill);	

}

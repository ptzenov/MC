#include <iterator>
#include <iostream>
#include <algorithm>
class A
{
	private:
		int data;
	public:
	// A(int a): data(a){;} 
	explicit A(int a): data(a){;}  // disables calls such as new A[5]{1,2,3,4,5};
	A() = delete; 
	int get(){return data;}
};

int main(int argc, char** argv)
{
	char vector[3] = {'I','a','m'};

	auto pritn_c = [&](char const & p){std::cout << p << " ";};
	std::for_each(std::begin(vector),std::end(vector),pritn_c); 
	A* arrayOfA = new A[5]{1,2,3,4,5}; // this is implicit conversion

	// A* arrayOfAA = new A[5](); // call to deleted constructor this is implicit conversion
	std::cout << arrayOfA[2].get();
}



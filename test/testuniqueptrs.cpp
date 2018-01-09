#include <iostream>

#include <algorithm>
#include <memory>
#include <vector>
#include <string>
class A
{
public:
        virtual void foo()
        {
                std::cout << "\nI am A\n" ;
        }

	virtual double get_data(){ return -1.;}

};



class B:public A
{
public:
        void foo()
        {
                std::cout << "\nI am B\n" ;
        }
	virtual double get_data() override 
	{		
		return 1.; 

	//	return std::string{"data from b"};
	}
};


using ptr2A = std::unique_ptr<A>;
using ptr2B = std::unique_ptr<B>;

void bar(ptr2A& v){
	std::cout<< "\n"<< v->get_data() << "\n";
}


int main()
{
	

	ptr2A pa (new A {});
        ptr2A pb (new B {});
	
	pa->foo();
        pb->foo();

	std::vector<ptr2A> v(0); 
	v.push_back(ptr2A(new A{})); 
	v.push_back(ptr2B(new B{}));
	v.push_back(ptr2B(new B{}));
	v.push_back(ptr2B(new B{}));


	for(auto & item:v)
		bar(item);	
}


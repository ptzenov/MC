#include<iostream>


using funcptr = void(*) (int,int); 

void bar(int a, int b){
	std::cout << a+b << "\n"; 
}
void foo(funcptr f, int a, int b)
{
	f(a,b); 
}

int main()
{

	funcptr f = &bar; 
	foo(f,1,2); 
}

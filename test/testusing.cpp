#include<iostream>

using intfunc = void (int&);

void bar(intfunc* f,int& num)
{
        f(num);
}

inline void increment(int& num)
{
        num++;
}

int main()
{
        int a =0;
        intfunc* f = &increment;
        
	bar(f,a);
        std::cout<< "\n a = "<< a << "\n";
        return 0;

}



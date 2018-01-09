#include <iostream>
#include <vector>
#include <list>
#include <memory>

#include <assert.h>
#include <random>
#include <algorithm>

template<typename RandomIt, typename OutputIt>
void draw_without_replacement(RandomIt first, RandomIt last,
                              OutputIt out, int num_elems)
{
        auto seq_size = std::distance(first,last);
        assert(num_elems <= seq_size);
        
	std::vector<int> indices(seq_size);
	std::iota(indices.begin(),indices.end(),0);

	std::random_device rd;
	std::mt19937 mt(rd()); 
	std::uniform_int_distribution<int> dist(0,seq_size); 
	std::function<int(int)> r = [mt,dist](int i){return dist(mt)%i;};
	std::cout << "r = " << r(10) << std::endl;



	std::random_shuffle(indices.begin(),indices.end());
	
	for(int i = 0; i < num_elems; i++ )
		out[i] =  *(first+indices[i]);
};


class A
{
public:
        virtual void print()
        {
                std::cout << "A" << std::endl;
        }
};

class B: public A
{
public:
        void print()
        {
                std::cout << "B" << std::endl;
        }
};

int main()
{
        using Aptr = std::unique_ptr<A>;
        std::vector<Aptr> v;
        v.push_back(Aptr(new B()));
        v[0]->print();


        std::vector<int> a(10);
	std::iota(a.begin(),a.end(),0);

        std::vector<int> b(2);
        draw_without_replacement(a.begin(),a.end(),b.begin(),2);
	for(int i= 0; i<b.size(); i++)
                std::cout<<b[i]  << std::endl;

        return 1;
}





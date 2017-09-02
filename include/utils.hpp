#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <assert.h>
#include <random>
#include <math.h>


#ifndef _UTILS_HPP_
#define _UTILS_HPP_


#define EMPTY_STATE -1
#define ABS(X) (X>=0? X:-X)




namespace MC{

	template<typename T> 
		std::vector<T> draw_without_replacement(std::vector<T> from,int num_elems){

			assert(num_elems <= from.size());
			std::vector<T> result;
			std::random_device rd;
			std::mt19937 mt(rd()); 

			std::uniform_int_distribution<int> dist(0,from.size());

			for(int i=0;i<num_elems;i++){
				int r = dist(mt) % from.size(); 
				T next_elem = from[r];
				from.erase(from.begin()+r); // takes interator! 
				result.push_back(next_elem); 
			}
			return result; 
		}


};

#endif



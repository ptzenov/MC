#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <iostream>
#include <random>
#include <complex>
#include <random>
#include <stdio.h>

#include <time.h>

#include <assert.h>

#include <iterator>
#include <vector>

#include <algorithm>

// complex<double> or complex<float>
#define C_TYPE std::complex<double>
//imaginary unit
#define I C_TYPE(0,1)

// real imag abs and coj
#define C_REAL(x) std::real(x)
#define C_IMAG(x) std::imag(x)
#define C_ABS(x) std::abs(x)
#define C_CONJ(x) std::conj(x)
#define ABS(X) (X>=0? X:-X)


namespace MC
{

template<typename RandomIt, typename OutputIt>
void draw_without_replacement(RandomIt first, RandomIt last,
                              OutputIt out, int num_elems)
{
        auto seq_size = std::distance(first,last);
        assert(num_elems <= seq_size);

        std::vector<int> indices(seq_size);



	srand(time(NULL));

	std::iota(indices.begin(),indices.end(), 0);
        std::random_shuffle(indices.begin(),indices.end());

        for(int i = 0; i < num_elems; i++ )
                *(out+i) =  *(first+indices[i]);
};

template<typename RandGen>
RandGen get_pseudorandom_generator()
{
        std::random_device rd;
        return  RandGen( rd() );
}

template<typename RandGen, typename FloatType>
FloatType draw_random_uniform_double(RandGen gen, FloatType a, FloatType b)
{

	std::uniform_real_distribution<FloatType> dist(a,b);
        return dist(gen);
}

template<typename RandGen, typename FloatType>
FloatType draw_random_uniform_double(FloatType a, FloatType b)
{
       	std::uniform_real_distribution<FloatType> dist(a,b);
        RandGen gen = get_pseudorandom_generator<RandGen>();
	return dist(gen);
}

template<typename RandGen, typename IntType>
IntType draw_random_uniform_int(RandGen gen, IntType a, IntType b)
{
        std::uniform_int_distribution<IntType> dist(a,b);
        return dist(gen);
}

template<typename RandGen, typename IntType>
IntType draw_random_uniform_int(IntType a, IntType b)
{
        std::uniform_int_distribution<IntType> dist(a,b);
        RandGen gen  = get_pseudorandom_generator<RandGen>();
        return dist(gen);
}


};
#endif









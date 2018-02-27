/**
 * file utils/random.hpp
 *
 *  Defines the main routines and algorithms for sampling from 
 *  random distributions
 *
 */
#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

#include <random>
#include <algorithm>


namespace MC
{

/*******  Sampling functions *******/

/**
 * Draw from Randomit and store in OutputIt
 */
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

template<typename RandomIt>
void shuffle(RandomIt first, RandomIt last, int num_elems=-1)
{
        auto seq_size = std::distance(first,last);
        if (num_elems == -1)
                num_elems = seq_size;

        assert ( num_elems <= seq_size );

        std::random_device rd {};
        std::mt19937 engine(rd());
        std::uniform_int_distribution<int> distr(0,seq_size-1);

        for(auto i = 0; i < num_elems; ++i)
        {
                auto j = i+ distr(engine)%(seq_size-i);
                std::swap(*(first+i),*(first+j));
        }
}
/*** DRAW RANDOM UNIFORM FLOATS AND INTS ***/

// retrieve a pseudorandom generator
template<typename RandGen>
RandGen get_pseudorandom_generator()
{
        std::random_device rd;
        return  RandGen( rd() );
}

// FLOATS
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
// INTS
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

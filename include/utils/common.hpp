/**
 * file: utils.hpp
 *
 * Defines the main utility functions for the project
 *
 */

#ifndef _COMMON_HPP_
#define _COMMON_HPP_


#include <random>
#include <complex>
#include <random>

#include <algorithm>
#include <vector>

#include <assert.h>


// change the precision at will.
#define C_TYPE std::complex<double>
#define R_TYPE double

//imaginary unit
#define I C_TYPE(0.,1.)

// real imag abs and coj
#define C_REAL(x) std::real(x)
#define C_IMAG(x) std::imag(x)
#define C_ABS(x) std::abs(x)
#define C_CONJ(x) std::conj(x)
#define ABS(X) (X>=0? X:-X)


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


template<typename ForwardIt,typename ElementComparator >
std::vector<size_t> partial_sort_idx(ForwardIt first, size_t middle, size_t N, ElementComparator comp)
{

        std::vector<size_t> idx(N);
        std::iota(idx.begin(),idx.end(),0);
        struct IndexComparator
        {
        public:
                IndexComparator(ForwardIt const & b, ElementComparator& c):
                        begin (b), compare(c)
                {
                        ;
                }
                bool operator()(int i1, int i2)
                {
                        return compare(begin[i1],begin[i2]);
                }
        private:
                ForwardIt const & begin;
                ElementComparator compare;
        };

        std::partial_sort(idx.begin(),idx.begin()+middle, idx.end(), IndexComparator (first,comp) );
        return idx;
}


// get the index of the partial sorted elements in ForwardIt
template<typename ForwardIt>
std::vector<size_t> partial_sort_idx(ForwardIt first, size_t middle, size_t N)
{
        std::vector<size_t> idx(N);
        std::iota(idx.begin(),idx.end(),0);
        struct IndexComparator
        {
        public:
                IndexComparator(ForwardIt const & b) : begin {b}
                {;}
                bool operator()(int i1, int i2) const
                {
                        return begin[i1] < begin[i2];
                }
        private:
                ForwardIt const & begin;
        };
        std::partial_sort(idx.begin(),idx.begin()+middle, idx.end(), IndexComparator {first} );
        return idx;
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

/*** Numeric functions ***/
template<typename ForwardIt, typename T>
T trapz(ForwardIt z0, ForwardIt zN, ForwardIt f0, ForwardIt fN)
{
        auto Nz = std::distance(z0,zN);
        auto Nf = std::distance(f0,fN);
        assert(Nz  == Nf);
        assert(Nz > 3);
        T integral {0.};
        for(auto i = 1; i < Nz; ++i)
                integral+= (f0[i+1]+f0[i-1])*(z0[i]-z0[i-1])/2.0;
        return integral;
}
};

#endif



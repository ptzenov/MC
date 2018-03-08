/**
 * file: utils/common.hpp
 * Defines the main utility functions for the project
 *
 */
#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <complex>
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
#define SQR(X) (X*X)


namespace MC
{

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


/*** Numeric functions ***/
template<typename ForwardIt>
auto trapz(ForwardIt z0, ForwardIt zN, ForwardIt f0, ForwardIt fN) -> decltype( *z0 * *f0 )
{
        auto Nz = std::distance(z0,zN);
        auto Nf = std::distance(f0,fN);
        assert(Nz  == Nf);
        assert(Nz > 3);
	using T = decltype(*z0 * *f0) ;
        T integral {0};
	for(auto i = 1; i < Nz; ++i)
                integral+= (f0[i]+f0[i-1])*(z0[i]-z0[i-1])/2.0;
        return integral;
}

template<typename ForwardIt_z, typename ForwardIt_f>
auto trapz(ForwardIt_z z0, ForwardIt_z zN, ForwardIt_f f0, ForwardIt_f fN) -> decltype( *z0 * *f0)
{
        auto Nz = std::distance(z0,zN);
        auto Nf = std::distance(f0,fN);
        assert(Nz  == Nf);
        assert(Nz > 3);
        using T = decltype(*z0 * *f0 );
	T integral {0};
	for(auto i = 1; i < Nz; ++i)
                integral+= (f0[i]+f0[i-1])*(z0[i]-z0[i-1])/2.0;
        return integral;
}

};

#endif






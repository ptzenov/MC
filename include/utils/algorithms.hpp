/**
 * file: utils/algorithms.hpp
 * Encapsulates some combinatorial algorithms (sorting, selecting etc.) used throughout.
 *
 */
#ifndef _ALGORITHMS_HPP_
#define _ALGORITHMS_HPP_

#include <algorithm>
#include <vector>
#include <assert.h>

namespace MC
{

template<typename ForwardIt,typename ElementComparator >
std::vector<unsigned int> partial_sort_idx(ForwardIt first, unsigned int middle, unsigned int N, ElementComparator comp)
{

	assert (N >= middle && "cannot implement partial sort when N < middle. aborting");
        std::vector<unsigned int> idx(N);
        std::iota(idx.begin(),idx.end(),0);
        struct IndexComparator
        {
        public:
                IndexComparator(ForwardIt const & b, ElementComparator& c):
                        _begin (b), _compare(c)
                {
                        ;
                }
                bool operator()(int i1, int i2)
                {
                        return _compare(_begin[i1], _begin[i2]);
                }
        private:
                ForwardIt const & _begin;
                ElementComparator _compare;
        };

        std::partial_sort(idx.begin(),idx.begin()+middle, idx.end(), IndexComparator (first,comp) );
        return idx;
}


// get the index of the partial sorted elements in ForwardIt
template<typename ForwardIt>
std::vector<unsigned int> partial_sort_idx(ForwardIt first, unsigned int middle, unsigned int N)
{
	assert (N >= middle && "cannot implement partial sort when N < middle. aborting");
	std::vector<unsigned int> idx(N);
        std::iota(idx.begin(),idx.end(),0);
        struct IndexComparator
        {
        public:
                IndexComparator(ForwardIt const & b) : _begin {b}
                {;}
                bool operator()(int i1, int i2) const
                {
                        return _begin[i1] < _begin[i2];
                }
        private:
                ForwardIt const & _begin;
        };
        std::partial_sort(idx.begin(),idx.begin()+middle, idx.end(), IndexComparator {first} );
        return idx;
}

};

#endif






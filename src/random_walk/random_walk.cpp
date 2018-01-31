#include "random_walk/random_walk.hpp"
#include "utils/common.hpp"

#include <iostream>

MC::RandomWalkState::RandomWalkState(size_t dim): AbstractState(),_dim {dim}, _coords(_dim)
{
        assert(dim > 0);
        for(auto&& c: _coords)
                c = 0.;
}
MC::RandomWalkState::~RandomWalkState()
{
        ;
}

MC::RandomWalkState::RandomWalkState(const RandomWalkState& other): RandomWalkState(other._dim)
{
        std::copy(other._coords.begin(),other._coords.end(),_coords.begin());
}

MC::RandomWalkState& MC::RandomWalkState::operator=(const RandomWalkState& other)
{

        assert(_dim == other._dim);
        std::copy(other._coords.begin(),other._coords.end(),_coords.begin());
        return *(this);
}

double& MC::RandomWalkState::operator[](size_t idx)
{
        assert(idx < _dim);
        return _coords[idx];
}


MC::BrownianRecord::BrownianRecord(size_t dim, size_t Nt): _dim {dim}, _Nt {Nt},
_data {new double[_Nt*_dim]}, _sum {new double[_dim]}, _ctr {0}
{
        for(size_t d = 0; d< _dim; ++d)
                _sum[d] = 0.0;

}

void MC::BrownianRecord::post_process(MC::RandomWalkState& in, MC::RandomWalkState& fin,bool scattered, size_t t)
{
        // store the data?
        for (size_t d = 0; d< in.get_dim(); d++)
        {
                _data[t*_dim + d] = fin[d];
        }
}


bool MC::random_step(MC::RandomWalkState& init, MC::RandomWalkState& fin, size_t t)
{
        for( size_t d = 0; d<init.get_dim(); d++)
        {
                int coin_toss = draw_random_uniform_int<std::mt19937>(0,1);
                double step_d = (-1. + coin_toss*2.0); // either -1/100 or 1/100
                fin[d] = init[d]+step_d/100.;
        }


        return true;
}



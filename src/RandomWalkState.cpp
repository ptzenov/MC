#include "RandomWalkState.hpp"
#include "utils.hpp"
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

double& MC::RandomWalkState::operator[](size_t idx)
{
        assert(idx < _dim);
        return _coords[idx];
}


MC::BrownianRecord::BrownianRecord(size_t dim, size_t Nt): _dim {dim}, _Nt {Nt}, _data {new double[_Nt*_dim]}, _t {0} {;}



void MC::BrownianRecord::post_process(MC::RandomWalkState& in, MC::RandomWalkState& fin,bool scattered)
{
       // store the data? 
	if ((_t% 1000)==0)
        {
                std::cout << "current state is: (" << in.get_coord(0);
                for (size_t d = 1; d< in.get_dim(); d++)
                        std::cout << ","<<in.get_coord(d);
                std::cout << ")"<<std::endl;
        }
        _t++;
}


bool MC::random_step(MC::RandomWalkState& init, MC::RandomWalkState& fin)
{
        for( size_t d = 0; d<init.get_dim(); d++)
        {
                int coin_toss = draw_random_uniform_int<std::mt19937>(0,1);
                double step_d = -1. + coin_toss*2.0; // either -1 or 1
                fin[d] = init[d]+step_d;
        }

        return true;
}


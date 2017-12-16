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

MC::RandomWalkState::RandomWalkState(const RandomWalkState& other): RandomWalkState(other._dim)
{
	std::copy(other._coords.begin(),other._coords.end(),_coords.begin());
}

MC::RandomWalkState& MC::RandomWalkState::operator=(const RandomWalkState& other){
	
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
       	_data {new double[_Nt*_dim]}, _sum{new double[_dim]}, _t {0}, _ctr{0} 
{
	for(size_t d = 0; d< _dim; ++d)
		_sum[d] = 0.0; 	

}



void MC::BrownianRecord::post_process(MC::RandomWalkState& in, MC::RandomWalkState& fin,bool scattered)
{
	
	double radius = 0.0f; 
	
	_t++;
 	// store the data?
	for (size_t d = 0; d< in.get_dim(); d++){
		_sum[d] =(_t-1.0)*_sum[d]/(_t) + fin[d]/(_t);
		radius += fin[d]*fin[d]; 
	}                
	radius = sqrt(radius); 
	if (radius < 1.0)
		_ctr++; 	

	if ((_t% 10000)==0)
        {
                std::cout <<" Expected distance @ time [" << _t << "] is: (" << _sum[0];
                for (size_t d = 1; d< _dim; d++)
                        std::cout << ","<<_sum[d];
                std::cout << ")"<< " current approx value of pi is "<< 4.0* double(_ctr)/double(_t) << std::endl;
        }
}


bool MC::random_step(MC::RandomWalkState& init, MC::RandomWalkState& fin)
{
        
	bool scatter = true;
	MC::RandomWalkState tmp{fin};
	for( size_t d = 0; d<init.get_dim(); d++)
        {
                int coin_toss = draw_random_uniform_int<std::mt19937>(0,1);
                double step_d = (-1. + coin_toss*2.0); // either -1/100 or 1/100
                tmp[d] = init[d]+step_d/100.;
		scatter = scatter & (tmp[d] >= 0 & tmp[d] <= 1);  
        }
       
	if(scatter)
		fin=tmp; // scatter only if inside! otherwise reject

	return scatter;
}


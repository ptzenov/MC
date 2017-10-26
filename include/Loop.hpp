#ifndef _LOOP_HPP_
#define _LOOP_HPP_

#include "utils.hpp"

#include <algorithm>
#include <vector>
#include <assert.h>
namespace MC
{

template<class RandomIt, typename Scatterer, typename Info>
void main_loop(RandomIt first, RandomIt last,
	       std::vector<Scatterer> scatterers,
               Info info, int num_particles, int Nt, double T)
{
	auto distance = std::distance(first, last); 
        assert(num_particles < distance);
        // pick a random state and give it a particle -> draw without replacement!
        std::vector<size_t> particle_ids(num_particles);
	std::vector<size_t> state_ids(distance);
	
	std::iota(state_ids.begin(),state_ids.end(),0);
        draw_without_replacement(state_ids.begin(),state_ids.end(), particle_ids.begin(),num_particles);

	int p_id =  0;
        for(auto loc : particle_ids)
	{
		first[loc].set_pID(++p_id);
	}

	info.output(first, last);
	// hmmm !!
        int num_events = 0;
        double dt = T/(Nt-1);
        for(int t=0; t<Nt; t++)
        {
                // for each particle p
                for(auto idx=0; idx< particle_ids.size(); ++idx) 
                {
                
			auto init_loc = particle_ids[idx];
			auto fin_loc = draw_random_uniform_int<std::mt19937,int>(0,distance-1);

			// select a target state
			auto& initstate = first[init_loc];
			auto& finstate = first[fin_loc];
			auto initID = initstate.get_pID(); 
			auto finID = finstate.get_pID(); 
			Scatterer scatter_func = scatterers[draw_random_uniform_int<std::mt19937,int>(0,scatterers.size()-1)];
			bool scattered = scatter_func(initstate,finstate, info);
			if(scattered)
                        {
				std::cout << "scattering event occured! " << std::endl;
				auto tmp = initstate.get_pID(); 
				initstate.set_pID(finstate.get_pID());
				finstate.set_pID(tmp);
				particle_ids[idx] = fin_loc; 	
				num_events++;
				info.output(first,last); 
			}
                }
        }
	
}


}
#endif




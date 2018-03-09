/**
 * file: loop.hpp
 * contains the templetized version of the main loop
 *
 */
#ifndef _LOOP_HPP_
#define _LOOP_HPP_

#include <utils/common.hpp>
#include <utils/random.hpp>


#include <algorithm>
#include <vector>
#include <assert.h>

namespace MC
{
/**
 * this is the main iteration loop
 * template arguments are (Random Iterator, a Scatterer type (function/functor that scaters states)
 * and an recorder struct responsible for post-processing the scattering event, i.e. recording sm data/displaying results etc.)
 */
template<class RandomIt, typename Scatterer, typename Recorder>
void main_loop(RandomIt first, RandomIt last,
               std::vector<Scatterer> scatterers,
               Recorder& recorder, size_t num_particles, size_t Nt)
{
        assert(num_particles > 0);
        assert(Nt > 0);
        size_t distance = std::distance(first, last);
        assert(num_particles <= distance);

        // pick a random state and give it a particle -> draw without replacement!
        std::vector<size_t> particle_locs(num_particles);
        std::vector<size_t> state_ids(distance);

        std::iota(state_ids.begin(),state_ids.end(),0);
        draw_without_replacement(state_ids.begin(),state_ids.end(), particle_locs.begin(),num_particles);

        int p_id =  0;
        for(auto loc : particle_locs)
        {
                first[loc].set_pID(++p_id);
        }

        for(auto t=0U; t<Nt; ++t)
        {
                // for each particle p
                for(auto idx = 0U; idx< particle_locs.size(); ++idx)
                {
                        auto init_loc = particle_locs[idx];
			// draw a random target location
                        auto fin_loc = draw_random_uniform_int<std::mt19937,int>(0,distance-1); 

                        // get refernce to states
			auto& initstate = first[init_loc];
                        auto& finstate = first[fin_loc];

			// randomly select a scatterrer
                        Scatterer scatter_func = scatterers[
				draw_random_uniform_int<std::mt19937,int>(0,scatterers.size()-1)];
                        // scatter
			bool scattered = scatter_func(initstate,finstate,t);
                        if(scattered)
                                particle_locs[idx] = fin_loc;
			// record something... if needed
                        recorder.post_process(initstate,finstate,scattered,t);
                }
        }
}
}
#endif





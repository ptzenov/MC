/**
 * file: loop.hpp
 * contains the templetized version of the main loop
 *
 */
#ifndef _LOOP_HPP_
#define _LOOP_HPP_

#include <utils/random.hpp>
#include <vector>
#include <assert.h>

namespace MC
{
/**
 * this is the main iteration loop
 * template arguments are (Random Iterator, a Scatterer type (function/functor that scaters states)
 * and an recorder struct responsible for post-processing the scattering event,
 * i.e. recording sm data/displaying results etc.)
 * The parameters are:
 * @param state_first - a RandomIt pointing to the first state from the set
 * @param state_last - a RandomIt pointing PAST the last state from the set
 * @param occ_first - a Forward it pointing to the first occupied state
 * @param occ_last - a Forward it pointing PAST the last occupied state
 *
 * @param  scatterers - a vector<Scatterer> of various possible scattering mechanisms for the simulation.
 * Prior to each scattering event, a scattering mechanism of the type Scatterer is randomly selected from
 * "scatterers". Formally, a Scatterer type is a function with a strictly defined signature
 * bool	scatter(T& init,T& fin,unsigned int t);
 * , where init and fin are the states (of type T) to be scattered; Returns true if the scattering from init
 * to fin was successful.
 * @param recorder a pointer to a Recorder function/functor which has the following signature
 *	Recorder(T& init, T& init, bool scattered, unsigned int t);
 * with init and fin the initial and final state, scattered a parameter specifying whether the scattering
 * mechanism selected returned true
 * @param Nt number of iterations
 */
template<class RandomIt, class ForwardIt, typename Scatterer, typename Recorder>
void main_loop(
	       // pointer to first and last states
	       RandomIt state_first, RandomIt state_last,
               // specifies the initial particle distribution
               ForwardIt occ_first,
               ForwardIt occ_last,
               // scattering mechanisms
               std::vector<Scatterer> scatterers,
               Recorder& recorder,
               unsigned int Nt
             )
{
        // some checks
        assert(Nt > 0);
        unsigned int num_states = std::distance(state_first,state_last);
        unsigned int num_particles = std::distance(occ_first,occ_last);
        assert(num_particles > 0 && "zero particles initialized. Cannot continue.");
        assert(num_particles <= num_states && "partile number grater than number of states. Cannot continue");

        std::vector<size_t> particle_dist(num_particles);

        for(auto t=0U; t<Nt; ++t)
        {
                // this for-loop alters the "particle distribution f(p)"
                for(auto  loc = occ_first; loc != occ_last; ++loc)
                {
                        // draw a random target location
                        auto fin_loc = draw_random_uniform_int<std::mt19937,int>(0,num_states-1);

                        // get refernce to states
                        auto& initstate = state_first[*loc];
                        auto& finstate = state_first[fin_loc];
                        // randomly select a scatterrer
                        Scatterer scatter_func = scatterers[
                                                         draw_random_uniform_int<std::mt19937,int>(
                                                                         0,scatterers.size()-1)];
                        //
                        // scatter, requires initstate, finstate and t
                        //
                        bool scattered = scatter_func(initstate,finstate,t);
                        if(scattered)
                                *loc = fin_loc; // record the new place for the particle ...
                        // record something... if needed
                        recorder.post_process(initstate,finstate,scattered,t);
                }
        }
}
}
#endif








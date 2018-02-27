/**
 * file: subband/random_walk.hpp
 * Incorporates the main classes and utility functions 
 * for implementation of a random walk
 */

#ifndef _RANDOM_WALK_HPP_
#define _RANDOM_WALK_HPP__

#include <state.hpp>


#include <memory>
#include <assert.h>
#include <cmath>

namespace MC
{


class RandomWalkState: public AbstractState
{
private:
        size_t _dim;
        std::vector<double> _coords;

public:

        RandomWalkState(size_t dim);

	RandomWalkState(const RandomWalkState& other); 
	RandomWalkState& operator=(const RandomWalkState& other); 

	RandomWalkState(RandomWalkState&& other) = default; 
	RandomWalkState& operator=(RandomWalkState&& other) = default; 
	~RandomWalkState();

        inline double get_coord(size_t i) const
        {
                assert( i < _dim);
                return _coords[i];
        }
	
	inline size_t get_dim() const
        {
                return _dim;
        };

	double& operator[](size_t idx);
};

/***
 * class that records the steps of the random walk. records the running average and the stdev
 *	implements the "post-process method";
 *
 **/

class BrownianRecord
{

private:
        size_t _dim;
        size_t	_Nt;
	        
	std::unique_ptr<double[]> _data ;
	std::unique_ptr<double[]> _sum;
        
	size_t _ctr; 
public:
        BrownianRecord(size_t dim, size_t N_t);
        void post_process(RandomWalkState&, RandomWalkState&,bool,size_t);
	inline double const * get_data() {return _data.get();}
};

bool random_step(RandomWalkState&,RandomWalkState&, size_t);

}

#endif



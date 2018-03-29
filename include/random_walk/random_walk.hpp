/**
 * file: subband/random_walk.hpp
 * Incorporates the main classes and utility functions
 * for implementation of a random walk
 */

#ifndef _RANDOM_WALK_HPP_
#define _RANDOM_WALK_HPP__

#include <vector> // vector
#include <memory> // unique_ptr

#include <assert.h>

namespace MC
{


class RandomWalkState
{
private:
        unsigned int _dim;
        std::vector<double> _coords;

public:

        RandomWalkState(unsigned int dim);

        RandomWalkState(const RandomWalkState& other);
        RandomWalkState& operator=(const RandomWalkState& other);

        RandomWalkState(RandomWalkState&& other) = default;
        RandomWalkState& operator=(RandomWalkState&& other) = default;
        ~RandomWalkState();

        double get_coord(unsigned int i) const
        {
                assert( i < _dim);
                return _coords[i];
        }

        unsigned int get_dim() const
        {
                return _dim;
        };

        double& operator[](unsigned int idx);
};

/***
 * class that records the steps of the random walk. records the running average and the stdev
 *	implements the "post-process method";
 *
 **/

class BrownianRecord
{

private:
        unsigned int _dim;
        unsigned int	_Nt;

        std::unique_ptr<double[]> _data ;
        std::unique_ptr<double[]> _sum;

        unsigned int _ctr;
public:
        BrownianRecord(unsigned int dim, unsigned int N_t);
	BrownianRecord(BrownianRecord const & other )  = delete; 
	BrownianRecord& operator=(BrownianRecord const & other )  = delete;

	BrownianRecord(BrownianRecord && other )  = default; 
	BrownianRecord& operator=(BrownianRecord && other )  = default; 
	
	
	void post_process(RandomWalkState&, RandomWalkState&,bool,unsigned int);
        double const * get_data()
        {
                return _data.get();
        }
};

bool random_step(RandomWalkState&,RandomWalkState&, unsigned int);

}

#endif




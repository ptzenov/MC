/**
 * file: checkers.hpp
 * Main classes and util functions for the checkers scattering state
 */
#ifndef _CHECKERS_HPP_
#define _CHECKERS_HPP_

#include <iostream>
#include "state.hpp"

#define DIAGONAL (false) // should we enable diagonal scattering?

namespace MC
{

/*
 * A class encapsulating the concept of a state on a checker box.
 * Publicly inherits from AbstractState
 */
class CheckersState: public AbstractState
{
public:

        CheckersState() = delete;

        CheckersState(int x0, int y0):
                AbstractState(),
                _coords {x0,y0}
        {
                ;
        }
        CheckersState(int x0, int y0, int id):
                AbstractState(id),
                _coords {x0,y0}
        {
                ;
        }

        ~CheckersState()
        {
        }

        inline        int get_x() const
        {
                return _coords[0];
        }
        inline int get_y() const
        {
                return _coords[1];
        }
private:
        /* These are the state variables;
         * x,y: current (x,y) coordinates;
         */
        int _coords[2];
};

/*
 * A templated class responsible for post-processing the scattering events!
 * ! must implement the post_process(AbstractState&, AbstractState&, bool) method
 */
template< typename RandomIt>
class RecordCheckers
{
private:
        int _Nx; // num elements in x
        int _Ny; // num elements in y

        RandomIt _first;// first element of the states
        RandomIt _last;// last element of the states

        void output()
        {

                // max num digits is
                int num_digits = int(ceil(log10((double)_Nx*_Ny)));
                auto distance = std::distance(_first,_last);
                assert(distance == _Nx*_Ny);
                std::cout << std::endl;

                for(int n = 0; n<_Ny; n++)
                {
                        std::cout<<"__";
                        for(int k=0; k<num_digits; k++)
                        {
                                std::cout<<"_";
                        }
                        std::cout<<"_  ";
                }
                std::cout<<"\n";
                for (int x=0; x<_Nx; x++)
                {
                        for (int y=0; y < _Ny; y++)
                        {
                                std::cout<<"|_";
                                int particle_id = _first[y+x*_Ny].get_pID();
                                int start_from = 0;
                                if (particle_id != EMPTY)
                                {
                                        start_from =  int(ceil(log10(particle_id+1)));
                                        std::cout<<particle_id;
                                }
                                else
                                {

                                        //	start_from = int(ceil(log10(2)));
                                        //      std::cout<<"*";
                                }
                                for (int k=start_from; k<num_digits; k++)
                                {
                                        std::cout<<"_";
                                }

                                std::cout <<"_| ";
                        }
                        std::cout<<std::endl;
                }
        }
public:
        RecordCheckers(int nx,int ny, RandomIt first, RandomIt last): _Nx(nx),_Ny(ny), _first {first}, _last {last}
        {
                ;
        }


	RecordCheckers(const RecordCheckers& other) = delete; 
	RecordCheckers& operator=(const RecordCheckers& other) = delete; 
	RecordCheckers(RecordCheckers&& other) = delete; 
	RecordCheckers& operator=(RecordCheckers&& other) = delete; 

	inline  int get_Nx() const
        {
                return _Nx;
        }
        inline int get_Ny() const
        {
                return _Ny;
        }
	void post_process(CheckersState& init, CheckersState& fin, bool scattered, size_t t)
	{
		if(scattered)
			output();
	}
};
/**
 * scatterer functor
 **/
class CheckersScatterer
{
private:
        int Nx;
        int Ny;
public:
	CheckersScatterer(int in_Nx, int in_Ny): Nx{in_Nx}, Ny{in_Ny}{;} 
        bool operator()(CheckersState&, CheckersState&, size_t t);
};


}
#endif


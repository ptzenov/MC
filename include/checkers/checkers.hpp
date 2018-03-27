/**
 * file: checkers.hpp
 * Main classes and util functions for the checkers scattering state
 */
#ifndef _CHECKERS_HPP_
#define _CHECKERS_HPP_

#include <iostream>

#define DIAGONAL (false) // should we enable diagonal scattering?

namespace MC
{

/*
 * A class encapsulating the concept of a state on a checker box.
 * Publicly inherits from AbstractState
 */
class CheckersState
{
public:

        CheckersState() = delete;

        CheckersState(int x0, int y0):
                _occupation_number(0U), _coords {x0,y0}
        {
                ;
        }
        ~CheckersState()
        {
        }
        // check if state is empty
        bool empty() const
        {
                return _occupation_number == 0U;
        }
        // add particle to state
        unsigned int add_particle()
        {
                return ++_occupation_number;
        }
        // remove particle from state
        unsigned int remove_particle()
        {
                if(!empty())
                        return --_occupation_number;
                else
                        return 0U;
        }

        int get_x() const
        {
                return _coords[0];
        }
        int get_y() const
        {
                return _coords[1];
        }
private:
        /* These are the state variables;
         * x,y: current (x,y) coordinates;
         */
        unsigned int _occupation_number;
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
        unsigned int _Nx; // num elements in x
        unsigned int _Ny; // num elements in y

        RandomIt _first;// first element of the states
        RandomIt _last;// last element of the states

        void output()
        {

                // max num digits is
                unsigned int distance = std::distance(_first,_last);
                assert(distance == _Nx*_Ny);
                std::cout << std::endl;

                for(auto n = 0U; n<_Ny; ++n)
                {
                        std::cout<<"____  ";
                }
                std::cout<<std::endl;
                for (auto x=0U; x<_Nx; ++x)
                {
                        for (auto y=0U; y < _Ny; ++y)
                        {
                                std::cout<<"|_";
                                if (_first[y+_Ny*x].empty())
                                {
                                        std::cout<<" ";
                                }
                                else
                                {

                                        std::cout<<"*";
                                }

                                std::cout <<"_| ";
                        }
                        std::cout<<std::endl;
                }
        }
public:
        RecordCheckers(unsigned int Nx,unsigned int Ny, RandomIt first, RandomIt last): _Nx(Nx),_Ny(Ny), _first {first}, _last {last}
        {
                ;
        }


        RecordCheckers(const RecordCheckers& other) = delete;
        RecordCheckers& operator=(const RecordCheckers& other) = delete;
        RecordCheckers(RecordCheckers&& other) = delete;
        RecordCheckers& operator=(RecordCheckers&& other) = delete;

        inline  unsigned int get_Nx() const
        {
                return _Nx;
        }
        inline unsigned int get_Ny() const
        {
                return _Ny;
        }
        void post_process(CheckersState& init, CheckersState& fin, bool scattered, unsigned int t)
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
        unsigned int _Nx;
        unsigned int _Ny;
public:
        CheckersScatterer(unsigned int Nx, unsigned int Ny): _Nx {Nx}, _Ny {Ny} {;}
        bool operator()(CheckersState&, CheckersState&, unsigned int t);
};


}
#endif







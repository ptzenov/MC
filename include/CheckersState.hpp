/**
 * file: CheckersState.hpp 
 *
 */ 

#include "State.hpp"

#ifndef _CHECKERS_HPP_
#define _CHECKERS_HPP_

#define DIAGONAL (false) // should we enable diagonal scattering?


namespace MC
{

class CheckersInfo; // forward declare; 

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

        int get_x() const
        {
                return _coords[0];
        }
        int get_y() const
        {
                return _coords[1];
        }
        void print_state();
private:
        /* These are the state variables;
         * x,y: current (x,y) coordinates;
         */
        int _coords[2];
};

class CheckersInfo
{
private:
        int _Nx; // num elements in x
        int _Ny; // num elements in y
        int _num_particles;  // obvious
        int _Nt;  // number of iterations
        double _T; // simulation time
public:
        CheckersInfo(int nx,int ny,int np, int nt, double t): _Nx(nx),_Ny(ny),
                _num_particles(np),_Nt(nt),_T(t)
        {
                ;
        }
        int get_Nx() const
        {
                return _Nx;
        }
        int get_Ny() const
        {
                return _Ny;
        }
        int get_num_particles() const
        {
                return _num_particles;
        }
        int get_Nt() const
        {
                return _Nt;
        }
        double get_T() const
        {
                return _T;
        }

	/** 
	 * a template funciton for printing the checkerboard. 
	 *
	 * The template argument is an object conforming to the RandomIt concept. 
	 */ 
        template< typename RandomIt>
        void output(RandomIt first, RandomIt last) 
        {

                // max num digits is
                int num_digits = int(ceil(log10((double)_Nx*_Ny)));
                auto distance = std::distance(first,last);
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
                                int particle_id = first[y+x*_Ny].get_pID();
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
};
/**
 * delcare scatterer fucntion
 **/
bool scatter_checkers(CheckersState&,CheckersState&,CheckersInfo&);

};


#endif












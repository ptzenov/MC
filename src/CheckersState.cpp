#include <random>
#include <iostream>

#include "utils.hpp"
#include "CheckersState.hpp"

/** CHECKERS STATE **/
void MC::CheckersState::print_state()
{
        std::cout<< "x: "<< get_x() << "; y: "<<get_y() << "\n";
}

bool MC::scatter_checkers(MC::CheckersState& init, MC::CheckersState& fin, MC::CheckersInfo&  info)
{

	
	int Nx = info.get_Nx();
        int Ny = info.get_Ny();
        // do not change anything !
        if ( &init == &fin)
                return false;

        // can only go on un_occupied state!

        if(fin.get_pID() != EMPTY )
                return false;

        double s_prob = 0.0;

        int ix = init.get_x();
        int iy = init.get_y();

        int fx = fin.get_x();
        int fy = fin.get_y();

        int dx = ix-fx;
        int dy = iy-fy;
	// Keep in mind the principle of detailed balance!
        if (DIAGONAL)
        {
                // allows diagonal transitions
                // -_______________
                // |1/9 |1/9 |1/9 |
                // |____|____|____|
                // |1/9 |1/9 |1/9 |
                // |____|____|____|
                // |1/9 |1/9 |1/9 |
                // |____|____|____|
                //
                if (ABS(dx) <= 1 && ABS(dy) <= 1)
                        s_prob =  1./9.;
        }
        else
        {
                // does not allow diagonal transitions
                // capture any kind of bias in the scattering mechanisms?
                // _______________
                // |0   |1/10| 0  |
                // |____|____|____|
                // |6/10|1/10|1/10|
                // |____|____|____|
                // |0   |1/10| 0  |
                // |____|____|____|

                /**
                 * trend
                 */
                double bias = 5./10.;
                double rest = (1. - bias)/4.;

                // scattering from left to right
                if( dy == 1 && dx==0)
                {
                        s_prob = bias;
                }
                else
                {
                        //other possible forms of scattering!
                        if((ABS(dx)==1 && dy ==0) || (ABS(dx)==0 && ABS(dy) == 1) ) s_prob =  rest;
                }
                /*
                 *periodic boundary in y-direction;
                 * */
                if( (dy == -(Ny-1)) && dx == 0) s_prob =  bias;
        }

        //draw a uniform random num from 0 to 1 over 1000 bins
        if (draw_random_uniform_double<std::mt19937,double>(0.,1.) < s_prob)
        {
		return true;
        }
        return false;
}





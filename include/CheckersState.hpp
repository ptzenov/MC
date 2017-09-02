#include "State.hpp"

#ifndef __CHECKERS__
#define _CHECKERS_


#define DIAGONAL (false) 


namespace MC{

	class CheckersInfo: public MetaInfo{
		private:
			int Nx; 
			int Ny;
			int num_particles; 
			int Nt;  // number of iterations 
			double T; // simulation time 
		public:
			CheckersInfo(int nx,int ny,int np, int nt, double t);

			int get_Nx() const;
			int get_Ny() const;
			int get_num_particles() const;
			int get_Nt() const;
			double get_T() const;		
	};


	class CheckersState: public AbstractState{

		private:
			/* These are the state variables; 
			 *
			 * x,y: current (x,y) coordinates; 
			 *
			 */
			int x,y; 
		public:

			CheckersState(int x0, int y0);
			void print_state();

			int get_x() const; 
			int get_y() const;
	};

	bool scatter_checkers(Particle*,AbstractState*,MetaInfo*);
	void print_board(CheckersState** states, CheckersInfo info);

};


#endif 

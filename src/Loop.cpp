#include <iostream>
#include <stdlib.h>
#include <random>

#include "utils.hpp"

#include "Loop.hpp"
#include "State.hpp"
#include "CheckersState.hpp"



using namespace MC;

/** 
 *  ToDo: How to count processses?
 *  current 
 *	
 *      {x-->x+1, jx++
 *  if 
 *      {x-->x-1, jx-- 
 *	summarize into 
 *	jx+=dx%(Nx-1) // in case of periodic boundary conditions
 *	jy+=dy%(Ny-1) // in case of periodic boundary conditions
 *
 *
 *  
 */


void main_checkers_loop(int num_particles,int Nt, double T){

	//initialize all possible states
	int Nx = 10; 
	int Ny = 5; 

	MC::CheckersInfo info(Nx,Ny,num_particles,Nt,T); 

	int num_states = Nx*Ny;
	int num_events = 0;	
	
	AbstractState** states_list = (AbstractState**) malloc(num_states*sizeof(AbstractState*));
	Particle** particles = (Particle**) malloc(num_particles*sizeof(Particle*));	
	
	
	std::vector<int> state_coords; 
	for(int x=0;x<Nx;x++){
		for(int y=0;y<Ny;y++){
			states_list[y+x*Ny] =  new CheckersState(x,y);
			state_coords.push_back(y+x*Ny);
		}
	}

	// pick a random state and give it a particle -> draw without replacement! 
	std::vector<int> init_locations = draw_without_replacement<int>(state_coords,num_particles); 
	std::cout << "draw without replacement done!";
	
	int p_id =  0; 
	for(int loc:init_locations){ // c++ 11 loop 
		// set the initial states of all particles  
		particles[p_id]=new Particle(states_list[loc],p_id+1);
		std::cout<<"creating particle on state "<<loc<< " with id="<<p_id +1 <<"\n";
		p_id++;
	}

	print_board((CheckersState**) states_list, info);


	/** chose a scattering mechanism **/
	scatterer_t scatter_func = &scatter_checkers;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, num_states-1);
	
	//Jx is incremented when scattering occured from x = 0 to x = Nx-1; 
	//Jy is incremented when scattering occured from y = 0 to y = Ny-1; 

	int Jx = 0; 
	int Jy = 0; 
	double dt = T/(Nt-1);
	for(int t=0; t<Nt; t++){
		// for each particle p 
		for(int p=0; p<num_particles;p++){
			
			Particle* particle = particles[p];	
			// select a target state
			int target_state_idx = dist(mt); 
			
			CheckersState* init_state = (CheckersState*) particle->get_current_state();
			CheckersState* target_state  = (CheckersState*) states_list[target_state_idx];

			bool scattered = scatter_func(particle,target_state,&info);
			if(scattered){
			       	std::cout<<"scatter event occured at t=" <<t <<"\n";  
				print_board((CheckersState**) states_list, info);
				num_events++;	
				int dx =target_state->get_x() - init_state->get_x(); 
				int dy =target_state->get_y() - init_state->get_y(); 

				Jx += (dx == Nx-1) ? 1:0;
			        Jy += (dy == Ny-1) ? 1:0; 

				std::cout<<"Xcurrent:" << Jx*1.0/(t*dt) << "; Ycurrent:" << Jy*1.0/(t*dt) << "\n";
			}
		}
	}
	std::cout << "Num scatt events : " << num_events << "\n";	
	
	for(int p=0;p<num_particles;p++) {
		free(particles[p]);
	}
	free(particles);

	
	for(int s=0; s<Nx*Ny;s++){
		free(states_list[s]);
	}
	free(states_list);
}

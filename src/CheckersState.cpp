#include <random>
#include <iostream>

#include "utils.hpp"
#include "CheckersState.hpp"

/** CHECKERS STATE **/ 
MC::CheckersState::CheckersState(int x0,int y0){
	x = x0; 
	y = y0; 
	particle_id = EMPTY_STATE;
}

void MC::CheckersState::print_state(){
	std::cout<< "x: "<< this-> x << "; y: "<<this->y << "\n";
}

int MC::CheckersState::get_x() const {return x;}
int MC::CheckersState::get_y() const {return y;}

/** CHECKERS INFO **/ 
MC::CheckersInfo::CheckersInfo(int nx,int ny,int np, int nt, double t){
	Nx = nx;  
	Ny = ny; 
	num_particles = np; 
	Nt = nt; 
	T = t; 
}	

int MC::CheckersInfo::get_Nx() const {return Nx;}
int MC::CheckersInfo::get_Ny() const {return Ny;}
int MC::CheckersInfo::get_num_particles() const {return num_particles;}
int MC::CheckersInfo::get_Nt() const {return Nt;}
double MC::CheckersInfo::get_T() const {return T;}		


/** SINGLE PARTICLE SCATTERING MECHANISM **/ 
bool MC::scatter_checkers(MC::Particle* particle, MC::AbstractState* target_state, MC::MetaInfo* meta_info){

	//obtain the necessary information by doing the appropriate casting!  
	MC::CheckersState* init = (MC::CheckersState*) particle->get_current_state(); 
	MC::CheckersState* target = (MC::CheckersState*) target_state;
	MC::CheckersInfo* info = (MC::CheckersInfo*) meta_info; 
	int Nx = info->get_Nx(); 
	int Ny = info->get_Ny(); 

	// do not change anything ! 
	if (init == target) { return false;} 

	// can only go on un_occupied state! 
	if (target->is_occupied() ){ return false;}

	double s_prob = 0.0;
	int ix = init->get_x(); int iy = init->get_y(); 

	int fx = target->get_x(); int fy = target->get_y();
	
	int dx = ix-fx;  
	int dy = iy-fy; 

	// Keep in mind the principle of detailed balance! 
	if DIAGONAL{
		// allows diagonal transitions 
		// -_______________
		// |1/9 |1/9 |1/9 |
		// |____|____|____|
		// |1/9 |1/9 |1/9 |
		// |____|____|____|
		// |1/9 |1/9 |1/9 |
		// |____|____|____|
		//
		if (ABS(dx) <= 1 && ABS(dy) <= 1) s_prob =  1./9.;  
	}else{	
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
		if( dy == 1 && dx==0){
			s_prob = bias;
		}else{
			//other possible forms of scattering! 
			if((ABS(dx)==1 && dy ==0) || (ABS(dx)==0 && ABS(dy) == 1) ) s_prob =  rest;
		}
		/*
		 *periodic boundary in y-direction;  
		 * */
		if( (dy == -(Ny-1)) && dx == 0) s_prob =  bias;
	}

	// now execute the scattering!	
	
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(.0, 1.0);

	double r = dist(mt);

	//draw a uniform random num from 0 to 1 over 1000 bins 	
	// make the transition! 
	if (r < s_prob){
		//leave-reassign-occupy
		init->set_particle_id(EMPTY_STATE); 
		target->set_particle_id(particle->get_id());
		particle->set_current_state(target);
		return true; 
	}

	return false; 
}


/** UTILITY FUNC **/ 
void MC::print_board(MC::CheckersState** states, MC::CheckersInfo info){

	int Nx = info.get_Nx(); 
	int Ny = info.get_Ny(); 

	// max num digits is 
	int num_digits = int(ceil(log10((double)Nx*Ny)));

	/* print the upper row*/ 
	for(int n = 0; n<Ny;n++){
		std::cout<<"__";
		for(int k=0;k<num_digits;k++){
			std::cout<<"_";
		}
		std::cout<<"_  ";
	}
	std::cout<<"\n";

	for (int n=0; n<Nx;n++){
		for (int m=0; m< Ny; m++){
			std::cout<<"|_";
			MC::CheckersState* state = states[m+n*Ny];
			int particle_id = state->get_particle_id();
			int start_from = 0;
			if (particle_id >=0){
				start_from =  int(ceil(log10(particle_id+1)));
				std::cout<<particle_id;
			}else{

				//	start_from = int(ceil(log10(2)));
				//      std::cout<<"*";	
			}
			for (int k=start_from;k<num_digits;k++) {std::cout<<"_";}		    

			std::cout <<"_| ";
		}
		std::cout<<"\n";
	}
}



#include "State.hpp"
#include <iostream>
#include <random>



MC::Particle::Particle(MC::AbstractState* init_state, int particle_id){
	id = particle_id; 	
	if (init_state != NULL)
	{
		state = init_state;
		state->set_particle_id(this->id);
	}else{
		state = NULL; 
	}
}
MC::Particle::~Particle(){;}

MC::AbstractState *  MC::Particle::get_current_state()  {return state;}
void MC::Particle::set_current_state(MC::AbstractState * s){ state=s;}






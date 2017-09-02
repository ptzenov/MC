#include <stdlib.h>
#include <time.h>

#ifndef _STATE_HPP_
#define _STATE_HPP_
#define EMPTY_STATE -1

namespace MC{

	class MetaInfo{
	};

	class AbstractState{
		protected: 
			int particle_id; 
		public:
			/*
			 *virtual pure methods. to be overwritten
			 * */
			virtual void print_state() = 0;
			
			int get_particle_id() const {return particle_id;} 
			void set_particle_id(int id){particle_id = id;}
			/**
			 * a universal api method. taken as is; 
			 */
			bool is_occupied(){return particle_id != EMPTY_STATE;}
	};

	class Particle{
		private:
			// state variable
			AbstractState* state;
			// particle identificator;
			int id;
		public: 
			
			/** constructor **/
			Particle(AbstractState* init_state,int id);
			/** destructor **/
			~Particle();
			/*function to a pointer to the current state*/
			AbstractState * get_current_state();
			void set_current_state(AbstractState * s);
			int get_id() const{return id;}
	};

	/**
	 * this type (scatterer) defines a function pointer to a scattering method/funciton. 
	 * the signature is as follows
	 * @param Particle p* : a pointer to the Particle to scatter
	 * @param AbstractState*: a pointer to the final state to scatter to
	 * @param MetaInfo* : some meta information which fully defines the scattering environment (boundary conditions etc). 
	 *
	 * @returns a bool -> true/false if the scattering succeeded  
	 */ 
	typedef bool (*scatterer_t)(Particle*,AbstractState*,MetaInfo*);			

};
#endif

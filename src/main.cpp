#include <iostream>
#include <vector>

#include "spsolver.hpp"
#include "Loop.hpp"
#include "CheckersState.hpp"

int main(int argc, char** argv)
{
	int num_particles = 8;

	int Nt = 100000;
        int Nx = 20;
        int Ny = 10;

        double T = 10.0f;
        std::vector<MC::CheckersState> states;
        
	for (auto x= 0; x <Nx; x++)
                for (auto y=0; y< Ny; y++)
                        states.push_back(MC::CheckersState(x,y));
	
	MC::CheckersInfo info(Nx,Ny,num_particles,Nt,T);
        using scatter_t = bool(*)(MC::CheckersState&,MC::CheckersState&, MC::CheckersInfo&);

	std::vector<scatter_t> scatterers;
       	scatterers.push_back(&MC::scatter_checkers);
	std::cout << "scatterrs size is " << scatterers.size() << std::endl;
	MC::main_loop(states.begin(),states.end(),scatterers,info,
			num_particles, Nt);
        std::cout<<"Bubye"<<"\n";

}





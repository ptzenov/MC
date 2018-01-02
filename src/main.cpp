#include <iostream>
#include <vector>

#include "spsolver.hpp"
#include "Loop.hpp"
#include "CheckersState.hpp"
#include "RandomWalkState.hpp"

#include "GNUplotter.hpp"

void monte_carlo_checkers()
{
        size_t Nt = 10000;
        size_t num_particles = 1;

        int Nx = 10;
        int Ny = 10;

        std::vector<MC::CheckersState> states;

        for (auto x= 0; x <Nx; ++x)
                for (auto y=0; y< Ny; ++y)
                        states.push_back(MC::CheckersState(x,y));
        MC::RecordCheckers<decltype(states.begin())> record{Nx,Ny,states.begin(),states.end()};
        MC::CheckersScatterer scatterer{Nx,Ny};
        std::vector<MC::CheckersScatterer> scatterers;
        scatterers.push_back(scatterer);

	MC::main_loop(states.begin(),states.end(),scatterers,
                      record,
                      num_particles, Nt);
        



}

void monte_carlo_randomwalk()
{
        size_t Nt = 10000;
        size_t num_particles = 1;
        size_t dim = 2;

        std::vector<MC::RandomWalkState> states;
        states.push_back(MC::RandomWalkState {2});

        using scatterer_func = bool(*)(MC::RandomWalkState&, MC::RandomWalkState&, size_t t);
        std::vector<scatterer_func> scatterers;
        scatterers.push_back(MC::random_step);

        MC::BrownianRecord record {dim,Nt};
        MC::main_loop(states.begin(),states.end(),scatterers,
                      record,
                      num_particles, Nt);
       
	GNUPlotter plotter{};
	
	plotter.set_line_color("#FF0000");
	plotter.set_line_specifier("lines");
	plotter.set_line_width(1.2); 
	
	plotter.plot(record.get_data(),dim, Nt);
		
	std::cout<<"Bubye"<<"\n";

}

int main(int argc, char** argv)
{

	monte_carlo_randomwalk();
}









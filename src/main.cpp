#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>
#include <ctime>

#include <time.h>

#include <subband/subband.hpp>
#include <loop.hpp>
#include <checkers/checkers.hpp>
#include <random_walk/random_walk.hpp>
#include <plot/GNUplotter.hpp>

#include <utils/writer.hpp>
#include <utils/constants.hpp>

void monte_carlo_checkers()
{
        unsigned int Nt = 10;
        unsigned int num_particles = 10;

        unsigned int Nx = 10;
        unsigned int Ny = 10;

        std::vector<MC::CheckersState> states;
        for (auto x= 0U; x <Nx; ++x)
                for (auto y=0U; y< Ny; ++y)
                        states.push_back(MC::CheckersState(x,y));
        MC::RecordCheckers<decltype(states.begin())> record {Nx,Ny,states.begin(),states.end()};
        MC::CheckersScatterer scatterer {Nx,Ny};
        std::vector<MC::CheckersScatterer> scatterers;
        scatterers.push_back(scatterer);

        //*** initial particle distribution *** //

        std::vector<unsigned int > particle_dist(num_particles);
        std::vector<unsigned int> state_ids(Nx*Ny);
        // randomly populate the states with num_particles , particles
        std::iota(state_ids.begin(),state_ids.end(),0);
        MC::draw_without_replacement(state_ids.begin(),state_ids.end(),
                                     particle_dist.begin(),num_particles);
        // occupy
        for( auto loc : particle_dist)
        {
                states[loc].add_particle();
        }
        MC::main_loop(states.begin(),
                      states.end(),
                      particle_dist.begin(),
		      particle_dist.end(),
                      scatterers,
		      Nt, record);
}

void monte_carlo_randomwalk()
{
        unsigned int Nt = 10000;
        unsigned int dim = 2;

        std::vector<MC::RandomWalkState> states;
        states.push_back(MC::RandomWalkState {dim});
        std::vector<unsigned int> particle_dist;
        particle_dist.push_back(0);

        using scatterer_func = bool(*)(MC::RandomWalkState&, MC::RandomWalkState&, unsigned int t);
        std::vector<scatterer_func> scatterers;
        scatterers.push_back(MC::random_step);

        MC::BrownianRecord record {dim,Nt};
        MC::main_loop(states.begin(),
		      states.end(),
                      particle_dist.begin(),
                      particle_dist.end(),
                      scatterers,
                      Nt, record);
        GNUPlotter plotter {};

        plotter.set_line_color("#FF0000");
        plotter.set_line_specifier("lines");
        plotter.set_line_width(1.2);
        plotter.plot(record.get_data(),dim, Nt); // dangerous
        std::cout<<"Bubye"<<"\n";
}


void sp_solve_calculate_and_plot()
{
        int nrWF = 2; // considered levels (number of wavefunctions)
        double T = 100.0; // Temperature K
        double rhoD = 1.9e22; // doping density m-3
        double E0 = 1.25e6; // applied field in V/m (bias electric field)
        double n2D = 3.0e14; // sheet doping density
        double dE = 1.0e-5 * MC::e0; // accuracy for Fermi level
        double dz = 1.0e-10; // grid point spacing

        double p = 0.15; // percentage of Al for GaAs / ALGaAs
        double CBO = MC::e0 * 0.62 * (1.594 * p +
                                      p * (1 - p) * (0.127 - 1.310 * p));   // conduction band offset in Joule

        // Definition of materials
        MC::Material GaAs, Al_15_GaAs_85;

        // Definition of their properties
        // material, bandgap in J, valence band offset in J,
        // effective mass in kg, absolute permittivity in A*s/V/m
        MC::material_properties(GaAs, 0., 0., 0.067*MC::me, 12.9*MC::eps0);
        MC::material_properties(Al_15_GaAs_85, CBO, 0., 0.067*MC::me, 12.9 * MC::eps0);

        // Definition of Layers: Material type, thickness in m, doping in m3
        double spacing = 1.0e-10;
        std::vector<MC::Layer>  layers =
        {
                MC::Layer(Al_15_GaAs_85, 55 * spacing, 0),
                MC::Layer(GaAs, 79 * spacing, 0),
                MC::Layer(Al_15_GaAs_85, 25 * spacing, 0),
                MC::Layer(GaAs, 65 * spacing, 0),
                MC::Layer(Al_15_GaAs_85, 41 * spacing, 0),
                MC::Layer(GaAs, 155 * spacing, rhoD),
                MC::Layer(Al_15_GaAs_85, 30 * spacing, 0),
                MC::Layer(GaAs, 90 * spacing, 0),
                MC::Layer(Al_15_GaAs_85, 55 * spacing, 0),
        };

        // execute spsolver: layers, Temperature in K, bias in V/m,
        // #wavefunctions, sheet dopnig density, fermi level accuracy, grid spacing

        MC::SimParams params;
        params.Temp_K = T;
        params.bias_V_m = E0;
        params.n2D = n2D;
        params.dE = dE;
        params.nrWF = nrWF;
        params.dz = dz;

        std::vector<MC::Subband> subbands =
                MC::sp_solve(layers, params);

        // write reuslts to file?
        auto idx = 0;
        const char* filename = "results.txt";
        auto now = std::chrono::system_clock::now();
        auto now_time_t = 	std::chrono::system_clock::to_time_t(now);
        MC::write_meta_data("TimeStamp: ", std::ctime(&now_time_t),filename,'w');
        for( auto& subband: subbands)
        {
                //get the wave funciton
                auto& WF = subband.PHI();
		MC::write_meta_data("IDX = ",idx++,filename,'a');
                auto Npts = std::distance(std::begin(WF),std::end(WF));
                MC::write_meta_data("npts = " , Npts, filename ,'a');
                MC::write_contiguous_array(std::begin(WF),std::end(WF),Npts,filename,'a');
        }
}


int main(int argc, char** argv)
{
	sp_solve_calculate_and_plot();
	monte_carlo_randomwalk();
	monte_carlo_checkers();
	std::cout<< "Done" << std::endl;

	return 1;
}






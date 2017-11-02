#include <iostream>

#include <vector>
#include <algorithm>

#include "spsolver.hpp"
#include "Loop.hpp"
#include "CheckersState.hpp"

int main(int argc, char** argv)
{
        

	using ArrayNd = Eigen::Array<double,1,Eigen::Dynamic>;
	ArrayNd test; 
	size_t N = 5; 
	
	test = ArrayNd::Zero(N);
	test(0) = 42.0f;
	test(1) = 123.0f; 
	test(2) = 4.0f; 
	test(3) = 11.0f; 
	test(4) = -123.0f; 
	
	std::cout << "unsorted array:\n" << test << "\n"; 
	
	auto idx = MC::partial_sort_idx(test.data(),3,N); 
	std::cout << "first 3 indices are " << idx[0] << " " << idx[1] << " " << idx[2] << std::endl;  
		
	return 0; 	
/*
	int num_particles = 8;
        int Nt = 10000;
        int Nx = 10;
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
			num_particles, Nt,T);
        std::cout<<"Bubye"<<"\n";
*/	
}





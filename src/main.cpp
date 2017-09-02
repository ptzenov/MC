#include "Loop.hpp"
#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;

int main(int argc, char** argv){

	int num_particles = 8;
	int Nt = 10000; 
	double T = 10; 

	main_checkers_loop(num_particles,Nt, T);
	std::cout<<"Bubye"<<"\n";

	/**
	  MatrixXd m(2,2);
	  m(0,0) = 3;
	  m(1,0) = 2.5;
	  m(0,1) = -1;
	  m(1,1) = m(1,0) + m(0,1);
	  std::cout << m << std::endl;
	 **/

}


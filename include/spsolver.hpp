#ifndef _SP_SOLVER_HPP_

#define _SP_SOLVER_HPP_

#include <iostream>
#include <math.h> // typical math stuff .. 
#include <vector>

// eigen lib stuff

#include <Eigen/Core> // Matrix and Array classes. Basic lin algebra 
#include <Eigen/LU> // inverse, determinant LU decomposition 
#include <Eigen/Eigenvalues> // eigenval eigenvec decompositions! 


namespace MC{
	// dynamically allocated matrix and array. self destruction after going out of scope!	


	class Material{
		public: 
			double	E_g_eV; 
			double	m_eff_kg; 
			Material(double E_g,double m_eff);						
	};

	struct Layer{
		Material material;
		double thickness_A; 	
		double doping_cm3; 
	};

	void sp_solve(std::vector<Layer>& layers,double Temp, double bias,int nrWF);	
};


#endif

#include "spsolver.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include <complex>

using ArrayNd = Eigen::Array<double,Eigen::Dynamic,1>;
using MatrixNd = Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>; 
using esolverType = Eigen::EigenSolver<MatrixNd>;

using evalType = esolverType::EigenvalueType;
using evecType = esolverType::EigenvectorsType;



void MC::setup_structure(std::vector<MC::Layer> const & layers, MC::StructureInfo& structure, double bias_kV_cm) 
{
	structure.bias_V_m = bias_kV_cm*1e3/1e-2;
	size_t ctr = 0; 
	structure.dz = 1e-10; 	
	for(auto l: layers){
		structure.Lp+=l.thickness_A*1e-10;
		for(int i=0;i<l.thickness_A*1e-10/structure.dz;++i){
			double z_i = ctr*structure.dz; // in meters
			structure.z.push_back(z_i); // z-coordinate
			structure.meff.push_back(l.material.m_eff_kg); // effective masses 
			structure.V0.push_back(MC::e0*l.material.E_g_eV-MC::e0*z_i*structure.bias_V_m); // conduction band profile
			// at each grid point constant doping 	
			structure.nD.push_back(1e6*l.doping_cm3);
			ctr++;
		}
	}
	structure.N = structure.z.size(); 
}


void MC::sp_solve(std::vector<Layer>& layers,double Temp,double bias_kV_cm,int nrWF){
	
	

	StructureInfo si{}; 
	setup_structure(layers,si,bias_kV_cm);

	int N = si.z.size();
	
	MatrixNd H0;
	H0 = MatrixNd::Zero(N,N);// for S.E. 

	MatrixNd M; 
	M =  MatrixNd::Zero(N,N); // for Poisson eqn. 
	
	ArrayNd DV; 
	DV = ArrayNd::Zero(N); 

	esolverType es;

	MatrixNd Psi_z; // wavefunctions  
	Psi_z = MatrixNd::Zero(N,nrWF);
	while(true){

		// hamiltonian discretization ...
		for(int m=0;m<N;m++){
			double c = MC::hbar*MC::hbar/2/si.meff[m]/si.dz/si.dz;
			H0(m,m) = 2*c+(si.V0[m]+DV(m));
			if(m>0)
				H0(m,m-1) = -c;
			if(m<N-1)
				H0(m,m+1) = -c; 		
		}
		
		//calc the eigenvalues
		es.compute(H0);
		evalType evals = es.eigenvalues(); 
		evecType evecs = es.eigenvectors();
			
		auto idx = MC::partial_sort_idx(evals.data(),nrWF,evals.size(),
				[](std::complex<double> a, std::complex<double> b){ return std::abs(a) < std::abs(b);}); 
		// ToDo -> finish up the algoritm
		//1) 
		// get the electron distribution -> fermi-dirac and chemical potential 
		//	
		//2)
		// get the charge density rho
		//
		//3) 
		// solve the poisson equaiton 
		//
		//4)
		// check for convergence
		break;			
	}
}




#include "spsolver.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include <complex>

using ArrayNd = Eigen::Array<double,Eigen::Dynamic,1>;
using MatrixNd = Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>; 
using esolverType = Eigen::EigenSolver<MatrixNd>;

using evalType = esolverType::EigenvalueType;
using evecType = esolverType::EigenvectorsType;



std::vector<int> search_WFs_by_min_real(size_t nrWF, evalType array){
	
	std::vector<int> res(nrWF); 
	return res; 	

}

void MC::sp_solve(std::vector<Layer>& layers,double Temp,double bias_kV_cm,int nrWF){
	
	double Lp = 0.; 
	
	std::vector<double> _V0, _z,_meff,_doping; 

	double bias_V_m = bias_kV_cm*1e3/1e-2;
	double dz = 1e-10;// dz spacing in Angstrom! 


	int ctr = 0; // length counter 
	for(auto l: layers){
	
		Lp+=l.thickness_A*1e-10;
		for(int i=0;i<l.thickness_A*1e-10/dz;++i){
			double z_i = ctr*dz; // in meters
			_z.push_back(z_i); 
			_meff.push_back(l.material.m_eff_kg);
			_V0.push_back(MC::e0*l.material.E_g_eV-MC::e0*z_i*bias_V_m);
			
			_doping.push_back(1e6*l.doping_cm3);
			ctr++;
		}
	}

	int N = _z.size();
	
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
			double c = MC::hbar*MC::hbar/2/_meff[m]/dz/dz;
			H0(m,m) = 2*c+(_V0[m]+DV(m));
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

			
		// get the first nrWF indices
		break;			
	}
}




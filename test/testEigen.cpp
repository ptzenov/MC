#include "../include/Eigen/Core"
#include "../include/Eigen/Eigenvalues"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace Eigen;

typedef Matrix<double,Dynamic,Dynamic> MatrixNd;
typedef Array<double,Dynamic,1> ArrayNd; 

typedef EigenSolver<MatrixNd>::EigenvalueType eValType;
typedef EigenSolver<MatrixNd>::EigenvectorsType eVecType;



std::vector<int> search_WFs_min_real(int nrWF, eValType array){
	
	assert(array.size()>=1);	
	std::vector<int> idx;

	double min = array(0).real();
	int min_idx = 0; 	
	for(int i=1;i<array.size();i++){
		double a = array(i).real(); 
		if (a<min){
			min = a; 
			min_idx = i; 
		}
	}
	idx.push_back(min_idx);
	for(int n=1;n<nrWF;n++){
		min = 1e20;

		for(int i=1;i<array.size();i++){
			double a = array(i).real(); 
			if (a<min && std::find(idx.begin(),idx.end(),i) == idx.end()){
				min = a; 
				min_idx = i; 
			}
		}	
		idx.push_back(min_idx);		
	}

	return idx; 
}


int main(int ac, char** av){


	std::vector<double> v({1.,2.,3.}); 


	int nrWF = 3;
	int N = 6;
	
	ArrayNd arr; 
	arr = ArrayNd::Zero(5); 
	arr(1) = 1; 
	std::cout<< arr;

	MatrixNd m = MatrixNd::Random(N,N);
	MatrixNd m_symm; 
       	m_symm = m+m.transpose();	
	std::cout << "\n"<< m_symm << std::endl;
	EigenSolver<MatrixNd> esolver;
	esolver.compute(m_symm);

	eValType evals = esolver.eigenvalues();
	evals(4) =evals(5);

	eVecType evecs = esolver.eigenvectors();
	std::cout<<std::endl;
	for(int i=0;i<evals.size();i++){
		std::cout<<evals(i).real() << " ";
	}
	std::cout<<std::endl;

	std::vector<int> E = search_WFs_min_real(nrWF,evals);
	
	for(auto e:E){
		std::cout<< e << " ";
	}
	std::cout<<std::endl;

	return 0;
}

#include <subband/subband.hpp>
#include <utils/constants.hpp>
#include <utils/common.hpp>
#include <plot/GNUplotter.hpp>

#include <iostream>
#include <complex>
#include <vector>
#include <assert.h>

#include <Eigen/Core> // Matrix and Array classes. Basic lin algebra 
#include <Eigen/LU> // inverse, determinant LU decompositions 
#include <Eigen/Eigenvalues> // eigenvalue eigenvector decompositions 
#include <Eigen/Sparse> // using sparse matrix & sparse operations

void MC::sp_solve(std::vector<MC::Layer> const & layers,
                  double const Temp, double const bias_V_m,
                  int const nrWF, const double n2D, double dE, double dz)
{


        using SparseMat = Eigen::SparseMatrix<double>;
        using Triplets = Eigen::Triplet<double>;

        double p = 0.15; // percentage of Al for GaAs / ALGaAs
        double CBO = MC::e0 * 0.62 * (1.594 * p +
                                      p * (1 - p) * (0.127 - 1.310 * p)); // conduction band offset in Joule
        double Error = 1;
        int iternr = 1; // iteration number of while-loop
        int N = 0;
        for (size_t i = 0; i < layers.size(); i++)
        {
                N += std::round(layers[i].thickness / dz); // N gives the number of grid points
        };

        VectorNd _z = VectorNd::Zero(N); // "grid vector" with spacing of dz
        VectorNd _V0 = VectorNd::Zero(N); // "energy landscape" - vector in Joule
        VectorNd _meff = VectorNd::Zero(N); // "effective mass" - vector in kg
        VectorNd _doping= VectorNd::Zero(N); // "doping" - vector in m^3
        VectorNd _eps = VectorNd::Zero(N); // "total permittivity" - vector in A*s/V/m

        // translating layers in Eigen::VectorNd vectors
        int ctr = 0; // length counter
        for (auto l : layers) // for-loop for all layers
        {
                for (int it = 0; it < std::round(l.thickness / dz); it++)
                {
                        _z(ctr) = ctr*dz;
                        _V0(ctr) = l.material.VBO + l.material.E_g - e0 * _z(ctr) * bias_V_m;
                        _meff(ctr) = l.material.m_eff;
                        _doping(ctr) = l.doping;
                        _eps(ctr) = l.material.eps;
                        ctr++;
                };
        };

        N = _z.size(); // redundant but just to be sure that N is the real length of _z

        MatrixNd Psi_z = MatrixNd::Zero(N, nrWF);
        MatrixNd Psi_sqt = MatrixNd::Zero(N, nrWF);
        MatrixNd Psi_out = MatrixNd::Zero(N, nrWF);

        VectorNd diag_vec_const; // constant part od diag_vec
        VectorNd diag_vec = VectorNd::Ones(N);
        VectorNd subdiag_vec= VectorNd::Ones(N-1);
        VectorNd E_val_N = VectorNd::Ones(N);
        VectorNd V_z = VectorNd::Zero(N);
        VectorNd E_vec_sqt = VectorNd::Zero(N);
        VectorNd rho = VectorNd::Zero(N);
        VectorNd DV = VectorNd::Zero(N);
        VectorNd E_vals = VectorNd::Zero(nrWF);
        VectorNd ni = VectorNd::Zero(nrWF);
        VectorNd occ = VectorNd::Zero(nrWF);
        VectorNd E_old = VectorNd::Zero(nrWF);
        VectorNd idxWF = VectorNd::Zero(nrWF);

        SparseMat Msp(N, N);

        /****
         *
         * Schrödinger hamiltonian discretization... one part out of loop because of efficiency
         *
         * */
        diag_vec_const = diag_vec.cwiseQuotient(_meff) * hbar * hbar / dz / dz;
        subdiag_vec = -hbar * hbar / 2.0 / dz / dz * subdiag_vec.cwiseQuotient(_meff.topRows(N - 1));

        while ((iternr < 20) && (Error >= dE)) // a maximal value for iterations should be defined
        {

                V_z = _V0 + DV;

                diag_vec = diag_vec_const + V_z;
                // Compute eigenvalues and eigenvectors
                Eigen::SelfAdjointEigenSolver<MatrixNd> es;
                es.computeFromTridiagonal(diag_vec, subdiag_vec);
                assert(!(es.info() && "Success"));
                VectorNd evals = es.eigenvalues();
                MatrixNd evecs = es.eigenvectors();
		
		////  Eigen functions selection criteria

		VectorNd evals_2 {evals}; 
		for(auto j = 0; j< evals.size();++j)
		{	
			VectorNd distr = evecs.col(j).real().cwiseAbs2(); 
			VectorNd integrand = distr.cwiseProduct(V_z);
			
			auto tmp = trapz<double *,double>(_z.data(),_z.data()+_z.size(), 
				integrand.data(), integrand.data()+integrand.size());
			evals_2(j) = evals_2(j)	- tmp;	
		}
		//// now extract the indices of the eigen-energies closest to 
		auto idx = MC::partial_sort_idx(evals_2.data(), nrWF, evals_2.size(),
                                                [](std::complex<double> a, std::complex<double> b)
						{
							return a.real() < b.real();
						}
						);


                // get the first nrWF indices
                for (int i = 0; i < nrWF; i++)
                {
                        idxWF(i) = idx[i];
                };

                // E_old contains eigenvalues of last iteration => needed for error calc. later
                E_old = E_vals;

                // get the #nrWF lowest eigenvalues and put them into a column vector
                for (int i = 0; i < nrWF; i++)
                {
                        E_vals(i) = evals(idxWF(i));
                };


                // get the corresponding eigenvectors (colums of the matrix Psi_z)
                for (int i = 0; i < nrWF; i++)
                {
                        Psi_z.col(i) = evecs.col(idxWF(i)).real(); // maybe .real()
                };

                // normalization of probability density fct Psi²
                for (int col = 0; col < nrWF; col++)
                {
                        E_vec_sqt = Psi_z.col(col).cwiseAbs2();
                        double Trapz = trapz<double*, double>
                                       (_z.data(), _z.data() + _z.size() - 1,
                                        E_vec_sqt.data(), E_vec_sqt.data() + E_vec_sqt.size() - 1);
                        Psi_z.col(col) = Psi_z.col(col) / sqrt(Trapz);
                };

                // will be needed later a few times
                Psi_sqt = Psi_z.cwiseAbs2();

                // Fermi - Dirac
                fermi_dirac(ni, E_vals, _meff, idxWF, Temp, nrWF, dE, n2D);
                occ = ni / n2D;

                // charge density due to doping
                rho = _doping * e0;
                for (int col = 0; col < nrWF; col++)
                {
                        rho = rho - e0*ni(col)*Psi_sqt.col(col);
                }

                // Poisson solver
                std::vector<Triplets> triplet_vec;
                double md = -2.0 / dz / dz;
                double nd = 1.0 / dz / dz;
                // estimated entries in the triplet vector
		// the order of pushes is NOT allowed to change in any way
		// the sparse matrix is built column-major
                triplet_vec.reserve(3 * N - 2);
                triplet_vec.push_back(Triplets(0, 0, md));
                for (int i = 1; i < N; i++)
                {
                        triplet_vec.push_back(Triplets(i, i - 1, nd));
                        triplet_vec.push_back(Triplets(i - 1, i, nd));
                        triplet_vec.push_back(Triplets(i, i, md));
                };
		// generate sparse matrix from triplets
                Msp.setFromTriplets(triplet_vec.begin(), triplet_vec.end());
		// Cholesky Solver
                Eigen::SimplicialCholesky<SparseMat> chol(Msp); // performs a Cholesky factorization of Msp
                DV = chol.solve(rho.cwiseQuotient(_eps) * e0); // use the factorization to solve
                assert(!(chol.info() && "Success"));

                Error = (E_vals - E_old).cwiseAbs().maxCoeff();

                std::cout << std::endl << std::endl;
                std::cout << "iteration " << iternr << 
			" completed. Maximum error (ev): " << Error / e0 << std::endl;
                std::cout << std::endl << std::endl;
                iternr++;

        }; // end of while loop

        std::cout << std::endl << std::endl;
        std::cout << "Schroedinger poisson solver iteration completed! Displaying eigenstates" << std::endl;
        std::cout << std::endl << std::endl;

// gnuplotter main: plot all WF and energy landscape
        GNUPlotter plotter {};
        double sc = CBO / e0 / Psi_sqt.maxCoeff(); // scaling factor for plot
        for (int col = 0; col < nrWF; col++) // Preparing Psi_z for plotting
        {
                Psi_out.col(col) = sc*Psi_sqt.col(col) + E_val_N * E_vals(col) / e0;
        };
        plot_WF(plotter, _z, V_z, nrWF, Psi_out); // plots wavefunctions in scale nm and eV
};







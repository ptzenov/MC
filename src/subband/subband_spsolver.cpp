#include <iostream>
#include <iterator>
#include <algorithm>

#include <subband/subband.hpp>
#include <utils/constants.hpp>
#include <utils/common.hpp>


#include <Eigen/Core> // Matrix and Array classes. Basic lin algebra 
#include <Eigen/Eigenvalues> // eigenvalue eigenvector decompositions 
#include <Eigen/Sparse> // using sparse matrix & sparse operations



std::vector<MC::Subband>
MC::sp_solve(std::vector<MC::Layer> const & layers,  const MC::SimParams& params)
{
        using SparseMat = Eigen::SparseMatrix<double>;
        using Triplets = Eigen::Triplet<double>;

        double Error = 1.0f;
        int iternr = 1; // iteration number of while-loop
        size_t N = std::accumulate(std::begin(layers),std::end(layers),0,[&params]( int& init,
                                   MC::Layer const& layer)
        {
                return init += std::round(layer.thickness/params.dz);
        }) ;
        std::cout << " N is = " << N  << std::endl;

        VectorNd _z = VectorNd::Zero(N); // "grid vector" with spacing of params.dz
        VectorNd _V0 = VectorNd::Zero(N); // "energy landscape" - vector in Joule
        VectorNd _meff = VectorNd::Zero(N); // "effective mass" - vector in kg
        VectorNd _doping= VectorNd::Zero(N); // "doping" - vector in m^3
        VectorNd _eps = VectorNd::Zero(N); // "total permittivity" - vector in A*s/V/m

        // translating layers in Eigen::VectorNd vectors
        int ctr = 0; // length counter

        for (auto l : layers) // for-loop for all layers
        {
                for (int it = 0; it < std::round(l.thickness / params.dz); ++it)
                {
                        _z(ctr) = ctr*params.dz;
                        _V0(ctr) = l.material.VBO + l.material.E_g -
                                   e0 * _z(ctr) * params.bias_V_m;
                        _meff(ctr) = l.material.m_eff;
                        _doping(ctr) = l.doping;
                        _eps(ctr) = l.material.eps;
                        ctr++;
                }
        }

        N = _z.size(); // redundant but just to be sure that N is the real length of _z
        //
        // preallocate some memory for the algorithm variables
        //
        MatrixNd Psi_z = MatrixNd::Zero(N, params.nrWF);
        MatrixNd Psi_sqrt = MatrixNd::Zero(N, params.nrWF);

        VectorNd E_val_N = VectorNd::Ones(N);
        VectorNd V_z = VectorNd::Zero(N);
        VectorNd E_vec_sqt = VectorNd::Zero(N);
        VectorNd rho = VectorNd::Zero(N);
        VectorNd DV = VectorNd::Zero(N);
        VectorNd E_vals = VectorNd::Zero(params.nrWF);
        VectorNd ni = VectorNd::Zero(params.nrWF);
        VectorNd occ = VectorNd::Zero(params.nrWF);
        VectorNd E_old = VectorNd::Zero(params.nrWF);

        //eigenvals and eigenvectors
        Eigen::SelfAdjointEigenSolver<MatrixNd> es;
        VectorNd evals = VectorNd::Zero(N);
        VectorNd evals_prime = VectorNd::Zero(N);
        MatrixNd evecs = MatrixNd::Zero(N,N);

        // save the coefficients in a sparse/symmetric matrix
        VectorNd main_diagonal = VectorNd::Ones(N);
        VectorNd sub_diagonal = VectorNd::Ones(N-1);
        SparseMat Msp(N, N);

        //Schrödinger hamiltonian discretization.
        // This part out of loop because of efficiency

        // Initialize the Hamiltonian's main and sub-diagonal
        main_diagonal(0) = SQR(MC::hbar)/_meff(0)/(SQR(params.dz));
        for(auto  _idx = 1U; _idx < N; ++_idx)
        {
                auto m_negative = (_meff(_idx)+_meff(_idx-1))/2.0;
                auto m_postitive = (_meff(_idx)+_meff(_idx+1 == N ? N-1:_idx+1))/2.0;
                main_diagonal(_idx) = SQR(MC::hbar)/2.0*(1.0/m_negative
                                      +1.0/m_postitive)/SQR(params.dz);
                sub_diagonal(_idx-1) = -(SQR(hbar)/2.0)*(1.0/m_negative)/SQR(params.dz);
        }

        while ((iternr < 20) && (Error >= params.dE)) // a maximal value for iterations should be defined
        {

                V_z = _V0 + DV;

                // Compute eigenvalues and eigenvectors
                es.computeFromTridiagonal(main_diagonal+V_z, sub_diagonal);
                assert(!(es.info() && "Success"));
                evals = es.eigenvalues();
                evecs = es.eigenvectors();

                ////  Eigen functions selection criteria
                // - renormalize the evals wrt their degree of "boundedness"
                evals_prime  = evals;
                for(auto j = 0; j< evals.size(); ++j)
                {
                        VectorNd distr = evecs.col(j).real().cwiseAbs2();
                        VectorNd integrand = distr.cwiseProduct(V_z);

                        auto tmp = trapz(_z.data(),_z.data()+_z.size(),
                                         integrand.data(), integrand.data()+integrand.size());
                        evals_prime(j) = evals_prime(j)	- tmp;
                }
                //// now extract the indices of the eigen-energies closest to
                auto wf_idx = MC::partial_sort_idx(evals_prime.data(), params.nrWF, evals_prime.size(),
                                                   [](std::complex<double> a, std::complex<double> b)
                {
                        return a.real() < b.real();
                }
                                                  );

                // E_old contains eigenvalues of last iteration => needed for error calc. later
                E_old = E_vals;

                // get the #params.nrWF lowest eigenvalues and put them into a column vector
                for (int i = 0; i < params.nrWF; i++)
                {
                        E_vals(i) = evals(wf_idx[i]);
                }


                // get the corresponding eigenvectors (colums of the matrix Psi_z)
                for (int i = 0; i < params.nrWF; i++)
                {
                        Psi_z.col(i) = evecs.col(wf_idx[i]).real(); // maybe .real()
                }
                Psi_sqrt = Psi_z.cwiseAbs2();

                // normalization of probability density fct Psi²
                for (int col = 0; col < params.nrWF; col++)
                {
                        auto Trapz = trapz(_z.data(), _z.data() + _z.size(),
                                           Psi_sqrt.col(col).data(),
                                           Psi_sqrt.col(col).data() + Psi_sqrt.col(col).size());
                        Psi_z.col(col) = Psi_z.col(col) / sqrt(Trapz);
                }

                // will be needed later a few times
                Psi_sqrt = Psi_z.cwiseAbs2();

                // Fermi - Dirac
                fermi_dirac(Psi_sqrt, E_vals, _meff, params);
                occ = ni / params.n2D;

                // charge density due to doping
                rho = _doping * e0;
                for (int col = 0; col < params.nrWF; col++)
                {
                        rho = rho - e0*ni(col)*Psi_sqrt.col(col);
                }

                // Poisson solver
                std::vector<Triplets> triplet_vec;
                double md = -2.0 / params.dz / params.dz;
                double nd = 1.0 / params.dz / params.dz;
                // estimated entries in the triplet vector
                // the order of pushes is NOT allowed to change in any way
                // the sparse matrix is built column-major
                triplet_vec.reserve(3 * N - 2);
                triplet_vec.push_back(Triplets(0, 0, md));
                for (auto i = 1U; i < N; ++i)
                {
                        triplet_vec.push_back(Triplets(i, i - 1, nd));
                        triplet_vec.push_back(Triplets(i - 1, i, nd));
                        triplet_vec.push_back(Triplets(i, i, md));
                }
                // generate sparse matrix from triplets
                Msp.setFromTriplets(triplet_vec.begin(), triplet_vec.end());
                // Cholesky Solver
                Eigen::SimplicialCholesky<SparseMat> chol(Msp); // performs a Cholesky factorization of Msp
                DV = chol.solve(rho.cwiseQuotient(_eps) * e0); // use the factorization to solve
                assert(!(chol.info() && "Success"));

                Error = (E_vals - E_old).cwiseAbs().maxCoeff();

                std::cout << "iteration " << iternr <<
                          " completed. Maximum error (ev): " << Error / e0 << std::endl;
                iternr++;
        } // end of while loop

        std::cout << "Schroedinger poisson solver iteration completed! Displaying eigenstates" << std::endl;

        // Plot the wavefucntions with GNUplot
        {
                GNUPlotter plotter {};
                double sc = 1e-9f ;
                for (int col = 0; col < params.nrWF; col++) // Preparing Psi_z for plotting
                {
                        Psi_sqrt.col(col) = sc*Psi_sqrt.col(col) + E_val_N * E_vals(col) / e0;
                };
                plot_WF(plotter, _z, V_z, params.nrWF, Psi_sqrt);
        }
        // Now transform them into subbands and return
        // inner product binary operations op1, and op2,
        // using op1_type = Ret(*)(const Type1 &a, const Type2 &b);
        // using op2_type = Ret(*)(const Type1 &a, const Type2 &b);
        // acc = op1(acc, op2(*first1, *first2))
        auto op1 = [&params](double const & acc, double const & b )
        {
                return acc+params.dz*b;
        };
        auto op2 = [](double const & mass, double const & WF)
        {
                return mass*SQR(WF);
        };

        // need to COPY the EIGEN data to our own heap structure since EIGEN frees its memory!
        double * z_on_heap = new double[N];
        std::copy(_z.data(),_z.data()+N,z_on_heap);
        MC::custom_shared_ptr<double> shared_z {z_on_heap,N};

        std::vector<MC::Subband>	subbands;
        for (auto col = 0U; col < Psi_z.cols(); ++col)
        {
                double * Psi_on_heap = new double[N];
                std::copy(Psi_z.col(col).data(), Psi_z.col(col).data()+N,Psi_on_heap);

                double effmass = std::inner_product(_meff.data(),_meff.data()+N,
                                                    Psi_on_heap,0,op1,op2);
                double centroid = std::inner_product(std::begin(shared_z),std::end(shared_z),
                                                     Psi_on_heap,0,op1,op2);
                // 2x subband inplace constructions + 2 move constr + 2x deletes
                subbands.push_back(
                        MC::Subband {shared_z, Psi_on_heap,N,E_vals(col),centroid,effmass}
                );

        }
        return subbands;
}





/**
 * file: subband/subband.hpp
 * Incorporates the main classes and util functions for storing and calculating subband
 * states
 */
#ifndef _SUBBAND_HPP_
#define _SUBBAND_HPP_

// internal headers
#include <state.hpp>
#include <utils/common.hpp>
#include <utils/constants.hpp>
#include <plot/GNUplotter.hpp>

#include <memory>

#include <vector>

// eigen lib stuff
#include <Eigen/Core> // Matrix and Array classes. Basic lin algebra 
#include <Eigen/LU> // inverse, determinant LU decomposition 
#include <Eigen/Eigenvalues> // eigenval eigenvec decompositions! 


using MatrixNd = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
using VectorNd = Eigen::Matrix<double, Eigen::Dynamic, 1>;

namespace MC
{

class SubbandState: public AbstractState
{
private:
        // C_TYPE  -> complex, R_TYPE -> real
        std::unique_ptr<C_TYPE []> Phi_z; // wave-funciton
        std::unique_ptr<R_TYPE []> z; // z-grid
        // wave vector in the transverse direction
        double k_x;
        double k_y;
        double E; // eigen-energy
        double m_eff; // effective mass - weighted average!
public:
        SubbandState(std::vector<double> z, double E_z,
                     std::vector<C_TYPE> Phi_z,
                     double k_x, double k_y,
                     double m_eff); // constructor
};


/************** Schroedinger-Poisson solver *******************/ 

class Material
{
public:
        double	E_g;	// band gap in J = kg*m*m/s/s
        double  VBO;	// valence band offset in J = kg*m*m/s/s  //w.r.t. GaN
        double	m_eff;	// effective mass in kg
        double  eps;	// absolute permittivity in A*s/V/m
};

struct Layer
{
        Layer(Material mat, double thick, double dope)
                : material(mat), thickness(thick), doping(dope) {};
        Material material;
        double thickness;							// thickness in m
        double doping;								// doping in m^3
};

void sp_solve(std::vector<MC::Layer> const & layers, double const Temp_K,
              double const bias_V_m, int const nrWF, const double n2D,
              double dE = 1.0e-10 * e0, double dz = 1.0e-10);

/** 
 * Utility functions  
 */
void material_properties(MC::Material &mat, double const E_g_Joule,
                         double const VBO_Joule, double const meff_kg, const double abs_permittivity);

void fermi_dirac(VectorNd &ni, VectorNd const &E_vals, VectorNd const &_meff,
	       	VectorNd const &idxWF, const double Temp, const int nrWF, const double dE, const double n2D);

void plot_WF(GNUPlotter &plotter, VectorNd const &_z, VectorNd const &V_z,
             const int nrWF, MatrixNd const &Psi_z_out);




};



#endif




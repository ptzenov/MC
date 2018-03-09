/**
 * file: subband/subband.hpp
 * Incorporates the main classes and util functions for storing and calculating subband states
 */
#ifndef _SUBBAND_HPP_
#define _SUBBAND_HPP_

// internal headers
#include <state.hpp>
#include <iostream>

#include <utils/memory.hpp>
#include <plot/GNUplotter.hpp>

#include <vector>

// eigen lib stuff
#include <Eigen/Core> // Matrix and Array classes. Basic lin algebra 

using MatrixNd = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
using VectorNd = Eigen::Matrix<double, Eigen::Dynamic, 1>;

namespace MC
{

//is it ok to be double ?
class Subband
{
private:
        // double -> real
        // !!! Make const to prevent modifications on the shared resource!
        MC::custom_shared_ptr<const double> _z; // z-grid
        MC::custom_shared_ptr<const double> _phi; // wave-funciton

        // number of grid points in z-direction
        size_t _Npts;
        // subband eigen-energy
        double _E;
        // centroid (coordinate) of the WF
        double _centroid;
        // weighted average of the subband effective mass parallel to the growth direction
        double _meff;

public:
        Subband(MC::custom_shared_ptr<const double> z,
                MC::custom_shared_ptr<const double> phi, size_t N,
                double E, double centroid, double meff);
        Subband(double* z, double* phi, size_t N, double E, double centroid,
                double meff);

        // provide mixed constructors
        Subband(double* z, MC::custom_shared_ptr<const double> phi,
                size_t N, double E, double centroid, double meff);
        Subband(MC::custom_shared_ptr<const double> z, double* phi,
                size_t N, double E, double centroid, double meff);

        Subband() =  delete;


        // copy constructors!
        Subband(const Subband & other) = default;
        Subband(Subband&& other)= default;

        Subband& operator=(const Subband & other) = default;
        Subband& operator=(Subband&& other)= default ;
        ~Subband()
        {
                ;
        }

        MC::custom_shared_ptr<const double> const & PHI() const ;
	MC::custom_shared_ptr<const double> const & z() const ;
 
 	double & E();
        double & centroid();
        double & meff();
};

class SubbandState:public AbstractState
{
private:
        MC::Subband const & _subband;
        double _kx;
        double _ky;
        double _meff_xy;
        size_t _idx;
public:
        explicit SubbandState(MC::Subband& subband,
                              double kx, double ky, double meff_xy, size_t idx);

        SubbandState() = delete;
        SubbandState(SubbandState& other) = default;
        SubbandState(SubbandState&& other) = default;
        SubbandState& operator=(SubbandState& other) = default;
        SubbandState& operator=(SubbandState&& other) = default;
        ~SubbandState()
        {
                ;
        }
        
	size_t idx() const;
        double kx() const;
        double ky() const;
        double meff_xy() const;
        MC::Subband const & subband();
};

/************** Subband scattering mechanisms *****************/
/*********
 *
 * LO-phonon scatterrer
 *
 *********/
class LOphononScatterer
{
private:
        // LO-phonon energy
        double _E_LO;
        std::unique_ptr<double []> rates; // the precomputed rates
public:
        LOphononScatterer(std::vector<MC::SubbandState> const & states,
                          double E_LO);
        bool operator()(MC::SubbandState&, MC::SubbandState&, size_t t);
};

/*********
 *
 * IR scatterrer
 *
 *********/

// TODO



/*********
 *
 * Impurity scatterrer
 *
 *********/

// TODO


/*********
 *
 * LA-phonon scatterrer
 *
 *********/

// TODO


/*********
 *
 * e-e scatterrer
 *
 *********/

// TODO


/*********
 *
 * optical-field scatterrer
 *
 *********/

// TODO



/*********
 *
 * resonant-tunneling  scatterrer
 *
 *********/

// TODO



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

struct SimParams
{
        double Temp_K;
        double bias_V_m;
        int nrWF;
        double n2D;
        double dE;
        double dz;
};

std::vector<MC::Subband> sp_solve(std::vector<MC::Layer> const & layers,
                                  const SimParams& params);

/**
 * Utility functions
 */
void material_properties(MC::Material &mat, double const E_g_Joule,
                         double const VBO_Joule, double const meff_kg, const double abs_permittivity);

void fermi_dirac(MatrixNd const & Psi_sqrt, VectorNd const &E_vals,
                 VectorNd const & meff,SimParams const & params);

void plot_WF(GNUPlotter &plotter, VectorNd const &_z, VectorNd const &V_z,
             const int nrWF, MatrixNd const &Psi_z_out);
};
#endif










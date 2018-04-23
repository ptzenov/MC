/**
 * file: subband/subband.hpp
 * Incorporates the main classes and util functions for storing and calculating subband states
 */
#ifndef _SUBBAND_HPP_
#define _SUBBAND_HPP_
// internal headers
#include <iostream>



#include <utils/memory.hpp>
#include <utils/numeric.hpp>

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
        MC::shared_array_ptr<const double> _z; // z-grid
        MC::shared_array_ptr<const double> _phi; // wave-funciton
// number of grid points in z-direction
        unsigned int _Npts;
// subband eigen-energy
        double _E;
// centroid (coordinate) of the WF
        double _centroid;
// weighted average of the subband effective mass parallel to the growth direction
        double _meff;

public:
	
        Subband() =  delete;
// construc subband from shared pointers
	Subband(MC::shared_array_ptr<const double> z,
                MC::shared_array_ptr<const double> phi, unsigned int N,
                double E, double centroid, double meff);
// construct subband from raw pointers
        Subband(double* z, double* phi, unsigned int N, double E, double centroid,
                double meff);
// provide raw_ptr/shared_ptr constructor
        Subband(double* z, MC::shared_array_ptr<const double> phi,
                unsigned int N, double E, double centroid, double meff);
// provide shared_ptr/raw_ptr constructor
        Subband(MC::shared_array_ptr<const double> z, double* phi,
                unsigned int N, double E, double centroid, double meff);

// copy constructors!
        Subband(const Subband & other) = default;
        Subband(Subband&& other)= default;

        Subband& operator=(const Subband & other) = default;
        Subband& operator=(Subband&& other)= default ;
        ~Subband();	
// getter for the subband wave function
        MC::shared_array_ptr<const double> const & PHI() const ;
// getter for the growth axis coordinate
      	MC::shared_array_ptr<const double> const & z() const ;
// getter for the subband energy
        double E() const
        {
                return _E;
        }
// getter for the subband centroid 
        double centroid() const
        {
                return _centroid;
        }
//getter for the effective mass in growth direction
        double meff() const
        {
                return _meff;
        }
};

class SubbandState
{
private:
        MC::Subband const & _subband;
        double _kx;
        double _ky;
        double _meff_xy;
        unsigned int _idx;
public:
// constructor of a subband state
        explicit SubbandState(MC::Subband& subband,
                              double kx, double ky, double meff_xy, unsigned int idx);

        SubbandState() = delete;
        SubbandState(SubbandState& other) = default;
        SubbandState(SubbandState&& other) = default;
        SubbandState& operator=(SubbandState& other) = default;
        SubbandState& operator=(SubbandState&& other) = default;
        ~SubbandState(){;}
// return the subband index
        unsigned int idx() const;
// return momentum in x-direction
        double kx() const;
// return momentum in y-direction
        double ky() const;
// return effective mass in transverse direction
        double meff_xy() const;
        MC::Subband const & subband();
// getters for the subband state energy,
// the kinetiv energy and the total energy (subband+kinetic)
        double E_subband() const;
        double E_kin() const;
        double E_tot() const;
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
        std::unique_ptr<double []> _rates_em; // the precomputed rates - LO-phonon emission
        std::unique_ptr<double []> _rates_abs; // the precomputed rates - LO-phonon absorption
public:
//Constructor for the LO-phonon scatterer
// @param states: all possible subband states
// @param E_LO: longitudinal optical phonon energy
        LOphononScatterer(std::vector<MC::SubbandState> const & states,
                          double E_LO);
// performs the scattering between the init and final states
        bool operator()(MC::SubbandState& init, MC::SubbandState& fin, unsigned int t);
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











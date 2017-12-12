/** 
 * file: spsolver.hpp
 *
 * Main header declaring the Schrodinger Poisson solver-related helper classes
 *  and  routines. Must be refactored.   
 *
 */ 
#ifndef _SP_SOLVER_HPP_
#define _SP_SOLVER_HPP_

#include <vector>

// eigen lib stuff
#include <Eigen/Core> // Matrix and Array classes. Basic lin algebra 
#include <Eigen/LU> // inverse, determinant LU decomposition 
#include <Eigen/Eigenvalues> // eigenval eigenvec decompositions! 


namespace MC
{

struct Material
{
        double	E_g_eV;
        double	m_eff_kg;
        Material(double E_g,double m_eff);
};
struct Layer
{
        Material material;
        double thickness_A;
        double doping_cm3;
};

struct StructureInfo
{
        double Lp;
	double bias_V_m;
	double dz; 
	size_t N;
	std::vector<double> V0; // conduction band profile
        std::vector<double> z; // grid
        std::vector<double> meff; // effective mass
        std::vector<double> nD; // nD(z) donor doping density in m^3;
        std::vector<double> nA; // nA(z) acceptor doping density in m^3;
	
	StructureInfo(): Lp{0}, bias_V_m{0}, dz{0}, N{0}, 
		V0(0), z(0), meff(0), nD(0), nA(0)
	{;}

};
void sp_solve(std::vector<Layer>& layers,double Temp, double bias,int nrWF);

void setup_structure(std::vector<Layer> const &, MC::StructureInfo&, double);

};
#endif


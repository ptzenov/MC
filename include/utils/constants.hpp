/**
 * file: constants.hpp
 * Fundamental physical constants (in SI units) useful for our simulations
 *
 */
#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

namespace MC
{

/********** physics constants **********/
// free space permittivity [F/m]
extern const double 	eps0;
// free space permeability [H/m]
extern const double 	mu0;
// elementary charge [C] == [A s]
extern const double 	e0;
// plank's constant	[J s]
extern const double	hbar;
// electron rest mass   [g]
extern const double	me;
//Boltmann constant     [J/K]
extern const double	kB;
// velocity of light in vacuum [m/s]
extern const double	c;

/********** math constants **********/
// pi
extern const double	pi;
// neper number
extern const double	e;
}

#endif


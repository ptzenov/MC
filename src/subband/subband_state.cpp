#include <subband/subband.hpp>
#include <utils/common.hpp>

MC::SubbandState::SubbandState(MC::custom_shared_ptr<const double> z,
                               MC::custom_shared_ptr<const double> phi, size_t Npts,
                               double E, double centroid, double meff): _z(z), _phi(phi),
        _Npts(Npts), _E(E),  _centroid(centroid), _meff(meff)
{
}


MC::SubbandState::SubbandState(double* z, double* phi, size_t Npts,
                               double E, double centroid, double meff):
        _z(z,Npts), _phi(phi,Npts), _Npts(Npts), _E(E),  _centroid(centroid), _meff(meff)
{
}

MC::SubbandState::SubbandState(double* z, MC::custom_shared_ptr<const double> phi,
                               size_t Npts, double E, double centroid, double meff):_z(z,Npts),
	_phi(phi),_Npts(Npts),_E(E),_centroid(centroid), _meff(meff)
{
        ;
}
MC::SubbandState::SubbandState(MC::custom_shared_ptr<const double> z, double* phi,
                               size_t Npts, double E, double centroid, double meff): _z(z),
       	_phi(phi,Npts), _Npts(Npts), _E(E), _centroid(centroid),_meff(meff)
{
        ;
}

MC::custom_shared_ptr<const double>  & MC::SubbandState::PHI()
{
        return _phi;
}
MC::custom_shared_ptr<const double>  & MC::SubbandState::z()
{
        return _z;
}
double & MC::SubbandState::E()
{
        return _E;
}
double & MC::SubbandState::centroid()
{
        return _centroid;
}
double & MC::SubbandState::meff()
{
        return _meff;
}






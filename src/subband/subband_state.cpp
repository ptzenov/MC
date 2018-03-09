#include <subband/subband.hpp>
#include <utils/common.hpp>

MC::Subband::Subband(MC::custom_shared_ptr<const double> z,
                     MC::custom_shared_ptr<const double> phi, size_t Npts,
                     double E, double centroid, double meff): _z(z), _phi(phi), _Npts(Npts),
        _E(E),  _centroid(centroid), _meff(meff)
{
        ;
}


MC::Subband::Subband(double* z, double* phi, size_t Npts,
                     double E, double centroid, double meff):
        _z(z,Npts), _phi(phi,Npts), _Npts(Npts), _E(E),  _centroid(centroid), _meff(meff)
{
        ;
}

MC::Subband::Subband(double* z, MC::custom_shared_ptr<const double> phi,
                     size_t Npts, double E, double centroid, double meff):_z(z,Npts),
        _phi(phi),_Npts(Npts),_E(E),_centroid(centroid), _meff(meff)
{
        ;
}
MC::Subband::Subband(MC::custom_shared_ptr<const double> z, double* phi,
                     size_t Npts, double E, double centroid, double meff): _z(z),
        _phi(phi,Npts), _Npts(Npts), _E(E), _centroid(centroid),_meff(meff)
{
        ;
}

MC::custom_shared_ptr<const double> const & MC::Subband::PHI() const
{
        return _phi;
}
MC::custom_shared_ptr<const double> const & MC::Subband::z() const
{
        return _z;
}
double & MC::Subband::E()
{
        return _E;
}
double & MC::Subband::centroid()
{
        return _centroid;
}
double & MC::Subband::meff()
{
        return _meff;
}


/********************** SubbandState ***********************/
MC::SubbandState::SubbandState(MC::Subband& subband,
                               double kx, double ky, double meff_xy, size_t idx): _subband(subband),
        _kx(kx),_ky(ky),_meff_xy(meff_xy), _idx(idx)
{
        ;
}

size_t MC::SubbandState::idx() const
{
        return _idx;
}

double MC::SubbandState::kx() const
{
        return _ky;
}

double MC::SubbandState::ky() const
{
        return _ky;
}

double MC::SubbandState::meff_xy() const
{
        return _meff_xy;
}

MC::Subband const & MC::SubbandState::subband()
{
        return _subband;
}




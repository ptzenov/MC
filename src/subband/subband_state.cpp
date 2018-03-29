#include <subband/subband.hpp>

// destructor
MC::Subband::~Subband()
{
        ;
}

/********** various constructors **********/

MC::Subband::Subband(MC::shared_array_ptr<const double> z,
                     MC::shared_array_ptr<const double> phi, unsigned int Npts,
                     double E, double centroid, double meff): _z(z), _phi(phi), _Npts(Npts),
        _E(E),  _centroid(centroid), _meff(meff)
{
        ;
}


MC::Subband::Subband(double* z, double* phi, unsigned int Npts,
                     double E, double centroid, double meff):
        _z(z,Npts), _phi(phi,Npts), _Npts(Npts), _E(E),  _centroid(centroid), _meff(meff)
{
        ;
}

MC::Subband::Subband(double* z, MC::shared_array_ptr<const double> phi,
                     unsigned int Npts, double E, double centroid, double meff):_z(z,Npts),
        _phi(phi),_Npts(Npts),_E(E),_centroid(centroid), _meff(meff)
{
        ;
}
MC::Subband::Subband(MC::shared_array_ptr<const double> z, double* phi,
                     unsigned int Npts, double E, double centroid, double meff): _z(z),
        _phi(phi,Npts), _Npts(Npts), _E(E), _centroid(centroid),_meff(meff)
{
        ;
}

MC::shared_array_ptr<const double> const & MC::Subband::PHI() const
{
        return _phi;
}
MC::shared_array_ptr<const double> const & MC::Subband::z() const
{
        return _z;
}


/******************** SubbandState ********************/

MC::SubbandState::SubbandState(MC::Subband& subband,
                               double kx, double ky, double meff_xy, unsigned int idx): _subband(subband),
        _kx(kx),_ky(ky),_meff_xy(meff_xy), _idx(idx)
{
        ;
}

unsigned int MC::SubbandState::idx() const
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
/******************** End SubbandState ********************/


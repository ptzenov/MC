#include <subband/subband.hpp>
#include <utils/constants.hpp>
#include <utils/common.hpp>

MC::LOphononScatterer::LOphononScatterer(std::vector<MC::SubbandState> const & states,
                double E_LO): _E_LO(E_LO),
        _rates_em {new double[(states.size()* states.size())]},	_rates_abs {new double[(states.size()* states.size())]}
{
;
}

bool MC::LOphononScatterer::operator()(MC::SubbandState& init, MC::SubbandState& fin, unsigned int t)
{
        return false;
}





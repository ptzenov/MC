#include <subband/subband.hpp>

MC::LOphononScatterer::LOphononScatterer(std::vector<MC::SubbandState> const & states,
                double E_LO): _E_LO(E_LO),
        rates {new double[(states.size()* states.size())]}
{
        // TODO -> precompute all scattring rates
        for(auto & state_i: states)
        {
                for(auto & state_f: states)
		{
                        // compute the integral here!
                        // rate = LOscatter
                        rates[state_f.idx()+state_i.idx()*states.size()] = 0.0;
                }
        }

}

bool MC::LOphononScatterer::operator()(MC::SubbandState&, MC::SubbandState&, size_t t)
{
	return false;
}



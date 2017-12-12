#ifndef _SUBBAND_STATE_HPP_
#define _SUBBAND_STATE_HPP_

#include "State.hpp"
#include "utils.hpp"

#include <memory>

namespace MC{

class SubbandState: public AbstractState{

	private: 
		
		std::unique_ptr<C_TYPE> Phi_z; 
		std::unique_ptr<R_TYPE> z; 

	        double k_x; 
		double k_y;
		double E_z; 
		double m_eff; 		
	public: 
		SubbandState(std::vector<double> z, double E_z,
			       	std::vector<C_TYPE> Phi_z,
			       	double k_x, double k_y,
				double m_eff);
		
};

};

#endif

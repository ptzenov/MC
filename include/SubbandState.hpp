#ifndef _SUBBAND_STATE_HPP_
#define _SUBBAND_STATE_HPP_

#include "State.hpp"
#include "utils.hpp"

namespace MC{

class SubbandState: public AbstractState{

	private: 
		vector<C_TYPE> Phi_z; 
		vector<R_TYPE> z;
	        double k_x; 
		double k_y;
		double E_z; 
		double m_eff; 		
	public: 
		SubbandState(vector<double> z, double E_z,
			       	vector<C_TYPE> Phi_z,
			       	double k_x, double k_y,
				double m_eff);
		
};

};

#endif

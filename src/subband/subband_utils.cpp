#include <subband/subband.hpp>
#include <utils/constants.hpp>
#include <plot/GNUplotter.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

void MC::material_properties(MC::Material &mat, double const E_g_Joule,
                             double const VBO_Joule, double const meff_kg,
                             const double abs_permittivity)
{
        mat.E_g = E_g_Joule;
        mat.VBO = VBO_Joule;
        mat.m_eff = meff_kg;
        mat.eps = abs_permittivity;
};

void MC::plot_WF(GNUPlotter  &plotter, VectorNd const &_z, VectorNd const &V_z,
                 const int nrWF, MatrixNd const &Psi_z_out)
{

        VectorNd Z_out = _z * 1e9;	// for grid plots in nm
        VectorNd V_z_out = V_z / MC::e0;
        int N = Z_out.size();
        std::vector<double> z_out(Z_out.data(), Z_out.data() + Z_out.size());
        std::vector<double> v_z(V_z_out.data(), V_z_out.data() + V_z_out.size());

        plotter.hold_on();
	plotter.set_line_width(1,1); 
	plotter.set_line_color("#000000",1);
        plotter.plot(z_out.begin(), v_z.begin(), N, 1);	// plot of energy landscape v_z (in eV) over z_out in nm

        for (int it = 0; it < nrWF; it++) // plot of wavefunctions
        {
                std::vector<double> evec_out(Psi_z_out.col(it).data(),
                                             Psi_z_out.col(it).data() + Psi_z_out.col(it).size());
		plotter.set_line_width(2,it+2);
		plotter.set_line_color("#FF0000",it+2);
		plotter.plot(z_out.begin(), evec_out.begin(), N, it + 2);
        };
        plotter.hold_off();

};


void MC::fermi_dirac(MatrixNd const & Psi_sqrt, VectorNd const &E_vals, 
		VectorNd const & meff, SimParams const & params)
{
        double a = 0;
        double Ea = E_vals.minCoeff();
        double fac = MC::kB*params.Temp_K/ M_PI / MC::hbar / MC::hbar;

	std::vector<double> occupation(params.nrWF); 
	std::vector<double> wf_meff(params.nrWF); 
	
	auto get_occupation = [&](const int i, double mu){
		return  wf_meff[i] * fac * std::log(1 + std::exp((mu - E_vals(i)) / MC::kB / params.Temp_K));
		};
       	
	for(int i = 0 ; i<params.nrWF;++i)
	{	
		wf_meff[i] = Psi_sqrt.col(i).cwiseProduct(meff).sum();	
	}
	while (a >= 0)
        {
		Ea = Ea - 0.1*e0;
                for (int i = 0; i < params.nrWF; i++)
                {
                        occupation[i] = get_occupation(i,Ea);
		}
                a = std::accumulate(std::begin(occupation),std::end(occupation),0.0) / params.n2D - 1;
        }

        double b = 0;
        double Eb = E_vals.minCoeff();
        while (b <= 0)
        {
                Eb = Eb + 0.1*e0;
                for (int i = 0; i < params.nrWF; i++)
                {
                        occupation[i] = get_occupation(i,Eb); 
                }
                b = std::accumulate(std::begin(occupation),std::end(occupation),0.0) / params.n2D - 1;
        }

        if (a*b < 0)
        {
                double mu = 0.;
                double bh = 0.;
                while (Eb - Ea > params.dE)
                {
                        mu = 0.5*(Ea + Eb);
                        for (int i = 0; i < params.nrWF; i++)
                        {
                                occupation[i] =get_occupation(i,mu);
                        }
                        bh = std::accumulate(std::begin(occupation),std::end(occupation),0.0) / params.n2D - 1;
                        if (bh*b < 0)
                        {
                                a = bh;
                                Ea = mu;
                        }
                        else
                        {
                                b = bh;
                                Eb = mu;
                        }
                }
        }
}









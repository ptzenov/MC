#ifndef _GNUPLOTTER_HPP_
#define _GNUPLOTTER_HPP_

#include <stdlib.h> // system()

// streams
#include <fstream> // ofstream 
#include <sstream>

//containers
#include <string>
#include <map>
#include <vector>

/**
 * Plot axis instance
 */
class Axis
{
private:
        const char* _filename; // plot datafile name
        int _index; // index from the plot datafile
        int _line_id; // line identifier for the current plot
        const char* _specifier; // line specifier , e.g. lines, points, linespoints etc...
public:
        Axis(const char* filename,
             int index,
             int line_id = 1,
             const char* specifier = "lines"):
                _filename(filename),
                _index(index),
                _line_id(line_id),
                _specifier(specifier)
        {
                ;
        }

        std::string get_plot_cmnd()
        {
                std::stringstream ss;
                ss<< " '" << _filename << "' index "<< _index << " linestyle " << _line_id << " with " << _specifier;
                return ss.str();
        }
        ~Axis()
        {
                ;
        }

};

/**
 * A class encapsulating basic plot routines for the gnuplot command line util.
 **/
class GNUPlotter
{

private:
        const char* command_filename;
        const char* data_filename;
        const char* line_specifier;

        std::map<size_t, double> lw_map; // line width map
        std::map<size_t, const char* > lcolor_map; // line color map

        std::vector<Axis> plot_axes;
        bool hold;
        int data_ctr;

        void plot_all()
        {
                apply_settings();
                std::ofstream file {};
                file.open(command_filename,std::ios_base::app);

                file << "plot ";
                int ctr = 0;
                for (auto it: plot_axes)
                {
                        if (ctr++ == 0)
                                file << it.get_plot_cmnd();
                        else
                                file << ", " << it.get_plot_cmnd();
                }
                file.flush();
                file.close();

                std::string command = std::string {"gnuplot -p "} + command_filename;
                system(command.c_str());
        }

        template<typename ForwardIt>
        void output_data(ForwardIt x, ForwardIt y, size_t N,
                         std::ios_base::openmode mode = std::ios_base::out)
        {
                std::ofstream file;
                file.open (data_filename, mode); // open file for writing;
                for( size_t i = 0; i < N; ++i)
                {
                        file << x[i] << " " << y[i] << std::endl;
                }
                file << std::endl << std::endl;
                file.flush();
                file.close();
        }

	template<typename ForwardIt>
        void output_data(ForwardIt data, size_t dim, size_t N,
                         std::ios_base::openmode mode = std::ios_base::out)
        {
                std::ofstream file;
                file.open (data_filename, mode); // open file for writing;
                for( size_t i = 0; i < N; ++i)
                {
			for (size_t d = 0 ; d < dim;++d)
				file << data[i*dim+d] << " ";
			file << std::endl;
                }
                file << std::endl << std::endl;
                file.flush();
                file.close();
        }


        void apply_settings()
        {
                std::ofstream sfile {};
                sfile.open(command_filename,std::ios_base::out);
                for( auto it : lw_map )
                {
                        sfile << "set style line "<< it.first << " linetype 1 linewidth " << it.second << std::endl;
                }
                for( auto it : lcolor_map )
                {
                        sfile << "set style line "<< it.first << " linetype 1 linecolor rgb '" << it.second <<"'"<< std::endl;
                }
                sfile.flush();
                sfile.close();
        }

        void clear()
        {
                std::remove(data_filename);
                std::remove(command_filename);
                plot_axes.clear();
                data_ctr = 0;
        }
public:
        GNUPlotter(
                const char* command_file = "do_plot.gnu",
                const char* data_file = "data.dat",
                const char* l_specifier = "lines"):
                command_filename(command_file),
                data_filename(data_file),
                line_specifier(l_specifier),
                lw_map(), lcolor_map(), hold {false}, data_ctr(0)
        {
                clear();
        }

        /**
         * set the linewidth of the line specified by the corresponding style number
         **/
        void set_line_width(double linewidth, size_t line_id = 1 )
        {
                lw_map[line_id] = linewidth;
        }
        /**
         * set plot line type:
         * "lines"
         * "points"
         * "linespoints"
         *
         */
        void set_line_specifier(char const * type)
        {
                line_specifier = type;
        }
        /**
         * set the line color of the line specified by the corresponding linetype
         **/
        void set_line_color(const char* linecolor, size_t line_id = 1)
        {
                lcolor_map[line_id] = linecolor;
        }


        template <class ForwardIt >
        void plot(ForwardIt x, ForwardIt y, size_t N, size_t line_id = 1)
        {
                plot_axes.push_back( Axis(data_filename,data_ctr++,line_id,line_specifier));
                if(hold)
                {
                        output_data(x,y,N,std::ios_base::app);
                }
                else
                {
                        output_data(x,y,N,std::ios_base::out);
                        plot_all();
                        clear();
                }
        }
	template <class ForwardIt >
        void plot(ForwardIt data,size_t dim, size_t N, size_t line_id = 1)
        {
                plot_axes.push_back( Axis(data_filename,data_ctr++,line_id,line_specifier));
                if(hold)
                {
                        output_data(data,dim,N,std::ios_base::app);
                }
                else
                {
                        output_data(data,dim,N,std::ios_base::out);
                        plot_all();
                        clear();
                }
        }

        inline void hold_on()
        {
                hold = true;
        }
        inline void hold_off()
        {
                hold = false;
                plot_all();
                plot_axes.clear();
        }
};


#endif





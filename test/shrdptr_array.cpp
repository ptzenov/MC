#include <memory>
#include <vector>

#include <iostream>
#include <assert.h>

/**
 *  A custom shared pointer class extending the std::shared_ptr
 *  in a way as to include memory management of
 *  arrays of data of type _Tp. At initializaiton, specify the size of the
 *  array.
 *
 *  a) provides the operator[] implementation
 *  b) provides begin(), end() functions for iterators/stl algorithms
 *  c) provices .size()  getter for the object size;
 *
 */
template<typename _Tp>
class custom_shared_ptr: public std::shared_ptr<_Tp>
{
private:
        size_t _N;
	using _Del = void(*)(_Tp*); 
	const _Del _deleter = [](_Tp* ptr)
        {
                delete[] ptr;
        }; 
public:
        custom_shared_ptr() = delete;  // cant have the void constructor
        explicit custom_shared_ptr(size_t len):
                std::shared_ptr<_Tp>(new _Tp[len], _deleter) , _N {len} { ; }

        // copy constructor
        template<typename _Yp>
        custom_shared_ptr(custom_shared_ptr<_Yp> const & other)
                : std::shared_ptr<_Yp>(other,_deleter), _N(other.N)
        {
                ;
        }

        // move constructor
        template<typename _Yp>
        custom_shared_ptr(custom_shared_ptr<_Yp> && other)
                : std::shared_ptr<_Yp>(std::move(other),_deleter), _N(other.N)
        {
                ;
        }

        /**
         *
         *
         */
        template<typename _Yp>
        custom_shared_ptr& operator=(custom_shared_ptr<_Yp> const & other)
        {
                if (&other != this)
                {
                        // constructs a copy of other! increments the ref ctr
                        custom_shared_ptr<_Tp> tmp(other);
                        // exchanges conents of this and tmp ;
                        tmp.swap(*this); // will call std::shared_ptr.swap();
                        this->_N = tmp._N;
                        // releases ownership of the shared object (whatever this was pointing to)
                        tmp.reset();
                }
                return *this;
        }

        template<typename _Yp>
        custom_shared_ptr& operator=(custom_shared_ptr<_Yp> && other)
        {
                // copy other into a temp;
                custom_shared_ptr<_Yp> tmp (std::move(other));
                tmp.swap(*this); // swap the temp with this -> calls std::share_ptr::swap(..);
                this->_N = tmp._N;  /// don't forget to copy the sizes;
                return *this;
        }

        virtual ~custom_shared_ptr()
        {
                ;
        }


        /**
         * overload the suffix operator
         */
        _Tp& operator[](std::ptrdiff_t idx)
        {
                assert(idx < _N);
                return *(this->get()+idx);
        }

        /**
         *  Provide a begin and end functions for ranged based loops, stl algos etc.
         *
         * */
        inline _Tp* begin()
        {
                return this->get();
        }
        inline _Tp* end()
        {
                return this->get()+this->_N;
        }

        inline const size_t & size() const
        {
                return _N;       // number of elements in the array
        }
};







std::vector<custom_shared_ptr<double> >
foo()
{
        size_t N = 15;
        custom_shared_ptr<double> csp(N);
        std:: cout << csp.use_count() << std::endl;

        custom_shared_ptr<double> csp2  = csp;
        std:: cout << csp.use_count() << std::endl;

        custom_shared_ptr<double> csp3 (N+10);
        // how come this is possible?
        for( size_t i = 0; i<csp.size(); ++i)
        {
                csp[i] = i;
        }
        for (size_t i=0; i < csp3.size(); ++i)
                csp3[i] = csp3.size()-i;
        std:: cout << csp3.use_count() << std::endl;

        std::vector<custom_shared_ptr<double> > result;
        result.push_back(csp);
        result.push_back(csp2);
        result.push_back(csp3);
        return result;
}



int main(int av, char** ac)
{

        auto res = foo();
        std::cout << "HELOO"<< std::endl;
        std:: cout << "0: " << res[0].get() <<"|"<<  res[0].use_count() << std::endl;
        std:: cout << "1: " << res[1].get() <<"|"<< res[1].use_count() << std::endl;
        std:: cout << "2: " << res[2].get() <<"|"<< res[2].use_count() << std::endl;

        for (auto & PSI: res)
        {
                for (auto b:PSI)
                {
                        std::cout << b << " " ;
                }
                std::cout << std::endl;
        }


        B b1(1);
        B b2 = b1;
}






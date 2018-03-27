#include <memory>
#include <vector>

#include <iostream>
#include <assert.h>


#define MSG(X) std::cout<<"MSG: " X << std::endl<< \
	"\tFILE: " << __FILE__<< std::endl << "\tLINE: " << __LINE__<< std::endl;



/**
 *  A custom shared pointer class extending the std::shared_ptr
 *  in a way as to include memory management of
 *  arrays of type _Tp. At initializaiton, specify the size of the
 *  array.
 *
 *  a) provides the operator[] implementation
 *  b) provides begin(), end() functions for iterators/stl algorithms
 *  c) provices .size()  getter for the object size;
 *
 *  d) it does not allow for direct de-referencing of the pointed-to object.
 *  e) it does not allow initialization with custom_shared_ptr<type>(std::make_shared<type>(arg1,arg2), len);
 *  f) it does not allow for initialization of arrays of shared poitners,
 *  via a call: new custom_shared_pointer();
 *  g) it does not allow any call to new custom_shared_pointer(); ....
 **/
template<typename _Tp>
class custom_shared_ptr
{
private:
        std::shared_ptr<_Tp> _data;
        size_t _N;
        struct _deleter
        {
                void operator()(_Tp* ptr)
                {
                        delete[] ptr;
                }
        };

public:
        // disable the void constructor -  disables call to: new custom_shared_ptr[N]();
        custom_shared_ptr() = delete;

        // disable heap management of custom_shared_ptr
        static void* operator new(std::size_t size)  = delete;  // cant allocate on the heap
        static void* operator new[](std::size_t size)  = delete;  // cant allocate arrays on the heap
        void operator delete(void * ptr) = delete; // does not make sense also to call delete
        void operator delete[](void *ptr) = delete; // does not make sense to call delete


        // constructs an "empty" shared pointer to an array of size len
        explicit custom_shared_ptr(size_t len):
                _data (new _Tp[len],_deleter()), _N (len)
        {
        }

        // Potentially dangerous! if user does not specify correct len!
        custom_shared_ptr(_Tp* raw_ptr, size_t len): _data(raw_ptr,_deleter()), _N(len)
        {
        }


        // copy constructor
        template<typename _Yp>
        custom_shared_ptr(custom_shared_ptr<_Yp> const & other)
                : _data (other.data()), _N {other.size()}
        {
        }

        // move constructor
        template<typename _Yp>
        custom_shared_ptr(custom_shared_ptr<_Yp> && other)
                : _data {std::forward(other.data())}, _N {other.size()}
        {
        }

        // copy assignement operator
        template<typename _Yp>
        custom_shared_ptr& operator=(custom_shared_ptr<_Yp> const & other)
        {
                if ( &other == this)
                        return *this;
                // constructs a copy of other! increments the ref ctr
                custom_shared_ptr<_Tp> tmp(other);
                // exchanges conents of this and tmp ;
                tmp.data().swap(_data); // will call std::shared_ptr.swap();
                this->_N = tmp._N;
                // releases ownership of the shared object (whatever this was pointing to)
                tmp.reset();
                return *this;
        }
        // move assignment operator
        template<typename _Yp>
        custom_shared_ptr& operator=(custom_shared_ptr<_Yp> && other)
        {
                // copy other into a temp;
                custom_shared_ptr<_Yp> tmp (std::forward(other));
                tmp.data().swap(_data); // swap the temp with this -> calls std::share_ptr::swap(..);
                this->_N = tmp._N;  /// don't forget to copy the sizes;
                return *this;
        }

        //suffix operator
        _Tp& operator[](size_t idx)
        {
                assert(idx < _N);
                return *(_data.get()+idx);
        }
        long use_count() const noexcept
        {
                return _data.use_count();
        }

        //Provide a begin and end functions for ranged based loops, stl algos etc.
        _Tp* begin()
        {
                return _data.get();
        }
        _Tp* end()
        {
                return _data.get()+this->_N;
        }

        _Tp* begin() const
        {
                return _data.get();
        }

        _Tp* end() const
        {
                return _data.get()+this->_N;
        }

        void reset()
        {
                _data.reset();
                _N = 0U;
        }

        size_t size() const
        {
                return _N;       // number of elements in the array
        }
        const std::shared_ptr<_Tp> & data() const
        {
                return _data;
        }
        // overload conversion
        explicit operator bool() const
        {
                return static_cast<bool>(_data);
        }
};


std::vector<custom_shared_ptr<double> >
foo()
{
        size_t N = 15;
        custom_shared_ptr<double> csp(N); // empty array A1
       	custom_shared_ptr<double> csp2  = csp; // point to A1
	// works	
        custom_shared_ptr<double> csp3 (new double[N+10],N+10); // A2

//	 this is fucked up - memory leaks	
//	custom_shared_ptr<double>*  csp4  = new custom_shared_ptr<double>(10); 
//	custom_shared_ptr<double>*  csp5  = new custom_shared_ptr<double>(15); 
//	custom_shared_ptr<double>*  csp6  = new custom_shared_ptr<double>(25); 

	 
	for( size_t i = 0; i<csp.size(); ++i)
        {
                csp[i] = i; 
        }
        for (size_t i=0; i < csp3.size(); ++i)
                csp3[i] = csp3.size()-i;

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

        for (auto & PSI: res)
        {
                for (auto b:PSI)
                {
                        std::cout << b << " " ;
                }
                std::cout << std::endl;
        }
	
	auto Deleter = [](double * ptr){delete [] ptr;};
	using del_type = decltype(Deleter);
	size_t N = 100;
	// works but uncomfortable
	std::shared_ptr<double> sp(new double[N],Deleter);
	*(sp.get()+2) = 0;
	
	std::cout << "finito " ; 

}






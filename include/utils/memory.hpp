/**
 * file memory.hpp
 * defines routines/classes etc. related to
 * the memory management in the project
 */
#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <memory>

/****** Memory Management headers *********/
namespace MC
{

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
public:

        custom_shared_ptr() = delete;  // cant have the void constructor

        explicit custom_shared_ptr(size_t len):
                std::shared_ptr<_Tp>(new _Tp[len],[](_Tp* ptr)
        {
                delete[] ptr;
        }) , _N {len} { ; }

        // copy constructor
        template<typename _Yp>
        custom_shared_ptr(custom_shared_ptr<_Yp> const & other)
                : std::shared_ptr<_Yp>(other), _N(other.N)
        {
                ;
        }

        // move constructor
        template<typename _Yp>
        custom_shared_ptr(custom_shared_ptr<_Yp> && other)
                : std::shared_ptr<_Yp>(std::move(other)), _N(other.N)
        {
                ;
        }

        /**
         *
         * copy assignement operator
         */
        template<typename _Yp>
        custom_shared_ptr& operator=(custom_shared_ptr<_Yp> const & other)
        {
                if ( &other == this)
                        return *this;
                // constructs a copy of other! increments the ref ctr
                custom_shared_ptr<_Tp> tmp(other);
                // exchanges conents of this and tmp ;
                tmp.swap(*this); // will call std::shared_ptr.swap();
                this->_N = tmp._N;
                // releases ownership of the shared object (whatever this was pointing to)
                tmp.reset();
                return *this;
        }
        /**
         * move assignment operator
         */
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
        _Tp& operator[](size_t idx)
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


}


#endif






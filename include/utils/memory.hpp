/**
 * file memory.hpp
 * defines routines/classes etc. related to
 * the memory management in the project
 */
#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <memory>
#include <assert.h>
#include <iostream>
#include <vector>

#include <utils/debug/macros.hpp>

/****** Memory Management headers *********/
namespace MC
{

/**
 *  A custom shared pointer class extending the std::shared_ptr
 *  in a way as to include memory management of
 *  arrays of _data of type _Tp. At initializaiton, specify the size of the
 *  array.
 *
 *  a) provides the operator[] implementation
 *  b) provides begin(), end() functions for iterators/stl algorithms
 *  c) provices .size()  getter for the object size;
 *
 */
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

        custom_shared_ptr() = delete;  // cant have the void constructor
        explicit custom_shared_ptr(size_t len):
                _data (new _Tp[len],_deleter()), _N (len)
        {
        }


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

        /**
         *
         * copy assignement operator
         */
        template<typename _Yp>
        custom_shared_ptr& operator=(custom_shared_ptr<_Yp> const & other)
        {
                MSG("copy = operator");
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
        /**
         * move assignment operator
         */
        template<typename _Yp>
        custom_shared_ptr& operator=(custom_shared_ptr<_Yp> && other)
        {
                
		// copy other into a temp;
                custom_shared_ptr<_Yp> tmp (std::forward(other));
                tmp.data().swap(_data); // swap the temp with this -> calls std::share_ptr::swap(..);
                this->_N = tmp._N;  /// don't forget to copy the sizes;
                return *this;
        }



        /**
         * overload the suffix operator
         */
        _Tp& operator[](size_t idx)
        {
                assert(idx < _N);
                return *(_data.get()+idx);
        }
        long use_count() const noexcept
        {
                return _data.use_count();
        }

        /**
         *  Provide a begin and end functions for ranged based loops, stl algos etc.
         * */
        inline _Tp* begin()
        {
                return _data.get();
        }
        inline _Tp* end()
        {
                return _data.get()+this->_N;
        }

        inline const size_t & size() const
        {
                return _N;       // number of elements in the array
        }
        inline const std::shared_ptr<_Tp> & data() const
        {
                return _data;
        }
};

// Insert arbitrary number of variable sized arrays of T inside a vector containing custom_shared_ptr<T>
// usage: recurrent_insert_homog_data(vec,first,N, ... variadic ...)
// where variadic can be any (even) number of arguments coming in pairs:
// ... Itarator first1, size_type N1,Itartor first2,size_type N2, Iterator first3, size_type N3 ...
//  with first# pointing to the first element in the array # and N# specifying the array's length.

// end of the recurrsion
template<typename T>
void recurrent_insert_homog_data(std::vector<MC::custom_shared_ptr<T> >& vec)
{
        return;
}

// recursively expand the variadic template
template<typename T, typename Iterator, typename size_type, typename... Targs>
void recurrent_insert_homog_data(std::vector<MC::custom_shared_ptr<T>>& vec,Iterator first,
                                 size_type N, Targs... args)
{
        MC::custom_shared_ptr<T> dummy(N);
        std::copy(first,first+N,dummy.begin());
        vec.push_back(dummy);
        recurrent_insert_homog_data(vec,args...);
        return;
}

};
#endif












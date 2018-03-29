/**
 * file memory.hpp
 * defines routines/classes etc. related to
 * the memory management in the project
 */
#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <memory>
#include <assert.h>
#include <vector>

/****** Memory Management headers *********/
namespace MC
{

/**
 *  A  shared array pointer class extending the std::shared_ptr
 *  in a way as to include memory management of
 *  arrays of type _Tp. At initializaiton, specify the size of the
 *  array.
 *
 *  a) provides the operator[] implementation
 *  b) provides begin(), end() functions for iterators/stl algorithms
 *  c) provices .size()  getter for the object size;
 *
 *  d) it does not allow for direct de-referencing of the pointed-to object.
 *  e) it does not allow initialization with shared_array_ptr<type>(std::make_shared<type>(arg1,arg2), len);
 *  f) it does not allow for initialization of arrays of shared poitners,
 *  via a call: new shared_array_ptr();
 *  g) it does not allow any call to new shared_array_ptr(); ....
 **/
template<typename _Tp>
class shared_array_ptr
{
private:
        std::shared_ptr<_Tp> _data;
        unsigned int _N;
        struct _deleter
        {
                void operator()(_Tp* ptr)
                {
                        delete[] ptr;
                }
        };

public:
        // disable the void constructor -  disables call to: new shared_array_ptr[N]();
        shared_array_ptr() = delete;

        // disable heap management of shared_array_ptr
        static void* operator new(std::size_t size)  = delete;  // cant allocate on the heap
        static void* operator new[](std::size_t size)  = delete;  // cant allocate arrays on the heap
        void operator delete(void * ptr) = delete; // does not make sense also to call delete
        void operator delete[](void *ptr) = delete; // does not make sense to call delete


        // constructs an "empty" shared pointer to an array of size len
        explicit shared_array_ptr(unsigned int len):
                _data (new _Tp[len],_deleter()), _N (len)
        {
        }

        // Potentially dangerous! if user does not specify correct len!
        shared_array_ptr(_Tp* raw_ptr, unsigned int len): _data(raw_ptr,_deleter()), _N(len)
        {
        }

        // copy constructor
        template<typename _Yp>
        shared_array_ptr(shared_array_ptr<_Yp> const & other)
                : _data (other.data()), _N {other.size()}
        {
        }

        // move constructor
        template<typename _Yp>
        shared_array_ptr(shared_array_ptr<_Yp> && other)
                : _data {std::forward(other.data())}, _N {other.size()}
        {
        }

        // copy assignement operator
        template<typename _Yp>
        shared_array_ptr& operator=(shared_array_ptr<_Yp> const & other)
        {
                if ( &other == this)
                        return *this;
                // constructs a copy of other! increments the ref ctr
                shared_array_ptr<_Tp> tmp(other);
                // exchanges conents of this and tmp ;
                tmp.data().swap(_data); // will call std::shared_ptr.swap();
                this->_N = tmp._N;
                // releases ownership of the shared object (whatever this was pointing to)
                tmp.reset();
                return *this;
        }
        // move assignment operator
        template<typename _Yp>
        shared_array_ptr& operator=(shared_array_ptr<_Yp> && other)
        {
                // copy other into a temp;
                shared_array_ptr<_Yp> tmp (std::forward(other));
                tmp.data().swap(_data); // swap the temp with this -> calls std::share_ptr::swap(..);
                this->_N = tmp._N;  /// don't forget to copy the sizes;
                return *this;
        }

        //suffix operator
        _Tp& operator[](unsigned int idx)
        {
                assert(idx < _N);
                return *(_data.get()+idx);
        }

	// to be able to call the suffix on const shared_array_ptrs
        _Tp operator[](unsigned int idx) const
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
                return _data.get()+_N;
        }

        _Tp const * begin() const
        {
                return _data.get();
        }

        _Tp const * end() const
        {
                return _data.get()+_N;
        }

        void reset()
        {
                _data.reset();
                _N = 0U;
        }

        unsigned int size() const
        {
                return _N;       // number of elements in the array
        }
	
	// needed for the copy constructor
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


//
// Insert arbitrary number of variable sized arrays of T inside a vector containing shared_array_ptr<T>
// usage: recurrent_insert_homog_data(vec,first,N, ... variadic ...)
// where variadic can be any (even) number of arguments coming in pairs:
// ... Itarator first1, size_type N1,Itartor first2,size_type N2, Iterator first3, size_type N3 ...
//  with first# pointing to the first element in the array # and N# specifying the array's length.
//

// end of the recurrsion
template<typename T>
void recurrent_insert_homog_data(std::vector<MC::shared_array_ptr<T> >& vec)
{
        return;
}

// recursively expand the variadic template
template<typename T, typename Iterator, typename size_type, typename... Targs>
void recurrent_insert_homog_data(std::vector<MC::shared_array_ptr<T>>& vec,Iterator first,
                                 size_type N, Targs... args)
{
        MC::shared_array_ptr<T> dummy(N);
        std::copy(first,first+N,dummy.begin());
        vec.push_back(dummy);
        recurrent_insert_homog_data(vec,args...);
        return;
}

};
#endif














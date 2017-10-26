#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>


#ifndef _STATE_HPP_
#define _STATE_HPP_

#define EMPTY -1

namespace MC
{
class MetaInfo { };

class AbstractState
{
public:
        explicit AbstractState(int id): _pID(id)
        {
                ;       //
        }
        explicit AbstractState() : _pID(EMPTY)
        {
                ;
        }
        virtual ~AbstractState()
        {
                ;
        }
        int get_pID() const
        {
                return _pID;
        }
        void set_pID(int id)
        {
                _pID = id;
        }
private:
        int _pID;
        friend void swap_particles(AbstractState & a, AbstractState & b){ std::swap(a._pID,b._pID);};
};

}
#endif



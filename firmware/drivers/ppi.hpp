#ifndef _APP_PPI_HPP__
#define _APP_PPI_HPP__


#include <stdint.h>


namespace ppi
{

    typedef uint32_t task_t;
    typedef uint32_t event_t;

    namespace task
    {
        namespace timer0
        {
            task_t const capture1 = 0x40008044;
            task_t const clear = 0x4000800C;
        }
        namespace timer4
        {
            task_t const clear = 0x4001B00C;
        }
        namespace radio
        {
        }
    }
    namespace event
    {
        namespace timer0
        {
        }
        namespace timer4
        {
            task_t const compare5 = 0x4001B154;
        }
        namespace radio
        {
            event_t const end = 0x4000110C;
        }
    }
}



class ppi_c
{
    public:
        ppi_c();
        void link(ppi::event_t event, ppi::task_t task, ppi::task_t fork = 0);
};

#endif /*_APP_PPI_HPP__*/

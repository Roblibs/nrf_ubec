#ifndef __COMPARE_HPP__
#define __COMPARE_HPP__


#include <stdint.h>
#include "nrfx_timer.h"
#include "ppi.hpp"

typedef void (*app_compare_handler_t)();

class compare_c 
{
    public:
        compare_c(uint8_t instance,uint32_t v_period);
        void start();
    public:
        nrfx_timer_t timer;
        uint32_t period;
};



#endif /*__COMPARE_HPP__*/


#include "compare.hpp"

#include <stdbool.h>
#include <stdint.h>

#include "nrfx_timer.h"

NRF_TIMER_Type* timer_base_addresses[5]={ (NRF_TIMER_Type*)NRF_TIMER0_BASE, 
                                        (NRF_TIMER_Type*)NRF_TIMER1_BASE,
                                        (NRF_TIMER_Type*)NRF_TIMER2_BASE,
                                        (NRF_TIMER_Type*)NRF_TIMER3_BASE,
                                        (NRF_TIMER_Type*)NRF_TIMER4_BASE};

uint8_t timer_nb_cc_channels[5]={ 4, 4, 4, 6, 6};


void timer_compare_handler(nrf_timer_event_t event_type, void * p_context)
{
    compare_c * p_compare = (compare_c*)p_context;
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
        {
            p_compare->timer = p_compare->timer;
        }
        break;
        case NRF_TIMER_EVENT_COMPARE1:
        {
            
        }
        break;
        default:
            //Do nothing.
            break;
    }
}

compare_c::compare_c(uint8_t instance,uint32_t v_period)
{
    period = v_period;

    timer.p_reg = timer_base_addresses[instance];
    timer.instance_id = instance;
    timer.cc_channel_count = timer_nb_cc_channels[instance];

    nrfx_timer_config_t timer_cfg = {
        .frequency          = (nrf_timer_frequency_t)NRF_TIMER_FREQ_1MHz,
        .mode               = (nrf_timer_mode_t)NRF_TIMER_MODE_TIMER,          
        .bit_width          = (nrf_timer_bit_width_t)NRF_TIMER_BIT_WIDTH_32,
        .interrupt_priority = 7,
        .p_context          = (void*)this
    };
    nrfx_timer_init(&timer, &timer_cfg, timer_compare_handler);

    //channel 5 is used for self loopback period
    nrfx_timer_compare(&timer,NRF_TIMER_CC_CHANNEL5,period,true);

}

void compare_c::start()
{
    nrfx_timer_enable(&timer);
}


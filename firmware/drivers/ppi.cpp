#include "ppi.hpp"

#include "nrfx_ppi.h"
#include "nrf_ppi.h"

using namespace ppi;


ppi_c::ppi_c()
{
    nrfx_ppi_free_all();

}

void ppi_c::link(event_t event, task_t task, task_t fork)
{
    nrf_ppi_channel_t channel;
    nrfx_ppi_channel_alloc(&channel);
    nrfx_ppi_channel_assign(channel, event,task);
    if(fork != 0)
    {
        nrfx_ppi_channel_fork_assign(channel,fork);
    }
    nrfx_ppi_channel_enable(channel);
}

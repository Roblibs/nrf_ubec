#include "hall.hpp"

#include "nrf_gpio.h"

#include "nrfx_saadc.h"

#define pi_div_3        1.0471975511965977461542144610932
#define pix2_div_3      2.0943951023931954923084289221863
#define pi              3.1415926535897932384626433832795
#define pix4_div_3      4.1887902047863909846168578443727
#define pix5_div_3      5.2359877559829887307710723054658
#define pix2            6.283185307179586476925286766559


#define GPIO_Debug_ADC    NRF_GPIO_PIN_MAP(1,10)

nrf_saadc_input_t an_chan_map[8]={NRF_SAADC_INPUT_AIN0,NRF_SAADC_INPUT_AIN1,NRF_SAADC_INPUT_AIN2,NRF_SAADC_INPUT_AIN3,
                                NRF_SAADC_INPUT_AIN4,NRF_SAADC_INPUT_AIN5,NRF_SAADC_INPUT_AIN6,NRF_SAADC_INPUT_AIN7};

hall_sensors_c *p_sensors = NULL;

void saadc_event_handler(nrfx_saadc_evt_t const * p_evt)
{
    if(p_sensors == NULL)
    {
        return;
    }
    if (p_evt->type == NRFX_SAADC_EVT_DONE)
    {
        nrf_gpio_pin_clear(GPIO_Debug_ADC);
        //values are now available in the buffers p_sensors->adc_values, ready to be processed
        p_sensors->process_adc_values();
    }
}

hall_sensors_c::hall_sensors_c(uint8_t c1, uint8_t c2, uint8_t c3,float v_amplitude)
{
    p_sensors = this;
    channel1 = c1;
    channel2 = c2;
    channel3 = c3;
    amplitude = v_amplitude;
}

void hall_sensors_c::start()
{
    nrfx_saadc_config_t init_config = {
        NRF_SAADC_RESOLUTION_12BIT,
        NRF_SAADC_OVERSAMPLE_8X,
        5,                                  //interrup priority
        false                               //low power mode disabled
    };
    nrfx_saadc_init(&init_config,saadc_event_handler);


    nrf_saadc_channel_config_t chan_config = {
        NRF_SAADC_RESISTOR_DISABLED,                //P
        NRF_SAADC_RESISTOR_DISABLED,                //N
        NRF_SAADC_GAIN1_4,
        NRF_SAADC_REFERENCE_VDD4,
        NRF_SAADC_ACQTIME_40US,
        NRF_SAADC_MODE_SINGLE_ENDED,
        NRF_SAADC_BURST_ENABLED,                    //required tofor auto - oversampling
        NRF_SAADC_INPUT_DISABLED,   
        NRF_SAADC_INPUT_DISABLED
    };

    chan_config.pin_p = an_chan_map[channel1];
    nrfx_saadc_channel_init(channel1,&chan_config);

    chan_config.pin_p = an_chan_map[channel2];
    nrfx_saadc_channel_init(channel2,&chan_config);

    chan_config.pin_p = an_chan_map[channel3];
    nrfx_saadc_channel_init(channel3,&chan_config);

}

void hall_sensors_c::convert()
{
    nrf_gpio_pin_set(GPIO_Debug_ADC);
    nrfx_saadc_buffer_convert(adc_values, 3);
    nrfx_saadc_sample();
}

//alternative would be stl algo sort
uint8_t sort_3f(float v1, float v2, float v3)
{
    uint8_t res = 0;
    if(v1 > v2)
    {
        if(v1 > v3)
        {
            if(v2 > v3)// => v1 > v2 > v3
            {
                res = 1;
            }
            else// => v1 > v3 > v2
            {
                res = 6;
            }
        }
        else// => v3 > v1 > v2
        {
            res = 5;
        }
    }
    else//(v2 > v1)
    {
        if(v2 > v3)
        {
            if(v1 > v3)// => v2 > v1 > v3
            {
                res = 2;
            }
            else// => v2 > v3 > v1
            {
                res = 3;
            }
        }
        else// => v3 > v2 > v1
        {
            res = 4;
        }
    }
    return res;
}

void hall_sensors_c::process_adc_values()
{
    v1 = adc_values[0] / 4096.0;// 12 bits
    v2 = adc_values[1] / 4096.0;// 12 bits
    v3 = adc_values[2] / 4096.0;// 12 bits
    phase_section = sort_3f(v1,v2,v3);
    v1 = (v1 - 0.5) / amplitude;
    v2 = (v2 - 0.5) / amplitude;
    v3 = (v3 - 0.5) / amplitude;
    switch(phase_section)
    {
        case 1:
            if(v2>0)
            {
                magnetic_angle = 0          + v2;
            }
            else
            {
                magnetic_angle = pix2       + v2;
            }
        break;
        case 2:
            magnetic_angle = pi_div_3   - v1;
        break;
        case 3:
            magnetic_angle = pix2_div_3 + v3;
        break;
        case 4:
            magnetic_angle = pi         - v2;
        break;
        case 5:
            magnetic_angle = pix4_div_3 + v1;
        break;
        case 6:
            magnetic_angle = pix5_div_3 - v3;
        break;
    }
}

/** @file main.c
 *
 * main entry for the application nRF52_dongle
 * 
 * @author Wassim FILALI
 *
 * @compiler arm gcc
 *
 *
 * $Date: 01.06.2018 adding doxy header this file existed since the repo creation
 *
*/

#include <stdbool.h>
#include <stdint.h>
#include <string>

extern "C"
{
    #include "sdk_config.h"

    //#include "nrf.h"
    //#include "nrf_error.h"
    #include "nrf_delay.h"
    #include "nrf_gpio.h"
    #include "boards.h"
    #include "app_util.h"

    #include "bsp.h"


    //for the log
    #include "nrf_log.h"
    #include "nrf_log_ctrl.h"
    #include "nrf_log_default_backends.h"

    // --------------------- inputs from sdk_config --------------------- 
    // ---> TWI0_ENABLED ---> TWI1_ENABLED
    #include "uicr_user_defines.h"
    //drivers
    //apps
    #include "clocks.h"
    #include "utils.h"
}

#include "strmap.hpp"
#include "usb_print.hpp"
#include "ppi.hpp"
#include "hall.hpp"
#include "bldc.hpp"
#include "compare.hpp"

#define PWM_INSTANCE 0
#define GPIO_M_P1 NRF_GPIO_PIN_MAP(0,10)
#define GPIO_M_P2 NRF_GPIO_PIN_MAP(0,9)
#define GPIO_M_P3 NRF_GPIO_PIN_MAP(1,0)
#define GPIO_M_EN NRF_GPIO_PIN_MAP(0,24)

#define GPIO_M_Hall1    NRF_GPIO_PIN_MAP(0,2)
#define GPIO_M_Hall2    NRF_GPIO_PIN_MAP(0,29)
#define GPIO_M_Hall3    NRF_GPIO_PIN_MAP(0,31)

#define GPIO_Debug_ADC    NRF_GPIO_PIN_MAP(1,10)

void app_usb_rx_handler(const char*msg,uint8_t size);

usb_c usb(app_usb_rx_handler);

uint8_t id = get_this_node_id();
std::string motor_topic(std::to_string(id) + "motor");

//sample command
//"motor>pwm:300;"
void app_usb_rx_handler(const char*msg,uint8_t size)
{
    strmap_c params(msg,size);
    if(params.topic.compare(motor_topic) == 0)
    {
        if(params.has("pwm"))
        {
            uint32_t pwm_value = std::stoi(params["pwm"]);
            usb.printf("%u/pwm>val:%0.2f;\r\n",id,pwm_value);
        }
        else
        {
            usb.printf("%u/pwm>error:no pwm param given;\r\n",id);
        }
    }
}


/**
 * @brief application rtc event which is a configurable period delay
 * through the uicr config "sleep" in the nodes databse
 * 
 */
void app_rtc_handler()
{
    //static uint32_t alive_count = 0;
    led2_green_on();
    //usb.printf("%u/alive>%lu\r\n",id,alive_count++);
    led2_green_off();
}

int main(void)
{
    //--------------------- Important UICR settings --------------------- 
    //UICR.NFCPINS = 0xFFFFFFFE - Disabled
    //UICR.REGOUT0 = 0xFFFFFFFD - 3.3 V

    clocks_start();

    bsp_board_init(BSP_INIT_LEDS);

    blink_red(1000,200);
    blink_green(1000,200);
    blink_blue(1000,200);

    //nrf_gpio_cfg_output(GPIO_Debug_ADC);

    nrf_gpio_cfg_output(GPIO_M_EN);
    nrf_gpio_pin_set(GPIO_M_EN);

    // ------------------------- Start Init ------------------------- 
    usb.printf("%u/reset>1\r\n");//will be lost if port is closed
    rtc_config(app_rtc_handler);

    // ------------------------- Start Events ------------------------- 
    uint32_t count = 0;
    while(true)
    {
        usb.loop();
        nrf_delay_us(1000);
        if((count % 2) == 0)
        {
            usb.printf("%u/pwm>alive;\r\n",id);
        }
        count++;
    }
}
/*lint -restore */

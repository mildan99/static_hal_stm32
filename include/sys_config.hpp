#include "stm32f4xx.h"
#include "mcu_clk.hpp"

constexpr uint32_t hse_speed = 25'000'000U;
namespace clk = mcu::clk;
constexpr mcu::clk::clk_cfg clk_25mhz_to96mhz{
    clk::sysclk_src::PLLCLK_P // 
    ,mcu::clk::pllsrc::HSE,
      25,
       192,
        0,
         4,
         mcu::clk::ahb_div::DIV1 ,
    clk::apbX_div::DIV2,   
        clk::apbX_div::DIV1};
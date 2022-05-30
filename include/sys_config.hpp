#include "stm32f4xx.h"
#include "mcu_clk.hpp"

constexpr uint32_t hse_speed = 25'000'000U;
namespace clk = mcu::clk;
constexpr mcu::clk::clk_cfg clk_25mhz_to96mhz{.sysclk_s = clk::sysclk_src::PLLCLK_P,
                                              .pll_s = mcu::clk::pllsrc::HSE,
                                              .M = 25,
                                              .N = 192,
                                              .P = 0,
                                              .Q = 4,
                                              .AHBDIV = mcu::clk::ahb_div::DIV1,
                                              .APB1DIV = clk::apbX_div::DIV2,
                                              .APB2DIV = clk::apbX_div::DIV1};
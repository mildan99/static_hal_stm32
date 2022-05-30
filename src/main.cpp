
#include "stm32f4xx.h"

#include "mcu_base.hpp"
#include "supergpio.hpp"
#include "dma_format.hpp"
#include "uart.hpp"
#include "dma.hpp"

#include <array>
#include <stdint.h>
#include <cstdint>
#include <string_view>
#include <tuple>


#include "Eigen/Dense"
Eigen::Matrix<float, 2, 2> a{{2,1},{1,2}}, b{{1,0},{0,-1}},cc;


//#include "FreeRTOS.h"
#include "system_stm32f4xx.h"
//#include "task.h"

#include "xprintf.h"

void exti_handler() {
  if (EXTI->PR & EXTI_PR_PR3) {
    EXTI->PR = EXTI_PR_PR3;

  } else {
    EXTI->PR = EXTI_PR_PR14;
  }

  for (uint32_t time = 1000000; time > 1; time--)
    ;
}

__attribute__((noreturn))  void task_blink(void *p) {
  (void)p;
  {
    using namespace mcu::gpio;
    mcu::gpio::init<13>(GPIOC, mode::output, type::PP);
  }
  while (true) {
    mcu::gpio::toggle<13>(GPIOC);
    vTaskDelay(1000);
  }
}

volatile uint32_t baud = 115200;

mcu::uart uart1(USART1, (RCC->APB2ENR), RCC_APB2ENR_USART1EN, USART1_IRQn);
mcu::uart uart6(USART6, (RCC->APB2ENR), RCC_APB2ENR_USART6EN, USART6_IRQn);
mcu::uart uart2(USART2, (RCC->APB1ENR), RCC_APB1ENR_USART2EN, USART2_IRQn);

mcu::dma dma1{DMA1,
              (RCC->AHB1ENR),
              RCC_AHB1ENR_DMA1EN,
              {mcu::dma_stream(DMA1_Stream0, 0, DMA1_Stream0_IRQn),
               mcu::dma_stream(DMA1_Stream1, 1, DMA1_Stream1_IRQn),
               mcu::dma_stream(DMA1_Stream2, 2, DMA1_Stream2_IRQn),
               mcu::dma_stream(DMA1_Stream3, 3, DMA1_Stream3_IRQn),
               mcu::dma_stream(DMA1_Stream4, 4, DMA1_Stream4_IRQn), 
               mcu::dma_stream(DMA1_Stream5, 5, DMA1_Stream5_IRQn),
               mcu::dma_stream(DMA1_Stream6, 6, DMA1_Stream6_IRQn),
               mcu::dma_stream(DMA1_Stream7, 7, DMA1_Stream7_IRQn)}};
           
mcu::dma dma2{DMA2,
              (RCC->AHB1ENR),
              RCC_AHB1ENR_DMA2EN,
              {mcu::dma_stream(DMA2_Stream0, 0, DMA2_Stream0_IRQn),
               mcu::dma_stream(DMA2_Stream1, 1, DMA2_Stream1_IRQn),
               mcu::dma_stream(DMA2_Stream2, 2, DMA2_Stream2_IRQn),
               mcu::dma_stream(DMA2_Stream3, 3, DMA2_Stream3_IRQn),
               mcu::dma_stream(DMA2_Stream4, 4, DMA2_Stream4_IRQn), 
               mcu::dma_stream(DMA2_Stream5, 5, DMA2_Stream5_IRQn),
               mcu::dma_stream(DMA2_Stream6, 6, DMA2_Stream6_IRQn),
               mcu::dma_stream(DMA2_Stream7, 7, DMA2_Stream7_IRQn)}};
          


volatile uint32_t ddel = 200;

_Noreturn void uart_task(void *p) {
  (void)p;
  {
    using namespace mcu::gpio;
    mcu::gpio::init<9, 10>(GPIOA, mode::output_af, type::PP, speed::very_high, pupd::nopupd, af::af7);
  }
  uart1.clk_enable();
  uart1.config(baud,mcu::uart_cfg::stop_bit::STOP_1,mcu::uart_cfg::parity::NONE);

  xdev_out([](unsigned char c) { uart1.send_char(c); });
  char msg[] = "hello usart, dma and evryone else, I welcome you in this wonderful world...\r\n";
uint32_t i=0;
  while (true) {
    xprintf("hello usart %6ld \r\n",++i);
    uart1.send_dma((uint8_t *)msg, sizeof(msg) - 1);
    vTaskDelay(ddel);
  }
}
void set_eee() {

  mcu::gpio::exti::exti_init({GPIOB, 14}, mcu::gpio::exti::edge::RISING, exti_handler);
  mcu::gpio::exti::exti_init({GPIOB, 3}, mcu::gpio::exti::edge::FALLING, exti_handler);
}


int main() {

 
  TaskHandle_t xHandle = NULL;
  xTaskCreate(task_blink, "blinker", 64, 0, 0, &xHandle);
  xTaskCreate(uart_task, "uart", 128, 0, 0, 0);
  vTaskStartScheduler();

  while (true) {}
}
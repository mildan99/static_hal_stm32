
#include "stm32f4xx.h"

#include "mcu_base.hpp"
#include "supergpio.hpp"
 #include "dma_format.hpp"
 #include "uart.hpp"
 #include "dma.hpp"

#include <stdint.h>
#include "FreeRTOS.h"
#include "system_stm32f4xx.h"
//#include "task.h"
#include <utility>
#include <memory>
#include <basic_format.hpp>
#include <Eigen/Dense>
#include <sml.hpp>
#include <ctre.hpp>
void exti_handler() {
  if (EXTI->PR & EXTI_PR_PR3) {
    EXTI->PR = EXTI_PR_PR3;

  } else {
    EXTI->PR = EXTI_PR_PR14;
  }

  for (uint32_t time = 1000000; time > 1; time--)
    ;
}


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
 
std::vector<char>  dev; 

int main() {
  float sum=0.0f;

  print_dev([&](auto* data,size_t size){for (size_t i = 0; i < size; i++) dev.push_back(data[i]); },"sone ",-1243);
   
 auto [match,cmd]=ctre::search<"CMD(\\d+)">("some CMD4");
  while (true) {} 
  return sum;
}
#include <cstring>
//#include <new>
#include "mcu_base.hpp"
#include "mcu_clk.hpp"
#include "sys_config.hpp"

// typedef void (*pfunc)();

extern unsigned int __vectors_start;
extern unsigned int __vectors_end;
// __attribute__((section(".bss.ITCMRAM"),aligned (512))) volatile pfunc
// vectors[128]={0};
__attribute__((aligned(512))) mcu::vector_func_t vectors[128] = {0};

extern "C" {
extern void _start();
// extern void Default_Handler();
 extern void SVC_Handler();
 extern void PendSV_Handler();
 extern void SysTick_Handler();
// void SysTick_Handler() {
//   __NOP();
//   mcu::clk::ticks++;
// }
//__attribute__((naked)) void Reset_Handler() { __asm volatile ("b _start"); }


void Default_Handler() {
  volatile unsigned int F=0;
  __disable_irq();
  while (true) {
    F++;
  }
}
//  void vConfigureTimerForRunTimeStats( void ){
//   CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
// 	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;   // запускаем счётчик
// }


/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static – otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task’s stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*———————————————————–*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static – otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task’s state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task’s stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


}
#pragma GCC optimize ("Os")

extern "C" void system_preinit() {}
extern "C" void system_vectors_init() {

  uint32_t size = ((uint32_t)(&__vectors_end) - (uint32_t)(&__vectors_start));
  memcpy(&vectors[0], &__vectors_start, size);
  SCB->VTOR = (uint32_t)&vectors[0];
  
}

extern "C" void system_config() {


#if defined(__FPU_USED) && (__FPU_USED == 1U)
  SCB->CPACR |= ((3U << 10U * 2U) | /* enable CP10 Full Access */
                 (3U << 11U * 2U)); /* enable CP11 Full Access */
#endif

  RCC->CR &= ~(RCC_CR_PLLON);
  RCC->APB1ENR|=RCC_APB1ENR_PWREN;
  (void)PWR->CR;
  PWR->CR|=PWR_CR_VOS|PWR_CR_DBP;

  FLASH->ACR |= (FLASH_ACR_LATENCY_3WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN|FLASH_ACR_DCEN);
  while (!((FLASH->ACR & FLASH_ACR_LATENCY) == FLASH_ACR_LATENCY_3WS));
  RCC->CR |= RCC_CR_HSEON;
  while (!((RCC->CR) & RCC_CR_HSERDY_Msk));
  RCC->PLLCFGR =
      ((4 << RCC_PLLCFGR_PLLQ_Pos) | (RCC_PLLCFGR_PLLSRC_HSE) | (0 << RCC_PLLCFGR_PLLP_Pos) |
       (192 << RCC_PLLCFGR_PLLN_Pos) | (25 << RCC_PLLCFGR_PLLM_Pos));
  RCC->CR |= (RCC_CR_PLLON);
  while (!((RCC->CR) & (RCC_CR_PLLRDY)));
  RCC->CFGR |= ((RCC_CFGR_PPRE1_DIV2) | (RCC_CFGR_PPRE2_DIV1) | (RCC_CFGR_SW_PLL));
  while (!((RCC->CFGR) & (RCC_CFGR_SWS_PLL)));
  //RCC->DCKCFGR |= RCC_DCKCFGR_TIMPRE;

  SystemCoreClockUpdate();

  RCC->PLLI2SCFGR = (25 < RCC_PLLI2SCFGR_PLLI2SM_Pos) | (384 << RCC_PLLI2SCFGR_PLLI2SN_Pos) |
                    (2 << RCC_PLLI2SCFGR_PLLI2SR_Pos);
  RCC->CR |= (RCC_CR_PLLI2SON);
  while (!((RCC->CR) & (RCC_CR_PLLI2SRDY)));

  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  (void)SYSCFG->CMPCR;
  SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD;
  while (!((SYSCFG->CMPCR) & (SYSCFG_CMPCR_READY)));

  // mcu::clk::ticks = 0;
  // SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos) | (SysTick_CTRL_TICKINT_Msk);
  // SysTick->LOAD |= (SystemCoreClock / 1000 - 1);
  // mcu::set_vector(SysTick_IRQn, SysTick_Handler);
  // NVIC_EnableIRQ(SysTick_IRQn);
  // SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk);
  
  // std::set_new_handler(Default_Handler);

  mcu::set_vector(SVCall_IRQn, SVC_Handler);
  mcu::set_vector(SysTick_IRQn , SysTick_Handler);
  mcu::set_vector(PendSV_IRQn , PendSV_Handler);
}

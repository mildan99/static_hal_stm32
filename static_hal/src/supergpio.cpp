#include "supergpio.hpp"
void mcu::gpio::exti::exti_init(mcu::gpio::GPIO_pin pin,const edge ee, mcu::vector_func_t const handler) {
  const uint8_t edge_rising = ((uint8_t)(ee)) & 0b01, edge_falling = ((uint8_t)(ee)) >> 1 & 0b01;
  uint32_t portn = ((uint32_t)pin.port - (uint32_t)GPIOA) / 0x400;
  pupd pup = pupd::nopupd;
  if (edge_falling ^ edge_rising) {
    pup = (edge_rising) ? (pupd::pd) : (pupd::pu);
  }
  {
    using namespace mcu::gpio;
    mcu::gpio::init(pin, mode::input, type::PP, speed::high, pup, af::af0);
  }
  SYSCFG->EXTICR[pin.pin / 4] |= portn << 4 * (pin.pin % 4);
  EXTI->IMR |= 1 << pin.pin;
  EXTI->RTSR |= edge_rising << pin.pin;
  EXTI->FTSR |= edge_falling << pin.pin;
  EXTI->PR |= 1 << pin.pin;
  mcu::set_vector(mcu::gpio::exti::irqn[pin.pin], handler);
  NVIC_EnableIRQ(mcu::gpio::exti::irqn[pin.pin]);
}



inline void mcu::gpio::init(const GPIO_pin &pin,const mode m,const type t,const speed s,const pupd p,const af a) {

  /*
    #ifdef GPIOA
      if (pin.port == GPIOA)
        temp= RCC_AHB1ENR_GPIOAEN;
    #endif
    #ifdef GPIOB
      if (pin.port == GPIOB)
        temp= RCC_AHB1ENR_GPIOBEN;
    #endif
    #ifdef GPIOC
      if (pin.port == GPIOC)
        temp= RCC_AHB1ENR_GPIOCEN;
    #endif
    #ifdef GPIOD
      if (pin.port == GPIOD)
        temp= RCC_AHB1ENR_GPIODEN;
    #endif
    #ifdef GPIOE
      if (pin.port == GPIOE)
        temp= RCC_AHB1ENR_GPIOEEN;
    #endif
    #ifdef GPIOF
      if (pin.port == GPIOF)
        temp= RCC_AHB1ENR_GPIOFEN;
    #endif
    #ifdef GPIOG
      if (pin.port == GPIOG)
        temp= RCC_AHB1ENR_GPIOGEN;
    #endif
    #ifdef GPIOI
      if (pin.port == GPIOI)
        temp= RCC_AHB1ENR_GPIOIEN;
    #endif
    #ifdef GPIOJ
      if (pin.port == GPIOJ) // for big F7
        temp= RCC_AHB1ENR_GPIOJEN;
    #endif
    #ifdef GPIOK
      if (pin.port == GPIOK)
        temp= RCC_AHB1ENR_GPIOKEN;
    #endif
      RCC->AHB1ENR |= temp;
  */

  // TODO: move to clk module
  uint16_t portn = ((uint32_t)pin.port - (uint32_t)GPIOA);
  portn /= 1024;
  RCC->AHB1ENR |= 1 << portn;
  // RCC->AHB1ENR |= 1<<1;

  // temp = pin.port->MODER;
  // temp &= ~((0b11 << (2 * pin.pin)));
  // (temp |= int(m) << (2 * pin.pin));
  // pin.port->MODER = temp;
  MODIFY_REG(pin.port->MODER, (0b11 << (2 * pin.pin)), (uint32_t)(m) << (2 * pin.pin));

  // temp = pin.port->OTYPER;
  // temp &= ~((0b1 << (pin.pin)));
  // (temp |= (int(t) << (pin.pin)));
  // pin.port->OTYPER = temp;

  MODIFY_REG(pin.port->OTYPER, (0b1 << (pin.pin)), ((uint32_t)(t) << (pin.pin)));

  // temp = pin.port->OSPEEDR;
  // temp &= ~((0b11 << (2 * pin.pin)));
  // (temp |= (int(s) << (2 * pin.pin)));
  // pin.port->OSPEEDR = temp;

  MODIFY_REG(pin.port->OSPEEDR, (0b11 << (2 * pin.pin)), ((uint32_t)(s) << (2 * pin.pin)));

  // temp = pin.port->PUPDR;
  // temp &= ~((0b11 << (2 * pin.pin)));
  // (temp |= (int(p) << (2 * pin.pin)));
  // pin.port->PUPDR = temp;
  MODIFY_REG(pin.port->PUPDR, (0b11 << (2 * pin.pin)), (uint32_t)(p) << (2 * pin.pin));

  // //uint32_t tmp[2]{0,0};
  const uint32_t id = pin.pin / 8;
  // temp = pin.port->AFR[id];
  //  // ((pin.pin < 8 ? temp1 : temp2) |= (0b11 << (4 * (pin.pin % 8)))); //
  // //  (0b11 << (4 * (pin.pin - (pin.pin < 8 ? 0 : 8))))); //
  // temp &= ~(0b11 << (4 * (pin.pin % 8)));
  // temp |= (int(a) << (4 * (pin.pin % 8)));
  // //(int(a) << (4 * (pin.pin - (pin.pin < 8 ? 0 : 8)))));
  // pin.port->AFR[id] = temp;
  MODIFY_REG(pin.port->AFR[id], 0b11 << (4 * (pin.pin % 8)), (uint32_t)a << (4 * (pin.pin % 8)));
}
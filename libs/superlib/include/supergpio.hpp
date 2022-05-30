#pragma once
#include "stm32f411xe.h"
#include "stm32f4xx.h"
#include "mcu_base.hpp"
#include <array>
namespace mcu {

namespace gpio {

enum class mode : uint32_t { analog = 0b11, input = 0b00, output = 0b01, output_af = 0b10 };
enum class type : uint32_t { PP = 0, OD = 1 };
enum class speed : uint32_t { low = 0, medium = 0b01, high = 0b10, very_high = 0b11 };
enum class pupd : uint32_t { nopupd = 0, pu = 0b01, pd = 0b10 };

enum class af : uint32_t {
  af0 = 0,
  af1,
  af2,
  af3,
  af4,
  af5,
  af6,
  af7,
  af8,
  af9,
  af10,
  af11,
  af12,
  af13,
  af14,
  af15
};
enum class pins : uint32_t {
  pin0 = 0,
  pin1,
  pin2,
  pin3,
  pin4,
  pin5,
  pin6,
  pin7,
  pin8,
  pin9,
  pin10,
  pin11,
  pin12,
  pin13,
  pin14,
  pin15
};
struct GPIO_pin {
  GPIO_TypeDef *port;
  uint8_t pin;
};
struct GPIO_pin_init {
  GPIO_TypeDef *port;
  uint32_t pin;
};
template <uint8_t... pins>
void init(GPIO_TypeDef *port, mode m = mode::input, type t = type::PP, speed s = speed::low,
          pupd p = pupd::nopupd, af a = af::af0);

template <uint32_t... pins> inline void set(GPIO_TypeDef *port);

template <uint32_t... pins> inline void reset(GPIO_TypeDef *port);

template <uint32_t... pins> inline void toggle(GPIO_TypeDef *port);

inline void init(const GPIO_pin &pin, const mode m = mode::input, const type t = type::PP,
                 const speed s = speed::low, const pupd p = pupd::nopupd, const af a = af::af0)
    __attribute((optimize("Os")));
inline void set(GPIO_pin &pin);
inline void reset(GPIO_pin &pin);
inline void toggle(GPIO_pin &pin);

namespace exti {
constexpr std::array<IRQn_Type, 16> irqn{
    EXTI0_IRQn,     /*!< EXTI Line0 Interrupt      */
    EXTI1_IRQn,     /*!< EXTI Line1 Interrupt      */
    EXTI2_IRQn,     /*!< EXTI Line2 Interrupt      */
    EXTI3_IRQn,     /*!< EXTI Line3 Interrupt      */
    EXTI4_IRQn,     /*!< EXTI Line4 Interrupt     */
    EXTI9_5_IRQn,   /*!< External Line[9:5] Interrupts   */
    EXTI9_5_IRQn,   /*!< External Line[9:5] Interrupts   */
    EXTI9_5_IRQn,   /*!< External Line[9:5] Interrupts   */
    EXTI9_5_IRQn,   /*!< External Line[9:5] Interrupts   */
    EXTI9_5_IRQn,   /*!< External Line[9:5] Interrupts   */
    EXTI15_10_IRQn, /*!< External Line[15:10] Interrupts */
    EXTI15_10_IRQn, /*!< External Line[15:10] Interrupts */
    EXTI15_10_IRQn, /*!< External Line[15:10] Interrupts */
    EXTI15_10_IRQn, /*!< External Line[15:10] Interrupts */
    EXTI15_10_IRQn, /*!< External Line[15:10] Interrupts */
    EXTI15_10_IRQn, /*!< External Line[15:10] Interrupts */
};
enum class edge {
  NONE = 0,
  RISING = 0b01,
  FALLING = 0b10,
  BOTH = 0b11,
};

// void exti_init(mcu::gpio::GPIO_pin pin, edge ee, mcu::vector_func_t handler)
// ;//__attribute((optimize("Os")));
void exti_init(mcu::gpio::GPIO_pin pin, const edge ee, mcu::vector_func_t const handler);
} // namespace exti
} // namespace gpio
} // namespace mcu

template <uint8_t... pins>
void mcu::gpio::init(GPIO_TypeDef *port, mode m, type t, speed s, pupd p, af a) {
  /*
   #ifdef GPIOA
     if (port == GPIOA)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
   #endif
   #ifdef GPIOB
     if (port == GPIOB)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
   #endif
   #ifdef GPIOC
     if (port == GPIOC)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
   #endif
   #ifdef GPIOD
     if (port == GPIOD)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
   #endif
   #ifdef GPIOE
     if (port == GPIOE)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
   #endif
   #ifdef GPIOF
     if (port == GPIOF)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
   #endif
   #ifdef GPIOG
     if (port == GPIOG)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
   #endif
   #ifdef GPIOI
     if (port == GPIOI)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
   #endif
   #ifdef GPIOJ
     if (port == GPIOJ) // for big F7
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
   #endif
   #ifdef GPIOK
     if (port == GPIOK)
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
   #endif
  */
  static_assert(((pins < 16) && ...), "pins should be in range [0,15]");

  uint16_t portn = ((uint32_t)port - (uint32_t)GPIOA);
  const uint32_t  sizeofport=1024;
  portn /= sizeofport;
  RCC->AHB1ENR |= 1 << portn;

  // ((temp1 |= (0b11 << (2 * pins))), ...);
  // port->MODER &= ~(temp1);
  // temp1 = 0;
  // ((temp1 |= int(m) << (2 * pins)), ...);
  // port->MODER |= temp1;
  MODIFY_REG(port->MODER, ((0b11 << (2 * pins)) | ...), ((int(m) << (2 * pins)) | ...));

  // temp1 = 0;
  // ((temp1 |= (0b1 << (pins))), ...);
  // port->OTYPER &= ~(temp1);
  // temp1 = 0;
  // ((temp1 |= (int(t) << (pins))), ...);
  // port->OTYPER |= temp1;
  MODIFY_REG(port->OTYPER, ((0b1 << (pins)) | ...), ((int(t) << (pins)) | ...));

  // temp1 = 0;
  // ((temp1 |= (0b11 << (2 * pins))), ...);
  // port->OSPEEDR &= ~(temp1);
  // temp1 = 0;
  // ((temp1 |= (int(s) << (2 * pins))), ...);
  // port->OSPEEDR |= temp1;
  MODIFY_REG(port->OSPEEDR, ((0b11 << (2 * pins)) | ...), ((int(s) << (2 * pins)) | ...));

  // temp1 = 0;
  // ((temp1 |= (0b11 << (2 * pins))), ...);
  // port->PUPDR &= ~(temp1);
  // temp1 = 0;
  // ((temp1 |= (int(p) << (2 * pins))), ...);
  // port->PUPDR |= temp1;
  MODIFY_REG(port->PUPDR, ((0b11 << (2 * pins)) | ...), ((int(p) << (2 * pins)) | ...));

  // temp1 = 0;
  // (((pins < 8 ? temp1 : temp2) |= (0b11 << (4 * (pins - (pins < 8 ? 0 : 8))))), ...);
  // port->AFR[0] &= ~temp1;
  // port->AFR[1] &= ~temp2;
  // temp1 = 0;
  // temp2 = 0;
  // (((pins < 8 ? temp1 : temp2) |= (int(a) << (4 * (pins - (pins < 8 ? 0 : 8))))), ...);
  // port->AFR[0] |= temp1;
  // port->AFR[1] |= temp2;
  uint32_t tmp[2]{port->AFR[0], port->AFR[1]};
  ((tmp[pins / 8] &= ~(0b11 << (4 * (pins % 8)))), ...);
  ((tmp[pins / 8] |= (int(a) << (4 * (pins % 8)))), ...);
  port->AFR[0] = tmp[0];
  port->AFR[1] = tmp[1];
}

template <uint32_t... pins> inline void mcu::gpio::set(GPIO_TypeDef *port) {
  // uint32_t t = 0;
  //((t |= 1 << pins), ...);
  // t=((0b1 << (pins)) | ...);

  // ((t |= 1 << pins), ...);
  static_assert(((pins < 16) && ...), "pins should be in range [0,15]");

  port->BSRR = ((0b1 << (pins)) | ...);
}

template <uint32_t... pins> inline void mcu::gpio::toggle(GPIO_TypeDef *port) {
  // uint32_t t = 0;
  // ((t |= 1 << pins), ...);
  // t=((0b1 << (pins)) | ...);
  static_assert(((pins < 16) && ...), "pins should be in range [0,15]");
  port->ODR ^= ((0b1 << (pins)) | ...);
}

template <uint32_t... pins> inline void mcu::gpio::reset(GPIO_TypeDef *port) {
  // uint32_t t=0;
  // ((t |= 1 << (pins + 16)), ...);
  // uint32_t t=((0b1 << (pins)) | ...)<<16;
  // t<<=16;
  static_assert(((pins < 16) && ...), "pins should be in range [0,15]");
  port->BSRR = ((0b1 << (pins)) | ...) << 16;
}

inline void mcu::gpio::set(GPIO_pin &pin) { pin.port->BSRR = (0b1 << (pin.pin)); }

inline void mcu::gpio::reset(GPIO_pin &pin) { pin.port->BSRR = (0b1 << (pin.pin)) << 16; }

inline void mcu::gpio::toggle(GPIO_pin &pin) { pin.port->ODR ^= (0b1 << (pin.pin)); }

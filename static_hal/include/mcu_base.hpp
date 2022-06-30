#ifndef MCU_BASE_HPP
#define MCU_BASE_HPP
#include "stm32f4xx.h"
#include "os_abstraction.hpp"

extern "C" void Default_Handler();
namespace mcu {

template <typename T> class periph_base;

extern const uint32_t vectors_len;

typedef void (*vector_func_t)();

// I dont know why but this makes smaller code even with global Os
inline void set_vector(IRQn_Type IRQn, vector_func_t handler) {
  uint32_t *vvectors = (uint32_t *)SCB->VTOR;
  vvectors[(int32_t)IRQn + NVIC_USER_IRQ_OFFSET] = (uint32_t)handler;
}

void set_ISR(IRQn_Type IRQn, uint8_t priority, void *obj, vector_func_t handler);

void register_obj(IRQn_Type, void *);

// only from ISR
void *get_periph_obj();

template <typename T> class periph_base {
private:
  periph_base(const periph_base &) = delete;
  periph_base( periph_base &&) = delete;
  periph_base() = delete;
  void operator=(const periph_base &) = delete;

  uint32_t *const rcc_enable_reg;
  const uint32_t rcc_enable_msk;

protected:
  const IRQn_Type irqn;

  periph_base(T *const p0, volatile uint32_t &rcc_enable_reg0, const uint32_t rcc_enable_msk0,
              const IRQn_Type irqn0, uint8_t id0 = 0)
      : rcc_enable_reg((uint32_t *)&rcc_enable_reg0), rcc_enable_msk(rcc_enable_msk0), irqn(irqn0),
        id(id0), periph(p0) {
    register_obj(irqn, this);
  }

  // if mcu has more than one peripheral this is it's id DMA1-->id=1
  // it is intended for use from peripheral logic
  const uint8_t id;
  os::os_abstraction os;

public:
  T *periph;
  void clk_enable() {
    (*rcc_enable_reg) |= rcc_enable_msk;
    (void)((uint32_t *)periph)[0];
  }
};

} // namespace mcu

#endif

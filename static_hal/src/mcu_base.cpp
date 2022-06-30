#include "mcu_base.hpp"

extern "C" {
extern unsigned int __vectors_start;
extern unsigned int __vectors_end;
}
namespace mcu {



void *periph_obj_index[128] = {nullptr};

void register_obj(IRQn_Type IRQn, void *pobj) {
  periph_obj_index[(int32_t)IRQn + NVIC_USER_IRQ_OFFSET] = pobj;
}


void set_ISR(IRQn_Type IRQn, uint8_t priority, void *pobj, vector_func_t handler) {
  set_vector(IRQn, handler);
  register_obj(IRQn, pobj);
  NVIC_SetPriority(IRQn, priority);
  NVIC_EnableIRQ(IRQn);
}

void *get_periph_obj() {
  uint32_t ISR_number = __get_IPSR();
  return periph_obj_index[ISR_number];
}

} // namespace mcu

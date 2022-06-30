#include "dma.hpp"

namespace mcu {

void dma_error_handler() {
  // TODO: error management (maybe)
  // for now
  Default_Handler();
}

void dma_handler() {
  auto s = (mcu::dma_stream *)mcu::get_periph_obj();
  auto status = s->get_status();
  auto e = s->get_event(status);
  if (e != mcu::dma_config::event::ERROR) {
    s->callback(s->obj, e);
    if (e == mcu::dma_config::event::TC) { s->is_busy = false; }
    s->clear_flags(e);
  } else {
    dma_error_handler();
  }
}

inline uint8_t get_id(DMA_TypeDef *p) {
  uint8_t id = 0;
  if (p == DMA1) {
    id = 1;
  } else if (p == DMA2) {
    id = 2;
  }
  return id;
}

dma_config::event dma_stream::get_event(uint8_t status) {

  dma_config::event e = dma_config::event::NONE;

  if (status & (DMA_LISR_HTIF0)) { e = dma_config::event::HTC; }
  if (status & (DMA_LISR_TCIF0_Msk)) { e = dma_config::event::TC; }
  if (status & (DMA_LISR_DMEIF0_Msk | DMA_LISR_FEIF0_Msk | DMA_LISR_TEIF0)) {
    e = dma_config::event::ERROR;
  }
  return e;
}

uint8_t dma_stream::get_status() {
  auto d = dma::dma_idx[dma_n];
  auto &n = this->stream_n;
  uint8_t shift = 0;
  uint8_t lohi = n / 4;
  switch (n % 4) {
  case 0: shift = 0; break;
  case 1: shift = 6; break;
  case 2: shift = 16; break;
  case 3: shift = 22; break;
  }
  uint32_t status = 0;
  switch (lohi) {
  case 0: status = (d->periph->LISR) >> shift; break;
  case 1: status = (d->periph->HISR) >> shift; break;
  }
  return status;
}

void dma_stream::clear_flags(dma_config::event e) {
  auto d = dma::dma_idx[this->dma_n];
  auto &n = this->stream_n;
  uint8_t shift = 0;
  uint8_t lohi = n / 4;
  uint32_t mask = 0;
  switch (e) {
  case dma_config::event::HTC: mask = 1 << 4; break;
  case dma_config::event::TC: mask = (0b111101); break;
  default: break;
  }
  switch (n % 4) {
  case 0: shift = 0; break;
  case 1: shift = 6; break;
  case 2: shift = 16; break;
  case 3: shift = 22; break;
  }
  switch (lohi) {
  case 0: d->periph->LIFCR = (mask << shift); break;
  case 1: d->periph->HIFCR = (mask << shift); break;
  }
}

 dma_stream::dma_stream(DMA_Stream_TypeDef *const pstr, uint8_t str_n, const IRQn_Type irqn)
    :  stream_n(str_n), is_busy(false), is_reserved(false),irq_n(irqn), pstream(pstr) {
    mcu::set_vector(irq_n, dma_handler);
    NVIC_SetPriority(irq_n, 6); // TODO: find a way to configure priorities
}


dma::dma(DMA_TypeDef *const p, volatile uint32_t &rcc_enable_reg0, const uint32_t rcc_enable_msk0,
         std::array<dma_stream, 8> &&s)
    : periph_base<DMA_TypeDef>(p, rcc_enable_reg0, rcc_enable_msk0, (IRQn_Type)0, get_id(p)),
      stream{s} {
  this->clk_enable();
  dma::dma_idx[this->id] = this;
  for (auto &st : stream) {
    st.dma_n = id;
    mcu::register_obj(st.irq_n, &st);

  }
}

} // namespace mcu

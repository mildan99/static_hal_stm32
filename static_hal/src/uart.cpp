
#include "uart.hpp"
#include "dma.hpp"
#include "mcu_clk.hpp"
//#include "supergpio.hpp"

void dma_tx_callback(void *p, mcu::dma_config::event e) {
  auto u = (mcu::uart *)p;
  if (e == mcu::dma_config::event::TC) {
    u->periph->CR3 &= ~USART_CR3_DMAT;
    // wait till comletion
    u->periph->SR &= ~USART_SR_TC;
    u->periph->CR1 |= USART_CR1_TCIE;
  } else {
    // get here on HT event, dma errors handled in dma module
    __NOP();
  }
}

void dma_rx_callback(void *p, mcu::dma_config::event e) {
  auto u = (mcu::uart *)p;
  if (e == mcu::dma_config::event::TC) {
    u->periph->CR3 &= ~USART_CR3_DMAR;
    // we recived all data
    u->resumeFromISR();
  } else {
    __NOP();
  }
}
namespace mcu {

uart_cfg::uart_ev uart::get_uart_event() {
  uint32_t s = this->periph->SR;
  auto e = uart_cfg::uart_ev::ERROR;
  if (!(s & (USART_SR_FE | USART_SR_NE | USART_SR_ORE | USART_SR_PE))) {
    if (s & USART_SR_TXE) { e = uart_cfg::uart_ev::TXE; }
    if (s & USART_SR_TC) { e = uart_cfg::uart_ev::TC; }
    if (s & USART_SR_RXNE) { e = uart_cfg::uart_ev::RXNE; }
  }
  return e;
}

void uart_handler() {
  using uart_t = mcu::uart;
  uart_t *p = (uart_t *)mcu::get_periph_obj();
  auto e = p->get_uart_event();
  if (e == uart_cfg::uart_ev::TXE) {

    if (p->IT_trans_stasus.pdat != p->IT_trans_stasus.pend) {
      USART1->DR = *(p->IT_trans_stasus.pdat++);
    } else {
      // TODO: it status is no longer polled so this flag aren't needed
      p->IT_trans_stasus.Competed = true;
      USART1->CR1 &= ~(USART_CR1_TXEIE);
    }
  } else if (e == uart_cfg::uart_ev::TC) {
    USART1->CR1 &= ~(USART_CR1_TCIE);
    p->periph->SR &= ~USART_SR_TC;
    p->os.resumeFromISR();
  } else if (e == uart_cfg::uart_ev::RXNE) {
    __NOP(); // TODO IMPLEMENT reception with dma/IT
    // actually should get here only when recive using IT, not dma
  }
}
// TODO use IRQn check instead
constexpr uint8_t get_id(IRQn_Type it_n) {
  uint8_t id = 0;
  switch (it_n) {
#ifdef USART1
  case USART1_IRQn: id=1; break;
  // if (p == USART1) { id = 1; }
#endif
#ifdef USART2
  case USART2_IRQn: id=2; break;
  // if (p == USART2) { id = 2; }
#endif
#ifdef USART3
  case USART3_IRQn: id=3; break;
  // if (p == USART3) { id = 3; }
#endif
#ifdef UART4
  case UART4_IRQn: id=4; break;
  // if (p == UART4) { id = 4; }
#endif
#ifdef UART5
  case UART5_IRQn: id=5; break;
  // if (p == UART5) { id = 5; }
#endif
#ifdef USART6
  case USART6_IRQn: id=6; break;
  // if (p == USART6) { id = 6; }
#endif
#ifdef UART7
  case UART7_IRQn: id=7; break;
  // if (p == UART7) { id = 7; }
#endif
#ifdef UART8
  case UART8_IRQn: id=8; break;
  // if (p == UART8) { id = 8; }
#endif
default: break;
  }
  return id;
}
 
uart::uart(USART_TypeDef *const p, volatile uint32_t &rcc_enable_reg0,
           const uint32_t rcc_enable_msk0, IRQn_Type irqn0)
    : periph_base<USART_TypeDef>(p, rcc_enable_reg0, rcc_enable_msk0, irqn0, get_id(irqn0)) {}

void uart::config(uint32_t baud,uart_cfg::stop_bit sb,uart_cfg::parity par) {

  periph->CR1 |= USART_CR1_UE;
  if (par!=uart_cfg::parity::NONE) {
 ;
  }
  periph->CR1 |= (USART_CR1_M) | USART_CR1_PCE;
  periph->CR2 |= 0 << USART_CR2_STOP_Pos;
  //TODO: add clock info (maybe from LL)
  uint32_t clk = ((id == 1) || id == 6) ? (SystemCoreClock) : SystemCoreClock / 2;
  periph->BRR =
      (clk / baud); // Tx\Rx baud= f_ck/(8*(2-OVER8))/USARTDIV or
                    // uart->BRR = (52 << 4) | (1); // Tx\Rx baud= f_ck/(8*(2-OVER8))/USARTDIV or
  mcu::set_vector(this->irqn, uart_handler);
  NVIC_SetPriority(this->irqn, 6);
  NVIC_EnableIRQ(this->irqn);
  periph->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

void uart::send_IT(uint8_t *pdata, uint16_t dsize) {
  while (!(periph->SR & USART_SR_TXE))
    ;
  IT_trans_stasus.Competed = false;
  IT_trans_stasus.pdat = pdata;
  IT_trans_stasus.len = dsize;
  IT_trans_stasus.pend = pdata + dsize;

  periph->CR1 |= USART_CR1_TXEIE;

  this->os.wait();
  while (!IT_trans_stasus.Competed) { __NOP(); }
}

// void uart::wait() {
//   //   this->thread=xTaskGetCurrentTaskHandle();
//   // ulTaskNotifyTake(true, portMAX_DELAY) ;
//   this->os.wait();
// }

 void uart::resumeFromISR() {
   this->os.resumeFromISR();
 }

void uart::send_dma(uint8_t *pdata, uint16_t dsize) {

  auto dmaC = dma::get_cfg(mcu::dma_config::routing::uartX_tx, this->id);
  if (dmaC.dma) {
    auto d = dma::dma_idx[dmaC.dma];
    auto &s = d->stream[dmaC.stream];
    mcu::dma_config::conf cfg;
    cfg.pbuf = (void *)&(periph->DR);
    cfg.m0buf = pdata;
    cfg.len = dsize;
    cfg.channel = dmaC.channel;
    cfg.dir = dma_config::t_dir::M2P;
    cfg.mem_size = dma_config::data_size::BYTE;
    cfg.per_size = dma_config::data_size::BYTE;
    cfg.mem_inc = true;
    cfg.mem_burst = dma_config::burst::BURST4;
    cfg.fsize = dma_config::fifo_size::WORDx4;
    cfg.fmode = dma_config::fifo_mode::FIFO;
    cfg.priority = dma_config::priority_t::MEDIUM;
    
    s.configure_transfer(cfg, this, dma_tx_callback);

    periph->CR3 |= USART_CR3_DMAT;

    // mcu::set_vector(DMA2_Stream7_IRQn, dma_s7_handler);
    // NVIC_SetPriority(DMA2_Stream7_IRQn,6);
    // NVIC_EnableIRQ(DMA2_Stream7_IRQn);

    this->os.wait();

  } else {
    send_IT(pdata, dsize);
  }
}

void uart::send_char(unsigned char c) {
  while (!(periph->SR & USART_SR_TXE))
    ;
  periph->DR = c;
  while (!(periph->SR & USART_SR_TXE))
    ;
}
} // namespace mcu
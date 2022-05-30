#pragma once
#include "mcu_base.hpp"

#define USART_AUTO_ID 1
namespace mcu {
namespace uart_cfg {

 struct IT_transfer 
 {
  uint8_t *pend;
  uint8_t *pdat;
  bool Competed; // true=TC
  uint8_t len;
};
constexpr auto ss = sizeof(IT_transfer);
enum class uart_ev { TC, TXE, RXNE, RXOVF, ERROR };
enum class parity { NONE, ODD, EVEN};
enum class stop_bit { STOP_1, STOP_2, STOP_0_5, STOP_1_5 };
//enum class data_bits { eight,nine }; //support only 8 bits mode

} // namespace uart_cfg
class uart : public periph_base<USART_TypeDef> {
private:
  friend void uart_handler();

  volatile uart_cfg::IT_transfer IT_trans_stasus;

  os::os_abstraction::queue<16, unsigned char> queue;
  // const uint8_t id; //if id is[1,2,3,6] it is usart and has spi mode
public:
   uart(USART_TypeDef *const p, volatile uint32_t &rcc_enable_reg, uint32_t rcc_enable_msk,
       const IRQn_Type irqn);
  void config(uint32_t baud,uart_cfg::stop_bit,uart_cfg::parity);
  void send_char(unsigned char);
  void send_dma(uint8_t *pdata, uint16_t dsize);
  void send_IT(uint8_t *pdata, uint16_t dsize);
  unsigned char get_char();
  void start_recive();
  uart_cfg::uart_ev get_uart_event();
  // void wait();
  void resumeFromISR(); // TODO: dont expose os
};


} // namespace mcu 

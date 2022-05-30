#pragma once
#include "mcu_base.hpp"
#include "dma_format.hpp"
#include <array>

namespace mcu {

namespace dma_config {

enum class t_dir : uint8_t { P2M = 0b00, M2P = 0b01, M2M = 0b11 };
enum class data_size : uint8_t { BYTE = 0, HALFWORD = 0b01, WORD = 0b10 };
enum class periph_align : uint8_t {
  ALIGN_BY_PSIZE = 0,
  ALIGN_BY_WORD = 1,
};

enum class fifo_mode : uint8_t {
  DIRECT = 0,
  FIFO = 1,
};

enum class priority_t : uint8_t { LOW = 0, MEDIUM = 0b01, HIGH = 0b10, VERY_HIGH = 0b11 };

enum class mode_t : uint8_t { NORMAL = 0, CIRCULAR = 1, DOUBLE_BUFFER = 3 };

enum class burst : uint8_t {
  NOBURST = 0,
  BURST4 = 0b01,
  BURST8 = 0b10,
  BURST16 = 0b11,
};

enum class fifo_size : uint8_t {
  WORD,
  WORDx2,
  WORDx3,
  WORDx4,
};

enum class event : uint8_t { NONE = 0, TC = 1, HTC = 2, ERROR = 3 };

struct conf {
  void *pbuf;
  void *m0buf;
  void *m1buf;
  uint16_t len;
  data_size mem_size;
  data_size per_size;
  t_dir dir;
  uint8_t channel;
  bool mem_inc;
  bool per_inc;
  priority_t priority;
  periph_align palign;
  fifo_mode fmode;
  fifo_size fsize;
  burst mem_burst;
  burst per_burst;
  mode_t mode;
  bool per_flow;
  conf()
      : pbuf(nullptr), m0buf(nullptr), m1buf(nullptr), len(0), mem_size(data_size::BYTE),
        per_size(data_size::BYTE), dir(t_dir::M2P), mem_inc(false), per_inc(false),
        priority(priority_t::LOW), palign(periph_align::ALIGN_BY_PSIZE), fmode(fifo_mode::DIRECT),
        fsize(fifo_size::WORDx4), mem_burst(burst::NOBURST), per_burst(burst::NOBURST),
        mode(mode_t::NORMAL), per_flow(false) {}
};
// constexpr auto s = sizeof(conf);

} // namespace dma_config

typedef void (*cb_func)(void *, dma_config::event);

class dma_stream {
  cb_func callback;
  void *obj;
  const uint8_t stream_n;

public:
  volatile bool is_busy;
  volatile bool is_reserved;
  uint8_t dma_n;
  const IRQn_Type irq_n;
  DMA_Stream_TypeDef *const pstream;
  // periph_base<DMA_TypeDef> *pdma;
 inline void configure_transfer(const dma_config::conf &cfg, void *obj, cb_func);
  dma_config::event get_event(uint8_t);
  uint8_t get_status();
  dma_stream(DMA_Stream_TypeDef *const pstr, uint8_t str_n, const IRQn_Type irq_n);

private:
  void operator=(const dma_stream &) = delete;
  dma_stream() = delete;
  friend void dma_handler();
  void clear_flags(dma_config::event);
};

class dma : public periph_base<DMA_TypeDef> {
public:
  std::array<dma_stream, 8> stream;
  inline static dma *dma_idx[3]{nullptr}; // index 0 means invalid cfg

  dma(DMA_TypeDef *const p, volatile uint32_t &rcc_enable_reg0, const uint32_t rcc_enable_msk0,
      std::array<dma_stream, 8> &&);

  template <size_t N, size_t M>
  inline static dma_config::routing::cfg
  get_cfg(const std::array<dma_config::routing::routes<N>, M> &periph_dma_arr, uint8_t id);
};

template <size_t N, size_t M>
dma_config::routing::cfg
dma::get_cfg(const std::array<dma_config::routing::routes<N>, M> &periph_dma_arr, uint8_t id) {
  auto route = periph_dma_arr[id - 1];
  // dma_config::routing::routes<N> route = periph_dma_arr[id - 1];
  dma_config::routing::cfg c{};
  for (auto cfg : route) {
    if (cfg.dma != 0) {
      dma *d = dma::dma_idx[cfg.dma];
      // bool busy = false;
      bool busy = ((d->stream[cfg.stream].is_busy) || (d->stream[cfg.stream].is_reserved));
      if (busy) {
        continue;
      } else {
        c = cfg;
        break;
      }
    } else {
      continue;
    }
  }
  return c;
}


void dma_stream::configure_transfer(const dma_config::conf& cfg, void *pobj, cb_func cb) {

  // TODO: should return error if busy, hovewer stream should be choden only if it is not busy
  // so maybe remove this guard instead
  //if (!is_busy) {
    this->obj = pobj;
    is_busy = true;
    pstream->CR &= ~(DMA_SxCR_EN_Msk);
    while (pstream->CR & DMA_SxCR_EN_Msk)
      ;

    clear_flags(dma_config::event::TC);
    pstream->PAR = (uint32_t)(cfg.pbuf);
    pstream->M0AR = (uint32_t)(cfg.m0buf);
    pstream->M1AR = (uint32_t)(cfg.m1buf);
    pstream->NDTR = cfg.len;

    uint32_t temp_reg = 0;
    temp_reg = (cfg.channel << DMA_SxCR_CHSEL_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.mem_inc) << DMA_SxCR_MINC_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.per_inc) << DMA_SxCR_PINC_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.per_size) << DMA_SxCR_PSIZE_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.mem_size) << DMA_SxCR_MSIZE_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.dir) << DMA_SxCR_DIR_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.priority) << DMA_SxCR_PL_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.mem_burst) << DMA_SxCR_MBURST_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.per_burst) << DMA_SxCR_PBURST_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.per_burst) << DMA_SxCR_PBURST_Pos);
    temp_reg |= (static_cast<uint32_t>(cfg.per_flow) << DMA_SxCR_PFCTRL_Pos);
    temp_reg |= ((static_cast<uint32_t>(cfg.mode)) & 0b1 << DMA_SxCR_CIRC_Pos);
    temp_reg |= (((static_cast<uint32_t>(cfg.mode)) >> 1) << DMA_SxCR_DBM_Pos);
    temp_reg |= (DMA_SxCR_TCIE | DMA_SxCR_HTIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);
    pstream->CR = temp_reg;

    temp_reg = 0;
    temp_reg |= ((uint32_t)cfg.fmode << DMA_SxFCR_DMDIS);
    temp_reg |= ((uint32_t)cfg.fsize << DMA_SxFCR_FTH_Pos);
    temp_reg |= (DMA_SxFCR_FEIE);
    pstream->FCR = temp_reg;
    callback = cb;


    NVIC_EnableIRQ(irq_n);
    pstream->CR |= DMA_SxCR_EN_Msk;
 // } //and this
}


} // namespace mcu

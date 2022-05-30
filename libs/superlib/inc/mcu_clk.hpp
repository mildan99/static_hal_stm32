#pragma once
#include "mcu_base.hpp"

namespace mcu {
namespace clk {
constexpr uint32_t hsi_freq = 16'000'000;

enum class pllsrc {
  HSE,
  HSI,
};
enum class sysclk_src {
  HSI,
  HSE,
  PLLCLK_P //  PLLCLK_P = (HSE/M)*N/P
};

enum class apbX_div {
  DIV1 = 0,
  DIV2 = 0b100,
  DIV4 = 0b101,
  DIV8 = 0b110,
  DIV16 = 0b111,
};

enum class ahb_div {
  DIV1 = 0,
  DIV2 = 0b1000,
  DIV4 = 0b1001,
  DIV8 = 0b1010,
  DIV16 = 0b1011,
  DIV64 = 0b1100,
  DIV128 = 0b1101,
  DIV256 = 0b1110,
  DIV512 = 0b1111,
};

struct clk_cfg {
  sysclk_src sysclk_s;
  clk::pllsrc pll_s;
  uint8_t M;
  uint16_t N;
  uint8_t P;
  uint8_t Q;
  ahb_div AHBDIV;
  apbX_div APB1DIV; // 50MHZ max
  apbX_div APB2DIV; // 100MHZ max
};
constexpr uint8_t get_apbX_divider(const apbX_div cfg_div) {
  uint8_t div = 0;
  switch (cfg_div) {
  case apbX_div::DIV1: div = 1; break;
  case apbX_div::DIV2: div = 2; break;
  case apbX_div::DIV4: div = 4; break;
  case apbX_div::DIV8: div = 8; break;
  case apbX_div::DIV16: div = 16; break;
  }
  return div;
}

constexpr uint32_t get_ahb_divider(const ahb_div cfg_div) {
  uint32_t div = 0;
  switch (cfg_div) {
  case ahb_div::DIV1: div = 1; break;
  case ahb_div::DIV2: div = 2; break;
  case ahb_div::DIV4: div = 4; break;
  case ahb_div::DIV8: div = 8; break;
  case ahb_div::DIV16: div = 16; break;
  case ahb_div::DIV64: div = 64; break;
  case ahb_div::DIV128: div = 128; break;
  case ahb_div::DIV256: div = 256; break;
  case ahb_div::DIV512: div = 512; break;
  }
  return div;
}

constexpr uint32_t get_pll_freq(uint32_t hse_freq, const clk_cfg cfg) {
  uint64_t speed = 0;
  switch (cfg.pll_s) {
  case pllsrc::HSI: speed = hsi_freq;
  case pllsrc::HSE: speed = hse_freq;
  }
  speed = speed * cfg.N / cfg.M / cfg.P;
  return speed;
}

constexpr uint32_t get_hclk_freq(uint32_t hse_freq, const clk_cfg cfg) {
  uint64_t speed = 0;
  switch (cfg.sysclk_s) {
  case sysclk_src::HSI: speed = (16'000'000ul); break;
  case sysclk_src::HSE: speed = (hse_freq); break;
  case sysclk_src::PLLCLK_P: speed = get_pll_freq(hse_freq, cfg); break;
  }
  return speed / get_ahb_divider(cfg.AHBDIV);
}

constexpr uint32_t get_apb2_freq(uint32_t hse_freq, const clk_cfg cfg) {
  uint64_t speed = get_hclk_freq(hse_freq, cfg);

  return speed / get_apbX_divider(cfg.APB2DIV);
}

constexpr uint8_t get_flash_ws(uint32_t core_clk_mhz) {
  uint8_t ws = 0;
  uint16_t t[]{30, 64, 90, 120, 150, 180, 210}; // TODO: adapt to different tables
  for (auto fclk : t) {
    if (core_clk_mhz > fclk) { ++ws; }
  }
  return ws;
}

inline void set_flash_latency(uint8_t ws) {
  FLASH->ACR |= (ws << FLASH_ACR_LATENCY_Pos | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN);
  while (!((FLASH->ACR & FLASH_ACR_LATENCY) == (ws << FLASH_ACR_LATENCY_Pos)))
    ;
}

} // namespace clk
} // namespace mcu

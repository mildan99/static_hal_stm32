#pragma once
#include <array>
#include <cstdint>

namespace mcu {
namespace dma_config {
namespace routing {
struct cfg {
  std::uint8_t dma;
  std::uint8_t stream;
  std::uint8_t channel;
};
template <size_t N> using routes = std::array<cfg, N>;
// enum class source {
//   adc1,
//   i2c1_rx,
//   i2c1_tx,
//   i2c2_rx,
//   i2c2_tx,
//   i2c3_rx,
//   i2c3_tx,
//   i2s2_ext_rx,
//   i2s2_ext_tx,
//   i2s3_ext_rx,
//   i2s3_ext_tx,
//   sdio,
//   spi1_rx,
//   spi1_tx,
//   spi2_rx,
//   spi2_tx,
//   spi3_rx,
//   spi3_tx,
//   spi4_rx,
//   spi4_tx,
//   spi5_rx,
//   spi5_tx,
//   tim1_ch1,
//   tim1_ch2,
//   tim1_ch3,
//   tim1_ch4,
//   tim1_com,
//   tim1_trig,
//   tim1_up,
//   tim2_ch1,
//   tim2_ch2,
//   tim2_ch3,
//   tim2_ch4,
//   tim2_up,
//   tim3_ch1,
//   tim3_ch2,
//   tim3_ch3,
//   tim3_ch4,
//   tim3_trig,
//   tim3_up,
//   tim4_ch1,
//   tim4_ch2,
//   tim4_ch3,
//   tim4_up,
//   tim5_ch1,
//   tim5_ch2,
//   tim5_ch3,
//   tim5_ch4,
//   tim5_trig,
//   tim5_up,
//   usart1_rx,
//   usart1_tx,
//   usart2_rx,
//   usart2_tx,
//   usart6_rx,
//   usart6_tx,
// };

constexpr routes<2> adc1{{{2, 0, 0}, {2, 4, 0}}};
//constexpr int i=sizeof(cfg);//3 bytes
constexpr routes<2> i2c1_rx{{{1, 0, 1}, {1, 5, 1}}};
constexpr routes<2> i2c2_rx{{{1, 2, 7}, {1, 3, 7}}};
constexpr routes<2> i2c3_rx{{{1, 1, 1}, {1, 2, 3}}};
constexpr std::array i2cX_rx{&i2c1_rx,&i2c2_rx,&i2c3_rx};

constexpr routes<3> i2c1_tx{{{1, 1, 0}, {1, 6, 1}, {1, 7, 1}}};
constexpr routes<3> i2c2_tx{{{1, 7, 7}}};
constexpr routes<3> i2c3_tx{{{1, 4, 3}, {1, 5, 6}}};
constexpr std::array i2cX_tx{i2c1_tx,i2c2_tx,i2c3_tx};

constexpr routes<1> i2s1_ext_rx{};
constexpr routes<1> i2s2_ext_rx{{{1, 3, 3}}};
// constexpr routes<2> i2s3_ext_rx{{{1, 2, 2}, {1, 0, 3}}};
constexpr routes<1> i2s3_ext_rx{{{1, 0, 3}}};
constexpr std::array i2sX_ext_rx{i2s1_ext_rx,i2s2_ext_rx,i2s3_ext_rx};

constexpr routes<1> i2s1_ext_tx{};
constexpr routes<1> i2s2_ext_tx{{{1, 4, 2}}};
constexpr routes<1> i2s3_ext_tx{{{1, 5, 2}}};
constexpr std::array i2sX_ext_tx{i2s1_ext_tx,i2s2_ext_tx,i2s3_ext_tx};

constexpr routes<2> sdio{{{2, 6, 4},{2, 3, 4}}};

constexpr routes<2> spi1_rx{{{2, 0, 3}, {2, 2, 3}}};
constexpr routes<2> spi2_rx{{{1, 3, 0}}};
constexpr routes<2> spi3_rx{{{1, 0, 0}, {1, 2, 0}}};
constexpr routes<2> spi4_rx{{{2, 0, 4}, {2, 3, 5}}};
// constexpr routes<3> spi4_rx{{{2, 0, 4}, {2, 4, 4}, {2, 3, 5}}};
constexpr routes<2> spi5_rx{{{2, 3, 2}, {2, 5, 7}}};
constexpr std::array spiX_rx{spi1_rx,spi2_rx,spi3_rx,spi4_rx,spi5_rx};

constexpr routes<3> spi1_tx{{{2, 2, 2}, {2, 3, 3}, {2, 5, 3}}};
constexpr routes<3> spi2_tx{{{1, 4, 0}}};
constexpr routes<3> spi3_tx{{{1, 5, 0}, {1, 7, 0}}};
constexpr routes<3> spi4_tx{{{2, 1, 4}, {2, 4, 5}}};
constexpr routes<3> spi5_tx{{{2, 4, 2}, {2, 5, 5}, {2, 6, 7}}};
constexpr std::array spiX_tx{spi1_tx,spi2_tx,spi3_tx,spi4_tx,spi5_tx};

// constexpr routes<3> tim1_ch1{{{2, 6, 0}, {2, 1, 6}, {2, 3, 6}}};
// constexpr routes<3> tim1_ch2{{{2, 6, 0}, {2, 2, 6}}};
// constexpr routes<3> tim1_ch3{{{2, 6, 0}, {2, 6, 6}}};
constexpr routes<1> tim1_ch1{{ {2, 1, 6}}};
constexpr routes<1> tim1_ch2{{ {2, 2, 6}}};
constexpr routes<1> tim1_ch3{{ {2, 6, 6}}};
constexpr routes<1> tim1_ch4{{{2, 4, 6}}};
constexpr std::array tim1_chY{tim1_ch1,tim1_ch2,tim1_ch3,tim1_ch4};

constexpr routes<1> tim1_com{{{2, 4, 6}}};
constexpr routes<2> tim1_trig{{{2, 0, 6}, {2, 4, 6}}};
constexpr routes<1> tim1_up{{{2, 5, 6}}};

constexpr routes<1> tim2_ch1{{{1, 5, 3}}};
constexpr routes<1> tim2_ch2{{{1, 6, 3}}};
constexpr routes<1> tim2_ch3{{{1, 1, 3}}};
constexpr routes<1> tim2_ch4{{{1, 7, 3}}};
constexpr std::array tim2_chY{tim2_ch1,tim2_ch2,tim2_ch3,tim2_ch4};

constexpr routes<2> tim2_up{{{1, 1, 3}, {1, 7, 3}}};

constexpr routes<1> tim3_ch1{{{1, 4, 5}}};
constexpr routes<1> tim3_ch2{{{1, 5, 5}}};
constexpr routes<1> tim3_ch3{{{1, 7, 5}}};
constexpr routes<1> tim3_ch4{{{1, 2, 5}}};
constexpr std::array tim3_chY{tim3_ch1,tim3_ch2,tim3_ch3,tim3_ch4};

constexpr routes<1> tim3_trig{{{1, 4, 5}}};
constexpr routes<1> tim3_up{{{1, 2, 5}}};

constexpr routes<1> tim4_ch1{{{1, 0, 2}}};
constexpr routes<1> tim4_ch2{{{1, 3, 2}}};
constexpr routes<1> tim4_ch3{{{1, 7, 2}}};
constexpr routes<1> tim4_ch4{};
constexpr std::array tim4_chY{tim4_ch1,tim4_ch2,tim4_ch3,tim4_ch4};

constexpr routes<1> tim4_up{{{1, 6, 2}}};

constexpr routes<1> tim5_ch1{{{1, 2, 6}}};
constexpr routes<1> tim5_ch2{{{1, 4, 6}}};
constexpr routes<1> tim5_ch3{{{1, 0, 6}}};
constexpr routes<1> tim5_ch4{{{1, 1, 6}}};
constexpr std::array tim5_chY{tim5_ch1,tim5_ch2,tim5_ch3,tim5_ch4};

constexpr routes<2> tim5_trig{{{1, 1, 6}, {1, 3, 6}}};
constexpr routes<2> tim5_up{{{1, 0, 6}, {1, 6, 6}}};

constexpr std::array timX_chY{tim1_chY,tim2_chY,tim3_chY,tim4_chY,tim5_chY};


constexpr routes<2> usart1_rx{{{2, 2, 4}, {2, 5, 4}}};
constexpr routes<2> usart2_rx{{{1, 5, 4}, {1, 7, 6}}};
constexpr routes<2> uart3_rx{};
constexpr routes<2> uart4_rx{};
constexpr routes<2> uart5_rx{};
constexpr routes<2> usart6_rx{{{2, 1, 5}, {2, 2, 5}}};
constexpr std::array uartX_rx{usart1_rx,usart2_rx,uart3_rx,uart4_rx,uart5_rx,usart6_rx};


constexpr routes<1> usart1_tx{{{2, 7, 4}}};
constexpr routes<1> usart2_tx{{{1, 6, 4}}};
constexpr routes<1> uart3_tx{};
constexpr routes<1> uart4_tx{};
constexpr routes<1> uart5_tx{};
// constexpr routes<1> usart6_tx{{{2, 6, 5}, {2, 7, 5}}};
constexpr routes<1> usart6_tx{{{2, 7, 5}}};
constexpr std::array uartX_tx{usart1_tx,usart2_tx,uart3_tx,uart4_tx,uart5_tx,usart6_tx};

// constexpr int i=sizeof(uartX_tx);//3 bytes per cfg
// constexpr auto j =i2cX_tx[1][2];//3 bytes


} // namespace routing
} // namespace dma
} // namespace mcu

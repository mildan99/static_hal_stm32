#ifndef BASIC_FORMAT_HPP
#define BASIC_FORMAT_HPP
#include <string_view>
#include <ranges>
#include <charconv>

template <class T> auto print_dev(auto printer, std::string_view sv, T arg) {
  std::array<char, ((static_cast<int>(sizeof(arg) * 2.40823996531184956) + 1))> str;
  if (auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(), arg); ec == std::errc()) {
    printer(sv.data(),sv.size());
    printer(str.data(),
            static_cast<size_t>(ptr - str.data())); // C++17 alt, using string_view(ptr, length)
  }
  return 0;
}

#endif // BASIC_FORMAT_HPP

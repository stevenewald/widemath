#pragma once

#include <cstdint>
namespace wm
{
struct uint128
{
  std::uint32_t high;
  std::uint32_t low;

  uint128() = default;

  explicit uint128(std::uint32_t val) : high{0}, low{val} {}
};

}  // namespace wm

#pragma once

#include "widemath/util.hpp"
#include "widemath/widemath_export.hpp"

#include <cmath>
#include <compare>

#include <limits>
#include <stdexcept>

namespace wm {
// 128-bit over/underflow is well-defined and uses modulo arithmetic
struct WIDEMATH_EXPORT uint128 {
    using underlying = unsigned long long;
    static_assert(sizeof(underlying) == 8);

    underlying low;
    underlying high;

    constexpr uint128() = default;

    constexpr uint128(__uint128_t val) :
        low{static_cast<underlying>(val)}, high{static_cast<underlying>(val >> 64)}
    {}

    constexpr uint128(underlying high_bits, underlying low_bits) :
        low{low_bits}, high{high_bits}
    {}

    explicit operator uint128::underlying() const noexcept { return low; }

    explicit operator __uint128_t() const noexcept
    {
        __uint128_t high_bits = high;
        high_bits <<= 64;
        return high_bits + low;
    }
};

} // namespace wm

namespace std {
template <>
struct numeric_limits<wm::uint128> {
    static constexpr wm::uint128 max()
    {
        return wm::uint128{
            numeric_limits<wm::uint128::underlying>::max(),
            numeric_limits<wm::uint128::underlying>::max()
        };
    }

    static constexpr wm::uint128 min() { return wm::uint128{0}; }
};
} // namespace std

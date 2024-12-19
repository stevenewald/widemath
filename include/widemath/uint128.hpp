#pragma once

#include "widemath/carryless_multiplication.hpp"
#include "widemath/util.hpp"
#include "widemath/widemath_export.hpp"

#include <cmath>
#include <compare>

#include <limits>
#include <stdexcept>

namespace wm {
class WIDEMATH_EXPORT uint128 {
// 128-bit over/underflow is well-defined and uses modulo arithmetic
public:
    // Cannot use uint64_t for __builtin_overflow compatibility
    using underlying = unsigned long long;
    static_assert(sizeof(underlying) == 8);

    underlying high;
    underlying low;

    constexpr uint128() = default;

    constexpr uint128(__uint128_t val) :
        high{static_cast<underlying>(val >> 64)}, low{static_cast<underlying>(val)}
    {}

    constexpr uint128(underlying high_bits, underlying low_bits) :
        high{high_bits}, low{low_bits}
    {}

    uint128& operator++();
    uint128 operator++(int);

    uint128& operator--();
    uint128 operator--(int);

    std::strong_ordering operator<=>(const uint128& other) const;
    bool operator==(const uint128& other) const = default;

    uint128& operator*=(const uint128& other) noexcept
    {
        auto [new_low_bits, overflow] = carryless_multiply(low, other.low);

        underlying new_high_bits = (high * other.low) + (low * other.high) + overflow;

        low = new_low_bits;
        high = new_high_bits;

        return *this;
    }

    uint128& operator+=(const uint128& other) noexcept
    {
        if (__builtin_uaddll_overflow(low, other.low, &low)) {
            ++high;
        }
        high += other.high;

        return *this;
    }

    uint128& operator-=(const uint128& other) noexcept
    {
        if (__builtin_usubll_overflow(low, other.low, &low)) {
            --high;
        }
        high -= other.high;

        return *this;
    }

    explicit operator underlying() const;
    explicit operator __uint128_t() const;

private:
    friend WIDEMATH_EXPORT uint128
    operator*(const uint128& first, const uint128& second);
    friend WIDEMATH_EXPORT uint128
    operator+(const uint128& first, const uint128& second);
    friend WIDEMATH_EXPORT uint128
    operator-(const uint128& first, const uint128& second);
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

#pragma once

#include "widemath/widemath_export.hpp"

#include <cmath>
#include <compare>

#include <limits>

namespace wm {
// 128-bit over/underflow is well-defined and will throw an exception
class WIDEMATH_EXPORT uint128 {
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

    uint128& operator*=(const uint128& other);
    uint128& operator+=(const uint128& other);
    uint128& operator-=(const uint128& other);

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

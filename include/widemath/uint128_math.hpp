#pragma once

#include "widemath/uint128.hpp"

namespace wm {
inline uint128& operator++(uint128& num) noexcept
{
    if (__builtin_uaddll_overflow(num.low, 1, &num.low)) {
        ++num.high;
    }
    return num;
}

inline uint128& operator--(uint128& num) noexcept
{
    if (__builtin_usubll_overflow(num.low, 1, &num.low)) {
        --num.high;
    }
    return num;
}

inline uint128 operator++(uint128& num, int) noexcept
{
    uint128 ret = num;
    ++num;
    return ret;
}

inline uint128 operator--(uint128& num, int) noexcept
{
    uint128 ret = num;
    --num;
    return ret;
}

inline uint128& operator+=(uint128& num, const uint128& other) noexcept
{
    if (__builtin_uaddll_overflow(num.low, other.low, &num.low)) {
        ++num.high;
    }
    num.high += other.high;

    return num;
}

inline uint128& operator-=(uint128& num, const uint128& other) noexcept
{
    if (__builtin_usubll_overflow(num.low, other.low, &num.low)) {
        --num.high;
    }
    num.high -= other.high;

    return num;
}

struct carryless_uint64_product {
    unsigned long long product;
    unsigned long long overflow;
};

inline carryless_uint64_product
carryless_multiply(unsigned long long first, unsigned long long second) noexcept
{
    using ull = unsigned long long;
    ull high_bits = (first >> 32) * (second >> 32);
    ull mid_bits_p1 = (first & 0xFFFFFFFF) * (second >> 32);
    ull mid_bits_p2 = (first >> 32) * (second & 0xFFFFFFFF);
    ull low_bits = (first & 0xFFFFFFFF) * (second & 0xFFFFFFFF);

    ull product = low_bits;
    ull overflow = high_bits;
    if (__builtin_uaddll_overflow(product, mid_bits_p1 << 32, &product)) {
        ++overflow;
    }
    if (__builtin_uaddll_overflow(product, mid_bits_p2 << 32, &product)) {
        ++overflow;
    }

    overflow += (mid_bits_p1 >> 32);
    overflow += (mid_bits_p2 >> 32);

    return {product, overflow};
}

inline uint128& operator*=(uint128& first, const uint128& other) noexcept
{
    auto [new_low_bits, overflow] = carryless_multiply(first.low, other.low);

    uint128::underlying new_high_bits =
        (first.high * other.low) + (first.low * other.high) + overflow;

    first.low = new_low_bits;
    first.high = new_high_bits;

    return first;
}

inline uint128 operator+(const uint128& first, const uint128& second) noexcept
{
    uint128 res = first;
    res += second;
    return res;
}

inline uint128 operator-(const uint128& first, const uint128& second) noexcept
{
    uint128 res = first;
    res -= second;
    return res;
}

inline uint128 operator*(const uint128& first, const uint128& second) noexcept
{
    uint128 res = first;
    res *= second;
    return res;
}

inline std::strong_ordering
operator<=>(const uint128& first, const uint128& second) noexcept
{
    std::strong_ordering cmp = first.high <=> second.high;
    if (cmp != std::strong_ordering::equal) {
        return cmp;
    }

    return first.low <=> second.low;
}

inline bool operator==(const uint128& first, const uint128& second) noexcept
{
    return first.low == second.low && first.high == second.high;
}
} // namespace wm

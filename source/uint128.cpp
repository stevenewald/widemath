#include "widemath/uint128.hpp"

#include <compare>

#include <stdexcept>

namespace wm {

uint128& uint128::operator++()
{
    if (__builtin_uaddll_overflow(low, 1, &low)) {
        if (high == std::numeric_limits<underlying>::max()) {
            throw std::overflow_error("OVERFLOW");
        }
        ++high;
    }
    return *this;
}

uint128 uint128::operator++(int)
{
    uint128 ret = *this;
    ++*this;
    return ret;
}

uint128 uint128::operator--(int)
{
    uint128 ret = *this;
    --*this;
    return ret;
}

uint128& uint128::operator--()
{
    if (__builtin_usubll_overflow(low, 1, &low)) {
        if (high == std::numeric_limits<underlying>::min()) {
            throw std::underflow_error("UNDERFLOW");
        }
        --high;
    }
    return *this;
}

std::strong_ordering uint128::operator<=>(const uint128& other) const
{
    std::strong_ordering cmp = high <=> other.high;
    if (cmp != std::strong_ordering::equal) {
        return cmp;
    }

    return low <=> other.low;
}

uint128& uint128::operator*=(const uint128& other)
{
    if (high != 0 && other.high != 0) {
        throw std::overflow_error("Multiplication caused 128-bit overflow");
    }

    underlying new_high_bits = (high * other.low) + (low * other.high);

    underlying low_high_bits = (low >> 32) * (other.low >> 32);
    underlying low_mid_bits_p1 = (low & 0xFFFFFFFFULL) * (other.low >> 32);
    underlying low_mid_bits_p2 = (low >> 32) * (other.low & 0xFFFFFFFFULL);
    underlying low_low_bits = (low & 0xFFFFFFFFULL) * (other.low & 0xFFFFFFFFULL);

    underlying new_low_bits = low_low_bits;
    if (__builtin_uaddll_overflow(new_low_bits, low_mid_bits_p1 << 32, &new_low_bits)) {
        ++new_high_bits;
    }
    if (__builtin_uaddll_overflow(new_low_bits, low_mid_bits_p2 << 32, &new_low_bits)) {
        ++new_high_bits;
    }

    new_high_bits += low_high_bits;
    new_high_bits += (low_mid_bits_p1 >> 32);
    new_high_bits += (low_mid_bits_p2 >> 32);

    high = new_high_bits;
    low = new_low_bits;

    return *this;
}

uint128& uint128::operator+=(const uint128& other)
{
    high += other.high;
    if (__builtin_uaddll_overflow(low, other.low, &low)) {
        if (high == std::numeric_limits<underlying>::max()) {
            throw std::overflow_error("OVERFLOW");
        }
        ++high;
    }

    return *this;
}

uint128& uint128::operator-=(const uint128& other)
{
    high -= other.high;
    if (__builtin_usubll_overflow(low, other.low, &low)) {
        if (high == std::numeric_limits<underlying>::min()) {
            throw std::underflow_error("UNDERFLOW");
        }
        --high;
    }

    return *this;
}

uint128::operator uint128::underlying() const
{
    if (high != 0) {
        throw std::overflow_error("Overflow during narrowing cast");
    }
    return low;
}

uint128::operator __uint128_t() const
{
    __uint128_t high_bits = high;
    high_bits <<= 64;
    return high_bits + low;
}

uint128 operator+(const uint128& first, const uint128& second)
{
    uint128 res = first;
    res += second;
    return res;
}

uint128 operator-(const uint128& first, const uint128& second)
{
    uint128 res = first;
    res -= second;
    return res;
}

uint128 operator*(const uint128& first, const uint128& second)
{
    uint128 res = first;
    res *= second;
    return res;
}
} // namespace wm

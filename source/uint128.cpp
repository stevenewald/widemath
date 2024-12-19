#include "widemath/uint128.hpp"

#include "widemath/carryless_multiplication.hpp"

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

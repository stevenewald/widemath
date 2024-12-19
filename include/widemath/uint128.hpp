#pragma once

#include "util.hpp"

#include <cmath>

#include <limits>
#include <stdexcept>

namespace wm {
struct uint128 {
    using underlying = unsigned long long;

    underlying high;
    underlying low;

    constexpr uint128() = default;

    constexpr uint128(__uint128_t val) :
        high{static_cast<underlying>(val >> 64)}, low{static_cast<underlying>(val)}
    {}

    constexpr uint128(underlying high_bits, underlying low_bits) :
        high{high_bits}, low{low_bits}
    {}

    uint128& operator++()
    {
        if (__builtin_uaddll_overflow(low, 1, &low)) {
            if (high == std::numeric_limits<underlying>::max()) {
                throw std::runtime_error("OVERFLOW");
            }
            ++high;
        }
        return *this;
    }

    void operator--()
    {
        if (__builtin_usubll_overflow(low, 1, &low)) {
            if (high == std::numeric_limits<underlying>::min()) {
                throw std::runtime_error("UNDERFLOW");
            }
            --high;
        }
    }

    uint128& operator*=(const uint128& other)
    {
        if (high != 0 && other.high != 0) {
            throw std::runtime_error("Multiplication caused 128-bit overflow");
        }

        underlying new_high_bits = (high * other.low) + (low * other.high);

        underlying low_high_bits = (low >> 32) * (other.low >> 32);
        underlying low_mid_bits_p1 = (low & 0xFFFFFFFFULL) * (other.low >> 32);
        underlying low_mid_bits_p2 = (low >> 32) * (other.low & 0xFFFFFFFFULL);
        underlying low_low_bits = (low & 0xFFFFFFFFULL) * (other.low & 0xFFFFFFFFULL);

        underlying new_low_bits = low_low_bits;
        if (__builtin_uaddll_overflow(
                new_low_bits, low_mid_bits_p1 << 32, &new_low_bits
            )) {
            ++new_high_bits;
        }
        if (__builtin_uaddll_overflow(
                new_low_bits, low_mid_bits_p2 << 32, &new_low_bits
            )) {
            ++new_high_bits;
        }

        new_high_bits += low_high_bits;
        new_high_bits += (low_mid_bits_p1 >> 32);
        new_high_bits += (low_mid_bits_p2 >> 32);

        high = new_high_bits;
        low = new_low_bits;

        return *this;
    }

    uint128& operator+=(const uint128& other)
    {
        high += other.high;
        if (__builtin_uaddll_overflow(low, other.low, &low)) {
            if (high == std::numeric_limits<underlying>::max()) {
                throw std::runtime_error("OVERFLOW");
            }
            ++high;
        }

        return *this;
    }

    uint128& operator-=(const uint128& other)
    {
        high -= other.high;
        if (__builtin_usubll_overflow(low, other.low, &low)) {
            if (high == std::numeric_limits<underlying>::min()) {
                throw std::runtime_error("UNDERFLOW");
            }
            --high;
        }

        return *this;
    }

    explicit operator underlying() const
    {
        if (high != 0) {
            throw std::runtime_error("Overflow during narrowing cast");
        }
        return low;
    }

    explicit operator __uint128_t() const
    {
        __uint128_t high_bits = high;
        high_bits <<= 64;
        return high_bits + low;
    }

private:
    // TODO: handwrite these
    friend uint128 operator+(const uint128& first, const uint128& second)
    {
        uint128 res = first;
        res += second;
        return res;
    }

    friend uint128 operator-(const uint128& first, const uint128& second)
    {
        uint128 res = first;
        res -= second;
        return res;
    }

    friend uint128 operator*(const uint128& first, const uint128& second)
    {
        uint128 res = first;
        res *= second;
        return res;
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

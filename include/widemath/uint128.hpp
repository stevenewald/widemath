#pragma once

#include "util.hpp"

#include <cstdint>

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

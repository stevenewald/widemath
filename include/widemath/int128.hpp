#pragma once

#include "util.hpp"

#include <cstdint>

namespace wm {
struct uint128 {
    unsigned long long high;
    unsigned long long low;

    uint128() = default;

    uint128(std::uint64_t val) : high{0}, low{val} {}

private:
    uint128(unsigned long long high_bits, unsigned long long low_bits) :
        high{high_bits}, low{low_bits}
    {}

    friend uint128 operator+(const uint128& first, const uint128& second)
    {
        assert(first.high + second.high >= first.high);
        assert(first.high + second.high >= second.high);

        unsigned long long high_bits = first.high + second.high;
        unsigned long long low_bits;
        if (__builtin_uaddll_overflow(first.low, second.low, &low_bits)) {
            ++high_bits;
        }

        return {high_bits, low_bits};
    }

    friend uint128 operator-(const uint128& first, const uint128& second)
    {
        assert(first.high >= second.high);

        unsigned long long high_bits = first.high - second.high;
        unsigned long long low_bits;
        if (__builtin_usubll_overflow(first.low, second.low, &low_bits)) {
            --high_bits;
        }

        return {high_bits, low_bits};
    }
};

} // namespace wm

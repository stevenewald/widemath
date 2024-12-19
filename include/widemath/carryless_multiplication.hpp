#pragma once

namespace wm {
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
} // namespace wm

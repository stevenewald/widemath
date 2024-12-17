# Widemath - VERY WIP

WideMath is a library designed to provide efficient support for 128-bit arithmetic, encompassing both integer and floating-point operations. Recognizing that native 128-bit arithmetic is rarely supported by modern hardware architectures, WideMath implements these operations in software. 

# Motivation

While many excellent arbitrary precision arithmetic libraries exist, few prioritize SIMD acceleration beyond compiler-driven auto-vectorization. WideMath distinguishes itself by treating SIMD support as a first-class citizen, enabling significantly faster 128-bit arithmetic operations through explicit, hand-optimized vectorized implementations.

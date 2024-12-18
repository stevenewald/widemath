#include "widemath/uint128.hpp"

#include <catch2/catch_test_macros.hpp>

#include <limits>

TEST_CASE("uint128 is trivial", "[uint128]")
{
    static_assert(std::is_trivial_v<wm::uint128>);
}

TEST_CASE("correctly adds small numbers", "[uint128]")
{
    wm::uint128 five{5};
    wm::uint128 six{6};

    wm::uint128 res = five + six;
    CHECK(static_cast<wm::uint128::underlying>(res) == 11);
}

TEST_CASE("correctly adds with 64-bit overflow", "[uint128]")
{
    wm::uint128 max = std::numeric_limits<wm::uint128::underlying>::max();

    wm::uint128 res = max + 1;
    CHECK(res.low == 0);
    CHECK(res.high == 1);

    res = max + 2;
    CHECK(res.low == 1);
    CHECK(res.high == 1);
}

TEST_CASE("pre-increment correctly increments small numbers", "[uint128]")
{
    wm::uint128 num{8};
    ++num;

    CHECK(static_cast<wm::uint128::underlying>(num) == 9);
}

TEST_CASE("pre-increment correctly increments with 64-bit overflow", "[uint128]")
{
    wm::uint128 num{std::numeric_limits<wm::uint128::underlying>::max()};
    ++num;

    CHECK(num.high == 1);
    CHECK(num.low == 0);

    ++num;
    CHECK(num.high == 1);
    CHECK(num.low == 1);
}

TEST_CASE("pre-decrement correctly decrements without overflow", "[uint128]")
{
    wm::uint128 num{8};
    --num;

    CHECK(static_cast<wm::uint128::underlying>(num) == 7);
}

TEST_CASE("pre-decrement correctly decrements with overflow", "[uint128]")
{
    wm::uint128 num{std::numeric_limits<wm::uint128::underlying>::max()};
    ++num;

    CHECK(num.high == 1);
    CHECK(num.low == 0);
    --num;

    CHECK(num.high == 0);
    CHECK(num.low == std::numeric_limits<wm::uint128::underlying>::max());
}

TEST_CASE("throws from 128 bit underflow", "[uint128]")
{
    wm::uint128 min = std::numeric_limits<wm::uint128>::min();
    CHECK_THROWS(min - 1);
    CHECK_THROWS(--min);
}

TEST_CASE("throws from 128 bit overflow", "[uint128]")
{
    wm::uint128 max = std::numeric_limits<wm::uint128>::max();
    CHECK_THROWS(max + 1);
    CHECK_THROWS(++max);
}

TEST_CASE("narrowing cast refuses if overflow present", "[uint128]")
{
    wm::uint128 max_64{std::numeric_limits<wm::uint128::underlying>::max()};
    max_64 += 1;

    CHECK_THROWS(static_cast<wm::uint128::underlying>(max_64));
}

TEST_CASE(
    "100 adds from 0 to uint128_max/2 is consistent with builtin uint128", "[uint128]"
)
{
    constexpr __uint128_t max_bound = std::numeric_limits<__uint128_t>::max() / 2;
    constexpr __uint128_t min_bound = std::numeric_limits<__uint128_t>::min();
    constexpr __uint128_t block = (max_bound - min_bound) / 100;

    wm::uint128 widemath_val = std::numeric_limits<wm::uint128>::min();

    for (__uint128_t builtin_val = min_bound; builtin_val < max_bound;
         builtin_val += block) {
        CHECK(static_cast<__uint128_t>(widemath_val) == builtin_val);
        widemath_val += block;
    }
}

TEST_CASE(
    "100 subtracts from uint128_max/2 to 0 is consistent with builtin uint128",
    "[uint128]"
)
{
    constexpr __uint128_t max_bound = std::numeric_limits<__uint128_t>::max();
    constexpr __uint128_t min_bound = max_bound / 10;
    constexpr __uint128_t block = (max_bound - min_bound) / 100;

    wm::uint128 widemath_val = std::numeric_limits<wm::uint128>::max();

    for (__uint128_t builtin_val = max_bound; builtin_val > min_bound;
         builtin_val -= block) {
        CHECK(static_cast<__uint128_t>(widemath_val) == builtin_val);
        widemath_val -= block;
    }
}

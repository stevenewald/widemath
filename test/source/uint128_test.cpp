#include "widemath/uint128_math.hpp"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#include <limits>

static constexpr wm::uint128 MAX_UINT64 =
    std::numeric_limits<wm::uint128::underlying>::max();
static constexpr wm::uint128 MAX_UINT128 = std::numeric_limits<wm::uint128>::max();
static constexpr wm::uint128 MIN_UINT128 = std::numeric_limits<wm::uint128>::min();

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
    wm::uint128 res = MAX_UINT64 + 1;
    CHECK(res.low == 0);
    CHECK(res.high == 1);

    res = MAX_UINT64 + 2;
    CHECK(res.low == 1);
    CHECK(res.high == 1);
}

TEST_CASE("pre-increment correctly increments small numbers", "[uint128]")
{
    wm::uint128 num{8};

    CHECK(static_cast<wm::uint128::underlying>(++num) == 9);
}

TEST_CASE("post-increment correctly increments small numbers", "[uint128]")
{
    wm::uint128 num{8};

    CHECK(static_cast<wm::uint128::underlying>(num++) == 8);
    CHECK(static_cast<wm::uint128::underlying>(num) == 9);
}

TEST_CASE("spaceship operator correctly compares small numbers", "[uint128]")
{
    wm::uint128 eight{8};
    CHECK(eight <=> eight == std::strong_ordering::equal);

    wm::uint128 nine{9};

    CHECK(eight <=> nine == std::strong_ordering::less);
    CHECK(nine <=> eight == std::strong_ordering::greater);
}

TEST_CASE("spaceship operator correctly compares large numbers", "[uint128]")
{
    wm::uint128 max64_plus_eight = MAX_UINT64 + 8;
    CHECK(max64_plus_eight <=> max64_plus_eight == std::strong_ordering::equal);

    wm::uint128 max64_plus_nine = MAX_UINT64 + 9;

    CHECK(max64_plus_eight <=> max64_plus_nine == std::strong_ordering::less);
    CHECK(max64_plus_nine <=> max64_plus_eight == std::strong_ordering::greater);
}

TEST_CASE("spaceship operator correctly compares small to large numbers", "[uint128]")
{
    wm::uint128 nine = 9;
    wm::uint128 max64_plus_eight = MAX_UINT64 + 8;

    CHECK(nine <=> max64_plus_eight == std::strong_ordering::less);
    CHECK(max64_plus_eight <=> nine == std::strong_ordering::greater);
}

TEST_CASE("pre-increment correctly increments with 64-bit overflow", "[uint128]")
{
    wm::uint128 num = MAX_UINT64 + 1;

    CHECK(num.high == 1);
    CHECK(num.low == 0);

    ++num;
    CHECK(num.high == 1);
    CHECK(num.low == 1);
}

TEST_CASE("pre-decrement correctly decrements small numbers", "[uint128]")
{
    wm::uint128 num{8};
    CHECK(static_cast<wm::uint128::underlying>(--num) == 7);
}

TEST_CASE("post-decrement correctly decrements small numbers", "[uint128]")
{
    wm::uint128 num{8};
    CHECK(static_cast<wm::uint128::underlying>(num--) == 8);
    CHECK(static_cast<wm::uint128::underlying>(num) == 7);
}

TEST_CASE("pre-decrement correctly decrements with overflow", "[uint128]")
{
    wm::uint128 num = MAX_UINT64 + 1;

    CHECK(num.high == 1);
    CHECK(num.low == 0);
    --num;

    CHECK(num.high == 0);
    CHECK(num.low == std::numeric_limits<wm::uint128::underlying>::max());
}

TEST_CASE("128 bit underflow uses modulo arithmetic correctly", "[uint128]")
{
    wm::uint128 min = MIN_UINT128;
    CHECK(min - 1 == MAX_UINT128);
    CHECK(--min == MAX_UINT128);
}

TEST_CASE(
    "128 bit multiplication overflow correctly uses modulo arithmetic", "[uint128]"
)
{
    wm::uint128 result_wm = MAX_UINT64 * MAX_UINT64;

    __uint128_t result_builtin = std::numeric_limits<uint64_t>::max();
    result_builtin *= result_builtin;

    CHECK(result_wm == result_builtin);
    CHECK(result_wm * result_wm == result_builtin * result_builtin);
}

TEST_CASE("128 bit overflow uses modulo arithmetic correctly", "[uint128]")
{
    wm::uint128 max = MAX_UINT128;
    CHECK(max + 1 == MIN_UINT128);
    CHECK(++max == MIN_UINT128);
}

TEST_CASE("narrowing cast only returns lower bits", "[uint128]")
{
    wm::uint128 max_64{std::numeric_limits<wm::uint128::underlying>::max()};
    max_64 += 1;

    CHECK(static_cast<wm::uint128::underlying>(max_64) == 0);
}

TEST_CASE(
    "100 adds from 0 to uint128_max/2 is consistent with builtin uint128", "[uint128]"
)
{
    constexpr __uint128_t max_bound = std::numeric_limits<__uint128_t>::max() / 2;
    constexpr __uint128_t min_bound = std::numeric_limits<__uint128_t>::min();
    constexpr __uint128_t block = (max_bound - min_bound) / 100;

    wm::uint128 widemath_val = MIN_UINT128;

    for (__uint128_t builtin_val = min_bound; builtin_val < max_bound;
         builtin_val += block) {
        CHECK(static_cast<__uint128_t>(widemath_val) == builtin_val);
        widemath_val += block;
    }
}

TEST_CASE(
    "100 squares from 0 to uint64_max is consistent with builtin uint128", "[uint128]"
)
{
    __uint128_t max_bound = std::numeric_limits<uint64_t>::max();
    __uint128_t min_bound = std::numeric_limits<__uint128_t>::min();
    __uint128_t block = (max_bound - min_bound) / 1000;

    for (__uint128_t value = min_bound; value < max_bound; value += block) {
        wm::uint128 val{value};
        wm::uint128 squared = val * val;
        REQUIRE(static_cast<__uint128_t>(squared) == value * value);
    }
}

TEST_CASE(
    "100 asymmetric multiples from 0 to uint64_max is consistent with builtin uint128",
    "[uint128]"
)
{
    constexpr __uint128_t max_bound = std::numeric_limits<uint64_t>::max();
    constexpr __uint128_t min_bound = std::numeric_limits<__uint128_t>::min();
    constexpr __uint128_t block = (max_bound - min_bound) / 1000;

    for (__uint128_t value = min_bound; value < max_bound; value += block) {
        __uint128_t left = value;
        __uint128_t right = max_bound - value;
        wm::uint128 left_wm{left};
        wm::uint128 right_wm{right};
        REQUIRE(static_cast<__uint128_t>(left_wm * right_wm) == left * right);
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

    wm::uint128 widemath_val = MAX_UINT128;

    for (__uint128_t builtin_val = max_bound; builtin_val > min_bound;
         builtin_val -= block) {
        CHECK(static_cast<__uint128_t>(widemath_val) == builtin_val);
        widemath_val -= block;
    }
}

TEST_CASE("multiplying two small uint128's yields correct value", "[uint128]")
{
    wm::uint128 five{5};
    wm::uint128 six{6};

    CHECK(static_cast<wm::uint128::underlying>(five * six) == 30);
}

TEST_CASE("multiplying with low bit overflow yields correct value", "[uint128]")
{
    __uint128_t max_64_builtin{std::numeric_limits<wm::uint128::underlying>::max()};

    CHECK(static_cast<__uint128_t>(MAX_UINT64 * 5) == max_64_builtin * 5);
}

TEST_CASE("multiplying with no low bit overflow yields correct value", "[uint128]")
{
    wm::uint128 max_64_plus_1 = MAX_UINT64 + 1;

    wm::uint128 five{5};

    __uint128_t max_64_builtin{std::numeric_limits<wm::uint128::underlying>::max()};
    ++max_64_builtin;

    CHECK(static_cast<__uint128_t>(max_64_plus_1 * five) == max_64_builtin * 5);
}

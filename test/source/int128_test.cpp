#include "widemath/int128.hpp"

#include <catch2/catch_test_macros.hpp>

#include <limits>

TEST_CASE("uint128 is trivial", "[uint128]")
{
    static_assert(std::is_trivial_v<wm::uint128>);
}

TEST_CASE("non-overflow correctly adds", "[uint128]")
{
    wm::uint128 five{5};
    wm::uint128 six{6};

    wm::uint128 res = five + six;
    CHECK(res.low == 11);
    CHECK(res.high == 0);
}

TEST_CASE("overflow correctly adds", "[uint128]")
{
    wm::uint128 max_64{std::numeric_limits<unsigned long long>::max()};
    wm::uint128 int2{1};

    wm::uint128 res = max_64 + 1;
    CHECK(res.low == 0);
    CHECK(res.high == 1);

    res = max_64 + 2;
    CHECK(res.low == 1);
    CHECK(res.high == 1);
}

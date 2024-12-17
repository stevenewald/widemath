#include "widemath/int128.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("uint128 is trivial", "[uint128]")
{
  static_assert(std::is_trivial_v<wm::uint128>);
}

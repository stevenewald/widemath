#include <string>

#include "widemath/widemath.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Name is widemath", "[library]")
{
  auto const exported = exported_class {};
  REQUIRE(std::string("widemath") == exported.name());
}

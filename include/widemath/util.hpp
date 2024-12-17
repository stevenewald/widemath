#pragma once

#include <stdexcept>

namespace wm {
constexpr void assert(bool expr)
{
#ifdef NDEBUG
    if (!expr) {
        std::unreachable();
    }
#else
    if (!expr) [[unlikely]] {
        throw std::runtime_error("Assertion error");
    }
#endif
}
} // namespace wm

#pragma once

#include <stdexcept>
#include <utility>

namespace wm {
inline void assert_true(bool expr, const std::string& err = "Assertion error")
{
#ifdef NDEBUG
    if (!expr) {
        std::unreachable();
    }
#else
    if (!expr) [[unlikely]] {
        throw std::runtime_error(err);
    }
#endif
}
} // namespace wm

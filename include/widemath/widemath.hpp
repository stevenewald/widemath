#pragma once

#include "widemath/widemath_export.hpp"

#include <string>

class WIDEMATH_EXPORT exported_class {
public:
    exported_class();

    auto name() const -> char const*;

private:
    WIDEMATH_SUPPRESS_C4251
    std::string m_name;
};

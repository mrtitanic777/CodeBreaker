// cb/java/strview.hpp -- `contains` for text (std::string_view::contains is C++23; this tree is C++20).
#pragma once

#include <string_view>

namespace cb::java {

inline bool contains(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }
inline bool contains(std::string_view s, char c) { return s.find(c) != std::string_view::npos; }

}  // namespace cb::java

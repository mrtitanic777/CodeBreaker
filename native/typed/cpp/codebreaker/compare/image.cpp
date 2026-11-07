// codebreaker/compare/image.cpp -- C++ from codebreaker.compare.image by CodeBreaker's Python front end.

#include "codebreaker/compare/image.hpp"

namespace codebreaker::compare::image {

// The offset of the first byte where `a` and `b` differ; where one ends first, its length.
// -> None when they are the same.
std::optional<int64_t> first_difference(const py::bytes& a, const py::bytes& b) {
    if (a == b) {
        return std::nullopt;
    }
    int64_t n = py::min(py::len(a), py::len(b));
    int64_t first = n;
    for (int64_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            first = i;
            break;
        }
    }
    return first;
}

}  // namespace codebreaker::compare::image

// Internal helpers shared by area C's .cpp files (not part of the contract; move to src/java/decompiler next to helpers.cpp at integration).
#pragma once

#include <stdexcept>
#include <vector>

#include "java/decompiler/exprents.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler::helpers_internal {

// stat.getExprents() where the original goes on to use it: a null list is its NullPointerException.
inline std::vector<Exprent*>& exprents_of(Statement* stat) {
    auto& e = stat->get_exprents();
    if (!e) throw std::runtime_error("NullPointerException: statement has no expressions");
    return *e;
}

// A pointer the original dereferences: null is its NullPointerException.
template <class T>
inline T* nn(T* p) {
    if (!p) throw std::runtime_error("NullPointerException");
    return p;
}

// `new ArrayList<Exprent>()` as a statement's expression list.
inline std::vector<Exprent*> empty_list() { return {}; }

// The i-th element of a vector (the original's List.get: std::out_of_range past the end).
template <class V>
inline decltype(auto) at(V&& v, int i) {
    if (i < 0 || static_cast<std::size_t>(i) >= v.size()) throw std::out_of_range("IndexOutOfBoundsException");
    return v[static_cast<std::size_t>(i)];
}

}  // namespace cb::java::decompiler::helpers_internal

// translate/backends/cpp17_tree/entry.hpp -- the C++17 back end on the typed tree, answering the L2 core library's C++17 entry points
// (cpp17_unit, cpp17_program) in-process: the same arguments (the JSON text the library reads) and the same answers (its text, its
// status), so native_l2::Library hands such a call here instead of to the library (unless CB_CPP17_TREE=0) and nothing else sees a
// difference.
//
//     if (cpp17tree::enabled() && cpp17tree::handles(fn)) auto [status, answer] = cpp17tree::call(fn, arguments_json);
#pragma once

#include <string>
#include <string_view>
#include <utility>

namespace cb::translate::cpp17tree {

// Whether the C++17 back end on the tree answers the library's C++17 entry points: the default, unless CB_CPP17_TREE=0 (then the library does).
bool enabled();

// Whether `fn` is one of the C++17 back end's entry points (cpp17_unit, cpp17_program).
bool handles(std::string_view fn);

// The call answered as the library answers it: {0, the answer} or {1, the error}, the answer JSON in the library's tagged form.
std::pair<int, std::string> call(std::string_view fn, std::string_view arguments_json);

}  // namespace cb::translate::cpp17tree

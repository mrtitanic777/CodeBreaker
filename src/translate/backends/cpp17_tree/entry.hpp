// translate/backends/cpp17_tree/entry.hpp -- the C++17 back end on the typed tree, answering its entry points (cpp17_unit,
// cpp17_program) in-process: the arguments as JSON text, the answer as JSON text in the tagged form with its status; native_l2::Library
// hands such a call here.
//
//     if (cpp17tree::handles(fn)) auto [status, answer] = cpp17tree::call(fn, arguments_json);
#pragma once

#include <string>
#include <string_view>
#include <utility>

namespace cb::translate::cpp17tree {

// Whether `fn` is one of the C++17 back end's entry points (cpp17_unit, cpp17_program).
bool handles(std::string_view fn);

// The call answered: {0, the answer} or {1, the error ({"type", "message", "facts"})}, the answer JSON in the tagged form.
std::pair<int, std::string> call(std::string_view fn, std::string_view arguments_json);

}  // namespace cb::translate::cpp17tree

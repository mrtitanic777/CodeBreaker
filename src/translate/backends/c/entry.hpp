// translate/backends/c/entry.hpp -- the C back end on the typed tree, answering its entry points in-process: the arguments as JSON
// text, the answer as JSON text in the tagged form with its status; native_l2::Library hands such a call here.
//
//     if (ctree::handles(fn)) auto [status, answer] = ctree::call(fn, arguments_json);
#pragma once

#include <string>
#include <string_view>
#include <utility>

namespace cb::translate::ctree {

// Whether `fn` is one of the C back end's entry points (function, flags, the module planners, the type queries, module, fragment,
// expr, condition ...), not the C++17 back end's or the L2 core's.
bool handles(std::string_view fn);

// The call answered: {0, the answer} or {1, the error ({"type", "message", "facts"})}, the answer JSON in the tagged form.
std::pair<int, std::string> call(std::string_view fn, std::string_view arguments_json);

}  // namespace cb::translate::ctree

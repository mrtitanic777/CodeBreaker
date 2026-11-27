// translate/backends/c/entry.hpp -- the C back end on the typed tree, answering the L2 core library's C entry points in-process: the
// same arguments (the JSON text the library reads) and the same answers (its text, its status), so native_l2::Library hands a call
// here instead of to the library (unless CB_C_TREE=0) and nothing else sees a difference.
//
//     if (ctree::handles(fn)) auto [status, answer] = ctree::call(fn, arguments_json);
#pragma once

#include <string>
#include <string_view>
#include <utility>

namespace cb::translate::ctree {

// Whether the C back end on the tree answers the library's C entry points: yes, unless CB_C_TREE=0 (then the library does).
bool enabled();

// Whether `fn` is one of the C back end's entry points (function, flags, the module planners, the type queries, module, fragment,
// expr, condition ...), not the library's C++17 back end or its L2 core.
bool handles(std::string_view fn);

// The call answered as the library answers it: {0, the answer} or {1, the error}, the answer JSON in the library's tagged form.
std::pair<int, std::string> call(std::string_view fn, std::string_view arguments_json);

}  // namespace cb::translate::ctree

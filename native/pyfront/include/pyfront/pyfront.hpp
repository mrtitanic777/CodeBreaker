// pyfront/pyfront.hpp -- parse Python 3 source into a pyfront::Module (C++ API).
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "pyfront/ast.hpp"

namespace pyfront {

// A syntax error, positioned as CPython's SyntaxError: lineno 1-based, offset 1-based
// (counted in characters, as CPython counts it); end_lineno/end_offset 0 when unknown.
struct SyntaxError {
    std::string msg;
    std::string filename;
    int lineno = 0, offset = 0, end_lineno = 0, end_offset = 0;
    std::string text;   // the offending source line
};

struct ParseResult {
    std::unique_ptr<Module> module;     // set on success
    std::optional<SyntaxError> error;   // set on failure
    explicit operator bool() const { return module != nullptr; }
};

// Parse a whole file (`exec` mode).  `source` is the file's bytes (UTF-8, optional BOM,
// any line endings).
ParseResult parse(std::string_view source, std::string_view filename = "<unknown>");

// The canonical text form: exactly CPython 3.14's
// `ast.dump(tree, include_attributes=include_attributes)`.
std::string dump(const Module& m, bool include_attributes = true);
std::string dump(const Expr* e, bool include_attributes = true);
std::string dump(const Stmt* s, bool include_attributes = true);

}  // namespace pyfront

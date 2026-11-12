// cb/translate/frontends/clang/ctypes.hpp -- clang's type spellings ("const char *", "int (*)[4]", "geo::Vec &") as IR
// types, and the sizes and layouts the source's compiler gives them.
//
// clang's JSON AST writes an expression's or declaration's type as text (`qualType`), so the front end reads C's
// declarator grammar back: specifiers, then an abstract declarator (pointers bind looser than array and function suffixes;
// a parenthesised part applies last). Typedef names, struct/union/class tags and C++ class names are resolved through the
// `NameResolver` the lowering supplies, so a type is always concrete: `uint8_t` is u8, `Pair` is the record it names.
//
// The data model is the target's: `Model::for_triple("x86_64-pc-windows-msvc")` is LLP64 (long is 4 bytes, pointers 8,
// plain char signed, long double = double), which is what the judge's clang build runs.
//
// A spelling that cannot be read is a ParseError; a type whose size or layout is not known here (a record the resolver
// does not have, __int128, a half float) is an UnsupportedError.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/translate/ir.hpp"

namespace cb::translate::frontends::clang::ctypes {

// Widths (in bytes) the target gives the types C leaves open.
struct Model {
    std::string triple;
    std::uint32_t pointer = 8;
    std::uint32_t long_bytes = 4;
    std::uint32_t wchar = 2;
    bool char_signed = true;
    std::uint32_t long_double = 8;

    static Model for_triple(std::string_view triple);
    ir::CompilerTarget target(std::string_view language, std::string_view standard) const;
};

// ['unsigned', 'long', 'long'] -> u64, by the model. UnsupportedError for __int128 and half floats.
ir::TypeP builtin_type(const std::vector<std::string>& words, const Model& model);

// The tokens of a type spelling, with clang's attributes, anonymous-type and lambda notes already turned into names.
// ParseError for a character that is no part of a type.
std::vector<std::string> tokens(std::string_view spelling);

// "3_9": the line and column clang named an anonymous record by.
std::string anon_key(std::int64_t line, std::int64_t column);

// What a name means: a typedef's meaning, or the class type of a record or class (with its template arguments); an empty
// TypeP for a name that is not known. `tag` is struct, union, class or enum when the spelling said so.
using NameResolver = std::function<ir::TypeP(const std::string& name, const std::vector<ir::TypeP>& args, const std::optional<std::string>& tag)>;

// Reads type spellings. The results are remembered by spelling, so one parser serves one lowering (and its resolver).
class Parser {
public:
    Parser(Model model, NameResolver names);
    ir::TypeP parse(const std::string& text);
    void clear_cache() { cache_.clear(); }
    const Model& model() const { return model_; }

private:
    struct Cursor {
        std::vector<std::string> tokens;
        std::size_t at = 0;
    };
    const std::string* peek(std::size_t ahead = 0) const;
    std::string take();
    void expect(std::string_view want);
    ir::TypeP type_();
    ir::TypeP specifiers();
    std::pair<std::string, std::vector<ir::TypeP>> qualified_name();
    std::vector<ir::TypeP> template_args();
    ir::TypeP declarator(ir::TypeP t);
    ir::TypeP suffixes(ir::TypeP t);

    Model model_;
    NameResolver names_;
    std::unordered_map<std::string, ir::TypeP> cache_;
    Cursor cur_;
};

// A function type: result and parameter types (and their names, where given), cdecl.
ir::TypeP fn_type(const ir::TypeP& result, const std::vector<ir::TypeP>& params, bool variadic = false,
                  const std::vector<std::string>& names = {});

// A function type as a pointer to it; an array type as a pointer to its element.
ir::TypeP decay(const ir::TypeP& t);

// Every pointer to a function as an fnptr, all the way down.
ir::TypeP normalise(const ir::TypeP& t);

// sizeof / alignof by the model; the record behind a class type is asked of the resolver (empty: not known).
class Layouts {
public:
    using RecordResolver = std::function<ir::TypeP(const ir::TypeP& class_type)>;
    Layouts(Model model, RecordResolver record) : model_(std::move(model)), record_(std::move(record)) {}

    std::uint64_t size(const ir::TypeP& t) const { return size_align(t).first; }
    std::uint64_t align(const ir::TypeP& t) const { return size_align(t).second; }
    std::pair<std::uint64_t, std::uint64_t> size_align(const ir::TypeP& t) const;

    // A record's alignment: its widest field's (L2 keeps offsets and size, not the alignment).
    std::uint64_t record_align(const ir::RecordType& record) const;

    struct Field {
        std::string name;
        ir::TypeP type;
    };
    // fields -> the record type, by C's rules: each field at the next multiple of its alignment, the whole padded to the
    // largest alignment (a union: every field at 0, the size the largest).
    ir::TypeP lay_out(const std::string& name, const std::vector<Field>& fields, bool is_union = false) const;

private:
    Model model_;
    RecordResolver record_;
};

}  // namespace cb::translate::frontends::clang::ctypes

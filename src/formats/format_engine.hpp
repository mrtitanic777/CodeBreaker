// formats/format_engine.hpp -- the one file-format engine: a binary format is a description (config/formats/<name>.fmt),
// the engine reads a file through it into a generic tree and writes the tree back.
//
// A description is a few lines per structure, Kaitai-Struct-like:
//
//     format mz
//     endian le
//     struct File {
//         magic        : str(2) == "MZ" | "ZM"          # a magic: the read fails on anything else
//         reloc_count  : u16
//         reloc_offset : u16
//         relocs @ reloc_offset : Reloc[reloc_count]    # `@ expr`: a pointer (the file offset of the structure)
//     }
//     struct Reloc { offset : u16  segment : u16 }
//
// Types: u8 u16 u32 u64 i8..i64 (little-endian by the format's `endian`, or `be`/`le` as a suffix: u32be), str(n)
// (n raw bytes of text), bytes(n), or the name of a struct. Arrays: T[count], T[until cond] (cond sees `_`, the
// element just read), T[size n] (elements while fewer than n bytes are consumed). After the type: `as Name` (an
// enum or flags: the tree shows the names), `== a | b` (magic), `size(n)` (the field takes exactly n bytes: its
// contents may not read past them, the cursor moves past all n), `if expr` (the field is only there when true).
// Statements: `let name = expr` (a computed value, not in the file), `if expr { ... } else { ... }`,
// `assert expr` (or `assert expr : "what is wrong"`: the words of the error). Expressions are C's: names of earlier fields (and of the enclosing structures' fields), a.b, a[i],
// $pos (the cursor), $start (where this structure starts), $end, $remaining, $size (the file), _root, _parent,
// _index, min(a,b) max(a,b) align(v,a) count(array), peek8(o) peek16(o) peek32(o) peek16be(o) peek32be(o) (the file's
// raw bytes at an offset, -1 outside it: a header that sits at one of several places), sum8(o,n) (the n raw bytes at
// offset o added: a header checksum) and printable(o,n) (how many of them are printable ASCII). `use name` brings in
// another description's structures.
//
// The tree keeps every field's offset and size. write() puts the values back: onto the bytes that were read (so what
// the description does not cover stays as it was; an unmodified tree gives the same bytes), or, with
// `covered_only`, onto zeros (only what the description names; a check that the description covers what it says).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::engine {

enum class Kind { Struct, Array, Int, Str, Bytes, Let };

struct Node {
    std::string name;
    Kind kind = Kind::Struct;
    std::string type;               // Struct: the struct's name; Int: "u16", "i32be"...
    std::uint64_t offset = 0;       // in the file (a Let has none)
    std::uint64_t size = 0;         // bytes the node covers (a Struct or Array: from its first to its last byte)
    std::uint64_t value = 0;        // Int, Let: the value (a signed one sign-extended to 64 bits)
    unsigned width = 0;             // Int: bytes
    bool big_endian = false;        // Int
    bool is_signed = false;         // Int
    std::string text;               // Str: the stored bytes (raw, with any NULs); Bytes: only after set_bytes
    std::string label;              // Int: the enum name, or the flags "a|b" (what `as` gave)
    bool edited = false;            // Str/Bytes: `text` replaces what was read
    std::vector<Node> children;     // Struct: its fields and lets in order; Array: its elements

    const Node* child(std::string_view child_name) const;
    Node* child(std::string_view child_name);
    // A path: "nt.coff.machine", "sections[2].name". Throws NotFoundError-like cb::FormatError when it is not there.
    const Node& get(std::string_view path) const;
    Node& get(std::string_view path);
    const Node* find(std::string_view path) const;   // nullptr instead of an error
    std::uint64_t u(std::string_view path) const { return get(path).value; }
    std::int64_t i(std::string_view path) const;      // an Int or Let as signed
    std::string str(std::string_view path) const;     // Str: the text up to its first NUL
    std::int64_t as_signed() const;
};

// Replace a Str / Bytes node's contents (same length as it covers: the file's layout does not move).
void set_text(Node& node, std::string_view text);
void set_bytes(Node& node, ByteView data);

struct Tree {
    std::string format;
    Node root;
    Bytes source;   // the bytes that were read: what write() writes onto
};

// The file's bytes a Bytes / Str / Int node covers (a view into tree.source; edits are not applied).
inline ByteView bytes_of(const Tree& tree, const Node& node) { return ByteView(tree.source).subspan(node.offset, node.size); }

// Every node, depth first, with its path.
void walk(const Node& node, const std::function<void(const std::string& path, const Node&)>& visit,
          const std::string& prefix = "");

// Bytes of the file the leaves of the tree cover (each byte once).
std::uint64_t covered_bytes(const Tree& tree);

// The tree as text: one line per node, `path @offset +size : value (label)`. Arrays show the first `max_elements`.
std::string dump(const Tree& tree, std::size_t max_elements = 8);

// The tree back to bytes. See the notes at the top.
Bytes write(const Tree& tree, bool covered_only = false);

class Format;   // a parsed description (opaque)

class Registry {
public:
    Registry();
    ~Registry();
    Registry(Registry&&) noexcept;
    Registry& operator=(Registry&&) noexcept;

    // Parse a description and register it under its `format` name. ParseError on a mistake in it.
    // `origin` (a file name) only goes into the messages.
    const std::string& add_text(std::string_view text, std::string_view origin = "description");
    void add_file(const std::filesystem::path& file);
    // Every *.fmt of a folder; returns how many.
    std::size_t add_directory(const std::filesystem::path& dir);

    bool has(std::string_view format) const;
    std::vector<std::string> names() const;

    // Read `data` as `format`. FormatError / TruncatedError when it is not one (a magic differs, a field is cut short).
    Tree read(std::string_view format, ByteView data) const;
    std::optional<Tree> try_read(std::string_view format, ByteView data) const;
    // The formats whose description reads `data` without an error.
    std::vector<std::string> identify(ByteView data) const;

private:
    std::map<std::string, std::shared_ptr<const Format>, std::less<>> formats_;
};

}  // namespace cb::formats::engine

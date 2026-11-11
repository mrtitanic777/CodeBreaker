// cb/port/original_program.hpp -- the program as its old compiler saw it.
//
// The sources of the original program, unchanged, say what its structures were (16-bit pointers, far ones 4 bytes,
// a word on an even offset), how big its arrays were and which of its functions give out blocks of memory. The
// port's passes ask these things while they rewrite the program; each answer is worked out once and kept here.
#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "cb/port/c_source.hpp"

namespace cb::port::pre_ansi {

// {allocator: the places of its arguments its size was given at}
using Allocators = OrderedMap<std::string, std::set<std::size_t>>;
// {BSS name: (base type, declarator)}: what the program's externs say of each of its arrays
using ExternDecls = OrderedMap<std::string, std::pair<std::string, std::string>>;

class OriginalProgram {
public:
    // `originals`: file -> text as the program had it; `pointer_bytes` the old pointer's size; `layout` the engine
    // that lays out structures (lay_out_structs when empty).
    OriginalProgram(Files originals, unsigned pointer_bytes, LayoutFn layout = {});

    const Files& files() const { return originals_; }
    unsigned pointer_bytes() const { return pointer_bytes_; }

    // The structures as the old compiler laid them out (16-bit pointers, far ones 4 bytes).
    const StructLayouts& layouts();
    // The layouts as a file sees them: the definitions of the headers it includes (in their order) before any other
    // file's -- two headers that define one tag two ways: the one it includes.
    const StructLayouts& layouts_for(const std::string& file);
    // What `layouts_for` lays out: the files whose definitions come first (the file's include order; empty when no tag
    // is defined two ways, then the layouts are `layouts()`) and the texts, in the order the layout engine takes them.
    struct LayoutInput {
        std::vector<std::string> order;
        std::vector<std::string> texts;
    };
    LayoutInput layout_input(const std::string& file);
    // {global array: the bytes of one of its elements in the original} (its far pointers 4, near ones 2)
    const OrderedMap<std::string, std::uint64_t>& elem_sizes();
    // {global array: its bytes in the original} (far pointers 4 bytes, structures as laid out); `asm_sizes` (what the
    // assembler's data placed) overrides it.
    OrderedMap<std::string, std::uint64_t> var_sizes(const OrderedMap<std::string, std::uint64_t>& asm_sizes);
    // The program's functions that give blocks of memory: a call of one given a structure's whole old size somewhere
    // (`(struct inode *)f0a42b(2, 32)`) is the evidence that a number given it is a size. `ported` are the files as
    // the port has them now (what a function passes on to another is read there); the answer is worked out once.
    const Allocators& allocators(const Files& ported);
    // What the program's externs say of each of its arrays named in `names`.
    ExternDecls extern_decls(const std::set<std::string>& names) const;

private:
    Files originals_;
    unsigned pointer_bytes_;
    LayoutFn layout_;
    std::optional<StructLayouts> layouts_;
    std::optional<std::set<std::string>> twice_tags_;                    // tags two files define
    std::map<std::vector<std::string>, StructLayouts> layouts_by_include_order_;
    std::optional<OrderedMap<std::string, std::uint64_t>> elem_sizes_;
    std::optional<OrderedMap<std::string, std::uint64_t>> var_sizes_;
    std::optional<Allocators> allocators_;
};

}  // namespace cb::port::pre_ansi

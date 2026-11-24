// engine/pcode_ops.hpp -- an instruction set's table of user operations bound to one emulator.
//
// SLEIGH names the work of most vector instructions and gives it no p-code (a user operation: NEON_sqadd, paddsb, vectorAddSignedByteSaturate). An instruction set's table
// (engine/pcode_ops_<set>.cpp) lists each operation's name and the host function that does it (built from engine/pcode_vector.hpp); bind() resolves the names once, when the
// emulator is made, to the description's operation numbers, and the emulator then calls the handler by number: no string is looked at while a program runs.
//
//     static const pcode_ops::Table kTable{{"NEON_sqadd", sqadd}, {"NEON_uqadd", uqadd}};
//     const pcode_ops::Binding b = pcode_ops::bind(emu, kTable);      // b.unbound: the description's operations the table does not serve
//
// A handler is `bool (Emulator&, UserOpCall&, void* context)`: it reads call.in[i] (call.inSize[i] bytes, least significant first), writes call.out (call.outSize bytes,
// cleared first) and returns true; false counts the operation as unimplemented (Emulator::unimplementedOps()).
#pragma once

#include <cstddef>
#include <initializer_list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"

namespace cb::engine::pcode_ops {

using Emu = decompile::sleigh::Emulator;
using Call = Emu::UserOpCall;
using Handler = Emu::UserOpHandler;

struct Entry {
    std::string_view name;   // the user operation's name in the description (must outlive the table: a string literal)
    Handler handler;
};

// The names an instruction set serves. Built once (static); std::logic_error for a name listed twice or a null handler.
class Table {
public:
    Table(std::initializer_list<Entry> entries);
    const Handler* find(std::string_view name) const;
    std::size_t size() const { return entries_.size(); }
    const std::vector<Entry>& entries() const { return entries_; }

private:
    std::vector<Entry> entries_;
    std::unordered_map<std::string_view, std::size_t> index_;
};

struct Binding {
    std::size_t bound = 0;                  // description operations now served by the table
    std::vector<std::string> unbound;       // description operations the table does not name (what is still handed to the hooks)
    std::vector<std::string> unknown;       // table names the description has no operation of (a misspelling, or another description's)
};

// Bind every operation of `emu` that `table` names (the previous binding of those numbers is replaced); `context` goes to each handler.
Binding bind(Emu& emu, const Table& table, void* context = nullptr);

}  // namespace cb::engine::pcode_ops

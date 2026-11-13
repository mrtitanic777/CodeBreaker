// cb/source/ne_asm/reachability.hpp -- which bytes of a segment are reachable CODE.
//
// The foundation of the safe readable form of a segment. Instead of guessing code from data by "looks printable" (which takes printable code for
// text), this follows control flow: from every entry point, the fall-through and the direct branch and call targets. Whatever is never reached is
// not executed -- a conservative signal for DATA that holds in the worst case too: a function only a jump table reaches is called data, which is
// less readable but still byte-exact (it is written as db).
//
// Entry points are the carved function starts, the exported entries, and the relocation targets inside the segment (code addresses the binary
// itself refers to).
//
//     codebreaker ne-asm reachability MODULE [--seg N]
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/base/corpus.hpp"
#include "cb/source/ne_asm/datacode.hpp"
#include "cb/source/ne_asm/fixups.hpp"
#include "cb/source/ne_asm/segment.hpp"

namespace cb::source::ne_asm::reachability {

// A direct jump, conditional jump, loop or call to a code offset, as objdump writes it (`jne 0x1f2`).
struct Branch {
    std::string op;
    std::uint32_t target = 0;
};
// nullopt for any other instruction. White space in the text does not matter.
std::optional<Branch> direct_branch(std::string_view instruction);

// The offset a fix-up target names inside segment `index` ("seg3:0x0012" in segment 3 -> 0x12), else nullopt.
std::optional<std::uint32_t> own_segment_target(std::string_view label, std::size_t index);

// The offsets of the instructions reachable from the segment's entry points: fall-through (except after jmp, ret, iret) and direct targets.
std::set<std::uint32_t> reachable(const segment::Seg& seg);

// Every offset something ENTERS by a jump, call or entry, which needs a label.
std::set<std::uint32_t> branch_targets(const segment::Seg& seg);

// [(source, target)] of every direct branch or call inside the segment's code.
std::vector<std::pair<std::uint32_t, std::uint32_t>> branches(const segment::Seg& seg);

// The data regions (datacode) nothing enters from outside: nothing outside the region jumps or falls into it, and no function start, export or
// relocation target lands in it. A region's own jumps -- a data blob objdump littered with fake branches -- do not count, so it can still be
// written as data. Stricter than `no target inside` where it matters (real entries) and looser where it does not (self-references).
std::vector<datacode::Region> safe_data_regions(const segment::Seg& seg);

// `MODULE [--seg N] [--ords FILE]`: one line, the share of the segment's instructions never reached. Returns 0.
int main(const base::Corpus& corpus, std::span<const std::string> args, const fixups::Objdump& objdump = fixups::run_objdump);

}  // namespace cb::source::ne_asm::reachability

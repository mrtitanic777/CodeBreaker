// cb/invert/flow16.hpp -- the control flow of a 16-bit function for the compiler models: its basic blocks and their edges, the switches
// (by a jump table, by a chain of compares), and the structure tree the dominators give (if, if-else, while, do-while, endless loops,
// switches, break / continue, and gotos for flow that has no structure -- never a fabricated one).
//
//     const flow16::Cfg cfg = flow16::build_cfg(instructions, {.switch_targets = ...});
//     const flow16::Tree tree = flow16::structure(cfg);
//
// A call is not an edge: the block continues past it. A jump out of the function (a tail call) ends its block; so does `ret`.
#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include "cb/platforms/decoder.hpp"

namespace cb::invert::flow16 {

inline constexpr int kNone = -1;   // "no block" in the fields that name one

// How a block ends.
enum class End { Return, Jump, Branch, Fall, TailCall, Switch, Dead };

struct Case {
    long long value = 0;
    int target = kNone;
};

struct Block {
    int id = 0;
    std::size_t first = 0, last = 0;       // the instructions [first, last) of the function
    End end = End::Fall;
    std::vector<int> successors;           // a branch: the taken block, then the fall-through
    std::optional<std::size_t> branch;     // the instruction of the jcc the block was built ending in (kept when the block becomes a switch)
    // A switch is one of the two: `table` (the case blocks in table order, a jump through a table not yet matched with its bounds test) or
    // `cases` (value -> block) with its default.
    std::optional<std::vector<int>> table;
    std::optional<std::vector<Case>> cases;
    int default_target = kNone;
    std::string selector;                  // the register the switch selects on
    int selector_end = kNone;              // by a table: the instruction of the block where the selector's code ends

    std::size_t size() const { return last - first; }
};

struct Cfg {
    std::vector<Block> blocks;
};

struct Options {
    // The case-target addresses (table order) of the indirect `jmp` at an address, if the caller knows them.
    std::function<std::optional<std::vector<long long>>(long long address)> switch_targets;
    bool compare_switches = false;         // a chain of compares against constants is a switch
    bool uncapped = false;                 // do not cap the copying of shared tails
};

// The blocks of `instructions` (the whole function). Empty instructions are a UsageError.
Cfg build_cfg(std::span<const platforms::Insn> instructions, const Options& options = {});

// ---- the structure tree -----------------------------------------------------------------------------------------------------------------

struct Node;
using Seq = std::vector<Node>;

struct CaseValue {
    bool is_default = false;
    long long value = 0;
};

struct Node {
    enum class Kind { Block, Break, Continue, Goto, If, IfElse, While, DoWhile, Loop, Switch };
    Kind kind = Kind::Block;
    int block = kNone;                 // Block: it; Goto: the target; the others: the block that tests (the header, the switch)
    int join = kNone;                  // where the construct's arms meet (a do-while: its latch)
    bool negated = false;              // If / While: the taken side is the body; DoWhile: the taken side continues the loop
    Seq body;                          // If, IfElse (then), While, DoWhile, Loop
    Seq else_body;                     // IfElse
    std::vector<std::pair<std::vector<CaseValue>, Seq>> arms;   // Switch
};
using Tree = Seq;

Tree structure(const Cfg& cfg, int entry = 0, bool uncapped = false);

}  // namespace cb::invert::flow16

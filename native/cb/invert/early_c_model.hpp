// cb/invert/early_c_model.hpp -- the Microsoft C 2.03 / 3.00 model's own interface: the register-value tracker (`recover`, the linear block
// walker, and `recover_cfg`, the walker over the structured control-flow graph) and the text passes over the statements they write.
//
// The model reads Capstone's operand text and keeps each register as the C expression that computes it. This header is the model's side of
// cb/invert/early_c.hpp, which gives it typed options, hooks and results; callers use that one.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::invert::early_c_model {

// What the driver knows from outside the function, asked through callbacks. Each returns whether it had an answer, and puts it in the last
// argument.
struct Hooks {
    // The case-target offsets (in table order) of the jump-table dispatch at `address`.
    std::function<bool(long long address, std::vector<long long>& targets)> switch_targets;
    // The bytes a call pops on return, for a call of this kind ("near" / "far") and target text.
    std::function<bool(long long address, const std::string& kind, const std::string& target, long long& pops)> callee_pop_bytes;
    // The registers a call reads as arguments.
    std::function<bool(long long address, const std::string& kind, const std::string& target, std::vector<std::string>& registers)> callee_registers;
};

struct Settings {
    bool mark = false, far_pointers = false, dead_registers = false, unsigned_compares = false, fp_casts = false;
    bool eager_register_variables = false, frame_after_bp = false, reread_register_params = true;
    bool caller_cleans = false;
    bool compare_switches = false;
    bool uncapped_structures = false;
    int returns_value = -1;   // -1: not known, else 0 / 1
    Hooks hooks;
};

// What a recovery finds out besides its text (the Python's module dict LAST, carried from `recover` to `recover_cfg`).
struct Findings {
    bool has_saved = false, has_register_params = false, has_fp_types = false, has_fp_return = false, has_unsigned_longs = false;
    std::vector<std::string> saved;
    std::vector<std::pair<std::string, long long>> register_params;
    std::vector<std::pair<std::string, std::string>> fp_types;
    std::string fp_return;
    std::vector<std::string> unsigned_longs;
};

struct Recovery {
    long long frame_size = 0;
    std::vector<std::string> params, statements;
    std::string return_type = "int";
    std::vector<long long> locals;
};

Recovery recover(const std::vector<std::uint8_t>& code, long long base, const std::map<long long, std::string>& helper_operators,
                 const Settings& settings, Findings& findings);
Recovery recover_cfg(const std::vector<std::uint8_t>& code, long long base, const std::map<long long, std::string>& helper_operators,
                     const Settings& settings, Findings& findings);

// ---- the text passes ----------------------------------------------------------------------------------------------------------------------
using Lines = std::vector<std::string>;
Lines tidy(Lines lines);
Lines strip_tail_returns(Lines lines);
Lines dead_regs(Lines lines);
Lines fold_ladders(Lines lines, bool mark);
Lines fold_goto_or(Lines lines);
Lines fold_longs(const Lines& lines);
std::string fold_long_expr(const std::string& expression);
Lines fold_shortcircuit(Lines lines);
Lines dead_store_recover(const Lines& statements, const std::vector<bool>& register_constants);
std::vector<std::pair<long long, long long>> detect_copies(const Lines& statements);
std::string to_c(const std::string& name, long long frame_size, const std::vector<std::string>& params, const Lines& statements,
                 const std::string& return_type, const std::vector<long long>& locals, const std::vector<long long>& longs);

}  // namespace cb::invert::early_c_model

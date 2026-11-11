#include "cb/decompile/asm_blocks.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>
#include <utility>

#include <nlohmann/json.hpp>

#include "cb/core/text.hpp"
#include "cb/decompile/l2tree.hpp"

namespace cb::decompile::asm_blocks {

namespace {

using l2tree::is_kind;
using l2tree::member;
using l2tree::text;

constexpr std::array<std::string_view, 2> kFlagVariables = {"int_CF", "int_ZF"};
constexpr std::array<std::string_view, 4> kSegmentRegisters = {"ds", "es", "ss", "cs"};
constexpr std::array<std::string_view, 6> kFeedingRegisters = {"ax", "bx", "cx", "dx", "si", "di"};

bool one_of(std::string_view s, std::span<const std::string_view> names) { return std::find(names.begin(), names.end(), s) != names.end(); }

std::string_view symbol_name(const Json* node) { return text(member(node, "sym"), "name"); }

bool is_word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }

bool is_lower_hex(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); }

// ^int([0-9a-f]{2})(_[a-z_]+)?$ : the name of an interrupt helper (`int21`, `int21_xyz`); its vector.
std::optional<std::uint32_t> helper_vector(std::string_view name) {
    if (name.size() < 5 || !name.starts_with("int") || !is_lower_hex(name[3]) || !is_lower_hex(name[4])) return std::nullopt;
    const std::string_view rest = name.substr(5);
    if (!rest.empty()) {
        if (rest.size() < 2 || rest[0] != '_') return std::nullopt;
        for (char c : rest.substr(1)) {
            if (!((c >= 'a' && c <= 'z') || c == '_')) return std::nullopt;
        }
    }
    return text::parse_int<std::uint32_t>(name.substr(3, 2), 16);
}

// ^[a-z]*Stack_([0-9a-f]+)$ : a local the decompiler names by its place in the frame; the number.
std::optional<std::int64_t> stack_number(std::string_view name) {
    std::size_t i = 0;
    while (i < name.size() && name[i] >= 'a' && name[i] <= 'z') ++i;
    if (!name.substr(i).starts_with("Stack_")) return std::nullopt;
    const std::string_view digits = name.substr(i + 6);
    if (digits.empty() || !std::all_of(digits.begin(), digits.end(), is_lower_hex)) return std::nullopt;
    return text::parse_int<std::int64_t>(digits, 16);
}

// ^int_(?!CF|ZF)[A-Z]{2} : what an interrupt leaves in a register (int_BX, int_DS), as the decompiler names it.
bool is_interrupt_register(std::string_view name) {
    if (name.size() < 6 || !name.starts_with("int_")) return false;
    const auto upper = [](char c) { return c >= 'A' && c <= 'Z'; };
    if (!upper(name[4]) || !upper(name[5])) return false;
    const std::string_view two = name.substr(4, 2);
    return two != "CF" && two != "ZF";
}

std::string label_for(std::uint32_t address) { return std::format("L_{:04x}", address); }

// the first and last of a statement list's origin offsets together
std::vector<std::uint32_t> offsets_of(const Json& stmts) {
    std::vector<std::uint32_t> out;
    if (!stmts.is_array()) return out;
    for (const Json& s : stmts) {
        const auto at = l2tree::origin_offsets(s);
        out.insert(out.end(), at.begin(), at.end());
    }
    std::sort(out.begin(), out.end());
    return out;
}

Json machine_fragment(std::string_view why, std::uint32_t length, const std::vector<std::optional<std::string>>& lines,
                      const std::vector<std::uint32_t>& at) {
    Json frag = Json::object();
    frag["node"] = "MachineFragment";
    frag["reason"] = "inline";
    frag["length"] = length;
    frag["why"] = std::string(why);
    Json asm_lines = Json::array();
    for (const auto& line : lines) asm_lines.push_back(*line);
    frag["asm"] = std::move(asm_lines);
    Json origin = Json::object();
    origin["at"] = at;
    frag["origin"] = std::move(origin);
    return frag;
}

}  // namespace

Frame frame_names(const Json& doc) {
    Frame out;
    const Json* params = member(member(member(&doc, "sym"), "type"), "params");
    if (params && params->is_array()) {
        for (const Json& p : *params) {
            if (const auto at = l2tree::integer(member(&p, "at"), "stack")) out[*at + 2] = std::string(text(&p, "name"));   // BP is SP at entry less the BP pushed
        }
    }
    if (const Json* body = member(&doc, "body")) {
        l2tree::walk(*body, [&](const Json& n) {
            if (!is_kind(n, "Name")) return;
            const std::string_view name = symbol_name(&n);
            if (const auto k = stack_number(name)) out.try_emplace(2 - *k, std::string(name));
        });
    }
    return out;
}

std::string name_frame_places(std::string_view operand, const Frame& frame) {
    // [bp + k] / [bp - k], k as 0x.. or decimal
    std::string out;
    std::size_t i = 0;
    while (i < operand.size()) {
        const std::size_t open = operand.find("[bp ", i);
        if (open == std::string_view::npos) break;
        std::size_t p = open + 4;
        std::optional<std::int64_t> k;
        std::size_t end = 0;
        if (p + 2 < operand.size() && (operand[p] == '+' || operand[p] == '-') && operand[p + 1] == ' ') {
            const bool negative = operand[p] == '-';
            p += 2;
            std::size_t q = p;
            std::optional<std::int64_t> value;
            if (operand.substr(q, 2) == "0x" && q + 2 < operand.size() && is_lower_hex(operand[q + 2])) {
                q += 2;
                std::size_t d = q;
                while (d < operand.size() && is_lower_hex(operand[d])) ++d;
                value = text::parse_int<std::int64_t>(operand.substr(q, d - q), 16);
                q = d;
            } else {
                std::size_t d = q;
                while (d < operand.size() && operand[d] >= '0' && operand[d] <= '9') ++d;
                if (d > q) value = text::parse_int<std::int64_t>(operand.substr(q, d - q), 10);
                q = d;
            }
            if (value && q < operand.size() && operand[q] == ']') {
                k = negative ? -*value : *value;
                end = q + 1;
            }
        }
        out += operand.substr(i, open - i);
        const std::string* named = k ? frame.find(*k) : nullptr;
        if (named) {
            out += *named;
            i = end;
        } else {
            out += operand.substr(open, 4);   // not a place with a name: the text as it was
            i = open + 4;
        }
    }
    out += operand.substr(i);
    return out;
}

std::optional<std::uint32_t> interrupt_called(const Json& stmt) {
    std::optional<std::uint32_t> found;
    if (const Json* expr = member(&stmt, "expr")) {
        l2tree::walk(*expr, [&](const Json& n) {
            if (found || !is_kind(n, "Call")) return;
            found = helper_vector(text(member(member(&n, "target"), "sym"), "name"));
        });
    }
    return found;
}

bool condition_reads(const Json& stmt, std::span<const std::string_view> names) {
    bool reads = false;
    if (const Json* cond = member(&stmt, "cond")) {
        l2tree::walk(*cond, [&](const Json& n) {
            if (is_kind(n, "Name") && one_of(symbol_name(&n), names)) reads = true;
        });
    }
    return reads;
}

bool feeds_interrupt(const Instruction& i) {
    if (inline_asm::is_386(i)) return false;
    const std::string_view mn = i.mnemonic;
    if (mn == "push" || mn == "pop") return one_of(i.op_str, kSegmentRegisters) || one_of(i.op_str, kFeedingRegisters);
    if (inline_asm::is_jump(mn) || mn == "call" || mn == "lcall" || mn == "ret" || mn == "retf" || mn == "int") return false;
    for (const inline_asm::Operand& op : i.operands) {
        if (op.kind == inline_asm::OperandKind::Mem && (op.access & inline_asm::kAccessWrite)) return false;
    }
    return !i.operands.empty() && i.operands[0].kind == inline_asm::OperandKind::Reg;
}

std::optional<std::string> inline_line(const Instruction& i, const Frame& frame,
                                       const std::unordered_map<std::uint32_t, std::string>& labels, const FixupSites& fixups) {
    const std::string_view mn = i.base_mnemonic();
    if (inline_asm::is_jump(mn)) {
        if (i.operands.empty()) return std::nullopt;
        const auto to = static_cast<std::uint32_t>(i.operands[0].imm & 0xFFFF);
        auto label = labels.find(to);
        if (label == labels.end()) return std::nullopt;
        return std::format("{} {}", mn, label->second);
    }
    for (std::uint32_t a = i.address; a < i.address + i.size; ++a) {
        if (fixups.contains(a)) return std::nullopt;   // (a relocation inside it: the symbol is needed)
    }
    if (mn == "call" || mn == "lcall") return std::nullopt;
    const std::string ops = inline_asm::operands_text(i);
    std::string prefix;
    if ((i.prefix[0] == 0xF2 || i.prefix[0] == 0xF3) && inline_asm::is_string_op(mn)) prefix = i.prefix[0] == 0xF3 ? "rep " : "repne ";
    return name_frame_places(std::format("{}{}{}", prefix, mn, ops.empty() ? std::string() : " " + ops), frame);
}

std::vector<std::string> place(Json& doc, ByteView code, std::uint32_t start, std::uint32_t end, const FixupSites& fixups,
                               Window window, const Decoder& decoder) {
    // code[start:end] (a range past the segment is the part that exists), decoded as if it ran at `start`
    const std::size_t from = std::min<std::size_t>(start, code.size());
    const std::size_t to = std::clamp<std::size_t>(end, from, code.size());
    const std::vector<Instruction> ins = (decoder ? decoder : Decoder(inline_asm::decode16))(code.subspan(from, to - from), start);
    if (ins.empty()) return {"no instructions"};

    std::unordered_map<std::uint32_t, std::size_t> index;   // address -> position (lookup only)
    for (std::size_t k = 0; k < ins.size(); ++k) index.emplace(ins[k].address, k);
    const Frame frame = frame_names(doc);
    std::vector<std::string> said;
    std::unordered_set<std::uint32_t> targets;   // where a jump of the function lands
    for (const Instruction& i : ins) {
        if (inline_asm::is_jump(i.mnemonic) && !i.operands.empty() && i.operands[0].kind == inline_asm::OperandKind::Imm) {
            targets.insert(static_cast<std::uint32_t>(i.operands[0].imm & 0xFFFF));
        }
    }
    const auto size_of = [&](std::size_t a, std::size_t b) {   // the bytes of ins[a..b)
        std::uint32_t n = 0;
        for (std::size_t x = a; x < b; ++x) n += ins[x].size;
        return n;
    };

    l2tree::for_each_statement_list(doc, [&](Json& body_json) {
        auto& body = body_json.get_ref<Json::array_t&>();
        std::size_t k = 0;
        while (k < body.size()) {
            const std::optional<std::uint32_t> vec = is_kind(body[k], "ExprStmt") ? interrupt_called(body[k]) : std::nullopt;
            if (!vec) {
                ++k;
                continue;
            }
            const std::vector<std::uint32_t> at = l2tree::origin_offsets(body[k]);
            std::optional<std::size_t> int_at;   // the first INT among the statement's instructions
            for (std::uint32_t a : at) {
                auto found = index.find(a);
                if (found != index.end() && ins[found->second].mnemonic == "int") {
                    int_at = found->second;
                    break;
                }
            }
            if (!int_at) {
                said.push_back("an interrupt helper with no INT among its instructions");
                ++k;
                continue;
            }
            const std::size_t p = *int_at;
            std::size_t lo_at = p;
            while (lo_at > 0 && feeds_interrupt(ins[lo_at - 1]) && !targets.contains(ins[lo_at - 1].address)) --lo_at;
            std::size_t hi_at = p;
            while (hi_at + 1 < ins.size() && (ins[hi_at + 1].mnemonic == "pop" || ins[hi_at + 1].mnemonic == "push") &&
                   one_of(ins[hi_at + 1].op_str, kSegmentRegisters)) {
                ++hi_at;
            }
            const std::int64_t lo = std::max<std::int64_t>(0, static_cast<std::int64_t>(lo_at) + window.first);
            const std::int64_t hi = std::min<std::int64_t>(static_cast<std::int64_t>(ins.size()) - 1, static_cast<std::int64_t>(hi_at) + window.last);
            if (hi < lo) {
                said.push_back("the window leaves no instruction in the block");
                ++k;
                continue;
            }
            const auto first_ins = static_cast<std::size_t>(lo);
            const auto last_ins = static_cast<std::size_t>(hi);
            std::vector<std::uint32_t> span;   // the block's instruction addresses, ascending
            std::vector<std::optional<std::string>> lines;
            for (std::size_t x = first_ins; x <= last_ins; ++x) {
                span.push_back(ins[x].address);
                lines.push_back(inline_line(ins[x], frame, {}, fixups));
            }
            if (auto bad = std::find_if(lines.begin(), lines.end(), [](const auto& l) { return !l; }); bad != lines.end()) {
                said.push_back(std::format("an instruction of the block the inline assembler cannot say ({:04x})",
                                           ins[first_ins + static_cast<std::size_t>(bad - lines.begin())].address));
                ++k;
                continue;
            }
            // what the interrupt leaves in registers read by the C after it (int_BX, int_DS): not a block of this shape --
            // the C would read registers no variable holds
            std::string later;
            for (std::size_t x = k + 1; x < body.size() && later.empty(); ++x) {
                l2tree::walk(std::as_const(body[x]), [&](const Json& n) {
                    if (later.empty() && is_kind(n, "Name") && is_interrupt_register(symbol_name(&n))) later = std::string(symbol_name(&n));
                });
            }
            if (!later.empty()) {
                said.push_back(std::format("INT {:02X}h: the C after it reads {}", *vec, later));
                ++k;
                continue;
            }
            // the statements made only of the block's instructions go; the block stands where the first was
            std::size_t first = k;
            while (first > 0 && is_kind(body[first - 1], "ExprStmt")) {
                const auto prior = l2tree::origin_offsets(body[first - 1]);
                if (prior.empty() || !std::includes(span.begin(), span.end(), prior.begin(), prior.end())) break;
                --first;
            }
            Json frag = machine_fragment(std::format("INT {:02X}h", *vec), size_of(first_ins, last_ins + 1), lines, span);
            body.erase(body.begin() + static_cast<std::ptrdiff_t>(first), body.begin() + static_cast<std::ptrdiff_t>(k) + 1);
            body.insert(body.begin() + static_cast<std::ptrdiff_t>(first), std::move(frag));
            said.push_back(std::format("INT {:02X}h: an _asm block {:04x}-{:04x}", *vec, ins[first_ins].address, ins[last_ins].address));
            k = first + 1;

            // its carry: the first If after it reading int_CF / int_ZF
            for (std::size_t j = k; j < body.size(); ++j) {
                const Json& cand = body[j];
                if (!is_kind(cand, "If") || !condition_reads(cand, kFlagVariables)) continue;
                const std::vector<std::uint32_t> cat = l2tree::origin_offsets(cand);
                Json then = Json::array();
                if (const Json* t = member(&cand, "then")) {
                    if (t->is_array()) then = *t;
                    else if (l2tree::present(t)) then.push_back(*t);
                }
                const std::vector<std::uint32_t> then_at = offsets_of(then);
                if (cat.empty() || then_at.empty() || !index.contains(cat.front())) {
                    said.push_back("its carry: an If the instructions do not place");
                    break;
                }
                const std::size_t a = index.at(cat.front());
                auto then_first = index.find(then_at.front());
                if (then_first == index.end() || then_first->second <= a) {
                    said.push_back("its carry: a then-part before its test");
                    break;
                }
                const std::size_t b = then_first->second;
                std::optional<std::uint32_t> after;   // the instruction after the then-part's last
                if (auto then_last = index.find(then_at.back()); then_last != index.end() && then_last->second + 1 < ins.size()) {
                    after = ins[then_last->second + 1].address;
                }
                std::unordered_map<std::uint32_t, std::string> labels;
                labels[then_at.front()] = label_for(then_at.front());
                if (after) labels[*after] = label_for(*after);
                std::set<std::uint32_t> inner;   // the test's own instructions, ascending
                for (std::size_t x = a; x < b; ++x) inner.insert(ins[x].address);
                for (std::uint32_t t : targets) {
                    if (inner.contains(t)) labels[t] = label_for(t);
                }
                std::vector<std::optional<std::string>> test;
                for (std::size_t x = a; x < b; ++x) {
                    if (x != a && targets.contains(ins[x].address)) test.push_back(label_for(ins[x].address) + ":");
                    test.push_back(inline_line(ins[x], frame, labels, fixups));
                }
                if (std::any_of(test.begin(), test.end(), [](const auto& l) { return !l; })) {
                    said.push_back("its carry: an instruction the block cannot say");
                    break;
                }
                const std::string then_label = labels.at(then_at.front());
                Json carry = machine_fragment(std::format("the carry INT {:02X}h left", *vec), size_of(a, b), test,
                                              std::vector<std::uint32_t>(inner.begin(), inner.end()));
                Json then_part = Json::object();
                then_part["node"] = "Labeled";
                then_part["label"] = then_label;
                then_part["body"] = std::move(then);
                std::vector<Json> parts;
                parts.push_back(std::move(carry));
                parts.push_back(std::move(then_part));
                if (after) {
                    Json tail = Json::object();
                    tail["node"] = "Labeled";
                    tail["label"] = labels.at(*after);
                    tail["body"] = Json::array();
                    parts.push_back(std::move(tail));
                }
                body.erase(body.begin() + static_cast<std::ptrdiff_t>(j));
                body.insert(body.begin() + static_cast<std::ptrdiff_t>(j), std::make_move_iterator(parts.begin()),
                            std::make_move_iterator(parts.end()));
                said.push_back(std::format("its carry: an _asm block {:04x}-{:04x}, the then-part at {}", ins[a].address, ins[b - 1].address, then_label));
                break;
            }
        }
    });
    if (std::any_of(said.begin(), said.end(), [](const std::string& s) { return s.find("block") != std::string::npos; })) drop_unused_locals(doc);
    return said;
}

void drop_unused_locals(Json& doc) {
    Json* body = member(&doc, "body");
    if (!body || !l2tree::present(body)) return;
    std::unordered_set<std::string> used;
    bool nameless_used = false;   // a symbol with no name is "used" by the nameless locals too
    std::string asm_text;
    l2tree::walk(std::as_const(*body), [&](const Json& n) {
        const Json* sym = member(&n, "sym");
        if (!is_kind(n, "Local") && sym && sym->is_object()) {
            const Json* name = member(sym, "name");
            if (name && name->is_string()) used.emplace(name->get_ref<const std::string&>());
            else nameless_used = true;
        }
        if (is_kind(n, "MachineFragment")) {
            if (const Json* lines = member(&n, "asm"); lines && lines->is_array()) {
                for (const Json& line : *lines) {
                    if (line.is_string()) asm_text += line.get_ref<const std::string&>() + " ";
                }
            }
        }
    });
    // the identifiers of the asm text
    for (std::size_t i = 0; i < asm_text.size();) {
        const char c = asm_text[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')) {
            ++i;
            continue;
        }
        std::size_t e = i + 1;
        while (e < asm_text.size() && is_word_char(asm_text[e])) ++e;
        used.emplace(asm_text.substr(i, e - i));
        i = e;
    }
    // (the decompiler's `_x` is x joined with the word above it, a far pointer's halves: the back end declares it in x's
    // place, so x is used where `_x` is -- SUWIN 8:1704's _pcStack_10c)
    const std::vector<std::string> joined(used.begin(), used.end());
    for (const std::string& n : joined) {
        if (n.starts_with("_")) used.emplace(n.substr(1));
    }
    const auto unused_local = [&](const Json& s) {
        if (!is_kind(s, "Local")) return false;
        const Json* name = member(member(&s, "sym"), "name");
        return name && name->is_string() ? !used.contains(name->get_ref<const std::string&>()) : !nameless_used;
    };
    const auto filter = [&](Json& list) {
        auto& items = list.get_ref<Json::array_t&>();
        items.erase(std::remove_if(items.begin(), items.end(), unused_local), items.end());
    };
    if (body->is_array()) filter(*body);
    l2tree::for_each_statement_list(*body, filter);
}

}  // namespace cb::decompile::asm_blocks

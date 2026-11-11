// cb/analyze/ppc_calls.cpp -- see ppc_calls.hpp.
#include "cb/analyze/ppc_calls.hpp"

#include <algorithm>
#include <format>
#include <unordered_map>

#include "cb/core/text.hpp"

namespace cb::analyze::ppc_calls {

namespace {

// instructions whose first operand is a register they set (for the call-site count)
bool sets_first_operand(std::string_view m) {
    static const std::set<std::string, std::less<>> kSets = {
        "li", "lis", "mr", "addi", "addis", "subi", "ori", "oris", "lwz", "lhz", "lbz", "ld", "add", "subf", "or", "and",
        "rlwinm", "mulli", "extsb", "extsh", "neg", "lwzx", "divwu", "mullw", "srawi", "xor", "slw", "srw", "addic",
        "rlwimi", "clrlwi", "slwi", "srwi", "rotlwi", "lha", "lwa", "extsw", "nor", "not"};
    return kSets.contains(m);
}

// Instructions that write no register (by prefix, or by name).
bool writes_nothing(std::string_view m) {
    static constexpr std::string_view kPrefixes[] = {"cmp", "b", "st", "dcb", "tw", "td", "tlb", "mt"};
    static const std::set<std::string, std::less<>> kNames = {"nop", "sync", "isync", "eieio", "lwsync", "twi", "tdi", "ptesync", "tlbsync", "dcbz128"};
    for (std::string_view p : kPrefixes) {
        if (m.starts_with(p)) return true;
    }
    return kNames.contains(m);
}

// Stores and loads that also change their base register.
bool updates_base_by_store(std::string_view m) {
    static const std::set<std::string, std::less<>> k = {"stwu", "sthu", "stbu", "stdu", "stwux", "sthux", "stbux", "stdux",
                                                          "stfsu", "stfdu", "stfsux", "stfdux", "lfsu", "lfdu", "lfsux", "lfdux"};
    return k.contains(m);
}
bool updates_base_by_load(std::string_view m) {
    static const std::set<std::string, std::less<>> k = {"lwzu", "lhzu", "lbzu", "ldu", "lhau", "lwau", "lwzux", "lhzux", "lbzux", "ldux", "lhaux"};
    return k.contains(m);
}

// A call through a register: it may change anything.
bool is_indirect_branch(std::string_view m) {
    return m == "bctrl" || m == "bctr" || m == "bcctr" || m == "bcctrl" || m == "bclrl";
}

// The printf family and its kin: the number of arguments varies from call to call.
bool is_variadic_import(std::string_view name) {
    for (std::string_view w : {"printf", "scanf", "DbgPrint", "ProducerLog", "vscw", "vsnw", "_vsn", "_vsc"}) {
        if (name.find(w) != std::string_view::npos) return true;
    }
    return false;
}

bool is_call(std::string_view b) { return b == "bl" || b == "bla"; }

// The mnemonic without its record-form dots.
std::string_view base_mnemonic(std::string_view m) {
    while (!m.empty() && m.back() == '.') m.remove_suffix(1);
    return m;
}

bool all_digits(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; });
}

}  // namespace

std::vector<std::string> split_operands(std::string_view text) {
    std::vector<std::string> out;
    if (cb::text::trim(text).empty()) return out;
    for (std::string_view piece : cb::text::split(text, ',')) out.emplace_back(cb::text::trim(piece));
    return out;
}

std::optional<int> register_number(std::string_view token, bool fp) {
    if (token.empty() || token[0] != (fp ? 'f' : 'r')) return std::nullopt;
    std::size_t i = 1;
    if (fp && i < token.size() && token[i] == 'r') ++i;
    const std::string_view digits = token.substr(i);
    if (!all_digits(digits) || digits.size() > 6) return std::nullopt;
    return cb::text::parse_int<int>(digits);
}

std::optional<MemoryOperand> memory_operand(std::string_view token) {
    const std::size_t open = token.find('(');
    if (open == std::string_view::npos || !token.ends_with(')')) return std::nullopt;
    std::string_view disp = token.substr(0, open);
    bool negative = false;
    if (disp.starts_with('-')) {
        negative = true;
        disp.remove_prefix(1);
    }
    if (disp.starts_with("0x")) disp.remove_prefix(2);
    const std::optional<std::int64_t> value = cb::text::parse_int<std::int64_t>(disp, 16);
    if (!value) return std::nullopt;
    const std::optional<int> base = register_number(token.substr(open + 1, token.size() - open - 2));
    if (!base) return std::nullopt;
    return MemoryOperand{negative ? -*value : *value, *base};
}

std::optional<std::uint64_t> branch_target(const std::vector<std::string>& operands) {
    if (operands.empty() || cb::text::lower(operands[0].substr(0, 2)) != "0x") return std::nullopt;
    return cb::text::parse_int<std::uint64_t>(operands[0], 16);
}

std::set<int> gpr_writes(std::string_view mnemonic, const std::vector<std::string>& operands) {
    const std::string_view b = base_mnemonic(mnemonic);
    std::set<int> out;
    if (operands.empty()) return out;
    if (b == "lmw") {
        if (const auto r = register_number(operands[0])) {
            for (int k = *r; k < 32; ++k) out.insert(k);
        }
        return out;
    }
    std::optional<int> base;
    if (operands.size() >= 2) {
        const auto m = memory_operand(operands[1]);
        base = m ? std::optional<int>(m->base) : register_number(operands[1]);
    }
    if (updates_base_by_store(b)) {
        if (base) out.insert(*base);
        return out;
    }
    if (updates_base_by_load(b)) {
        if (const auto r = register_number(operands[0])) out.insert(*r);
        if (base) out.insert(*base);
        return out;
    }
    if (writes_nothing(b)) return out;
    if (const auto r = register_number(operands[0])) out.insert(*r);
    return out;
}

std::set<int> fpr_writes(std::string_view mnemonic, const std::vector<std::string>& operands) {
    const std::string_view b = base_mnemonic(mnemonic);
    std::set<int> out;
    if (operands.empty() || b.starts_with("stf") || b.starts_with("fcmp") || b.starts_with("mtfsf") || b.starts_with('b')) return out;
    if (const auto r = register_number(operands[0], true)) out.insert(*r);
    return out;
}

int site_arity(const Body& body, std::size_t i) {
    bool set[8] = {};   // r3 .. r10
    for (std::size_t back = 1; back < static_cast<std::size_t>(kLookback) && back <= i; ++back) {
        const Instruction& ins = body[i - back];
        if (is_call(ins.mnemonic)) break;
        if (sets_first_operand(base_mnemonic(ins.mnemonic)) && !ins.operands.empty()) {
            const auto r = register_number(ins.operands[0]);
            if (r && *r >= 3 && *r <= 10) set[*r - 3] = true;
        }
    }
    int k = 0;
    while (k < 8 && set[k]) ++k;
    return k;
}

std::optional<int> consensus(std::span<const int> arities, bool stub) {
    if (arities.size() < 3) return std::nullopt;
    std::vector<std::pair<int, std::size_t>> counts;   // in the order first seen: a tie goes to the first
    for (int a : arities) {
        auto it = std::find_if(counts.begin(), counts.end(), [a](const auto& c) { return c.first == a; });
        if (it == counts.end()) counts.emplace_back(a, 1);
        else ++it->second;
    }
    const auto top = std::max_element(counts.begin(), counts.end(), [](const auto& x, const auto& y) { return x.second < y.second; });
    const double share = static_cast<double>(top->second) / static_cast<double>(arities.size());
    if (stub) return share >= 0.85 ? std::optional<int>(top->first) : std::nullopt;
    if (top->first == 8) return share >= 0.85 ? std::optional<int>(8) : std::nullopt;
    return share >= 0.6 && top->first >= 1 && top->first <= 7 ? std::optional<int>(top->first) : std::nullopt;
}

Result analyse(const Module& module) {
    Result r;
    r.functions = module.functions.size();
    std::unordered_map<std::uint64_t, std::vector<int>> sites;     // call target -> the arity at each call site
    std::map<std::uint64_t, std::uint32_t> local, fplocal;          // registers a function writes itself
    std::map<std::uint64_t, std::set<std::uint64_t>> callees;
    std::set<std::uint64_t> everything;                              // functions that may change every volatile register
    for (const auto& [a, body] : module.functions) {
        // what the function stored on its own frame: reloading it again is not a change
        std::set<std::pair<int, std::int64_t>> saved;
        for (const Instruction& ins : body) {
            if (ins.mnemonic == "stw" && ins.operands.size() >= 2) {
                const auto m = memory_operand(ins.operands[1]);
                const auto reg = register_number(ins.operands[0]);
                if (m && m->base == 1 && reg) saved.emplace(*reg, m->displacement);
            }
        }
        // storing r8-r10 to the caller's frame before the function's own stwu is homing the va_list area
        std::set<int> homed;
        for (std::size_t k = 0; k < std::min<std::size_t>(16, body.size()); ++k) {
            const Instruction& ins = body[k];
            if (ins.mnemonic == "stwu") break;
            if ((ins.mnemonic == "stw" || ins.mnemonic == "std") && ins.operands.size() >= 2 && ins.operands[1].ends_with("(r1)")) {
                if (const auto reg = register_number(ins.operands[0])) homed.insert(*reg);
            }
        }
        if (homed.contains(8) && homed.contains(9) && homed.contains(10)) {
            int lowest = 10;
            for (int reg : homed) {
                if (reg >= 3 && reg <= 10) lowest = std::min(lowest, reg);
            }
            const int fixed = lowest - 3;
            if (fixed >= 0 && fixed <= 7) r.variadic[a] = fixed;
        }
        for (std::size_t i = 0; i < body.size(); ++i) {
            const Instruction& ins = body[i];
            const std::string_view b = base_mnemonic(ins.mnemonic);
            if (is_call(b)) {
                const auto t = branch_target(ins.operands);
                if (t) sites[*t].push_back(site_arity(body, i));
                if (t && module.transparent.contains(*t)) continue;
                if (t && module.functions.contains(*t)) callees[a].insert(*t);
                else everything.insert(a);
                continue;
            }
            if (is_indirect_branch(b)) {
                everything.insert(a);
                continue;
            }
            if ((b == "lwz" || b == "lhz" || b == "lbz" || b == "ld") && ins.operands.size() >= 2) {
                const auto m = memory_operand(ins.operands[1]);
                const auto reg = register_number(ins.operands[0]);
                if (m && m->base == 1 && reg && saved.contains({*reg, m->displacement})) continue;   // the value it saved
            }
            for (int w : gpr_writes(ins.mnemonic, ins.operands)) {
                if (kVolatileMask >> w & 1u) local[a] |= 1u << w;
            }
            for (int w : fpr_writes(ins.mnemonic, ins.operands)) {
                if (w < 14) fplocal[a] |= 1u << w;
            }
        }
    }
    for (const auto& [t, arities] : sites) {
        if (const auto name = module.import_names.find(t); name != module.import_names.end()) {
            if (!is_variadic_import(name->second)) {
                if (const auto n = consensus(arities, true)) r.imports[name->second] = *n;
            }
        } else if (module.functions.contains(t)) {
            if (const auto n = consensus(arities, false)) r.arity[t] = *n;
        }
    }
    // what a function may change: its own writes and everything its callees may, to a fixed point
    for (int pass = 0; pass < 2; ++pass) {
        const bool fp = pass == 1;
        const std::uint32_t full = fp ? kFpVolatileMask : kVolatileMask;
        const auto& own = fp ? fplocal : local;
        std::map<std::uint64_t, std::uint32_t> mask;
        for (const auto& [a, body] : module.functions) {
            const auto it = own.find(a);
            mask[a] = everything.contains(a) ? full : (it == own.end() ? 0 : it->second);
        }
        for (bool changed = true; changed;) {
            changed = false;
            for (auto& [a, m] : mask) {
                if (m == full) continue;
                std::uint32_t grown = m;
                if (const auto c = callees.find(a); c != callees.end()) {
                    for (std::uint64_t g : c->second) grown |= mask.at(g);
                }
                if (grown != m) {
                    m = grown;
                    changed = true;
                }
            }
        }
        auto& out = fp ? r.fpclobber : r.clobber;
        for (const auto& [a, m] : mask) {
            if (m != full) out[a] = m;
        }
    }
    return r;
}

std::string summary(const Result& r) {
    return std::format("{} functions: arity for {}, import arity for {}, {} variadic, {} keep a volatile register, {} keep a volatile FP register",
                       r.functions, r.arity.size(), r.imports.size(), r.variadic.size(), r.clobber.size(), r.fpclobber.size());
}

std::string to_tsv(const Result& r) {
    std::string out = "kind\taddress or name\tvalue\n";
    for (const auto& [a, n] : r.arity) out += std::format("arity\t{:08X}\t{}\n", a, n);
    for (const auto& [name, n] : r.imports) out += std::format("import arity\t{}\t{}\n", name, n);
    for (const auto& [a, n] : r.variadic) out += std::format("variadic\t{:08X}\t{}\n", a, n);
    for (const auto& [a, m] : r.clobber) {
        std::vector<std::string> regs;
        for (int k : kVolatileRegisters) {
            if (m >> k & 1u) regs.push_back(std::format("r{}", k));
        }
        out += std::format("clobber\t{:08X}\t{}\n", a, regs.empty() ? std::string("-") : cb::text::join(regs, " "));
    }
    for (const auto& [a, m] : r.fpclobber) {
        std::vector<std::string> regs;
        for (int k = 0; k < 14; ++k) {
            if (m >> k & 1u) regs.push_back(std::format("f{}", k));
        }
        out += std::format("fpclobber\t{:08X}\t{}\n", a, regs.empty() ? std::string("-") : cb::text::join(regs, " "));
    }
    return out;
}

}  // namespace cb::analyze::ppc_calls

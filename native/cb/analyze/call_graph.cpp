#include "cb/analyze/call_graph.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <unordered_map>
#include <unordered_set>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::analyze::call_graph {

std::string_view name_of(CallKind kind) {
    switch (kind) {
        case CallKind::Call: return "call";
        case CallKind::Import: return "import";
        case CallKind::Tail: return "tail";
        case CallKind::Indirect: return "indirect";
    }
    return "?";
}

std::string_view name_of(Access how) {
    switch (how) {
        case Access::Read: return "r";
        case Access::Write: return "w";
        case Access::ReadWrite: return "rw";
        case Access::AddressTaken: return "a";
    }
    return "?";
}

namespace {

// Instructions whose memory operand is only read, and those that only write it (the rest read and write).
constexpr std::array<std::string_view, 22> kReads = {"cmp",  "test", "push",  "bt",   "call", "jmp",   "lcall", "ljmp",
                                                    "fld",  "fild", "fcomp", "fcom", "fadd", "fsub",  "fmul",  "fdiv",
                                                    "ficom", "ficomp", "bound", "div", "idiv", "mul"};
constexpr std::array<std::string_view, 15> kPureWrites = {"mov",   "pop",   "fstp",  "fst",   "fistp",
                                                         "fist",  "lea",   "movzx", "movsx", "sidt",
                                                         "sgdt",  "fstcw", "fnstcw", "fnstsw", "fstsw"};

template <std::size_t N>
bool in(std::string_view s, const std::array<std::string_view, N>& words) {
    return std::find(words.begin(), words.end(), s) != words.end();
}

struct Found {
    Va address;
    Access how;
};

// Every operand of one instruction that refers to a place in the module.
void references(const Image& img, const Instruction& ins, std::vector<Found>& out) {
    const bool has_known = img.has_pointers();
    std::set<Va> known;   // the targets the file says are written inside this instruction
    if (has_known) {
        const Va last = ins.addr + ins.size - (img.bits() == 32 ? 3 : 1);
        for (Va a = ins.addr + 1; a < last; ++a) {
            const Pointer* p = img.pointer_at(a);
            if (p != nullptr && p->target) known.insert(*p->target);
        }
    }
    if (has_known && known.empty() && img.bits() == 32) return;
    for (std::size_t k = 0; k < ins.operands.size(); ++k) {
        const Operand& op = ins.operands[k];
        if (op.kind == Operand::Kind::Immediate) {
            if (ins.flow != Flow::None || ins.target) continue;
            if (img.bits() == 32) {
                if (has_known ? known.contains(op.value) : (op.size == 4 && img.is_data(op.value)))
                    out.push_back({op.value, Access::AddressTaken});
            }
        } else if (op.kind == Operand::Kind::Memory && op.base.empty()) {
            Va va = 0;
            if (img.bits() == 32) {
                va = op.value;
                if (!(has_known ? known.contains(va) : img.span(va) != nullptr)) continue;
            } else {
                if (!(op.seg.empty() || op.seg == "ds") || img.data_segment() == 0 || !op.index.empty()) continue;
                va = img.data_segment() + (op.value & 0xFFFF);
                if (img.span(va) == nullptr) continue;
            }
            if (ins.mnemonic == "lea") out.push_back({va, Access::AddressTaken});
            else if (k != 0 || in(ins.mnemonic, kReads)) out.push_back({va, Access::Read});
            else
                out.push_back({va, (in(ins.mnemonic, kPureWrites) || ins.mnemonic.starts_with("set")) ? Access::Write
                                                                                                         : Access::ReadWrite});
        } else if (op.kind == Operand::Kind::Memory && img.bits() == 32) {   // [reg + ADDRESS]: a table indexed from a register
            const Va va = op.value & 0xFFFFFFFF;
            if (has_known ? known.contains(va) : (va >= 0x10000 && img.span(va) != nullptr))
                out.push_back({va, ins.mnemonic == "lea" ? Access::AddressTaken
                                   : (k != 0 || in(ins.mnemonic, kReads)) ? Access::Read
                                                                          : Access::ReadWrite});
        }
    }
    if (img.bits() == 16 && has_known && !known.empty()) {   // a far address or a segment written into the code
        for (Va va : known)
            if (ins.flow == Flow::None || !ins.target) out.push_back({va, Access::AddressTaken});
    }
}

// Where a call or jump goes: its direct target, or the import it goes through a slot to.
std::optional<Va> destination(const Image& img, const Instruction& ins) {
    if (ins.target) return ins.target;
    if (!ins.operands.empty() && ins.operands[0].kind == Operand::Kind::Memory && ins.operands[0].base.empty() &&
        ins.operands[0].index.empty() && img.is_import(ins.operands[0].value))
        return ins.operands[0].value;
    return std::nullopt;
}

// Tarjan's algorithm without recursion over `edges`: the components, callees first.
std::vector<std::vector<Va>> strongly_connected(const std::vector<Va>& nodes,
                                                const std::unordered_map<Va, std::vector<Va>>& edges) {
    std::unordered_map<Va, std::int64_t> index, low;
    std::unordered_set<Va> on_stack;
    std::vector<Va> stack;
    std::vector<std::vector<Va>> out;
    std::int64_t counter = 0;
    static const std::vector<Va> none;
    auto successors = [&](Va v) -> const std::vector<Va>& {
        const auto it = edges.find(v);
        return it == edges.end() ? none : it->second;
    };
    for (Va root : nodes) {
        if (index.contains(root)) continue;
        std::vector<std::pair<Va, std::size_t>> work{{root, 0}};
        index[root] = low[root] = counter++;
        stack.push_back(root);
        on_stack.insert(root);
        while (!work.empty()) {
            const Va v = work.back().first;
            const std::vector<Va>& ws = successors(v);
            bool descended = false;
            while (work.back().second < ws.size()) {
                const Va w = ws[work.back().second++];
                if (!index.contains(w)) {
                    index[w] = low[w] = counter++;
                    stack.push_back(w);
                    on_stack.insert(w);
                    work.emplace_back(w, 0);
                    descended = true;
                    break;
                }
                if (on_stack.contains(w)) low[v] = std::min(low[v], index[w]);
            }
            if (descended) continue;
            work.pop_back();
            if (!work.empty()) {
                const Va u = work.back().first;
                low[u] = std::min(low[u], low[v]);
            }
            if (low[v] == index[v]) {
                std::vector<Va> component;
                while (true) {
                    const Va w = stack.back();
                    stack.pop_back();
                    on_stack.erase(w);
                    component.push_back(w);
                    if (w == v) break;
                }
                out.push_back(std::move(component));
            }
        }
    }
    return out;
}

// `s` padded with spaces to `width` characters (counted as code points, so a name that is not valid UTF-8 -- a label is
// whatever the file called the thing -- pads without error, which std::format's width would not).
std::string pad_right(std::string_view s, std::size_t width) {
    std::size_t chars = 0;
    for (unsigned char c : s)
        if ((c & 0xC0) != 0x80) ++chars;
    std::string out(s);
    if (chars < width) out.append(width - chars, ' ');
    return out;
}

void add_unique(std::vector<Va>& sorted_values, Va v) {
    const auto it = std::lower_bound(sorted_values.begin(), sorted_values.end(), v);
    if (it == sorted_values.end() || *it != v) sorted_values.insert(it, v);
}

}  // namespace

Graph build(Image& img) {
    Graph g;
    const std::vector<Va> functions = img.functions();   // a copy: decoding never changes the set, but be safe
    const std::unordered_set<Va> is_function(functions.begin(), functions.end());
    std::unordered_map<Va, std::vector<Va>> direct;       // callees in the order the code calls them, once each
    std::unordered_map<Va, std::set<Va>> direct_seen;
    std::map<Va, std::vector<Va>> import_users;           // by the address standing for the import
    std::vector<Found> found;

    for (Va start : functions) {
        const Function& fn = img.function(start);
        std::vector<Call>& calls = g.calls[fn.start];
        std::vector<Va>& edges = direct[fn.start];
        for (const Instruction& ins : fn.insns) {
            if (ins.flow == Flow::Call || ins.flow == Flow::Jump) {
                const std::optional<Va> dest = destination(img, ins);
                if (dest && img.is_import(*dest)) {
                    calls.push_back({ins.addr, ins.flow == Flow::Call ? CallKind::Import : CallKind::Tail, dest});
                    add_unique(import_users[*dest], fn.start);
                } else if (!dest) {
                    const bool table_site = std::any_of(fn.tables.begin(), fn.tables.end(),
                                                        [&](const JumpTable& t) { return t.site == ins.addr; });
                    // a jump through a recognised table is not an unknown destination
                    if (ins.flow == Flow::Call || fn.tables.empty() || !table_site)
                        calls.push_back({ins.addr, CallKind::Indirect, std::nullopt});
                } else if (is_function.contains(*dest) &&
                           (ins.flow == Flow::Call || (*dest != fn.start && !(fn.start < *dest && *dest < fn.limit)))) {
                    calls.push_back({ins.addr, ins.flow == Flow::Call ? CallKind::Call : CallKind::Tail, dest});
                    if (direct_seen[fn.start].insert(*dest).second) edges.push_back(*dest);
                    add_unique(g.callers[*dest], fn.start);
                }
            }
            found.clear();
            references(img, ins, found);
            for (const Found& r : found) {
                if (img.is_import(r.address)) continue;
                g.refs.push_back({fn.start, ins.addr, r.address, r.how});
                if (is_function.contains(r.address) && r.how == Access::AddressTaken)
                    g.taken[r.address].push_back({true, fn.start, ins.addr});
            }
        }
    }

    // function addresses held in data
    std::set<Va> data_taken;
    if (img.has_pointers()) {
        for (const auto& [site, ptr] : img.pointer_list()) {
            if (ptr.target && is_function.contains(*ptr.target) && (!img.is_code(site) || !img.owner(site))) {
                g.taken[*ptr.target].push_back({false, std::nullopt, site});
                data_taken.insert(*ptr.target);
            }
        }
    } else {
        for (const Span& s : img.spans()) {
            if (s.executable) continue;
            // (the file holds `initialised` bytes of the span; the rest is reserved memory)
            const std::int64_t held = std::min<std::int64_t>(s.initialised, static_cast<std::int64_t>(s.data.size()));
            for (std::int64_t off = 0; off + 4 <= held; off += 4) {
                const std::uint8_t* b = s.data.data() + off;
                const Va v = static_cast<Va>(b[0] | (b[1] << 8) | (b[2] << 16) | (static_cast<std::uint32_t>(b[3]) << 24));
                if (is_function.contains(v)) {
                    g.taken[v].push_back({false, std::nullopt, s.va + off});
                    data_taken.insert(v);
                }
            }
        }
    }

    // a name can stand at several addresses: merge the users by the import's name
    for (const auto& [address, users] : import_users) {
        const std::string name = img.import_name(address).empty() ? img.label(address) : img.import_name(address);
        std::vector<Va>& merged = g.users[name];
        for (Va f : users) add_unique(merged, f);
    }

    // callees before callers; a cycle in one piece
    const std::vector<std::vector<Va>> components = strongly_connected(functions, direct);
    for (const std::vector<Va>& c : components) {
        for (Va f : c) g.order.push_back(f);
        const std::vector<Va>& edges = direct[c[0]];
        const bool self_loop = std::find(edges.begin(), edges.end(), c[0]) != edges.end();
        if (c.size() > 1 || self_loop) {
            std::vector<Va> sorted = c;
            std::sort(sorted.begin(), sorted.end());
            g.cycles.push_back(std::move(sorted));
        }
    }

    // what the exports reach: calls, tail jumps, and addresses taken by reached code
    std::unordered_map<Va, std::vector<Va>> takes;
    for (const Reference& r : g.refs)
        if (r.how == Access::AddressTaken && is_function.contains(r.address)) takes[r.function].push_back(r.address);
    auto reach = [&](const std::vector<Va>& roots) {
        std::set<Va> seen;
        std::vector<Va> work;
        for (Va r : roots)
            if (is_function.contains(r) && seen.insert(r).second) work.push_back(r);
        while (!work.empty()) {
            const Va f = work.back();
            work.pop_back();
            if (const auto d = direct.find(f); d != direct.end())
                for (Va t : d->second)
                    if (seen.insert(t).second) work.push_back(t);
            if (const auto k = takes.find(f); k != takes.end())
                for (Va t : k->second)
                    if (seen.insert(t).second) work.push_back(t);
        }
        return seen;
    };
    g.reachable = reach(img.roots());
    std::vector<Va> with_tables = img.roots();
    for (const auto& [f, sites] : g.taken)
        if (data_taken.contains(f)) with_tables.push_back(f);
    g.reachable_with_tables = reach(with_tables);

    const std::set<Va> roots(img.roots().begin(), img.roots().end());
    for (Va f : functions)
        if (!g.callers.contains(f) && !g.taken.contains(f) && !roots.contains(f)) g.unreferenced.push_back(f);
    return g;
}

// ---- the report ------------------------------------------------------------------------------------------------

Report make_report(const ReportInput& in, const Graph& g) {
    Report report;
    std::int64_t kinds[4] = {0, 0, 0, 0};
    std::int64_t edges = 0;
    for (Va f : in.functions) {
        std::set<Va> callees;
        std::set<std::string> imports;
        std::int64_t indirect = 0;
        const auto own_calls = g.calls.find(f);
        CB_ASSERT(own_calls != g.calls.end(), "the graph has no calls entry for a function of the module");
        for (const Call& c : own_calls->second) {
            ++kinds[static_cast<int>(c.kind)];
            if (!c.destination) ++indirect;
            else if (const std::optional<std::string> name = in.import_name(*c.destination)) imports.insert(*name);
            else callees.insert(*c.destination);
        }
        edges += static_cast<std::int64_t>(callees.size());
        CallsFact fact;
        fact.addr = f;
        for (Va c : callees) fact.callees.push_back(in.label(c));
        fact.imports.assign(imports.begin(), imports.end());
        fact.indirect = indirect;
        if (const auto cs = g.callers.find(f); cs != g.callers.end())
            for (Va c : cs->second) fact.callers.push_back(in.label(c));
        if (const auto t = g.taken.find(f); t != g.taken.end()) fact.address_taken = static_cast<std::int64_t>(t->second.size());
        fact.reached = g.reachable.contains(f);
        report.facts.push_back(std::move(fact));
    }
    std::int64_t leaves = 0;
    for (Va f : in.functions)
        if (g.calls.at(f).empty()) ++leaves;   // (every function has an entry: asserted above)
    Counts& c = report.counts;
    c.functions = static_cast<std::int64_t>(in.functions.size());
    c.functions_found_from_calls = static_cast<std::int64_t>(in.discovered.size());
    c.edges = edges;
    c.call_sites = kinds[static_cast<int>(CallKind::Call)];
    c.import_call_sites = kinds[static_cast<int>(CallKind::Import)];
    c.tail_jumps = kinds[static_cast<int>(CallKind::Tail)];
    c.indirect_call_sites = kinds[static_cast<int>(CallKind::Indirect)];
    c.imports_used = static_cast<std::int64_t>(g.users.size());
    c.leaf_functions = leaves;
    c.recursive_groups = static_cast<std::int64_t>(g.cycles.size());
    c.address_taken_functions = static_cast<std::int64_t>(g.taken.size());
    c.data_references = static_cast<std::int64_t>(g.refs.size());
    c.roots = static_cast<std::int64_t>(in.roots);
    c.reached_from_roots = static_cast<std::int64_t>(g.reachable.size());
    c.reached_with_data_tables = static_cast<std::int64_t>(g.reachable_with_tables.size());
    c.unreferenced_functions = static_cast<std::int64_t>(g.unreferenced.size());
    c.code_bytes_decoded = in.decoded_bytes;
    c.code_section_bytes_not_reached = std::max<std::int64_t>(in.code_bytes_held - in.decoded_bytes, 0);

    // the most called functions and the most used imports
    std::vector<const std::pair<const Va, std::vector<Va>>*> top;
    for (const auto& kv : g.callers) top.push_back(&kv);
    std::stable_sort(top.begin(), top.end(), [](const auto* a, const auto* b) { return a->second.size() > b->second.size(); });
    std::vector<const std::pair<const std::string, std::vector<Va>>*> used;
    for (const auto& kv : g.users) used.push_back(&kv);
    std::stable_sort(used.begin(), used.end(), [](const auto* a, const auto* b) { return a->second.size() > b->second.size(); });
    report.text.push_back("most called:");
    for (std::size_t i = 0; i < std::min<std::size_t>(top.size(), 15); ++i)
        report.text.push_back("  " + pad_right(in.label(top[i]->first), 28) + " " + std::to_string(top[i]->second.size()) + " callers");
    report.text.push_back("most used imports:");
    for (std::size_t i = 0; i < std::min<std::size_t>(used.size(), 15); ++i)
        report.text.push_back("  " + pad_right(used[i]->first, 36) + " " + std::to_string(used[i]->second.size()) + " functions");
    if (!g.cycles.empty()) {
        std::vector<std::string> groups;
        for (std::size_t i = 0; i < std::min<std::size_t>(g.cycles.size(), 8); ++i) {
            const std::vector<Va>& cycle = g.cycles[i];
            std::vector<std::string> labels;
            for (std::size_t k = 0; k < std::min<std::size_t>(cycle.size(), 4); ++k) labels.push_back(in.label(cycle[k]));
            groups.push_back(text::join(labels, ",") + (cycle.size() > 4 ? "..." : ""));
        }
        report.text.push_back("recursive groups: " + text::join(groups, "; "));
    }
    if (!in.discovered.empty()) {
        std::vector<Va> found = in.discovered;
        std::sort(found.begin(), found.end());
        std::vector<std::string> labels;
        for (std::size_t i = 0; i < std::min<std::size_t>(found.size(), 6); ++i) labels.push_back(in.label(found[i]));
        report.text.push_back(std::format("function starts found from calls (not in the decompiler's description): {}, e.g. {}",
                                          found.size(), text::join(labels, ", ")));
    }
    return report;
}

Report analyse(Image& img, const Graph& g) {
    ReportInput in;
    in.functions = img.functions();
    in.roots = img.roots().size();
    in.discovered = img.discovered();
    for (Va f : std::vector<Va>(img.functions()))
        for (const Instruction& i : img.function(f).insns) in.decoded_bytes += i.size;
    for (const Span& s : img.spans())
        if (s.executable) in.code_bytes_held += s.initialised;
    in.label = [&img](Va va) { return img.label(va); };
    in.import_name = [&img](Va va) -> std::optional<std::string> {
        if (!img.is_import(va)) return std::nullopt;
        return img.import_name(va);
    };
    return make_report(in, g);
}

}  // namespace cb::analyze::call_graph

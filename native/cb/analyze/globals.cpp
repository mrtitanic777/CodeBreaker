// cb/analyze/globals.cpp -- see globals.hpp. The engine is native/analyze/globals.cc (was cbanalyze.dll's), on an
// Image, the call graph and the strings table directly.
#include "cb/analyze/globals.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core/text.hpp"

namespace cb::analyze::globals {

std::string_view name_of(SlotKind kind) {
    switch (kind) {
        case SlotKind::Function: return "function";
        case SlotKind::Import: return "import";
        case SlotKind::String: return "string";
        case SlotKind::Code: return "code";
        case SlotKind::Data: return "data";
    }
    return "data";
}

std::string_view name_of(PointerTable::Shape shape) { return shape == PointerTable::Shape::Run ? "run" : "records"; }

namespace {

// The integer type of a size, or nothing.
const char* word(std::int64_t size) {
    switch (size) {
        case 1: return "unsigned char";
        case 2: return "unsigned short";
        case 4: return "unsigned long";
        case 8: return "double";
        case 10: return "long double";
    }
    return nullptr;
}

// The memory operand of `ins` that names `va` -> its position.
std::optional<std::size_t> operand(const Image& image, const Instruction& ins, Va va) {
    for (std::size_t k = 0; k < ins.operands.size(); ++k) {
        const Operand& op = ins.operands[k];
        if (op.kind == Operand::Kind::Memory && op.base.empty()) {
            const Va at = image.bits() == 32 ? op.value : image.data_segment() + (op.value & 0xFFFF);
            if (at == va) return k;
        }
    }
    return std::nullopt;
}

// Is `reg`, loaded at fn.insns[i], used as an address before it is overwritten?
bool dereferenced(const Function& fn, std::size_t i, const std::string& reg) {
    const std::string_view f = family(reg);
    for (std::size_t j = i + 1; j < fn.insns.size() && j < i + 9; ++j) {
        const Instruction& b = fn.insns[j];
        for (const Operand& op : b.operands) {
            if (op.kind == Operand::Kind::Memory && ((!op.base.empty() && family(op.base) == f) || (!op.index.empty() && family(op.index) == f))) {
                return true;
            }
        }
        if (b.writes.contains(std::string(f)) || b.flow == Flow::Jump || b.flow == Flow::Return ||
            (b.flow == Flow::Call && (f == "eax" || f == "ecx" || f == "edx"))) {
            return false;
        }
    }
    return false;
}

std::string type_of(const Variable& g, const strings::Entry* string, std::int64_t size) {
    const auto& notes = g.notes;
    if (string != nullptr && g.writers.empty()) {
        return std::format("{}[{}]", string->encoding == strings::Encoding::Wide ? "wchar_t" : "char", string->text.size() + 1);
    }
    if (notes.contains("called")) return "function pointer";
    if (notes.contains("far pointer")) return "far pointer";
    if (notes.contains("string pointer")) return "char *";
    if (notes.contains("float")) return size == 4 ? "float" : size == 8 ? "double" : size == 10 ? "long double" : "float";
    for (const std::string& n : notes) {   // sorted: the first "result of"
        if (n.starts_with("result of ")) return n;
    }
    if (notes.contains("array")) {
        const char* w = word(g.element != 0 ? g.element : size);
        return std::string(w != nullptr ? w : "unsigned char") + "[]";
    }
    if (notes.contains("pointer")) return "pointer";
    if (notes.contains("counter")) return std::format("counter ({} bytes)", size);
    if (notes.contains("flags") && g.values.size() >= 2) return std::format("flags ({} bytes)", size);
    if (g.taken != 0 && g.readers.empty() && g.writers.empty()) return std::format("object passed by address (up to {} bytes)", g.extent);
    const char* w = word(size);
    return w != nullptr ? w : std::format("{} bytes", size);
}

// The instruction of `fn` at `addr`.
std::size_t index_of(const Function& fn, Va addr) {
    const auto it = std::lower_bound(fn.insns.begin(), fn.insns.end(), addr, [](const Instruction& i, Va a) { return i.addr < a; });
    CB_ASSERT(it != fn.insns.end() && it->addr == addr, "a reference names an instruction of its function");
    return static_cast<std::size_t>(it - fn.insns.begin());
}

}  // namespace

std::map<Va, Variable> variables(Image& image, const call_graph::Graph& graph, const strings::Table& table) {
    std::map<Va, Variable> out;
    for (const call_graph::Reference& r : graph.refs) {
        const Va va = r.address;
        if (image.is_function(va) || image.is_import(va)) continue;
        const Span* s = image.span(va);
        if (s == nullptr || (s->executable && image.covering(va) != nullptr)) continue;   // an instruction, not data
        auto [it, fresh] = out.try_emplace(va);
        Variable& g = it->second;
        if (fresh) {
            g.at = va;
            g.writable = s->writable;
        }
        const Function& fn = image.function(r.function);
        const std::size_t i = index_of(fn, r.instruction);
        const Instruction& ins = fn.insns[i];
        const std::optional<std::size_t> k = operand(image, ins, va);
        if (r.how == call_graph::Access::AddressTaken) {
            ++g.taken;
            continue;
        }
        if (!k) {   // [reg + ADDRESS]: indexed from a register
            g.notes.insert("array");
            for (const Operand& o : ins.operands) {
                if (o.kind == Operand::Kind::Memory && !o.base.empty()) g.element = std::max<std::int64_t>(g.element, !o.index.empty() ? o.scale : o.size);
            }
            (r.how == call_graph::Access::Read ? g.readers : g.writers).insert(r.function);
            continue;
        }
        const Operand& op = ins.operands[*k];
        g.size = std::max<std::int64_t>(g.size, op.size);
        if (r.how == call_graph::Access::Read || r.how == call_graph::Access::ReadWrite) g.readers.insert(r.function);
        if (r.how == call_graph::Access::Write || r.how == call_graph::Access::ReadWrite) g.writers.insert(r.function);
        if (!op.index.empty()) {
            g.notes.insert("array");
            g.element = std::max<std::int64_t>(g.element, op.scale > 1 ? op.scale : op.size);
        }
        const std::string& mn = ins.mnemonic;
        const auto& ops = ins.operands;
        const bool immediate_second = ops.size() == 2 && ops[1].kind == Operand::Kind::Immediate;
        if (ins.flow == Flow::Call) {
            g.notes.insert("called");
        } else if (!mn.empty() && mn[0] == 'f' && mn != "fwait") {
            g.notes.insert(mn.starts_with("fi") ? "integer in the FPU" : "float");
        } else if (mn == "les" || mn == "lds") {
            g.notes.insert("far pointer");
        } else if (mn == "inc" || mn == "dec" || ((mn == "add" || mn == "sub") && immediate_second && ops[1].value == 1 && *k == 0)) {
            g.notes.insert("counter");
        } else if ((mn == "mov" || mn == "cmp" || mn == "test" || mn == "or" || mn == "and") && immediate_second && *k == 0) {
            // (an immediate the loader fixes up is an address, not a value)
            if (!(image.has_pointers() && image.pointer_at(ins.addr + ins.size - op.size) != nullptr)) g.values.insert(ops[1].value);
            if (mn == "or" || mn == "and" || mn == "test") g.notes.insert("flags");
        }
        if (mn == "mov" && *k == 1 && ops[0].kind == Operand::Kind::Register && dereferenced(fn, i, ops[0].reg)) g.notes.insert("pointer");
        if (mn == "mov" && *k == 0 && ops.size() > 1 && ops[1].kind == Operand::Kind::Register && family(ops[1].reg) == "eax") {
            const std::optional<Argument> src = definition(fn, static_cast<int>(i), ops[1].reg, 14);
            if (src && src->kind == Argument::Kind::CallResult && src->call_target && image.is_import(*src->call_target)) {
                const std::string& api = image.import_name(*src->call_target);
                const std::size_t bang = api.rfind('!');
                g.notes.insert("result of " + (bang == std::string::npos ? api : api.substr(bang + 1)));
            }
        }
    }
    // what is known at a higher address bounds a variable's extent
    std::set<Va> known;
    for (const auto& [va, g] : out) known.insert(va);
    for (const auto& [va, entry] : table) known.insert(va);
    for (auto& [va, g] : out) {
        const Span* s = image.span(va);
        const Va end = s->va + static_cast<Va>(s->data.size());
        const auto above = known.upper_bound(va);
        g.extent = std::min<std::int64_t>((above != known.end() && *above < end ? *above : end) - va, 0x10000);
        const std::int64_t size = g.size != 0 ? g.size : g.element != 0 ? g.element : (image.bits() == 32 ? 4 : 2);
        if (va - s->va < s->initialised) {
            const Bytes raw = image.read(va, std::min<std::int64_t>(size, 8));
            std::uint64_t held = 0;
            for (std::size_t b = 0; b < raw.size(); ++b) held |= static_cast<std::uint64_t>(raw[b]) << (8 * b);
            g.initial = {Initial::Kind::Value, held, {}};
        }
        const Pointer* p = image.pointer_at(va);
        if (image.has_pointers() && p != nullptr && p->target) {
            const Va t = *p->target;
            g.initial = {Initial::Kind::Address, 0, "&" + image.label(t)};
            if (image.is_function(t) || image.is_import(t)) g.notes.insert("called");
            else if (table.contains(t)) g.notes.insert("string pointer");
            else g.notes.insert("pointer");
        }
        const auto string = table.find(va);
        g.type = type_of(g, string != table.end() ? &string->second : nullptr, size);
        g.size = size;
    }
    return out;
}

namespace {

SlotKind slot_kind(const Image& image, const strings::Table& table, Va t) {
    if (image.is_function(t)) return SlotKind::Function;
    if (image.is_import(t)) return SlotKind::Import;
    if (table.contains(t)) return SlotKind::String;
    if (image.is_code(t)) return SlotKind::Code;
    return SlotKind::Data;
}

}  // namespace

std::vector<PointerTable> pointer_tables(Image& image, const strings::Table& table) {
    constexpr std::int64_t kWord = 4;
    std::map<Va, Va> sites;   // where an address is held -> the address
    if (image.has_pointers()) {
        for (const auto& [site, p] : image.pointer_list()) {
            if (!p.target || (p.kind != PointerKind::Abs32 && p.kind != PointerKind::Far) ||
                (image.is_code(site) && (image.kind() == ImageKind::Ne || image.covering(site) != nullptr))) {
                continue;
            }
            sites[site] = *p.target;
        }
    } else {   // no relocations: a word that is a function or a referenced string is an address
        for (const Span& s : image.spans()) {
            if (s.executable) continue;
            for (std::int64_t off = 0; off < s.initialised - 3; off += 4) {
                std::uint64_t v = 0;
                for (int b = 0; b < 4 && off + b < static_cast<std::int64_t>(s.data.size()); ++b) {
                    v |= static_cast<std::uint64_t>(s.data[static_cast<std::size_t>(off + b)]) << (8 * b);
                }
                const auto t = table.find(static_cast<Va>(v));
                if (image.is_function(static_cast<Va>(v)) || (t != table.end() && t->second.referenced)) sites[s.va + off] = static_cast<Va>(v);
            }
        }
    }
    std::vector<Va> order;
    for (const auto& [site, target] : sites) order.push_back(site);
    std::vector<PointerTable> tables;
    std::set<Va> used;
    auto fill = [&](PointerTable& t, std::size_t from, std::size_t to, const std::vector<Va>& places) {
        for (std::size_t k = from; k <= to; ++k) {
            t.targets.push_back(sites[places[k]]);
            t.kinds.push_back(slot_kind(image, table, sites[places[k]]));
        }
    };
    std::size_t i = 0;
    while (i < order.size()) {   // runs of consecutive words
        std::size_t j = i;
        while (j + 1 < order.size() && order[j + 1] == order[j] + kWord) ++j;
        if (j > i) {
            PointerTable t{order[i], static_cast<std::int64_t>(j - i + 1), kWord, PointerTable::Shape::Run, {}, {}};
            fill(t, i, j, order);
            const bool all_code = std::all_of(t.kinds.begin(), t.kinds.end(), [](SlotKind k) { return k == SlotKind::Code; });
            if (!all_code) tables.push_back(std::move(t));   // all-code runs are jump tables (see dispatch)
            for (std::size_t k = i; k <= j; ++k) used.insert(order[k]);
        }
        i = j + 1;
    }
    std::vector<Va> singles;
    for (Va a : order) {
        if (!used.contains(a)) singles.push_back(a);
    }
    i = 0;
    while (i + 2 < singles.size()) {   // the same field of an array of structures
        const std::int64_t stride = singles[i + 1] - singles[i];
        const bool plausible = 8 <= stride && stride <= 128;
        std::size_t j = i + 1;
        while (plausible && j + 1 < singles.size() && singles[j + 1] - singles[j] == stride) ++j;
        if (j - i >= 2 && plausible) {
            PointerTable t{singles[i], static_cast<std::int64_t>(j - i + 1), stride, PointerTable::Shape::Records, {}, {}};
            fill(t, i, j, singles);
            tables.push_back(std::move(t));
            i = j + 1;
        } else {
            ++i;
        }
    }
    std::stable_sort(tables.begin(), tables.end(), [](const PointerTable& a, const PointerTable& b) { return a.at < b.at; });
    return tables;
}

namespace {

// How a type is counted: its name without the size or length, strings together, system-call results together.
std::string type_key(const std::string& type) {
    if (type.starts_with("result of")) return "result of a system call";
    if (type.ends_with("]") && (type[0] == 'c' || type[0] == 'w')) return "string";
    std::string_view key = type;
    key = key.substr(0, key.find('['));
    key = key.substr(0, key.find(" ("));
    return std::string(key);
}

}  // namespace

Report make_report(const std::map<Va, Variable>& variables, const std::vector<PointerTable>& tables,
                   const std::function<std::string(Va)>& label) {
    Report report;
    OrderedMap<std::string, std::int64_t> by_type;   // first seen first: a tie in the summary keeps that order
    std::int64_t arrays = 0;
    for (const auto& [va, v] : variables) {
        GlobalFact g;
        g.at = v.at;
        g.size = v.size;
        g.readers = static_cast<std::int64_t>(v.readers.size());
        g.writers = static_cast<std::int64_t>(v.writers.size());
        g.taken = v.taken;
        g.notes.assign(v.notes.begin(), v.notes.end());
        for (std::int64_t x : v.values) {
            if (g.values.size() < 24) g.values.push_back(x);
        }
        g.writable = v.writable;
        g.extent = v.extent;
        g.initial = v.initial;
        g.type = v.type;
        report.globals.push_back(std::move(g));
        ++by_type[type_key(v.type)];
        if (v.type.ends_with("[]")) ++arrays;
    }
    for (const PointerTable& t : tables) {
        TableFact f;
        f.at = t.at;
        f.count = t.count;
        f.stride = t.stride;
        f.shape = t.shape;
        for (SlotKind k : t.kinds) ++f.holds[std::string(name_of(k))];
        for (std::size_t k = 0; k < std::min<std::size_t>(8, t.targets.size()); ++k) f.first.push_back(label(t.targets[k]));
        report.tables.push_back(std::move(f));
    }
    auto count_of = [&](const char* key) -> std::int64_t {
        const std::int64_t* n = by_type.find(key);
        return n == nullptr ? 0 : *n;
    };
    Counts& c = report.counts;
    c.globals = static_cast<std::int64_t>(variables.size());
    for (const auto& [key, n] : by_type) {
        const bool plain = key == "unsigned char" || key == "unsigned short" || key == "unsigned long";
        if (!plain && key.find("bytes") == std::string::npos) c.typed_beyond_size += n;
    }
    c.pointers = count_of("pointer") + count_of("char *") + count_of("far pointer");
    c.function_pointers = count_of("function pointer");
    c.strings = count_of("string");
    c.arrays = arrays;
    c.counters = count_of("counter");
    c.flags = count_of("flags");
    c.results_of_system_calls = count_of("result of a system call");
    c.floats = count_of("float") + count_of("double");
    for (const auto& [va, v] : variables) {
        if (v.initial.kind == Initial::Kind::Uninitialised) ++c.uninitialised;
        if (!v.writers.empty() && v.readers.empty() && v.taken == 0) ++c.written_never_read;
        if (!v.readers.empty() && v.writers.empty() && v.taken == 0) ++c.read_never_written;
    }
    for (const PointerTable& t : tables) {
        (t.shape == PointerTable::Shape::Run ? c.pointer_tables : c.record_tables) += 1;
        c.table_entries += t.count;
        c.function_table_entries += std::count(t.kinds.begin(), t.kinds.end(), SlotKind::Function);
    }

    // the summary
    std::vector<std::pair<std::string, std::int64_t>> types(by_type.begin(), by_type.end());
    std::stable_sort(types.begin(), types.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    std::vector<std::string> parts;
    for (const auto& [key, n] : types) parts.push_back(std::format("{} {}", key, n));
    report.text.push_back("types: " + text::join(parts, ", "));
    std::vector<const Variable*> busiest;
    for (const auto& [va, v] : variables) busiest.push_back(&v);
    std::stable_sort(busiest.begin(), busiest.end(), [](const Variable* a, const Variable* b) {
        return a->readers.size() + a->writers.size() > b->readers.size() + b->writers.size();
    });
    for (std::size_t n = 0; n < std::min<std::size_t>(12, busiest.size()); ++n) {
        const Variable& v = *busiest[n];
        std::string values;
        if (!v.values.empty()) {
            std::vector<std::string> shown;
            for (std::int64_t x : v.values) {
                if (shown.size() < 8) shown.push_back(std::format("{:x}", x));
            }
            values = "  values " + text::join(shown, ",");
        }
        report.text.push_back(std::format("  {:x}  {:<28} read by {}, written by {}{}", v.at, v.type, v.readers.size(), v.writers.size(), values));
    }
    for (std::size_t n = 0; n < std::min<std::size_t>(8, tables.size()); ++n) {
        const PointerTable& t = tables[n];
        std::set<std::string> kinds;
        for (SlotKind k : t.kinds) kinds.insert(std::string(name_of(k)));
        std::vector<std::string> targets;
        for (std::size_t k = 0; k < std::min<std::size_t>(4, t.targets.size()); ++k) targets.push_back(label(t.targets[k]));
        report.text.push_back(std::format("  table {:x}: {} {}, e.g. {}", t.at, t.count, text::join(kinds, "/"), text::join(targets, ", ")));
    }
    return report;
}

Report analyse(Image& image, const call_graph::Graph& graph, const strings::Table& table) {
    const std::map<Va, Variable> vars = variables(image, graph, table);
    const std::vector<PointerTable> tables = pointer_tables(image, table);
    return make_report(vars, tables, [&](Va a) { return image.label(a); });
}

}  // namespace cb::analyze::globals

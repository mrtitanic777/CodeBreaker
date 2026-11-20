// analyze/structures.cpp -- see structures.hpp.
#include "analyze/structures.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "core/text.hpp"

namespace cb::analyze::structures {

std::string_view name_of(Note note) {
    switch (note) {
        case Note::AddressTaken: return "address taken";
        case Note::Array: return "array";
        case Note::Called: return "called";
        case Note::Float: return "float";
        case Note::Pointer: return "pointer";
    }
    return "?";
}

bool operator==(const Root& a, const Root& b) {
    if (a.kind != b.kind || a.at != b.at || a.index != b.index) return false;
    if (static_cast<bool>(a.of) != static_cast<bool>(b.of)) return false;
    return !a.of || *a.of == *b.of;
}

std::strong_ordering operator<=>(const Root& a, const Root& b) {
    if (const auto c = a.kind <=> b.kind; c != 0) return c;
    if (const auto c = a.at <=> b.at; c != 0) return c;
    if (const auto c = a.index <=> b.index; c != 0) return c;
    if (!a.of || !b.of) return static_cast<bool>(a.of) <=> static_cast<bool>(b.of);
    return *a.of <=> *b.of;
}

namespace {

// The mnemonics whose first operand is read (not written), and those that only write it: as call_graph reads them.
constexpr std::array<std::string_view, 22> kReads = {"cmp",  "test", "push", "bt",   "call", "jmp",   "lcall", "ljmp",  "fld",  "fild",  "fcomp",
                                                     "fcom", "fadd", "fsub", "fmul", "fdiv", "ficom", "ficomp", "bound", "div",  "idiv", "mul"};
constexpr std::array<std::string_view, 15> kPureWrites = {"mov",   "pop",    "fstp",   "fst",    "fistp",  "fist",  "lea",  "movzx",
                                                          "movsx", "sidt",   "sgdt",   "fstcw",  "fnstcw", "fnstsw", "fstsw"};

template <std::size_t N>
bool in(std::string_view mnemonic, const std::array<std::string_view, N>& list) {
    return std::find(list.begin(), list.end(), mnemonic) != list.end();
}

// The allocators whose result is a structure, and which argument (from 0) is its size; none: no constant size to read.
std::optional<std::optional<int>> allocator_size_argument(std::string_view stem) {
    static const std::map<std::string_view, std::optional<int>> table = {
        {"LocalAlloc", 1}, {"GlobalAlloc", 1}, {"HeapAlloc", 2}, {"malloc", 0}, {"calloc", std::nullopt}, {"??2@YAPAXI@Z", 0},
        {"LOCALALLOC", 0}, {"GLOBALALLOC", std::nullopt}, {"CoTaskMemAlloc", 0}, {"SHAlloc", 0}, {"VirtualAlloc", 1}};
    const auto it = table.find(stem);
    if (it == table.end()) return std::nullopt;
    return it->second;
}

// Bytes `ins` adds to what is pushed (negative: removes), or none when it is not plain.
std::optional<Va> stack_delta(const Instruction& ins) {
    const auto& ops = ins.operands;
    if (ins.mnemonic == "push") return ops.empty() ? 4 : ops[0].size;
    if (ins.mnemonic == "pop") return -(ops.empty() ? 4 : ops[0].size);
    if ((ins.mnemonic == "add" || ins.mnemonic == "sub") && ops.size() == 2 && ops[0].kind == Operand::Kind::Register &&
        family(ops[0].reg) == "esp" && ops[1].kind == Operand::Kind::Immediate) {
        const int size = ops[1].size;
        Va n = ops[1].value;
        if (size > 0 && size < 8 && n >= (Va{1} << (8 * size - 1))) n -= Va{1} << (8 * size);
        return ins.mnemonic == "sub" ? n : -n;
    }
    if (ins.writes.contains("esp")) return std::nullopt;
    return 0;
}

// A pointer and how far past its start the register points.
struct Held {
    Root root;
    Va delta = 0;
};

Root make(Root::Kind kind, Va at, Va index = 0) {
    Root r;
    r.kind = kind;
    r.at = at;
    r.index = index;
    return r;
}
Root field_of(const Root& parent, Va offset) {
    Root r;
    r.kind = Root::Kind::Field;
    r.index = offset;
    r.of = std::make_shared<const Root>(parent);
    return r;
}

using Fields = std::map<Root, std::map<Va, Field>>;   // by root, then offset
using Edge = std::pair<Root, Root>;                    // (the pointer, the pointer it is handed on as)

// What every scan adds to.
struct Findings {
    Fields fields;
    std::set<Edge> edges;
    std::map<Root, Va> sizes;                          // an allocation's exact size
    std::map<Root, std::set<std::string>> uses;        // the system calls a root is handed to
    std::vector<std::pair<Root, Va>> passed;           // `this` handed on in ecx: (pointer, callee)
};

Field& field(Fields& fields, const Root& root, Va offset, Va size) {
    Field& f = fields[root][offset];
    f.size = std::max(f.size, size);
    return f;
}

// Follows pointers through one function.
class Scan {
public:
    Scan(Image& image, const Function& fn, const ConventionFact& fact, const call_graph::Graph& graph, Findings& out)
        : image_(image), fn_(fn), fact_(fact), out_(out) {
        word_ = image.bits() == 32 ? 4 : 2;
        first_ = image.bits() == 32 ? 2 * word_ : (fact.is_far() ? 6 : 4);
        for (const call_graph::Call& c : graph.calls.at(fn.start)) calls_.emplace(c.site, &c);
        if (fact.takes_in("ecx") && image.bits() == 32) holds_["ecx"] = Held{make(Root::Kind::This, fn.start), 0};
        if (!fact.frame) depth_ = 0;
    }

    void run() {
        std::unordered_set<Va> labels;
        for (const Instruction& i : fn_.insns) {
            if ((i.flow == Flow::CondJump || i.flow == Flow::Jump) && i.target) labels.insert(*i.target);
        }
        for (std::size_t i = 0; i < fn_.insns.size(); ++i) {
            const Instruction& ins = fn_.insns[i];
            if (labels.contains(ins.addr)) {
                for (const char* r : {"eax", "ecx", "edx"}) {
                    if (!(std::string_view(r) == "ecx" && i == 0)) holds_.erase(r);
                }
                pending_.clear();
                if (depth_ && *depth_ != 0) depth_.reset();
            }
            step(i, ins);
        }
    }

private:
    // The (root, delta) a memory operand's base register holds.
    const Held* bound(const Operand& op) const {
        if (op.base.empty()) return nullptr;
        const auto it = holds_.find(std::string(family(op.base)));
        return it == holds_.end() ? nullptr : &it->second;
    }

    // Which argument slot a memory operand is.
    std::optional<Va> argument(const Operand& op) const {
        if (op.base.empty() || !op.index.empty()) return std::nullopt;
        const std::string_view fam = family(op.base);
        if (fact_.frame && fam == "ebp" && op.value >= first_) return (op.value - first_) / word_;
        if (depth_ && fam == "esp" && image_.bits() == 32 && op.value >= *depth_ + word_) return (op.value - *depth_ - word_) / word_;
        return std::nullopt;
    }

    // The address a memory operand with no base names in the data segment.
    Va data_address(const Operand& op) const { return image_.bits() == 32 ? op.value : image_.data_segment() + (op.value & 0xFFFF); }

    void drop(std::initializer_list<const char*> registers) {
        for (const char* r : registers) {
            holds_.erase(r);
            pending_.erase(r);
        }
    }

    void step(std::size_t i, const Instruction& ins) {
        const std::string& mn = ins.mnemonic;
        const auto& ops = ins.operands;
        // every access through a followed pointer is a field
        for (std::size_t k = 0; k < ops.size(); ++k) {
            const Operand& op = ops[k];
            if (op.kind != Operand::Kind::Memory) continue;
            if (!op.base.empty() && mn != "lea") {
                const std::string fam(family(op.base));
                const auto loaded = pending_.find(fam);
                if (loaded != pending_.end()) {   // a loaded field used as an address
                    const auto [root, off] = loaded->second;
                    field(out_.fields, root, off, word_).notes.insert(Note::Pointer);
                    holds_[fam] = Held{field_of(root, off), 0};
                    pending_.erase(loaded);
                }
            }
            const Held* b = bound(op);
            if (b == nullptr) continue;
            const Va off = b->delta + op.value;
            if (off < 0 || off >= kMaxOffset) continue;
            if (mn == "lea") {
                if (off != 0) field(out_.fields, b->root, off, 0).notes.insert(Note::AddressTaken);
                continue;
            }
            Field& f = field(out_.fields, b->root, off, op.size);
            f.users.insert(fn_.start);
            const bool write = k == 0 && !in(mn, kReads);
            ++(write ? f.writes : f.reads);
            if (write && !in(mn, kPureWrites)) ++f.reads;
            if (!op.index.empty()) {
                f.notes.insert(Note::Array);
                f.element = std::max(f.element, op.scale > 1 ? Va{op.scale} : Va{op.size});
            }
            if (mn[0] == 'f' && mn != "fwait") f.notes.insert(Note::Float);
            if (ins.flow == Flow::Call) f.notes.insert(Note::Called);
            if (write && mn == "mov" && ops.size() == 2 && ops[1].kind == Operand::Kind::Immediate && ops[1].size == word_) {
                f.stores.insert(ops[1].value);
            }
            if (write && mn == "mov" && ops.size() == 2 && ops[1].kind == Operand::Kind::Register && (op.size == word_ || op.size == 4)) {
                const auto src = holds_.find(std::string(family(ops[1].reg)));   // a followed pointer stored in a field
                if (src != holds_.end() && src->second.delta == 0) out_.edges.emplace(src->second.root, field_of(b->root, off));
            }
        }
        if (ins.flow == Flow::Call) return call(i, ins);
        if (ins.flow == Flow::Return) {
            const auto b = holds_.find("eax");
            if (b != holds_.end() && b->second.delta == 0 && b->second.root != make(Root::Kind::Returns, fn_.start)) {
                out_.edges.emplace(b->second.root, make(Root::Kind::Returns, fn_.start));
            }
            return;
        }
        if (mn == "push") {
            std::optional<Held> b;
            if (!ops.empty() && ops[0].kind == Operand::Kind::Register) {
                if (const auto it = holds_.find(std::string(family(ops[0].reg))); it != holds_.end()) b = it->second;
            } else if (!ops.empty() && ops[0].kind == Operand::Kind::Memory) {
                if (const auto a = argument(ops[0])) {
                    b = Held{make(Root::Kind::Arg, fn_.start, *a), 0};
                } else if (const auto it = slots_.find({std::string(family(ops[0].base)), ops[0].value}); it != slots_.end()) {
                    b = it->second;
                }
            }
            pushed_.push_back(b);
            if (depth_) *depth_ += word_;
            return;
        }
        if (depth_) {
            const auto d = stack_delta(ins);
            if (d) *depth_ += *d; else depth_.reset();
        }
        if (ops.empty() || (ops[0].kind != Operand::Kind::Register && ops[0].kind != Operand::Kind::Memory)) return;
        // where pointers go
        if (ops[0].kind == Operand::Kind::Register) return register_written(ins);
        if (mn == "mov" && !ops[0].base.empty() && (family(ops[0].base) == "ebp" || family(ops[0].base) == "esp") && ops[0].index.empty() &&
            ops.size() == 2) {
            const std::pair<std::string, Va> key{std::string(family(ops[0].base)), ops[0].value};
            const auto b = ops[1].kind == Operand::Kind::Register ? holds_.find(std::string(family(ops[1].reg))) : holds_.end();
            if (b != holds_.end()) slots_[key] = b->second; else slots_.erase(key);
        } else if (mn == "mov" && ops[0].base.empty() && ops[0].index.empty() && ops.size() == 2 && ops[1].kind == Operand::Kind::Register) {
            const auto b = holds_.find(std::string(family(ops[1].reg)));   // a followed pointer stored in a global
            const Va va = data_address(ops[0]);
            if (b != holds_.end() && b->second.delta == 0 && image_.span(va) != nullptr && (ops[0].seg.empty() || ops[0].seg == "ds")) {
                out_.edges.emplace(b->second.root, make(Root::Kind::Global, va));
            }
        } else {
            for (const std::string& r : ins.writes) {
                if (r != "esp" && r != "ebp") holds_.erase(r);
                pending_.erase(r);
            }
        }
    }

    // calls: what is handed on, and what comes back
    void call(std::size_t i, const Instruction& ins) {
        const auto found = calls_.find(ins.addr);
        const bool direct = found != calls_.end() && found->second->kind == call_graph::CallKind::Call && found->second->destination;
        const bool imported = found != calls_.end() && found->second->kind == call_graph::CallKind::Import && found->second->destination;
        const Va dest = found != calls_.end() && found->second->destination ? *found->second->destination : 0;
        const std::size_t recent = std::min<std::size_t>(pushed_.size(), 8);
        // (the last eight pushes, the most recent first: the first argument first)
        auto each_pushed = [&](auto&& visit) {
            for (std::size_t j = 0; j < recent; ++j) {
                const auto& b = pushed_[pushed_.size() - 1 - j];
                if (b && b->delta == 0) visit(static_cast<Va>(j), b->root);
            }
        };
        std::string api;
        if (imported) {
            const std::string& full = image_.import_name(dest);
            api = full.substr(full.rfind('!') == std::string::npos ? 0 : full.rfind('!') + 1);
        }
        if (direct) {
            each_pushed([&](Va j, const Root& r) { out_.edges.emplace(r, make(Root::Kind::Arg, dest, j)); });
            const auto ecx = holds_.find("ecx");
            if (ecx != holds_.end() && ecx->second.delta == 0 && image_.bits() == 32) out_.passed.emplace_back(ecx->second.root, dest);
        } else if (imported) {
            each_pushed([&](Va j, const Root& r) {
                out_.fields[r];
                out_.uses[r].insert(std::format("{} argument {}", api, j));
            });
        }
        drop({"eax", "ecx", "edx"});
        if (imported) {
            const std::string_view stem = (api.size() > 1 && (api.back() == 'A' || api.back() == 'W') &&
                                           allocator_size_argument(std::string_view(api).substr(0, api.size() - 1)))
                                              ? std::string_view(api).substr(0, api.size() - 1)
                                              : std::string_view(api);
            const auto size_argument = allocator_size_argument(stem);
            if (size_argument && *size_argument) {
                const std::size_t k = static_cast<std::size_t>(**size_argument);
                const std::vector<Argument> args = arguments(fn_, static_cast<int>(i), k + 1);
                if (k < args.size() && args[k].kind == Argument::Kind::Immediate && args[k].value > 0 && args[k].value < kMaxOffset) {
                    const Root root = make(Root::Kind::Alloc, ins.addr);
                    holds_["eax"] = Held{root, 0};
                    out_.sizes[root] = args[k].value;
                    out_.fields[root];
                }
            }
        }
        if (direct) {
            holds_["eax"] = Held{make(Root::Kind::Returns, dest), 0};
        } else if (imported && !holds_.contains("eax")) {
            holds_["eax"] = Held{make(Root::Kind::Result, ins.addr), 0};
        }
        pushed_.clear();
    }

    // The first operand is a register: it takes a pointer, or loses the one it held.
    void register_written(const Instruction& ins) {
        const std::string& mn = ins.mnemonic;
        const auto& ops = ins.operands;
        const std::string dst(family(ops[0].reg));
        std::optional<Held> fresh;
        if ((mn == "mov" || mn == "les" || mn == "lds") && ops.size() == 2) {
            const Operand& src = ops[1];
            if (src.kind == Operand::Kind::Register) {
                const std::string fam(family(src.reg));
                if (const auto it = holds_.find(fam); it != holds_.end()) {
                    fresh = it->second;
                } else if (pending_.contains(fam) && dst != fam) {
                    pending_[dst] = pending_.at(fam);
                    holds_.erase(dst);
                    return;
                }
            } else if (src.kind == Operand::Kind::Memory) {
                if (const auto a = argument(src)) {
                    fresh = Held{make(Root::Kind::Arg, fn_.start, *a), 0};
                } else if (src.base.empty() && src.index.empty()) {
                    const Va va = data_address(src);
                    if (image_.span(va) != nullptr && !image_.is_import(va) && (src.size == word_ || src.size == 4)) {
                        fresh = Held{make(Root::Kind::Global, va), 0};
                    }
                } else if (!src.base.empty() && (family(src.base) == "ebp" || family(src.base) == "esp") &&
                           slots_.contains({std::string(family(src.base)), src.value})) {
                    fresh = slots_.at({std::string(family(src.base)), src.value});
                } else if (const Held* b = bound(src); b != nullptr && (src.size == word_ || src.size == 4) && src.index.empty()) {
                    if (b->delta + src.value >= 0 && b->delta + src.value < kMaxOffset) {
                        pending_[dst] = {b->root, b->delta + src.value};
                        holds_.erase(dst);
                        return;
                    }
                }
            }
        } else if (mn == "lea" && ops.size() == 2 && ops[1].kind == Operand::Kind::Memory && ops[1].index.empty()) {
            if (const Held* b = bound(ops[1])) fresh = Held{b->root, b->delta + ops[1].value};
        }
        if (ins.writes.contains(dst) || fresh) {
            pending_.erase(dst);
            if (fresh) holds_[dst] = *fresh; else holds_.erase(dst);
        }
        for (const std::string& r : ins.writes) {
            if (r != dst && r != "esp" && r != "ebp") holds_.erase(r);
        }
    }

    Image& image_;
    const Function& fn_;
    const ConventionFact& fact_;
    Findings& out_;
    Va word_ = 4, first_ = 8;
    std::optional<Va> depth_;                                     // bytes pushed so far, while it can be followed
    std::unordered_map<Va, const call_graph::Call*> calls_;       // by call site
    std::map<std::string, Held> holds_;                           // register -> the pointer it holds
    std::map<std::pair<std::string, Va>, Held> slots_;            // frame slot -> the same
    std::vector<std::optional<Held>> pushed_;
    std::map<std::string, std::pair<Root, Va>> pending_;          // register -> the (root, offset) of the field it was loaded from
};

}  // namespace

std::string describe(const Image& image, const Root& root) {
    switch (root.kind) {
        case Root::Kind::Arg: return std::format("argument {} of {}", root.index, image.label(root.at));
        case Root::Kind::This: return std::format("this of {}", image.label(root.at));
        case Root::Kind::Global: return std::format("what global {:x} points at", root.at);
        case Root::Kind::Alloc: return std::format("allocated at {:x}", root.at);
        case Root::Kind::Returns: return std::format("what {} returns", image.label(root.at));
        case Root::Kind::Result: return std::format("result of the call at {:x}", root.at);
        case Root::Kind::Field: return std::format("field +{:x} of ({})", root.index, describe(image, *root.of));
    }
    return {};
}

std::vector<Structure> recover(Image& image, const call_graph::Graph& graph, const ConventionFacts& conventions) {
    Findings found;
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        if (!fn.insns.empty()) Scan(image, fn, conventions.at(start), graph, found).run();
    }
    for (const auto& [root, dest] : found.passed) {   // `this` handed on in ecx
        if (conventions.at(dest).takes_in("ecx")) found.edges.emplace(root, make(Root::Kind::This, dest));
    }
    // Which pointers are one structure. A pointer handed on is the pointer received -- unless the receiver takes
    // pointers from many places (a helper for anything: free, a list insert), or leads to no field at all, or the two
    // are known to differ in size.
    std::vector<Edge> edges;
    for (const Edge& e : found.edges) {
        if (e.first != e.second) edges.push_back(e);
    }
    std::map<Root, std::set<Root>> sources;
    for (const auto& [a, b] : edges) sources[b].insert(a);
    std::set<Root> useful;
    for (const auto& [root, fs] : found.fields) {
        if (!fs.empty()) useful.insert(root);
    }
    for (bool grew = true; grew;) {
        grew = false;
        for (const auto& [a, b] : edges) {
            if (useful.contains(b) && !useful.contains(a)) {
                useful.insert(a);
                grew = true;
            }
        }
    }
    std::map<Root, Root> parent;
    auto find = [&](const Root& x) {
        Root root = x;
        while (parent.contains(root) && parent.at(root) != root) root = parent.at(root);
        for (Root at = x; parent.contains(at) && parent.at(at) != root;) {   // (the path is shortened)
            Root next = parent.at(at);
            parent[at] = root;
            at = std::move(next);
        }
        parent.try_emplace(root, root);
        return root;
    };
    struct Extent {
        std::optional<Va> exact;   // an allocation's size
        Va end = 0;                // where its last field ends
    };
    std::map<Root, Extent> extent;
    for (const auto& [root, fs] : found.fields) {
        Extent e;
        if (const auto it = found.sizes.find(root); it != found.sizes.end()) e.exact = it->second;
        for (const auto& [off, f] : fs) e.end = std::max(e.end, off + f.size);
        extent[root] = e;
    }
    for (const auto& [a, b] : edges) {
        if (sources.at(b).size() > kGeneric || !useful.contains(b) || !useful.contains(a)) continue;
        const Root ra = find(a), rb = find(b);
        if (ra == rb) continue;
        const Extent ea = extent.contains(ra) ? extent.at(ra) : Extent{}, eb = extent.contains(rb) ? extent.at(rb) : Extent{};
        if (ea.exact && eb.exact && *ea.exact != *eb.exact) continue;
        const std::optional<Va> exact = ea.exact ? ea.exact : eb.exact;
        if (exact && std::max(ea.end, eb.end) > *exact) continue;
        const auto& [keep, gone] = ra < rb ? std::pair<const Root&, const Root&>(ra, rb) : std::pair<const Root&, const Root&>(rb, ra);
        parent[gone] = keep;
        extent[keep] = Extent{exact, std::max(ea.end, eb.end)};
    }
    std::map<Root, std::vector<Root>> groups;
    for (const auto& [root, fs] : found.fields) groups[find(root)].push_back(root);
    std::vector<Structure> out;
    for (const auto& [rep, roots] : groups) {
        std::map<Va, Field> merged;
        for (const Root& r : roots) {
            for (const auto& [off, f] : found.fields.at(r)) {
                Field& m = merged[off];
                m.size = std::max(m.size, f.size);
                m.reads += f.reads;
                m.writes += f.writes;
                m.notes.insert(f.notes.begin(), f.notes.end());
                m.stores.insert(f.stores.begin(), f.stores.end());
                m.users.insert(f.users.begin(), f.users.end());
                m.element = std::max(m.element, f.element);
            }
        }
        std::erase_if(merged, [](const auto& kv) { return kv.second.size == 0 && kv.second.notes.empty(); });
        std::vector<Va> exact;
        for (const Root& r : roots) {
            if (const auto it = found.sizes.find(r); it != found.sizes.end()) exact.push_back(it->second);
        }
        if (merged.size() < 2 && !(!merged.empty() && !exact.empty())) continue;
        Structure s;
        s.roots = roots;
        std::sort(s.roots.begin(), s.roots.end());
        Va low = 0;
        for (const auto& [off, f] : merged) low = std::max(low, off + std::max<Va>(f.size, 1));
        std::set<std::string> uses;
        for (const Root& r : roots) {
            if (const auto it = found.uses.find(r); it != found.uses.end()) uses.insert(it->second.begin(), it->second.end());
        }
        std::set<Va> functions;
        for (const auto& [off, f] : merged) functions.insert(f.users.begin(), f.users.end());
        s.exact = !exact.empty();
        s.size = exact.empty() ? low : *std::max_element(exact.begin(), exact.end());
        s.passed_to.assign(uses.begin(), uses.end());
        s.functions.assign(functions.begin(), functions.end());
        s.fields = std::move(merged);
        out.push_back(std::move(s));
    }
    std::sort(out.begin(), out.end(), [](const Structure& a, const Structure& b) {
        if (a.fields.size() != b.fields.size()) return a.fields.size() > b.fields.size();
        return a.roots.front() < b.roots.front();
    });
    for (std::size_t n = 0; n < out.size(); ++n) out[n].name = std::format("struct_{}", n + 1);
    return out;
}

std::string field_type(const Field& f) {
    const bool called = f.notes.contains(Note::Called);
    auto by_size = [](Va size, bool wide) -> std::string {
        switch (size) {
            case 1: return "unsigned char";
            case 2: return "unsigned short";
            case 4: return "unsigned long";
            case 8: return wide ? "double" : "unsigned char";
            case 10: return wide ? "long double" : "unsigned char";
            default: return "unsigned char";
        }
    };
    if (called) return "function pointer";
    if (f.notes.contains(Note::Float)) return f.size == 4 ? "float" : "double";
    if (f.notes.contains(Note::Pointer)) return "pointer";
    if (f.notes.contains(Note::Array)) return by_size(f.element != 0 ? f.element : f.size, false) + "[]";
    if (f.notes.contains(Note::AddressTaken) && f.size == 0) return "embedded object";
    return by_size(f.size, true);
}

std::string layout(const Structure& s) {
    std::vector<std::string> lines;
    lines.push_back(std::format("struct {} {{{}", s.name, s.exact ? std::format("    /* {} bytes */", s.size) : std::string()));
    Va at = 0;
    for (const auto& [off, f] : s.fields) {
        if (off > at) lines.push_back(std::format("    unsigned char gap_{:x}[{}];", at, off - at));
        if (off < at) continue;   // bytes of the field before it, accessed singly
        if (f.size == 0 && f.notes.contains(Note::AddressTaken)) {
            const auto inner = s.fields.upper_bound(off);
            if (inner != s.fields.end() && inner->first < off + 4) continue;
        }
        const std::string type = field_type(f);
        const std::string name = std::format("f_{:x}", off);
        std::string decl;
        if (type == "function pointer") decl = std::format("void (*{})()", name);
        else if (type == "pointer") decl = std::format("void *{}", name);
        else if (type == "embedded object") decl = std::format("unsigned char {}[1]", name);
        else if (type.ends_with("[]")) decl = std::format("{} {}[1]", type.substr(0, type.size() - 2), name);
        else decl = std::format("{} {}", type, name);
        std::vector<std::string> extra;
        for (Note n : f.notes) {
            if (n != Note::Pointer && n != Note::Called && n != Note::Float) extra.emplace_back(name_of(n));
        }
        lines.push_back(std::format("    {:<34} /* +{:x}  read {}, written {}{} */", decl + ";", off, f.reads, f.writes,
                                    extra.empty() ? std::string() : ", " + text::join(extra, ", ")));
        at = off + (f.size != 0 ? f.size : 1);
    }
    lines.push_back("};");
    return text::join(lines, "\n");
}

Report analyse(Image& image, const call_graph::Graph& graph, const ConventionFacts& conventions) {
    const std::vector<Structure> found = recover(image, graph, conventions);
    Report r;
    Counts& c = r.counts;
    c.structures = static_cast<std::int64_t>(found.size());
    std::set<Va> touched;
    for (const Structure& s : found) {
        StructureFact f;
        f.name = s.name;
        f.size = s.size;
        f.exact_size = s.exact;
        for (const auto& [off, fld] : s.fields) {
            FieldFact ff;
            ff.offset = off;
            ff.size = fld.size;
            ff.type = field_type(fld);
            ff.reads = fld.reads;
            ff.writes = fld.writes;
            for (Note n : fld.notes) ff.notes.emplace_back(name_of(n));
            f.fields.push_back(std::move(ff));
            ++c.fields;
            c.pointer_fields += fld.notes.contains(Note::Pointer) ? 1 : 0;
            c.function_pointer_fields += fld.notes.contains(Note::Called) ? 1 : 0;
            c.array_fields += fld.notes.contains(Note::Array) ? 1 : 0;
        }
        for (std::size_t i = 0; i < s.roots.size() && i < 12; ++i) f.seen_as.push_back(describe(image, s.roots[i]));
        f.pointers_unified = static_cast<std::int64_t>(s.roots.size());
        for (std::size_t i = 0; i < s.functions.size() && i < 40; ++i) f.functions.push_back(image.label(s.functions[i]));
        f.passed_to.assign(s.passed_to.begin(), s.passed_to.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(s.passed_to.size(), 12)));
        r.facts.push_back(std::move(f));
        c.with_exact_size += s.exact ? 1 : 0;
        c.seen_through_this += std::any_of(s.roots.begin(), s.roots.end(), [](const Root& x) { return x.kind == Root::Kind::This; }) ? 1 : 0;
        c.seen_through_global_pointer += std::any_of(s.roots.begin(), s.roots.end(), [](const Root& x) { return x.kind == Root::Kind::Global; }) ? 1 : 0;
        c.unified_across_functions += s.functions.size() > 1 ? 1 : 0;
        c.largest_fields = std::max(c.largest_fields, static_cast<std::int64_t>(s.fields.size()));
        touched.insert(s.functions.begin(), s.functions.end());
    }
    c.functions_touching_a_structure = static_cast<std::int64_t>(touched.size());
    for (std::size_t i = 0; i < found.size() && i < 3; ++i) {
        const Structure& s = found[i];
        std::vector<std::string> seen;
        for (std::size_t k = 0; k < s.roots.size() && k < 3; ++k) seen.push_back(describe(image, s.roots[k]));
        r.text.push_back(std::format("{}: {} fields, {} bytes{}, used by {} functions, seen as {}", s.name, s.fields.size(), s.size,
                                     s.exact ? "" : " at least", s.functions.size(), text::join(seen, "; ")));
    }
    if (!found.empty()) {
        const std::string as_c = layout(found.front());
        const auto lines = text::split(as_c, "\n");   // (views into `as_c`)
        for (std::size_t i = 0; i < lines.size() && i < 24; ++i) r.text.emplace_back(lines[i]);
    }
    return r;
}

}  // namespace cb::analyze::structures

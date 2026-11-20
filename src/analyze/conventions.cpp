// cb-unity: alone  (it names helpers after the passes of the analysis; structures.cpp has a stack_delta of its own)
#include "analyze/conventions.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <unordered_map>

#include "core/crypto.hpp"
#include "core/intmath.hpp"
#include "core/text.hpp"

namespace cb::analyze::conventions {

namespace {

// The registers a 32-bit function may be passed values in.
constexpr std::string_view kInputRegisters[] = {"eax", "ecx", "edx", "ebx", "esi", "edi"};

// How far the entry sequence reaches, and how many paths and instructions the search for register inputs may follow.
constexpr std::size_t kEntryInstructions = 12;
constexpr int kInputSearchBudget = 90;
constexpr int kInputSearchBranchDepth = 3;
// How far before a return the result of the function is looked for.
constexpr int kResultLookBack = 12;
// How many instructions after a call the caller's use of the result is looked for.
constexpr std::size_t kUseLookAhead = 5;

// A register pushed in the first few instructions counts as an input when the callers load it at least this often.
constexpr std::int64_t kPushedInputSites = 2;

bool is_input(const std::string& reg) { return std::find(std::begin(kInputRegisters), std::end(kInputRegisters), reg) != std::end(kInputRegisters); }

std::string_view fam(const std::string& reg) { return family(reg); }

// The value of an operand's `size` bytes as a signed number.
std::int64_t sign_extended(std::int64_t value, int size) {
    const int bits = 8 * size;
    if (bits <= 0 || bits > 63) return value;
    return (value >> (bits - 1)) != 0 ? value - (std::int64_t{1} << bits) : value;
}

bool same_operand(const Operand& a, const Operand& b) {
    return a.kind == b.kind && a.reg == b.reg && a.base == b.base && a.index == b.index && a.seg == b.seg && a.value == b.value &&
           a.scale == b.scale && a.size == b.size;
}

bool is_register(const Operand& op) { return op.kind == Operand::Kind::Register; }
bool is_immediate(const Operand& op) { return op.kind == Operand::Kind::Immediate; }
bool is_memory(const Operand& op) { return op.kind == Operand::Kind::Memory; }

// add/sub esp, K
bool is_esp_immediate(const Instruction& ins) {
    return ins.operands.size() == 2 && is_register(ins.operands[0]) && fam(ins.operands[0].reg) == "esp" && is_immediate(ins.operands[1]);
}

bool starts(const std::string& s, std::string_view prefix) { return s.starts_with(prefix); }

struct Frame {
    bool framed = false;
    std::int64_t local = 0;
    std::vector<std::string> saved;
};

// The entry sequence: what it saves, whether it builds a frame, how many bytes of locals it makes room for.
Frame frame_of(const Function& fn) {
    Frame f;
    std::vector<std::string> saved;
    for (std::size_t k = 0; k < fn.insns.size() && k < kEntryInstructions; ++k) {
        const Instruction& ins = fn.insns[k];
        const std::string& mn = ins.mnemonic;
        const auto& ops = ins.operands;
        if (mn == "push" && !ops.empty() && is_register(ops[0])) {
            const std::string r(fam(ops[0].reg));
            if ((r == "ebx" || r == "esi" || r == "edi" || (r == "ebp" && !f.framed)) && std::find(saved.begin(), saved.end(), r) == saved.end()) {
                saved.push_back(r);
                continue;
            }
            if (r == "ds" || r == "es") continue;
            break;
        }
        if (mn == "mov" && ops.size() == 2 && is_register(ops[0]) && is_register(ops[1]) && fam(ops[0].reg) == "ebp" && fam(ops[1].reg) == "esp") {
            f.framed = true;
        } else if (mn == "enter" && !ops.empty()) {
            f.framed = true;
            f.local = ops[0].value;
        } else if ((mn == "sub" || mn == "add") && is_esp_immediate(ins)) {
            const std::int64_t n = sign_extended(ops[1].value, ops[1].size);
            f.local = std::max(f.local, mn == "sub" ? n : -n);
        } else if (!(mn == "inc" || mn == "nop" || mn == "pop" ||
                     (mn == "mov" && !ops.empty() && is_register(ops[0]) &&
                      (fam(ops[0].reg) == "eax" || fam(ops[0].reg) == "edi" || fam(ops[0].reg) == "ds" || fam(ops[0].reg) == "es")))) {
            break;   // past the entry sequence
        }
    }
    f.local = std::max<std::int64_t>(f.local, 0);
    for (const std::string& r : saved)
        if (r != "ebp") f.saved.push_back(r);
    return f;
}

// The argument slots the code reads, by their offset from the first argument, with the widest read of each. With a frame the
// slots are [ebp + n]; without one, [esp + n] followed through the entry code (what is pushed moves the offset).
std::map<std::int64_t, std::int64_t> argument_slots(const Function& fn, bool framed, std::int64_t first, std::int64_t word) {
    std::map<std::int64_t, std::int64_t> slots;
    const auto widest = [&](std::int64_t offset, std::int64_t size) {
        std::int64_t& slot = slots[offset];
        slot = std::max(slot, size);
    };
    if (framed) {
        for (const Instruction& ins : fn.insns)
            for (const Operand& op : ins.operands)
                if (is_memory(op) && !op.base.empty() && fam(op.base) == "ebp" && op.index.empty() && op.value >= first) widest(op.value - first, op.size);
        return slots;
    }
    std::int64_t depth = 0;   // no frame: follow the stack through the entry code
    for (const Instruction& ins : fn.insns) {
        if (ins.flow == Flow::Call || ins.flow == Flow::Jump || ins.flow == Flow::Return) break;
        for (const Operand& op : ins.operands)
            if (is_memory(op) && !op.base.empty() && fam(op.base) == "esp" && op.index.empty() && op.value >= depth + first - word)
                widest(op.value - depth - (first - word), op.size);
        const auto delta = stack_delta(ins);
        if (!delta) break;
        depth += *delta;
    }
    return slots;
}

// Does the instruction set its register to a constant (xor r,r; or r,-1; and r,0)? Then it reads nothing.
bool sets_constant(const Instruction& ins) {
    const auto& ops = ins.operands;
    if (ops.size() != 2 || !is_register(ops[0])) return false;
    if (ins.mnemonic == "xor" || ins.mnemonic == "sub" || ins.mnemonic == "sbb") return same_operand(ops[0], ops[1]);
    if (!is_immediate(ops[1])) return false;
    return (ins.mnemonic == "or" && sign_extended(ops[1].value, ops[1].size) == -1) || (ins.mnemonic == "and" && ops[1].value == 0);
}

std::unordered_map<Va, std::size_t> index_of(const Function& fn) {
    std::unordered_map<Va, std::size_t> index;
    for (std::size_t k = 0; k < fn.insns.size(); ++k) index[fn.insns[k].addr] = k;
    return index;
}

// The registers the function reads before writing them, along every path from its start (a bounded search: the budget
// counts instructions followed).
std::set<std::string> register_inputs(const Function& fn, const std::unordered_map<Va, std::size_t>& index) {
    std::set<std::string> found;
    std::set<std::pair<std::size_t, std::set<std::string>>> seen;
    struct Path {
        bool live;
        std::size_t at;
        std::set<std::string> written;
        int depth;
    };
    std::vector<Path> work{{true, 0, {}, 0}};
    int budget = kInputSearchBudget;
    while (!work.empty() && budget > 0) {
        Path path = std::move(work.back());
        work.pop_back();
        bool live = path.live;
        std::size_t i = path.at;
        std::set<std::string> written = std::move(path.written);
        while (live && i < fn.insns.size() && budget > 0) {
            auto key = std::make_pair(i, written);
            if (seen.contains(key)) break;
            seen.insert(std::move(key));
            --budget;
            const Instruction& ins = fn.insns[i];
            const auto& ops = ins.operands;
            const bool zeroing = sets_constant(ins);
            if ((ins.mnemonic != "push" && !zeroing) || (ins.mnemonic == "push" && !ops.empty() && is_memory(ops[0]))) {
                for (const std::string& r : ins.reads)
                    if (is_input(r) && !written.contains(r)) found.insert(r);
            }
            if (ins.flow == Flow::Call) {
                written.insert({"eax", "ecx", "edx"});
            } else if (ins.flow == Flow::Return) {
                break;
            }
            for (const std::string& r : ins.writes)
                if (is_input(r)) written.insert(r);
            if (ins.mnemonic == "pop" && !ops.empty() && is_register(ops[0])) written.insert(std::string(fam(ops[0].reg)));
            if (ins.flow == Flow::Jump) {
                live = false;
                if (ins.target) {
                    const auto target = index.find(*ins.target);
                    if (target != index.end()) {
                        live = true;
                        i = target->second;
                    }
                }
                continue;
            }
            if (ins.flow == Flow::CondJump && ins.target && path.depth < kInputSearchBranchDepth) {
                const auto target = index.find(*ins.target);
                if (target != index.end()) work.push_back({true, target->second, written, path.depth + 1});
            }
            const auto next = index.find(ins.addr + ins.size);
            live = next != index.end();
            if (live) i = next->second;
        }
    }
    return found;
}

// What a return leaves in eax, as far as the code before it says.
enum class Sets { Unknown, Call, Value, Constant };
struct ReturnKind {
    Sets sets = Sets::Unknown;
    std::int64_t value = 0;
    bool wide = false;   // edx is written too
    bool flt = false;    // a float instruction was the last thing to touch the stack top, and it left a value
};

// Every return of the function: the bytes it pops, and what it leaves.
void returns_of(const Function& fn, std::vector<std::int64_t>& pops, std::vector<ReturnKind>& kinds) {
    std::unordered_set<Va> labels;
    for (const Instruction& ins : fn.insns)
        if ((ins.flow == Flow::CondJump || ins.flow == Flow::Jump) && ins.target) labels.insert(*ins.target);
    for (int i = 0; i < static_cast<int>(fn.insns.size()); ++i) {
        const Instruction& ins = fn.insns[static_cast<std::size_t>(i)];
        if (ins.flow != Flow::Return) continue;
        pops.push_back(ins.operands.empty() ? 0 : ins.operands[0].value);
        ReturnKind k;
        int float_left = -1;   // not seen
        for (int j = i - 1; j > std::max(-1, i - kResultLookBack); --j) {
            const Instruction& before = fn.insns[static_cast<std::size_t>(j)];
            const std::string& mn = before.mnemonic;
            if (mn[0] == 'f' && float_left < 0 && mn != "fwait" && mn != "fnclex" && mn != "fldcw" && mn != "fnstcw" && mn != "fstcw")
                float_left = !(starts(mn, "fstp") || starts(mn, "fistp") || mn == "fcompp" || mn == "fucompp");
            if (before.writes.contains("edx") && before.flow != Flow::Call && mn != "pop" && mn != "cdq" && mn != "cwd") k.wide = true;
            if (before.flow == Flow::Call) {
                k.sets = Sets::Call;
                break;
            }
            if (before.writes.contains("eax") && mn != "pop") {
                const auto& ops = before.operands;
                if ((mn == "xor" || mn == "sub") && ops.size() == 2 && same_operand(ops[0], ops[1])) {
                    k.sets = Sets::Constant;
                    k.value = 0;
                } else if (mn == "mov" && ops.size() > 1 && is_immediate(ops[1])) {
                    k.sets = Sets::Constant;
                    k.value = ops[1].value;
                } else if (mn == "or" && ops.size() > 1 && is_immediate(ops[1]) && sign_extended(ops[1].value, ops[1].size) == -1) {
                    k.sets = Sets::Constant;
                    k.value = -1;
                } else {
                    k.sets = Sets::Value;
                }
                break;
            }
            if (before.flow == Flow::Jump || before.flow == Flow::Return || (labels.contains(before.addr) && j < i - 1)) break;
        }
        k.flt = float_left > 0;
        kinds.push_back(k);
    }
}

// Where a jump or call goes: its direct target, or the import its `[address]` operand names.
std::optional<Va> destination(const Image& image, const Instruction& ins) {
    if (ins.target) return ins.target;
    if (!ins.operands.empty() && is_memory(ins.operands[0]) && ins.operands[0].base.empty() && ins.operands[0].index.empty() &&
        image.is_import(ins.operands[0].value))
        return ins.operands[0].value;
    return std::nullopt;
}

bool ends_with_icase(std::string_view s, std::string_view suffix) { return text::iends_with(s, suffix); }

bool contains_icase(std::string_view s, std::string_view part) { return text::lower(s).find(text::lower(part)) != std::string::npos; }

// The name of a window, dialog, hook or enumeration procedure, by the way the SDK and its users spell one.
std::optional<std::string_view> procedure_by_name(const std::string& name) {
    if (name.empty() || starts(name, "sub_")) return std::nullopt;
    for (std::string_view tail : {"WNDPROC", "WINDOWPROC", "MDICHILDPROC", "FRAMEPROC", "SUBCLASSPROC"})
        if (ends_with_icase(name, tail)) return "window procedure";
    if (contains_icase(name, "WNDPROC")) return "window procedure";
    for (std::string_view tail : {"DLG", "DIALOG", "DLGPROC", "DIALOGPROC", "DLGFN", "DIALOGFN"})
        if (ends_with_icase(name, tail)) return "dialog procedure";
    if (contains_icase(name, "DLGPROC")) return "dialog procedure";
    for (std::string_view tail : {"HOOK", "HOOKPROC"})
        if (ends_with_icase(name, tail)) return "hook procedure";
    // ENUM, then letters, digits and underscores, ending in PROC
    if (ends_with_icase(name, "PROC")) {
        const std::string upper = text::upper(name);
        for (std::size_t p = upper.find("ENUM"); p != std::string::npos && p + 8 <= upper.size(); p = upper.find("ENUM", p + 1)) {
            const bool word = std::all_of(upper.begin() + static_cast<std::ptrdiff_t>(p), upper.end(),
                                          [](char c) { return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; });
            if (word) return "enumeration procedure";
        }
    }
    return std::nullopt;
}

// A function that calls what a callback calls, and removes what a callback removes, is one.
std::optional<std::string_view> procedure_by_calls(const std::vector<std::string>& apis, std::optional<std::int64_t> pop) {
    static const std::map<std::string, std::string_view> kCalls = {
        {"DEFWINDOWPROC", "window procedure"}, {"DEFMDICHILDPROC", "window procedure"}, {"DEFFRAMEPROC", "window procedure"},
        {"CALLWINDOWPROC", "window procedure"}, {"DEFDLGPROC", "dialog procedure"},      {"ENDDIALOG", "dialog procedure"},
        {"CALLNEXTHOOKEX", "hook procedure"},   {"DEFHOOKPROC", "hook procedure"}};
    for (const std::string& api : apis) {
        const std::size_t bang = api.rfind('!');
        if (bang == std::string::npos) continue;
        std::string key = text::upper(api.substr(bang + 1));
        while (!key.empty() && (key.back() == 'A' || key.back() == 'W')) key.pop_back();
        const auto it = kCalls.find(key);
        if (it == kCalls.end()) continue;
        const std::string_view kind = it->second;
        if (pop && (*pop == 10 || *pop == 16 || (kind == "hook procedure" && (*pop == 8 || *pop == 12)))) return kind;
    }
    return std::nullopt;
}

// The imports that take a va_list or a format and a list of arguments: a function that calls one and leaves its arguments
// to the caller is probably variadic.
bool takes_va_list(const std::string& api) {
    static const std::set<std::string> kNames = [] {
        std::set<std::string> names;
        for (std::string w1 : {"", "w"})   // [w]v[s][n][w]printf
            for (std::string s : {"", "s"})
                for (std::string n : {"", "n"})
                    for (std::string w2 : {"", "w"}) names.insert(w1 + "v" + s + n + w2 + "printf");
        for (std::string end : {"", "a", "w"}) names.insert("wvsprintf" + end);
        for (std::string n : {"", "n"})   // _vs[n][w]printf
            for (std::string w : {"", "w"}) names.insert("_vs" + n + w + "printf");
        for (std::string w : {"", "w"}) names.insert("vf" + w + "printf");
        for (std::string u : {"", "_"}) names.insert(u + "vsprintf");
        for (std::string end : {"a", "w"}) names.insert("formatmessage" + end);
        return names;
    }();
    const std::string lower = text::lower(api);
    if (kNames.contains(lower)) return true;
    for (std::size_t bang = lower.find('!'); bang != std::string::npos; bang = lower.find('!', bang + 1))
        if (kNames.contains(lower.substr(bang + 1))) return true;
    return false;
}

// The kind of inline string operation a `rep` instruction is.
std::optional<std::string_view> string_operation(const std::string& mn) {
    if (!starts(mn, "rep") || mn.size() < 5) return std::nullopt;
    const std::string_view kind = std::string_view(mn).substr(mn.size() - 5, 4);   // rep movsb -> "movs"
    if (kind == "movs") return "copy";
    if (kind == "stos") return "fill";
    if (kind == "scas") return "scan";
    if (kind == "cmps") return "compare";
    if (kind == "lods") return "load";
    return std::nullopt;
}

// How callers load ecx and edx before calling a function: the evidence that it takes them.
struct Loads {
    std::int64_t sites = 0, ecx = 0, edx = 0;
};

// The 16-bit entry sequence: what the first bytes say about how the loader and the stack walker treat the function.
std::vector<std::string> entry_notes(Image& image, Va start) {
    Bytes head_bytes = image.read(start, 8);
    const Bytes twelve = image.read(start, 12);
    std::vector<std::string> notes;
    const auto has_prefix = [](const Bytes& b, std::initializer_list<std::uint8_t> prefix) {
        return b.size() >= prefix.size() && std::equal(prefix.begin(), prefix.end(), b.begin());
    };
    if (has_prefix(head_bytes, {0x1E, 0x58, 0x90}) || has_prefix(head_bytes, {0x8C, 0xD8, 0x90})) {   // push ds; pop ax; nop  /  mov ax,ds; nop
        notes.emplace_back("exported entry (the loader patches in the data segment)");
        head_bytes.erase(head_bytes.begin(), head_bytes.begin() + std::min<std::size_t>(3, head_bytes.size()));
    }
    if (!head_bytes.empty() && head_bytes[0] == 0xB8) {   // mov ax,imm16 ... mov ds,ax
        const std::uint8_t mov_ds_ax[] = {0x8E, 0xD8};
        if (std::search(twelve.begin(), twelve.end(), std::begin(mov_ds_ax), std::end(mov_ds_ax)) != twelve.end())
            notes.emplace_back("loads its own data segment");
    }
    if (has_prefix(head_bytes, {0x45, 0x55, 0x8B, 0xEC})) notes.emplace_back("far frame marked for the stack walker");   // inc bp; push bp; mov bp,sp
    return notes;
}

}  // namespace

// ---- names -----------------------------------------------------------------------------------------------------------------

std::string_view name_of(Shape shape) {
    switch (shape) {
        case Shape::ImportThunk: return "import thunk";
        case Shape::JumpThunk: return "jump thunk";
        case Shape::Empty: return "empty";
        case Shape::Constant: return "constant";
        case Shape::Leaf: return "leaf";
        case Shape::Calls: return "calls";
        case Shape::NotCode: return "not code";
    }
    return "";
}

std::string_view name_of(ResultKind kind) {
    switch (kind) {
        case ResultKind::None: return "none";
        case ResultKind::Value: return "value";
        case ResultKind::Flag: return "flag";
        case ResultKind::Float: return "float";
        case ResultKind::Wide: return "wide";
    }
    return "";
}

// ---- the pass --------------------------------------------------------------------------------------------------------------------

std::optional<std::int64_t> stack_delta(const Instruction& ins) {
    const std::string& mn = ins.mnemonic;
    const auto& ops = ins.operands;
    if (mn == "push") return ops.empty() ? 4 : ops[0].size;
    if (mn == "pop") return -(ops.empty() ? 4 : ops[0].size);
    if ((mn == "add" || mn == "sub") && is_esp_immediate(ins)) {
        const std::int64_t n = sign_extended(ops[1].value, ops[1].size);
        return mn == "sub" ? n : -n;
    }
    if (!ins.writes.contains("esp")) return 0;
    return std::nullopt;
}

Facts functions(Image& image, const call_graph::Graph& graph) {
    const std::int64_t word = image.bits() == 32 ? 4 : 2;
    // what callers do with the result, what they remove after the call, what they load before it
    std::unordered_map<Va, std::pair<std::int64_t, std::int64_t>> uses;   // callee -> (callers that use the result, callers that ignore it)
    std::unordered_map<Va, std::map<std::int64_t, std::int64_t>> cleanup; // callee -> bytes removed after a call -> how many callers
    std::unordered_map<Va, Loads> loads;
    std::unordered_map<Va, std::unordered_map<Va, std::size_t>> indexes;
    for (const Va f : image.functions()) {
        const Function& fn = image.function(f);
        auto& index = indexes[f] = index_of(fn);
        const auto calls = graph.calls.find(f);
        if (calls == graph.calls.end()) continue;
        for (const call_graph::Call& call : calls->second) {
            if (call.kind != call_graph::CallKind::Call || !call.destination) continue;
            const Va callee = *call.destination;
            const std::size_t i = index.at(call.site);
            auto& tally = uses[callee];
            for (std::size_t k = i + 1; k < fn.insns.size() && k < i + kUseLookAhead; ++k) {
                const Instruction& after = fn.insns[k];
                if (after.reads.contains("eax") && !sets_constant(after) && after.flow != Flow::Return) {
                    ++tally.first;
                    break;
                }
                if (after.writes.contains("eax") || after.flow == Flow::Call) {
                    ++tally.second;
                    break;
                }
                if (after.flow != Flow::None) break;
            }
            std::size_t j = i + 1;
            std::int64_t removed = 0;
            while (j < fn.insns.size()) {
                const Instruction& after = fn.insns[j];
                if (after.mnemonic == "add" && is_esp_immediate(after)) {
                    removed += sign_extended(after.operands[1].value, after.operands[1].size);
                } else if (after.mnemonic == "pop" && !after.operands.empty() && is_register(after.operands[0]) &&
                           (fam(after.operands[0].reg) == "ecx" || fam(after.operands[0].reg) == "ebx") && removed < 2 * word) {
                    removed += word;
                    ++j;
                    continue;
                }
                break;
            }
            ++cleanup[callee][removed];
            if (image.bits() == 32) {
                bool ecx = false, edx = false;
                for (std::size_t k = i; k > (i >= 3 ? i - 3 : 0);) {
                    --k;
                    const Instruction& before = fn.insns[k];
                    if (before.flow != Flow::None) break;
                    if (before.writes.contains("ecx")) ecx = true;
                    if (before.writes.contains("edx")) edx = true;
                }
                Loads& l = loads[callee];
                ++l.sites;
                l.ecx += ecx;
                l.edx += edx;
            }
        }
    }

    Facts facts;
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        Fact fact;
        fact.addr = fn.start;
        fact.name = image.function_name(fn.start);
        if (fact.name.empty()) fact.name = std::format("{:x}", fn.start);
        if (fn.insns.empty()) {
            fact.shape = Shape::NotCode;
            facts.emplace(start, std::move(fact));
            continue;
        }
        const auto& index = indexes[start];
        std::vector<std::int64_t> pops;
        std::vector<ReturnKind> returns;
        returns_of(fn, pops, returns);
        bool far = false;
        if (image.bits() == 16)
            for (const Instruction& ins : fn.insns)
                if (ins.mnemonic == "retf" || ins.mnemonic == "lret") far = true;
        const Frame frame = frame_of(fn);
        const std::int64_t first = image.bits() == 32 ? 2 * word : (far ? 6 : 4);
        fact.arg_slots = argument_slots(fn, frame.framed, first, word);
        std::set<std::string> registers = register_inputs(fn, index);
        std::vector<const Instruction*> real;   // the instructions that are not padding
        for (const Instruction& ins : fn.insns)
            if (ins.mnemonic != "nop" && ins.mnemonic != "int3") real.push_back(&ins);
        {   // a register pushed at once is a register passed in, when the callers load it
            const auto load = loads.find(fn.start);
            for (std::size_t k = 0; k < real.size() && k < 4; ++k) {
                const Instruction& ins = *real[k];
                if (ins.mnemonic == "push" && !ins.operands.empty() && is_immediate(ins.operands[0])) continue;
                if (ins.mnemonic != "push" || ins.operands.empty() || !is_register(ins.operands[0])) break;
                const std::string r(fam(ins.operands[0].reg));
                if ((r == "ecx" || r == "edx") && load != loads.end() && load->second.sites >= kPushedInputSites &&
                    (r == "ecx" ? load->second.ecx : load->second.edx) * 10 >= load->second.sites * 9)
                    registers.insert(r);
            }
        }
        for (const std::string& s : frame.saved) registers.erase(s);
        registers.erase("ebp");
        registers.erase("esp");
        fact.reg_args.assign(registers.begin(), registers.end());

        const auto calls = graph.calls.find(fn.start);
        static const std::vector<call_graph::Call> kNoCalls;
        const std::vector<call_graph::Call>& call_list = calls == graph.calls.end() ? kNoCalls : calls->second;
        std::set<std::string> api_set;
        for (const call_graph::Call& c : call_list)
            if (c.destination && image.is_import(*c.destination)) api_set.insert(image.import_name(*c.destination));
        const std::vector<std::string> apis(api_set.begin(), api_set.end());

        // what it is
        const auto only_destination = real.size() == 1 && real[0]->flow == Flow::Jump ? destination(image, *real[0]) : std::nullopt;
        if (only_destination && image.is_import(*only_destination)) {
            fact.shape = Shape::ImportThunk;
            fact.thunk_of = image.import_name(*only_destination);
        } else if (real.size() == 1 && real[0]->flow == Flow::Jump && real[0]->target && !image.function_name(*real[0]->target).empty()) {
            fact.shape = Shape::JumpThunk;
            fact.thunk_of = image.label(*real[0]->target);
        } else if (real.size() == 1 && real[0]->flow == Flow::Return) {
            fact.shape = Shape::Empty;
        } else if (real.empty()) {
            fact.shape = Shape::Empty;
        } else if (real.size() == 2 && real.back()->flow == Flow::Return && !returns.empty() && returns[0].sets == Sets::Constant &&
                   real[0]->flow == Flow::None) {
            fact.shape = Shape::Constant;
            fact.constant = returns[0].value;
        } else {
            const bool any = std::any_of(call_list.begin(), call_list.end(), [](const call_graph::Call& c) {
                return c.kind == call_graph::CallKind::Call || c.kind == call_graph::CallKind::Import || c.kind == call_graph::CallKind::Indirect;
            });
            fact.shape = any ? Shape::Calls : Shape::Leaf;
        }
        std::set<std::string> tails;
        for (const call_graph::Call& c : call_list)
            if (c.kind == call_graph::CallKind::Tail && c.destination) tails.insert(image.label(*c.destination));
        fact.tail_calls.assign(tails.begin(), tails.end());

        // the convention and the arguments
        std::optional<std::int64_t> pop;
        if (!pops.empty()) {   // the most common: ties go to the smaller
            std::map<std::int64_t, std::int64_t> counts;
            for (std::int64_t p : pops) ++counts[p];
            std::int64_t best_count = -1;
            for (const auto& [value, count] : counts)
                if (count > best_count) {
                    best_count = count;
                    pop = value;
                }
        }
        fact.removes = pop;
        const bool has_ecx = registers.contains("ecx"), has_edx = registers.contains("edx");
        if (image.bits() == 32) {
            const bool others = std::any_of(fact.reg_args.begin(), fact.reg_args.end(), [](const std::string& r) { return r != "ecx" && r != "edx"; });
            if (!pop) {
                // no return seen: no convention
            } else if (has_ecx && has_edx) {
                fact.convention = "fastcall";
            } else if (has_ecx && !others) {
                fact.convention = "thiscall";
            } else if (!fact.reg_args.empty()) {
                fact.convention = "register";
            } else {
                fact.convention = *pop != 0 ? "stdcall" : "cdecl";
            }
        } else if (pop) {
            fact.convention = std::string(*pop != 0 ? "pascal" : "cdecl") + (far ? " far" : " near");
        }
        const auto removal = cleanup.find(fn.start);
        static const std::map<std::int64_t, std::int64_t> kNoRemoval;
        const std::map<std::int64_t, std::int64_t>& removals = removal == cleanup.end() ? kNoRemoval : removal->second;
        if (pop && *pop != 0) {
            fact.args = floor_div<std::int64_t>(*pop, word);
            fact.arg_source = "return";
        } else if (pop) {   // the callers clean up: what they remove says how many
            std::int64_t best_bytes = 0, best_count = -1;
            for (const auto& [bytes, count] : removals)
                if (bytes > 0 && (count > best_count || (count == best_count && bytes < best_bytes))) {
                    best_count = count;
                    best_bytes = bytes;
                }
            if (best_count >= 0) {
                fact.args = floor_div<std::int64_t>(best_bytes, word);
                fact.arg_source = "callers";
            } else if (!fact.arg_slots.empty()) {
                const auto& [offset, width] = *fact.arg_slots.rbegin();
                fact.args = floor_div<std::int64_t>(offset + std::max(width, word), word);
                fact.arg_source = "frame";
            } else {
                const auto none = removals.find(0);
                const std::int64_t zero = none == removals.end() ? 0 : none->second;
                if (zero >= 2 || fact.shape == Shape::Empty || fact.shape == Shape::Constant) {
                    fact.args = 0;
                    fact.arg_source = removals.empty() ? "shape" : "callers";
                }
            }
        }
        fact.frame = frame.framed;
        fact.locals = frame.local;
        fact.saved = frame.saved;

        // the result
        const auto use = uses.find(fn.start);
        fact.callers_use = use == uses.end() ? 0 : use->second.first;
        fact.callers_ignore = use == uses.end() ? 0 : use->second.second;
        std::size_t constants = 0, sets = 0, explicit_values = 0;
        bool only_flags = true, any_float = false, any_wide = false;
        for (const ReturnKind& k : returns) {
            if (k.sets == Sets::Constant) {
                ++constants;
                if (k.value != 0 && k.value != 1) only_flags = false;
            }
            if (k.sets != Sets::Unknown) ++sets;
            if (k.sets != Sets::Unknown && k.sets != Sets::Call) ++explicit_values;
            any_float = any_float || k.flt;
            any_wide = any_wide || k.wide;
        }
        const bool wide_result = image.bits() == 16 && any_wide;
        if (any_float && explicit_values == 0) fact.result = ResultKind::Float;
        else if (fact.callers_use != 0) fact.result = wide_result ? ResultKind::Wide : ResultKind::Value;
        else if (fact.callers_ignore >= 2 && explicit_values == 0) fact.result = ResultKind::None;
        else if (!returns.empty() && constants == returns.size() && only_flags) fact.result = ResultKind::Flag;
        else if (sets != 0 && fact.callers_ignore == 0) fact.result = wide_result ? ResultKind::Wide : ResultKind::Value;

        // what the body does
        std::set<std::string> string_ops;
        for (const Instruction& ins : fn.insns) {
            if ((ins.flow == Flow::CondJump || ins.flow == Flow::Jump) && ins.target && fn.start <= *ins.target && *ins.target <= ins.addr) ++fact.loops;
            if (const auto op = string_operation(ins.mnemonic)) string_ops.emplace(*op);
            if (ins.mnemonic[0] == 'f' && ins.mnemonic != "fwait") ++fact.float_instructions;
        }
        fact.string_ops.assign(string_ops.begin(), string_ops.end());
        const auto ebp_slot = [&](const Operand& op) { return is_memory(op) && !op.base.empty() && fam(op.base) == "ebp" && op.value >= first; };
        if (fact.float_instructions != 0 && frame.framed) {
            std::set<std::int64_t> float_slots;
            for (const Instruction& ins : fn.insns) {
                const std::string& mn = ins.mnemonic;
                if (!(mn == "fld" || mn == "fadd" || mn == "fsub" || mn == "fmul" || mn == "fdiv" || mn == "fcomp" || mn == "fcom")) continue;
                for (const Operand& op : ins.operands)
                    if (ebp_slot(op) && (op.size == 4 || op.size == 8)) float_slots.insert(op.value - first);
            }
            fact.float_args.assign(float_slots.begin(), float_slots.end());
        }

        // a function that takes a variable argument list: it takes the address of its last named argument, or its callers
        // remove different amounts, or it hands its arguments to a formatting API
        bool takes_slot = false;
        if (frame.framed)
            for (const Instruction& ins : fn.insns)
                if (ins.operands.size() == 2 && ins.mnemonic == "lea" && ebp_slot(ins.operands[1])) takes_slot = true;
        std::vector<std::int64_t> removed;
        if (pop && *pop == 0)
            for (const auto& [bytes, count] : removals)
                if (bytes > 0) removed.push_back(bytes);
        std::optional<std::int64_t> listed;   // a slot whose address is taken right after the slot before it is read
        if (frame.framed && pop && *pop == 0) {
            std::set<std::int64_t> direct, taken;
            for (const Instruction& ins : fn.insns) {
                if (ins.mnemonic != "lea")
                    for (const Operand& op : ins.operands)
                        if (ebp_slot(op) && op.index.empty()) direct.insert(op.value - first);
                if (ins.mnemonic == "lea" && ins.operands.size() == 2 && ebp_slot(ins.operands[1]) && ins.operands[1].index.empty())
                    taken.insert(ins.operands[1].value - first);
            }
            for (const std::int64_t at : taken) {
                const bool beyond = std::any_of(direct.begin(), direct.end(), [&](std::int64_t d) { return d >= at; });
                if (at >= word && direct.contains(at - word) && !beyond) listed = at;
            }
        }
        const bool va_api = std::any_of(apis.begin(), apis.end(), takes_va_list);
        if ((takes_slot && (va_api || removed.size() >= 2)) || removed.size() >= 3 || (listed && fact.shape == Shape::Calls)) {
            fact.variadic = true;
            if (listed) {
                fact.args = floor_div<std::int64_t>(*listed, word);
                fact.arg_source = "frame (named arguments)";
            } else if (!removed.empty()) {
                const std::int64_t smallest = floor_div<std::int64_t>(removed[0], word);
                const std::int64_t known = fact.args && *fact.args != 0 ? *fact.args : 99;
                fact.args = std::min(smallest, known);
                fact.arg_source = "callers (at least)";
            }
        }
        if (image.bits() == 16) fact.entry = entry_notes(image, fn.start);
        if (const auto by_name = procedure_by_name(fact.name)) fact.procedure = std::string(*by_name);
        else if (const auto by_calls = procedure_by_calls(apis, pop)) fact.procedure = std::string(*by_calls);
        facts.emplace(start, std::move(fact));
    }
    return facts;
}

ConventionFacts summary(const Facts& facts) {
    ConventionFacts out;
    for (const auto& [start, fact] : facts) {
        ConventionFact c;
        c.convention = fact.convention.value_or(std::string());
        c.procedure = fact.procedure;
        if (fact.removes) c.removes = static_cast<int>(*fact.removes);
        c.frame = fact.frame;
        c.reg_args = fact.reg_args;
        out.emplace(start, std::move(c));
    }
    return out;
}

// ---- as a document -------------------------------------------------------------------------------------------------------------

knowledge::Value to_value(const Fact& f) {
    using knowledge::Value;
    const auto text_list = [](const std::vector<std::string>& items) {
        Value::List list;
        for (const std::string& s : items) list.emplace_back(s);
        return Value(std::move(list));
    };
    Value::Map m;
    m.insert_or_assign("addr", Value(static_cast<std::int64_t>(f.addr)));
    m.insert_or_assign("kind", Value("convention"));
    m.insert_or_assign("name", Value(f.name));
    if (f.shape == Shape::NotCode) {   // nothing else is known of what is not code
        m.insert_or_assign("shape", Value(std::string(name_of(f.shape))));
        m.insert_or_assign("convention", Value());
        m.insert_or_assign("args", Value());
        m.insert_or_assign("arg_source", Value());
        return Value(std::move(m));
    }
    if (f.thunk_of) m.insert_or_assign("thunk_of", Value(*f.thunk_of));   // (what the shape rests on is said before the shape)
    if (f.constant) m.insert_or_assign("constant", Value(*f.constant));
    m.insert_or_assign("shape", Value(std::string(name_of(f.shape))));
    if (!f.tail_calls.empty()) m.insert_or_assign("tail_calls", text_list(f.tail_calls));
    m.insert_or_assign("convention", f.convention ? Value(*f.convention) : Value());
    m.insert_or_assign("reg_args", text_list(f.reg_args));
    m.insert_or_assign("removes", f.removes ? Value(*f.removes) : Value());
    m.insert_or_assign("args", f.args ? Value(*f.args) : Value());
    m.insert_or_assign("arg_source", f.arg_source ? Value(*f.arg_source) : Value());
    if (!f.arg_slots.empty()) {
        Value::Map slots;
        for (const auto& [offset, width] : f.arg_slots) slots.insert_or_assign("+" + std::to_string(offset), Value(width));
        m.insert_or_assign("arg_slots", Value(std::move(slots)));
    }
    m.insert_or_assign("frame", Value(f.frame));
    m.insert_or_assign("locals", Value(f.locals));
    m.insert_or_assign("saved", text_list(f.saved));
    m.insert_or_assign("result", f.result ? Value(std::string(name_of(*f.result))) : Value());
    m.insert_or_assign("result_evidence", Value(Value::Map{{"callers_use", Value(f.callers_use)}, {"callers_ignore", Value(f.callers_ignore)}}));
    if (f.loops != 0) m.insert_or_assign("loops", Value(f.loops));
    if (!f.string_ops.empty()) m.insert_or_assign("string_ops", text_list(f.string_ops));
    if (f.float_instructions != 0) {
        m.insert_or_assign("float_instructions", Value(f.float_instructions));
        if (!f.float_args.empty()) {
            Value::List slots;
            for (std::int64_t s : f.float_args) slots.emplace_back(s);
            m.insert_or_assign("float_args", Value(std::move(slots)));
        }
    }
    if (f.variadic) m.insert_or_assign("variadic", Value(true));
    if (!f.entry.empty()) m.insert_or_assign("entry", text_list(f.entry));
    if (f.procedure) m.insert_or_assign("procedure", Value(*f.procedure));
    return Value(std::move(m));
}

// ---- duplicates and the bridge layer -------------------------------------------------------------------------------------------------

std::string signature(Image& image, const Function& fn) {
    std::string text;
    for (const Instruction& ins : fn.insns) {
        if (!text.empty()) text += '\n';
        if (ins.target && ins.flow != Flow::None) {
            const std::string where = fn.start <= *ins.target && *ins.target < fn.limit ? std::format("+{}", *ins.target - fn.start) : image.label(*ins.target);
            text += ins.mnemonic + " " + where;
        } else {
            text += ins.text;
        }
    }
    return crypto::to_hex(crypto::sha1(as_bytes(text)));
}

std::vector<std::vector<Va>> duplicates(Image& image, std::size_t least) {
    std::vector<std::string> order;   // signatures, in the order first met
    std::unordered_map<std::string, std::vector<Va>> groups;
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        if (fn.insns.size() < least) continue;
        auto [it, fresh] = groups.try_emplace(signature(image, fn));
        if (fresh) order.push_back(it->first);
        it->second.push_back(start);
    }
    std::vector<std::vector<Va>> out;
    for (const std::string& sig : order)
        if (groups[sig].size() > 1) out.push_back(std::move(groups[sig]));
    std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.front() < b.front(); });
    return out;
}

std::vector<std::string> thunk_layer(const Image& image) {
    // The APIs of the 16/32-bit bridge (the thunk compiler's runtime), upper case.
    static const std::set<std::string> kThunkApis = {
        "THUNKCONNECT32", "THUNKCONNECT16", "C16THKSL01", "C32THKSL01", "THUNKINITLS", "QT_THUNK", "FT_THUNK", "FT_PROLOG",
        "FT_EXIT", "SSONBIGSTACK", "SSCALL", "MAPLS", "UNMAPLS", "MAPSL", "GETVDMPOINTER32W", "ALLOCSLCALLBACK",
        "GETPROCADDRESS32W", "K32THK1632PROLOG", "FT_EXIT0"};
    std::set<std::string> names;
    for (const auto& [va, name] : image.imports()) {
        const std::size_t bang = name.rfind('!');
        const std::string api = text::upper(bang == std::string::npos ? name : name.substr(bang + 1));
        if (kThunkApis.contains(api)) names.insert(api);
    }
    return std::vector<std::string>(names.begin(), names.end());
}

// ---- the report ----------------------------------------------------------------------------------------------------------------------

Report analyse(Image& image, const call_graph::Graph& graph) {
    Report report;
    report.facts = functions(image, graph);
    const auto groups = duplicates(image);
    report.bridge_imports = thunk_layer(image);
    Counts& c = report.counts;
    c.functions = static_cast<std::int64_t>(report.facts.size());
    std::vector<std::pair<std::string, std::int64_t>> by_convention;   // in the order first met
    std::vector<const Fact*> procedures;
    for (const auto& [start, f] : report.facts) {
        const std::string convention = f.convention && !f.convention->empty() ? *f.convention : "unknown";
        auto kind = std::find_if(by_convention.begin(), by_convention.end(), [&](const auto& p) { return p.first == convention; });
        if (kind == by_convention.end()) by_convention.emplace_back(convention, 1);
        else ++kind->second;
        if (f.convention && !f.convention->empty()) ++c.convention_known;
        if (f.removes && *f.removes != 0) ++c.callee_removes_arguments;
        if (f.removes && *f.removes == 0) ++c.caller_removes_arguments;
        if (f.args) ++c.argument_count_known;
        if (f.arg_source == "return") ++c.argument_count_from_return;
        if (f.arg_source == "callers") ++c.argument_count_from_callers;
        if (f.arg_source == "frame") ++c.argument_count_from_frame;
        if (!f.reg_args.empty()) ++c.register_arguments;
        if (f.convention == "thiscall") ++c.this_in_ecx;
        if (f.result) ++c.result_known;
        if (f.result == ResultKind::None) ++c.result_none;
        if (f.shape == Shape::ImportThunk) ++c.import_thunks;
        if (f.shape == Shape::JumpThunk) ++c.jump_thunks;
        if (!f.tail_calls.empty()) ++c.tail_calling;
        if (f.shape == Shape::Empty || f.shape == Shape::Constant) ++c.empty_or_constant;
        if (f.shape == Shape::Leaf) ++c.leaf;
        if (f.loops != 0) ++c.with_loops;
        if (!f.string_ops.empty()) ++c.inline_string_operations;
        if (f.float_instructions != 0) ++c.floating_point;
        if (f.variadic) ++c.variadic;
        if (f.procedure) {
            ++c.system_called_procedures;
            procedures.push_back(&f);
        }
    }
    c.duplicate_groups = static_cast<std::int64_t>(groups.size());
    c.bridge_imports = static_cast<std::int64_t>(report.bridge_imports.size());
    std::size_t largest = 0;
    for (const auto& g : groups) {
        c.duplicate_functions += static_cast<std::int64_t>(g.size());
        largest = std::max(largest, g.size());
        report.duplicates.push_back({g.front(), {}});
        for (const Va f : g) report.duplicates.back().functions.push_back(image.label(f));
    }

    std::stable_sort(by_convention.begin(), by_convention.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    std::vector<std::string> parts;
    for (const auto& [convention, count] : by_convention) parts.push_back(std::format("{} {}", convention, count));
    report.text.push_back("conventions: " + text::join(parts, ", "));
    if (!procedures.empty()) {
        std::vector<std::string> shown;
        for (std::size_t i = 0; i < procedures.size() && i < 12; ++i) shown.push_back(std::format("{} ({})", procedures[i]->name, *procedures[i]->procedure));
        report.text.push_back("system-called procedures: " + text::join(shown, ", "));
    }
    if (!report.bridge_imports.empty()) report.text.push_back("16/32-bit bridge imports: " + text::join(report.bridge_imports, ", "));
    if (!groups.empty()) report.text.push_back(std::format("identical functions: {} groups, largest {} copies", groups.size(), largest));
    return report;
}

std::vector<facts::Row> fact_rows(const Report& report, ImageKind kind) {
    using knowledge::Value;
    std::vector<facts::Row> rows;
    for (const auto& [start, f] : report.facts) rows.push_back({facts::key(kind, f.addr), "convention", to_value(f)});
    for (const DuplicatesFact& d : report.duplicates) {
        Value::List names;
        for (const std::string& n : d.functions) names.emplace_back(n);
        rows.push_back({facts::key(kind, d.addr), "duplicates",
                        Value(Value::Map{{"addr", Value(static_cast<std::int64_t>(d.addr))}, {"kind", Value("duplicates")}, {"functions", Value(std::move(names))}})});
    }
    if (!report.bridge_imports.empty()) {
        Value::List names;
        for (const std::string& n : report.bridge_imports) names.emplace_back(n);
        rows.push_back({"", "thunk_layer", Value(Value::Map{{"addr", Value()}, {"kind", Value("thunk_layer")}, {"imports", Value(std::move(names))}})});
    }
    return rows;
}

}  // namespace cb::analyze::conventions

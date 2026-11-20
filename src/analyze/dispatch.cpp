// analyze/dispatch.cpp -- see dispatch.hpp.
#include "analyze/dispatch.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "analyze/call_graph.hpp"
#include "analyze/globals.hpp"
#include "analyze/strings.hpp"
#include "core/text.hpp"
#include "knowledge/package.hpp"
#include "knowledge/reads.hpp"

namespace cb::analyze::dispatch {

namespace {

// The names of the window messages, the driver control messages and the DOS, BIOS and DPMI services are knowledge
// (config/knowledge/{windows/messages,vxd/messages,dos/services}.json), read through knowledge::standard().

// The handful of messages every window handles.
constexpr Va kCoreMessages[] = {0x000F, 0x0010, 0x0081, 0x0082, 0x0110, 0x0111, 0x0112};

// The entries of a description file that are named by a number: {number -> the text of `field`}. Three ways an entry holds
// its number: in a register of its `select` (the function of an interrupt: AH of int 21h, AX of int 31h, a driver's EAX; for
// an interrupt `vector` names which), as its `vector` alone (a BIOS interrupt: the entries with no `select`), or as its
// `value` (a constant). Every ask is recorded as a read of the domain (knowledge/reads.hpp), so a fact made from a name is
// stale when the file changes.
enum class By { Register, Vector, Value };

struct Named {
    const char* platform;
    const char* domain;
    By by;
    const char* register_name;   // By::Register: the `select` key
    const char* vector;          // By::Register: only the entries of this vector ("" any); By::Vector: unused
    const char* field;           // the text to give
};

class NameTable {
public:
    explicit NameTable(const Named& what) {
        using KnownValue = knowledge::Value;
        for (const auto& [id, entry] : knowledge::standard().entries(what.platform, what.domain)) {
            const KnownValue* field = entry.get(what.field);
            const std::string* text = field != nullptr ? field->text() : nullptr;
            if (text == nullptr) continue;
            const KnownValue* vec = entry.get("vector");
            const KnownValue* sel = entry.get("select");
            const std::string* vector = vec != nullptr ? vec->text() : nullptr;
            const std::string* number = nullptr;
            switch (what.by) {
                case By::Register: {
                    const KnownValue* reg = sel != nullptr ? sel->get(what.register_name) : nullptr;
                    number = reg != nullptr ? reg->text() : nullptr;
                    if (*what.vector != '\0' && (vector == nullptr || *vector != what.vector)) number = nullptr;
                    break;
                }
                case By::Vector: number = sel == nullptr ? vector : nullptr; break;
                case By::Value: {
                    const KnownValue* value = entry.get("value");
                    number = value != nullptr ? value->text() : nullptr;
                    break;
                }
            }
            if (number == nullptr) continue;
            if (const std::optional<std::uint64_t> parsed = knowledge::hex_value(*number)) names_.try_emplace(static_cast<Va>(*parsed), *text);
        }
    }
    std::optional<std::string_view> at(Va value) const {
        const auto it = names_.find(value);
        if (it == names_.end()) return std::nullopt;
        return std::string_view(it->second);
    }

private:
    std::unordered_map<Va, std::string> names_;
};

// The table of `what`, made at the first ask; the read is recorded at every ask.
std::optional<std::string_view> lookup(const Named& what, Va value) {
    knowledge::record_kind(what.platform, what.domain);
    static std::mutex mutex;
    static std::map<std::string, std::unique_ptr<NameTable>> tables;
    const NameTable* table = nullptr;
    {
        const std::lock_guard<std::mutex> lock(mutex);
        std::unique_ptr<NameTable>& slot = tables[std::string(what.platform) + "/" + what.domain + "/" + what.vector + "/" +
                                                  what.register_name + "/" + std::to_string(static_cast<int>(what.by)) + "/" + what.field];
        if (!slot) slot = std::make_unique<NameTable>(what);
        table = slot.get();
    }
    return table->at(value);
}

constexpr Named kMessages{"windows", "messages", By::Value, "", "", "id"};
constexpr Named kControlMessages{"vxd", "messages", By::Register, "EAX", "", "name"};
constexpr Named kDosServices{"dos", "services", By::Register, "AH", "21", "label"};
constexpr Named kBiosServices{"dos", "services", By::Vector, "", "", "label"};
constexpr Named kDpmiServices{"dos", "services", By::Register, "AX", "31", "label"};

// The string-comparing functions whose call with literals makes a keyword dispatch.
bool is_comparer(std::string_view api) {
    static constexpr std::string_view names[] = {"lstrcmp", "lstrcmpi", "strcmp", "_stricmp", "stricmp", "_strcmpi", "strcmpi",
                                                 "strncmp", "_strnicmp", "strnicmp", "wcscmp", "_wcsicmp", "wcsicmp",
                                                 "_mbscmp", "_mbsicmp", "comparestring"};
    return std::find(std::begin(names), std::end(names), api) != std::end(names);
}

// An immediate as a signed number of its own size.
Va signed_immediate(Va value, int size) {
    if (size <= 0 || size >= 8) return value;
    const Va half = Va{1} << (8 * size - 1);
    return value >= half ? value - (half << 1) : value;
}

// "0x10" with the sign always shown: "+0x10", "-0x8".
std::string signed_hex(Va v) { return std::format("{:+#x}", v); }

// The position in a function's instructions of the one at `addr`.
std::optional<std::size_t> position(const Function& fn, Va addr) {
    const auto it = std::lower_bound(fn.insns.begin(), fn.insns.end(), addr, [](const Instruction& i, Va a) { return i.addr < a; });
    if (it == fn.insns.end() || it->addr != addr) return std::nullopt;
    return static_cast<std::size_t>(it - fn.insns.begin());
}

bool same_register(const Operand& a, const Operand& b) {
    return a.kind == Operand::Kind::Register && b.kind == Operand::Kind::Register && a.reg == b.reg && a.size == b.size;
}

// What an operand of fn.insns[i] really is: the place a register was loaded from when that is in sight. -> a key and a text.
std::pair<SwitchKey, std::string> origin(const Function& fn, std::size_t i, const Operand& operand) {
    const Operand* op = &operand;
    std::optional<Definition> loaded;
    if (op->kind == Operand::Kind::Register) {
        loaded = definition_operand(fn, static_cast<int>(i), op->reg);
        if (loaded && loaded->kind == Argument::Kind::Memory) {
            op = &loaded->operand;
        } else {
            SwitchKey key;
            key.kind = SwitchKey::Kind::Register;
            key.base = std::string(family(op->reg));
            return {key, key.base};
        }
    }
    SwitchKey key;
    key.kind = SwitchKey::Kind::Memory;
    key.base = op->base.empty() ? std::string() : std::string(family(op->base));
    key.disp = op->value;
    if (!op->index.empty()) {
        key.index = std::string(family(op->index));
        const std::string base = key.base.empty() ? std::string() : key.base + "+";
        return {key, std::format("[{}{}*{}{}]", base, *key.index, op->scale, signed_hex(op->value))};
    }
    if (!key.base.empty()) return {key, std::format("[{}{}]", key.base, signed_hex(op->value))};
    return {key, std::format("[{:#x}]", op->value)};
}

// What a chain test has accumulated about a register: the thing tested, and how much has been subtracted from it.
struct Tested {
    SwitchKey key;
    std::string text;
    Va amount = 0;
};

bool is_jump_flow(Flow f) { return f == Flow::CondJump || f == Flow::Jump; }

}  // namespace

std::optional<std::string_view> message_name(Va message) { return lookup(kMessages, message); }
std::optional<std::string_view> control_message_name(Va message) { return lookup(kControlMessages, message); }

std::size_t SwitchKeyHash::operator()(const SwitchKey& key) const {
    std::size_t h = std::hash<std::string>()(key.base);
    h = h * 31 + (key.index ? std::hash<std::string>()(*key.index) + 1 : 0);
    h = h * 31 + std::hash<Va>()(key.disp);
    return h * 31 + static_cast<std::size_t>(key.kind);
}

Chains chains(const Function& fn) {
    Chains out;
    std::unordered_set<Va> labels;
    for (const Instruction& i : fn.insns) {
        if (is_jump_flow(i.flow) && i.target) labels.insert(*i.target);
    }
    std::map<std::string, Tested> acc;                 // register -> what it holds and how much was subtracted so far
    std::optional<Tested> pending;                     // what the last test compared: a following je/jne is a case
    for (std::size_t i = 0; i < fn.insns.size(); ++i) {
        const Instruction& ins = fn.insns[i];
        const std::string& mn = ins.mnemonic;
        const std::vector<Operand>& ops = ins.operands;
        if (labels.contains(ins.addr)) acc.clear();
        if (ins.flow == Flow::CondJump) {
            if (pending && (mn == "je" || mn == "jz" || mn == "jne" || mn == "jnz") && ins.target) {
                Chain& slot = out.try_emplace(pending->key, Chain{pending->text, {}}).first->second;
                slot.cases.try_emplace(pending->amount, (mn == "je" || mn == "jz") ? *ins.target : ins.addr + ins.size);
            }
            continue;   // flags survive a conditional jump
        }
        pending.reset();
        if (mn == "cmp" && ops.size() == 2 && ops[1].kind == Operand::Kind::Immediate &&
            (ops[0].kind == Operand::Kind::Register || ops[0].kind == Operand::Kind::Memory)) {
            Tested t;
            Va base = 0;
            const auto held = ops[0].kind == Operand::Kind::Register ? acc.find(std::string(family(ops[0].reg))) : acc.end();
            if (held != acc.end()) {
                t = held->second;
                base = t.amount;
            } else {
                auto [key, text] = origin(fn, i, ops[0]);
                t.key = std::move(key);
                t.text = std::move(text);
            }
            t.amount = (ops[1].value + base) & 0xFFFFFFFF;
            pending = std::move(t);
        } else if ((mn == "sub" || mn == "dec" || mn == "add" || mn == "inc") && !ops.empty() &&
                   ops[0].kind == Operand::Kind::Register && (ops.size() == 1 || ops[1].kind == Operand::Kind::Immediate)) {
            const std::string fam(family(ops[0].reg));
            if (fam == "esp" || fam == "ebp") continue;
            auto held = acc.find(fam);
            if (held == acc.end()) {
                auto [key, text] = origin(fn, i, ops[0]);
                held = acc.emplace(fam, Tested{std::move(key), std::move(text), 0}).first;
            }
            const Va step = mn == "dec"   ? 1
                            : mn == "inc" ? -1
                            : mn == "sub" ? ops[1].value
                                          : -signed_immediate(ops[1].value, ops[1].size);
            held->second.amount += step;
            pending = held->second;
            pending->amount = held->second.amount & 0xFFFFFFFF;
        } else if ((mn == "test" || mn == "or" || mn == "and") && ops.size() == 2 && same_register(ops[0], ops[1])) {
            const auto held = acc.find(std::string(family(ops[0].reg)));
            if (held != acc.end()) {
                pending = held->second;                 // (the amount subtracted so far, as it stands)
            } else {
                auto [key, text] = origin(fn, i, ops[0]);
                pending = Tested{std::move(key), std::move(text), 0};
            }
        } else {
            for (const std::string& r : ins.writes) acc.erase(r);
        }
    }
    return out;
}

std::vector<Switch> switches(Image& image) {
    std::vector<Switch> out;
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        Chains found = chains(fn);
        std::set<std::optional<SwitchKey>> tabled;
        for (const JumpTable& t : fn.tables) {
            std::optional<SwitchKey> key;
            std::string text = t.selector;
            if (const auto site = position(fn, t.site)) {
                const std::size_t i = *site;
                for (std::size_t j = i; j-- > 0 && i - j < 10;) {   // the ten instructions before the jump, nearest first
                    const Instruction& b = fn.insns[j];
                    if (b.mnemonic != "cmp" || b.operands.size() < 2 || b.operands[1].kind != Operand::Kind::Immediate) continue;
                    auto [k, tx] = origin(fn, j, b.operands[0]);
                    key = std::move(k);
                    text = std::move(tx);
                    for (std::size_t p = j; p-- > 0 && j - p < 4;) {   // past the `sub reg, low`
                        const Instruction& q = fn.insns[p];
                        if ((q.mnemonic == "sub" || q.mnemonic == "dec" || q.mnemonic == "add" || q.mnemonic == "lea") && !q.operands.empty() &&
                            q.operands[0].kind == Operand::Kind::Register && b.operands[0].kind == Operand::Kind::Register &&
                            family(q.operands[0].reg) == family(b.operands[0].reg)) {
                            auto [k2, tx2] = origin(fn, p, q.operands[0]);
                            key = std::move(k2);
                            text = std::move(tx2);
                        }
                    }
                    break;
                }
            }
            std::map<Va, Va> cases;
            if (t.index_bytes) {
                for (std::size_t n = 0; n < t.index_bytes->size(); ++n) {
                    const std::size_t slot = (*t.index_bytes)[n];
                    if (slot < t.targets.size() && (!t.default_target || t.targets[slot] != *t.default_target)) {
                        cases[t.low + static_cast<Va>(n)] = t.targets[slot];
                    }
                }
            } else {
                for (std::size_t n = 0; n < t.targets.size(); ++n) cases[t.low + static_cast<Va>(n)] = t.targets[n];
            }
            tabled.insert(key);
            std::map<Va, Va> merged;
            if (key) {
                if (const Chain* c = found.find(*key)) merged = c->cases;
            }
            for (const auto& [v, target] : cases) merged[v] = target;
            Switch s;
            s.function = fn.start;
            s.on = std::move(text);
            s.key = key;
            s.cases = std::move(merged);
            s.default_target = t.default_target;
            s.table = t.table;
            s.site = t.site;
            s.bounded = t.bounded;
            out.push_back(std::move(s));
        }
        for (const auto& [key, c] : found) {
            if (tabled.contains(key) || c.cases.size() < 3) continue;
            Switch s;
            s.function = fn.start;
            s.on = c.on;
            s.key = key;
            s.cases = c.cases;
            out.push_back(std::move(s));
        }
    }
    return out;
}

namespace {

// Is `key` the message argument of a window or dialog procedure? (The second argument; a 16-bit far one is at bp+0Ch.)
bool is_message_argument(const Image& image, const std::optional<SwitchKey>& key, bool far) {
    if (!key || key->kind != SwitchKey::Kind::Memory || key->index) return false;
    if (image.bits() == 16) return key->base == "ebp" && key->disp == (far ? 0x0C : 0x0A);
    return (key->base == "ebp" && key->disp == 0x0C) || (key->base == "esp" && key->disp == 8);
}

// Is `key` the command argument (wParam)?
bool is_command_argument(const Image& image, const std::optional<SwitchKey>& key, bool far) {
    if (!key || key->kind != SwitchKey::Kind::Memory || key->index) return false;
    if (image.bits() == 16) return key->base == "ebp" && key->disp == (far ? 0x0A : 0x08);
    return (key->base == "ebp" && key->disp == 0x10) || (key->base == "esp" && key->disp == 0x0C);
}

bool is_core_message(Va v) { return std::find(std::begin(kCoreMessages), std::end(kCoreMessages), v) != std::end(kCoreMessages); }

}  // namespace

std::vector<MessageDispatch> messages(Image& image, const std::vector<Switch>& all, const ConventionFacts& conventions) {
    std::map<Va, std::vector<const Switch*>> by_function;
    for (const Switch& s : all) by_function[s.function].push_back(&s);
    std::vector<MessageDispatch> out;
    for (const auto& [function, sws] : by_function) {
        const auto fact_it = conventions.find(function);
        const ConventionFact fact = fact_it != conventions.end() ? fact_it->second : ConventionFact{};
        const bool far = fact.is_far();
        const bool proc = fact.procedure && !fact.procedure->empty();
        MessageDispatch found;
        found.function = function;
        found.procedure = fact.procedure;
        bool matched = false;
        for (const Switch* s : sws) {
            std::size_t known = 0, telling = 0;
            bool telling_core = false;
            for (const auto& [v, target] : s->cases) {
                if (!message_name(v)) continue;
                ++known;
                if (v >= 0x0F) {   // 1, 2, 3 are also just small numbers
                    ++telling;
                    telling_core = telling_core || is_core_message(v);
                }
            }
            const std::size_t values = s->cases.size();
            const bool on_message = is_message_argument(image, s->key, far) &&
                                    (fact.removes == (image.bits() == 32 ? 16 : 10) || telling > 0);
            if (image.kind() == ImageKind::Le) continue;
            if ((on_message && known >= 2) || (proc && telling >= 2 && known * 2 >= values) ||
                (telling >= 3 && telling_core && known * 4 >= values * 3)) {
                if (!matched) {
                    found.why = on_message ? "switch on the message argument" : proc ? "a " + *fact.procedure : "cases are message numbers";
                }
                matched = true;
                for (const auto& [v, target] : s->cases) found.messages[v] = target;
            } else if (is_command_argument(image, s->key, far)) {
                for (const auto& [v, target] : s->cases) found.commands[v] = target;
            }
        }
        if (matched) out.push_back(std::move(found));
    }
    return out;
}

Keywords keywords(Image& image) {
    Keywords out;
    const call_graph::Graph graph = call_graph::build(image);
    strings::Table table = strings::build_table(image, graph);
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        struct Compared {
            std::string text, api;
        };
        std::vector<Compared> words;
        const auto calls = graph.calls.find(start);
        if (calls == graph.calls.end()) continue;
        for (const call_graph::Call& call : calls->second) {
            if (!call.destination || !image.is_import(*call.destination)) continue;
            const std::string& full = image.import_name(*call.destination);
            const std::size_t bang = full.rfind('!');
            std::string api = text::lower(full.substr(bang == std::string::npos ? 0 : bang + 1));
            if (api.size() > 1 && (api.back() == 'a' || api.back() == 'w') && is_comparer(std::string_view(api).substr(0, api.size() - 1))) api.pop_back();
            if (!is_comparer(api)) continue;
            const auto at = position(fn, call.site);
            if (!at) continue;
            const std::vector<Argument> args = arguments(fn, static_cast<int>(*at), image.bits() == 16 ? 4 : 2);
            for (std::size_t k = 0; k < 2; ++k) {
                if (const auto lit = strings::literal(image, table, args, k)) {
                    words.push_back({lit->entry->text, api});
                    break;
                }
            }
        }
        std::set<std::string_view> distinct;
        for (const Compared& w : words) distinct.insert(w.text);
        if (distinct.size() >= 2) {
            KeywordFunction k;
            k.function = start;
            k.compares_with = words.front().api;
            for (const Compared& w : words) k.keywords.push_back(w.text);
            out.functions.push_back(std::move(k));
        }
    }
    // tables in the data that pair a string with a function
    for (const globals::PointerTable& t : globals::pointer_tables(image, table)) {
        if (t.shape != globals::PointerTable::Shape::Run || t.count < 4 || t.count % 2 != 0) continue;
        bool pairs = true;
        for (std::size_t k = 0; k < static_cast<std::size_t>(t.count) && pairs; ++k) {
            pairs = t.kinds[k] == (k % 2 == 0 ? globals::SlotKind::String : globals::SlotKind::Function);
        }
        if (!pairs) continue;
        KeywordTable k;
        k.table = t.at;
        for (std::size_t n = 0; n < static_cast<std::size_t>(t.count); n += 2) {
            k.keywords.push_back(table.at(t.targets[n]).text);
            k.handlers.push_back(t.targets[n + 1]);
        }
        out.tables.push_back(std::move(k));
    }
    return out;
}

std::vector<Interrupt> interrupts(Image& image) {
    std::vector<Interrupt> out;
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        for (std::size_t i = 0; i < fn.insns.size(); ++i) {
            const Instruction& ins = fn.insns[i];
            if (ins.mnemonic != "int" || ins.operands.empty() || ins.operands[0].kind != Operand::Kind::Immediate) continue;
            const auto n = static_cast<std::uint8_t>(ins.operands[0].value & 0xFF);
            if (image.kind() == ImageKind::Le && n == 0x20) continue;   // a driver's service call, already an import
            const bool wide = n == 0x31 || n == 0x2F;                      // these take a number in AX
            std::optional<std::uint32_t> code;
            for (std::size_t back = i; back-- > 0 && i - back <= 8;) {
                const Instruction& b = fn.insns[back];
                if (b.flow != Flow::None || b.mnemonic == "int") break;
                if (b.mnemonic == "mov" && b.operands.size() == 2 && b.operands[0].kind == Operand::Kind::Register &&
                    b.operands[1].kind == Operand::Kind::Immediate) {
                    const Va v = b.operands[1].value;
                    if (b.operands[0].reg == "ah") {
                        if (!wide) code = static_cast<std::uint32_t>(v);
                        break;
                    }
                    if (b.operands[0].reg == "ax" || b.operands[0].reg == "eax") {
                        code = wide ? static_cast<std::uint32_t>(v & 0xFFFF) : static_cast<std::uint32_t>((v >> 8) & 0xFF);
                        break;
                    }
                }
            }
            Interrupt it;
            it.function = start;
            it.site = ins.addr;
            it.number = n;
            it.code = code;
            if (n == 0x21) {
                const auto known = code ? lookup(kDosServices, *code) : std::nullopt;
                it.name = "DOS: " + (known ? std::string(*known) : "function " + (code ? std::format("{:02X}h", *code) : std::string("?")));
                it.named = code.has_value();
            } else if (n == 0x31) {
                const auto known = code ? lookup(kDpmiServices, *code) : std::nullopt;
                it.name = "DPMI: " + (known ? std::string(*known) : "function " + (code ? std::format("{:04X}h", *code) : std::string("?")));
                it.named = code.has_value();
            } else {
                const auto bios = lookup(kBiosServices, n);
                it.name = bios ? std::string(*bios) : std::format("interrupt {:02X}h", n);
                if (code) it.name += std::format(" function {:02X}h", *code);
                it.named = bios.has_value();
            }
            out.push_back(std::move(it));
        }
    }
    return out;
}

std::optional<ControlMessages> control_messages(Image& image, const std::vector<Switch>& all, const std::optional<DeviceBlock>& device) {
    if (image.kind() != ImageKind::Le || !device) return std::nullopt;
    const std::optional<std::uint32_t> proc_address = image.u32(device->descriptor + 0x18);
    if (!proc_address || !image.is_function(*proc_address)) return std::nullopt;
    const Va proc = *proc_address;
    std::map<Va, Va> best;
    for (const Switch& s : all) {
        if (s.function == proc && s.key && s.key->kind == SwitchKey::Kind::Register && s.key->base == "eax") {
            for (const auto& [v, target] : s.cases) best[v] = target;
        }
    }
    // short procedures test one or two messages only
    for (const auto& [key, c] : chains(image.function(proc))) {
        if (c.on != "eax") continue;
        for (const auto& [v, target] : c.cases) best.try_emplace(v, target);
    }
    ControlMessages out;
    out.procedure = proc;
    for (const auto& [v, target] : best) {
        const auto name = control_message_name(v);
        out.handles.push_back({v, name ? std::string(*name) : std::format("message {:#x}", v), target});
    }
    return out;
}

Report analyse(Image& image, const ConventionFacts& conventions, const std::optional<DeviceBlock>& device) {
    const std::vector<Switch> sw = switches(image);
    const std::vector<MessageDispatch> msgs = messages(image, sw, conventions);
    const Keywords kw = keywords(image);
    const std::vector<Interrupt> ints = interrupts(image);
    const std::optional<ControlMessages> ctl = control_messages(image, sw, device);

    Report r;
    for (const Switch& s : sw) {
        SwitchFact f;
        f.function = s.function;
        f.on = s.on;
        f.site = s.site;
        f.table = s.table;
        f.default_target = s.default_target;
        f.cases.assign(s.cases.begin(), s.cases.end());
        r.facts.emplace_back(std::move(f));
    }
    for (const MessageDispatch& m : msgs) {
        MessageFact f;
        f.function = m.function;
        f.procedure = m.procedure;
        f.why = m.why;
        for (const auto& [v, handler] : m.messages) {
            const auto name = message_name(v);
            f.messages.push_back({v, name ? std::string(*name) : std::format("{:#06x}", v), handler});
        }
        f.commands.assign(m.commands.begin(), m.commands.end());
        r.facts.emplace_back(std::move(f));
    }
    for (const KeywordFunction& k : kw.functions) r.facts.emplace_back(KeywordFunctionFact{k});
    for (const KeywordTable& k : kw.tables) r.facts.emplace_back(KeywordTableFact{k});
    std::map<Va, std::set<std::string>> services;
    for (const Interrupt& i : ints) services[i.function].insert(i.name);
    for (const auto& [function, names] : services) r.facts.emplace_back(ServicesFact{function, {names.begin(), names.end()}});
    if (ctl) r.facts.emplace_back(DriverControlFact{ctl->procedure, device ? std::optional<std::string>(device->name) : std::nullopt, ctl->handles});

    Counts& c = r.counts;
    for (const Switch& s : sw) {
        if (s.table) {
            ++c.jump_tables;
            c.jump_table_cases += static_cast<std::int64_t>(s.cases.size());
            if (s.bounded) ++c.jump_tables_with_bound;
        } else {
            ++c.compare_chains;
            c.compare_chain_cases += static_cast<std::int64_t>(s.cases.size());
        }
        if (s.key && s.key->kind == SwitchKey::Kind::Memory && (s.key->base == "ebp" || s.key->base == "esp") && s.key->disp > 0) {
            ++c.switches_on_an_argument;
        }
    }
    c.message_dispatchers = static_cast<std::int64_t>(msgs.size());
    for (const MessageDispatch& m : msgs) {
        c.messages_handled += static_cast<std::int64_t>(m.messages.size());
        for (const auto& [v, handler] : m.messages) c.named_messages += message_name(v) ? 1 : 0;
        if (!m.commands.empty()) ++c.command_dispatchers;
        c.command_ids += static_cast<std::int64_t>(m.commands.size());
    }
    c.keyword_dispatchers = static_cast<std::int64_t>(kw.functions.size() + kw.tables.size());
    for (const KeywordFunction& k : kw.functions) c.keywords += static_cast<std::int64_t>(k.keywords.size());
    for (const KeywordTable& k : kw.tables) c.keywords += static_cast<std::int64_t>(k.keywords.size());
    c.interrupt_sites = static_cast<std::int64_t>(ints.size());
    for (const Interrupt& i : ints) c.named_services += i.named ? 1 : 0;
    c.driver_control_messages = ctl ? static_cast<std::int64_t>(ctl->handles.size()) : 0;

    // a few lines saying what was found
    auto wm = [](Va v) { const auto n = message_name(v); return n ? std::string(*n) : std::format("{:#x}", v); };
    for (std::size_t i = 0; i < msgs.size() && i < 6; ++i) {
        std::vector<std::string> shown;
        for (const auto& [v, handler] : msgs[i].messages) {
            if (shown.size() == 10) break;
            shown.push_back(wm(v));
        }
        r.text.push_back(std::format("{} ({}): {}", image.label(msgs[i].function), msgs[i].why, text::join(shown, ", ")));
    }
    auto quoted = [](const std::string& w) { return "'" + w + "'"; };
    for (std::size_t i = 0; i < kw.functions.size() + kw.tables.size() && i < 4; ++i) {
        const bool from_function = i < kw.functions.size();
        const std::vector<std::string>& words = from_function ? kw.functions[i].keywords : kw.tables[i - kw.functions.size()].keywords;
        std::vector<std::string> shown;
        for (std::size_t k = 0; k < words.size() && k < 8; ++k) shown.push_back(quoted(words[k]));
        const std::string where = from_function ? image.label(kw.functions[i].function)
                                                : std::format("table {:x}", kw.tables[i - kw.functions.size()].table);
        r.text.push_back(std::format("keywords in {}: {}", where, text::join(shown, ", ")));
    }
    if (ctl) {
        std::vector<std::string> names;
        for (const HandledMessage& h : ctl->handles) names.push_back(h.name);
        r.text.push_back(std::format("device {} control procedure {} handles: {}", device ? device->name : std::string(),
                                     image.label(ctl->procedure), text::join(names, ", ")));
    }
    if (!ints.empty()) {
        std::vector<std::pair<std::string, int>> seen;   // in the order first met
        for (const Interrupt& i : ints) {
            const auto it = std::find_if(seen.begin(), seen.end(), [&](const auto& p) { return p.first == i.name; });
            if (it == seen.end()) seen.emplace_back(i.name, 1);
            else ++it->second;
        }
        std::stable_sort(seen.begin(), seen.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
        std::vector<std::string> shown;
        for (std::size_t k = 0; k < seen.size() && k < 10; ++k) shown.push_back(std::format("{} x{}", seen[k].first, seen[k].second));
        r.text.push_back("system services: " + text::join(shown, ", "));
    }
    return r;
}

}  // namespace cb::analyze::dispatch

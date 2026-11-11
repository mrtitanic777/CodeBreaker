#include "cb/analyze/strings.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <set>
#include <unordered_map>

#include "cb/core/text.hpp"

namespace cb::analyze::strings {

namespace {

bool printable(std::uint8_t c) { return c >= 0x20 && c < 0x7F; }
bool texty(std::uint8_t c) { return printable(c) || c == 9 || c == 10 || c == 13; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }

// The string starting at `start`, if it is one of at least `least` characters: printable bytes, then a zero.
std::optional<std::string> ascii_at(ByteView d, Va start, Va least) {
    Va end = start;
    const Va n = static_cast<Va>(d.size());
    while (end < n && texty(d[static_cast<std::size_t>(end)])) ++end;
    if (end - start >= least && end < n && d[static_cast<std::size_t>(end)] == 0)
        return std::string(d.begin() + start, d.begin() + end);
    return std::nullopt;
}

// The same for 16-bit characters: a printable byte and a zero, up to a zero word.
std::optional<std::string> wide_at(ByteView d, Va start, Va least) {
    Va end = start;
    const Va n = static_cast<Va>(d.size());
    while (end + 1 < n && d[static_cast<std::size_t>(end) + 1] == 0 && texty(d[static_cast<std::size_t>(end)])) end += 2;
    if ((end - start) / 2 >= least && end + 1 < n && d[static_cast<std::size_t>(end)] == 0 &&
        d[static_cast<std::size_t>(end) + 1] == 0) {
        std::string text;
        for (Va k = start; k < end; k += 2) text += static_cast<char>(d[static_cast<std::size_t>(k)]);
        return text;
    }
    return std::nullopt;
}

// Does the text hold two letters in a row? (re.search("[A-Za-z]{2}", text))
bool has_letters(std::string_view t) {
    auto alpha = [](char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); };
    for (std::size_t k = 0; k + 1 < t.size(); ++k)
        if (alpha(t[k]) && alpha(t[k + 1])) return true;
    return false;
}

}  // namespace

// ---- the table -------------------------------------------------------------------------------------------------

std::optional<std::string> read_ascii(ByteView data, std::int64_t start, std::int64_t least) { return ascii_at(data, start, least); }
std::optional<std::string> read_wide(ByteView data, std::int64_t start, std::int64_t least) { return wide_at(data, start, least); }

std::optional<Found> string_in(ByteView data, std::int64_t off, std::int64_t initialised, int bits, int least) {
    if (off < 0 || off >= initialised) return std::nullopt;
    if (bits == 32) {
        if (auto w = wide_at(data, off, least)) return Found{std::move(*w), Encoding::Wide};
    }
    if (auto a = ascii_at(data, off, least)) {
        // not the tail of a longer one
        const bool tail = a->size() < 4 && off != 0 && printable(data[static_cast<std::size_t>(off) - 1]);
        if (!tail) return Found{std::move(*a), Encoding::Ascii};
    }
    return std::nullopt;
}

std::optional<Found> string_at(const Image& img, Va va, int least) {
    const Span* s = img.span(va);
    if (s == nullptr) return std::nullopt;
    return string_in(s->data, va - s->va, s->initialised, img.bits(), least);
}

Table build_table(const Image& img, const call_graph::Graph& graph) {
    Table table;
    // what the code or the file's relocations point at
    std::set<Va> referenced;
    for (const call_graph::Reference& r : graph.refs)
        if (r.how == call_graph::Access::AddressTaken) referenced.insert(r.address);
    if (img.has_pointers()) {
        for (const auto& [site, ptr] : img.pointer_list())
            if (ptr.target && !img.is_code(site)) referenced.insert(*ptr.target);
    }
    for (Va va : referenced) {
        if (img.is_function(va)) continue;   // a function's start is not a string
        if (auto found = string_at(img, va, 2)) table[va] = Entry{std::move(found->text), found->encoding, true};
    }
    // what nothing refers to: outside the code, and outside the sections that are not data
    for (const Span& s : img.spans()) {
        if (s.executable || s.name == ".rsrc" || s.name == ".reloc" || s.name == ".edata" || s.name == ".idata") continue;
        const Bytes& d = s.data;
        Va off = 0;
        const Va held = std::min<Va>(s.initialised, static_cast<Va>(d.size()));
        while (off < held) {
            if (printable(d[static_cast<std::size_t>(off)])) {
                std::optional<std::string> text = ascii_at(d, off, img.bits() == 32 ? 5 : 4);
                int unit = 1;
                if (!text && img.bits() == 32 && off + 1 < held && d[static_cast<std::size_t>(off) + 1] == 0) {
                    text = wide_at(d, off, 5);
                    unit = 2;
                }
                if (text) {
                    const Va next = off + (static_cast<Va>(text->size()) + 1) * unit;
                    table.try_emplace(s.va + off, Entry{std::move(*text), unit == 1 ? Encoding::Ascii : Encoding::Wide, false});
                    off = next;
                    continue;
                }
                while (off < held && printable(d[static_cast<std::size_t>(off)])) ++off;   // the rest of a run that is not one
            }
            ++off;
        }
    }
    return table;
}

// ---- uses ------------------------------------------------------------------------------------------------------

namespace {

// Is the data segment pushed or stored beside instruction i? (a 16-bit string reference is an offset and DS)
bool near_ds(const Function& fn, std::size_t i) {
    const std::size_t lo = i >= 2 ? i - 2 : 0, hi = std::min(fn.insns.size(), i + 3);
    for (std::size_t k = lo; k < hi; ++k)
        for (const Operand& op : fn.insns[k].operands)
            if (op.kind == Operand::Kind::Register && op.reg == "ds") return true;
    return false;
}

}  // namespace

std::vector<FunctionUses> find_uses(Image& img, const call_graph::Graph& graph, Table& table) {
    std::vector<FunctionUses> out;
    std::unordered_map<Va, std::size_t> row_of;   // function -> its place in `out`
    auto add = [&](Va f, Va site, Va va, const std::string& text) {
        auto it = row_of.find(f);
        if (it == row_of.end()) {
            it = row_of.emplace(f, out.size()).first;
            out.push_back({f, {}});
        }
        out[it->second].uses.push_back({site, va, text});
    };
    if (img.bits() == 32) {
        for (const call_graph::Reference& r : graph.refs) {
            if (r.how != call_graph::Access::AddressTaken) continue;
            if (const auto it = table.find(r.address); it != table.end()) add(r.function, r.instruction, r.address, it->second.text);
        }
        return out;
    }
    // 16-bit: an offset into the data segment, beside DS
    const Va base = img.data_segment();
    for (Va start : std::vector<Va>(img.functions())) {
        const Function& fn = img.function(start);
        for (std::size_t i = 0; i < fn.insns.size(); ++i) {
            const Instruction& ins = fn.insns[i];
            if ((ins.mnemonic != "push" && ins.mnemonic != "mov" && ins.mnemonic != "lea") || ins.flow != Flow::None ||
                ins.operands.empty())
                continue;
            const Operand& op = ins.operands.back();
            std::optional<Va> va;
            if (op.kind == Operand::Kind::Immediate && op.size == 2 && op.value >= 0x10) va = base + op.value;
            else if (op.kind == Operand::Kind::Memory && ins.mnemonic == "lea" && op.base.empty() && op.index.empty())
                va = base + (op.value & 0xFFFF);
            if (!va || base == 0) continue;
            const auto it = table.find(*va);
            if (it == table.end() || !(ins.mnemonic == "lea" || near_ds(fn, i))) continue;
            add(fn.start, ins.addr, *va, it->second.text);
            it->second.referenced = true;
        }
    }
    return out;
}

// ---- strings built on the stack ----------------------------------------------------------------------------------

std::vector<Piece> assemble(const Stores& stores, std::size_t least) {
    std::vector<Piece> out;
    std::vector<std::pair<std::pair<std::string, std::int64_t>, Store>> keys(stores.begin(), stores.end());
    std::size_t i = 0;
    while (i < keys.size()) {
        // a run of consecutive offsets from one base
        const std::string& base = keys[i].first.first;
        const std::int64_t off = keys[i].first.second;
        std::size_t j = i;
        while (j + 1 < keys.size() && keys[j + 1].first.first == base && keys[j + 1].first.second == keys[j].first.second + 1) ++j;
        std::vector<Store> run;
        for (std::size_t k = i; k <= j; ++k) run.push_back(keys[k].second);
        // 8-bit characters
        std::size_t start = 0;
        for (std::size_t k = 0; k <= run.size(); ++k) {
            if (k == run.size() || !printable(run[k].byte)) {
                std::string text;
                std::set<int> numbers;
                for (std::size_t x = start; x < k; ++x) {
                    text += static_cast<char>(run[x].byte);
                    numbers.insert(run[x].number);
                }
                if (k - start >= least && numbers.size() >= 2 && has_letters(text))
                    out.push_back({base, off + static_cast<std::int64_t>(start), text, false});
                start = k + 1;
            }
        }
        // 16-bit characters: letter, 0, letter, 0
        std::size_t k = 0;
        while (k + 1 < run.size()) {
            std::size_t m = k;
            while (m + 1 < run.size() && printable(run[m].byte) && run[m + 1].byte == 0) m += 2;
            if ((m - k) / 2 >= 3) {
                std::string text;
                for (std::size_t x = k; x < m; x += 2) text += static_cast<char>(run[x].byte);
                if (has_letters(text)) out.push_back({base, off + static_cast<std::int64_t>(k), text, true});
            }
            k = std::max(m, k + 1);
        }
        i = j + 1;
    }
    return out;
}

std::vector<StackString> stack_strings(Image& img) {
    std::vector<StackString> out;
    for (Va start : std::vector<Va>(img.functions())) {
        const Function& fn = img.function(start);
        Stores stores;
        int n = 0;
        for (const Instruction& ins : fn.insns) {
            const auto& ops = ins.operands;
            if (ins.mnemonic != "mov" || ops.size() != 2 || ops[0].kind != Operand::Kind::Memory ||
                ops[1].kind != Operand::Kind::Immediate || ops[0].base.empty() || !ops[0].index.empty())
                continue;
            const std::string base(family(ops[0].base));
            if (base != "ebp" && base != "esp") continue;
            ++n;
            for (int k = 0; k < ops[0].size; ++k) {
                const Va v = ops[1].value;
                const int byte = 8 * k < 64 ? static_cast<int>((v >> (8 * k)) & 0xFF) : (v < 0 ? 0xFF : 0);
                stores[{base, ops[0].value + k}] = Store{static_cast<std::uint8_t>(byte), n};
            }
        }
        if (stores.size() < 4) continue;
        for (const Piece& p : assemble(stores)) {
            const std::int64_t magnitude = p.offset < 0 ? -p.offset : p.offset;
            out.push_back({fn.start, std::format("{}{}0x{:x}", p.base, p.offset < 0 ? "-" : "+", magnitude), p.text, p.wide});
        }
    }
    return out;
}

// ---- format strings --------------------------------------------------------------------------------------------

namespace {

// One conversion of a printf-style format: %[flags][width][.precision][length]conversion
struct Spec {
    bool width_star = false;
    bool precision_star = false;
    std::string_view length;
    char conv = 0;
};

constexpr std::string_view kConversions = "diouxXeEfgGcCsSpn%";
constexpr std::array<std::string_view, 8> kLengths = {"hh", "h", "ll", "l", "L", "w", "I64", "I32"};

// The conversions in a format, left to right. A '%' that does not start a conversion is skipped, as is the rest of
// anything that does not read as one (`%.f` is not a conversion here: a precision needs digits or '*').
std::vector<Spec> scan_conversions(std::string_view t) {
    std::vector<Spec> out;
    std::size_t p = 0;
    while (p < t.size()) {
        if (t[p] != '%') {
            ++p;
            continue;
        }
        std::size_t q = p + 1;
        Spec spec;
        while (q < t.size() && std::string_view("-+ 0#").find(t[q]) != std::string_view::npos) ++q;
        if (q < t.size() && t[q] == '*') {
            spec.width_star = true;
            ++q;
        } else {
            while (q < t.size() && is_digit(t[q])) ++q;
        }
        if (q + 1 < t.size() && t[q] == '.' && (t[q + 1] == '*' || is_digit(t[q + 1]))) {
            ++q;
            if (t[q] == '*') {
                spec.precision_star = true;
                ++q;
            } else {
                while (q < t.size() && is_digit(t[q])) ++q;
            }
        }
        // a length modifier, when a conversion follows it
        for (std::string_view len : kLengths) {
            if (t.compare(q, len.size(), len) == 0 && q + len.size() < t.size() &&
                kConversions.find(t[q + len.size()]) != std::string_view::npos) {
                spec.length = len;
                q += len.size();
                break;
            }
        }
        if (q < t.size() && kConversions.find(t[q]) != std::string_view::npos) {
            spec.conv = t[q];
            out.push_back(spec);
            p = q + 1;
        } else {
            ++p;
        }
    }
    return out;
}

// The printf-like imports: (name, position of the format argument).
struct Formatter {
    std::string_view name;
    int position;
};
constexpr std::array<Formatter, 18> kFormatters = {{{"wsprintf", 1}, {"wvsprintf", 1}, {"sprintf", 1},  {"swprintf", 1},
                                                    {"vsprintf", 1}, {"printf", 0},    {"wprintf", 0},  {"fprintf", 1},
                                                    {"fwprintf", 1}, {"_snprintf", 2}, {"_snwprintf", 2}, {"sscanf", 1},
                                                    {"swscanf", 1},  {"fscanf", 1},    {"_vsnprintf", 2}, {"wnsprintf", 2},
                                                    {"_wsprintf", 1}, {"_cprintf", 0}}};
// the arguments of these are in a va_list elsewhere
constexpr std::array<std::string_view, 3> kNoList = {"wvsprintf", "vsprintf", "_vsnprintf"};

std::string_view after_last_bang(std::string_view name) {
    const std::size_t bang = name.rfind('!');
    return bang == std::string_view::npos ? name : name.substr(bang + 1);
}

std::optional<int> formatter_position(std::string_view lowered) {
    for (const Formatter& f : kFormatters)
        if (f.name == lowered) return f.position;
    return std::nullopt;
}

struct FormatterInfo {
    int position = 0;
    bool wide = false;   // takes a wide format string
};

// What a printf-like import is: where its format string is, and whether it takes a wide one.
std::optional<FormatterInfo> formatter(std::string_view api) {
    const std::string_view bare = after_last_bang(api);
    if (std::any_of(bare.begin(), bare.end(), [](char c) { return static_cast<unsigned char>(c) >= 0x80; })) return std::nullopt;
    std::string_view stem = bare;
    // wsprintfA / wsprintfW: the A/W is a suffix
    if (!bare.empty() && (bare.back() == 'A' || bare.back() == 'W') &&
        formatter_position(text::lower(bare.substr(0, bare.size() - 1))))
        stem = bare.substr(0, bare.size() - 1);
    const std::string lowered = text::lower(stem);
    const std::optional<int> position = formatter_position(lowered);
    if (!position) return std::nullopt;
    std::string rest = text::replace_all(text::replace_all(text::replace_all(lowered, "wsprintf", ""), "wvsprintf", ""), "wnsprintf", "");
    FormatterInfo info;
    info.position = *position;
    info.wide = (!bare.empty() && bare.back() == 'W') || rest.find('w') != std::string::npos;
    return info;
}

bool lists_arguments(std::string_view api) {
    const std::string bare = text::lower(after_last_bang(api));
    return std::none_of(kNoList.begin(), kNoList.end(), [&](std::string_view n) { return bare.starts_with(n); });
}

}  // namespace

std::vector<std::string> conversions(std::string_view format, bool wide) {
    std::vector<std::string> types;
    for (const Spec& s : scan_conversions(format)) {
        if (s.conv == '%') continue;
        for (int i = 0; i < (s.width_star ? 1 : 0) + (s.precision_star ? 1 : 0); ++i) types.push_back("int");
        if (s.conv == 's' || s.conv == 'S') {
            const bool wide_string = s.length == "l" || s.length == "w" || ((s.conv == 'S') != wide);
            types.push_back(wide_string ? "wchar_t *" : "char *");
        } else if (s.conv == 'c' || s.conv == 'C') {
            types.push_back((s.conv == 'C') != wide ? "wchar_t" : "char");
        } else if (s.conv == 'p') {
            types.push_back("void *");
        } else if (s.conv == 'n') {
            types.push_back("int *");
        } else if (std::string_view("eEfgG").find(s.conv) != std::string_view::npos) {
            types.push_back("double");
        } else {
            const bool is_signed = s.conv == 'd' || s.conv == 'i';
            if (s.length == "ll" || s.length == "I64") types.push_back(is_signed ? "__int64" : "unsigned __int64");
            else if (s.length == "l") types.push_back(is_signed ? "long" : "unsigned long");
            else if (s.length == "h") types.push_back(is_signed ? "short" : "unsigned short");
            else types.push_back(is_signed ? "int" : "unsigned");
        }
    }
    return types;
}

std::optional<Literal> literal(const Image& img, const Table& table, const std::vector<Argument>& args, std::size_t k) {
    auto entry = [&](Va va) -> std::optional<Literal> {
        const auto it = table.find(va);
        if (it == table.end()) return std::nullopt;
        return Literal{va, &it->second};
    };
    if (img.bits() == 32) {
        if (k < args.size() && args[k].kind == Argument::Kind::Immediate) return entry(args[k].value);
        return std::nullopt;
    }
    // 16-bit: a far pointer is two words, offset pushed last; the format is the k-th far pointer
    const std::size_t lo = 2 * k, hi = 2 * k + 1;
    if (hi < args.size() && args[lo].kind == Argument::Kind::Immediate) {
        const Argument& seg = args[hi];
        Va base = 0;
        if (seg.kind == Argument::Kind::Register && seg.reg == "ds") base = img.data_segment();
        else if (seg.kind == Argument::Kind::Immediate && img.span(seg.value * 65536) != nullptr) base = seg.value * 65536;
        else return std::nullopt;
        return entry(base + args[lo].value);
    }
    return std::nullopt;
}

Formats find_formats(Image& img, const call_graph::Graph& graph, const Table& table) {
    Formats out;
    struct Site {
        Va function, site, address;
    };
    using Key = std::pair<Va, int>;
    std::vector<Key> candidate_order;
    std::map<Key, std::vector<Site>> candidates;
    for (Va f : std::vector<Va>(img.functions())) {
        const Function& fn = img.function(f);
        std::unordered_map<Va, int> index;
        for (std::size_t k = 0; k < fn.insns.size(); ++k) index.emplace(fn.insns[k].addr, static_cast<int>(k));
        const auto calls = graph.calls.find(f);
        if (calls == graph.calls.end()) continue;
        for (const call_graph::Call& c : calls->second) {
            if (!c.destination) continue;
            const auto at = index.find(c.site);
            const int i = at == index.end() ? 0 : at->second;
            if (img.is_import(*c.destination)) {
                const std::string& api = img.import_name(*c.destination);
                const std::optional<FormatterInfo> info = formatter(api);
                if (!info) continue;
                ++out.sites;
                const auto args = arguments(fn, i, img.bits() == 16 ? 2 * static_cast<std::size_t>(info->position + 1)
                                                                      : static_cast<std::size_t>(info->position + 1));
                if (const auto lit = literal(img, table, args, static_cast<std::size_t>(info->position)))
                    out.calls.push_back({f, c.site, api, true, lit->entry->text,
                                         info->wide || lit->entry->encoding == Encoding::Wide, lists_arguments(api)});
            } else if (c.kind == call_graph::CallKind::Call && img.bits() == 32) {
                // a function of the module's own that is handed a literal with conversions in it
                const auto args = arguments(fn, i, 4);
                for (std::size_t k = 0; k < args.size(); ++k) {
                    if (args[k].kind != Argument::Kind::Immediate) continue;
                    const auto it = table.find(args[k].value);
                    if (it == table.end() || scan_conversions(text::replace_all(it->second.text, "%%", "")).empty()) continue;
                    const Key key{*c.destination, static_cast<int>(k)};
                    auto found = candidates.find(key);
                    if (found == candidates.end()) {
                        candidate_order.push_back(key);
                        found = candidates.emplace(key, std::vector<Site>{}).first;
                    }
                    found->second.push_back({f, c.site, args[k].value});
                }
            }
        }
    }
    // believed when the function is handed such literals from several places, and not always the same one
    for (const Key& key : candidate_order) {
        const std::vector<Site>& sites = candidates[key];
        std::set<Va> distinct;
        for (const Site& s : sites) distinct.insert(s.address);
        if (sites.size() < 2 || distinct.size() < 2) continue;
        out.own[key.first] = key.second;
        const std::string label = img.label(key.first);
        for (const Site& s : sites) {
            const Entry& e = table.at(s.address);
            out.calls.push_back({s.function, s.site, label, false, e.text, e.encoding == Encoding::Wide, true});
        }
    }
    return out;
}

// ---- the report ------------------------------------------------------------------------------------------------

namespace {

// A string for a summary line: in double quotes, with backslash, quote and anything not printable ASCII escaped.
std::string quote_text(std::string_view s) {
    std::string out = "\"";
    for (unsigned char c : s) {
        if (c == '"' || c == '\\') {
            out += '\\';
            out += static_cast<char>(c);
        } else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (c < 0x20 || c >= 0x7F) out += std::format("\\x{:02x}", c);
        else out += static_cast<char>(c);
    }
    return out + "\"";
}

}  // namespace

std::vector<FormatFact> format_facts(const Formats& formats) {
    std::vector<FormatFact> out;
    for (const FormatCall& c : formats.calls) {
        FormatFact f{c.function, c.site, c.api, c.imported, c.format, std::nullopt};
        if (c.lists_arguments) f.arguments = conversions(c.format, c.wide);
        out.push_back(std::move(f));
    }
    return out;
}

Report make_report(const Table& table, const std::vector<FunctionUses>& uses, const std::vector<StackString>& stack,
                   const std::vector<FormatFact>& formats, const std::map<Va, int>& own, std::int64_t formatter_sites,
                   const std::function<std::string(Va)>& label) {
    Report report;
    for (const auto& [va, e] : table) report.strings.push_back({va, e.text, e.encoding == Encoding::Wide, e.referenced});
    std::vector<const FunctionUses*> by_function;
    for (const FunctionUses& u : uses) by_function.push_back(&u);
    std::sort(by_function.begin(), by_function.end(), [](const auto* a, const auto* b) { return a->function < b->function; });
    for (const FunctionUses* u : by_function) {
        std::set<std::string> texts;
        for (const Use& use : u->uses) texts.insert(use.text);
        report.string_uses.push_back({u->function, {texts.begin(), texts.end()}});
    }
    report.stack_strings = stack;
    report.formats = formats;
    for (const auto& [function, position] : own) report.printf_like.push_back({function, position});

    Counts& c = report.counts;
    c.strings = static_cast<std::int64_t>(table.size());
    for (const auto& [va, e] : table) {
        if (e.referenced) ++c.referenced;
        if (e.encoding == Encoding::Wide) ++c.wide;
    }
    c.functions_using_strings = static_cast<std::int64_t>(uses.size());
    for (const FunctionUses& u : uses) c.string_uses += static_cast<std::int64_t>(u.uses.size());
    c.stack_strings = static_cast<std::int64_t>(stack.size());
    c.formatter_call_sites = formatter_sites;
    for (const FormatFact& f : report.formats) {
        if (f.imported) ++c.format_strings_read;
        else ++c.format_strings_at_own_functions;
        if (f.arguments) c.arguments_typed_by_formats += static_cast<std::int64_t>(f.arguments->size());
    }
    c.own_printf_like_functions = static_cast<std::int64_t>(own.size());

    // a short summary
    for (std::size_t i = 0; i < std::min<std::size_t>(stack.size(), 8); ++i)
        report.text.push_back(std::format("stack string in {} at {}: {}", label(stack[i].function), stack[i].where,
                                          quote_text(stack[i].text)));
    for (std::size_t i = 0; i < std::min<std::size_t>(report.formats.size(), 10); ++i) {
        const FormatFact& f = report.formats[i];
        const std::string args = f.arguments && !f.arguments->empty() ? text::join(*f.arguments, ", ") : "(argument list elsewhere)";
        report.text.push_back(std::format("{}({}) in {} -> {}", after_last_bang(f.api), quote_text(f.format.substr(0, 50)),
                                          label(f.function), args));
    }
    std::size_t shown = 0;
    for (const PrintfLikeFact& p : report.printf_like) {
        if (shown++ >= 8) break;
        report.text.push_back(std::format("printf-like: {}, format is argument {}", label(p.function), p.format_argument));
    }
    return report;
}

Report analyse(Image& img, const call_graph::Graph& graph) {
    Table table = build_table(img, graph);
    const std::vector<FunctionUses> uses = find_uses(img, graph, table);   // (marks 16-bit strings referenced)
    const std::vector<StackString> stack = stack_strings(img);
    const Formats formats = find_formats(img, graph, table);
    return make_report(table, uses, stack, format_facts(formats), formats.own, formats.sites, [&img](Va va) { return img.label(va); });
}


}  // namespace cb::analyze::strings

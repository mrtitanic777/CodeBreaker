#include "cb/port/asmdata.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <tuple>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::port::asmdata {

namespace {

using asm16::Asm16Error;

// A DW/DD item: (displacement, nullopt) for a number, (displacement, symbol) for a name and what was added to it.
struct ItemValue {
    std::int64_t value = 0;
    std::optional<std::string> symbol;
};

ItemValue ad_value(std::string_view raw, const std::map<std::string, std::string>& equ) {
    static const Regex offset_word(R"((?i)^offset\s+(?:\w+:)?)");
    static const Regex named(R"(([A-Za-z_$@?][\w$@?]*)\s*(?:([+-])\s*(\w+))?)");
    const std::string v = offset_word.replace_first(text::trim(raw), "");
    if (const auto n = asm16::number(v)) return {*n, std::nullopt};
    if (const auto e = equ.find(v); e != equ.end())
        if (const auto n = asm16::number(e->second)) return {*n, std::nullopt};
    if (const auto m = named.full_match(v)) {
        std::int64_t d = 0;
        if (m->has(3)) {
            const auto n = asm16::number(m->group(3));
            if (!n) throw Asm16Error("a data item not understood: " + v);   // (sym+text that is no number)
            d = *n;
        }
        return {m->has(2) && m->group(2) == "-" ? -d : d, std::string(m->group(1))};
    }
    throw Asm16Error("a data item not understood: " + v);
}

// A DB item: its bytes.
Bytes ad_bytes(std::string_view raw) {
    const std::string_view item = text::trim(raw);
    if (item.size() >= 2 && item.front() == item.back() && (item.front() == '\'' || item.front() == '"')) {
        const char q = item.front();
        Bytes out;
        for (std::size_t i = 1; i + 1 < item.size(); ++i) {
            out.push_back(static_cast<std::uint8_t>(item[i]));
            if (item[i] == q && i + 2 < item.size() && item[i + 1] == q) ++i;   // (a doubled quote is one)
        }
        return out;
    }
    const auto n = asm16::number(item);
    if (!n) throw Asm16Error("a byte not understood: " + std::string(item));
    return Bytes{static_cast<std::uint8_t>(*n & 0xFF)};
}

// `value` as `unit` little-endian bytes (the low ones, as a two's complement number of that width).
void ad_append_word(Bytes& out, std::int64_t value, std::size_t unit, std::size_t times = 1) {
    for (std::size_t t = 0; t < times; ++t)
        for (std::size_t i = 0; i < unit; ++i) out.push_back(i < 8 ? static_cast<std::uint8_t>((static_cast<std::uint64_t>(value) >> (8 * i)) & 0xFF) : (value < 0 ? 0xFF : 0));
}

// The `n` bytes at `at` as one unsigned little-endian number (the bytes past the segment's end are not there).
std::uint64_t ad_word(const Segment& s, std::size_t at, std::size_t n) {
    if (n > 8) throw Asm16Error("a data word of more than 8 bytes");
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < n && at + i < s.data.size(); ++i) v |= static_cast<std::uint64_t>(s.data[at + i]) << (8 * i);
    return v;
}

std::int64_t ad_signed(std::uint64_t v, std::size_t n) {
    if (n >= 8) return static_cast<std::int64_t>(v);
    const std::uint64_t half = std::uint64_t{1} << (8 * n - 1);
    return v >= half ? static_cast<std::int64_t>(v) - static_cast<std::int64_t>(half << 1) : static_cast<std::int64_t>(v);
}

std::optional<std::size_t> ad_base_size(std::string_view type) {
    static const std::map<std::string, std::size_t, std::less<>> sizes = {
        {"char", 1},  {"signed char", 1}, {"unsigned char", 1}, {"short", 2},     {"int", 2},      {"unsigned", 2},
        {"unsigned int", 2}, {"unsigned short", 2}, {"int16_t", 2}, {"uint16_t", 2}, {"long", 4}, {"unsigned long", 4},
        {"int32_t", 4}, {"uint32_t", 4}, {"int8_t", 1}, {"uint8_t", 1}};
    const auto it = sizes.find(type);
    if (it == sizes.end()) return std::nullopt;
    return it->second;
}

bool has_bracket(std::string_view s) { return s.find('[') != std::string_view::npos; }

std::string ad_collapse(std::string_view s) { return std::string(text::join(text::split_ws(s), " ")); }

std::size_t ad_stars(std::string_view declarator, std::string_view name) {
    std::string d(declarator);
    if (const std::size_t at = d.find(name); at != std::string::npos && !name.empty()) d.erase(at, name.size());
    return static_cast<std::size_t>(std::count(d.begin(), d.end(), '*'));
}

}  // namespace

Image image(std::string_view text_in) {
    Image segs;
    Segment* cur = nullptr;
    std::map<std::string, std::string> equ;
    static const Regex quoted(R"('([^']*)')");
    static const Regex dup(R"((?i)\s*(\w+)\s+DUP\s*\((.*)\)\s*$)");
    for (const asm16::Line& l : asm16::parse(text_in)) {
        if (l.label && cur) cur->labels.insert_or_assign(*l.label, cur->data.size());
        if (!l.directive) continue;
        const std::string& kind = l.directive->kind;
        const std::string name = l.directive->name.value_or("");
        const std::string rest(text::trim(asm16::strip_comment(l.directive->rest)));
        if (kind == "SEGMENT") {
            const auto m = quoted.search(rest);
            cur = &segs.try_emplace(name).first->second;
            if (cur->name.empty()) {
                cur->name = name;
                if (m) cur->cls = std::string(m->group(1));
            } else if (m && (!cur->cls || cur->cls->empty())) {
                cur->cls = std::string(m->group(1));
            }
            continue;
        }
        if (kind == "ENDS") {
            cur = nullptr;
            continue;
        }
        if (kind == "EQU" || kind == "=") {
            equ[name] = rest;
            continue;
        }
        if (!cur) continue;
        if (kind == "LABEL") {
            cur->labels.insert_or_assign(name, cur->data.size());
            continue;
        }
        if (kind != "DB" && kind != "DW" && kind != "DD") continue;
        if (!name.empty()) cur->labels.insert_or_assign(name, cur->data.size());
        const std::size_t unit = kind == "DB" ? 1 : kind == "DW" ? 2 : 4;
        if (const auto m = dup.match(rest)) {
            const auto n = asm16::number(m->group(1));
            if (!n || *n < 0) throw Asm16Error("a DUP count not understood: " + std::string(m->group(1)));
            const std::string_view v = text::trim(m->group(2));
            const std::int64_t fill = v == "?" ? 0 : asm16::number(v).value_or(0);
            const std::size_t at = cur->data.size();
            ad_append_word(cur->data, fill, unit, static_cast<std::size_t>(*n));
            cur->items.push_back({at, unit * static_cast<std::size_t>(*n)});
            continue;
        }
        for (const std::string& item : asm16::split_ops(rest)) {
            const std::size_t at = cur->data.size();
            if (unit == 1) {
                const Bytes b = ad_bytes(item);
                cur->data.insert(cur->data.end(), b.begin(), b.end());
            } else {
                ItemValue v = ad_value(item, equ);
                std::int64_t word = v.value;
                if (v.symbol) {
                    cur->reloc[at] = {*v.symbol, v.value};
                    word = 0;
                }
                ad_append_word(cur->data, word, unit);
            }
            cur->items.push_back({at, cur->data.size() - at});
        }
    }
    return segs;
}

std::string c_string(ByteView bytes) {
    std::string out;
    for (const std::uint8_t c : bytes) {
        if (c == '\\') out += "\\\\";
        else if (c == '"') out += "\\\"";
        else if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\r') out += "\\r";
        else if (c >= 32 && c < 127) out.push_back(static_cast<char>(c));
        else out += std::format("\\{:03o}", c);
    }
    static const Regex octal_then_digit(R"((\\[0-7]{3})(?=[0-7]))");
    return "\"" + octal_then_digit.replace_all(out, "${1}\"\"") + "\"";
}

std::set<std::string> publics(std::string_view text_in) {
    std::set<std::string> out;
    for (const asm16::Line& l : asm16::parse(text_in)) {
        if (!l.directive || l.directive->kind != "PUBLIC") continue;
        for (std::string_view x : text::split(l.directive->rest, ',')) {
            const std::string_view t = text::trim(x);
            if (!t.empty()) out.emplace(t);
        }
    }
    return out;
}

std::optional<TypedData> typed(std::string_view text_in, const asm16::CTypes& types) {
    Image segs = image(text_in);
    bool any_reloc = false;
    for (const auto& kv : segs) any_reloc = any_reloc || !kv.second.reloc.empty();
    if (!any_reloc) return std::nullopt;
    struct Place {
        const Segment* seg;
        std::size_t offset;
    };
    std::map<std::string, Place> where;
    for (const auto& kv : segs)
        for (const auto& [label, offset] : kv.second.labels) where[label] = {&kv.second, offset};
    const std::set<std::string> pubs = publics(text_in);
    auto cname = [&](const std::string& n) { return n.starts_with('_') && pubs.contains(n) ? n.substr(1) : n; };
    std::vector<std::string> notes;

    auto osize = [&](std::string_view raw) -> std::optional<std::size_t> {
        const std::string t = ad_collapse(raw);
        if (t.ends_with('*')) return 2;
        if (t.starts_with("struct ") || t.starts_with("union ")) {
            const pre_ansi::StructLayout* lay = types.old_layouts.find(t);
            if (!lay) return std::nullopt;
            return lay->size;
        }
        std::string_view base = t;
        for (std::string_view prefix : {"signed ", "const "})
            if (base.starts_with(prefix)) {
                base.remove_prefix(prefix.size());
                break;
            }
        return ad_base_size(base);
    };

    std::map<std::string, std::string> kinds;   // label: the type of its elements
    std::vector<std::string> assigned;          // the labels in the order they were typed
    std::vector<std::string> queue;
    auto assign = [&](const std::string& sym, std::string_view raw, const std::string& why) {
        const std::string t = ad_collapse(raw);
        if (!where.contains(sym)) {
            notes.push_back(std::format("{}: not in the module ({})", sym, why));
            return;
        }
        if (const auto it = kinds.find(sym); it != kinds.end()) {
            if (it->second != t) notes.push_back(std::format("{}: {} and {} ({})", sym, it->second, t, why));
            return;
        }
        kinds[sym] = t;
        assigned.push_back(sym);
        queue.push_back(sym);
    };

    for (const std::string& p : pubs) {
        const asm16::VarType* v = types.var(cname(p));
        if (!v || !where.contains(p)) continue;
        static const Regex dimension(R"(\[[^\]]*\])");
        const std::size_t stars = ad_stars(dimension.replace_all(v->declarator, ""), cname(p));
        assign(p, std::string(text::trim(v->base)) + (stars ? " " + std::string(stars, '*') : ""), "declared");
    }

    auto extent = [&](const std::string& sym) {   // (the segment, the table's offset, where the next label is)
        const Place& w = where.at(sym);
        std::size_t next = w.seg->data.size();
        for (const auto& [label, offset] : w.seg->labels)
            if (offset > w.offset) next = std::min(next, offset);
        return std::tuple<const Segment*, std::size_t, std::size_t>{w.seg, w.offset, next};
    };

    // what the element of type t at s+at points to: typed
    auto walk = [&](const std::string& t, const Segment& s, std::size_t at) {
        if (t.ends_with('*')) {
            if (const auto r = s.reloc.find(at); r != s.reloc.end()) assign(r->second.symbol, std::string_view(t).substr(0, t.size() - 1), "pointed to");
            return;
        }
        const pre_ansi::StructLayout* lay = types.old_layouts.find(t);
        if (!lay) return;
        for (const pre_ansi::Member& m : lay->members) {
            if (!m.pointer) continue;
            const std::size_t stars = ad_stars(m.declarator, m.name);
            if (const auto r = s.reloc.find(at + m.offset); r != s.reloc.end())
                assign(r->second.symbol, m.type + (stars > 1 ? " " + std::string(stars - 1, '*') : ""), t + "." + m.name);
        }
    };
    while (!queue.empty()) {
        const std::string sym = queue.back();
        queue.pop_back();
        const std::string t = kinds.at(sym);
        const auto size = osize(t);
        if (!size || *size == 0) {
            notes.push_back(std::format("{}: no size for {}", sym, t));
            continue;
        }
        const auto [s, o, end] = extent(sym);
        const std::size_t count = std::max<std::size_t>(1, (end - o) / *size);
        for (std::size_t k = 0; k < count; ++k) walk(t, *s, o + k * *size);
    }

    auto reference = [&](const Reloc& r, const std::string& t) -> std::optional<std::string> {
        const auto u = kinds.find(r.symbol);
        if (u == kinds.end()) return std::nullopt;
        const std::int64_t us = static_cast<std::int64_t>(osize(u->second).value_or(1));
        const std::string name = cname(r.symbol);
        if (r.displacement % us == 0) return std::format("&{}[{}]", name, r.displacement / us);   // (&name[0] for none)
        return std::format("({})((char *){} + {})", t, name, r.displacement);   // (into the middle of an element)
    };

    std::function<std::string(const std::string&, const Segment&, std::size_t)> value = [&](const std::string& t, const Segment& s, std::size_t at) -> std::string {
        if (t.ends_with('*')) {
            if (const auto r = s.reloc.find(at); r != s.reloc.end()) {
                if (const auto v = reference(r->second, t)) return *v;
                notes.push_back(std::format("{}+{}: {}, not typed", s.name, at, r->second.symbol));
                return "0";
            }
            const std::uint64_t n = ad_word(s, at, 2);
            return n == 0 ? "0" : std::format("({}){}", t, n);
        }
        if (const pre_ansi::StructLayout* lay = types.old_layouts.find(t)) {
            std::vector<std::string> vals;
            for (const pre_ansi::Member& m : lay->members) {
                if (m.pointer) {
                    const std::size_t stars = ad_stars(m.declarator, m.name);
                    vals.push_back(value(m.type + " " + std::string(stars, '*'), s, at + m.offset));
                } else if (has_bracket(m.declarator)) {
                    const std::size_t n = osize(m.type).value_or(1);
                    std::vector<std::string> elems;
                    for (std::size_t j = 0; j < m.size / n; ++j) elems.push_back(std::to_string(ad_word(s, at + m.offset + j * n, n)));
                    vals.push_back("{ " + text::join(elems, ", ") + " }");
                } else {
                    const std::uint64_t n = ad_word(s, at + m.offset, m.size);
                    const bool signed_member = (m.size == 1 || m.size == 2 || m.size == 4) && !m.type.starts_with("unsigned");
                    vals.push_back(signed_member ? std::to_string(ad_signed(n, m.size)) : std::to_string(n));
                }
            }
            return "{ " + text::join(vals, ", ") + " }";
        }
        const std::size_t n = osize(t).value_or(1);
        const std::uint64_t v = ad_word(s, at, n);
        const bool signed_type = (n == 1 || n == 2 || n == 4) && !t.starts_with("unsigned");
        return signed_type ? std::to_string(ad_signed(v, n)) : std::to_string(v);
    };

    std::vector<std::string> order = assigned;
    std::stable_sort(order.begin(), order.end(), [&](const std::string& a, const std::string& b) {
        return std::make_pair(where.at(a).seg->name, where.at(a).offset) < std::make_pair(where.at(b).seg->name, where.at(b).offset);
    });
    std::vector<std::string> decls, defs;
    for (const std::string& sym : order) {
        const std::string& t = kinds.at(sym);
        const auto size = osize(t);
        if (!size || *size == 0) continue;   // (noted above: no size to lay a table out by)
        const auto [s, o, end] = extent(sym);
        const std::size_t count = std::max<std::size_t>(1, (end - o) / *size);
        const std::string name = cname(sym);
        const asm16::VarType* v = pubs.contains(sym) ? types.var(name) : nullptr;
        const bool array = !v || has_bracket(v->declarator);
        const std::string storage = pubs.contains(sym) ? "" : "static ";
        const std::string sep = t.ends_with('*') ? "" : " ";
        const std::string dim = array ? std::format("[{}]", count) : "";
        decls.push_back(storage + t + sep + name + dim + ";");
        if (t == "char" && array) {
            ByteView b(s->data.data() + std::min(o, s->data.size()), std::min(count, s->data.size() - std::min(o, s->data.size())));
            ByteView text_bytes = !b.empty() && b.back() == 0 ? b.first(b.size() - 1) : b;
            if (std::find(text_bytes.begin(), text_bytes.end(), std::uint8_t{0}) == text_bytes.end()) {
                defs.push_back(storage + t + " " + name + dim + " = " + c_string(text_bytes) + ";");
                continue;
            }
        }
        if (array) {
            std::vector<std::string> items;
            for (std::size_t k = 0; k < count; ++k) items.push_back(value(t, *s, o + k * *size));
            defs.push_back(storage + t + sep + name + dim + " = {\n\t" + text::join(items, ",\n\t") + "\n};");
        } else {
            defs.push_back(storage + t + sep + name + " = " + value(t, *s, o) + ";");
        }
    }
    std::vector<std::string> head = {"/* the tables of an assembler module, with the addresses in them (made by CodeBreaker's port: typed by the C program's declarations and what points where) */"};
    for (std::size_t i = 0; i < notes.size() && i < 20; ++i) head.push_back("/* (not typed: " + notes[i] + ") */");
    std::vector<std::string> all = head;
    all.insert(all.end(), decls.begin(), decls.end());
    all.push_back("");
    all.insert(all.end(), defs.begin(), defs.end());
    return TypedData{text::join(all, "\n"), notes};
}

std::optional<std::string> messages(std::string_view text_in) {
    const Image segs = image(text_in);
    std::map<std::string, std::vector<const Segment*>> tables = {{"MSG", {}}, {"FAR_MSG", {}}};
    for (const auto& kv : segs)
        if (kv.second.cls && tables.contains(*kv.second.cls) && !kv.second.data.empty()) tables[*kv.second.cls].push_back(&kv.second);
    if (tables["MSG"].empty() && tables["FAR_MSG"].empty()) return std::nullopt;
    std::vector<std::string> out = {"/* the numbered messages of the C library's __NMSG_TEXT (near) and __FMSG_TEXT (far) */"};
    for (const auto& [cls, name] : {std::pair<std::string, std::string>{"MSG", "dos_nmsgs"}, {"FAR_MSG", "dos_fmsgs"}}) {
        out.push_back(std::format("static const struct {{ int16_t num; const char *text; }} {}[] = {{", name));
        for (const Segment* sg : tables[cls]) {
            const Bytes& b = sg->data;
            std::size_t i = 0;
            while (i + 2 <= b.size()) {
                const unsigned num = b[i] | (static_cast<unsigned>(b[i + 1]) << 8);
                if (num == 0xFFFF) break;
                const auto zero = std::find(b.begin() + static_cast<std::ptrdiff_t>(i + 2), b.end(), std::uint8_t{0});
                const std::size_t j = static_cast<std::size_t>(zero - b.begin());
                out.push_back(std::format("\t{{ {}, {} }},", num, c_string(ByteView(b.data() + i + 2, j - (i + 2)))));
                i = j + 1;
            }
        }
        out.insert(out.end(), {"\t{ -1, 0 }", "};", ""});
    }
    for (const auto& [fn, name] : {std::pair<std::string, std::string>{"__NMSG_TEXT", "dos_nmsgs"}, {"__FMSG_TEXT", "dos_fmsgs"}}) {
        out.push_back(std::format("char *{}(int32_t n)\t\t/* (an int: as the callers give it) */", fn));
        out.insert(out.end(), {"{", "\tint32_t i;", std::format("\tfor (i = 0; {}[i].text; i++)", name),
                               std::format("\t\tif ({}[i].num == n) return (char *){}[i].text;", name, name), "\treturn (char *)\"\";", "}", ""});
    }
    return text::join(out, "\n");
}

}  // namespace cb::port::asmdata

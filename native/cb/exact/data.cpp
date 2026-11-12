#include "cb/exact/data.hpp"

#include <algorithm>
#include <format>
#include <unordered_map>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/bytesearch.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::data {

namespace stdfs = std::filesystem;
namespace omf = cb::formats::omf;

namespace {

// Structs are laid out as the compiler of the period laid them out: members on the alignment of their size, up to a word.
constexpr std::size_t kStructAlign = 2;
constexpr std::size_t kUnknownSize = 2;   // what a type the table does not know takes
constexpr std::string_view kNothingNames = "                 /* (nothing names it) */";

bool is_word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }
bool is_ascii_text(std::uint8_t c) { return c >= 32 && c < 127; }
bool has(std::string_view s, std::string_view what) { return s.find(what) != std::string_view::npos; }

std::string without_spaces(std::string_view s) {
    std::string out;
    for (char c : s) {
        if (c != ' ') out.push_back(c);
    }
    return out;
}

// The text with each /* comment */ replaced; one with no end is left (and what follows it).
std::string replace_comments(std::string_view s, std::string_view replacement) {
    std::string out;
    std::size_t at = 0;
    while (at < s.size()) {
        const std::size_t open = s.find("/*", at);
        const std::size_t close = open == std::string_view::npos ? open : s.find("*/", open + 2);
        if (close == std::string_view::npos) break;
        out.append(s.substr(at, open - at));
        out.append(replacement);
        at = close + 2;
    }
    out.append(s.substr(at));
    return out;
}

// The words ([A-Za-z0-9_] runs) of a text.
std::vector<std::string_view> words_of(std::string_view s) {
    std::vector<std::string_view> out;
    for (std::size_t i = 0; i < s.size();) {
        if (!is_word_char(s[i])) {
            ++i;
            continue;
        }
        std::size_t e = i;
        while (e < s.size() && is_word_char(s[e])) ++e;
        out.push_back(s.substr(i, e - i));
        i = e;
    }
    return out;
}

std::uint8_t byte_at(ByteView ds, std::size_t at) {
    if (at >= ds.size()) throw TruncatedError("data segment", at, 1, 0);
    return ds[at];
}

ByteView slice_of(ByteView ds, std::size_t at, std::size_t len) {
    if (at > ds.size() || len > ds.size() - at) throw TruncatedError("data segment", at, len, at < ds.size() ? ds.size() - at : 0);
    return ds.subspan(at, len);
}

std::string_view lstrip_underscores(std::string_view s) {
    while (s.starts_with('_')) s.remove_prefix(1);
    return s;
}

std::uint32_t word_at(ByteView ds, std::size_t at) {
    if (at + 2 > ds.size()) throw TruncatedError("data segment", at, 2, ds.size() > at ? ds.size() - at : 0);
    return static_cast<std::uint32_t>(ds[at] | (ds[at + 1] << 8));
}

std::string tail_of(const std::string& console) { return console.size() > 1500 ? console.substr(console.size() - 1500) : console; }

// The size of a base type the table knows.
std::optional<std::size_t> scalar_size(std::string_view base) {
    static const std::map<std::string, std::size_t, std::less<>> sizes = {
        {"char", 1}, {"unsigned char", 1}, {"int", 2}, {"unsigned", 2}, {"unsigned int", 2}, {"short", 2},
        {"unsigned short", 2}, {"long", 4}, {"unsigned long", 4}, {"float", 4}, {"double", 8}};
    const auto it = sizes.find(base);
    return it == sizes.end() ? std::nullopt : std::optional<std::size_t>(it->second);
}

struct Layout {
    std::size_t size = 0;
    std::size_t align = 1;
};

Layout struct_layout(const std::string& name, const Structs& structs);

// The size and alignment of one declared thing: a pointer is a word, a struct its layout, a scalar its size.
Layout layout_of(const std::string& base, unsigned stars, const std::vector<std::uint32_t>& dims, const Structs& structs) {
    Layout l;
    if (stars != 0) {
        l = {2, 2};
    } else if (base.starts_with("struct ")) {
        l = struct_layout(base.substr(7), structs);
    } else {
        l.size = scalar_size(base).value_or(kUnknownSize);
        l.align = std::min(l.size, kStructAlign);
    }
    for (std::uint32_t d : dims) l.size *= d;
    return l;
}

Layout struct_layout(const std::string& name, const Structs& structs) {
    std::size_t pos = 0, top = 1;
    if (const auto it = structs.find(name); it != structs.end()) {
        for (const Field& f : it->second) {
            const Layout l = layout_of(f.base, f.stars, f.dims, structs);
            const std::size_t align = std::max<std::size_t>(l.align, 1);
            pos += (align - pos % align) % align;
            pos += l.size;
            top = std::max(top, l.align);
        }
    }
    return {pos + (top - pos % top) % top, top};
}

}  // namespace

std::string c_string(ByteView bytes) {
    std::string out = "\"";
    for (std::uint8_t c : bytes) {
        switch (c) {
            case '\n': out += "\\n"; break;
            case '\t': out += "\\t"; break;
            case '\r': out += "\\r"; break;
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            default:
                if (is_ascii_text(c)) out.push_back(static_cast<char>(c));
                else out += std::format("\\{:03o}", c);
        }
    }
    out.push_back('"');
    return out;
}

// ---- C declarations ------------------------------------------------------------------------------------------

namespace {

// The name an item of an extern line declares: the first word followed by "[" or the end ("tab[4]", "*p", "(*fn[2])()").
std::optional<std::string> declared_name(std::string_view item) {
    item = text::trim(item);
    while (item.starts_with('*')) item.remove_prefix(1);
    for (std::size_t i = 0; i < item.size();) {
        if (!is_word_char(item[i])) {
            ++i;
            continue;
        }
        std::size_t e = i;
        while (e < item.size() && is_word_char(item[e])) ++e;
        std::size_t after = e;
        while (after < item.size() && text::is_space(item[after])) ++after;
        if (after == item.size() || item[after] == '[') return std::string(item.substr(i, e - i));
        i = e;
    }
    return std::nullopt;
}

}  // namespace

Declarations externs(const stdfs::path& src, const std::optional<std::vector<stdfs::path>>& files) {
    static const Regex extern_line(R"re(extern\s+(unsigned\s+\w+|unsigned|struct\s+\w+|union\s+\w+|\w+)\s+(.*))re");
    std::vector<stdfs::path> paths;
    if (files) {
        paths = *files;
    } else {
        for (const auto& e : stdfs::directory_iterator(src)) {
            const std::string name = e.path().filename().string();
            if (name.size() > 2 && name[0] == 'f' && text::iends_with(name, ".c")) paths.push_back(e.path());
        }
        std::sort(paths.begin(), paths.end());
    }
    Declarations decl;
    for (const stdfs::path& p : paths) {
        const std::string content = fs::read_text(p);
        for (std::string_view raw : text::lines(content)) {
            if (!raw.starts_with("extern")) continue;
            std::string line(text::trim(replace_comments(raw, "")));
            while (line.ends_with(';')) line.pop_back();
            const auto m = extern_line.match(line);
            if (!m) continue;
            const std::string_view rest = m->group(2);
            if (has(rest, "(") && !has(without_spaces(rest), "(*")) continue;   // (a function; a pointer to one is data)
            const std::string type(text::trim(m->group(1)));
            for (std::string_view item : text::split(rest, ',')) {
                if (const auto name = declared_name(item)) decl[*name].insert({type, std::string(text::trim(item))});
            }
        }
    }
    return decl;
}

StructTable parse_structs(const modules::Project& project) {
    static const Regex struct_block(R"re(struct\s+(\w+)\s*\{([^{}]*)\}\s*;)re");
    static const Regex field_decl(R"re(((?:unsigned\s+)?(?:struct\s+\w+|union\s+\w+|\w+))\s+(.*))re");
    static const Regex field_name(R"re(\**\s*(\w+))re");
    std::vector<std::pair<std::string, std::string>> texts;   // (header's name, its text as bytes)
    for (const auto& [name, bytes] : project.headers()) texts.emplace_back(name, std::string(as_text(bytes)));
    for (const std::string& dir : project.include()) {
        std::vector<stdfs::path> found;
        std::error_code ec;
        for (const auto& e : stdfs::directory_iterator(fs::from_text(dir), ec)) {
            if (text::iends_with(e.path().filename().string(), ".h")) found.push_back(e.path());
        }
        std::sort(found.begin(), found.end());
        for (const stdfs::path& p : found) texts.emplace_back(p.filename().string(), fs::read_text(p));
    }
    StructTable out;
    for (const auto& [header, raw] : texts) {
        const std::string t = replace_comments(raw, " ");
        struct_block.for_each_match(t, [&](const RegexMatch& m) {
            std::vector<Field> fields;
            for (std::string_view piece : text::split(m.group(2), ';')) {
                const std::string decl = text::join(text::split_ws(piece), " ");
                if (decl.empty()) continue;
                const auto dm = field_decl.match(decl);
                if (!dm) continue;
                const std::string base(dm->group(1));
                for (std::string_view raw_item : text::split(dm->group(2), ',')) {
                    const std::string_view item = text::trim(raw_item);
                    Field f;
                    f.base = base;
                    while (f.stars < item.size() && item[f.stars] == '*') ++f.stars;
                    const auto nm = field_name.match(item);
                    if (!nm) continue;
                    f.name = std::string(nm->group(1));
                    for (std::size_t i = 0; i < item.size(); ++i) {   // each [WORD] that is a number
                        if (item[i] != '[') continue;
                        std::size_t e = i + 1;
                        while (e < item.size() && is_word_char(item[e])) ++e;
                        if (e == i + 1 || e >= item.size() || item[e] != ']') continue;
                        const std::string_view dim = item.substr(i + 1, e - i - 1);
                        if (const auto v = text::parse_int<std::uint32_t>(dim, 0)) f.dims.push_back(*v);
                    }
                    fields.push_back(std::move(f));
                }
            }
            const std::string name(m.group(1));
            out.structs[name] = std::move(fields);
            out.home.try_emplace(name, header);
            return true;
        });
    }
    return out;
}

std::optional<std::uint32_t> declared_size(const std::set<Declaration>& declarations, const Structs& structs) {
    for (const Declaration& d : declarations) {   // (sorted: a name declared more than one way answers the same every time)
        const std::string& i = d.declarator;
        if (has(i, "(") && !has(i, "(*")) continue;
        const unsigned stars = has(std::string_view(i).substr(0, i.find('[')), "*") ? 1 : 0;
        if (has(without_spaces(i), "[]")) continue;   // (an array of no count: its run says)
        std::vector<std::uint32_t> dims;
        for (std::size_t k = 0; k < i.size(); ++k) {
            if (i[k] != '[') continue;
            std::size_t e = k + 1;
            while (e < i.size() && i[e] >= '0' && i[e] <= '9') ++e;
            if (e > k + 1 && e < i.size() && i[e] == ']') dims.push_back(*text::parse_int<std::uint32_t>(std::string_view(i).substr(k + 1, e - k - 1)));
        }
        return static_cast<std::uint32_t>(layout_of(d.type, stars, dims, structs).size);
    }
    return std::nullopt;
}

// ---- where the module's data stands --------------------------------------------------------------------------

namespace {

ByteView data_segment(const modules::Project& project) {
    const ByteView image = project.image();
    const std::size_t dgroup = project.program().dgroup;
    if (dgroup > image.size()) throw DataError(std::format("the data segment (at {:x}) is past the end of the program ({:x} bytes)", dgroup, image.size()));
    return image.subspan(dgroup);
}

struct FunctionsAlone {
    omf::Object object;
    std::vector<std::uint32_t> addresses;
};

// The module's functions (its fADDR.c files) compiled together without its data files, with their addresses.
std::optional<FunctionsAlone> functions_alone(const modules::Project& project, const modules::Row& row, const modules::FunctionTypes& types) {
    std::vector<stdfs::path> paths;
    if (row.code_start) {
        for (const stdfs::path& q : modules::files_of(project.src(), *row.code_start, row.code_end.value_or(0xFFFFFFFFu))) {
            if (q.filename().string().starts_with('f')) paths.push_back(q);
        }
    }
    if (paths.empty()) return std::nullopt;
    const auto tc = project.toolchain();
    const std::string default_switches = project.switches().value_or(tc->profile("switches").value_or(""));
    const std::string tag = tc->profile("file_switches").value_or("");
    const modules::Combined combined = modules::combine(paths, default_switches, tag.empty() ? std::nullopt : std::optional<std::string_view>(tag), {}, types);
    const std::string flags = row.switches && !row.switches->empty() ? *row.switches
                              : !combined.switches.empty()           ? *combined.switches.begin()
                                                                     : default_switches;
    std::string source = combined.text;
    if (!tag.empty() && !flags.empty()) source = "/* " + tag + ": " + flags + " */\n" + source;
    const modules::CompileResult r = tc->compile(as_bytes(source), {project.headers(), project.include(), project.switches()});
    if (!r.object) throw DataError(std::format("{}'s functions do not compile alone:\n{}", row.name, tail_of(r.console)));
    FunctionsAlone out{omf::read(*r.object), {}};
    for (const stdfs::path& q : paths) {
        const std::string name = q.filename().string();
        const auto at = name.size() >= 6 ? text::parse_int<std::uint32_t>(name.substr(1, 5), 16) : std::nullopt;
        if (!at) throw DataError(name + ": the address in a function file's name is not hexadecimal");
        out.addresses.push_back(*at);
    }
    return out;
}

// The places of the literals, in order, in ds[lo, hi): each at an occurrence after the one before such that all the
// rest can still be found (an initializer's string can equal a function's literal and stand before it).
class Aligner {
public:
    Aligner(ByteView ds, const std::vector<ByteView>& lits, std::size_t hi) : ds_(ds), lits_(lits), hi_(hi) {}

    std::vector<std::optional<std::size_t>> places(std::size_t lo) {
        std::vector<std::optional<std::size_t>> out;
        std::size_t pos = lo;
        for (std::size_t i = 0; i < lits_.size(); ++i) {   // (the latest that lets the rest fit: a declaration's initializer
            std::optional<std::size_t> best;                //  strings stand before the next function's literals, and may equal them)
            for (std::size_t at = find_bytes(ds_, lits_[i], pos, hi_); at != ds_.size(); at = find_bytes(ds_, lits_[i], at + 1, hi_)) {
                if (fits(i + 1, at + lits_[i].size())) best = at;
            }
            out.push_back(best);
            if (best) pos = *best + lits_[i].size();
        }
        return out;
    }

private:
    // Whether literals i.. can all be found from pos on, each at its first occurrence after the one before. The chain
    // of first occurrences is followed to its end and every step on it remembered.
    bool fits(std::size_t i, std::size_t pos) {
        std::vector<std::uint64_t> path;
        bool result = true;
        while (i != lits_.size()) {
            const std::uint64_t key = (static_cast<std::uint64_t>(i) << 32) | pos;
            if (const auto it = memo_.find(key); it != memo_.end()) {
                result = it->second;
                break;
            }
            path.push_back(key);
            const std::size_t at = find_bytes(ds_, lits_[i], pos, hi_);
            if (at == ds_.size()) {
                result = false;
                break;
            }
            pos = at + lits_[i].size();
            ++i;
        }
        for (std::uint64_t key : path) memo_[key] = result;
        return result;
    }

    ByteView ds_;
    const std::vector<ByteView>& lits_;
    std::size_t hi_;
    std::unordered_map<std::uint64_t, bool> memo_;   // (literal, position) -> fits; lookup only
};

}  // namespace

Walk walk(const modules::Project& project, const modules::Row& row) {
    return walk(project, row, modules::FunctionTypes::read(project.src()));
}

Walk walk(const modules::Project& project, const modules::Row& row, const modules::FunctionTypes& types) {
    if (!row.data_start) throw DataError(row.name + ": src/MODULES gives it no data start");
    const std::uint32_t dstart = *row.data_start;
    const ByteView ds = data_segment(project);
    std::uint32_t dend = static_cast<std::uint32_t>(ds.size());
    for (const modules::Row& r : modules::rows(project)) {
        if (r.data_start && *r.data_start > dstart && *r.data_start < dend) dend = *r.data_start;
    }
    Walk out;
    out.data_start = dstart;
    out.data_end = dend;
    const auto got = functions_alone(project, row, types);
    if (!got) {
        out.runs.push_back({dstart, dend, std::string(kStart), RunKind::Vars});
        return out;
    }
    const omf::Object& o = got->object;
    const auto code_segments = omf::code_segments(o);
    if (code_segments.empty()) throw DataError(row.name + ": the functions compiled alone have no code segment");
    const std::string& text_segment = code_segments[0];
    const std::vector<omf::FunctionSpan> fns = omf::functions(o, text_segment);
    const Bytes* data_bytes = o.segments.find("_DATA");
    const std::uint64_t* data_filled = o.filled.find("_DATA");
    const ByteView lit = data_bytes ? ByteView(*data_bytes).first(std::min<std::size_t>(data_bytes->size(), data_filled ? *data_filled : 0)) : ByteView();
    // the function that names each literal first: the one whose code has a word that points at it
    std::map<std::uint32_t, std::string> owner;   // lookup only
    const Bytes* text_bytes = o.segments.find(text_segment);
    for (const omf::Fixup& fx : o.fixups) {
        if (fx.segment != text_segment || fx.kind != omf::FixupKind::Offset || fx.target.kind != omf::Target::Kind::Segment ||
            fx.target.name != "_DATA" || text_bytes == nullptr || fx.at < 0 || static_cast<std::size_t>(fx.at) + 2 > text_bytes->size())
            continue;
        const std::uint32_t target = static_cast<std::uint32_t>((word_at(*text_bytes, static_cast<std::size_t>(fx.at)) + fx.displacement) & 0xFFFF);
        for (const omf::FunctionSpan& f : fns) {
            if (f.offset <= fx.at && fx.at < f.offset + f.length) {
                owner.try_emplace(target, f.name);
                break;
            }
        }
    }
    for (std::size_t k = 0; k < lit.size();) {
        const auto nul = std::find(lit.begin() + static_cast<std::ptrdiff_t>(k), lit.end(), std::uint8_t(0));
        const std::size_t e = static_cast<std::size_t>(nul - lit.begin());
        const std::size_t stop = std::min(e + 1, lit.size());
        Literal l;
        l.offset = static_cast<std::uint32_t>(k);
        l.bytes.assign(lit.begin() + static_cast<std::ptrdiff_t>(k), lit.begin() + static_cast<std::ptrdiff_t>(stop));
        if (const auto it = owner.find(l.offset); it != owner.end()) l.function = it->second;
        out.literals.push_back(std::move(l));
        k = e + 1;
    }
    std::optional<std::string> current;   // a literal nothing names takes the function before it
    for (Literal& l : out.literals) {
        if (!l.function) l.function = current;
        else current = l.function;
    }
    std::vector<ByteView> lits;
    for (const Literal& l : out.literals) lits.emplace_back(l.bytes);
    const auto places = Aligner(ds, lits, dend).places(dstart);
    struct Gap {
        std::size_t lo, hi;
        std::optional<std::string> after;
    };
    std::vector<Gap> gaps;
    std::size_t pos = dstart;
    std::optional<std::string> last;
    for (std::size_t i = 0; i < out.literals.size(); ++i) {
        if (!places[i]) continue;   // (a literal the data does not hold: left out)
        if (*places[i] != pos) gaps.push_back({pos, *places[i], last});
        pos = *places[i] + out.literals[i].bytes.size();
        last = out.literals[i].function;
    }
    if (pos < dend) gaps.push_back({pos, dend, last});
    for (const Gap& g : gaps) {
        const ByteView seg = ds.subspan(g.lo, g.hi - g.lo);
        const std::size_t zeros = static_cast<std::size_t>(std::count(seg.begin(), seg.end(), std::uint8_t(0)));
        const bool is_text = std::all_of(seg.begin(), seg.end(), [](std::uint8_t c) { return is_ascii_text(c) || c == 0 || c == 9 || c == 10 || c == 13; }) &&
                             zeros <= std::max<std::size_t>(1, seg.size() / 6);
        const std::string after = g.after && !g.after->empty() ? *g.after : std::string(kStart);
        out.runs.push_back({static_cast<std::uint32_t>(g.lo), static_cast<std::uint32_t>(g.hi), after, is_text ? RunKind::Strings : RunKind::Vars});
    }
    for (std::size_t i = 0; i < fns.size() && i < got->addresses.size(); ++i) out.functions.push_back({fns[i].name, got->addresses[i]});
    return out;
}

// ---- proposing the definitions -------------------------------------------------------------------------------

namespace {

// Everything a definition reads of the program, and what the draft learns while it writes them.
class Definer {
public:
    Definer(ByteView ds, const std::map<std::int64_t, std::string>& syms, const Declarations& decl, const std::map<std::size_t, Bytes>& strs,
            const StructTable& structs, const std::set<std::string>& local, const std::map<std::uint32_t, std::string>& code)
        : ds_(ds), syms_(syms), decl_(decl), strs_(strs), structs_(structs), local_(local), code_(code) {}

    // The functions the tables of a draft name, with the type their table gives them.
    const std::map<std::string, std::string>& functions() const { return funcs_; }

    std::string definition(std::size_t a, std::size_t nx);

private:
    const std::set<Declaration>& declarations_of(const std::string& name) const {
        static const std::set<Declaration> none;
        const auto it = decl_.find(name);
        return it == decl_.end() ? none : it->second;
    }
    // What a word of the data says it is: a string of the module, or a place it names ("&name" for a variable).
    std::optional<std::string> value(std::uint32_t v, std::size_t a) const;
    std::string struct_value(std::size_t at, const std::string& name, std::size_t a) const;
    std::string char_or_number(std::uint8_t c, bool plain_zero) const;

    ByteView ds_;
    const std::map<std::int64_t, std::string>& syms_;   // lookup only
    const Declarations& decl_;
    const std::map<std::size_t, Bytes>& strs_;
    const StructTable& structs_;
    const std::set<std::string>& local_;
    const std::map<std::uint32_t, std::string>& code_;
    std::map<std::string, std::string> funcs_;
};

std::optional<std::string> Definer::value(std::uint32_t v, std::size_t a) const {
    if (const auto s = strs_.find(v); s != strs_.end()) return c_string(s->second);
    if (const auto s = syms_.find(static_cast<std::int64_t>(v)); s != syms_.end() && v != a) {
        const auto& ty = declarations_of(s->second);
        const bool array = std::any_of(ty.begin(), ty.end(), [](const Declaration& d) { return d.declarator.ends_with(']'); });
        return (array ? "" : "&") + s->second;
    }
    return std::nullopt;
}

// A byte of a one-byte variable: a character when it is one, else a number.
std::string Definer::char_or_number(std::uint8_t c, bool plain_zero) const {
    if (c > 32 && c < 127 && c != '\'' && c != '\\') return std::format("'{}'", static_cast<char>(c));
    return plain_zero || c <= 9 ? std::to_string(c) : std::format("0x{:02x}", c);
}

std::string Definer::struct_value(std::size_t at, const std::string& name, std::size_t a) const {
    std::vector<std::string> parts;
    std::size_t pos = 0;
    const auto st = structs_.structs.find(name);
    if (st == structs_.structs.end()) return "{ 0 }";   // (a struct no header defines: its bytes are not told)
    for (const Field& f : st->second) {
        const Layout l = layout_of(f.base, f.stars, f.dims, structs_.structs);
        const std::size_t align = std::max<std::size_t>(l.align, 1);
        pos += (align - pos % align) % align;
        const std::size_t p = at + pos;
        const bool unsigned_base = f.base.starts_with("unsigned");
        const auto scalar = scalar_size(f.base).value_or(kUnknownSize);
        if (f.stars != 0) {
            const std::uint32_t v = word_at(ds_, p);
            std::optional<std::string> x = value(v, a);
            const std::string stars(f.stars, '*');
            if (!x) x = v == 0 ? std::string("0") : std::format("({} {})0x{:04x} /* ? */", f.base, stars, v);
            else if (x->starts_with('&') || !x->starts_with('"')) x = std::format("({} {}){}", f.base, stars, *x);
            parts.push_back(*x);
        } else if (f.base.starts_with("struct ") && f.dims.empty()) {
            parts.push_back(struct_value(p, f.base.substr(7), a));
        } else if ((f.base == "char" || f.base == "unsigned char") && !f.dims.empty()) {
            const ByteView body = slice_of(ds_, p, l.size);
            std::size_t end = body.size();
            while (end > 0 && body[end - 1] == 0) --end;
            const ByteView s = body.first(end);
            if (std::find(s.begin(), s.end(), std::uint8_t(0)) == s.end()) {
                parts.push_back(c_string(s));
            } else {
                std::vector<std::string> bytes;
                for (std::uint8_t c : body) bytes.push_back(std::to_string(c));
                parts.push_back("{ " + text::join(bytes, ", ") + " }");
            }
        } else if (scalar == 1 && f.dims.empty()) {
            parts.push_back(char_or_number(byte_at(ds_, p), false));
        } else if (scalar == 4 && f.dims.empty()) {
            const std::uint32_t v = word_at(ds_, p) | (word_at(ds_, p + 2) << 16);
            parts.push_back(unsigned_base ? std::format("{}L", v) : std::format("{}L", static_cast<std::int32_t>(v)));
        } else if (f.dims.empty()) {
            const std::uint32_t v = word_at(ds_, p);
            parts.push_back(std::to_string(unsigned_base ? static_cast<std::int32_t>(v) : static_cast<std::int16_t>(v)));
        } else {
            std::vector<std::string> items;
            for (std::size_t j = 0; j < l.size; j += scalar) {
                std::uint64_t v = 0;
                for (std::size_t b = 0; b < scalar; ++b) v |= static_cast<std::uint64_t>(byte_at(ds_, p + j + b)) << (8 * b);
                items.push_back(std::to_string(v));
            }
            parts.push_back("{ " + text::join(items, ", ") + " }");
        }
        pos += l.size;
    }
    while (parts.size() > 1 && (parts.back() == "0" || parts.back() == "0L")) parts.pop_back();   // (the rest of an element left to the compiler's zeros)
    return "{ " + text::join(parts, ", ") + " }";
}

// "[ n ]" and the like in a declarator become "[count]".
std::string with_count(std::string_view declarator, std::size_t count) {
    std::string out;
    for (std::size_t i = 0; i < declarator.size(); ++i) {
        if (declarator[i] == '[') {
            std::size_t e = i + 1;
            while (e < declarator.size() && text::is_space(declarator[e])) ++e;
            while (e < declarator.size() && is_word_char(declarator[e])) ++e;
            while (e < declarator.size() && text::is_space(declarator[e])) ++e;
            if (e < declarator.size() && declarator[e] == ']') {
                out += std::format("[{}]", count);
                i = e;
                continue;
            }
        }
        out.push_back(declarator[i]);
    }
    return out;
}

std::string signed_word(std::uint32_t v) { return std::to_string(static_cast<std::int16_t>(v)); }

std::string Definer::definition(std::size_t a, std::size_t nx) {
    const std::size_t n = nx > a ? nx - a : 0;
    const auto sym = syms_.find(static_cast<std::int64_t>(a));
    const std::string nm = sym != syms_.end() ? sym->second : std::format("g_{:04x}", a);
    const std::set<Declaration>* ty = &declarations_of(nm);
    std::vector<std::uint32_t> words;
    for (std::size_t q = 0; q < n / 2; ++q) words.push_back(word_at(ds_, a + 2 * q));
    const auto val = [&](std::uint32_t v) { return value(v, a); };
    // a table of functions: their names
    std::vector<Declaration> fp;
    for (const Declaration& d : *ty) {
        if (has(d.declarator, "(*") && has(without_spaces(d.declarator), ")(")) fp.push_back(d);
    }
    if (!fp.empty() && n % 2 == 0) {
        const Declaration& d = *std::min_element(fp.begin(), fp.end());
        std::vector<std::string> vals;
        for (std::uint32_t v : words) {
            const auto c = code_.find(v);
            vals.push_back(c != code_.end() ? c->second : v == 0 ? std::string("0") : std::format("({} (*)())0x{:04x} /* ? */", d.type, v));
        }
        for (const std::string& v : vals) {
            if (!v.starts_with('0') && !v.starts_with('(')) funcs_[v] = d.type;
        }
        if (has(d.declarator, "[")) return std::format("{} {} = {{ {} }};", d.type, with_count(d.declarator, n / 2), text::join(vals, ", "));
        return std::format("{} {} = {};", d.type, d.declarator, vals.empty() ? std::string("0") : vals[0]);
    }
    bool is_array = std::any_of(ty->begin(), ty->end(), [](const Declaration& d) { return d.declarator.ends_with(']'); });
    bool is_ptr = std::any_of(ty->begin(), ty->end(), [](const Declaration& d) { return has(d.declarator, "*"); });
    std::vector<std::string> kinds;
    for (const Declaration& d : *ty) kinds.push_back(d.type);
    std::sort(kinds.begin(), kinds.end());
    std::optional<std::string> base;
    for (const std::string& k : kinds) {
        if (k.starts_with("struct ")) {
            base = k;
            break;
        }
    }
    if (!base && !kinds.empty()) base = kinds[0];
    const std::size_t named_words = static_cast<std::size_t>(std::count_if(words.begin(), words.end(), [&](std::uint32_t v) { return val(v).has_value(); }));
    if (!ty->empty() && local_.find(nm) == local_.end() && named_words >= 2 &&
        (base == "char" || base == "int" || base == "unsigned" || (!is_array && !is_ptr))) {
        ty = &declarations_of("");   // (another module's type, and the bytes are a table of places: the bytes say what it is)
        is_array = is_ptr = false;
        base.reset();
    }
    std::optional<std::size_t> k;
    if (is_ptr && !is_array && !std::any_of(ty->begin(), ty->end(), [](const Declaration& d) { return has(d.declarator, "("); })) k = 2;
    else if (!is_array && !is_ptr && base) k = scalar_size(*base);
    if (!ty->empty() && k && *k != 0 && *k < n && n - *k >= 2) {   // a variable, and after it one nothing names
        return definition(a, a + *k) + "\n" + definition(a + *k, nx);
    }
    if (!ty->empty() && k == 1 && (base == "char" || base == "unsigned char") && n <= 2 && (n == 1 || byte_at(ds_, a + 1) == 0)) {
        return std::format("{} {} = {};", *base, nm, char_or_number(byte_at(ds_, a), true));   // (a char: the byte after it the next word's pad)
    }
    const std::string note = ty->empty() && sym == syms_.end() ? std::string(kNothingNames) : std::string();
    if (base && base->starts_with("struct ") && structs_.structs.contains(base->substr(7)) && !is_ptr) {
        const Layout l = struct_layout(base->substr(7), structs_.structs);
        if (l.size != 0 && n % l.size == 0) {
            std::vector<std::string> elems;
            for (std::size_t e = 0; e < n / l.size; ++e) elems.push_back(struct_value(a + e * l.size, base->substr(7), a));
            if (is_array || elems.size() > 1) return std::format("{} {}[] = {{\n    {},\n}};", *base, nm, text::join(elems, ",\n    "));
            return std::format("{} {} = {};", *base, nm, elems[0]);
        }
    }
    if (base && *base != "char" && *base != "int" && *base != "unsigned" && *base != "long" && is_ptr) {
        std::vector<std::string> vals;
        for (std::uint32_t v : words) {
            std::string x = val(v).value_or(v == 0 ? std::string("0") : std::format("({} *)0x{:04x} /* ? */", *base, v));
            if (x.starts_with('&')) x = "(" + *base + " *)" + x;
            vals.push_back(std::move(x));
        }
        const bool any = std::any_of(words.begin(), words.end(), [](std::uint32_t v) { return v != 0; });
        if (is_array) return std::format("{} *{}[{}] = {{ {} }};", *base, nm, n / 2, any ? text::join(vals, ", ") : std::string("0"));
        return std::format("{} *{} = {};", *base, nm, vals.empty() ? std::string("0") : vals[0]);
    }
    if (base == "char" && is_array && !is_ptr) {
        const ByteView body = slice_of(ds_, a, n);
        std::size_t end = body.size();
        while (end > 0 && body[end - 1] == 0) --end;
        const ByteView s = body.first(end);
        std::string init;
        if (!s.empty() && std::find(s.begin(), s.end(), std::uint8_t(0)) == s.end()) {
            init = c_string(s);
        } else if (!s.empty()) {
            std::vector<std::string> bytes;
            for (std::uint8_t c : body) bytes.push_back(std::to_string(c));
            init = "{ " + text::join(bytes, ", ") + " }";
        } else {
            init = "\"\"";
        }
        return std::format("char {}[{}] = {};{}", nm, n, init, note);
    }
    if (n == 2) {
        const std::uint32_t v = words[0];
        const auto pv = val(v);
        if (pv && pv->starts_with("\"@(#)") && sym == syms_.end()) return "static char *sccsid = " + *pv + ";";
        if (pv && pv->starts_with('"')) return std::format("char *{} = {};{}", nm, *pv, note);
        if (pv) return std::format("char *{} = (char *){};{}", nm, *pv, note);
        if (is_ptr) return std::format("char *{} = {};{}", nm, v == 0 ? std::string("0") : std::format("(char *)0x{:04x} /* ? */", v), note);
        const std::string type = (base == "int" || base == "unsigned") ? *base : "int";
        return std::format("{} {} = {};{}", type, nm, base == "unsigned" ? std::to_string(v) : signed_word(v), note);
    }
    if (n == 4 && base == "long") {
        return std::format("long {} = {}L;{}", nm, static_cast<std::int32_t>(words[0] | (words[1] << 16)), note);
    }
    if (n % 2 == 0 && (is_ptr || std::any_of(words.begin(), words.end(), [&](std::uint32_t v) { return val(v).has_value(); }))) {
        std::vector<std::string> vs;
        for (std::uint32_t v : words) {
            std::string x = val(v).value_or(v == 0 ? std::string("0") : std::format("(char *)0x{:04x} /* ? */", v));
            vs.push_back(x.starts_with('&') ? "(char *)" + x : x);
        }
        return std::format("char *{}[{}] = {{ {} }};{}", nm, n / 2, text::join(vs, ", "), note);
    }
    if (n % 2 == 0) {
        std::vector<std::string> vs;
        for (std::uint32_t v : words) vs.push_back(signed_word(v));
        return std::format("int {}[{}] = {{ {} }};{}", nm, n / 2, text::join(vs, ", "), note);
    }
    std::vector<std::string> bytes;
    for (std::size_t i = a; i < nx; ++i) bytes.push_back(std::to_string(byte_at(ds_, i)));
    return std::format("char {}[{}] = {{ {} }};{}", nm, n, text::join(bytes, ", "), note);
}

// The names of the code the program's functions have, by the low word of their address: a table of functions names them.
std::map<std::uint32_t, std::string> code_names(const modules::Project& project) {
    std::map<std::uint32_t, std::string> out;
    for (const auto& [name, place] : project.symbols()) {
        if (place.kind == SymKind::Code && !has(name, ":")) out.try_emplace(static_cast<std::uint32_t>(place.address & 0xFFFF), std::string(lstrip_underscores(name)));
    }
    std::vector<std::string> files;
    for (const auto& e : stdfs::directory_iterator(project.src())) {
        const std::string name = e.path().filename().string();
        if (name.size() == 7 && name[0] == 'f' && name.ends_with(".c") && text::parse_int<std::uint32_t>(name.substr(1, 5), 16) && name.find_first_of("ABCDEF") == std::string::npos)
            files.push_back(name);
    }
    std::sort(files.begin(), files.end());   // (the addresses that share a low word: the first file by name)
    for (const std::string& name : files) out.try_emplace(*text::parse_int<std::uint32_t>(name.substr(1, 5), 16) & 0xFFFF, name.substr(0, 6));
    return out;
}

// The strings in the data from `dstart` to `dend` that stand alone: printable bytes ended by a NUL.
std::map<std::size_t, Bytes> find_strings(ByteView ds, std::size_t dstart, std::size_t dend) {
    std::map<std::size_t, Bytes> strs;
    for (std::size_t k = dstart; k < dend;) {
        const std::uint8_t c = byte_at(ds, k);
        if ((c != 0 && is_ascii_text(c)) || c == 9 || c == 10) {
            const auto nul = std::find(ds.begin() + static_cast<std::ptrdiff_t>(k), ds.end(), std::uint8_t(0));
            if (nul != ds.end()) {
                const std::size_t e = static_cast<std::size_t>(nul - ds.begin());
                if (std::all_of(ds.begin() + static_cast<std::ptrdiff_t>(k), nul, [](std::uint8_t b) { return is_ascii_text(b) || b == 9 || b == 10 || b == 13; })) {
                    strs[k].assign(ds.begin() + static_cast<std::ptrdiff_t>(k), nul);
                    k = e + 1;
                    continue;
                }
            }
        }
        ++k;
    }
    return strs;
}

}  // namespace

std::vector<DataFile> draft(const Project& project, const modules::Row& row) {
    return draft(project, row, modules::FunctionTypes::read(project.src()));
}

std::vector<DataFile> draft(const Project& project, const modules::Row& row, const modules::FunctionTypes& types) {
    static const Regex defined_names(R"re(\b(\w+)(?=(?:\[\d*\])?\s*=))re");
    static const Regex string_literal(R"re("(?:[^"\\]|\\.)*")re");
    static const Regex struct_use(R"re(\bstruct\s+(\w+))re");
    if (const std::string rule = project.data_order(); rule != "after_next_function") throw DataError("no data rule '" + rule + "' yet");
    const Walk w = walk(project, row, types);
    const ByteView ds = data_segment(project);
    const std::size_t dstart = w.data_start, dend = w.data_end;
    // the program's names of its variables (not a function's: the file of that name is its)
    std::map<std::int64_t, std::string> syms;   // lookup only
    for (const auto& [name, place] : project.symbols()) {
        if (place.kind == SymKind::Data && !has(name, ":") && name.starts_with('_') && !stdfs::exists(project.path(name.substr(1) + ".c")))
            syms.try_emplace(place.address, name.substr(1));
    }
    const Declarations decl = externs(project.src());
    std::vector<stdfs::path> mine;
    if (row.code_start) {
        for (const stdfs::path& q : modules::files_of(project.src(), *row.code_start, row.code_end.value_or(0xFFFFFFFFu))) {
            if (q.filename().string().starts_with('f')) mine.push_back(q);
        }
    }
    std::set<std::string> local;
    if (!mine.empty()) {
        for (const auto& [name, ignored] : externs(project.src(), mine)) local.insert(name);
    }
    for (const auto& [name, ignored] : decl) {   // (the engine's names of places: g_ADDR)
        if (name.size() != 6 || !name.starts_with("g_")) continue;
        const auto at = text::parse_int<std::uint32_t>(name.substr(2), 16);
        if (at && name.find_first_not_of("0123456789abcdef", 2) == std::string::npos && dstart <= *at && *at < dend) syms.try_emplace(*at, name);
    }
    std::map<std::size_t, Bytes> strs = find_strings(ds, dstart, dend);
    std::set<std::size_t> targets;
    for (std::size_t q = dstart; q + 1 < dend; ++q) targets.insert(word_at(ds, q));
    std::set<std::size_t> string_ends;   // where each string ends (the byte after its NUL is a place the next begins)
    for (const auto& [at, s] : strs) string_ends.insert(at + s.size());
    for (std::size_t v : targets) {   // (a 0 pointed at, no variable's place: an empty string)
        if (dstart <= v && v < dend && byte_at(ds, v) == 0 && !syms.contains(static_cast<std::int64_t>(v)) && !strs.contains(v) &&
            (v == dstart || byte_at(ds, v - 1) == 0 || string_ends.contains(v - 1))) {
            strs[v] = {};
            string_ends.insert(v);
        }
    }
    const StructTable structs = parse_structs(project);
    std::vector<std::string> order;
    std::map<std::string, std::string> file_of;   // function -> its file's stem, fADDR
    for (const FunctionPlace& f : w.functions) {
        const std::string name(lstrip_underscores(f.name));
        order.push_back(name);
        file_of[name] = std::format("f{:05x}", f.address);
    }
    OrderedMap<std::string, std::vector<std::string>> files;
    struct VarRun {
        std::string key;
        std::size_t lo, hi;
    };
    std::vector<VarRun> varruns;
    const std::string stem = stdfs::path(row.name).stem().string();
    for (const Run& r : w.runs) {
        if (r.kind == RunKind::Strings) continue;
        std::size_t lo = r.lo, hi = r.hi;
        std::optional<std::string> fn;
        if (r.after == kStart) {
            std::size_t q = lo;   // (the file's start: its initializers' strings first)
            while (q < hi) {      // (a string nothing points at is a char array of its own)
                if (strs.contains(q) && targets.contains(q)) q += strs.at(q).size() + 1;
                else if (byte_at(ds, q) == 0 && (q & 1)) ++q;
                else break;
            }
            lo = q;
            if (!order.empty()) fn = order[0];
        } else {
            std::size_t q = hi;   // (after a function's literals: variables, then the strings of the initializers before the next one)
            while (true) {
                std::optional<std::size_t> best;
                for (const auto& [s, bytes] : strs) {
                    const std::size_t end = s + bytes.size() + 1;
                    if (lo <= s && s < q && (end == q || end + 1 == q) && targets.contains(s)) best = s;   // (ascending: the last is the greatest)
                }
                if (!best) break;
                q = *best;
            }
            hi = q;
            fn = std::string(lstrip_underscores(r.after));   // (its variables were declared before that function)
        }
        if (lo >= hi) continue;
        std::string key;
        if (fn && file_of.contains(*fn)) key = "d" + file_of.at(*fn).substr(1) + ".c";
        else if (!row.code_start || w.functions.empty()) key = stem + "-data.c";
        else key = std::format("e{:05x}.c", w.functions.back().address);
        varruns.push_back({key, (lo & 1) && byte_at(ds, lo) == 0 ? lo + 1 : lo, hi});
    }
    // inside the variables' runs nothing is a literal: what looks like a string there is a variable's bytes, and a word
    // that points into a run points at a variable (one of its own, named after its place)
    for (const VarRun& v : varruns) {
        for (auto it = strs.lower_bound(v.lo); it != strs.end() && it->first < v.hi;) it = strs.erase(it);
    }
    for (const VarRun& v : varruns) {
        for (std::size_t t : targets) {
            if (v.lo < t && t < v.hi && !syms.contains(static_cast<std::int64_t>(t)) && t % 2 == 0) syms[static_cast<std::int64_t>(t)] = std::format("g_{:04x}", t);
        }
    }
    const std::map<std::uint32_t, std::string> code = code_names(project);
    Definer definer(ds, syms, decl, strs, structs, local, code);
    for (const VarRun& v : varruns) {
        std::vector<std::string>& lines = files[v.key];
        std::set<std::size_t> candidates{v.lo};
        for (const auto& [a, name] : syms) {
            if (static_cast<std::int64_t>(v.lo) <= a && a < static_cast<std::int64_t>(v.hi)) candidates.insert(static_cast<std::size_t>(a));
        }
        std::vector<std::size_t> places;   // (a place inside a variable of a declared size is part of it)
        std::size_t covered = 0;
        for (std::size_t a : candidates) {
            if (!places.empty() && a < covered) continue;
            places.push_back(a);
            const auto sym = syms.find(static_cast<std::int64_t>(a));
            const auto it = sym == syms.end() ? decl.end() : decl.find(sym->second);
            const auto size = it == decl.end() ? std::nullopt : declared_size(it->second, structs.structs);
            covered = a + size.value_or(0);
        }
        for (std::size_t i = 0; i < places.size(); ++i) lines.push_back(definer.definition(places[i], i + 1 < places.size() ? places[i + 1] : v.hi));
    }
    std::set<std::string> defined;
    for (const auto& [key, lines] : files) {
        for (const auto& m : defined_names.find_all(text::join(lines, "\n"))) defined.insert(m);
    }
    std::set<std::string> known;
    for (const auto& [a, name] : syms) known.insert(name);
    std::vector<DataFile> out;
    for (auto& [key, lines] : files) {   // (the places named that are not the file's: externs)
        std::set<std::string> used;
        for (const std::string& l : lines) {
            const std::string body = l.find('=') == std::string::npos ? std::string() : l.substr(l.find('=') + 1);
            for (std::string_view word : words_of(string_literal.replace_all(body, ""))) {
                if (known.contains(std::string(word))) used.insert(std::string(word));
            }
        }
        std::vector<std::string> prefix;
        for (const std::string& r : used) {
            if (defined.contains(r)) continue;
            const auto it = decl.find(r);
            const Declaration d = it == decl.end() || it->second.empty() ? Declaration{"int", r} : *it->second.begin();
            prefix.push_back(std::format("extern {} {};", d.type, d.declarator));
        }
        lines.insert(lines.begin(), prefix.begin(), prefix.end());
        std::set<std::string> named;
        for (const std::string& l : lines) {
            for (std::string_view word : words_of(l)) {
                if (definer.functions().contains(std::string(word))) named.insert(std::string(word));
            }
        }
        prefix.clear();
        for (const std::string& f : named) prefix.push_back(std::format("extern {} {}();", definer.functions().at(f), f));
        lines.insert(lines.begin(), prefix.begin(), prefix.end());
        std::vector<std::string> headers;   // (the headers that define the structs it uses)
        struct_use.for_each_match(text::join(lines, "\n"), [&](const RegexMatch& m) {
            const auto home = structs.home.find(std::string(m.group(1)));
            if (home != structs.home.end() && std::find(headers.begin(), headers.end(), home->second) == headers.end() && mine.empty()) headers.push_back(home->second);   // (a module with functions sees them already)
            return true;
        });
        prefix.clear();
        for (const std::string& h : headers) prefix.push_back("#include \"" + h + "\"");
        lines.insert(lines.begin(), prefix.begin(), prefix.end());
        out.push_back({key, text::join(lines, "\n") + "\n"});
    }
    return out;
}

int run(const Project& project, const std::vector<std::string>& args) {
    std::vector<std::string> want;
    bool draft_only = false, write = false;
    for (const std::string& a : args) {
        if (a == "--draft") draft_only = true;
        else if (a == "--write") write = true;
        else if (!a.starts_with('-')) want.push_back(a);
    }
    const modules::FunctionTypes types = modules::FunctionTypes::read(project.src());
    for (const modules::Row& row : modules::rows(project)) {
        const std::string stem = stdfs::path(row.name).stem().string();
        if (!want.empty() && std::find(want.begin(), want.end(), row.name) == want.end() && std::find(want.begin(), want.end(), stem) == want.end()) continue;
        if (text::iends_with(row.name, ".OBJ")) continue;
        if (draft_only || write) {
            for (const DataFile& f : draft(project, row, types)) {
                out("== {}\n{}\n", f.name, f.text);
                const stdfs::path p = project.src() / f.name;
                if (!write) continue;
                if (stdfs::exists(p)) out("   (exists: not written)\n");
                else fs::write_text(p, f.text);
            }
            continue;
        }
        const Walk w = walk(project, row, types);
        out("{}: data {:04x}..{:04x}, {} literals of its functions\n", row.name, w.data_start, w.data_end, w.literals.size());
        for (const Run& r : w.runs) {
            out("   {:04x}..{:04x} {:4} bytes after {:<10} {}\n", r.lo, r.hi, r.hi - r.lo, r.after, r.kind == RunKind::Strings ? "strings" : "vars");
        }
    }
    return 0;
}

}  // namespace cb::exact::data

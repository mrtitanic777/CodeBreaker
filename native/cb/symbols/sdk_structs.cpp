// cb/symbols/sdk_structs.cpp -- structure layouts from the SDK headers (see sdk_structs.hpp).
//
// The headers' text is scanned for `typedef struct [tag] { ... } Name, *PName;` with the scanners below (what a regular
// expression engine would do, without one); each structure's fields are then laid out at the offsets the headers'
// packing gives them.
#include "cb/symbols/sdk_structs.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/knowledge/reads.hpp"

namespace cb::symbols::sdk_structs {
namespace {

using std::string;
using std::string_view;

bool is_word(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }

// ---- the sizes of the types the headers use ---------------------------------------------------------------------------

std::optional<std::uint32_t> known_size(string_view t) {
    static const std::unordered_map<string_view, std::uint32_t> sizes = {
        {"BYTE", 1}, {"CHAR", 1}, {"char", 1}, {"UCHAR", 1}, {"BOOLEAN", 1}, {"CCHAR", 1}, {"TBYTE", 1}, {"TCHAR", 1},
        {"WORD", 2}, {"SHORT", 2}, {"short", 2}, {"USHORT", 2}, {"WCHAR", 2}, {"ATOM", 2}, {"LANGID", 2},
        {"double", 8}, {"DOUBLE", 8}, {"LONGLONG", 8}, {"DWORDLONG", 8}, {"ULONGLONG", 8}, {"__int64", 8}, {"FILETIME", 8},
        {"LARGE_INTEGER", 8}};
    const auto it = sizes.find(t);
    if (it == sizes.end()) return std::nullopt;
    return it->second;
}

bool is_four_bytes(string_view t) {
    static const std::set<string_view> four = {"LONG", "DWORD", "UINT", "INT", "int", "BOOL", "ULONG", "COLORREF", "WPARAM", "LPARAM",
                                               "LRESULT", "FARPROC", "WNDPROC", "DLGPROC", "HOOKPROC", "float", "FLOAT", "long",
                                               "SIZE_T", "LCID", "LCTYPE", "ACCESS_MASK", "REGSAM", "HRESULT", "UINT_PTR", "LONG_PTR",
                                               "DWORD_PTR", "unsigned", "TIMERPROC"};
    if (four.contains(t)) return true;
    // handles (HWND), long and near pointers (LPSTR, NPSTR), pointers to constants (PCSTR), pointers (PRECT)
    if (t.size() >= 2 && t[0] == 'H' && is_upper(t[1])) return true;
    if (t.starts_with("LP") || t.starts_with("PC") || t.starts_with("NP")) return true;
    return t.size() >= 2 && t[0] == 'P' && is_upper(t[1]);
}

bool is_signed_type(string_view t) {
    static const std::set<string_view> signed_types = {"LONG", "INT", "int", "BOOL", "LPARAM", "LRESULT", "SHORT", "short", "CHAR",
                                                        "char", "long", "HRESULT", "LONG_PTR", "CCHAR", "INT_PTR"};
    return signed_types.contains(t);
}

// ---- reading the headers --------------------------------------------------------------------------------------------

struct RawStruct {
    string body;   // between the braces
    int pack = 8;  // the packing in force where the structure is
};

// The text with every run of the words in `drop` (whole words) replaced by a space.
string drop_words(string_view s, std::initializer_list<string_view> drop) {
    string out;
    size_t i = 0;
    while (i < s.size()) {
        if (!is_word(s[i])) {
            out.push_back(s[i++]);
            continue;
        }
        size_t j = i;
        while (j < s.size() && is_word(s[j])) ++j;
        const string_view word = s.substr(i, j - i);
        if (std::find(drop.begin(), drop.end(), word) != drop.end()) out.push_back(' ');
        else out.append(word);
        i = j;
    }
    return out;
}

string strip_comments(string text) {
    // /* ... */ (not nested; one that is never closed stays), then // ... to the end of the line
    string out;
    out.reserve(text.size());
    size_t i = 0;
    while (i < text.size()) {
        if (text[i] == '/' && i + 1 < text.size() && text[i + 1] == '*') {
            const size_t end = text.find("*/", i + 2);
            if (end != string::npos) {
                out.push_back(' ');
                i = end + 2;
                continue;
            }
        }
        out.push_back(text[i++]);
    }
    text.clear();
    string result;
    result.reserve(out.size());
    i = 0;
    while (i < out.size()) {
        if (out[i] == '/' && i + 1 < out.size() && out[i + 1] == '/') {
            const size_t end = out.find('\n', i);
            result.push_back(' ');
            i = end == string::npos ? out.size() : end;
            continue;
        }
        result.push_back(out[i++]);
    }
    return result;
}

// Where `#include <pshpackN.h>` and `<poppack.h>` stand, and the packing in force from there on.
std::vector<std::pair<size_t, int>> pack_changes(const string& text) {
    std::vector<std::pair<size_t, int>> packs;
    std::vector<int> stack;
    size_t at = 0;
    while ((at = text.find("#include", at)) != string::npos) {
        size_t p = at + 8;
        while (p < text.size() && text::is_space(text[p])) ++p;
        int push = -1;   // -1: poppack
        bool match = false;
        if (text.compare(p, 8, "<pshpack") == 0 && p + 9 < text.size() && is_digit(text[p + 8]) && text.compare(p + 9, 3, ".h>") == 0) {
            push = text[p + 8] - '0';
            match = true;
        } else if (text.compare(p, 11, "<poppack.h>") == 0) {
            match = true;
        }
        if (match) {
            if (push >= 0) stack.push_back(push);
            else if (!stack.empty()) stack.pop_back();
            packs.emplace_back(at, stack.empty() ? 8 : stack.back());
        }
        at += 8;
    }
    return packs;
}

// The last word of a declarator, without its stars: `FAR *LPRECT` is LPRECT; nullopt for a blank one.
std::optional<string> declarator_name(string_view declarator) {
    const auto words = text::split_ws(declarator);
    if (words.empty()) return std::nullopt;
    string_view last = words.back();
    while (last.starts_with('*')) last.remove_prefix(1);
    return string(last);
}

// `typedef struct [tag] { body } declarators;` at or after `from`; the match's start and end are set.
struct Typedef {
    size_t start = 0;
    size_t end = 0;
    string tag;
    string body;
    string declarators;
};

std::optional<Typedef> next_struct_typedef(const string& text, size_t from) {
    size_t at = from;
    while ((at = text.find("typedef", at)) != string::npos) {
        const size_t start = at;
        at += 1;   // the next attempt, should this one fail
        size_t p = start + 7;
        size_t q = p;
        while (q < text.size() && text::is_space(text[q])) ++q;
        if (q == p || text.compare(q, 6, "struct") != 0) continue;
        p = q + 6;
        q = p;
        while (q < text.size() && text::is_space(text[q])) ++q;
        if (q == p) continue;
        p = q;
        while (q < text.size() && is_word(text[q])) ++q;
        string tag = text.substr(p, q - p);
        while (q < text.size() && text::is_space(text[q])) ++q;
        if (q >= text.size() || text[q] != '{') continue;
        const size_t body_start = q + 1;
        size_t body_end = body_start;
        while (body_end < text.size() && text[body_end] != '}' && text[body_end] != '{') ++body_end;
        if (body_end >= text.size() || text[body_end] != '}') continue;   // a brace inside: not a plain structure
        size_t d = body_end + 1;
        while (d < text.size() && text::is_space(text[d])) ++d;
        const size_t semi = text.find(';', d);
        if (semi == string::npos || semi == d) continue;
        return Typedef{start, semi + 1, std::move(tag), text.substr(body_start, body_end - body_start), text.substr(d, semi - d)};
    }
    return std::nullopt;
}

// `typedef [const] Type [FAR|NEAR] *Name;` at or after `from`: (Type, Name) and where the match ends.
struct PointerTypedef {
    size_t end = 0;
    string type;
    string name;
};

std::optional<PointerTypedef> next_pointer_typedef(const string& text, size_t from) {
    size_t at = from;
    while ((at = text.find("typedef", at)) != string::npos) {
        const size_t start = at;
        at += 1;
        size_t p = start + 7;
        const auto skip_space = [&](size_t i) {
            while (i < text.size() && text::is_space(text[i])) ++i;
            return i;
        };
        const auto word_end = [&](size_t i) {
            while (i < text.size() && is_word(text[i])) ++i;
            return i;
        };
        size_t q = skip_space(p);
        if (q == p) continue;
        // the optional `const `, tried first and given up if the rest does not follow
        for (const bool with_const : {true, false}) {
            size_t t = q;
            if (with_const) {
                if (text.compare(t, 5, "const") != 0 && text.compare(t, 5, "CONST") != 0) continue;
                const size_t after = skip_space(t + 5);
                if (after == t + 5) continue;
                t = after;
            }
            const size_t type_end = word_end(t);
            if (type_end == t) continue;
            const size_t a = skip_space(type_end);
            if (a == type_end) continue;
            size_t b = a;
            for (const string_view qualifier : {"FAR", "NEAR", "far", "near"}) {
                if (text.compare(b, qualifier.size(), qualifier) == 0) {
                    b = skip_space(b + qualifier.size());
                    break;
                }
            }
            if (b >= text.size() || text[b] != '*') continue;
            const size_t n = skip_space(b + 1);
            const size_t name_end = word_end(n);
            if (name_end == n) continue;
            const size_t semi = skip_space(name_end);
            if (semi >= text.size() || text[semi] != ';') continue;
            return PointerTypedef{semi + 1, text.substr(t, type_end - t), text.substr(n, name_end - n)};
        }
    }
    return std::nullopt;
}

// ---- laying a structure out ----------------------------------------------------------------------------------------------

class Builder {
public:
    Builder(const OrderedMap<string, RawStruct>& raw, const Constants& constants) : raw_(raw), constants_(constants) {}

    Structs run() {
        for (const auto& [name, r] : raw_) build(name, 0);
        return std::move(structs_);
    }

private:
    // The layout of one structure, or nullopt: unknown, too deeply nested, or with a field of a type of unknown size.
    const std::optional<Layout>* build(const string& name, int depth) {
        if (const auto* known = structs_.find(name)) return known;
        structs_.try_emplace(name, std::nullopt);   // (while it is being laid out: a structure inside itself stays unknown)
        const auto raw = raw_.find(name);
        if (raw == nullptr || depth > 6) return structs_.find(name);
        std::optional<Layout> layout = lay_out(*raw, depth);
        *structs_.find(name) = std::move(layout);
        return structs_.find(name);
    }

    std::optional<std::uint32_t> size_of(string_view type) const {
        const string_view t = text::trim(type);
        if (const auto s = known_size(t)) return s;
        if (is_four_bytes(t)) return 4;
        if (const auto* l = structs_.find(string(t)); l != nullptr && l->has_value()) return (*l)->size;
        return std::nullopt;
    }

    std::optional<Layout> lay_out(const RawStruct& raw, int depth) {
        Layout out;
        std::int64_t off = 0;
        std::int64_t biggest = 1;
        for (string_view piece : text::split(raw.body, ';')) {
            const string decl(text::trim(drop_words(piece, {"CONST", "const", "volatile", "FAR", "NEAR", "far", "near", "struct"})));
            if (decl.empty()) continue;
            // <base type> [*]... <item>, <item>...
            size_t i = 0;
            while (i < decl.size() && is_word(decl[i])) ++i;
            if (i == 0) return std::nullopt;
            const string base = decl.substr(0, i);
            while (i < decl.size() && text::is_space(decl[i])) ++i;
            size_t stars_end = i;
            while (stars_end < decl.size() && decl[stars_end] == '*') ++stars_end;
            const size_t star_count = stars_end - i;
            i = stars_end;
            while (i < decl.size() && text::is_space(decl[i])) ++i;
            const string rest = decl.substr(i);
            const auto allowed = [](char c) { return is_word(c) || text::is_space(c) || c == ',' || c == '[' || c == ']' || c == '*'; };
            if (rest.empty() || !std::all_of(rest.begin(), rest.end(), allowed)) return std::nullopt;
            for (string_view raw_item : text::split(rest, ',')) {
                string_view item = text::trim(raw_item);
                size_t stars = star_count;
                while (item.starts_with('*')) {
                    ++stars;
                    item.remove_prefix(1);
                }
                item = text::trim(item);
                // name[count]
                string field_name(item);
                std::optional<std::int64_t> count;
                if (const size_t open = item.find('['); open != string_view::npos) {
                    const string_view name_part = text::rtrim(item.substr(0, open));
                    const size_t close = item.find(']', open);
                    if (close == string_view::npos || close + 1 != item.size()) return std::nullopt;
                    const string_view count_text = text::trim(item.substr(open + 1, close - open - 1));
                    if (name_part.empty() || count_text.empty() || !std::all_of(name_part.begin(), name_part.end(), is_word) ||
                        !std::all_of(count_text.begin(), count_text.end(), is_word)) {
                        return std::nullopt;
                    }
                    field_name = string(name_part);
                    if (is_digit(count_text[0])) {
                        count = text::parse_int<std::int64_t>(count_text, 0);
                    } else if (const auto it = constants_.find(string(count_text)); it != constants_.end()) {
                        count = it->second;
                    }
                    if (!count || *count < 0 || *count > (1 << 20)) return std::nullopt;   // a length the headers do not give
                }
                if (field_name.empty() || !std::all_of(field_name.begin(), field_name.end(), is_word)) return std::nullopt;

                std::int64_t n = 0;
                string ctype;
                bool pointer = false;
                std::int64_t alignment = 0;
                if (stars > 0) {
                    n = 4;
                    ctype = "uint4";
                    pointer = true;
                    alignment = n;
                } else if (raw_.contains(base) && !known_size(base) && !is_four_bytes_exact(base)) {
                    const auto* inner = build(base, depth + 1);
                    if (inner == nullptr || !inner->has_value()) return std::nullopt;
                    n = (*inner)->size;
                    ctype = "struct " + base;
                    alignment = (*inner)->alignment;
                } else {
                    const auto size = size_of(base);
                    if (!size) return std::nullopt;
                    n = *size;
                    if (n != 1 && n != 2 && n != 4 && n != 8) return std::nullopt;
                    ctype = std::format("{}{}", is_signed_type(base) ? "int" : "uint", n);
                    alignment = n;
                }
                const std::int64_t align = std::min<std::int64_t>({alignment, raw.pack, 8});
                off = (off + align - 1) / align * align;
                biggest = std::max(biggest, align);
                const std::int64_t total = n * ((count && *count != 0) ? *count : 1);
                Field f;
                f.offset = static_cast<std::uint32_t>(off);
                f.size = static_cast<std::uint32_t>(count ? total : n);
                f.name = field_name;
                f.ctype = count ? std::format("{}[{}]", ctype, *count) : ctype;
                f.pointer = pointer && !count;
                out.fields.push_back(std::move(f));
                off += total;
            }
        }
        out.size = static_cast<std::uint32_t>((off + biggest - 1) / biggest * biggest);
        out.alignment = static_cast<std::uint32_t>(biggest);
        return out;
    }

    // `base in _FOUR`: the exact members of that list (not the pattern-matched pointer and handle names)
    static bool is_four_bytes_exact(string_view t) {
        static const std::set<string_view> four = {"LONG", "DWORD", "UINT", "INT", "int", "BOOL", "ULONG", "COLORREF", "WPARAM", "LPARAM",
                                                   "LRESULT", "FARPROC", "WNDPROC", "DLGPROC", "HOOKPROC", "float", "FLOAT", "long",
                                                   "SIZE_T", "LCID", "LCTYPE", "ACCESS_MASK", "REGSAM", "HRESULT", "UINT_PTR", "LONG_PTR",
                                                   "DWORD_PTR", "unsigned", "TIMERPROC"};
        return four.contains(t);
    }

    const OrderedMap<string, RawStruct>& raw_;
    const Constants& constants_;
    Structs structs_;
};

}  // namespace

HeaderStructs read_headers(std::span<const std::filesystem::path> folders, const Constants& constants) {
    OrderedMap<string, RawStruct> raw;   // in the order the structures were met: the order they are laid out in
    Pointers pointers;
    for (const std::filesystem::path& folder : folders) {
        std::error_code ec;
        if (!std::filesystem::is_directory(folder, ec)) continue;
        std::vector<std::filesystem::path> headers;
        for (const auto& entry : std::filesystem::directory_iterator(folder, ec)) {
            if (text::iends_with(fs::to_text(entry.path().filename()), ".h")) headers.push_back(entry.path());
        }
        std::sort(headers.begin(), headers.end(), [](const auto& a, const auto& b) { return fs::to_text(a.filename()) < fs::to_text(b.filename()); });
        for (const auto& header : headers) {
            string text;
            try {
                const Bytes bytes = fs::read_file(header);
                text.assign(as_text(bytes));
            } catch (const IoError&) {   // a header that cannot be read has no structures
                continue;
            }
            text = text::replace_all(text, "\r\n", "\n");
            std::replace(text.begin(), text.end(), '\r', '\n');
            text = strip_comments(std::move(text));
            const auto packs = pack_changes(text);
            const auto pack_at = [&](size_t position) {
                int current = 8;
                for (const auto& [start, value] : packs) {
                    if (start > position) break;
                    current = value;
                }
                return current;
            };
            size_t pos = 0;
            while (const auto decl = next_struct_typedef(text, pos)) {
                pos = decl->end;
                std::vector<string> names;
                const auto parts = text::split(decl->declarators, ',');
                for (const string_view part : parts) {
                    if (const auto name = declarator_name(part)) names.push_back(*name);
                }
                if (names.empty()) continue;
                if (decl->body.find("union") != string::npos || decl->body.find('#') != string::npos) continue;
                const string& main_name = names[0];
                if (raw.contains(main_name)) continue;
                raw.try_emplace(main_name, RawStruct{decl->body, pack_at(decl->start)});
                for (size_t k = 1; k < parts.size(); ++k) {
                    if (parts[k].find('*') == string_view::npos) continue;
                    if (const auto name = declarator_name(parts[k])) pointers.insert_or_assign(*name, main_name);
                }
                if (!decl->tag.empty()) raw.try_emplace(decl->tag, *raw.find(main_name));
            }
            pos = 0;
            while (const auto ptr = next_pointer_typedef(text, pos)) {
                pos = ptr->end;
                pointers.try_emplace(ptr->name, ptr->type);
            }
        }
    }
    return {Builder(raw, constants).run(), std::move(pointers)};
}

// ---- the table ------------------------------------------------------------------------------------------------------------

StructTable::StructTable(HeaderStructs data) : data_(std::move(data)) {}

const Layout* StructTable::layout(std::string_view name) const {
    const string key(name);
    knowledge::record("win32", "struct", name);
    const auto* found = data_.structs.find(key);
    return found != nullptr && found->has_value() ? &**found : nullptr;
}

std::optional<std::string> StructTable::pointed_to(std::string_view type_text) const {
    const string t(text::trim(drop_words(type_text, {"CONST", "const", "FAR", "NEAR", "far", "near"})));
    const auto known = [&](const string& name) {
        const auto* s = data_.structs.find(name);
        return s != nullptr && s->has_value();
    };
    if (t.ends_with('*')) {
        const string base(text::trim(string_view(t).substr(0, t.size() - 1)));
        knowledge::record("win32", "struct", base);
        return known(base) ? std::optional<string>(base) : std::nullopt;
    }
    const auto* base = data_.pointers.find(t);
    knowledge::record("win32", "struct.pointer", t);
    if (base != nullptr) knowledge::record("win32", "struct", *base);
    if (base != nullptr && known(*base)) return *base;
    return std::nullopt;
}

std::optional<std::string> StructTable::field_at(const Layout& layout, std::uint32_t offset, std::uint32_t size) const {
    for (const Field& f : layout.fields) {
        if (f.offset == offset && f.size == size) return f.name;
        if (f.offset <= offset && offset < f.offset + f.size && f.ctype.starts_with("struct ")) {
            if (const Layout* inner = this->layout(string_view(f.ctype).substr(7))) {
                if (const auto sub = field_at(*inner, offset - f.offset, size)) return f.name + "." + *sub;
            }
        }
    }
    return std::nullopt;
}

std::optional<StructTable::Typed> StructTable::typed_field(const Layout& layout, std::uint32_t offset, std::uint32_t size) const {
    for (const Field& f : layout.fields) {
        if (f.offset == offset && f.size == size && !f.ctype.starts_with("struct ") && f.ctype.find('[') == string::npos) {
            return Typed{f.name, f.ctype};
        }
        if (f.offset <= offset && offset < f.offset + f.size && f.ctype.starts_with("struct ")) {
            if (const Layout* inner = this->layout(string_view(f.ctype).substr(7))) {
                if (const auto sub = typed_field(*inner, offset - f.offset, size)) return Typed{f.name + "." + sub->path, sub->ctype};
            }
        }
    }
    return std::nullopt;
}

std::string StructTable::definition(std::string_view name, const std::vector<std::string>& known) const {
    string out;
    std::set<string> done(known.begin(), known.end());
    const std::function<void(const string&)> emit = [&](const string& struct_name) {
        if (done.contains(struct_name)) return;
        done.insert(struct_name);
        const Layout* st = layout(struct_name);
        CB_ASSERT(st != nullptr, "sdk_structs: a structure to define has no layout");
        std::vector<string> lines;
        std::uint32_t off = 0;
        for (const Field& f : st->fields) {
            if (f.ctype.starts_with("struct ")) emit(f.ctype.substr(7));
            if (f.offset > off) lines.push_back(std::format("    uint1 codebreaker_pad_{:x}[{}];", off, f.offset - off));
            // `type[count]`
            string base = f.ctype;
            string count;
            if (base.ends_with(']')) {
                const size_t open = base.rfind('[');
                const string digits = base.substr(open + 1, base.size() - open - 2);
                if (open != string::npos && !digits.empty() && std::all_of(digits.begin(), digits.end(), is_digit)) {
                    count = "[" + digits + "]";
                    base.resize(open);
                }
            }
            if (base.starts_with("struct ")) base = base.substr(7);
            lines.push_back(std::format("    {} {}{};", base, f.name, count));
            off = f.offset + f.size;
        }
        if (st->size > off) lines.push_back(std::format("    uint1 codebreaker_pad_{:x}[{}];", off, st->size - off));
        out += std::format("#pragma pack(push, 1)\ntypedef struct {} {{\n{}\n}} {};\n#pragma pack(pop)\n", struct_name,
                           text::join(lines, "\n"), struct_name);
    };
    emit(string(name));
    return out;
}

}  // namespace cb::symbols::sdk_structs

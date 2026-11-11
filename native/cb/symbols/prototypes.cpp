// cb/symbols/prototypes.cpp -- reading the Windows API's prototypes from the C headers (see prototypes.hpp).
//
// A header is read as text of single bytes (the headers are Latin-1; nothing here cares). Comments and preprocessor
// lines are blanked, then every `name(` that follows a calling-convention word is a function declaration if its
// parentheses close and a ';' follows. The scanners are written out by hand: they are what a regular expression
// engine would do, without one.
#include "cb/symbols/prototypes.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <set>
#include <string_view>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/knowledge/reads.hpp"

namespace cb::symbols::prototypes {
namespace {

using std::string;
using std::string_view;

bool is_word(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }
bool is_first(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }
bool is_upper_digit(char c) { return is_upper(c) || (c >= '0' && c <= '9'); }

// The calling convention a word stands for ("stdcall" or "cdecl"), or "".
string_view convention_of_word(string_view w) {
    static const std::unordered_map<string_view, string_view> table = {
        {"WINAPI", "stdcall"}, {"APIENTRY", "stdcall"}, {"PASCAL", "stdcall"}, {"CALLBACK", "stdcall"},
        {"STDAPICALLTYPE", "stdcall"}, {"STDCALL", "stdcall"}, {"__stdcall", "stdcall"}, {"_stdcall", "stdcall"},
        {"STDAPI", "stdcall"}, {"NTAPI", "stdcall"}, {"API", "stdcall"}, {"__pascal", "stdcall"}, {"_pascal", "stdcall"},
        {"WINAPIV", "cdecl"}, {"CDECL", "cdecl"}, {"cdecl", "cdecl"}, {"__cdecl", "cdecl"}, {"_cdecl", "cdecl"},
        {"STDAPIVCALLTYPE", "cdecl"}};
    const auto it = table.find(w);
    return it == table.end() ? string_view() : it->second;
}

bool is_com_marker(string_view w) { return w == "STDAPI" || w == "WINOLEAPI" || w == "WINOLEAUTAPI"; }

// An upper-case word ending in `suffix` and at least `min_size` long, made of capitals and digits only: a macro
// like WINAPI or WINOLEAPI that stands for a convention.
bool is_api_macro(string_view w, string_view suffix, size_t min_size) {
    return w.size() >= min_size && is_upper(w[0]) && std::all_of(w.begin(), w.end(), is_upper_digit) && w.ends_with(suffix);
}

// A word that is no part of the result's type: linkage and storage words.
bool not_a_type(string_view w) {
    static const std::set<string_view> plain = {"extern", "C", "__declspec", "_CRTIMP", "static", "inline", "__inline",
                                                "dllimport", "dllexport", "_loadds", "__loadds", "EXPORTDBG"};
    if (plain.contains(w)) return true;
    if (w.starts_with("EXTERN_C")) return true;
    if (w.starts_with("DECLSPEC_") && w.size() > 9) return true;
    if (w.size() >= 4 && is_upper(w[0]) && std::all_of(w.begin(), w.end(), [](char c) { return is_upper_digit(c) || c == '_'; }) &&
        w.ends_with("API"))
        return true;
    if (w.size() > 8 && w.ends_with("_LINKAGE")) return true;
    return false;
}

// Spaces collapsed to one and the ends trimmed.
string collapse(string_view s) {
    string out;
    bool space = false;
    for (char c : s) {
        if (text::is_space(c)) {
            if (!space) out.push_back(' ');
            space = true;
        } else {
            out.push_back(c);
            space = false;
        }
    }
    return string(text::trim(out));
}

// The parameters of a list at its top level (commas inside parentheses do not split).
std::vector<string> split_parameters(string_view params) {
    std::vector<string> out;
    int depth = 0;
    string current;
    const auto flush = [&] {
        const string_view t = text::trim(current);
        if (!t.empty()) out.emplace_back(t);
        current.clear();
    };
    for (char c : params) {
        depth += c == '(';
        depth -= c == ')';
        if (c == ',' && depth == 0) flush();
        else current.push_back(c);
    }
    flush();
    return out;
}

// One parameter as (type, name): the name is the last identifier when the text before it is a type that says
// something besides const/volatile/far/near; array brackets after the name move to the type.
Parameter parameter(string_view raw) {
    const string text = collapse(raw);
    const size_t n = text.size();
    for (size_t k = n; k >= 1; --k) {
        const char last = text[k - 1];
        if (!(is_word(last) || last == '*' || last == ']')) continue;
        size_t q = k;
        while (q < n && text::is_space(text[q])) ++q;
        if (q == k || q >= n || !is_first(text[q])) continue;
        size_t r = q + 1;
        while (r < n && is_word(text[r])) ++r;
        size_t t = r;
        for (;;) {   // array dimensions: (\s*\[\w*\])*
            size_t u = t;
            while (u < n && text::is_space(text[u])) ++u;
            if (u >= n || text[u] != '[') break;
            ++u;
            while (u < n && is_word(text[u])) ++u;
            if (u >= n || text[u] != ']') break;
            t = u + 1;
        }
        if (t != n) continue;
        const string type_part = text.substr(0, k);
        const string name = text.substr(q, r - q);
        const string dims = text.substr(r, n - r);
        static const std::set<string_view> type_words = {"int", "long", "short", "char", "unsigned", "signed", "void", "float",
                                                         "double", "const", "volatile", "far", "near", "struct", "union",
                                                         "enum", "huge", "_far", "__far", "_near", "__near"};
        const string lowered = text::lower(name);
        const bool all_upper = std::any_of(name.begin(), name.end(), is_upper) &&
                               std::none_of(name.begin(), name.end(), [](char c) { return c >= 'a' && c <= 'z'; });
        if (type_words.contains(lowered) || all_upper) break;   // the "name" is really part of the type
        bool says_something = false;                              // a word of the type that is not just a qualifier
        for (size_t a = 0; a < type_part.size();) {
            if (!is_word(type_part[a])) {
                ++a;
                continue;
            }
            size_t b = a;
            while (b < type_part.size() && is_word(type_part[b])) ++b;
            const string w = text::lower(string_view(type_part).substr(a, b - a));
            if (w != "const" && w != "volatile" && w != "far" && w != "near") says_something = true;
            a = b;
        }
        if (!says_something) break;
        return {string(text::trim(type_part + dims)), name};
    }
    return {text, ""};
}

std::vector<Parameter> parameters(string_view inside) {
    std::vector<Parameter> out;
    const string lowered = text::lower(inside);
    if (lowered.empty() || lowered == "void") return out;
    for (const string& p : split_parameters(inside)) out.push_back(parameter(p));
    return out;
}

// A header's text with line endings made \n.
string read_header_text(const std::filesystem::path& path) {
    const Bytes raw = fs::read_file(path);
    string out;
    out.reserve(raw.size());
    for (size_t i = 0; i < raw.size(); ++i) {
        if (raw[i] == '\r') {
            out.push_back('\n');
            if (i + 1 < raw.size() && raw[i + 1] == '\n') ++i;
        } else {
            out.push_back(static_cast<char>(raw[i]));
        }
    }
    return out;
}

// // and /* */ comments become one space each.
string blank_comments(const string& s) {
    string out;
    out.reserve(s.size());
    const size_t n = s.size();
    size_t i = 0;
    while (i < n) {
        if (s[i] == '/' && i + 1 < n && s[i + 1] == '/') {
            const size_t e = s.find('\n', i);
            i = e == string::npos ? n : e;
            out.push_back(' ');
            continue;
        }
        if (s[i] == '/' && i + 1 < n && s[i + 1] == '*') {
            const size_t e = s.find("*/", i + 2);
            if (e != string::npos) {
                i = e + 2;
                out.push_back(' ');
                continue;
            }
        }
        out.push_back(s[i++]);
    }
    return out;
}

// A preprocessor line (with its backslash continuations) becomes one space.
string blank_directives(const string& s) {
    string out;
    out.reserve(s.size());
    const size_t n = s.size();
    size_t i = 0;
    while (i < n) {
        if (i == 0 || s[i - 1] == '\n') {
            size_t p = i;
            while (p < n && (s[p] == ' ' || s[p] == '\t')) ++p;
            if (p < n && s[p] == '#') {
                ++p;
                for (;;) {   // whole lines ending in a backslash, then the rest of the line
                    const size_t e = s.find('\n', p);
                    if (e == string::npos) {
                        p = n;
                        break;
                    }
                    if (e > p && s[e - 1] == '\\') {
                        p = e + 1;
                        continue;
                    }
                    p = e;
                    break;
                }
                out.push_back(' ');
                i = p;
                continue;
            }
        }
        out.push_back(s[i++]);
    }
    return out;
}

// The `<result> <CONVENTION> name(...);` declarations of one header.
void read_declarations(const string& text, const string& header, PrototypeMap& out) {
    const size_t n = text.size();
    size_t pos = 0;
    while (pos < n) {
        const size_t start = pos;
        if (!is_first(text[start]) || (start > 0 && is_word(text[start - 1]))) {
            ++pos;
            continue;
        }
        size_t e = start + 1;
        while (e < n && is_word(text[e])) ++e;
        size_t f = e;
        while (f < n && text::is_space(text[f])) ++f;
        if (f >= n || text[f] != '(') {
            ++pos;
            continue;
        }
        const size_t params_start = f + 1;
        pos = params_start;
        const string name = text.substr(start, e - start);
        // the words before the name: identifiers and the punctuation that ends a declaration, in the 200 bytes before
        const size_t window = start >= 200 ? start - 200 : 0;
        // (quick refusal: when the last thing before the name is punctuation, the last word is not a calling convention)
        bool last_is_punctuation = true;
        for (size_t a = start; a > window; --a) {
            const char c = text[a - 1];
            if (c == '*' || c == ';' || c == '{' || c == '}' || c == '(' || c == ')' || c == '=' || c == ',') break;
            if (is_word(c)) {
                last_is_punctuation = false;
                break;
            }
        }
        if (last_is_punctuation) continue;
        std::vector<string> words;
        for (size_t a = window; a < start;) {
            const char c = text[a];
            if (is_first(c)) {
                size_t b = a + 1;
                while (b < start && is_word(text[b])) ++b;
                words.push_back(text.substr(a, b - a));
                a = b;
            } else {
                if (c == '*' || c == ';' || c == '{' || c == '}' || c == '(' || c == ')' || c == '=' || c == ',') {
                    words.emplace_back(1, c);
                }
                ++a;
            }
        }
        if (words.empty() || !convention_of_word(name).empty()) continue;
        const string& marker = words.back();
        string_view convention = convention_of_word(marker);
        if (convention.empty() && is_api_macro(marker, "API", 4)) convention = "stdcall";
        else if (convention.empty() && is_api_macro(marker, "APIV", 5)) convention = "cdecl";
        if (convention.empty()) continue;
        std::vector<string> result;   // the result's words, back to the previous declaration's end
        for (size_t k = words.size() - 1; k-- > 0;) {
            const string& w = words[k];
            if (w.size() == 1 && string_view(";{}()=,").find(w[0]) != string_view::npos) break;
            result.push_back(w);
        }
        std::reverse(result.begin(), result.end());
        std::vector<string> kept;
        bool is_typedef = false;
        for (const string& w : result) {
            if (not_a_type(w) || !convention_of_word(w).empty() || text::iequals(w, "FAR") || text::iequals(w, "NEAR")) continue;
            kept.push_back(w);
            if (w == "typedef") is_typedef = true;
        }
        int depth = 0;
        size_t close = params_start - 1;
        while (close < n) {
            depth += text[close] == '(';
            depth -= text[close] == ')';
            if (depth == 0) break;
            ++close;
        }
        if (depth != 0) continue;
        {   // a ';' within the next dozen bytes
            size_t a = close + 1;
            const size_t limit = std::min(n, close + 12);
            while (a < limit && text::is_space(text[a])) ++a;
            if (a >= limit || text[a] != ';') continue;
        }
        if (is_typedef) continue;
        Prototype p;
        p.returns = text::join(kept, " ");
        if (p.returns.empty()) p.returns = is_com_marker(marker) ? "HRESULT" : "void";
        p.convention = string(convention);
        p.marker = marker;
        p.parameters = parameters(collapse(string_view(text).substr(params_start, close - params_start)));
        p.header = header;
        out.try_emplace(name, std::move(p));
    }
}

// COM's `STDAPI_(type) name(params);` form.
void read_com_declarations(const string& text, const string& header, PrototypeMap& out) {
    static constexpr string_view kMarkers[] = {"STDAPI", "WINOLEAPI", "WINOLEAUTAPI"};
    const size_t n = text.size();
    size_t pos = 0;
    while (pos < n) {
        const size_t start = pos;
        if (start > 0 && is_word(text[start - 1])) {
            ++pos;
            continue;
        }
        size_t p = string::npos;
        for (const string_view m : kMarkers) {
            if (string_view(text).substr(start, m.size()) == m && start + m.size() + 1 < n && text[start + m.size()] == '_' &&
                text[start + m.size() + 1] == '(') {
                p = start + m.size() + 2;
                break;
            }
        }
        if (p == string::npos) {
            ++pos;
            continue;
        }
        const size_t close = text.find(')', p);
        if (close == string::npos) {
            ++pos;
            continue;
        }
        const string result(text::trim(string_view(text).substr(p, close - p)));
        size_t q = close + 1;
        const size_t q0 = q;
        while (q < n && text::is_space(text[q])) ++q;
        if (q == q0 || q >= n || !is_first(text[q])) {
            ++pos;
            continue;
        }
        size_t r = q + 1;
        while (r < n && is_word(text[r])) ++r;
        const string name = text.substr(q, r - q);
        size_t s = r;
        while (s < n && text::is_space(text[s])) ++s;
        if (s >= n || text[s] != '(') {
            ++pos;
            continue;
        }
        const size_t list_start = s + 1;
        size_t t = list_start;
        size_t end = string::npos;
        for (; t < n; ++t) {
            const char ch = text[t];
            if (ch == ')') {
                size_t u = t + 1;
                while (u < n && text::is_space(text[u])) ++u;
                if (u < n && text[u] == ';') {
                    end = u + 1;
                    break;
                }
            }
            if (ch == ';' || ch == '{' || ch == '}') break;
        }
        if (end == string::npos) {
            ++pos;
            continue;
        }
        Prototype proto;
        proto.returns = result;
        proto.convention = "stdcall";
        proto.marker = "STDAPI";
        proto.parameters = parameters(collapse(string_view(text).substr(list_start, t - list_start)));
        proto.header = header;
        out.try_emplace(name, std::move(proto));
        pos = end;
    }
}

PrototypeMap read_header(const std::filesystem::path& path, const string& header) {
    PrototypeMap out;
    string text;
    try {
        text = read_header_text(path);
    } catch (const IoError&) {   // an unreadable header contributes nothing
        return out;
    }
    text = blank_directives(blank_comments(text));
    read_declarations(text, header, out);
    read_com_declarations(text, header, out);
    return out;
}

// The headers (lower-cased names, sorted) of a folder.
std::vector<std::filesystem::path> header_files(const std::filesystem::path& dir) {
    std::vector<std::filesystem::path> files;
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(dir, ec)) {
        const string name = text::lower(fs::to_text(entry.path().filename()));
        if (name.ends_with(".h") && name.find("win386") == string::npos) files.push_back(entry.path());
    }
    // by the names as written (capitals before small letters): which header declares a name first decides whose
    // declaration is kept
    std::sort(files.begin(), files.end(),
              [](const auto& a, const auto& b) { return fs::to_text(a.filename()) < fs::to_text(b.filename()); });
    return files;
}

PrototypeMap read_folders(const std::vector<std::filesystem::path>& folders) {
    PrototypeMap table;
    for (size_t rank = 0; rank < folders.size(); ++rank) {
        for (const auto& file : header_files(folders[rank])) {
            const string header = text::lower(fs::to_text(file.filename()));
            for (auto& [name, proto] : read_header(file, header)) {
                const auto [it, fresh] = table.try_emplace(name, proto);
                if (fresh || rank == 0) continue;
                // a later folder (Microsoft's) names the parameters the first one left unnamed
                Prototype& known = it->second;
                if (known.parameters.size() != proto.parameters.size()) continue;
                const bool named = std::any_of(known.parameters.begin(), known.parameters.end(),
                                               [](const Parameter& p) { return !p.name.empty(); });
                if (named) continue;
                for (size_t k = 0; k < known.parameters.size(); ++k) known.parameters[k].name = proto.parameters[k].name;
            }
        }
    }
    return table;
}

bool is_upper_text(string_view s) {   // has a capital and no small letter
    return std::any_of(s.begin(), s.end(), is_upper) &&
           std::none_of(s.begin(), s.end(), [](char c) { return c >= 'a' && c <= 'z'; });
}

}  // namespace

HeaderFolders HeaderFolders::standard(const std::filesystem::path& openwatcom_root, const std::filesystem::path& reference_root) {
    const auto existing = [](std::initializer_list<std::filesystem::path> dirs) {
        std::vector<std::filesystem::path> out;
        for (const auto& d : dirs) {
            if (!d.empty() && std::filesystem::is_directory(d)) out.push_back(d);
        }
        return out;
    };
    HeaderFolders f;
    f.w32 = existing({openwatcom_root.empty() ? std::filesystem::path() : openwatcom_root / "h" / "nt",
                      reference_root / "WIN32SDK" / "MSTOOLS" / "INCLUDE", reference_root / "Microsoft Windows 95 DDK" / "INC32"});
    f.w16 = existing({openwatcom_root.empty() ? std::filesystem::path() : openwatcom_root / "h" / "win",
                      reference_root / "Microsoft Windows 95 DDK" / "INC16"});
    return f;
}

std::string stamp(const HeaderFolders& folders) {
    std::vector<string> parts = {std::to_string(kReaderRevision)};
    const auto add = [&](const std::vector<std::filesystem::path>& dirs) {
        for (const auto& d : dirs) {
            std::error_code ec;
            std::size_t count = 0;
            for (const auto& entry : std::filesystem::directory_iterator(d, ec)) {
                if (text::iends_with(fs::to_text(entry.path().filename()), ".h")) ++count;
            }
            const auto mtime = std::filesystem::last_write_time(d, ec);
            const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(mtime.time_since_epoch()).count();
            parts.push_back(std::format("{}:{}:{}", fs::to_text(d), count, seconds));
        }
    };
    add(folders.w16);
    add(folders.w32);
    return text::join(parts, "|");
}

Headers read_headers(const HeaderFolders& folders) {
    return {read_folders(folders.w32), read_folders(folders.w16)};
}

PrototypeTable::PrototypeTable(Headers headers) : headers_(std::move(headers)) {
    for (const auto& [name, proto] : headers_.w32) spelling_[text::upper(name)] = name;
    for (const auto& [name, proto] : headers_.w16) spelling_[text::upper(name)] = name;   // the 16-bit spelling wins: that is where it is needed
}

const Prototype* PrototypeTable::of(std::string_view name, Bits bits) const {
    const string_view platform = bits == Bits::W16 ? "win16" : "win32";
    const PrototypeMap& protos = map(bits);
    const string key(name);
    const Prototype* found = protos.find(key);
    knowledge::record(platform, "prototype", name);   // (the step asking is made from it)
    if (found == nullptr) {
        const auto it = spelling_.find(text::upper(name));
        if (it != spelling_.end() && it->second != name) {
            knowledge::record(platform, "prototype", it->second);
            found = protos.find(it->second);
        }
    }
    return found;
}

std::string PrototypeTable::spelling(std::string_view name) const {
    if (name.empty() || !is_upper_text(name)) return string(name);
    if (const auto it = spelling_.find(string(name)); it != spelling_.end()) return it->second;
    if (name.starts_with('_')) {
        if (const auto it = spelling_.find(string(name.substr(1))); it != spelling_.end()) return it->second;
    }
    return string(name);
}

std::optional<std::string> PrototypeTable::declaration(std::string_view name, Bits bits) const {
    const Prototype* p = of(name, bits);
    if (p == nullptr) return std::nullopt;
    std::vector<string> params;
    for (const Parameter& a : p->parameters) params.emplace_back(text::trim(a.type + " " + a.name));
    string joined = text::join(params, ", ");
    if (joined.empty()) joined = "void";
    string real(name);
    if (!map(bits).contains(real)) {
        if (const auto it = spelling_.find(text::upper(name)); it != spelling_.end()) real = it->second;
    }
    return p->returns + " " + p->marker + " " + real + "(" + joined + ")";
}

}  // namespace cb::symbols::prototypes

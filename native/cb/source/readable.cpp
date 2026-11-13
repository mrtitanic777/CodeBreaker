#include "cb/source/readable.hpp"

#include <algorithm>
#include <bit>
#include <filesystem>
#include <format>
#include <set>
#include <unordered_map>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::source::readable {

namespace stdfs = std::filesystem;
using symbols::prototypes::Prototype;

namespace {

// ---- characters and words ------------------------------------------------------------------------------------------------------

bool word_char(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }
bool ident_start(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
bool digit(char c) { return c >= '0' && c <= '9'; }
bool blank(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f'; }

// The maximal runs of word characters that start like a C name: the identifiers of a text, with where each starts.
struct Word {
    std::size_t at = 0;
    std::string_view text;
};
std::vector<Word> words_of(std::string_view text) {
    std::vector<Word> out;
    for (std::size_t i = 0; i < text.size();) {
        if (!word_char(text[i])) {
            ++i;
            continue;
        }
        std::size_t j = i;
        while (j < text.size() && word_char(text[j])) ++j;
        if (ident_start(text[i])) out.push_back({i, text.substr(i, j - i)});
        i = j;
    }
    return out;
}

// A number as C writes it, read with the rules of base 0 (0x.. hex; a decimal has no leading zero unless it is zero), the low 32 bits of it.
// nullopt: not a number of that kind.
std::optional<std::uint32_t> number_low32(std::string_view s) {
    bool negative = false;
    if (!s.empty() && s.front() == '-') {
        negative = true;
        s.remove_prefix(1);
    }
    std::uint64_t v = 0;
    if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        for (const char c : s.substr(2)) {
            int d;
            if (digit(c)) d = c - '0';
            else if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
            else return std::nullopt;
            v = v * 16 + static_cast<std::uint64_t>(d);
        }
    } else {
        if (s.empty()) return std::nullopt;
        for (const char c : s) {
            if (!digit(c)) return std::nullopt;
            v = v * 10 + static_cast<std::uint64_t>(c - '0');
        }
        if (s.size() > 1 && s[0] == '0' && v != 0) return std::nullopt;   // (0755 is not a decimal)
    }
    if (negative) v = 0 - v;
    return static_cast<std::uint32_t>(v & 0xFFFFFFFFu);
}

// What a call's argument is, as a whole number: `0x40`, `(int4)-16`, `10u` ...
const Regex& number_form() {
    static const Regex re(R"(^\(?\s*(?:\(\s*[A-Za-z_][\w ]*\*?\s*\)\s*)?(-?(?:0x[0-9a-fA-F]+|\d+))[uUlL]*\s*\)?$)");
    return re;
}
// The bare name an argument is (a cast in front is allowed): group 1.
const Regex& bare_name_form() {
    static const Regex re(R"(^\s*(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*$)");
    return re;
}
// The function a text calls first (a cast in front is allowed): group 1.
const Regex& first_call_form() {
    static const Regex re(R"(^\s*(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*\()");
    return re;
}
const Regex& placeholder_form() {
    static const Regex re(R"(^(?:[a-z]{1,4}Var\d+|local_[0-9a-fA-F]+|a\d+(?:_\d+)?|param_\d+|in_\w+|extraout_\w+)$)");
    return re;
}

std::string without_ending(std::string_view function) {   // `lstrcpyA` -> `lstrcpy`: an A or W after a lower-case letter
    std::string base(function);
    if (base.size() >= 2 && (base.back() == 'A' || base.back() == 'W') && base[base.size() - 2] >= 'a' && base[base.size() - 2] <= 'z') base.pop_back();
    return base;
}
std::string family_name(std::string_view function) { return text::lower(without_ending(function)); }

bool is_keyword_call(std::string_view name) {
    return name == "if" || name == "while" || name == "for" || name == "switch" || name == "return" || name == "sizeof";
}

// ---- the parameters whose values the SDK names ----------------------------------------------------------------------------------

struct Family {
    std::vector<std::string> prefixes;
    bool flags = false;   // an OR of constants; else one constant
};

const std::map<std::pair<std::string, std::string>, Family>& families() {
    static const std::map<std::pair<std::string, std::string>, Family> table = [] {
        std::map<std::pair<std::string, std::string>, Family> t;
        const auto add = [&t](std::string function, std::string parameter, std::vector<std::string> prefixes, bool flags) {
            t[{std::move(function), std::move(parameter)}] = Family{std::move(prefixes), flags};
        };
        add("messagebox", "uType", {"MB_"}, true);
        add("messageboxex", "uType", {"MB_"}, true);
        add("messagebeep", "uType", {"MB_"}, false);
        add("localalloc", "uFlags", {"LMEM_"}, true);
        add("localrealloc", "uFlags", {"LMEM_"}, true);
        add("globalalloc", "uFlags", {"GMEM_"}, true);
        add("globalrealloc", "uFlags", {"GMEM_"}, true);
        add("heapalloc", "dwFlags", {"HEAP_"}, true);
        add("heaprealloc", "dwFlags", {"HEAP_"}, true);
        add("heapfree", "dwFlags", {"HEAP_"}, true);
        add("heapcreate", "flOptions", {"HEAP_"}, true);
        add("virtualalloc", "flAllocationType", {"MEM_"}, true);
        add("virtualalloc", "flProtect", {"PAGE_"}, true);
        add("virtualfree", "dwFreeType", {"MEM_"}, true);
        add("virtualprotect", "flNewProtect", {"PAGE_"}, true);
        add("showwindow", "nCmdShow", {"SW_"}, false);
        add("showwindowasync", "nCmdShow", {"SW_"}, false);
        add("sendmessage", "Msg", {"WM_"}, false);
        add("postmessage", "Msg", {"WM_"}, false);
        add("senddlgitemmessage", "Msg", {"WM_"}, false);
        add("defwindowproc", "Msg", {"WM_"}, false);
        add("callwindowproc", "Msg", {"WM_"}, false);
        add("postthreadmessage", "Msg", {"WM_"}, false);
        add("createwindowex", "dwExStyle", {"WS_EX_"}, true);
        add("createwindowex", "dwStyle", {"WS_"}, true);
        add("setwindowpos", "uFlags", {"SWP_"}, true);
        add("getwindowlong", "nIndex", {"GWL_"}, false);
        add("setwindowlong", "nIndex", {"GWL_"}, false);
        add("getclasslong", "nIndex", {"GCL_"}, false);
        add("setclasslong", "nIndex", {"GCL_"}, false);
        add("getsystemmetrics", "nIndex", {"SM_"}, false);
        add("getsyscolor", "nIndex", {"COLOR_"}, false);
        add("getsyscolorbrush", "nIndex", {"COLOR_"}, false);
        add("getdevicecaps", "index", {"HORZRES", "VERTRES", "LOGPIXELS", "BITSPIXEL", "PLANES", "NUMCOLORS", "RASTERCAPS", "TECHNOLOGY", "SIZEPALETTE"}, false);
        add("drawtext", "uFormat", {"DT_"}, true);
        add("drawtextex", "dwDTFormat", {"DT_"}, true);
        add("peekmessage", "wRemoveMsg", {"PM_"}, true);
        add("trackpopupmenu", "uFlags", {"TPM_"}, true);
        add("trackpopupmenuex", "fuFlags", {"TPM_"}, true);
        add("enablemenuitem", "uEnable", {"MF_"}, true);
        add("checkmenuitem", "uCheck", {"MF_"}, true);
        add("appendmenu", "uFlags", {"MF_"}, true);
        add("insertmenu", "uFlags", {"MF_"}, true);
        add("modifymenu", "uFlags", {"MF_"}, true);
        add("deletemenu", "uFlags", {"MF_"}, true);
        add("removemenu", "uFlags", {"MF_"}, true);
        add("checkdlgbutton", "uCheck", {"BST_"}, false);
        add("regopenkeyex", "samDesired", {"KEY_"}, true);
        add("regcreatekeyex", "samDesired", {"KEY_"}, true);
        add("regcreatekeyex", "dwOptions", {"REG_OPTION_"}, true);
        add("regsetvalueex", "dwType", {"REG_"}, false);
        add("regopenkeyex", "hKey", {"HKEY_"}, false);
        add("regcreatekeyex", "hKey", {"HKEY_"}, false);
        add("regopenkey", "hKey", {"HKEY_"}, false);
        add("regcreatekey", "hKey", {"HKEY_"}, false);
        add("regqueryvalue", "hKey", {"HKEY_"}, false);
        add("regsetvalue", "hKey", {"HKEY_"}, false);
        add("regdeletekey", "hKey", {"HKEY_"}, false);
        add("regenumkey", "hKey", {"HKEY_"}, false);
        add("createfile", "dwDesiredAccess", {"GENERIC_"}, true);
        add("createfile", "dwShareMode", {"FILE_SHARE_"}, true);
        add("createfile", "dwFlagsAndAttributes", {"FILE_ATTRIBUTE_", "FILE_FLAG_"}, true);
        add("setfileattributes", "dwFileAttributes", {"FILE_ATTRIBUTE_"}, true);
        add("setfilepointer", "dwMoveMethod", {"FILE_BEGIN", "FILE_CURRENT", "FILE_END"}, false);
        add("formatmessage", "dwFlags", {"FORMAT_MESSAGE_"}, true);
        add("loadimage", "uType", {"IMAGE_"}, false);
        add("loadimage", "fuLoad", {"LR_"}, true);
        add("copyimage", "uType", {"IMAGE_"}, false);
        add("copyimage", "fuFlags", {"LR_"}, true);
        add("setbkmode", "iBkMode", {"TRANSPARENT", "OPAQUE"}, false);
        add("getstockobject", "i",
            {"WHITE_BRUSH", "LTGRAY_BRUSH", "GRAY_BRUSH", "DKGRAY_BRUSH", "BLACK_BRUSH", "NULL_BRUSH", "HOLLOW_BRUSH", "WHITE_PEN", "BLACK_PEN", "NULL_PEN",
             "OEM_FIXED_FONT", "ANSI_FIXED_FONT", "ANSI_VAR_FONT", "SYSTEM_FONT", "DEVICE_DEFAULT_FONT", "DEFAULT_PALETTE", "SYSTEM_FIXED_FONT", "DEFAULT_GUI_FONT"},
            false);
        add("waitforsingleobject", "dwMilliseconds", {"INFINITE"}, false);
        add("waitformultipleobjects", "dwMilliseconds", {"INFINITE"}, false);
        add("setwindowshookex", "idHook", {"WH_"}, false);
        add("redrawwindow", "flags", {"RDW_"}, true);
        add("getwindow", "uCmd", {"GW_"}, false);
        add("seterrormode", "uMode", {"SEM_"}, true);
        add("getfiletype", "", {}, false);
        return t;
    }();
    return table;
}

const Family* family_for(std::string_view base, std::string_view parameter) {
    const auto found = families().find({std::string(base), std::string(parameter)});
    return found == families().end() ? nullptr : &found->second;
}

// Families whose prefix would also catch the names of another family: those are left out of it.
std::vector<std::string> narrower_than(const std::string& prefix) {
    if (prefix == "WS_") return {"WS_EX_"};
    if (prefix == "MB_") return {"MB_ERR_"};
    if (prefix == "SW_") return {"SW_ERASE", "SW_INVALIDATE", "SW_SCROLLCHILDREN"};
    if (prefix == "REG_") return {"REG_OPTION_", "REG_CREATED_", "REG_OPENED_", "REG_NOTIFY_", "REG_LEGAL_", "REG_WHOLE_", "REG_REFRESH_", "REG_NO_"};
    return {};
}

// ---- the SDK's #defines --------------------------------------------------------------------------------------------------------

// A header as text with a line continued by a backslash made one line.
std::string joined_lines(std::string text) {
    static const Regex continued(R"(\\[ \t]*\r?\n)");
    return continued.replace_all(text, " ");
}

// `#define NAME 123` / `#define NAME (0x10)` / `#define NAME ((UINT)-1)`: group 1 the name, group 2 the number.
const Regex& top_define() {
    static const Regex re(R"(^#define\s+([A-Z][A-Z0-9_]+)\s+\(?\s*(?:\(\s*\w+\s*\)\s*)?(-?\s*(?:0x[0-9A-Fa-f]+|\d+))[uUlL]*\s*\)?\s*(?:/[/*].*)?$)", RegexFlags::Multiline);
    return re;
}
// `#define NAME (A | B)`: group 1 the name, group 2 the OR.
const Regex& or_define() {
    static const Regex re(R"(^#define\s+([A-Z][A-Z0-9_]+)\s+\(\s*([A-Z][A-Z0-9_]*(?:\s*\|\s*[A-Z][A-Z0-9_]*)+)\s*\)\s*(?:/[/*].*)?$)", RegexFlags::Multiline);
    return re;
}

std::vector<Define> read_defines(const symbols::prototypes::HeaderFolders& folders) {
    std::vector<Define> out;
    std::map<std::string, std::uint32_t, std::less<>> values;
    struct Later {
        std::string name, expression, header;
    };
    std::vector<Later> later;
    for (const std::vector<stdfs::path>* bits : {&folders.w32, &folders.w16}) {
        for (const stdfs::path& folder : *bits) {
            std::error_code ec;
            if (!stdfs::is_directory(folder, ec)) continue;
            std::vector<std::string> names;
            for (const auto& entry : stdfs::directory_iterator(folder, ec)) names.push_back(fs::to_text(entry.path().filename()));
            std::sort(names.begin(), names.end());
            for (const std::string& file : names) {
                if (!text::iends_with(file, ".h")) continue;
                std::string body;
                try {
                    body = text::from_latin1(fs::read_file(folder / fs::from_text(file)));
                } catch (const Error&) {   // a header that cannot be read is skipped
                    continue;
                }
                const std::string header = text::lower(file);
                const std::string joined = joined_lines(std::move(body));
                // Only a line that starts with `#define` can be one (the patterns are anchored at a line's start), so those are all that is matched.
                std::vector<Later> ors;
                for (std::size_t line = 0; line < joined.size();) {
                    if (std::string_view(joined).substr(line, 7) == "#define") {
                        if (const auto m = top_define().match(joined, line)) {
                            const std::string name(m->group(1));
                            std::string digits(m->group(2));
                            std::erase(digits, ' ');
                            const std::optional<std::uint32_t> v = number_low32(digits);
                            if (!values.contains(name) && v) {
                                values[name] = *v;
                                out.push_back({name, *v, header});
                            }
                        } else if (const auto combined = or_define().match(joined, line)) {
                            ors.push_back({std::string(combined->group(1)), std::string(combined->group(2)), header});
                        }
                    }
                    const std::size_t newline = joined.find('\n', line);
                    if (newline == std::string::npos) break;
                    line = newline + 1;
                }
                later.insert(later.end(), ors.begin(), ors.end());
            }
        }
    }
    for (const Later& l : later) {
        std::vector<std::string> parts;
        for (const std::string_view part : text::split(l.expression, '|')) parts.emplace_back(text::trim(part));
        if (values.contains(l.name)) continue;
        std::uint32_t v = 0;
        bool known = true;
        for (const std::string& part : parts) {
            const auto found = values.find(part);
            if (found == values.end()) {
                known = false;
                break;
            }
            v |= found->second;
        }
        if (!known) continue;
        values[l.name] = v;
        out.push_back({l.name, v, l.header});
    }
    return out;
}

// What a pass reads of a family: the constants of the headers whose names start with one of the prefixes, from `header` (the function's own header)
// when it defines any, so MB_ for MessageBox is winuser.h's, not winnls.h's.
std::vector<std::pair<std::string, std::uint32_t>> members_of(const std::vector<Define>& defines, const std::vector<std::string>& prefixes, std::string_view header) {
    std::vector<std::pair<std::string, std::uint32_t>> own, all;
    for (const Define& d : defines) {
        const bool wanted = std::any_of(prefixes.begin(), prefixes.end(), [&](const std::string& p) { return d.name.starts_with(p); });
        if (!wanted) continue;
        bool narrower = false;
        for (const std::string& p : prefixes)
            for (const std::string& x : narrower_than(p)) narrower = narrower || d.name.starts_with(x);
        if (narrower) continue;
        static constexpr std::string_view kEnds[] = {"MASK", "_FIRST", "_LAST", "_MAX", "_MIN", "_RESERVED"};
        if (std::any_of(std::begin(kEnds), std::end(kEnds), [&](std::string_view e) { return d.name.ends_with(e); })) continue;
        all.emplace_back(d.name, d.value);
        if (!header.empty() && d.header == header) own.emplace_back(d.name, d.value);
    }
    return own.empty() ? all : own;
}

}  // namespace

// ---- the SDK -------------------------------------------------------------------------------------------------------------------

Sdk::Sdk(std::vector<Define> defines, const symbols::prototypes::PrototypeTable& prototypes, symbols::sdk_structs::StructTable structures)
    : defines_(std::move(defines)), prototypes_(&prototypes), structures_(std::move(structures)) {
    for (const Define& d : defines_) values_.emplace(d.name, d.value);
}

Sdk Sdk::read(const symbols::prototypes::HeaderFolders& folders, const symbols::prototypes::PrototypeTable& prototypes) {
    std::vector<Define> defines = read_defines(folders);
    symbols::sdk_structs::Constants constants;
    for (const Define& d : defines) constants.emplace(d.name, d.value);
    return Sdk(std::move(defines), prototypes, symbols::sdk_structs::StructTable(symbols::sdk_structs::read_headers(folders.w32, constants)));
}

std::optional<std::uint32_t> Sdk::value(std::string_view name) const {
    const auto found = values_.find(name);
    if (found == values_.end()) return std::nullopt;
    return found->second;
}

const Prototype* Sdk::prototype(std::string_view name) const {
    if (const Prototype* p = prototypes_->of(name)) return p;
    return prototypes_->of(without_ending(name));
}

const std::vector<std::string>& Sdk::family_of(std::string_view function, std::string_view parameter) {
    static const std::vector<std::string> none;
    const Family* family = family_for(family_name(function), parameter);
    return family ? family->prefixes : none;
}

std::optional<std::string> Sdk::constant(std::string_view function, std::string_view parameter, std::int64_t value) const {
    const std::string base = family_name(function);
    const Family* family = family_for(base, parameter);
    if (!family || family->prefixes.empty()) return std::nullopt;
    const Prototype* proto = prototype(function);
    std::vector<std::pair<std::string, std::uint32_t>> members = members_of(defines_, family->prefixes, proto ? text::lower(proto->header) : std::string());
    const std::uint32_t wanted = static_cast<std::uint32_t>(static_cast<std::uint64_t>(value) & 0xFFFFFFFFu);
    for (const auto& [name, v] : members)
        if (v == wanted) return name;   // the first the headers give that value
    if (!family->flags || wanted == 0) return std::nullopt;

    // an OR of constants: the ones with most bits first, each taking what is left
    std::vector<std::pair<std::string, std::uint32_t>> by_bits = members;
    std::stable_sort(by_bits.begin(), by_bits.end(), [](const auto& a, const auto& b) {
        const int pa = std::popcount(a.second), pb = std::popcount(b.second);
        if (pa != pb) return pa > pb;
        return a.second > b.second;
    });
    std::uint32_t left = wanted;
    std::vector<std::pair<std::uint32_t, std::string>> picked;
    for (const auto& [name, v] : by_bits) {
        if (v != 0 && (v & left) == v) {
            picked.emplace_back(v, name);
            left &= ~v;
        }
        if (left == 0) break;
    }
    if (left != 0 || picked.size() < 2) return std::nullopt;
    if (family->prefixes.size() == 1 && family->prefixes.front() == "WS_") {   // a child window's 0x20000/0x10000 are WS_GROUP and WS_TABSTOP, a
        const bool child = (wanted & 0x40000000u) != 0;                       // top-level one's the minimize and maximize boxes
        const std::map<std::uint32_t, std::string> mean = {{0x20000u, child ? "WS_GROUP" : "WS_MINIMIZEBOX"}, {0x10000u, child ? "WS_TABSTOP" : "WS_MAXIMIZEBOX"}};
        for (auto& [v, name] : picked) {
            const auto m = mean.find(v);
            if (m != mean.end()) name = m->second;
        }
        std::erase_if(members, [&](const auto& m) {
            const auto it = mean.find(m.second);
            return it != mean.end() && m.first != it->second;
        });
    }
    for (const auto& [v, name] : picked) {   // a part two names share: which was meant is not in the number, so the number stays
        std::set<std::string> sharing;
        for (const auto& [m, w] : members)
            if (w == v) sharing.insert(m);
        if (sharing.size() > 1) return std::nullopt;
    }
    std::sort(picked.begin(), picked.end(), [](const auto& a, const auto& b) { return a > b; });
    std::string joined;
    for (const auto& [v, name] : picked) joined += (joined.empty() ? "" : " | ") + name;
    return joined;
}

// ---- reading calls ---------------------------------------------------------------------------------------------------------------

std::vector<Call> calls(std::string_view text) {
    std::vector<Call> out;
    for (std::size_t at = 0; at < text.size();) {
        if (!word_char(text[at])) {
            ++at;
            continue;
        }
        std::size_t end = at;
        while (end < text.size() && word_char(text[end])) ++end;
        const std::string_view name = text.substr(at, end - at);
        std::size_t paren = end;
        while (paren < text.size() && blank(text[paren])) ++paren;
        if (!ident_start(name.front()) || paren >= text.size() || text[paren] != '(') {
            at = end;
            continue;
        }
        const std::size_t i = paren + 1;
        at = i;   // the next call is looked for after this one's '(' (its arguments hold calls too)
        if (is_keyword_call(name)) continue;
        int depth = 1;
        std::size_t start = i, j = i;
        std::vector<std::string> args;
        while (j < text.size() && depth != 0) {
            const char c = text[j];
            if (c == '(' || c == '[') {
                ++depth;
            } else if (c == ')' || c == ']') {
                --depth;
                if (depth == 0) {
                    args.emplace_back(text.substr(start, j - start));
                    break;
                }
            } else if (c == ',' && depth == 1) {
                args.emplace_back(text.substr(start, j - start));
                start = j + 1;
            } else if (c == '"') {
                ++j;
                while (j < text.size() && text[j] != '"') j += text[j] == '\\' ? 2 : 1;
            }
            ++j;
        }
        if (depth == 0) out.push_back({std::string(name), i, j, std::move(args)});
    }
    return out;
}

namespace {

// ---- names for placeholders ------------------------------------------------------------------------------------------------------

const std::map<std::string, std::string, std::less<>>& handle_names() {
    static const std::map<std::string, std::string, std::less<>> table = {
        {"HWND", "hwnd"},   {"HDC", "hdc"},       {"HGLOBAL", "hMem"},   {"HLOCAL", "hMem"}, {"HMENU", "hmenu"},          {"HINSTANCE", "hinst"},
        {"HMODULE", "hmod"}, {"HBRUSH", "hbr"},   {"HPEN", "hpen"},      {"HFONT", "hfont"}, {"HBITMAP", "hbm"},          {"HICON", "hicon"},
        {"HCURSOR", "hcur"}, {"HKEY", "hkey"},    {"HANDLE", "h"},       {"HRGN", "hrgn"},   {"HPALETTE", "hpal"},        {"HGDIOBJ", "hobj"},
        {"HACCEL", "haccel"}, {"HRSRC", "hrsrc"}, {"HFILE", "hf"},       {"HDWP", "hdwp"},   {"HIMAGELIST", "himl"},      {"HTREEITEM", "hitem"},
        {"HPROPSHEETPAGE", "hpage"}};
    return table;
}

bool is_c_keyword(const std::string& name) {
    static const std::set<std::string, std::less<>> keywords = {"auto",   "break",  "case",   "char",     "const",   "continue", "default", "do",     "double",
                                                                "else",   "enum",   "extern", "float",    "for",     "goto",     "if",      "int",    "long",
                                                                "register", "return", "short", "signed",  "sizeof",  "static",   "struct",  "switch", "typedef",
                                                                "union",  "unsigned", "void", "volatile", "while",   "far",      "near",    "huge",   "pascal",
                                                                "cdecl",  "i",      "j",      "k"};
    return keywords.contains(name);
}

// What a placeholder is assigned: the texts after `var =` up to the ';' (not `==`, not a member `x.var`).
std::vector<std::string> assignments_of(std::string_view text, std::string_view var) {
    const Regex re(std::format(R"((?<![\w.>]){}\s*=(?!=)\s*([^;]+);)", var));
    std::vector<std::string> out;
    re.for_each_match(text, [&](const RegexMatch& m) {
        out.emplace_back(m.group(1));
        return true;
    });
    return out;
}

// The name every one of `values` is the result of calling, when they all call the same function.
std::optional<std::string> single_callee(const std::vector<std::string>& values) {
    std::set<std::string> callee;
    for (const std::string& v : values)
        if (const auto m = first_call_form().search(v)) callee.insert(std::string(m->group(1)));
    if (values.empty() || callee.size() != 1) return std::nullopt;
    for (const std::string& v : values) {
        const auto m = first_call_form().search(v);
        if (!m || m->group(1) != *callee.begin()) return std::nullopt;
    }
    return *callee.begin();
}

// {placeholder: name} for one function's locals and parameters.
std::map<std::string, std::string> local_names(const Sdk& sdk, std::string_view text) {
    std::set<std::string> used;
    const std::vector<Word> words = words_of(text);
    for (const Word& w : words) used.emplace(w.text);
    std::map<std::string, std::set<std::string>> as_param;   // var -> the parameter names it is passed as
    for (const Call& call : calls(text)) {
        const Prototype* p = sdk.prototype(call.function);
        if (!p) continue;
        for (std::size_t k = 0; k < call.args.size(); ++k) {
            const auto m = bare_name_form().search(text::trim(call.args[k]));
            if (!m) continue;
            const std::string var(m->group(1));
            if (placeholder_form().full_match(var) && k < p->parameters.size() && !p->parameters[k].name.empty()) as_param[var].insert(p->parameters[k].name);
        }
    }
    std::map<std::string, std::string> names;
    std::set<std::string> taken;   // the names given: wanted in names.values()
    std::set<std::string> vars;
    for (const Word& w : words) vars.emplace(w.text);
    for (const std::string& var : vars) {   // (sorted)
        if (!placeholder_form().full_match(var)) continue;
        std::string wanted;
        const std::vector<std::string> given = assignments_of(text, var);
        if (const auto callee = single_callee(given)) {
            const Prototype* p = sdk.prototype(*callee);
            if (p) {
                const auto handle = handle_names().find(p->returns);
                if (handle != handle_names().end()) wanted = handle->second;
            }
        }
        if (wanted.empty()) {
            const auto param = as_param.find(var);
            if (param != as_param.end() && param->second.size() == 1) wanted = *param->second.begin();
        }
        if (wanted.empty()) {
            const Regex counted(std::format(R"(\b{0}\s*=\s*{0}\s*\+\s*1\s*[;)])", var));
            const Regex compared(std::format(R"(\b{}\s*<)", var));
            if (counted.search(text) && compared.search(text)) wanted = "i";
        }
        if (wanted.empty()) continue;
        const std::string base = wanted;
        int n = 2;
        while (used.contains(wanted) || taken.contains(wanted) || (is_c_keyword(wanted) && wanted != "i" && wanted != "j" && wanted != "k")) {
            if (base == "i" && (wanted == "i" || wanted == "j")) {
                wanted = wanted == "i" ? "j" : "k";
                continue;
            }
            wanted = base + std::to_string(n++);
        }
        names[var] = wanted;
        taken.insert(wanted);
        used.insert(wanted);
    }
    return names;
}

// `text` with each whole word that is a key of `names` written as its value (not after '.' or '>', never inside a string).
std::string rename_segment(std::string_view text, const std::map<std::string, std::string>& names) {
    std::string out;
    out.reserve(text.size());
    std::size_t copied = 0;
    for (const Word& w : words_of(text)) {
        if (w.at > 0 && (text[w.at - 1] == '.' || text[w.at - 1] == '>')) continue;
        const auto found = names.find(std::string(w.text));
        if (found == names.end()) continue;
        out.append(text.substr(copied, w.at - copied));
        out += found->second;
        copied = w.at + w.text.size();
    }
    out.append(text.substr(copied));
    return out;
}

std::string rename_words(std::string_view text, const std::map<std::string, std::string>& names) {
    if (names.empty()) return std::string(text);
    static const Regex literal(R"("(?:\\.|[^"\\])*")");
    std::string out;
    std::size_t from = 0;
    literal.for_each_match(text, [&](const RegexMatch& m) {
        out += rename_segment(text.substr(from, m.start() - from), names);
        out.append(m.whole());
        from = m.end();
        return true;
    });
    out += rename_segment(text.substr(from), names);
    return out;
}

// ---- fields of SDK structures -------------------------------------------------------------------------------------------------------

std::optional<std::uint32_t> access_size(std::string_view type) {
    static const std::map<std::string, std::uint32_t, std::less<>> sizes = {
        {"int1", 1}, {"uint1", 1}, {"xunknown1", 1}, {"undefined1", 1}, {"char", 1}, {"byte", 1}, {"undefined", 1}, {"int2", 2}, {"uint2", 2},
        {"xunknown2", 2}, {"undefined2", 2}, {"short", 2}, {"int4", 4}, {"uint4", 4}, {"xunknown4", 4}, {"undefined4", 4}, {"float4", 4}, {"int", 4},
        {"uint", 4}, {"int8", 8}, {"uint8", 8}, {"xunknown8", 8}, {"undefined8", 8}, {"float8", 8}};
    const auto found = sizes.find(type);
    if (found == sizes.end()) return std::nullopt;
    return found->second;
}

// {variable: structure} for the pointers passed to Windows functions as one SDK structure.
std::map<std::string, std::string> struct_vars(const Sdk& sdk, std::string_view text) {
    std::map<std::string, std::set<std::optional<std::string>>> seen;
    for (const Call& call : calls(text)) {
        const Prototype* p = sdk.prototype(call.function);
        if (!p) continue;
        for (std::size_t k = 0; k < call.args.size(); ++k) {
            const auto m = bare_name_form().search(call.args[k]);
            if (!m || k >= p->parameters.size()) continue;
            seen[std::string(m->group(1))].insert(sdk.structures().pointed_to(p->parameters[k].type));
        }
    }
    std::map<std::string, std::string> out;
    for (const auto& [var, targets] : seen)
        if (targets.size() == 1 && targets.begin()->has_value()) out[var] = **targets.begin();
    return out;
}

// ---- the pass ------------------------------------------------------------------------------------------------------------------------

class Pass final : public module_writer::ReadablePass {
public:
    Pass(const Sdk& sdk, const translate::l2::Made& stored, StringAt string_at)
        : sdk_(sdk), texts_(stored.texts()), string_at_(std::move(string_at)), header_(stored.header()) {
        if (!string_at_) string_at_ = [](std::uint32_t) { return std::nullopt; };
        globals_ = global_names();
    }

    std::string function(const std::string& name) override {
        const std::string* source = texts_.find(name);
        if (!source) throw NotFoundError(stdfs::path(name), 2, "no such function in the module");
        std::string text = constants_in(*source);
        std::map<std::string, std::string> names = local_names(sdk_, text);
        for (const auto& [from, to] : globals_) names[from] = to;
        text = rename_words(text, names);
        text = compared_in(std::move(text));
        text = fields_in(std::move(text));
        return strings_in(std::move(text));
    }

    // The names of the module's functions that are called and whose value no caller uses: what they return is left over in a register, not a
    // result (their declared type stays, as the compiled code depends on it).
    bool results_unused(const std::string& name) const override {
        if (!unused_) unused_ = unused_results();
        return unused_->contains(name);
    }

    std::string calls_hint(std::string_view text) const override { return readable::calls_hint(sdk_, text); }
    std::string rename_globals(const std::string& header) const override { return rename_words(header, globals_); }
    bool constants_used() const override { return !constants_used_.empty(); }

    std::string definitions() const override {
        std::string out;
        for (const auto& [name, value] : constants_used_) out += std::format("#ifndef {0}\n#define {0} 0x{1:X}\n#endif\n", name, value);
        return out;
    }

    std::string struct_definitions() const override {
        std::string out;
        std::vector<std::string> done;
        for (const std::string& name : structs_used_) {
            const std::string text = sdk_.structures().definition(name, done);
            static const Regex defined(R"(\} (\w+);)");
            defined.for_each_match(text, [&](const RegexMatch& m) {
                done.emplace_back(m.group(1));
                return true;
            });
            out += text;
        }
        return out.empty() ? std::string() : "\n/* SDK structures, at the SDK's offsets */\n" + out;
    }

private:
    // A placeholder global assigned only GetProcAddress(h, "Name") is g_pfnName; one assigned only the handle a Windows function returns is g_<handle>.
    std::map<std::string, std::string> global_names() const {
        static const Regex global(R"(\b(?:[a-z]{1,4}Ram[0-9a-f]+|DAT_[0-9a-f]+|PTR_\w+)\b)");
        std::map<std::string, std::vector<std::string>> assigned;
        std::set<std::string> taken;
        for (const auto& [name, text] : texts_) {
            std::set<std::string> here;
            for (const std::string& g : global.find_all(text)) here.insert(g);
            for (const std::string& g : here)
                for (const std::string& value : assignments_of(text, g)) assigned[g].emplace_back(text::trim(value));
            for (const Word& w : words_of(text)) taken.emplace(w.text);
        }
        std::map<std::string, std::string> names;
        std::set<std::string> given;
        for (const auto& [g, values] : assigned) {
            std::string wanted;
            std::set<std::string> procs;
            bool all_proc = true;
            for (const std::string& v : values) {
                if (v.find("GetProcAddress") == std::string::npos) {
                    all_proc = false;
                    continue;
                }
                if (const auto proc = proc_name(v)) procs.insert(*proc);
            }
            if (procs.size() == 1 && all_proc) {
                wanted = "g_pfn" + *procs.begin();
            } else if (const auto callee = single_callee(values)) {
                std::set<std::string> heads;   // (what is written before the first '(' of each)
                for (const std::string& v : values) heads.insert(v.substr(0, v.find('(')));
                if (heads.size() == 1) {
                    const Prototype* p = sdk_.prototype(*callee);
                    if (p) {
                        const auto handle = handle_names().find(p->returns);
                        if (handle != handle_names().end()) wanted = "g_" + handle->second;
                    }
                }
            }
            if (wanted.empty()) continue;
            const std::string base = wanted;
            int n = 2;
            while (taken.contains(wanted) || given.contains(wanted)) wanted = base + std::to_string(n++);
            names[g] = wanted;
            given.insert(wanted);
        }
        return names;
    }

    // The function GetProcAddress(h, name) asks for: a string in the text, or the module's string at the address the text passes.
    std::optional<std::string> proc_name(const std::string& value) const {
        static const Regex ask(R"re(GetProcAddress\w*\s*\([^,]*,\s*(?:\([^()]*\)\s*)?(?:"(\w+)"|(-?(?:0x[0-9a-fA-F]+|\d+)))\s*\))re");
        const auto m = ask.search(value);
        if (!m) return std::nullopt;
        if (m->has(1)) return std::string(m->group(1));
        const std::optional<std::uint32_t> address = number_low32(m->group(2));
        if (!address) return std::nullopt;
        const std::optional<std::string> found = string_at_(*address);
        if (found && text::is_identifier(*found)) return found;
        return std::nullopt;
    }

    std::set<std::string> unused_results() const {
        static const Regex first_declared(R"(^[\w ]+?[\s*](\w+)\()", RegexFlags::Multiline);
        std::set<std::string> names;
        for (const auto& [name, text] : texts_)
            if (const auto m = first_declared.search(text)) names.emplace(m->group(1));
        std::set<std::string> called, used;
        for (const auto& [name, text] : texts_) {
            const std::size_t brace = text.find('{');
            const std::string_view body = brace == std::string::npos ? std::string_view(text) : std::string_view(text).substr(brace + 1);
            for (const Word& w : words_of(body)) {
                if (!names.contains(std::string(w.text))) continue;
                std::size_t paren = w.at + w.text.size();
                while (paren < body.size() && blank(body[paren])) ++paren;
                if (paren >= body.size() || body[paren] != '(') continue;
                called.emplace(w.text);
                const std::size_t newline = w.at == 0 ? std::string_view::npos : body.rfind('\n', w.at - 1);
                const std::size_t line_start = newline == std::string_view::npos ? 0 : newline + 1;
                const std::string_view before = text::trim(body.substr(line_start, w.at - line_start));
                if (!before.empty() && before != "}" && before != "{" && before != "else" && before != "do") used.emplace(w.text);
            }
        }
        std::set<std::string> out;
        for (const std::string& n : called)
            if (!used.contains(n)) out.insert(n);
        return out;
    }

    // Numbers a variable is compared with or switched on, as the SDK constants of the one parameter it is passed to Windows as.
    std::string compared_in(std::string text) {
        std::vector<std::pair<std::string, std::set<std::pair<std::string, std::string>>>> families_used;   // var -> (function, parameter); first met first
        for (const Call& call : calls(text)) {
            const Prototype* p = sdk_.prototype(call.function);
            if (!p) continue;
            for (std::size_t k = 0; k < call.args.size(); ++k) {
                const auto m = bare_name_form().search(call.args[k]);
                if (!m || k >= p->parameters.size()) continue;
                if (!family_for(family_name(call.function), p->parameters[k].name)) continue;
                const std::string var(m->group(1));
                auto at = std::find_if(families_used.begin(), families_used.end(), [&](const auto& e) { return e.first == var; });
                if (at == families_used.end()) at = families_used.insert(families_used.end(), {var, {}});
                at->second.insert({call.function, p->parameters[k].name});
            }
        }
        for (const auto& [var, uses] : families_used) {
            std::set<std::vector<std::string>> kinds;
            for (const auto& [f, q] : uses) kinds.insert(family_for(family_name(f), q)->prefixes);
            if (kinds.size() != 1) continue;
            const std::string function = uses.begin()->first, parameter = uses.begin()->second;   // (one of them: the first by name)
            const auto named = [&](std::string_view number) -> std::optional<std::string> {
                const std::optional<std::uint32_t> v = number_low32(number);
                if (!v) return std::nullopt;
                const std::optional<std::string> n = sdk_.constant(function, parameter, static_cast<std::int64_t>(*v));
                if (n && n->find(" | ") == std::string::npos) {
                    constants_used_[*n] = *sdk_.value(*n);
                    return n;
                }
                return std::nullopt;
            };
            const Regex equal(std::format(R"(((?<![\w.>]){}\s*)(==|!=)\s*(0x[0-9a-fA-F]+|\d+)\b)", var));
            text = equal.replace_all(text, [&](const RegexMatch& m) -> std::string {
                const std::optional<std::string> n = named(m.group(3));
                return n ? std::string(m.group(1)) + std::string(m.group(2)) + " " + *n : std::string(m.whole());
            });
            const Regex switched(std::format(R"(switch\s*\(\s*(?:\([^()]*\)\s*)?{}\s*\)\s*\{{)", var));
            std::vector<std::pair<std::size_t, std::size_t>> switches;   // each: where its '{' ends
            switched.for_each_match(text, [&](const RegexMatch& m) {
                switches.emplace_back(m.start(), m.end());
                return true;
            });
            for (auto sw = switches.rbegin(); sw != switches.rend(); ++sw) {
                int depth = 1;
                std::size_t i = sw->second;
                while (i < text.size() && depth != 0) {
                    if (text[i] == '{') ++depth;
                    else if (text[i] == '}') --depth;
                    ++i;
                }
                const std::string body = text.substr(sw->second, i - sw->second);
                static const Regex marker(R"([{}]|\bcase\s+(0x[0-9a-fA-F]+|\d+)\s*:)");
                int level = 0;
                std::string rebuilt;
                std::size_t from = 0;
                marker.for_each_match(body, [&](const RegexMatch& m) {
                    if (m.whole() == "{") {
                        ++level;
                    } else if (m.whole() == "}") {
                        --level;
                    } else if (level == 0) {
                        if (const std::optional<std::string> n = named(m.group(1))) {
                            rebuilt += body.substr(from, m.start() - from) + "case " + *n + ":";
                            from = m.end();
                        }
                    }
                    return true;
                });
                rebuilt += body.substr(from);
                text = text.substr(0, sw->second) + rebuilt + text.substr(i);
            }
        }
        return text;
    }

    // The string at an address passed to Windows as a string, beside it; the type each conversion of a printf-style format takes, beside its argument.
    std::string strings_in(std::string text) const {
        std::vector<std::pair<std::size_t, std::string>> edits;   // where, what is put there
        for (const Call& call : calls(text)) {
            const Prototype* p = sdk_.prototype(call.function);
            if (!p) continue;
            std::size_t pos = call.start;
            std::optional<std::size_t> format_at;
            std::string format;
            for (std::size_t k = 0; k < call.args.size(); ++k) {
                const std::string& arg = call.args[k];
                const std::size_t a = pos;
                pos += arg.size() + 1;
                const std::string kind = k < p->parameters.size() ? text::upper(p->parameters[k].type) : std::string();
                const auto m = number_form().full_match(text::trim(arg));
                if (!m || kind.find("STR") == std::string::npos || arg.find("/*") != std::string::npos) continue;
                const std::optional<std::uint32_t> address = number_low32(m->group(1));
                if (!address) continue;
                const std::optional<std::string> found = string_at_(*address);
                if (!found) continue;
                std::string shown = found->size() <= 60 ? *found : found->substr(0, 57) + "...";
                shown = text::replace_all(shown, "\\", "\\\\");
                shown = text::replace_all(shown, "\n", "\\n");
                shown = text::replace_all(shown, "\r", "\\r");
                shown = text::replace_all(shown, "\t", "\\t");
                shown = text::replace_all(shown, "\"", "\\\"");
                shown = text::replace_all(shown, "*/", "*\\/");
                edits.emplace_back(a + std::string_view(text::rtrim(arg)).size(), " /* \"" + shown + "\" */");
                if (k + 1 < p->parameters.size() && p->parameters[k + 1].type == "...") {   // the format
                    format_at = k;
                    format = *found;
                }
            }
            const std::string lower = text::lower(call.function);
            if (!format_at || !(lower.starts_with("wsprintf") || lower.starts_with("sprintf") || lower.starts_with("printf") || lower.starts_with("fprintf") ||
                                lower.starts_with("_snprintf")))
                continue;
            static const Regex conversion(R"(%[-+ 0#]*(?:\*|\d+)?(?:\.(?:\*|\d+))?(?:hh|h|ll|l|L|w|I64)?[diouxXeEfgGcCsSp])");
            const std::vector<std::string> specs = conversion.find_all(format);
            pos = call.start;
            for (std::size_t k = 0; k < call.args.size(); ++k) {
                const std::string& arg = call.args[k];
                const std::size_t a = pos;
                pos += arg.size() + 1;
                if (k <= *format_at) continue;
                const std::size_t n = k - *format_at - 1;
                if (n >= specs.size() || arg.find("/*") != std::string::npos) continue;
                const auto m = number_form().full_match(text::trim(arg));
                std::optional<std::string> found;
                if (m && (specs[n].back() == 's' || specs[n].back() == 'S')) {
                    if (const std::optional<std::uint32_t> address = number_low32(m->group(1))) found = string_at_(*address);
                }
                static const Regex plain(R"(^[^"\\*\r\n\t]*$)");
                const std::size_t where = a + std::string_view(text::rtrim(arg)).size();
                if (found && found->size() <= 60 && plain.search(*found)) edits.emplace_back(where, " /* " + specs[n] + ": \"" + *found + "\" */");
                else edits.emplace_back(where, " /* " + specs[n] + " */");
            }
        }
        std::sort(edits.begin(), edits.end(), [](const auto& a, const auto& b) { return a > b; });
        for (const auto& [at, note] : edits) text.insert(at, note);
        return text;
    }

    // A pointer passed to Windows as one SDK structure, whose every dereference lands on a field of it, has them written as fields.
    std::string fields_in(std::string text) {
        for (const auto& [var, name] : struct_vars(sdk_, text)) {
            const symbols::sdk_structs::Layout* layout = sdk_.structures().layout(name);
            if (!layout) continue;
            const Regex typedefd(std::format(R"(\btypedef\b[^;]*\b{}\s*;)", name));
            if (typedefd.search(header_)) continue;
            const Regex deref(std::format(R"(\*\((\w+) \*\)(?:\({0}(?: \+ (0x[0-9a-fA-F]+|\d+))?\)|{0}\b))", var));
            struct Edit {
                std::size_t from, to;
                std::string text;
            };
            std::vector<Edit> edits;
            bool ok = true;
            deref.for_each_match(text, [&](const RegexMatch& m) {
                const std::optional<std::uint32_t> size = access_size(m.group(1));
                const std::optional<std::uint32_t> offset = m.has(2) ? number_low32(m.group(2)) : std::optional<std::uint32_t>(0);
                std::optional<symbols::sdk_structs::StructTable::Typed> found;
                if (size && offset) found = sdk_.structures().typed_field(*layout, *offset, *size);
                if (!found) {
                    ok = false;
                    return false;
                }
                const std::string field = std::format("(({} *){})->{}", name, var, found->path);
                edits.push_back({m.start(), m.end(), found->ctype == m.group(1) ? field : std::format("*({} *)&{}", m.group(1), field)});
                return true;
            });
            const std::string rest = deref.replace_all(text, "");
            const Regex moved(std::format(R"((?<![\w.>]){}\s*(?:[-+]|\[))", var));
            const Regex stepped(std::format(R"((?<![\w.>]){}\s*(?:\+\+|--|[-+]=))", var));
            if (!ok || edits.empty() || moved.search(rest) || stepped.search(rest)) continue;   // read past it, moved, or indexed: keep the numbers
            for (auto e = edits.rbegin(); e != edits.rend(); ++e) text.replace(e->from, e->to - e->from, e->text);
            structs_used_.insert(name);
        }
        return text;
    }

    // A number passed to a Windows function where the SDK names that parameter's values, written as the name.
    std::string constants_in(std::string text) {
        struct Edit {
            std::size_t from, to;
            std::string name;
        };
        std::vector<Edit> edits;
        for (const Call& call : calls(text)) {
            const Prototype* p = sdk_.prototype(call.function);
            if (!p) continue;
            std::size_t pos = call.start;
            for (std::size_t k = 0; k < call.args.size(); ++k) {
                const std::string& arg = call.args[k];
                const std::size_t start = pos;
                pos += arg.size() + 1;
                if (k >= p->parameters.size()) break;
                const auto m = number_form().full_match(text::trim(arg));
                if (!m) continue;
                const std::optional<std::uint32_t> v = number_low32(m->group(1));
                if (!v) continue;
                const std::optional<std::string> named = sdk_.constant(call.function, p->parameters[k].name, static_cast<std::int64_t>(*v));
                if (!named) continue;
                const std::size_t lead = arg.size() - std::string_view(text::ltrim(arg)).size();
                edits.push_back({start + lead, start + lead + text::trim(arg).size(), *named});
                for (const std::string_view part : text::split(*named, " | ")) constants_used_[std::string(part)] = *sdk_.value(part);
            }
        }
        for (auto e = edits.rbegin(); e != edits.rend(); ++e) text.replace(e->from, e->to - e->from, e->name);
        return text;
    }

    const Sdk& sdk_;
    OrderedMap<std::string, std::string> texts_;
    StringAt string_at_;
    std::string header_;
    std::map<std::string, std::string> globals_;
    std::map<std::string, std::uint32_t> constants_used_;   // {NAME: value}, by name: what `definitions` writes in that order
    std::set<std::string> structs_used_;
    mutable std::optional<std::set<std::string>> unused_;
};

}  // namespace

std::unique_ptr<module_writer::ReadablePass> make_pass(const Sdk& sdk, const translate::l2::Made& stored, StringAt string_at) {
    return std::make_unique<Pass>(sdk, stored, std::move(string_at));
}

// ---- which areas a function works with ------------------------------------------------------------------------------------------------

std::string calls_hint(const Sdk& sdk, std::string_view text) {
    struct Area {
        std::string_view name;
        std::vector<std::string_view> heads;
    };
    static const std::vector<Area> areas = {
        {"registry", {"RegOpen", "RegCreate", "RegQuery", "RegSet", "RegClose", "RegDelete", "RegEnum", "RegFlush"}},
        {"files", {"CreateFile", "ReadFile", "WriteFile", "CloseHandle", "_lopen", "_lread", "_lwrite", "_lclose", "OpenFile", "SetFilePointer", "FindFirstFile", "DeleteFile"}},
        {"memory", {"LocalAlloc", "LocalFree", "GlobalAlloc", "GlobalFree", "GlobalLock", "GlobalUnlock", "HeapAlloc", "HeapFree", "VirtualAlloc", "VirtualFree", "LocalLock", "LocalUnlock", "LocalReAlloc", "GlobalReAlloc"}},
        {"windows", {"CreateWindow", "DestroyWindow", "ShowWindow", "SetWindowPos", "MoveWindow", "GetWindowRect", "GetClientRect", "InvalidateRect", "UpdateWindow", "SetWindowLong", "GetWindowLong", "SetWindowText", "GetWindowText", "GetParent", "GetDlgItem", "EnableWindow"}},
        {"messages", {"SendMessage", "PostMessage", "SendDlgItemMessage", "DefWindowProc", "CallWindowProc", "GetMessage", "PeekMessage", "DispatchMessage", "TranslateMessage"}},
        {"drawing", {"BeginPaint", "EndPaint", "GetDC", "ReleaseDC", "SelectObject", "DeleteObject", "TextOut", "DrawText", "BitBlt", "PatBlt", "FillRect", "CreatePen", "CreateSolidBrush", "SetTextColor", "SetBkColor", "SetBkMode", "LineTo", "MoveToEx", "Rectangle", "CreateCompatibleDC", "StretchBlt", "ExtTextOut"}},
        {"strings", {"lstrcpy", "lstrcat", "lstrlen", "lstrcmp", "lstrcmpi", "wsprintf", "wvsprintf", "LoadString", "CharNext", "CharPrev", "CharUpper", "CharLower", "MultiByteToWideChar", "WideCharToMultiByte"}},
        {"modules", {"LoadLibrary", "FreeLibrary", "GetProcAddress", "GetModuleHandle", "GetModuleFileName"}},
        {"dialogs", {"DialogBox", "CreateDialog", "EndDialog", "MessageBox", "CheckDlgButton", "IsDlgButtonChecked", "SetDlgItemText", "GetDlgItemText", "SetDlgItemInt", "GetDlgItemInt"}},
        {"menus", {"CreateMenu", "CreatePopupMenu", "AppendMenu", "InsertMenu", "EnableMenuItem", "CheckMenuItem", "TrackPopupMenu", "GetSubMenu", "DestroyMenu", "LoadMenu"}}};
    std::vector<std::string> seen;
    for (const Call& call : calls(text))
        if (std::find(seen.begin(), seen.end(), call.function) == seen.end() && sdk.prototype(call.function)) seen.push_back(call.function);
    if (seen.empty()) return "";
    std::vector<std::string_view> hit;
    for (const Area& area : areas) {
        const bool any = std::any_of(seen.begin(), seen.end(), [&](const std::string& f) {
            return std::any_of(area.heads.begin(), area.heads.end(), [&](std::string_view h) { return f.starts_with(h); });
        });
        if (any) hit.push_back(area.name);
    }
    std::vector<std::string> shown(seen.begin(), seen.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(5, seen.size())));
    const std::string list = text::join(shown, ", ") + (seen.size() > 5 ? ", ..." : "");
    if (hit.empty()) return "calls " + list;
    std::vector<std::string_view> two(hit.begin(), hit.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(2, hit.size())));
    return text::join(two, " and ") + ": " + list;
}

}  // namespace cb::source::readable

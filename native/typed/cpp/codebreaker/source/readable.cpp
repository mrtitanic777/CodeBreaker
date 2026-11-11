// codebreaker/source/readable.cpp -- typed C++ port of codebreaker.source.readable (see readable.hpp).

#include "codebreaker/source/readable.hpp"

#include <algorithm>
#include <mutex>

#include "codebreaker/symbols/prototypes.hpp"
#include "codebreaker/symbols/sdk_structs.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::source::readable {

namespace {

using symbols::prototypes::Prototype;

py::str S(const char* s) { return py::str(s); }

py::re::Pattern rx(const char* pattern, int64_t flags = 0) { return py::re::Pattern(py::str(pattern), flags); }
constexpr int64_t kM = 8, kS = 16;

// int(text, 0) as the low 64 bits (a signed int64): Python's ints are unbounded, and a define of 0xFFFFFFFFFFFFFFFF or a
// 64-bit literal in decompiled text is past int64; every use here masks to 32 bits (or reads a small number), which the
// low bits give exactly. A literal that is not one (ValueError) is still refused, as int() refuses it.
int64_t int0(const py::str& text) {
    try {
        return py::int_parse(text, 0);
    } catch (const py::OverflowError&) {
        std::string s = text.strip().utf8();
        size_t i = 0;
        bool neg = false;
        if (i < s.size() && (s[i] == '-' || s[i] == '+')) neg = s[i++] == '-';
        uint64_t base = 10;
        if (s.size() > i + 1 && s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) base = 16, i += 2;
        else if (s.size() > i + 1 && s[i] == '0' && (s[i + 1] == 'o' || s[i + 1] == 'O')) base = 8, i += 2;
        else if (s.size() > i + 1 && s[i] == '0' && (s[i + 1] == 'b' || s[i + 1] == 'B')) base = 2, i += 2;
        uint64_t v = 0;
        for (; i < s.size(); ++i) {
            char c = s[i];
            if (c == '_') continue;
            uint64_t d = c >= '0' && c <= '9' ? static_cast<uint64_t>(c - '0') : static_cast<uint64_t>((c | 0x20) - 'a' + 10);
            v = v * base + d;
        }
        if (neg) v = 0 - v;
        return static_cast<int64_t>(v);
    }
}

// the bits of a number set (bin(x).count("1"))
int64_t popcount(int64_t v) {
    int64_t n = 0;
    for (uint64_t u = static_cast<uint64_t>(v); u; u >>= 1) n += static_cast<int64_t>(u & 1);
    return n;
}

}  // namespace

// ---- the families ----
const py::dict<std::tuple<py::str, py::str>, Family>& FAMILIES() {
    static const py::dict<std::tuple<py::str, py::str>, Family> d = [] {
        py::dict<std::tuple<py::str, py::str>, Family> x;
        auto add = [&](const char* fn, const char* param, std::initializer_list<const char*> prefixes, const char* kind) {
            py::list<py::str> p;
            for (const char* s : prefixes) p.append(py::str(s));
            x.set(std::make_tuple(py::str(fn), py::str(param)), Family{p, py::str(kind)});
        };
        add("messagebox", "uType", {"MB_"}, "flags");
        add("messageboxex", "uType", {"MB_"}, "flags");
        add("messagebeep", "uType", {"MB_"}, "enum");
        add("localalloc", "uFlags", {"LMEM_"}, "flags");
        add("localrealloc", "uFlags", {"LMEM_"}, "flags");
        add("globalalloc", "uFlags", {"GMEM_"}, "flags");
        add("globalrealloc", "uFlags", {"GMEM_"}, "flags");
        add("heapalloc", "dwFlags", {"HEAP_"}, "flags");
        add("heaprealloc", "dwFlags", {"HEAP_"}, "flags");
        add("heapfree", "dwFlags", {"HEAP_"}, "flags");
        add("heapcreate", "flOptions", {"HEAP_"}, "flags");
        add("virtualalloc", "flAllocationType", {"MEM_"}, "flags");
        add("virtualalloc", "flProtect", {"PAGE_"}, "flags");
        add("virtualfree", "dwFreeType", {"MEM_"}, "flags");
        add("virtualprotect", "flNewProtect", {"PAGE_"}, "flags");
        add("showwindow", "nCmdShow", {"SW_"}, "enum");
        add("showwindowasync", "nCmdShow", {"SW_"}, "enum");
        add("sendmessage", "Msg", {"WM_"}, "enum");
        add("postmessage", "Msg", {"WM_"}, "enum");
        add("senddlgitemmessage", "Msg", {"WM_"}, "enum");
        add("defwindowproc", "Msg", {"WM_"}, "enum");
        add("callwindowproc", "Msg", {"WM_"}, "enum");
        add("postthreadmessage", "Msg", {"WM_"}, "enum");
        add("createwindowex", "dwExStyle", {"WS_EX_"}, "flags");
        add("createwindowex", "dwStyle", {"WS_"}, "flags");
        add("setwindowpos", "uFlags", {"SWP_"}, "flags");
        add("getwindowlong", "nIndex", {"GWL_"}, "enum");
        add("setwindowlong", "nIndex", {"GWL_"}, "enum");
        add("getclasslong", "nIndex", {"GCL_"}, "enum");
        add("setclasslong", "nIndex", {"GCL_"}, "enum");
        add("getsystemmetrics", "nIndex", {"SM_"}, "enum");
        add("getsyscolor", "nIndex", {"COLOR_"}, "enum");
        add("getsyscolorbrush", "nIndex", {"COLOR_"}, "enum");
        add("getdevicecaps", "index",
            {"HORZRES", "VERTRES", "LOGPIXELS", "BITSPIXEL", "PLANES", "NUMCOLORS", "RASTERCAPS", "TECHNOLOGY",
             "SIZEPALETTE"},
            "enum");
        add("drawtext", "uFormat", {"DT_"}, "flags");
        add("drawtextex", "dwDTFormat", {"DT_"}, "flags");
        add("peekmessage", "wRemoveMsg", {"PM_"}, "flags");
        add("trackpopupmenu", "uFlags", {"TPM_"}, "flags");
        add("trackpopupmenuex", "fuFlags", {"TPM_"}, "flags");
        add("enablemenuitem", "uEnable", {"MF_"}, "flags");
        add("checkmenuitem", "uCheck", {"MF_"}, "flags");
        add("appendmenu", "uFlags", {"MF_"}, "flags");
        add("insertmenu", "uFlags", {"MF_"}, "flags");
        add("modifymenu", "uFlags", {"MF_"}, "flags");
        add("deletemenu", "uFlags", {"MF_"}, "flags");
        add("removemenu", "uFlags", {"MF_"}, "flags");
        add("checkdlgbutton", "uCheck", {"BST_"}, "enum");
        add("regopenkeyex", "samDesired", {"KEY_"}, "flags");
        add("regcreatekeyex", "samDesired", {"KEY_"}, "flags");
        add("regcreatekeyex", "dwOptions", {"REG_OPTION_"}, "flags");
        add("regsetvalueex", "dwType", {"REG_"}, "enum");
        add("regopenkeyex", "hKey", {"HKEY_"}, "enum");
        add("regcreatekeyex", "hKey", {"HKEY_"}, "enum");
        add("regopenkey", "hKey", {"HKEY_"}, "enum");
        add("regcreatekey", "hKey", {"HKEY_"}, "enum");
        add("regqueryvalue", "hKey", {"HKEY_"}, "enum");
        add("regsetvalue", "hKey", {"HKEY_"}, "enum");
        add("regdeletekey", "hKey", {"HKEY_"}, "enum");
        add("regenumkey", "hKey", {"HKEY_"}, "enum");
        add("createfile", "dwDesiredAccess", {"GENERIC_"}, "flags");
        add("createfile", "dwShareMode", {"FILE_SHARE_"}, "flags");
        add("createfile", "dwFlagsAndAttributes", {"FILE_ATTRIBUTE_", "FILE_FLAG_"}, "flags");
        add("setfileattributes", "dwFileAttributes", {"FILE_ATTRIBUTE_"}, "flags");
        add("setfilepointer", "dwMoveMethod", {"FILE_BEGIN", "FILE_CURRENT", "FILE_END"}, "enum");
        add("formatmessage", "dwFlags", {"FORMAT_MESSAGE_"}, "flags");
        add("loadimage", "uType", {"IMAGE_"}, "enum");
        add("loadimage", "fuLoad", {"LR_"}, "flags");
        add("copyimage", "uType", {"IMAGE_"}, "enum");
        add("copyimage", "fuFlags", {"LR_"}, "flags");
        add("setbkmode", "iBkMode", {"TRANSPARENT", "OPAQUE"}, "enum");
        add("getstockobject", "i",
            {"WHITE_BRUSH", "LTGRAY_BRUSH", "GRAY_BRUSH", "DKGRAY_BRUSH", "BLACK_BRUSH", "NULL_BRUSH", "HOLLOW_BRUSH",
             "WHITE_PEN", "BLACK_PEN", "NULL_PEN", "OEM_FIXED_FONT", "ANSI_FIXED_FONT", "ANSI_VAR_FONT", "SYSTEM_FONT",
             "DEVICE_DEFAULT_FONT", "DEFAULT_PALETTE", "SYSTEM_FIXED_FONT", "DEFAULT_GUI_FONT"},
            "enum");
        add("waitforsingleobject", "dwMilliseconds", {"INFINITE"}, "enum");
        add("waitformultipleobjects", "dwMilliseconds", {"INFINITE"}, "enum");
        add("setwindowshookex", "idHook", {"WH_"}, "enum");
        add("redrawwindow", "flags", {"RDW_"}, "flags");
        add("getwindow", "uCmd", {"GW_"}, "enum");
        add("seterrormode", "uMode", {"SEM_"}, "flags");
        add("getfiletype", "", {}, "enum");
        return x;
    }();
    return d;
}

namespace {

const py::re::Pattern& EXPR() {
    static const py::re::Pattern p(
        py::str(R"(^#define\s+([A-Z][A-Z0-9_]+)\s+\(\s*([A-Z][A-Z0-9_]*(?:\s*\|\s*[A-Z][A-Z0-9_]*)+)\s*\)\s*(?:/[/*].*)?$)"), kM);
    return p;
}
const py::re::Pattern& TOP() {
    static const py::re::Pattern p(
        py::str(R"(^#define\s+([A-Z][A-Z0-9_]+)\s+\(?\s*(?:\(\s*\w+\s*\)\s*)?(-?\s*(?:0x[0-9A-Fa-f]+|\d+))[uUlL]*\s*\)?\s*(?:/[/*].*)?$)"),
        kM);
    return p;
}

std::mutex g_lock;
std::optional<Constants> g_db;       // _db

}  // namespace

const Constants& _constants() {
    std::lock_guard<std::mutex> guard(g_lock);
    if (!g_db) {
        Constants c;
        py::list<std::tuple<py::str, py::str, py::str>> later;      // (name, expr, header)
        symbols::prototypes::Folders folders = symbols::prototypes::_folders();
        static const py::re::Pattern continued(py::str(R"(\\[ \t]*\r?\n)"), 0);
        for (const py::list<py::str>& dirs : {folders.w32, folders.w16}) {
            for (const py::str& folder : dirs) {
                if (!py::os::path::isdir(folder)) continue;
                for (const py::str& fn : py::sorted(py::os::listdir(folder))) {
                    if (!fn.lower().endswith(py::str(".h"))) continue;
                    py::str text;
                    try {
                        text = py::open_rb(py::os::path::join(folder, fn)).read().decode("latin1");
                    } catch (const py::OSError&) {
                        continue;
                    }
                    text = continued.sub(py::str(" "), text);          // a define continued on the next line
                    for (const py::re::Match& m : TOP().finditer(text)) {
                        py::str name = m.group(1);
                        if (c.values.contains(name)) continue;
                        int64_t v;
                        try {
                            v = int0(m.group(2).replace(py::str(" "), py::str("")));
                        } catch (const py::ValueError&) {
                            continue;
                        }
                        c.values.set(name, v & 0xFFFFFFFF);
                        c.order.append({name, v & 0xFFFFFFFF, fn.lower()});
                    }
                    for (const py::re::Match& m : EXPR().finditer(text)) later.append({m.group(1), m.group(2), fn.lower()});
                }
            }
        }
        for (const auto& item : later) {
            const py::str& name = std::get<0>(item);
            py::list<py::str> parts;
            for (const py::str& x : py::split(std::get<1>(item), py::str("|"))) parts.append(x.strip());
            bool all_known = true;
            for (const py::str& x : parts) {
                if (!c.values.contains(x)) {
                    all_known = false;
                    break;
                }
            }
            if (!c.values.contains(name) && all_known) {
                int64_t v = 0;
                for (const py::str& x : parts) v |= c.values.at(x);
                c.values.set(name, v);
                c.order.append({name, v, std::get<2>(item)});
            }
        }
        g_db = std::move(c);
    }
    return *g_db;
}

namespace {

// _NARROWER: prefix -> the prefixes of the names it must not take
const py::list<py::str>& narrower(const py::str& p) {
    static const py::dict<py::str, py::list<py::str>> d = [] {
        py::dict<py::str, py::list<py::str>> x;
        x.set(S("WS_"), py::list<py::str>{S("WS_EX_")});
        x.set(S("MB_"), py::list<py::str>{S("MB_ERR_")});
        x.set(S("SW_"), py::list<py::str>{S("SW_ERASE"), S("SW_INVALIDATE"), S("SW_SCROLLCHILDREN")});
        x.set(S("REG_"), py::list<py::str>{S("REG_OPTION_"), S("REG_CREATED_"), S("REG_OPENED_"), S("REG_NOTIFY_"),
                                           S("REG_LEGAL_"), S("REG_WHOLE_"), S("REG_REFRESH_"), S("REG_NO_")});
        return x;
    }();
    static const py::list<py::str> none;
    auto f = d.get(p);
    return f ? *f : none;
}

}  // namespace

py::list<std::tuple<py::str, int64_t>> _family(const py::list<py::str>& prefixes, const std::optional<py::str>& header) {
    const Constants& c = _constants();
    py::list<std::tuple<py::str, int64_t, py::str>> out;
    for (const auto& item : c.order) {
        const py::str& name = std::get<0>(item);
        bool any_prefix = false;
        for (const py::str& p : prefixes) {
            if (p.endswith(py::str("_")) ? name.startswith(p) : (name == p || name.startswith(p))) {
                any_prefix = true;
                break;
            }
        }
        if (!any_prefix) continue;
        bool narrowed = false;
        for (const py::str& p : prefixes) {
            for (const py::str& x : narrower(p)) {
                if (name.startswith(x)) narrowed = true;
            }
        }
        if (narrowed) continue;
        if (name.endswith(std::make_tuple(S("MASK"), S("_FIRST"), S("_LAST"), S("_MAX"), S("_MIN"), S("_RESERVED")))) continue;
        out.append(item);
    }
    py::list<std::tuple<py::str, int64_t>> own;
    for (const auto& item : out) {
        if (header && !header->empty() && std::get<2>(item) == *header) own.append({std::get<0>(item), std::get<1>(item)});
    }
    if (!own.empty()) return own;
    py::list<std::tuple<py::str, int64_t>> all;
    for (const auto& item : out) all.append({std::get<0>(item), std::get<1>(item)});
    return all;
}

py::list<py::str> prefixes_of(const py::str& base, const py::str& parameter) {
    auto fam = FAMILIES().get(std::make_tuple(base, parameter));
    return fam ? std::get<0>(*fam) : py::list<py::str>();
}

std::optional<py::str> constant(const py::str& function, const py::str& parameter, int64_t value) {
    static const py::re::Pattern ending = rx(R"((?<=[a-z])[AW]$)");
    py::str base = ending.sub(py::str(""), function).lower();
    auto fam = FAMILIES().get(std::make_tuple(base, parameter));
    if (!fam || std::get<0>(*fam).empty()) return std::nullopt;
    std::optional<Prototype> proto = _prototype(function);
    std::optional<py::str> header;
    if (proto) header = proto->header.lower();
    py::list<std::tuple<py::str, int64_t>> members = _family(std::get<0>(*fam), header);
    value &= 0xFFFFFFFF;
    for (const auto& m : members) {
        if (std::get<1>(m) == value) return std::get<0>(m);            // the first the headers give that value
    }
    if (std::get<1>(*fam) != "flags" || value == 0) return std::nullopt;
    int64_t left = value;
    std::vector<std::tuple<int64_t, py::str>> picked;
    std::vector<std::tuple<py::str, int64_t>> by_bits(members.vec().begin(), members.vec().end());
    std::stable_sort(by_bits.begin(), by_bits.end(), [](const auto& a, const auto& b) {
        int64_t pa = -popcount(std::get<1>(a)), pb = -popcount(std::get<1>(b));
        if (pa != pb) return pa < pb;
        return -std::get<1>(a) < -std::get<1>(b);
    });
    for (const auto& nv : by_bits) {
        int64_t v = std::get<1>(nv);
        if (v && (v & left) == v) {
            picked.emplace_back(v, std::get<0>(nv));
            left &= ~v;
        }
        if (!left) break;
    }
    if (left || picked.size() < 2) return std::nullopt;
    py::list<py::str> ws = prefixes_of(base, parameter);
    if (ws.size() == 1 && ws[0] == "WS_") {      // a child window's 0x20000/0x10000 are WS_GROUP and
        int64_t child = value & 0x40000000;      // WS_TABSTOP, a top-level one's the minimize and
        py::dict<int64_t, py::str> mean;         // maximize boxes (the SDK's own reading of those bits)
        mean.set(0x20000, child ? S("WS_GROUP") : S("WS_MINIMIZEBOX"));
        mean.set(0x10000, child ? S("WS_TABSTOP") : S("WS_MAXIMIZEBOX"));
        for (auto& pv : picked) std::get<1>(pv) = mean.get(std::get<0>(pv), std::get<1>(pv));
        py::list<std::tuple<py::str, int64_t>> kept;
        for (const auto& nw : members) {
            auto w = mean.get(std::get<1>(nw));
            if (!w || std::get<0>(nw) == *w) kept.append(nw);
        }
        members = kept;
    }
    for (const auto& pv : picked) {                     // a part two names share: which was meant is
        py::set<py::str> names;                         // not in the number, so the number stays
        for (const auto& m : members) {
            if (std::get<1>(m) == std::get<0>(pv)) names.add(std::get<0>(m));
        }
        if (names.size() > 1) return std::nullopt;
    }
    std::sort(picked.begin(), picked.end(), [](const auto& a, const auto& b) { return b < a; });   // reverse=True
    py::list<py::str> parts;
    for (const auto& pv : picked) parts.append(std::get<1>(pv));
    return py::join(py::str(" | "), parts);
}

// ---- reading calls in C text ----
namespace {
const py::re::Pattern& CALL() {
    static const py::re::Pattern p = rx(R"(\b([A-Za-z_]\w*)\s*\()");
    return p;
}
const py::re::Pattern& NUMBER() {
    static const py::re::Pattern p =
        rx(R"(^\(?\s*(?:\(\s*[A-Za-z_][\w ]*\*?\s*\)\s*)?(-?(?:0x[0-9a-fA-F]+|\d+))[uUlL]*\s*\)?$)");
    return p;
}
}  // namespace

py::list<Call> calls(const py::str& text) {
    py::list<Call> out;
    int64_t n = text.size();
    for (const py::re::Match& m : CALL().finditer(text)) {
        py::str name = m.group(1);
        int64_t i = m.end();
        if (name == "if" || name == "while" || name == "for" || name == "switch" || name == "return" || name == "sizeof") continue;
        int64_t depth = 1, start = i;
        py::list<py::str> args;
        int64_t j = i;
        while (j < n && depth) {
            char32_t c = text.at_(j);
            if (c == U'(' || c == U'[') {
                depth += 1;
            } else if (c == U')' || c == U']') {
                depth -= 1;
                if (!depth) {
                    args.append(text.slice(start, j));
                    break;
                }
            } else if (c == U',' && depth == 1) {
                args.append(text.slice(start, j));
                start = j + 1;
            } else if (c == U'"') {
                j += 1;
                while (j < n && text.at_(j) != U'"') j += text.at_(j) == U'\\' ? 2 : 1;
            }
            j += 1;
        }
        if (depth == 0) out.append({name, i, j, args});
    }
    return out;
}

std::optional<Prototype> _prototype(const py::str& name) {
    static const py::re::Pattern ending = rx(R"((?<=[a-z])[AW]$)");
    for (const py::str& candidate : {name, ending.sub(py::str(""), name)}) {
        auto p = symbols::prototypes::of(candidate);
        if (p) return p;
    }
    return std::nullopt;
}

// ---- names for placeholders ----
namespace {

const py::re::Pattern& PLACEHOLDER() {
    static const py::re::Pattern p =
        rx(R"(^(?:[a-z]{1,4}Var\d+|local_[0-9a-fA-F]+|a\d+(?:_\d+)?|param_\d+|in_\w+|extraout_\w+)$)");
    return p;
}
const py::re::Pattern& GLOBAL() {
    static const py::re::Pattern p = rx(R"(\b(?:[a-z]{1,4}Ram[0-9a-f]+|DAT_[0-9a-f]+|PTR_\w+)\b)");
    return p;
}
const py::re::Pattern& IDENT() {
    static const py::re::Pattern p = rx(R"(\b[A-Za-z_]\w*\b)");
    return p;
}
// a Windows function the right-hand side of an assignment starts with
const py::re::Pattern& CALLED_FIRST() {
    static const py::re::Pattern p = rx(R"(^\s*(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*\()");
    return p;
}

// the same, for an assigned value that is already stripped
const py::re::Pattern& CALLED_FIRST_PLAIN() {
    static const py::re::Pattern p = rx(R"(^(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*\()");
    return p;
}

const py::dict<py::str, py::str>& HANDLE_NAMES() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> x;
        for (auto kv : std::initializer_list<std::pair<const char*, const char*>>{
                 {"HWND", "hwnd"},   {"HDC", "hdc"},       {"HGLOBAL", "hMem"}, {"HLOCAL", "hMem"},
                 {"HMENU", "hmenu"}, {"HINSTANCE", "hinst"}, {"HMODULE", "hmod"}, {"HBRUSH", "hbr"},
                 {"HPEN", "hpen"},   {"HFONT", "hfont"},   {"HBITMAP", "hbm"},  {"HICON", "hicon"},
                 {"HCURSOR", "hcur"}, {"HKEY", "hkey"},    {"HANDLE", "h"},     {"HRGN", "hrgn"},
                 {"HPALETTE", "hpal"}, {"HGDIOBJ", "hobj"}, {"HACCEL", "haccel"}, {"HRSRC", "hrsrc"},
                 {"HFILE", "hf"},    {"HDWP", "hdwp"},     {"HIMAGELIST", "himl"}, {"HTREEITEM", "hitem"},
                 {"HPROPSHEETPAGE", "hpage"}})
            x.set(S(kv.first), S(kv.second));
        return x;
    }();
    return d;
}

const py::set<py::str>& KEYWORDS() {
    static const py::set<py::str> s = [] {
        py::set<py::str> x;
        for (const char* k : {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
                              "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return",
                              "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
                              "void", "volatile", "while", "far", "near", "huge", "pascal", "cdecl", "i", "j", "k"})
            x.add(S(k));
        return x;
    }();
    return s;
}

// the values of a dict (names.values())
bool in_values(const py::dict<py::str, py::str>& d, const py::str& v) {
    for (const py::str& x : d.values()) {
        if (x == v) return true;
    }
    return false;
}

}  // namespace

py::list<py::str> _assignments(const py::str& text, const py::str& var) {
    py::re::Pattern p(py::str("(?<![\\w.>])") + py::re::escape(var) + py::str("\\s*=(?!=)\\s*([^;]+);"), 0);
    py::list<py::str> out;
    for (const auto& t : p.findall_groups<py::str>(text)) out.append(std::get<0>(t));
    return out;
}

py::dict<py::str, py::str> _local_names(const py::str& text) {
    py::set<py::str> used;
    for (const py::str& w : IDENT().findall(text)) used.add(w);
    // var -> {parameter name}
    py::dict<py::str, py::list<py::str>> as_param;
    static const py::re::Pattern plain_arg = rx(R"(^(?:\([^()]*\)\s*)?([A-Za-z_]\w*)$)");
    for (const Call& c : calls(text)) {
        const py::str& fn = std::get<0>(c);
        std::optional<Prototype> p = _prototype(fn);
        if (!p) continue;
        int64_t k = 0;
        for (const py::str& arg_in : std::get<3>(c)) {
            py::str arg = arg_in.strip();
            auto m = plain_arg.match(arg);
            if (m && PLACEHOLDER().match(m->group(1)) && k < p->parameters.size() &&
                !std::get<1>(p->parameters[k]).empty()) {
                py::list<py::str>& names = as_param.setdefault(m->group(1), py::list<py::str>());
                const py::str& pn = std::get<1>(p->parameters[k]);
                bool have = false;
                for (const py::str& x : names) have = have || x == pn;
                if (!have) names.append(pn);
            }
            ++k;
        }
    }
    py::dict<py::str, py::str> names;
    py::set<py::str> distinct;
    for (const py::str& w : IDENT().findall(text)) distinct.add(w);
    py::list<py::str> vars;
    for (const py::str& w : distinct) vars.append(w);
    for (const py::str& var : py::sorted(vars)) {
        if (!PLACEHOLDER().match(var)) continue;
        std::optional<py::str> wanted;
        py::list<py::str> given = _assignments(text, var);
        py::set<py::str> sources;
        for (const py::str& g : given) {
            auto m = CALLED_FIRST().match(g);
            if (m) sources.add(m->group(1));
        }
        py::set<py::str> stripped;
        for (const py::str& g : given) stripped.add(g.strip());
        bool single = !given.empty() && sources.size() == 1;
        bool by_call = false;
        if (single) {
            py::str src;
            for (const py::str& s : sources) src = s;
            if (sources.size() == stripped.size()) {
                by_call = true;
            } else {
                by_call = true;
                py::re::Pattern again(py::str("^\\s*(?:\\([^()]*\\)\\s*)?") + py::re::escape(src) + py::str("\\s*\\("), 0);
                for (const py::str& g : given) {
                    if (!again.match(g)) by_call = false;
                }
            }
            if (by_call) {
                std::optional<Prototype> p = _prototype(src);
                if (p) {
                    auto h = HANDLE_NAMES().get(p->returns);
                    if (h) wanted = *h;
                }
            }
        }
        auto ap = as_param.get(var);
        if (!wanted && ap && ap->size() == 1) wanted = (*ap)[0];
        if (!wanted) {
            py::str e = py::re::escape(var);
            py::re::Pattern counted(py::str("\\b") + e + py::str("\\s*=\\s*") + e + py::str("\\s*\\+\\s*1\\s*[;)]"), 0);
            py::re::Pattern compared(py::str("\\b") + e + py::str("\\s*<"), 0);
            if (counted.search(text) && compared.search(text)) wanted = S("i");
        }
        if (!wanted || wanted->empty()) continue;
        py::str base = *wanted, w = *wanted;
        int64_t n = 2;
        while (used.contains(w) || in_values(names, w) ||
               (KEYWORDS().contains(w) && !(w == "i" || w == "j" || w == "k"))) {
            if (base == "i" && (w == "i" || w == "j")) {
                w = w == "i" ? S("j") : S("k");
                continue;
            }
            w = py::percent(S("%s%d"), base, n);
            n += 1;
        }
        names.set(var, w);
        used.add(w);
    }
    return names;
}

py::str _rename(const py::str& text, const py::dict<py::str, py::str>& names) {
    if (names.empty()) return text;
    std::vector<py::str> keys;
    for (const py::str& k : names.keys()) keys.push_back(k);
    std::stable_sort(keys.begin(), keys.end(), [](const py::str& a, const py::str& b) { return a.size() > b.size(); });
    py::list<py::str> escaped;
    for (const py::str& k : keys) escaped.append(py::re::escape(k));
    py::re::Pattern pattern(py::str("(?<![\\w.>])(") + py::join(py::str("|"), escaped) + py::str(")\\b"), 0);
    auto sub = [&](const py::re::Match& g) { return names.at(g.group(1)); };
    static const py::re::Pattern strings = rx(R"("(?:\\.|[^"\\])*")");
    py::str out;
    int64_t i = 0;
    for (const py::re::Match& m : strings.finditer(text)) {                // never inside a string
        out += pattern.sub(sub, text.slice(i, m.start()));
        out += m.group(0);
        i = m.end();
    }
    out += pattern.sub(sub, text.slice_from(i));
    return out;
}

py::re::Pattern _deref(const py::str& var) {
    py::str e = py::re::escape(var);
    return py::re::Pattern(py::str("\\*\\((\\w+) \\*\\)(?:\\(") + e + py::str("(?: \\+ (0x[0-9a-fA-F]+|\\d+))?\\)|") + e +
                               py::str("\\b)"),
                           0);
}

py::dict<py::str, py::str> _struct_vars(const py::str& text) {
    // var -> the structures it was passed as (None: not a structure)
    py::dict<py::str, std::vector<std::optional<py::str>>> seen;
    static const py::re::Pattern var_arg = rx(R"(^\s*(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*$)");
    for (const Call& c : calls(text)) {
        std::optional<Prototype> p = _prototype(std::get<0>(c));
        if (!p) continue;
        int64_t k = 0;
        for (const py::str& arg : std::get<3>(c)) {
            auto m = var_arg.match(arg);
            if (!m || k >= p->parameters.size()) {
                ++k;
                continue;
            }
            std::optional<py::str> target = symbols::sdk_structs::pointed_to(std::get<0>(p->parameters[k]));
            std::vector<std::optional<py::str>>& v = seen.setdefault(m->group(1), std::vector<std::optional<py::str>>());
            bool have = false;
            for (const auto& x : v) have = have || x == target;
            if (!have) v.push_back(target);
            ++k;
        }
    }
    py::dict<py::str, py::str> out;
    for (const auto& kv : seen.items()) {
        const auto& t = std::get<1>(kv);
        if (t.size() == 1 && t[0]) out.set(std::get<0>(kv), *t[0]);
    }
    return out;
}

// ---- Readable ----
namespace {
const py::dict<py::str, int64_t>& ACCESS_SIZE() {
    static const py::dict<py::str, int64_t> d = [] {
        py::dict<py::str, int64_t> x;
        for (const char* n : {"int1", "uint1", "xunknown1", "undefined1", "char", "byte", "undefined"}) x.set(S(n), 1);
        for (const char* n : {"int2", "uint2", "xunknown2", "undefined2", "short"}) x.set(S(n), 2);
        for (const char* n : {"int4", "uint4", "xunknown4", "undefined4", "float4", "int", "uint"}) x.set(S(n), 4);
        for (const char* n : {"int8", "uint8", "xunknown8", "undefined8", "float8"}) x.set(S(n), 8);
        return x;
    }();
    return d;
}

}  // namespace

Readable::Readable(const translate::l2::Dict& doc_, StringAt string_at_) : doc(doc_) {
    texts = translate::l2::texts(doc);
    string_at = string_at_ ? string_at_ : StringAt([](int64_t) -> std::optional<py::str> { return std::nullopt; });
    auto header = doc.get(py::str("header"));
    header_text_ = py::str("");
    if (header) {
        if (const py::str* s = std::get_if<py::str>(&header->v)) header_text_ = *s;     // doc.get("header") or ""
    }
    globals = _global_names();
}

py::dict<py::str, py::str> Readable::_global_names() {
    py::dict<py::str, py::list<py::str>> assigned;
    for (const py::str& text : texts.values()) {
        py::set<py::str> found;
        for (const py::str& g : GLOBAL().findall(text)) found.add(g);
        for (const py::str& g : found) {
            for (const py::str& value : _assignments(text, g)) assigned.setdefault(g, py::list<py::str>()).append(value.strip());
        }
    }
    py::dict<py::str, py::str> names;
    py::set<py::str> taken;
    for (const py::str& text : texts.values()) {
        for (const py::str& w : IDENT().findall(text)) taken.add(w);
    }
    py::list<py::str> gs;
    for (const py::str& g : assigned.keys()) gs.append(g);
    for (const py::str& g : py::sorted(gs)) {
        const py::list<py::str>& values = assigned.at(g);
        std::optional<py::str> wanted;
        py::set<py::str> procs;
        bool all_proc = true;
        for (const py::str& v : values) {
            if (v.contains(S("GetProcAddress"))) {
                auto pn = _proc_name(v);
                if (pn) procs.add(*pn);
            } else {
                all_proc = false;
            }
        }
        if (!procs.empty() && procs.size() == 1 && all_proc) {
            py::str only;
            for (const py::str& p : procs) only = p;
            wanted = py::str("g_pfn") + only;
        } else {
            py::set<py::str> called;
            for (const py::str& v : values) {
                auto m = CALLED_FIRST_PLAIN().match(v);
                if (m) called.add(m->group(1));
            }
            py::set<py::str> heads;
            for (const py::str& v : values) heads.add(py::split(v, S("("))[0]);
            if (called.size() == 1 && called.size() == heads.size()) {
                py::str only;
                for (const py::str& c : called) only = c;
                py::re::Pattern again(py::str("^(?:\\([^()]*\\)\\s*)?") + py::re::escape(only) + py::str("\\s*\\("), 0);
                bool all_same = true;
                for (const py::str& v : values) {
                    if (!again.match(v)) all_same = false;
                }
                if (all_same) {
                    std::optional<Prototype> p = _prototype(only);
                    if (p) {
                        auto h = HANDLE_NAMES().get(p->returns);
                        if (h) wanted = py::str("g_") + *h;
                    }
                }
            }
        }
        if (!wanted) continue;
        py::str base = *wanted, w = *wanted;
        int64_t n = 2;
        while (taken.contains(w) || in_values(names, w)) {
            w = py::percent(S("%s%d"), base, n);
            n += 1;
        }
        names.set(g, w);
    }
    return names;
}

std::optional<py::str> Readable::_proc_name(const py::str& value) {
    // The function GetProcAddress(h, name) asks for: a string in the text, or the module's string at the
    // address the text passes.
    static const py::re::Pattern p = rx(
        R"re(GetProcAddress\w*\s*\([^,]*,\s*(?:\([^()]*\)\s*)?(?:"(\w+)"|(-?(?:0x[0-9a-fA-F]+|\d+)))\s*\))re");
    auto m = p.search(value);
    if (!m) return std::nullopt;
    auto g1 = m->group_opt(1);
    if (g1 && !g1->empty()) return *g1;
    int64_t address = int0(*m->group_opt(2)) & 0xFFFFFFFF;
    std::optional<py::str> found = string_at(address);
    static const py::re::Pattern ident = rx(R"(^[A-Za-z_]\w*$)");
    if (found && !found->empty() && ident.match(*found)) return found;
    return std::nullopt;
}

py::str Readable::function(const py::str& name) {
    py::str text = texts.at(name);
    text = _constants_pass(text);
    py::dict<py::str, py::str> merged = _local_names(text);
    for (const auto& kv : globals.items()) merged.set(std::get<0>(kv), std::get<1>(kv));      // dict(_local_names, **globals)
    text = _rename(text, merged);
    text = _compared(text);
    text = _fields(text);
    return _strings(text);
}

const py::set<py::str>& Readable::results_unused() {
    if (!unused_) {
        py::set<py::str> names;
        static const py::re::Pattern first = rx(R"(^[\w ]+?[\s*](\w+)\()", kM);
        for (const py::str& text : texts.values()) {
            auto m = first.search(text);
            if (m) names.add(m->group(1));
        }
        py::set<py::str> called, used;
        py::list<py::str> sorted_names;
        for (const py::str& n : names) sorted_names.append(n);
        sorted_names = py::sorted(sorted_names);
        py::str pattern;
        if (!sorted_names.empty()) {
            py::list<py::str> escaped;
            for (const py::str& n : sorted_names) escaped.append(py::re::escape(n));
            pattern = S("(\\S?)\\s*\\b(") + py::join(S("|"), escaped) + S(")\\s*\\(");
        } else {
            pattern = S("(?!)");
        }
        py::re::Pattern calls_re(pattern, 0);
        static const py::re::Pattern structural = rx(R"(^(?:\}|\{|else|do)?$)");
        for (const py::str& text : texts.values()) {
            py::list<py::str> halves = py::split(text, S("{"), 1);
            py::str body = halves[-1];
            for (const py::re::Match& m : calls_re.finditer(body)) {
                called.add(m.group(2));
                int64_t line_start = body.rfind(S("\n"), 0, m.start(2)) + 1;
                py::str before = body.slice(line_start, m.start(2)).strip();
                if (!before.empty() && !structural.match(before)) used.add(m.group(2));
            }
        }
        py::set<py::str> diff;
        for (const py::str& n : called) {
            if (!used.contains(n)) diff.add(n);
        }
        unused_ = diff;
    }
    return *unused_;
}

py::str Readable::_compared(const py::str& text_in) {
    // Numbers a variable is compared with or switched on, as the SDK constants of the one parameter it is passed
    // to Windows as.
    py::str text = text_in;
    static const py::re::Pattern ending = rx(R"((?<=[a-z])[AW]$)");
    static const py::re::Pattern var_arg = rx(R"(^\s*(?:\([^()]*\)\s*)?([A-Za-z_]\w*)\s*$)");
    // var -> {(function, parameter)}
    py::dict<py::str, std::vector<std::tuple<py::str, py::str>>> families;
    for (const Call& c : calls(text)) {
        const py::str& fn = std::get<0>(c);
        std::optional<Prototype> p = _prototype(fn);
        if (!p) continue;
        int64_t k = 0;
        for (const py::str& arg : std::get<3>(c)) {
            auto m = var_arg.match(arg);
            if (m && k < p->parameters.size()) {
                py::str base = ending.sub(S(""), fn).lower();
                const py::str& param = std::get<1>(p->parameters[k]);
                if (FAMILIES().contains(std::make_tuple(base, param))) {
                    auto& v = families.setdefault(m->group(1), std::vector<std::tuple<py::str, py::str>>());
                    bool have = false;
                    for (const auto& x : v) have = have || (std::get<0>(x) == fn && std::get<1>(x) == param);
                    if (!have) v.emplace_back(fn, param);
                }
            }
            ++k;
        }
    }
    const Constants& cs = _constants();
    for (const auto& kv : families.items()) {
        const py::str& var = std::get<0>(kv);
        const auto& uses = std::get<1>(kv);
        std::vector<py::list<py::str>> distinct;
        for (const auto& u : uses) {
            py::list<py::str> pre = std::get<0>(FAMILIES().at(std::make_tuple(ending.sub(S(""), std::get<0>(u)).lower(), std::get<1>(u))));
            bool have = false;
            for (const auto& d : distinct) {
                if (d.size() == pre.size()) {
                    bool same = true;
                    for (int64_t i = 0; i < d.size(); ++i) same = same && d[i] == pre[i];
                    have = have || same;
                }
            }
            if (!have) distinct.push_back(pre);
        }
        if (distinct.size() != 1) continue;
        // (one of them: the first by name -- min(uses))
        std::size_t least = 0;
        for (std::size_t u = 1; u < uses.size(); ++u) {
            const auto& a = uses[u];
            const auto& b = uses[least];
            if (std::make_tuple(std::get<0>(a).u32(), std::get<1>(a).u32()) < std::make_tuple(std::get<0>(b).u32(), std::get<1>(b).u32())) least = u;
        }
        const py::str fn = std::get<0>(uses[least]);
        const py::str param = std::get<1>(uses[least]);

        auto named = [&](const py::str& number) -> std::optional<py::str> {
            int64_t v = int0(number);
            std::optional<py::str> n = constant(fn, param, v);
            if (n && !n->contains(S(" | "))) {
                constants_used.set(*n, cs.values.at(*n));
                return n;
            }
            return std::nullopt;
        };
        py::str v = py::re::escape(var);
        py::re::Pattern compare(py::str("((?<![\\w.>])") + v + py::str("\\s*)(==|!=)\\s*(0x[0-9a-fA-F]+|\\d+)\\b"), 0);
        text = compare.sub(
            [&](const py::re::Match& m) -> py::str {
                std::optional<py::str> n = named(m.group(3));
                if (n) return m.group(1) + m.group(2) + S(" ") + *n;
                return m.group(0);
            },
            text);
        py::re::Pattern switches(py::str("switch\\s*\\(\\s*(?:\\([^()]*\\)\\s*)?") + v + py::str("\\s*\\)\\s*\\{"), 0);
        py::list<py::re::Match> found = switches.finditer(text);
        static const py::re::Pattern inner = rx(R"([{}]|\bcase\s+(0x[0-9a-fA-F]+|\d+)\s*:)");
        for (int64_t si = found.size() - 1; si >= 0; --si) {
            const py::re::Match& sw = found[si];
            int64_t depth = 1, i = sw.end();
            while (i < text.size() && depth) {
                char32_t ch = text.at_(i);
                depth += ch == U'{' ? 1 : (ch == U'}' ? -1 : 0);
                i += 1;
            }
            py::str body = text.slice(sw.end(), i);
            int64_t level = 0, j = 0;
            py::str out;
            for (const py::re::Match& m : inner.finditer(body)) {
                py::str whole = m.group(0);
                if (whole == "{") {
                    level += 1;
                } else if (whole == "}") {
                    level -= 1;
                } else if (level == 0) {
                    std::optional<py::str> n = named(m.group(1));
                    if (n) {
                        out += body.slice(j, m.start()) + S("case ") + *n + S(":");
                        j = m.end();
                    }
                }
            }
            out += body.slice_from(j);
            text = text.slice(0, sw.end()) + out + text.slice_from(i);
        }
    }
    return text;
}

py::str Readable::_strings(const py::str& text_in) {
    // The string at an address passed to Windows as a string, beside it; the type each conversion of a
    // printf-style format takes, beside its argument.
    py::str text = text_in;
    std::vector<std::pair<int64_t, py::str>> edits;
    static const py::re::Pattern specs_re =
        rx(R"(%[-+ 0#]*(?:\*|\d+)?(?:\.(?:\*|\d+))?(?:hh|h|ll|l|L|w|I64)?[diouxXeEfgGcCsSp])");
    static const py::re::Pattern plain = rx(R"(^[^"\\*\r\n\t]*$)");
    for (const Call& c : calls(text)) {
        const py::str& fn = std::get<0>(c);
        int64_t start = std::get<1>(c);
        const py::list<py::str>& args = std::get<3>(c);
        std::optional<Prototype> p = _prototype(fn);
        if (!p) continue;
        int64_t pos = start;
        std::optional<int64_t> fmt_at;
        std::optional<py::str> fmt;
        int64_t k = 0;
        for (const py::str& arg : args) {
            int64_t a = pos;
            pos += arg.size() + 1;
            py::str kind = k < p->parameters.size() ? std::get<0>(p->parameters[k]) : S("");
            auto m = NUMBER().match(arg.strip());
            if (m && kind.upper().contains(S("STR")) && !arg.contains(S("/*"))) {
                std::optional<py::str> found = string_at(int0(m->group(1)) & 0xFFFFFFFF);
                if (found) {
                    py::str shown = found->size() <= 60 ? *found : found->slice(0, 57) + S("...");
                    shown = shown.replace(S("\\"), S("\\\\")).replace(S("\n"), S("\\n")).replace(S("\r"), S("\\r"))
                                .replace(S("\t"), S("\\t")).replace(S("\""), S("\\\"")).replace(S("*/"), S("*\\/"));
                    edits.emplace_back(a + arg.rstrip().size(), S(" /* \"") + shown + S("\" */"));
                    if (k + 1 < p->parameters.size() && std::get<0>(p->parameters[k + 1]) == "...") {    // the format
                        fmt_at = k;
                        fmt = *found;
                    }
                }
            }
            ++k;
        }
        py::str lowered = fn.lower();
        if (fmt && lowered.startswith(std::make_tuple(S("wsprintf"), S("sprintf"), S("printf"), S("fprintf"), S("_snprintf")))) {
            py::list<py::str> specs;
            for (const py::re::Match& m : specs_re.finditer(*fmt)) specs.append(m.group(0));
            pos = start;
            int64_t kk = 0;
            for (const py::str& arg : args) {
                int64_t a = pos;
                pos += arg.size() + 1;
                int64_t n = kk - *fmt_at - 1;
                if (0 <= n && n < specs.size() && !arg.contains(S("/*"))) {
                    auto m = NUMBER().match(arg.strip());
                    std::optional<py::str> found;
                    py::str last = specs[n].slice_from(specs[n].size() - 1);        // specs[n][-1] in "sS"
                    if (m && (last == "s" || last == "S")) {
                        found = string_at(int0(m->group(1)) & 0xFFFFFFFF);
                    }
                    if (found && found->size() <= 60 && plain.match(*found)) {
                        edits.emplace_back(a + arg.rstrip().size(), S(" /* ") + specs[n] + S(": \"") + *found + S("\" */"));
                    } else {
                        edits.emplace_back(a + arg.rstrip().size(), S(" /* ") + specs[n] + S(" */"));
                    }
                }
                ++kk;
            }
        }
    }
    std::sort(edits.begin(), edits.end(), [](const auto& a, const auto& b) { return b < a; });      // sorted(edits, reverse=True)
    for (const auto& e : edits) text = text.slice(0, e.first) + e.second + text.slice_from(e.first);
    return text;
}

py::str Readable::_fields(const py::str& text_in) {
    py::str text = text_in;
    py::dict<py::str, py::str> vars = _struct_vars(text);
    std::vector<std::tuple<py::str, py::str>> sorted_vars;
    for (const auto& kv : vars.items()) sorted_vars.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(sorted_vars.begin(), sorted_vars.end());
    for (const auto& vs : sorted_vars) {
        const py::str& var = std::get<0>(vs);
        const py::str& sname = std::get<1>(vs);
        std::optional<symbols::sdk_structs::Struct> st = symbols::sdk_structs::layout(sname);
        py::re::Pattern typedef_re(py::str("\\btypedef\\b[^;]*\\b") + py::re::escape(sname) + py::str("\\s*;"), 0);
        if (!st || typedef_re.search(header_text_)) continue;
        py::re::Pattern pattern = _deref(var);
        std::vector<std::tuple<int64_t, int64_t, py::str>> edits;
        bool ok = true;
        for (const py::re::Match& m : pattern.finditer(text)) {
            auto size = ACCESS_SIZE().get(m.group(1));
            auto off_text = m.group_opt(2);
            int64_t offset = (off_text && !off_text->empty()) ? int0(*off_text) : 0;
            std::optional<std::tuple<py::str, py::str>> found;
            if (size && *size) found = st->typed(offset, *size);
            if (!found) {
                ok = false;
                break;
            }
            const py::str& path = std::get<0>(*found);
            const py::str& ctype = std::get<1>(*found);
            py::str field = py::percent(S("((%s *)%s)->%s"), sname, var, path);
            edits.emplace_back(m.start(), m.end(),
                               ctype == m.group(1) ? field : py::percent(S("*(%s *)&%s"), m.group(1), field));
        }
        py::str rest = pattern.sub(S(""), text);
        py::str e = py::re::escape(var);
        py::re::Pattern moved(py::str("(?<![\\w.>])") + e + py::str("\\s*(?:[-+]|\\[)"), 0);
        py::re::Pattern stepped(py::str("(?<![\\w.>])") + e + py::str("\\s*(?:\\+\\+|--|[-+]=)"), 0);
        if (!ok || edits.empty() || moved.search(rest) || stepped.search(rest)) continue;      // read past it, moved, or indexed
        std::sort(edits.begin(), edits.end(), [](const auto& a, const auto& b) { return b < a; });
        for (const auto& ed : edits) text = text.slice(0, std::get<0>(ed)) + std::get<2>(ed) + text.slice_from(std::get<1>(ed));
        structs_used.add(sname);
    }
    return text;
}

py::str Readable::_constants_pass(const py::str& text_in) {
    py::str text = text_in;
    std::vector<std::tuple<int64_t, int64_t, py::str>> edits;
    for (const Call& c : calls(text)) {
        const py::str& fn = std::get<0>(c);
        std::optional<Prototype> p = _prototype(fn);
        if (!p) continue;
        int64_t pos = std::get<1>(c);
        int64_t k = 0;
        for (const py::str& arg : std::get<3>(c)) {
            int64_t start = pos;
            pos += arg.size() + 1;
            if (k >= p->parameters.size()) break;
            auto m = NUMBER().match(arg.strip());
            if (!m) {
                ++k;
                continue;
            }
            std::optional<py::str> named = constant(fn, std::get<1>(p->parameters[k]), int0(m->group(1)));
            if (named) {
                int64_t lead = arg.size() - arg.lstrip().size();
                edits.emplace_back(start + lead, start + lead + arg.strip().size(), *named);
                const Constants& cs = _constants();
                for (const py::str& part : py::split(*named, S(" | "))) constants_used.set(part, cs.values.at(part));
            }
            ++k;
        }
    }
    std::sort(edits.begin(), edits.end(), [](const auto& a, const auto& b) { return b < a; });
    for (const auto& e : edits) text = text.slice(0, std::get<0>(e)) + std::get<2>(e) + text.slice_from(std::get<1>(e));
    return text;
}

py::str Readable::header(const py::str& text) {
    return _rename(text, globals) + (!constants_used.empty() ? S("\n") + definitions() : S("")) + struct_definitions();
}

py::str Readable::struct_definitions() {
    py::list<py::str> names;
    for (const py::str& n : structs_used) names.append(n);
    py::str out;
    py::set<py::str> done;
    static const py::re::Pattern defined = rx(R"(\} (\w+);)");
    for (const py::str& name : py::sorted(names)) {
        std::optional<symbols::sdk_structs::Struct> st = symbols::sdk_structs::layout(name);
        if (!st) throw py::AttributeError("'NoneType' object has no attribute 'name'");
        py::str text = symbols::sdk_structs::definition(*st, done);
        for (const py::str& d : defined.findall(text)) done.add(d);
        out += text;
    }
    return out.empty() ? S("") : S("\n/* SDK structures, at the SDK's offsets */\n") + out;
}

py::str Readable::definitions() {
    py::list<py::str> names;
    for (const py::str& n : constants_used.keys()) names.append(n);
    py::str out;
    for (const py::str& n : py::sorted(names)) {
        out += py::percent(S("#ifndef %s\n#define %s 0x%X\n#endif\n"), n, n, constants_used.at(n));
    }
    return out;
}

namespace {
struct Area {
    const char* name;
    std::vector<const char*> heads;
};
const std::vector<Area>& AREAS() {
    static const std::vector<Area> a = {
        {"registry", {"RegOpen", "RegCreate", "RegQuery", "RegSet", "RegClose", "RegDelete", "RegEnum", "RegFlush"}},
        {"files", {"CreateFile", "ReadFile", "WriteFile", "CloseHandle", "_lopen", "_lread", "_lwrite", "_lclose", "OpenFile",
                   "SetFilePointer", "FindFirstFile", "DeleteFile"}},
        {"memory", {"LocalAlloc", "LocalFree", "GlobalAlloc", "GlobalFree", "GlobalLock", "GlobalUnlock", "HeapAlloc",
                    "HeapFree", "VirtualAlloc", "VirtualFree", "LocalLock", "LocalUnlock", "LocalReAlloc", "GlobalReAlloc"}},
        {"windows", {"CreateWindow", "DestroyWindow", "ShowWindow", "SetWindowPos", "MoveWindow", "GetWindowRect",
                     "GetClientRect", "InvalidateRect", "UpdateWindow", "SetWindowLong", "GetWindowLong", "SetWindowText",
                     "GetWindowText", "GetParent", "GetDlgItem", "EnableWindow"}},
        {"messages", {"SendMessage", "PostMessage", "SendDlgItemMessage", "DefWindowProc", "CallWindowProc", "GetMessage",
                      "PeekMessage", "DispatchMessage", "TranslateMessage"}},
        {"drawing", {"BeginPaint", "EndPaint", "GetDC", "ReleaseDC", "SelectObject", "DeleteObject", "TextOut", "DrawText",
                     "BitBlt", "PatBlt", "FillRect", "CreatePen", "CreateSolidBrush", "SetTextColor", "SetBkColor",
                     "SetBkMode", "LineTo", "MoveToEx", "Rectangle", "CreateCompatibleDC", "StretchBlt", "ExtTextOut"}},
        {"strings", {"lstrcpy", "lstrcat", "lstrlen", "lstrcmp", "lstrcmpi", "wsprintf", "wvsprintf", "LoadString",
                     "CharNext", "CharPrev", "CharUpper", "CharLower", "MultiByteToWideChar", "WideCharToMultiByte"}},
        {"modules", {"LoadLibrary", "FreeLibrary", "GetProcAddress", "GetModuleHandle", "GetModuleFileName"}},
        {"dialogs", {"DialogBox", "CreateDialog", "EndDialog", "MessageBox", "CheckDlgButton", "IsDlgButtonChecked",
                     "SetDlgItemText", "GetDlgItemText", "SetDlgItemInt", "GetDlgItemInt"}},
        {"menus", {"CreateMenu", "CreatePopupMenu", "AppendMenu", "InsertMenu", "EnableMenuItem", "CheckMenuItem",
                   "TrackPopupMenu", "GetSubMenu", "DestroyMenu", "LoadMenu"}},
    };
    return a;
}
}  // namespace

py::str calls_hint(const py::str& text) {
    py::list<py::str> seen;
    for (const Call& c : calls(text)) {
        const py::str& fn = std::get<0>(c);
        bool have = false;
        for (const py::str& s : seen) have = have || s == fn;
        if (!have && _prototype(fn)) seen.append(fn);
    }
    if (seen.empty()) return S("");
    py::list<py::str> areas;
    for (const Area& a : AREAS()) {
        bool any = false;
        for (const py::str& f : seen) {
            for (const char* h : a.heads) {
                if (f.startswith(S(h))) any = true;
            }
        }
        if (any) areas.append(S(a.name));
    }
    py::list<py::str> first5;
    for (int64_t i = 0; i < seen.size() && i < 5; ++i) first5.append(seen[i]);
    py::str shown = py::join(S(", "), first5) + (seen.size() > 5 ? S(", ...") : S(""));
    if (areas.empty()) return S("calls ") + shown;
    py::list<py::str> two;
    for (int64_t i = 0; i < areas.size() && i < 2; ++i) two.append(areas[i]);
    return py::percent(S("%s: %s"), py::join(S(" and "), two), shown);
}

}  // namespace codebreaker::source::readable

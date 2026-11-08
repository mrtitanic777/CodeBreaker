// codebreaker/symbols/sdk_structs.cpp -- typed C++ port of codebreaker.symbols.sdk_structs (see sdk_structs.hpp).

#include "codebreaker/symbols/sdk_structs.hpp"

#include <functional>
#include <mutex>

#include "codebreaker/engine/facts.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/source/readable.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::symbols::sdk_structs {

namespace {

using engine::facts::Json;
using engine::facts::Value;

constexpr int64_t kReading = 1;       // stands for the Python's source-text digest of the reading functions (see the header)

const py::dict<py::str, int64_t>& SIZE() {
    static const py::dict<py::str, int64_t> d = [] {
        py::dict<py::str, int64_t> x;
        for (const char* n : {"BYTE", "CHAR", "char", "UCHAR", "BOOLEAN", "CCHAR", "TBYTE", "TCHAR"}) x.set(py::str(n), 1);
        for (const char* n : {"WORD", "SHORT", "short", "USHORT", "WCHAR", "ATOM", "LANGID"}) x.set(py::str(n), 2);
        for (const char* n : {"double", "DOUBLE", "LONGLONG", "DWORDLONG", "ULONGLONG", "__int64", "FILETIME", "LARGE_INTEGER"})
            x.set(py::str(n), 8);
        return x;
    }();
    return d;
}
const py::set<py::str>& FOUR() {
    static const py::set<py::str> s = [] {
        py::set<py::str> x;
        for (const char* n : {"LONG", "DWORD", "UINT", "INT", "int", "BOOL", "ULONG", "COLORREF", "WPARAM", "LPARAM",
                              "LRESULT", "FARPROC", "WNDPROC", "DLGPROC", "HOOKPROC", "float", "FLOAT", "long", "SIZE_T",
                              "LCID", "LCTYPE", "ACCESS_MASK", "REGSAM", "HRESULT", "UINT_PTR", "LONG_PTR", "DWORD_PTR",
                              "unsigned", "TIMERPROC"})
            x.add(py::str(n));
        return x;
    }();
    return s;
}
const py::set<py::str>& SIGNED() {
    static const py::set<py::str> s = [] {
        py::set<py::str> x;
        for (const char* n : {"LONG", "INT", "int", "BOOL", "LPARAM", "LRESULT", "SHORT", "short", "CHAR", "char", "long",
                              "HRESULT", "LONG_PTR", "CCHAR", "INT_PTR"})
            x.add(py::str(n));
        return x;
    }();
    return s;
}

const py::re::Pattern& DECL() {
    static const py::re::Pattern p(py::str(R"(typedef\s+struct\s+(\w+)?\s*\{([^{}]*)\}\s*([^;]+);)"), 16);
    return p;
}
const py::re::Pattern& POINTER_TYPEDEF() {
    static const py::re::Pattern p(
        py::str(R"(typedef\s+(?:const\s+|CONST\s+)?(\w+)\s+(?:FAR\s*|NEAR\s*|far\s*|near\s*)?\*\s*(\w+)\s*;)"), 0);
    return p;
}

std::mutex g_lock;
std::optional<std::tuple<Structs, Pointers>> g_db;       // _db

// _size(t, structs)
std::optional<int64_t> size_of(const py::str& t_in, const Structs& structs) {
    py::str t = t_in.strip();
    if (SIZE().contains(t)) return SIZE().at(t);
    if (FOUR().contains(t) || (t.slice(0, 1) == "H" && t.slice(1, 2).isupper()) || t.startswith(py::str("LP")) ||
        t.startswith(py::str("PC")) || t.startswith(py::str("NP")) || (t.slice(0, 1) == "P" && t.slice(1, 2).isupper())) {
        return 4;
    }
    auto s = structs.get(t);
    if (s && *s) return (*s)->size;
    return std::nullopt;
}

}  // namespace

// ---- Struct ----
std::optional<std::tuple<py::str, py::str>> Struct::typed(int64_t offset, int64_t size_) const {
    for (const Field& f : fields) {
        int64_t off = std::get<0>(f), n = std::get<1>(f);
        const py::str& name_ = std::get<2>(f);
        const py::str& ctype = std::get<3>(f);
        if (off == offset && n == size_ && !ctype.startswith(py::str("struct ")) && !ctype.contains(py::str("["))) {
            return std::make_tuple(name_, ctype);
        }
        if (off <= offset && offset < off + n && ctype.startswith(py::str("struct "))) {
            std::optional<Struct> inner = layout(ctype.slice_from(7));
            std::optional<std::tuple<py::str, py::str>> found;
            if (inner) found = inner->typed(offset - off, size_);
            if (found) return std::make_tuple(name_ + py::str(".") + std::get<0>(*found), std::get<1>(*found));
        }
    }
    return std::nullopt;
}

std::optional<py::str> Struct::at(int64_t offset, int64_t size_) const {
    for (const Field& f : fields) {
        int64_t off = std::get<0>(f), n = std::get<1>(f);
        const py::str& name_ = std::get<2>(f);
        const py::str& ctype = std::get<3>(f);
        if (off == offset && n == size_) return name_;
        if (off <= offset && offset < off + n && ctype.startswith(py::str("struct "))) {
            std::optional<Struct> inner = layout(ctype.slice_from(7));
            if (inner) {
                std::optional<py::str> sub = inner->at(offset - off, size_);
                if (sub && !sub->empty()) return name_ + py::str(".") + *sub;
            }
        }
    }
    return std::nullopt;
}

// ---- the layouts, from the headers ----
namespace {

// Json forms of what the knowledge base keeps: a structure is [size, [[offset, size, name, ctype], ...], biggest],
// or null.
Json json_int(int64_t v) {
    Json j;
    j.kind = Json::Int;
    j.i = v;
    return j;
}
Json json_text(const py::str& s) {
    Json j;
    j.kind = Json::Str;
    j.s = s;
    return j;
}
Json layout_json(const std::optional<Layout>& l) {
    Json j;
    if (!l) return j;     // null
    j.kind = Json::List;
    j.list.push_back(json_int(l->size));
    Json fields;
    fields.kind = Json::List;
    for (const Field& f : l->fields) {
        Json x;
        x.kind = Json::List;
        x.list.push_back(json_int(std::get<0>(f)));
        x.list.push_back(json_int(std::get<1>(f)));
        x.list.push_back(json_text(std::get<2>(f)));
        x.list.push_back(json_text(std::get<3>(f)));
        fields.list.push_back(x);
    }
    j.list.push_back(fields);
    j.list.push_back(json_int(l->biggest));
    return j;
}
std::optional<Layout> layout_of(const Json& j) {
    if (j.kind != Json::List) return std::nullopt;
    Layout l;
    l.size = j.list.at(0).i;
    for (const Json& x : j.list.at(1).list) l.fields.append({x.list.at(0).i, x.list.at(1).i, x.list.at(2).s, x.list.at(3).s});
    l.biggest = j.list.at(2).i;
    return l;
}

// the layouts under construction
struct Builder {
    const py::dict<py::str, std::tuple<py::str, int64_t>>& raw;
    const py::dict<py::str, int64_t>& values;
    Structs structs;

    std::optional<Layout> build(const py::str& name, int64_t depth = 0) {
        auto have = structs.get(name);
        if (have) return *have;
        structs.set(name, std::nullopt);
        if (!raw.contains(name) || depth > 6) return std::nullopt;
        const py::str& body = std::get<0>(raw.at(name));
        int64_t pack = std::get<1>(raw.at(name));
        py::list<Field> fields;
        int64_t off = 0, biggest = 1;
        static const py::re::Pattern drop(py::str(R"(\b(?:CONST|const|volatile|FAR|NEAR|far|near|struct)\b)"), 0);
        static const py::re::Pattern decl_re(py::str(R"(^(\w+)\s*(\**)\s*([\w\s,\[\]*]+)$)"), 0);
        static const py::re::Pattern arr_re(py::str(R"(^(\w+)\s*\[\s*(\w+)\s*\]$)"), 0);
        static const py::re::Pattern word_re(py::str(R"(^\w+$)"), 0);
        for (const py::str& decl_in : py::split(body, py::str(";"))) {
            py::str decl = drop.sub(py::str(" "), decl_in).strip();
            if (decl.empty()) continue;
            auto m = decl_re.match(decl);
            if (!m) return std::nullopt;
            py::str base = m->group(1), star = m->group(2);
            for (const py::str& item_in : py::split(m->group(3), py::str(","))) {
                py::str item = item_in.strip();
                py::str lead = item.lstrip(py::str("*"));
                py::str stars = star + py::str(std::u32string(static_cast<size_t>(item.size() - lead.size()), U'*'));
                item = lead.strip();
                auto arr = arr_re.match(item);
                py::str fname = item;
                std::optional<py::str> count_text;
                if (arr) {
                    fname = arr->group(1);
                    count_text = arr->group(2);
                }
                if (!word_re.match(fname)) return std::nullopt;
                std::optional<int64_t> count;
                if (count_text) {
                    if (count_text->slice(0, 1).isdigit()) count = py::int_parse(*count_text, 0);
                    else count = values.get(*count_text);
                    if (!count) return std::nullopt;
                }
                int64_t n;
                py::str ctype;
                if (!stars.empty()) {
                    n = 4;
                    ctype = py::str("uint4");
                } else if (raw.contains(base) && !SIZE().contains(base) && !FOUR().contains(base)) {
                    std::optional<Layout> inner = build(base, depth + 1);
                    if (!inner) return std::nullopt;
                    n = inner->size;
                    ctype = py::str("struct ") + base;
                } else {
                    std::optional<int64_t> sz = size_of(base, structs);
                    if (!sz) return std::nullopt;
                    n = *sz;
                    if (n == 1 || n == 2 || n == 4 || n == 8) {
                        ctype = py::percent(SIGNED().contains(base) ? py::str("int%d") : py::str("uint%d"), n);
                    } else {
                        return std::nullopt;
                    }
                }
                int64_t align_base = !ctype.startswith(py::str("struct ")) ? n : (*structs.at(base)).biggest;
                int64_t align = std::min<int64_t>(std::min<int64_t>(align_base, pack), 8);
                off = (off + align - 1) / align * align;
                biggest = std::max<int64_t>(biggest, align);
                int64_t total = n * ((count && *count != 0) ? *count : 1);
                fields.append({off, !count ? n : total, fname,
                               !count ? ctype : py::percent(py::str("%s[%d]"), ctype, *count)});
                off += total;
            }
        }
        int64_t size = (off + biggest - 1) / biggest * biggest;
        Layout l;
        l.size = size;
        l.fields = fields;
        l.biggest = biggest;
        structs.set(name, l);
        return l;
    }
};

}  // namespace

std::tuple<Structs, Pointers> _from_headers() {
    const py::dict<py::str, int64_t>& values = source::readable::_constants().values;
    py::dict<py::str, std::tuple<py::str, int64_t>> raw;
    Pointers pointers;
    static const py::re::Pattern block_comment(py::str(R"(/\*.*?\*/)"), 16);
    static const py::re::Pattern line_comment(py::str(R"(//[^\n]*)"), 0);
    static const py::re::Pattern pack_re(py::str(R"(#include\s*<(pshpack(\d)|poppack)\.h>)"), 0);
    for (const py::str& folder : prototypes::_folders().w32) {
        if (!py::os::path::isdir(folder)) continue;
        for (const py::str& fn : py::sorted(py::os::listdir(folder))) {
            if (!fn.lower().endswith(py::str(".h"))) continue;
            py::str text;
            try {
                text = py::open_rb(py::os::path::join(folder, fn)).read().decode("latin1");
            } catch (const py::OSError&) {
                continue;
            }
            text = block_comment.sub(py::str(" "), text);
            text = line_comment.sub(py::str(" "), text);
            // the packing in force at each point of the file
            std::vector<std::pair<int64_t, int64_t>> packs;
            std::vector<int64_t> stack;
            for (const py::re::Match& m : pack_re.finditer(text)) {
                auto d = m.group_opt(2);
                if (d && !d->empty()) {
                    stack.push_back(py::int_parse(*d));
                } else if (!stack.empty()) {
                    stack.pop_back();
                }
                packs.emplace_back(m.start(), stack.empty() ? 8 : stack.back());
            }
            auto pack_at = [&](int64_t p) {
                int64_t cur = 8;
                for (const auto& sv : packs) {
                    if (sv.first > p) break;
                    cur = sv.second;
                }
                return cur;
            };
            for (const py::re::Match& m : DECL().finditer(text)) {
                py::list<py::str> names;
                py::list<py::str> parts = py::split(m.group(3), py::str(","));
                for (const py::str& x : parts) {
                    py::list<py::str> words = py::split(x.strip());
                    names.append(words[-1].lstrip(py::str("*")));
                }
                py::str body = m.group(2);
                if (body.contains(py::str("union")) || body.contains(py::str("#"))) continue;
                py::str main = names[0];
                if (raw.contains(main)) continue;
                raw.set(main, std::make_tuple(body, pack_at(m.start())));
                for (int64_t i = 1; i < parts.size(); ++i) {
                    const py::str& x = parts[i];
                    if (x.contains(py::str("*"))) {
                        py::list<py::str> words = py::split(x.strip());
                        pointers.set(words[-1].lstrip(py::str("*")), main);
                    }
                }
                auto tag = m.group_opt(1);
                if (tag && !tag->empty()) raw.setdefault(*tag, raw.at(main));
            }
            for (const py::re::Match& m : POINTER_TYPEDEF().finditer(text)) pointers.setdefault(m.group(2), m.group(1));
        }
    }
    Builder b{raw, values, Structs()};
    for (const py::str& name : raw.keys()) b.build(name);
    return {b.structs, pointers};
}

std::tuple<Structs, Pointers> _read() {
    std::lock_guard<std::mutex> guard(g_lock);
    if (g_db) return *g_db;
    prototypes::Folders folders = prototypes::_folders();
    py::str stamp = py::hashlib::sha1(py::bytes((prototypes::_stamp(folders) + py::str("|sdk_structs:") +
                                                  py::str(std::to_string(kReading)))
                                                     .utf8()))
                        .hexdigest();
    auto kb = knowledge::store::open();
    std::optional<std::tuple<Structs, Pointers>> made;
    auto from_made = [&]() -> const std::tuple<Structs, Pointers>& {
        if (!made) made = _from_headers();
        return *made;
    };
    py::list<py::str> dirs;
    for (const py::str& d : folders.w32) dirs.append(d);
    py::str source = py::str("the 32-bit headers: ") + py::join(py::str("; "), dirs);
    auto structs_v = kb->ensure_values(py::str("win32"), py::str("struct"), stamp,
                                       [&]() {
                                           py::dict<py::str, Value> out;
                                           for (const auto& kv : std::get<0>(from_made()).items())
                                               out.set(std::get<0>(kv), Value(layout_json(std::get<1>(kv))));
                                           return out;
                                       },
                                       source, knowledge::store::DERIVED);
    auto pointers_v = kb->ensure_values(py::str("win32"), py::str("struct.pointer"), stamp,
                                        [&]() {
                                            py::dict<py::str, Value> out;
                                            for (const auto& kv : std::get<1>(from_made()).items())
                                                out.set(std::get<0>(kv), Value(json_text(std::get<1>(kv))));
                                            return out;
                                        },
                                        source, knowledge::store::DERIVED);
    Structs structs;
    for (const auto& kv : structs_v.items()) structs.set(std::get<0>(kv), layout_of(std::get<Json>(std::get<1>(kv))));
    Pointers pointers;
    for (const auto& kv : pointers_v.items()) {
        const Value& v = std::get<1>(kv);
        pointers.set(std::get<0>(kv), std::holds_alternative<py::str>(v) ? std::get<py::str>(v) : std::get<Json>(v).s);
    }
    g_db = std::make_tuple(structs, pointers);
    return *g_db;
}

std::optional<Struct> layout(const py::str& name) {
    auto [structs, pointers] = _read();
    (void)pointers;
    knowledge::reads::record(py::str("win32"), py::str("struct"), name);
    auto s = structs.get(name);
    if (s && *s) return Struct(name, (*s)->size, (*s)->fields);
    return std::nullopt;
}

std::optional<py::str> pointed_to(const py::str& type_text) {
    auto [structs, pointers] = _read();
    static const py::re::Pattern noise(py::str(R"(\b(?:CONST|const|FAR|NEAR|far|near)\b)"), 0);
    py::str t = noise.sub(py::str(" "), type_text).strip();
    if (t.endswith(py::str("*"))) {
        py::str base = t.slice(0, t.size() - 1).strip();
        knowledge::reads::record(py::str("win32"), py::str("struct"), base);
        auto s = structs.get(base);
        if (s && *s) return base;
        return std::nullopt;
    }
    std::optional<py::str> base = pointers.get(t);
    knowledge::reads::record(py::str("win32"), py::str("struct.pointer"), t);
    if (base && !base->empty()) knowledge::reads::record(py::str("win32"), py::str("struct"), *base);
    if (base && !base->empty()) {
        auto s = structs.get(*base);
        if (s && *s) return base;
    }
    return std::nullopt;
}

py::str definition(const Struct& s, const py::set<py::str>& known) {
    py::str out;
    py::set<py::str> done;
    done.update(known);
    std::function<void(const py::str&)> emit = [&](const py::str& name) {
        if (done.contains(name)) return;
        done.add(name);
        std::optional<Struct> st = layout(name);
        if (!st) throw py::AttributeError("'NoneType' object has no attribute 'fields'");
        py::list<py::str> lines;
        int64_t off = 0;
        static const py::re::Pattern arr_re(py::str(R"(^(.*)\[(\d+)\]$)"), 0);
        for (const Field& f : st->fields) {
            int64_t f_off = std::get<0>(f), n = std::get<1>(f);
            const py::str& fname = std::get<2>(f);
            const py::str& ctype = std::get<3>(f);
            if (ctype.startswith(py::str("struct "))) emit(ctype.slice_from(7));
            if (f_off > off) lines.append(py::percent(py::str("    uint1 codebreaker_pad_%x[%d];"), off, f_off - off));
            auto arr = arr_re.match(ctype);
            py::str base = arr ? arr->group(1) : ctype;
            if (base.startswith(py::str("struct "))) base = base.slice_from(7);
            lines.append(py::percent(py::str("    %s %s%s;"), base, fname,
                                     arr ? py::percent(py::str("[%s]"), arr->group(2)) : py::str("")));
            off = f_off + n;
        }
        if (st->size > off) lines.append(py::percent(py::str("    uint1 codebreaker_pad_%x[%d];"), off, st->size - off));
        out += py::percent(py::str("#pragma pack(push, 1)\ntypedef struct %s {\n%s\n} %s;\n#pragma pack(pop)\n"), name,
                           py::join(py::str("\n"), lines), name);
    };
    emit(s.name);
    return out;
}

}  // namespace codebreaker::symbols::sdk_structs

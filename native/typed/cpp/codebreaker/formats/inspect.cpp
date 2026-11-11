// codebreaker/formats/inspect.cpp -- typed C++ of codebreaker.formats.inspect (see inspect.hpp).
//
// inspect.py -- everything an inspector shows about an executable, as one description. The file is read by the
// format readers (pe, ne, le, detect: the typed ports); nothing is parsed a second time here.  The byte-level
// parts the Python asked formats.dll for (strings, hex listing, categorize: native/formats/win/inspect.cc) are
// written out below on typed values.

#include "codebreaker/formats/inspect.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <set>
#include <string>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/resources.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py_json.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::formats::inspect {

using py::str;

// =====================================================================================================
// Value
// =====================================================================================================
Value Value::from_strings(const py::list<py::str>& xs) {
    Value v = Value::list();
    for (const str& x : xs) v.items.push_back(Value(x));
    return v;
}

Value& Value::push(Value v) {
    items.push_back(std::move(v));
    return items.back();
}

Value& Value::set(const str& key, Value v) {
    for (size_t k = 0; k < keys.size(); k++) {
        if (keys[k] == key) {
            items[k] = std::move(v);
            return items[k];
        }
    }
    keys.push_back(key);
    items.push_back(std::move(v));
    return items.back();
}

const Value* Value::get(const str& key) const {
    for (size_t k = 0; k < keys.size(); k++)
        if (keys[k] == key) return &items[k];
    return nullptr;
}
Value* Value::get(const str& key) {
    for (size_t k = 0; k < keys.size(); k++)
        if (keys[k] == key) return &items[k];
    return nullptr;
}

const Value& Value::at(const str& key) const {
    const Value* v = get(key);
    if (!v) throw py::KeyError(py::repr(key).utf8());
    return *v;
}

bool Value::erase(const str& key) {
    for (size_t k = 0; k < keys.size(); k++) {
        if (keys[k] == key) {
            keys.erase(keys.begin() + static_cast<std::ptrdiff_t>(k));
            items.erase(items.begin() + static_cast<std::ptrdiff_t>(k));
            return true;
        }
    }
    return false;
}

int64_t Value::get_int(const str& key, int64_t dflt) const {
    const Value* v = get(key);
    return v && v->kind == Kind::Int ? v->i : dflt;
}
str Value::get_str(const str& key, const str& dflt) const {
    const Value* v = get(key);
    return v && v->kind == Kind::Str ? v->s : dflt;
}

bool Value::truthy() const {
    switch (kind) {
        case Kind::None: return false;
        case Kind::Bool: return b;
        case Kind::Int: return i != 0;
        case Kind::Str: return !s.empty();
        case Kind::List:
        case Kind::Dict: return !items.empty();
    }
    return false;
}

namespace {

void dump_(std::string& o, const Value& v, const std::optional<int64_t>& indent, bool sort_keys, int64_t level) {
    auto newline = [&](int64_t lv) {
        o += '\n';
        o.append(static_cast<size_t>(*indent * lv), ' ');
    };
    switch (v.kind) {
        case Value::Kind::None: o += "null"; return;
        case Value::Kind::Bool: o += v.b ? "true" : "false"; return;
        case Value::Kind::Int: o += std::to_string(v.i); return;
        case Value::Kind::Str: py::json::put(o, v.s); return;
        case Value::Kind::List: {
            if (v.items.empty()) { o += "[]"; return; }
            o += '[';
            for (size_t k = 0; k < v.items.size(); k++) {
                if (k) o += indent ? "," : ", ";
                if (indent) newline(level + 1);
                dump_(o, v.items[k], indent, sort_keys, level + 1);
            }
            if (indent) newline(level);
            o += ']';
            return;
        }
        case Value::Kind::Dict: {
            if (v.keys.empty()) { o += "{}"; return; }
            std::vector<size_t> order(v.keys.size());
            for (size_t k = 0; k < order.size(); k++) order[k] = k;
            if (sort_keys) std::stable_sort(order.begin(), order.end(), [&](size_t a, size_t b) { return v.keys[a].u32() < v.keys[b].u32(); });
            o += '{';
            bool first = true;
            for (size_t k : order) {
                if (!first) o += indent ? "," : ", ";
                first = false;
                if (indent) newline(level + 1);
                py::json::put(o, v.keys[k]);
                o += ": ";
                dump_(o, v.items[k], indent, sort_keys, level + 1);
            }
            if (indent) newline(level);
            o += '}';
            return;
        }
    }
}

}  // namespace

str dumps(const Value& v, std::optional<int64_t> indent, bool sort_keys) {
    std::string o;
    dump_(o, v, indent, sort_keys, 0);
    return str(o);
}

// =====================================================================================================
// the vocabulary
// =====================================================================================================
const py::list<py::str> SECTIONS = py::list<py::str>{
    "summary", "dos", "headers", "directories", "sections", "exports", "imports", "resources", "relocations", "tls",
    "debug", "certificates", "clr", "rich", "driver", "members", "strings", "hex"};
const py::list<py::str> DIRECTORIES = py::list<py::str>{
    "Export", "Import", "Resource", "Exception", "Security", "BaseReloc", "Debug", "Architecture", "GlobalPtr", "TLS",
    "LoadConfig", "BoundImport", "IAT", "DelayImport", "CLR", "Reserved"};

const py::dict<int64_t, py::str>& RESOURCE_TYPES() {
    static const py::dict<int64_t, py::str> t = [] {
        py::dict<int64_t, py::str> d;
        const std::pair<int64_t, const char*> rows[] = {
            {1, "CURSOR"}, {2, "BITMAP"}, {3, "ICON"}, {4, "MENU"}, {5, "DIALOG"}, {6, "STRING"}, {7, "FONTDIR"},
            {8, "FONT"}, {9, "ACCELERATOR"}, {10, "RCDATA"}, {11, "MESSAGETABLE"}, {12, "GROUP_CURSOR"},
            {14, "GROUP_ICON"}, {16, "VERSION"}, {17, "DLGINCLUDE"}, {19, "PLUGPLAY"}, {20, "VXD"}, {21, "ANICURSOR"},
            {22, "ANIICON"}, {23, "HTML"}, {24, "MANIFEST"}};
        for (const auto& r : rows) d.set(r.first, str(r.second));
        return d;
    }();
    return t;
}

namespace {

using Flags = std::vector<std::pair<int64_t, const char*>>;

const Flags FILE_FLAGS = {{0x0001, "NO_RELOCS"}, {0x0002, "EXECUTABLE"}, {0x0020, "LARGE_ADDRESS"}, {0x0100, "32BIT"},
                          {0x0200, "NO_DEBUG"}, {0x1000, "SYSTEM"}, {0x2000, "DLL"}};
const Flags SECTION_FLAGS = {{0x20, "CODE"}, {0x40, "INITDATA"}, {0x80, "UNINITDATA"}, {0x02000000, "DISCARDABLE"},
                             {0x04000000, "NOTCACHED"}, {0x08000000, "NOTPAGED"}, {0x10000000, "SHARED"},
                             {0x20000000, "EXECUTE"}, {0x40000000, "READ"}, {0x80000000LL, "WRITE"}};
const Flags NE_FLAGS = {{0x0001, "SINGLE_DATA"}, {0x0002, "MULTIPLE_DATA"}, {0x0004, "GLOBAL_INIT"},
                        {0x0008, "PROTECTED_ONLY"}, {0x0800, "SELF_LOADING"}, {0x2000, "LINK_ERRORS"}, {0x8000, "LIBRARY"}};
const Flags SEGMENT_FLAGS = {{0x0001, "DATA"}, {0x0010, "MOVABLE"}, {0x0020, "SHAREABLE"}, {0x0040, "PRELOAD"},
                             {0x0080, "READ_ONLY"}, {0x0100, "RELOCINFO"}, {0x1000, "DISCARDABLE"}};
const Flags OBJECT_FLAGS = {{0x0001, "READ"}, {0x0002, "WRITE"}, {0x0004, "EXECUTE"}, {0x0008, "RESOURCE"},
                            {0x0010, "DISCARDABLE"}, {0x0020, "SHARED"}, {0x0040, "PRELOAD"}, {0x2000, "32BIT"}};

str ne_system(int64_t t) {
    switch (t) {
        case 0: return str("unknown");
        case 1: return str("OS/2");
        case 2: return str("Windows");
        case 3: return str("DOS 4");
        case 4: return str("Windows 386");
        default: return py::to_str(t);
    }
}

str debug_type(int64_t t) {
    switch (t) {
        case 0: return str("Unknown");
        case 1: return str("COFF");
        case 2: return str("CodeView");
        case 3: return str("FPO");
        case 4: return str("Misc");
        case 5: return str("Exception");
        case 6: return str("Fixup");
        case 9: return str("Borland");
        case 12: return str("VC feature");
        case 16: return str("Repro");
        default: return py::to_str(t);
    }
}

py::list<str> flags_of(int64_t value, const Flags& table) {
    py::list<str> out;
    for (const auto& f : table)
        if (value & f.first) out.append(str(f.second));
    return out;
}

// time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime(stamp)) for 0 < stamp < 0x7FFFFFFF, else ''
str when(int64_t stamp) {
    if (!(0 < stamp && stamp < 0x7FFFFFFF)) return str("");
    int64_t days = stamp / 86400, rem = stamp % 86400;
    // civil from days (Howard Hinnant)
    int64_t z = days + 719468;
    int64_t era = (z >= 0 ? z : z - 146096) / 146097;
    int64_t doe = z - era * 146097;
    int64_t yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    int64_t y = yoe + era * 400;
    int64_t doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    int64_t mp = (5 * doy + 2) / 153;
    int64_t d = doy - (153 * mp + 2) / 5 + 1;
    int64_t m = mp < 10 ? mp + 3 : mp - 9;
    if (m <= 2) y++;
    char buf[40];
    std::snprintf(buf, sizeof buf, "%04lld-%02lld-%02lld %02lld:%02lld:%02lld", static_cast<long long>(y), static_cast<long long>(m),
                  static_cast<long long>(d), static_cast<long long>(rem / 3600), static_cast<long long>(rem % 3600 / 60),
                  static_cast<long long>(rem % 60));
    return str(buf);
}

str preview_of(int64_t type_id) {
    if (type_id == 1 || type_id == 2 || type_id == 3) return str("image");
    if (type_id == 6 || type_id == 10 || type_id == 16 || type_id == 23 || type_id == 24) return str("text");
    return str("hex");
}

// "%s: could not be read (%s: %s)" -- a part that failed is noted, not raised
str note_of(const char* key, const py::Exception& e) {
    return py::percent("%s: could not be read (%s: %s)", str(key), str(e.type()), str(std::string(e.what())).slice(0, 80));
}

// _Parts: the value of make(), or `dflt` and a note when it raises
template <class T, class F>
T part(py::list<str>& notes, const char* key, T dflt, F&& make) {
    try {
        return make();
    } catch (const py::Exception& e) {
        notes.append(note_of(key, e));
        return dflt;
    }
}

Value address_value(const Address& a) {
    if (std::holds_alternative<int64_t>(a)) return Value(std::get<int64_t>(a));
    return Value(std::get<str>(a));
}

Value tuple_ints(int64_t a, int64_t b, int64_t c) {
    Value v = Value::list();
    v.push(Value(a));
    v.push(Value(b));
    v.push(Value(c));
    return v;
}

}  // namespace

// ---- to_value
Value to_value(const Section& s) {
    Value v = Value::dict();
    v.set("name", Value(s.name));
    v.set("address", address_value(s.address));
    v.set("size", Value(s.size));
    v.set("file_offset", Value(s.file_offset));
    v.set("file_size", Value(s.file_size));
    v.set("flags", Value::from_strings(s.flags));
    v.set("code", Value(s.code));
    return v;
}

namespace {

Value to_value(const ExportSymbol& e) {
    Value v = Value::dict();
    v.set("name", Value(e.name));
    v.set("ordinal", Value(e.ordinal));
    v.set("hint", Value(e.hint));
    if (e.ne) {
        v.set("address", address_value(e.address));
        v.set("forward", Value(e.forward));
        v.set("movable", Value(e.movable));
        v.set("exported", Value(e.exported));
    } else {
        v.set("forward", Value(e.forward));
        v.set("address", address_value(e.address));
    }
    return v;
}

Value to_value(const ImportSymbol& s) {
    Value v = Value::dict();
    v.set("name", Value(s.name));
    v.set("ordinal", Value(s.ordinal));
    v.set("hint", Value(s.hint));
    if (s.slot) v.set("slot", Value(*s.slot));
    return v;
}

Value opt_int(const std::optional<int64_t>& x) { return x ? Value(*x) : Value(); }

Value to_value(const ResourceItem& it) {
    Value v = Value::dict();
    v.set("name", Value(it.name));
    if (it.pe) v.set("id", opt_int(it.id));
    v.set("language", Value(it.language));
    if (it.pe) v.set("rva", opt_int(it.rva));
    v.set("offset", opt_int(it.offset));
    v.set("size", Value(it.size));
    v.set("type_id", Value(it.type_id));
    v.set("preview", Value(it.preview));
    v.set("wide", Value(it.wide));
    return v;
}

Value to_value(const ResourceType& t) {
    Value v = Value::dict();
    v.set("type", Value(t.type));
    v.set("id", opt_int(t.id));
    v.set("count", Value(t.count));
    Value items = Value::list();
    for (const ResourceItem& it : t.items) items.push(to_value(it));
    v.set("items", std::move(items));
    return v;
}

}  // namespace

Value to_value(const Exports& e) {
    Value v = Value::dict();
    v.set("module", Value(e.module));
    v.set("count", Value(e.count));
    if (e.ordinal_base) v.set("ordinal_base", Value(*e.ordinal_base));
    Value syms = Value::list();
    for (const ExportSymbol& s : e.symbols) syms.push(to_value(s));
    v.set("symbols", std::move(syms));
    return v;
}

Value to_value(const ImportModule& m) {
    Value v = Value::dict();
    v.set("module", Value(m.module));
    v.set("count", Value(m.count));
    v.set("delay", Value(m.delay));
    Value syms = Value::list();
    for (const ImportSymbol& s : m.symbols) syms.push(to_value(s));
    v.set("symbols", std::move(syms));
    return v;
}

Value to_value(const Resources& r) {
    Value v = Value::dict();
    v.set("count", Value(r.count));
    Value types = Value::list();
    for (const ResourceType& t : r.types) types.push(to_value(t));
    v.set("types", std::move(types));
    return v;
}

Value& Description::extra(const str& key) {
    for (auto& e : extras)
        if (e.first == key) return e.second;
    extras.emplace_back(key, Value());
    return extras.back().second;
}
const Value* Description::find_extra(const str& key) const {
    for (const auto& e : extras)
        if (e.first == key) return &e.second;
    return nullptr;
}

Value to_value(const Description& d) {
    Value v = Value::dict();
    v.set("path", Value(d.path));
    v.set("name", Value(d.name));
    v.set("size", Value(d.size));
    v.set("format", d.format ? Value(*d.format) : Value());
    v.set("kind", d.kind ? Value(*d.kind) : Value());
    v.set("ok", Value(d.ok));
    v.set("notes", Value::from_strings(d.notes));
    v.set("summary", d.summary);
    v.set("dos", d.dos);
    v.set("headers", d.headers);
    Value secs = Value::list();
    for (const Section& s : d.sections) secs.push(to_value(s));
    v.set("sections", std::move(secs));
    v.set("exports", to_value(d.exports));
    Value imps = Value::list();
    for (const ImportModule& m : d.imports) imps.push(to_value(m));
    v.set("imports", std::move(imps));
    v.set("resources", to_value(d.resources));
    for (const auto& e : d.extras) v.set(e.first, e.second);
    return v;
}

// =====================================================================================================
// the readers, one format at a time
// =====================================================================================================
namespace {

Value empty_exports_value() { return to_value(Exports{}); }

Value dos_value(const detect::DosHeader& h) {
    Value v = Value::dict();
    v.set("bytes_in_last_page", Value(h.bytes_in_last_page));
    v.set("pages", Value(h.pages));
    v.set("relocations", Value(h.relocations));
    v.set("header_paragraphs", Value(h.header_paragraphs));
    v.set("header_size", Value(h.header_size));
    v.set("min_alloc", Value(h.min_alloc));
    v.set("max_alloc", Value(h.max_alloc));
    v.set("ss", Value(h.ss));
    v.set("sp", Value(h.sp));
    v.set("checksum", Value(h.checksum));
    v.set("ip", Value(h.ip));
    v.set("cs", Value(h.cs));
    v.set("relocation_table", Value(h.relocation_table));
    v.set("overlay", Value(h.overlay));
    v.set("new_header", Value(h.new_header));
    v.set("load_size", Value(h.load_size));
    return v;
}

Value summary_dict(const str& format, const str& type, const str& machine, const str& module, const str& description,
                   const str& subsystem, const str& linker, const str& linked, const Value& image_base, const Value& entry,
                   int64_t code_bytes, int64_t sections, int64_t exports, int64_t import_modules, int64_t imports,
                   int64_t resources) {
    Value s = Value::dict();
    s.set("format", Value(format));
    s.set("type", Value(type));
    s.set("machine", Value(machine));
    s.set("module", Value(module));
    s.set("description", Value(description));
    s.set("subsystem", Value(subsystem));
    s.set("linker", Value(linker));
    s.set("linked", Value(linked));
    s.set("image_base", image_base);
    s.set("entry", entry);
    s.set("code_bytes", Value(code_bytes));
    s.set("sections", Value(sections));
    s.set("exports", Value(exports));
    s.set("import_modules", Value(import_modules));
    s.set("imports", Value(imports));
    s.set("resources", Value(resources));
    return s;
}

int64_t sum_counts(const py::list<ImportModule>& im) {
    int64_t n = 0;
    for (const ImportModule& m : im) n += m.count;
    return n;
}

// ---- 32-bit Windows
void describe_pe(const str& path, Description& info) {
    pe::PEFile f = pe::open_pe(path);
    // headers
    Value dflt_headers = Value::dict();
    dflt_headers.set("file", Value::dict());
    dflt_headers.set("optional", Value::dict());
    py::list<str>& notes = info.notes;
    pe::OptionalHeader optional_header;
    bool have_opt = false;
    info.headers = part(notes, "headers", dflt_headers, [&] {
        Value file = Value::dict();
        file.set("machine", Value(f.machine_name()));
        file.set("machine_id", Value(f.machine));
        file.set("sections", Value(f.n_sections));
        file.set("timestamp", Value(f.timestamp));
        file.set("linked", Value(when(f.timestamp)));
        file.set("optional_header_size", Value(f.opt_size));
        file.set("characteristics", Value::from_strings(flags_of(f.characteristics, FILE_FLAGS)));
        optional_header = f.optional_header();
        have_opt = true;
        const pe::OptionalHeader& o = optional_header;
        Value opt = Value::dict();
        opt.set("magic", Value(o.magic));
        opt.set("linker_version", Value(o.linker_version));
        opt.set("code_size", Value(o.code_size));
        opt.set("initialized_data_size", Value(o.initialized_data_size));
        opt.set("uninitialized_data_size", Value(o.uninitialized_data_size));
        opt.set("entry_point", Value(o.entry_point));
        opt.set("code_base", Value(o.code_base));
        opt.set("image_base", Value(static_cast<int64_t>(o.image_base)));
        opt.set("section_alignment", Value(o.section_alignment));
        opt.set("file_alignment", Value(o.file_alignment));
        opt.set("os_version", Value(o.os_version));
        opt.set("image_version", Value(o.image_version));
        opt.set("subsystem_version", Value(o.subsystem_version));
        opt.set("image_size", Value(o.image_size));
        opt.set("headers_size", Value(o.headers_size));
        opt.set("checksum", Value(o.checksum));
        opt.set("subsystem", Value(o.subsystem));
        opt.set("subsystem_name", Value(o.subsystem_name));
        opt.set("dll_characteristics", Value(o.dll_characteristics));
        Value h = Value::dict();
        h.set("file", std::move(file));
        h.set("optional", std::move(opt));
        return h;
    });
    str subsystem_name, linker_version;
    if (have_opt) {
        subsystem_name = optional_header.subsystem_name;
        linker_version = optional_header.linker_version;
    }
    // directories
    info.extra("directories") = part(notes, "directories", Value::list(), [&] {
        Value out = Value::list();
        int64_t i = 0;
        for (const auto& d : f.datadirs.slice(0, 16)) {
            Value row = Value::dict();
            row.set("name", Value(i < 16 ? DIRECTORIES[i] : py::to_str(i)));
            row.set("rva", Value(std::get<0>(d)));
            row.set("size", Value(std::get<1>(d)));
            out.push(std::move(row));
            i++;
        }
        return out;
    });
    // sections
    info.sections = part(notes, "sections", py::list<Section>(), [&] {
        py::list<Section> out;
        for (const pe::Section& s : f.sections) {
            Section r;
            r.name = s.name;
            r.address = Address(static_cast<int64_t>(f.image_base) + s.vaddr);
            r.size = s.vsize;
            r.file_offset = s.rawptr;
            r.file_size = s.rawsize;
            r.flags = flags_of(s.chars, SECTION_FLAGS);
            r.code = (s.chars & 0x20000020) != 0;
            out.append(r);
        }
        return out;
    });
    // exports
    Exports dflt_exports;
    info.exports = part(notes, "exports", dflt_exports, [&] {
        pe::ExportTable t = f.export_table();
        Exports e;
        e.module = t.module;
        e.count = t.symbols.size();
        e.ordinal_base = t.ordinal_base;
        for (const pe::ExportSymbol& s : t.symbols) {
            ExportSymbol r;
            r.name = s.name;
            r.ordinal = s.ordinal;
            r.hint = s.hint;
            r.forward = s.forward;
            r.address = Address(static_cast<int64_t>(f.image_base) + s.rva);
            e.symbols.append(r);
        }
        return e;
    });
    // imports
    info.imports = part(notes, "imports", py::list<ImportModule>(), [&] {
        py::list<ImportModule> out;
        for (bool delay : {false, true}) {
            for (const pe::ImportModule& m : f.import_table(delay)) {
                ImportModule r;
                r.module = m.module;
                r.count = m.symbols.size();
                r.delay = delay;
                for (const pe::ImportSymbol& s : m.symbols) {
                    ImportSymbol x;
                    x.name = s.name;
                    x.ordinal = s.ordinal;
                    x.hint = s.hint;
                    x.slot = s.slot;
                    r.symbols.append(x);
                }
                out.append(r);
            }
        }
        return out;
    });
    // resources
    info.resources = part(notes, "resources", Resources(), [&] {
        Resources out;
        for (const pe::ResourceType& t : f.resource_tree()) {
            int64_t tid = t.id ? *t.id : -1;
            ResourceType r;
            if (t.id && std::holds_alternative<int64_t>(t.type)) {     // RESOURCE_TYPES.get(type, str(type))
                int64_t n = std::get<int64_t>(t.type);
                r.type = RESOURCE_TYPES().contains(n) ? RESOURCE_TYPES().at(n) : py::to_str(n);
            } else {
                r.type = std::holds_alternative<str>(t.type) ? std::get<str>(t.type) : py::to_str(std::get<int64_t>(t.type));
            }
            r.id = t.id;
            r.count = t.items.size();
            for (const pe::ResourceItem& it : t.items) {
                ResourceItem x;
                x.name = it.name;
                x.pe = true;
                x.id = it.id;
                x.language = it.language;
                x.rva = it.rva;
                x.offset = it.offset;
                x.size = it.size;
                x.type_id = tid;
                x.preview = preview_of(tid);
                x.wide = true;
                r.items.append(x);
            }
            out.count += r.count;
            out.types.append(r);
        }
        return out;
    });
    // relocations
    {
        Value dflt = Value::dict();
        dflt.set("blocks", Value(0));
        dflt.set("entries", Value(0));
        dflt.set("addresses", Value(0));
        info.extra("relocations") = part(notes, "relocations", dflt, [&] {
            auto blocks = f.reloc_blocks();
            int64_t entries = 0;
            for (const auto& b : blocks) entries += std::get<1>(b);
            Value v = Value::dict();
            v.set("blocks", Value(blocks.size()));
            v.set("entries", Value(entries));
            v.set("addresses", Value(f.base_relocs().size()));
            return v;
        });
    }
    // tls
    info.extra("tls") = part(notes, "tls", Value(), [&] {
        std::optional<pe::Tls> t = f.tls();
        if (!t) return Value();
        Value v = Value::dict();
        v.set("start_va", Value(static_cast<int64_t>(t->start_va)));
        v.set("end_va", Value(static_cast<int64_t>(t->end_va)));
        v.set("index_va", Value(static_cast<int64_t>(t->index_va)));
        v.set("callbacks_va", Value(static_cast<int64_t>(t->callbacks_va)));
        return v;
    });
    // debug
    info.extra("debug") = part(notes, "debug", Value::list(), [&] {
        Value out = Value::list();
        for (const pe::DebugEntry& e : f.debug_entries()) {
            Value v = Value::dict();
            v.set("type", Value(debug_type(e.type)));
            v.set("timestamp", Value(e.timestamp));
            v.set("size", Value(e.size));
            v.set("offset", Value(e.offset));
            v.set("linked", Value(when(e.timestamp)));
            out.push(std::move(v));
        }
        return out;
    });
    info.extra("certificates") = part(notes, "certificates", Value::list(), [&] {
        Value out = Value::list();
        for (const pe::Certificate& c : f.certificates()) {
            Value v = Value::dict();
            v.set("length", Value(c.length));
            v.set("revision", Value(c.revision));
            v.set("type", Value(c.type));
            out.push(std::move(v));
        }
        return out;
    });
    info.extra("clr") = part(notes, "clr", Value(), [&] {
        std::optional<pe::Clr> c = f.clr();
        if (!c) return Value();
        Value v = Value::dict();
        v.set("runtime_version", Value(c->runtime_version));
        v.set("flags", Value(c->flags));
        v.set("entry_token", Value(c->entry_token));
        return v;
    });
    info.extra("rich") = part(notes, "rich", Value(), [&] {
        std::optional<pe::Rich> r = f.rich();
        if (!r) return Value();
        Value v = Value::dict();
        v.set("key", Value(r->key));
        Value entries = Value::list();
        for (const auto& e : r->entries) entries.push(tuple_ints(std::get<0>(e), std::get<1>(e), std::get<2>(e)));
        v.set("entries", std::move(entries));
        return v;
    });
    info.extra("load_config") = part(notes, "load_config", Value(false), [&] { return Value(std::get<1>(f.directory(10)) > 0); });
    info.extra("symbols_file") = part(notes, "symbols_file", Value(), [&] {
        std::optional<pe::CodeView> c = f.debug_codeview();
        if (!c) return Value();
        Value v = Value::dict();
        v.set("format", Value(c->format));
        if (c->guid) v.set("guid", Value(*c->guid));
        if (c->signature) v.set("signature", Value(*c->signature));
        v.set("age", Value(c->age));
        v.set("pdb", Value(c->pdb));
        return v;
    });
    const Exports& ex = info.exports;
    Value entry(f.entry ? static_cast<int64_t>(f.image_base) + f.entry : int64_t(0));
    info.summary = summary_dict(f.is_plus ? str("PE32+") : str("PE32"), f.is_dll() ? str("DLL") : str("EXE"), f.machine_name(),
                                ex.module, str(""), subsystem_name, linker_version, when(f.timestamp),
                                Value(static_cast<int64_t>(f.image_base)), entry, f.code_bytes(), f.sections.size(), ex.count,
                                info.imports.size(), sum_counts(info.imports), info.resources.count);
}

// ---- 16-bit Windows
void describe_ne(const str& path, Description& info) {
    ne::NEFile f(path);
    const ne::Header& h = f.h;
    py::list<str>& notes = info.notes;
    str linker = py::percent("%d.%d", h.linker_ver, h.linker_rev);
    info.headers = part(notes, "headers", Value::dict(), [&] {
        Value v = Value::dict();
        v.set("linker_version", Value(linker));
        v.set("flags", Value::from_strings(flags_of(h.flags, NE_FLAGS)));
        v.set("flags_value", Value(h.flags));
        v.set("automatic_data_segment", Value(h.autodata_seg));
        v.set("heap", Value(h.heap));
        v.set("stack", Value(h.stack));
        v.set("entry_segment", Value(h.csip >> 16));
        v.set("entry_offset", Value(h.csip & 0xFFFF));
        v.set("stack_segment", Value(h.sssp >> 16));
        v.set("stack_pointer", Value(h.sssp & 0xFFFF));
        v.set("segments", Value(h.n_segs));
        v.set("module_references", Value(h.n_modrefs));
        v.set("movable_entries", Value(h.n_movable));
        v.set("alignment_shift", Value(h.align_shift));
        v.set("target_system", Value(ne_system(h.exetype)));
        bool has_win = h.expect_winver && *h.expect_winver != 0;
        v.set("expected_windows", Value(has_win ? py::percent("%d.%d", *h.expect_winver >> 8, *h.expect_winver & 0xFF) : str("")));
        v.set("checksum", Value(h.crc));
        return v;
    });
    info.sections = part(notes, "sections", py::list<Section>(), [&] {
        py::list<Section> out;
        for (const ne::Segment& s : f.segments()) {
            Section r;
            r.name = py::percent("seg %d", s.idx);
            r.address = Address(py::percent("%d:0000", s.idx));
            r.size = s.minalloc;
            r.file_offset = s.file_off;
            r.file_size = s.length;
            r.flags = flags_of(s.flags, SEGMENT_FLAGS);
            r.code = s.kind == str("CODE");
            out.append(r);
        }
        return out;
    });
    // _names: (module, description, {ordinal: name})
    struct Names {
        str module, description;
        std::vector<std::pair<int64_t, str>> by_ordinal;   // in the dict's order
    };
    auto set_name = [](Names& n, int64_t ordinal, const str& name) {
        for (auto& e : n.by_ordinal) {
            if (e.first == ordinal) { e.second = name; return; }
        }
        n.by_ordinal.emplace_back(ordinal, name);
    };
    Names names = part(notes, "_names", Names(), [&] {
        auto res = f.resident_names(), non = f.nonresident_names();
        Names n;
        n.module = res.size() ? std::get<0>(res[0]) : str("");
        n.description = non.size() ? std::get<0>(non[0]) : str("");
        for (int64_t k = 1; k < res.size(); k++) set_name(n, std::get<1>(res[k]), std::get<0>(res[k]));
        for (int64_t k = 1; k < non.size(); k++) set_name(n, std::get<1>(non[k]), std::get<0>(non[k]));
        return n;
    });
    auto name_of = [&](int64_t ordinal) {
        for (const auto& e : names.by_ordinal)
            if (e.first == ordinal) return e.second;
        return str("");
    };
    Exports dflt_exports;
    dflt_exports.module = names.module;
    info.exports = part(notes, "exports", dflt_exports, [&] {
        Exports out;
        py::set<int64_t> seen;
        for (const ne::Entry& e : f.entries()) {
            if (e.kind == str("UNUSED")) continue;
            seen.add(e.ordinal);
            ExportSymbol s;
            s.ne = true;
            s.name = name_of(e.ordinal);
            s.ordinal = e.ordinal;
            s.hint = -1;
            s.address = Address(py::percent("%d:%04x", *e.seg, *e.offset));
            s.forward = str("");
            s.movable = e.kind == str("MOVEABLE");
            s.exported = (*e.flags & 1) != 0;
            out.symbols.append(s);
        }
        std::vector<std::pair<int64_t, str>> sorted_names = names.by_ordinal;
        std::sort(sorted_names.begin(), sorted_names.end(), [](const auto& a, const auto& b) {
            return a.first != b.first ? a.first < b.first : a.second.u32() < b.second.u32();
        });
        for (const auto& on : sorted_names) {
            if (seen.contains(on.first)) continue;
            ExportSymbol s;
            s.ne = true;
            s.name = on.second;
            s.ordinal = on.first;
            s.hint = -1;
            s.address = Address(str(""));
            s.forward = str("");
            out.symbols.append(s);
        }
        out.module = names.module;
        out.count = out.symbols.size();
        return out;
    });
    info.imports = part(notes, "imports", py::list<ImportModule>(), [&] {
        py::list<str> mods = f.module_refs();
        std::vector<std::pair<std::set<int64_t>, std::vector<str>>> used(static_cast<size_t>(mods.size()));
        for (const ne::Segment& s : f.segments()) {
            for (const ne::Relocation& r : f.relocations(s)) {
                if ((r.kind == str("IMPORTORD") || r.kind == str("IMPORTNAME")) && 1 <= *r.module && *r.module <= mods.size()) {
                    auto& u = used[static_cast<size_t>(*r.module - 1)];
                    if (r.kind == str("IMPORTORD")) {
                        u.first.insert(*r.ordinal);
                    } else {
                        str n = f.imported_name(*r.name_off);
                        if (std::find(u.second.begin(), u.second.end(), n) == u.second.end()) u.second.push_back(n);
                    }
                }
            }
        }
        py::list<ImportModule> out;
        for (int64_t k = 0; k < mods.size(); k++) {
            ImportModule m;
            m.module = mods[k];
            m.delay = false;
            for (int64_t o : used[static_cast<size_t>(k)].first) {
                ImportSymbol s;
                s.name = str("");
                s.ordinal = o;
                s.hint = -1;
                m.symbols.append(s);
            }
            std::vector<str> by_name = used[static_cast<size_t>(k)].second;
            std::sort(by_name.begin(), by_name.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
            for (const str& n : by_name) {
                ImportSymbol s;
                s.name = n;
                s.ordinal = -1;
                s.hint = -1;
                m.symbols.append(s);
            }
            m.count = m.symbols.size();
            out.append(m);
        }
        return out;
    });
    info.resources = part(notes, "resources", Resources(), [&] {
        std::vector<std::pair<str, py::list<ResourceItem>>> types;   // insertion order
        for (const ne::Resource& r : f.resources()) {
            str tname = r.type.empty() ? str("?") : r.type;
            int64_t tid = -1;
            bool known = false;
            for (const auto& kv : RESOURCE_TYPES().items()) {
                if (std::get<1>(kv) == tname) { tid = std::get<0>(kv); known = true; break; }
            }
            if (!known && py::re::fullmatch("#\\d+", tname)) tid = py::int_parse(tname.slice_from(1));
            ResourceItem it;
            it.name = r.id;
            it.pe = false;
            it.language = 0;
            it.offset = r.off;
            it.size = r.length;
            it.type_id = tid;
            it.preview = preview_of(tid);
            it.wide = false;
            bool placed = false;
            for (auto& t : types) {
                if (t.first == tname) { t.second.append(it); placed = true; break; }
            }
            if (!placed) {
                types.emplace_back(tname, py::list<ResourceItem>());
                types.back().second.append(it);
            }
        }
        Resources out;
        for (auto& t : types) {
            ResourceType r;
            r.type = t.first;
            r.id = t.second[0].type_id >= 0 ? std::optional<int64_t>(t.second[0].type_id) : std::nullopt;
            r.count = t.second.size();
            r.items = t.second;
            out.count += r.count;
            out.types.append(r);
        }
        return out;
    });
    {
        Value dflt = Value::dict();
        dflt.set("records", Value(0));
        info.extra("relocations") = part(notes, "relocations", dflt, [&] {
            int64_t n = 0;
            for (const ne::Segment& s : f.segments()) n += f.relocations(s).size();
            Value v = Value::dict();
            v.set("records", Value(n));
            return v;
        });
    }
    int64_t code_bytes = 0;
    for (const Section& s : info.sections)
        if (s.code) code_bytes += s.file_size;
    info.summary = summary_dict(str("NE"), (h.flags & 0x8000) ? str("DLL") : str("EXE"), str("i286/i386 (16-bit)"), names.module,
                                names.description, ne_system(h.exetype), linker, str(""), Value(0),
                                Value(py::percent("%d:%04x", h.csip >> 16, h.csip & 0xFFFF)), code_bytes, info.sections.size(),
                                info.exports.count, info.imports.size(), sum_counts(info.imports), info.resources.count);
}

// ---- drivers: what _driver() fills in, in the order it fills it
struct DriverParts {
    std::optional<Value> headers;
    std::optional<py::list<Section>> sections;
    std::optional<py::list<ImportModule>> imports;
    std::optional<Value> driver;
    std::optional<Exports> exports;
    std::optional<Resources> resources;
    std::optional<Value> summary;
};

str cpu_name(int64_t c) {
    switch (c) {
        case 1: return str("80286");
        case 2: return str("80386");
        case 3: return str("80486");
        case 4: return str("Pentium");
        default: return py::to_str(c);
    }
}
str driver_os_name(int64_t o) {
    switch (o) {
        case 1: return str("OS/2");
        case 2: return str("Windows");
        case 3: return str("DOS 4");
        case 4: return str("Windows 386");
        default: return py::to_str(o);
    }
}

// one LE/LX image: a driver file, or one member of a W3 bundle
void describe_driver(const le::LEFile& f, const str& name, DriverParts& out, py::list<str>& notes) {
    Value head = part(notes, "headers", Value::dict(), [&] {
        le::Header h = f.header();
        Value v = Value::dict();
        v.set("signature", Value(h.signature));
        v.set("cpu", Value(h.cpu));
        v.set("os", Value(h.os));
        v.set("module_version", Value(h.module_version));
        v.set("module_flags", Value(h.module_flags));
        v.set("pages", Value(h.pages));
        v.set("entry_object", Value(h.entry_object));
        v.set("entry", Value(h.entry));
        v.set("stack_object", Value(h.stack_object));
        v.set("stack", Value(h.stack));
        v.set("page_size", Value(h.page_size));
        v.set("last_page_bytes", Value(h.last_page_bytes));
        v.set("fixup_section_size", Value(h.fixup_section_size));
        v.set("loader_section_size", Value(h.loader_section_size));
        v.set("objects", Value(h.objects));
        v.set("imported_modules", Value(h.imported_modules));
        v.set("data_pages_offset", Value(h.data_pages_offset));
        v.set("device_id", Value(h.device_id));
        v.set("ddk_version", Value(h.ddk_version));
        v.set("cpu_name", Value(cpu_name(f.cpu)));
        v.set("os_name", Value(driver_os_name(f.ostype)));
        return v;
    });
    out.headers = head;
    py::list<Section> objs = part(notes, "sections", py::list<Section>(), [&] {
        py::list<Section> r;
        for (const le::Object& o : f.objects) {
            Section s;
            s.name = py::percent("object %d", o.idx);
            s.address = Address(o.base);
            s.size = o.vsize;
            s.file_offset = f.datapages + (o.pmidx - 1) * f.pagesize;
            s.file_size = o.pcnt * f.pagesize;
            s.flags = flags_of(o.flags, OBJECT_FLAGS);
            s.code = o.exec;
            r.append(s);
        }
        return r;
    });
    out.sections = objs;
    py::list<ImportModule> im = part(notes, "imports", py::list<ImportModule>(), [&] {
        py::list<str> mods = f.import_modules();
        std::vector<std::set<int64_t>> used(static_cast<size_t>(mods.size()));
        try {
            for (const auto& kv : f.fixups().items()) {
                const le::Fixup& fx = std::get<1>(kv);
                if (std::holds_alternative<le::ImportFixup>(fx)) {
                    const le::ImportFixup& ix = std::get<le::ImportFixup>(fx);
                    if (1 <= ix.module && ix.module <= mods.size()) used[static_cast<size_t>(ix.module - 1)].insert(ix.ordinal);
                }
            }
        } catch (const py::Exception& e) {
            notes.append(py::percent("imports: the fixup records could not all be read (%s)", str(e.type())));
        }
        py::list<ImportModule> r;
        for (int64_t k = 0; k < mods.size(); k++) {
            ImportModule m;
            m.module = mods[k];
            m.count = static_cast<int64_t>(used[static_cast<size_t>(k)].size());
            for (int64_t o : used[static_cast<size_t>(k)]) {
                ImportSymbol s;
                s.name = str("");
                s.ordinal = o;
                s.hint = -1;
                m.symbols.append(s);
            }
            r.append(m);
        }
        return r;
    });
    out.imports = im;
    Value ddb = part(notes, "driver", Value(), [&] {
        std::optional<le::Ddb> d = f.ddb();
        if (!d) return Value();
        Value v = Value::dict();
        v.set("obj", Value(d->obj));
        v.set("rva", Value(d->rva));
        v.set("version", Value(d->version));
        v.set("device_id", Value(d->device_id));
        v.set("major", Value(d->major));
        v.set("minor", Value(d->minor));
        v.set("flags", Value(d->flags));
        v.set("name", Value(d->name));
        v.set("init_order", Value(d->init_order));
        v.set("control_proc", Value(d->control_proc));
        return v;
    });
    out.driver = ddb;
    str module = part(notes, "_name", str(""), [&] { return f.resident_name(); });
    str description = part(notes, "_text", str(""), [&] { return f.description(); });
    Exports ex;
    ex.module = module;
    out.exports = ex;
    out.resources = Resources();
    int64_t code_bytes = 0;
    for (const Section& o : objs)
        if (o.code) code_bytes += o.size;
    str ddb_name = ddb.is_dict() ? ddb.get_str("name") : str("");
    str mod = !ddb_name.empty() ? ddb_name : !module.empty() ? module : name;
    Value entry = f.eip_obj ? Value(py::percent("%d:%08x", f.eip_obj, f.eip)) : Value(0);
    out.summary = summary_dict(str(f.sig.decode("latin1")), str("driver"), head.get_str("cpu_name"), mod, description,
                               head.get_str("os_name"), str(""), str(""), Value(0), entry, code_bytes, objs.size(), 0,
                               im.size(), sum_counts(im), 0);
}

void describe_le(const str& path, Description& info) {
    py::bytes data = py::open_rb(path).read();
    le::LEFile f = le::LEFile::from_buffer(data);
    DriverParts parts;
    // the parts are copied out as _driver fills them in, so a failure keeps what was done
    struct Copy {
        Description& info;
        DriverParts& p;
        ~Copy() {
            if (p.headers) info.headers = *p.headers;
            if (p.sections) info.sections = *p.sections;
            if (p.imports) info.imports = *p.imports;
            if (p.driver) info.extra("driver") = *p.driver;
            if (p.exports) info.exports = *p.exports;
            if (p.resources) info.resources = *p.resources;
            if (p.summary) info.summary = *p.summary;
        }
    } copy{info, parts};
    describe_driver(f, info.name, parts, info.notes);
}

// a W3 file (vmm32.vxd): several drivers packed one after another
void describe_bundle(const py::bytes& data, Description& info) {
    if (data.size() < 0x40)
        throw py::StructError(py::percent("unpack_from requires a buffer of at least %d bytes for unpacking 4 bytes at offset 60 (actual buffer size is %d)", 64, data.size()).utf8());
    int64_t lfa = py::unpack_from1<int64_t>("<I", data, 0x3C);
    le::W3File w3(data, lfa);
    Value members = Value::list();
    std::vector<Value> summaries;
    for (const le::W3Entry& e : w3.entries) {
        py::list<str> notes;
        DriverParts parts;
        if (data.slice(e.off, e.off + 2) == py::bytes("LE") || data.slice(e.off, e.off + 2) == py::bytes("LX")) {
            try {
                le::LEFile lf(data, e.off);
                describe_driver(lf, e.name, parts, notes);
            } catch (const py::Exception& err) {
                notes.append(py::percent("%s: %s", str(err.type()), str(std::string(err.what())).slice(0, 80)));
            }
        }
        Value sub = Value::dict();
        sub.set("name", Value(e.name));
        sub.set("offset", Value(e.off));
        sub.set("size", Value(e.size));
        sub.set("notes", Value::from_strings(notes));
        if (parts.headers) sub.set("headers", *parts.headers);
        if (parts.sections) {
            Value v = Value::list();
            for (const Section& s : *parts.sections) v.push(to_value(s));
            sub.set("sections", std::move(v));
        }
        if (parts.imports) {
            Value v = Value::list();
            for (const ImportModule& m : *parts.imports) v.push(to_value(m));
            sub.set("imports", std::move(v));
        }
        if (parts.driver) sub.set("driver", *parts.driver);
        if (parts.exports) sub.set("exports", to_value(*parts.exports));
        if (parts.resources) sub.set("resources", to_value(*parts.resources));
        if (parts.summary) {
            sub.set("summary", *parts.summary);
            summaries.push_back(*parts.summary);
        }
        members.push(std::move(sub));
    }
    info.extra("members") = members;
    Value headers = Value::dict();
    headers.set("version", Value(w3.version));
    headers.set("members", Value(w3.count));
    info.headers = headers;
    info.sections = py::list<Section>();
    for (const Value& m : members.items) {
        Section s;
        s.name = m.get_str("name");
        s.address = Address(int64_t(0));
        s.size = m.get_int("size");
        s.file_offset = m.get_int("offset");
        s.file_size = m.get_int("size");
        s.code = true;
        info.sections.append(s);
    }
    info.exports = Exports();
    info.imports = py::list<ImportModule>();
    info.resources = Resources();
    int64_t code_bytes = 0, imports = 0;
    for (const Value& s : summaries) {
        code_bytes += s.get_int("code_bytes");
        imports += s.get_int("imports");
    }
    info.summary = summary_dict(str("W3"), str("driver bundle"), str("80386"), info.name,
                                py::percent("%d drivers in one file", members.size()), str("Windows 386"), str(""), str(""),
                                Value(0), Value(0), code_bytes, members.size(), 0, 0, imports, 0);
}

// ---- DOS
void describe_dos(const py::bytes& data, Description& info, const py::list<std::tuple<int64_t, int64_t>>& relocs) {
    const Value& h = info.dos;
    int64_t start = h.get_int("header_size"), size = h.get_int("load_size");
    info.headers = h;
    info.sections = py::list<Section>();
    Section program;
    program.name = str("program");
    program.address = Address(str("0000:0000"));
    program.size = size + h.get_int("min_alloc") * 16;
    program.file_offset = start;
    program.file_size = size;
    program.code = true;
    info.sections.append(program);
    int64_t extra = data.size() - start - size;
    if (extra > 0) {
        Section after;
        after.name = str("after the program");
        after.address = Address(str(""));
        after.size = extra;
        after.file_offset = start + size;
        after.file_size = extra;
        after.code = false;
        info.sections.append(after);
    }
    Value r = Value::dict();
    r.set("records", Value(h.get_int("relocations")));
    Value entries = Value::list();
    int64_t n = 0;
    for (const auto& e : relocs) {
        if (n++ >= 512) break;
        entries.push(Value(py::percent("%04x:%04x", std::get<0>(e), std::get<1>(e))));
    }
    r.set("entries", std::move(entries));
    info.extra("relocations") = r;
    info.exports = Exports();
    info.imports = py::list<ImportModule>();
    info.resources = Resources();
    info.summary = summary_dict(str("MZ"), str("DOS program"), str("8086 (16-bit)"), info.name, str(""), str("DOS"), str(""),
                                str(""), Value(0), Value(py::percent("%04x:%04x", h.get_int("cs"), h.get_int("ip"))), size, 1, 0,
                                0, 0, 0);
}

Description make_base(const str& path) {
    Description info;
    info.path = path;
    info.name = py::os::path::basename(path).lower();
    info.size = 0;
    info.ok = false;
    info.exports.module = str("");
    return info;
}

// a file that is not an MZ executable: what detect.identify says it is, with what the format's reader adds
Description other(const str& path, Description info) {
    try {
        info.size = py::os::path::getsize(path);
    } catch (const py::OSError& e) {
        info.notes.append(py::percent("cannot be read: %s", str(std::string(e.what()))));
        return info;
    }
    py::list<detect::Detected> found = detect::identify(path);
    if (found.empty()) {
        info.notes.append(str("not a format CodeBreaker recognises"));
        return info;
    }
    const detect::Detected& m = found[0];
    info.format = m.format;
    info.kind = m.family;
    Value s = Value::dict();
    s.set("format", Value(m.format));
    s.set("type", Value(m.detail));
    s.set("family", Value(m.family));
    s.set("read with", Value(!m.command.empty() ? str("codebreaker ") + m.command : str("(nothing yet)")));
    info.summary = s;
    if (m.command == str("media") || m.family == str("xbox") || m.family == str("xbox360")) {
        try {
            Value add = xbox_summary() ? xbox_summary()(path, m)
                                       : throw py::ImportError("codebreaker.formats.xbox.summary has no typed port yet: set inspect::xbox_summary()");
            for (size_t k = 0; k < add.keys.size(); k++) info.summary.set(add.keys[k], add.items[k]);
        } catch (const py::Exception& e) {
            info.notes.append(py::percent("%s: %s", str(e.type()), str(std::string(e.what())).slice(0, 120)));
        }
    }
    info.ok = true;
    return info;
}

}  // namespace

XboxSummary& xbox_summary() {
    static XboxSummary hook;
    return hook;
}

Description describe(const str& path) {
    Description info = make_base(path);
    std::optional<str> fmt = py::os::path::isfile(path) ? detect::format_of(path) : std::nullopt;
    if (!fmt) return other(path, info);
    py::bytes data;
    try {
        data = py::open_rb(path).read();
    } catch (const py::OSError& e) {
        info.notes.append(py::percent("cannot be read: %s", str(std::string(e.what()))));
        return info;
    }
    info.size = data.size();
    info.format = *fmt;
    info.kind = detect::lane(path);
    try {
        std::optional<detect::DosHeader> h = detect::dos_header(data);
        py::list<std::tuple<int64_t, int64_t>> relocs;
        info.dos = Value::dict();
        if (h) {
            relocs = h->relocation_entries;
            info.dos = dos_value(*h);
        }
        if (*fmt == str("PE")) {
            describe_pe(path, info);
        } else if (*fmt == str("NE")) {
            describe_ne(path, info);
        } else if (*fmt == str("LE") || *fmt == str("LX")) {
            describe_le(path, info);
        } else if (*fmt == str("W3")) {
            describe_bundle(data, info);
        } else {
            describe_dos(data, info, relocs);
        }
        info.ok = true;
    } catch (const py::Exception& e) {                      // the header itself is damaged
        info.notes.append(py::percent("%s: %s", str(e.type()), str(std::string(e.what())).slice(0, 120)));
    }
    return info;
}

// =====================================================================================================
// folders
// =====================================================================================================
namespace {

const std::vector<std::pair<const char*, const char*>>& SOURCE() {
    static const std::vector<std::pair<const char*, const char*>> t = {
        {".c", "C"}, {".h", "C/C++ header"}, {".cc", "C++"}, {".cpp", "C++"}, {".cxx", "C++"}, {".hpp", "C++ header"},
        {".hh", "C++ header"}, {".cs", "C#"}, {".java", "Java"}, {".py", "Python"}, {".asm", "assembly"},
        {".s", "assembly"}, {".inc", "assembly include"}, {".mac", "assembly macros"}, {".js", "JavaScript"},
        {".ts", "TypeScript"}, {".rs", "Rust"}, {".go", "Go"}, {".lua", "Lua"}, {".hlsl", "HLSL shader"},
        {".fx", "effect shader"}, {".ps1", "PowerShell"}, {".bat", "batch"}, {".cmd", "batch"}, {".sh", "shell"},
        {".pl", "Perl"}, {".md", "Markdown"}, {".txt", "text"}, {".json", "JSON"}, {".xml", "XML"}, {".toml", "TOML"},
        {".yml", "YAML"}, {".yaml", "YAML"}, {".ini", "INI"}, {".cfg", "config"}, {".csv", "CSV"}, {".html", "HTML"},
        {".css", "CSS"}, {".def", "module definition"}, {".mak", "makefile"}, {".cmake", "CMake"},
        {".gradle", "Gradle"}, {".csproj", "C# project"}, {".vcxproj", "C++ project"},
        {".sln", "Visual Studio solution"}, {".obj", "Wavefront OBJ model"}, {".mtl", "Wavefront material"},
        {".resx", ".NET resources"}};
    return t;
}

// the contents of os.walk(top) for one directory: its sub-directories and files, as the OS lists them
void split_dir(const str& d, py::list<str>& dirs, py::list<str>& files) {
    for (const str& n : py::os::listdir(d)) {
        if (py::os::path::isdir(py::os::path::join(d, n))) dirs.append(n);
        else files.append(n);
    }
}

bool contains_name(const std::vector<str>& v, const str& n) { return std::find(v.begin(), v.end(), n) != v.end(); }

}  // namespace

std::optional<detect::Detected> _text_kind(const str& path, const str& name) {
    str ext = std::get<1>(py::os::path::splitext(name)).lower();
    str what;
    static const std::vector<str> special = {"Makefile", "CMakeLists.txt", "LICENSE", "NOTICE", "README", "AUTHORS"};
    if (contains_name(special, name)) {
        what = name == str("Makefile") ? str("makefile") : name == str("CMakeLists.txt") ? str("CMake") : str("text");
    } else {
        bool found = false;
        for (const auto& e : SOURCE()) {
            if (ext == str(e.first)) {
                what = str(e.second);
                found = true;
                break;
            }
        }
        if (!found) return std::nullopt;
    }
    try {
        py::file f = py::open_rb(path);
        py::bytes head = f.read(4096);
        if (head.raw().find('\0') != std::string::npos) return std::nullopt;
    } catch (const py::OSError&) {
        return std::nullopt;
    }
    detect::Detected d;
    d.format = str("source / text: ") + what;
    d.family = str("source");
    d.command = str("");
    return d;
}

FolderMap folder_map(const str& folder, const std::function<void(const str&)>& log) {
    // by: (format, detail) -> row; other: extension -> row (both in insertion order)
    std::vector<std::pair<std::pair<str, str>, FolderFormat>> by;
    std::vector<FolderOther> other_rows;
    int64_t files = 0, total = 0;
    static const std::vector<str> skip = {".git", "__pycache__", "node_modules", ".gradle", ".vs"};

    auto visit_dir = [&](auto&& self, const str& dp) -> void {
        py::list<str> dn, fn;
        split_dir(dp, dn, fn);
        std::vector<str> dirs;
        for (const str& d : dn)
            if (!contains_name(skip, d)) dirs.push_back(d);
        std::sort(dirs.begin(), dirs.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
        std::vector<str> names;
        for (const str& x : fn) names.push_back(x);
        std::sort(names.begin(), names.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
        for (const str& n : names) {
            str p = py::os::path::join(dp, n);
            int64_t size;
            try {
                size = py::os::path::getsize(p);
            } catch (const py::OSError&) {
                continue;
            }
            files++;
            total += size;
            if (!size) continue;
            std::optional<str> fmt = detect::format_of(p);
            std::optional<detect::Detected> m;
            if (fmt) {
                detect::Detected d;
                str f = *fmt;
                d.format = f == str("PE") ? str("PE executable") : f == str("NE") ? str("16-bit Windows (NE)")
                           : f == str("LE") ? str("virtual device driver (LE)") : f == str("LX") ? str("OS/2 / VxD (LX)")
                           : f == str("MZ") ? str("DOS program (MZ)") : f;
                d.family = f == str("MZ") ? str("dos") : str("windows");
                d.command = str("info");
                m = d;
                if (f == str("PE") || f == str("W3") || f == str("W4")) {   // the CPU decides the platform; a bundle of drivers
                    py::list<detect::Detected> found = detect::identify(p);
                    if (!found.empty()) m = found[0];
                }
            } else {
                py::list<detect::Detected> found = detect::identify(p);
                if (!found.empty()) m = found[0];
            }
            if (!m) m = _text_kind(p, n);
            if (!m) {
                str ext = std::get<1>(py::os::path::splitext(n)).lower();
                if (ext.empty()) ext = str("(none)");
                FolderOther* r = nullptr;
                for (auto& o : other_rows)
                    if (o.extension == ext) { r = &o; break; }
                if (!r) {
                    other_rows.push_back(FolderOther{ext, 0, 0, p});
                    r = &other_rows.back();
                }
                r->files += 1;
                r->bytes += size;
            } else {
                str detail = m->format == str("PE executable") ? m->detail : str("");
                FolderFormat* r = nullptr;
                for (auto& b : by)
                    if (b.first.first == m->format && b.first.second == detail) { r = &b.second; break; }
                if (!r) {
                    FolderFormat row;
                    row.format = m->format + (!detail.empty() ? py::percent(" (%s)", detail) : str(""));
                    row.family = m->family;
                    row.command = m->command;
                    row.example = p;
                    by.emplace_back(std::make_pair(m->format, detail), row);
                    r = &by.back().second;
                }
                r->files += 1;
                r->bytes += size;
            }
        }
        if (log && files && files % 2000 == 0) log(py::percent("  %d files...", files));
        for (const str& d : dirs) {
            str sub = py::os::path::join(dp, d);
            if (!py::os::path::islink(sub)) self(self, sub);
        }
    };
    if (py::os::path::isdir(folder)) visit_dir(visit_dir, folder);

    FolderMap out;
    out.folder = folder;
    out.files = files;
    out.bytes = total;
    std::vector<FolderFormat> formats;
    for (auto& b : by) formats.push_back(b.second);
    std::stable_sort(formats.begin(), formats.end(), [](const FolderFormat& a, const FolderFormat& b) {
        return a.bytes != b.bytes ? a.bytes > b.bytes : a.files > b.files;
    });
    std::stable_sort(other_rows.begin(), other_rows.end(), [](const FolderOther& a, const FolderOther& b) {
        return a.bytes != b.bytes ? a.bytes > b.bytes : a.files > b.files;
    });
    for (const auto& f : formats) out.formats.append(f);
    for (const auto& o : other_rows) out.unrecognised.append(o);
    return out;
}

Value to_value(const FolderMap& m) {
    Value v = Value::dict();
    v.set("folder", Value(m.folder));
    v.set("files", Value(m.files));
    v.set("bytes", Value(m.bytes));
    Value formats = Value::list();
    for (const FolderFormat& f : m.formats) {
        Value r = Value::dict();
        r.set("format", Value(f.format));
        r.set("family", Value(f.family));
        r.set("command", Value(f.command));
        r.set("files", Value(f.files));
        r.set("bytes", Value(f.bytes));
        r.set("example", Value(f.example));
        formats.push(std::move(r));
    }
    v.set("formats", std::move(formats));
    Value un = Value::list();
    for (const FolderOther& o : m.unrecognised) {
        Value r = Value::dict();
        r.set("extension", Value(o.extension));
        r.set("files", Value(o.files));
        r.set("bytes", Value(o.bytes));
        r.set("example", Value(o.example));
        un.push(std::move(r));
    }
    v.set("unrecognised", std::move(un));
    return v;
}

str _size(double n) {
    static const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    for (const char* unit : units) {
        bool last = std::string(unit) == "TB";
        if (n < 1024 || last) {
            if (std::string(unit) == "B") return py::percent("%d %s", static_cast<int64_t>(n), str(unit));
            return py::percent("%.1f %s", n, str(unit));
        }
        n /= 1024.0;
    }
    return str("");
}

// =====================================================================================================
// as text
// =====================================================================================================
// repr(value) as Python writes a dict / list / str / int / bool / None
static str py_repr(const Value& v) {
    switch (v.kind) {
        case Value::Kind::None: return str("None");
        case Value::Kind::Bool: return v.b ? str("True") : str("False");
        case Value::Kind::Int: return py::to_str(v.i);
        case Value::Kind::Str: return py::repr(v.s);
        case Value::Kind::List: {
            py::list<str> parts;
            for (const Value& x : v.items) parts.append(py_repr(x));
            return str("[") + py::join(str(", "), parts) + str("]");
        }
        case Value::Kind::Dict: {
            py::list<str> parts;
            for (size_t i = 0; i < v.keys.size(); i++) parts.append(py::repr(v.keys[i]) + str(": ") + py_repr(v.items[i]));
            return str("{") + py::join(str(", "), parts) + str("}");
        }
    }
    return str("");
}

str _cell(const str& key, const Value& value) {
    static const py::re::Pattern hex_keys = py::re::compile(
        "address|offset|image_base|code_base|^entry$|entry_point|rva|_va$|slot|checksum|^key$|flags_value|module_flags|magic|"
        "machine_id|device_id|ddk_version");
    switch (value.kind) {
        case Value::Kind::Bool: return value.b ? str("yes") : str("no");
        case Value::Kind::Int:
            if (hex_keys.search(key) && value.i >= 0) return str("0x") + py::hex(value.i).slice_from(2);
            return py::to_str(value.i);
        case Value::Kind::List: {
            py::list<str> parts;
            for (const Value& v : value.items) parts.append(_cell(key, v));
            return py::join(str(" "), parts);
        }
        case Value::Kind::None: return str("");
        case Value::Kind::Str: return value.s.replace(str("\r"), str("\\r")).replace(str("\n"), str("\\n"));
        case Value::Kind::Dict: return py_repr(value);   // str(dict), for a dict inside a list that render() does not tabulate
    }
    return str("");
}

namespace {

bool first_is_dict(const Value& v) { return v.is_list() && !v.items.empty() && v.items[0].is_dict(); }

}  // namespace

py::list<str> render(const Value& value, const str& key, int64_t indent) {
    str pad = py::repeat(str("  "), indent);
    py::list<str> lines;
    if (value.is_dict()) {
        int64_t width = 1;
        for (const str& k : value.keys) width = std::max(width, k.size());
        for (size_t n = 0; n < value.keys.size(); n++) {
            const str& k = value.keys[n];
            const Value& v = value.items[n];
            if (v.is_dict() || first_is_dict(v)) {
                lines.append(pad + k + str(":"));
                lines.extend(render(v, k, indent + 1));
            } else {
                lines.append(pad + k.ljust(width) + str("  ") + _cell(k, v));
            }
        }
        return lines;
    }
    if (first_is_dict(value)) {
        const Value& first = value.items[0];
        std::vector<str> plain, nested;
        for (size_t n = 0; n < first.keys.size(); n++) {
            const Value& x = first.items[n];
            if (!(x.is_dict() || x.is_list()) || (x.is_list() && !first_is_dict(x))) plain.push_back(first.keys[n]);
            else nested.push_back(first.keys[n]);
        }
        std::vector<std::vector<str>> rows;
        for (const Value& r : value.items) {
            std::vector<str> row;
            for (const str& k : plain) {
                const Value* c = r.get(k);
                row.push_back(_cell(k, c ? *c : Value()));
            }
            rows.push_back(row);
        }
        std::vector<int64_t> widths;
        for (size_t i = 0; i < plain.size(); i++) {
            int64_t w = plain[i].size();
            for (const auto& r : rows) w = std::max(w, r[i].size());
            widths.push_back(std::min<int64_t>(48, w));
        }
        auto joined = [&](const std::vector<str>& cells) {
            py::list<str> parts;
            for (size_t i = 0; i < cells.size(); i++) parts.append(cells[i].ljust(widths[i]));
            return pad + py::join(str("  "), parts).rstrip();
        };
        lines.append(joined(plain));
        for (size_t n = 0; n < value.items.size(); n++) {
            lines.append(joined(rows[n]));
            for (const str& k : nested) lines.extend(render(value.items[n].at(k), k, indent + 2));
        }
        return lines;
    }
    if (value.is_list()) {
        for (const Value& v : value.items) lines.append(pad + _cell(key, v));
        if (lines.empty()) lines.append(pad + str("(none)"));
        return lines;
    }
    lines.append(pad + _cell(key, value));
    return lines;
}

// ---- markdown
str markdown(const Value& info, const std::optional<Value>& xex_header) {
    py::list<str> md;
    if (info.has("formats")) {                                   // a folder
        md.append(py::percent("# %s", info.at("folder").s));
        md.append(str(""));
        md.append(py::percent("%d files, %s. Recognised:", info.at("files").i, _size(static_cast<double>(info.at("bytes").i))));
        md.append(str(""));
        md.append(str("| Format | Platform | Files | Size | Read with |"));
        md.append(str("|---|---|---:|---:|---|"));
        for (const Value& r : info.at("formats").items) {
            md.append(py::percent("| %s | %s | %d | %s | %s |", r.at("format").s, r.at("family").s, r.at("files").i,
                                  _size(static_cast<double>(r.at("bytes").i)),
                                  r.at("command").s.empty() ? str("-") : py::percent("`codebreaker %s`", r.at("command").s)));
        }
        if (!info.at("unrecognised").items.empty()) {
            md.append(str(""));
            md.append(str("Not recognised, by extension:"));
            md.append(str(""));
            md.append(str("| Extension | Files | Size | Example |"));
            md.append(str("|---|---:|---:|---|"));
            int64_t n = 0;
            for (const Value& r : info.at("unrecognised").items) {
                if (n++ >= 40) break;
                md.append(py::percent("| `%s` | %d | %s | `%s` |", r.at("extension").s, r.at("files").i,
                                      _size(static_cast<double>(r.at("bytes").i)),
                                      py::os::path::relpath(r.at("example").s, info.at("folder").s)));
            }
        }
        return py::join(str("\n"), md) + str("\n");
    }
    const Value& fmt = info.at("format");
    md.append(py::percent("# %s", info.at("name").s));
    md.append(str(""));
    md.append(py::percent("%s, %s.", fmt.truthy() ? fmt.s : str("not recognised"), _size(static_cast<double>(info.at("size").i))));
    md.append(str(""));
    if (xex_header && xex_header->truthy()) {
        md.append(str("## Xbox 360 executable"));
        md.append(str(""));
        md.append(str("| Field | Value |"));
        md.append(str("|---|---|"));
        for (size_t n = 0; n < xex_header->keys.size(); n++)
            md.append(py::percent("| %s | %s |", xex_header->keys[n], xex_header->items[n].s));
        md.append(str(""));
    }
    const Value* summary = info.get("summary");
    if (summary && summary->truthy()) {
        md.append(str("## Summary"));
        md.append(str(""));
        md.append(str("| Field | Value |"));
        md.append(str("|---|---|"));
        for (size_t n = 0; n < summary->keys.size(); n++)
            md.append(py::percent("| %s | %s |", summary->keys[n], _cell(summary->keys[n], summary->items[n])));
        md.append(str(""));
    }
    const Value* sections = info.get("sections");
    if (sections && sections->truthy()) {
        md.append(str("## Sections"));
        md.append(str(""));
        md.append(str("| Name | Address | Size | Flags |"));
        md.append(str("|---|---|---:|---|"));
        for (const Value& s : sections->items) {
            const Value* name = s.get("name");
            const Value* flags = s.get("flags");
            const Value* addr = s.get("address");
            const Value* size = s.get("size");
            md.append(py::percent("| %s | %s | %s | %s |", name ? name->s : str(""),
                                  _cell(str("address"), addr ? *addr : Value()), _cell(str("size"), size ? *size : Value()),
                                  _cell(str("flags"), flags ? *flags : Value(""))));
        }
        md.append(str(""));
    }
    const Value* imps = info.get("imports");
    if (imps && imps->truthy()) {
        md.append(str("## Imports"));
        md.append(str(""));
        md.append(str("| Module | Functions |"));
        md.append(str("|---|---:|"));
        for (const Value& i : imps->items) {
            const Value* module = i.get("module");
            const Value* count = i.get("count");
            md.append(py::percent("| %s | %d |", module ? module->s : str(""), count ? count->i : int64_t(0)));
        }
        md.append(str(""));
    }
    const Value* exp = info.get("exports");
    if (exp && exp->truthy() && exp->get_int("count")) {
        md.append(py::percent("- **Exports:** %d", exp->get_int("count")));
        md.append(str(""));
    }
    const Value* notes = info.get("notes");
    if (notes)
        for (const Value& n : notes->items) md.append(py::percent("> %s", n.s));
    return py::join(str("\n"), md) + str("\n");
}

Value xex_facts(const str& path) {
    Value out = Value::dict();
    if (!xex::is_xex(path)) return out;
    xex::Header h = xex::header(path);
    int64_t base = h.base ? *h.base : 0, entry = h.entry ? *h.entry : 0;
    out.set("image name", Value(h.name ? *h.name : str("None")));
    out.set("load address", Value(py::percent("0x%08X", base)));
    out.set("entry point", Value(py::percent("0x%08X", entry)));
    out.set("import libraries", Value(py::join(str(", "), h.imports)));
    if (!h.libraries.empty()) {
        py::list<str> libs;
        for (const auto& lv : h.libraries) libs.append(py::percent("%s %s", std::get<0>(lv), std::get<1>(lv)));
        out.set("built with", Value(py::join(str(", "), libs)));
    }
    return out;
}

// =====================================================================================================
// strings, bytes, resource bodies
// =====================================================================================================
namespace {

bool printable_byte(int64_t b) { return b == '\t' || b == '\r' || b == '\n' || (b >= 0x20 && b <= 0x7e); }

bool ci_ends_with(const std::string& t, const char* ext) {
    size_t n = std::strlen(ext);
    if (t.size() < n) return false;
    for (size_t i = 0; i < n; ++i) {
        char c = t[t.size() - n + i];
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
        if (c != ext[i]) return false;
    }
    return true;
}

bool ci_contains(const std::string& t, const char* word) {
    size_t n = std::strlen(word);
    for (size_t i = 0; i + n <= t.size(); ++i) {
        size_t k = 0;
        for (; k < n; ++k) {
            char c = t[i + k];
            if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
            if (c != word[k]) break;
        }
        if (k == n) return true;
    }
    return false;
}

bool format_spec(const std::string& t) {   // %[-+ 0-9.*#hlLIw]*[sdiouxXeEfgGcpn]
    const std::string A = "-+ 0123456789.*#hlLIw", B = "sdiouxXeEfgGcpn";
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i] != '%') continue;
        size_t j = i + 1;
        while (j < t.size() && A.find(t[j]) != std::string::npos) ++j;
        if (j < t.size() && B.find(t[j]) != std::string::npos) return true;
    }
    return false;
}

bool constant_form(const std::string& t) {   // [A-Z][A-Z0-9]*(?:_[A-Z0-9]+)+
    auto up = [](char c) { return c >= 'A' && c <= 'Z'; };
    auto upd = [&](char c) { return up(c) || (c >= '0' && c <= '9'); };
    if (t.empty() || !up(t[0])) return false;
    size_t i = 1;
    while (i < t.size() && upd(t[i])) ++i;
    int groups = 0;
    while (i < t.size() && t[i] == '_') {
        size_t j = i + 1;
        while (j < t.size() && upd(t[j])) ++j;
        if (j == i + 1) return false;
        ++groups;
        i = j;
    }
    return i == t.size() && groups >= 1;
}

}  // namespace

namespace {

// categorize() of a text that is printable ASCII (a run strings() found): str.strip() of it is the ASCII white space
const char* categorize_ascii(const std::string& text) {
    size_t a = 0, b = text.size();
    auto space = [](char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; };
    while (a < b && space(text[a])) ++a;
    while (b > a && space(text[b - 1])) --b;
    std::string t = text.substr(a, b - a);
    if (format_spec(t)) return "format";
    for (const char* e : {".dll", ".exe", ".sys", ".drv", ".vxd", ".386", ".ocx", ".cpl", ".scr", ".inf", ".ini", ".hlp"})
        if (ci_ends_with(t, e)) return "module";
    if (t.find('\\') != std::string::npos || t.find('/') != std::string::npos) return "path";
    if (constant_form(t)) return "constant";
    for (const char* w : {"error", "fail", "invalid", "cannot", "unexpected", "exception"})
        if (ci_contains(t, w)) return "error";
    return "other";
}

// the hex listing of `data` as text, the lines joined by `sep`
std::string hex_text(const py::bytes& data, int64_t base, char sep) {
    const std::string& d = data.raw();
    static const char* H = "0123456789ABCDEF";
    std::string out;
    out.reserve(d.size() / 16 * 80 + 80);
    for (size_t i = 0; i < d.size(); i += 16) {
        if (i) out += sep;
        char addr[32];
        std::snprintf(addr, sizeof addr, "%08llX  ", static_cast<unsigned long long>(base + static_cast<int64_t>(i)));
        out += addr;
        size_t n = std::min<size_t>(16, d.size() - i);
        for (size_t k = 0; k < 16; ++k) {
            if (k) out += ' ';
            if (k < n) {
                uint8_t b = static_cast<uint8_t>(d[i + k]);
                out += H[b >> 4];
                out += H[b & 15];
            } else {
                out += "  ";
            }
        }
        out += "  ";
        for (size_t k = 0; k < n; ++k) {
            uint8_t b = static_cast<uint8_t>(d[i + k]);
            out += (b >= 32 && b < 127) ? static_cast<char>(b) : '.';
        }
    }
    return out;
}

}  // namespace

str categorize(const str& text) {
    std::string t = text.strip().utf8();
    return str(categorize_ascii(t));
}

py::list<str> _hex_lines(const py::bytes& data, int64_t base) {
    py::list<str> out;
    const std::string& d = data.raw();
    for (size_t i = 0; i < d.size(); i += 16) {
        py::bytes chunk(d.substr(i, 16));
        out.append(str(hex_text(chunk, base + static_cast<int64_t>(i), '\n')));
    }
    return out;
}

namespace {

// file offset -> where that byte is loaded: an address in a 32-bit image, 'segment:offset' in a 16-bit one, None elsewhere
std::function<std::optional<Address>(int64_t)> locator(const str& path) {
    std::optional<str> fmt = detect::format_of(path);
    try {
        if (fmt && *fmt == str("PE")) {
            pe::PEFile f = pe::open_pe(path);
            struct Span { int64_t at, n, va; };
            auto spans = std::make_shared<std::vector<Span>>();
            for (const pe::Section& s : f.sections) spans->push_back({s.rawptr, s.rawsize, static_cast<int64_t>(f.image_base) + s.vaddr});
            return [spans](int64_t off) -> std::optional<Address> {
                for (const Span& s : *spans)
                    if (s.at <= off && off < s.at + s.n) return Address(s.va + off - s.at);
                return std::nullopt;
            };
        }
        if (fmt && *fmt == str("NE")) {
            ne::NEFile nef(path);
            struct Span { int64_t at, n, idx; };
            auto spans = std::make_shared<std::vector<Span>>();
            for (const ne::Segment& s : nef.segments())
                if (s.file_off) spans->push_back({s.file_off, s.length, s.idx});
            return [spans](int64_t off) -> std::optional<Address> {
                for (const Span& s : *spans)
                    if (s.at <= off && off < s.at + s.n) return Address(py::percent("%d:%04x", s.idx, off - s.at));
                return std::nullopt;
            };
        }
    } catch (const py::Exception&) {
    }
    return [](int64_t) -> std::optional<Address> { return std::nullopt; };
}

}  // namespace

Value to_value(const StringRow& r) {
    Value v = Value::dict();
    v.set("offset", Value(r.offset));
    v.set("address", r.address ? address_value(*r.address) : Value());
    v.set("kind", Value(r.kind));
    v.set("category", Value(r.category));
    v.set("text", Value(r.text));
    return v;
}

py::list<StringRow> strings(const str& path, int64_t min_len, bool wide, std::optional<int64_t> limit) {
    py::bytes bytes = py::open_rb(path).read();
    auto where = locator(path);
    struct Found {
        int64_t off;
        bool wide;
        std::string text;
    };
    std::vector<Found> found;
    const std::string& d = bytes.raw();
    int64_t n = static_cast<int64_t>(d.size());
    auto at = [&](int64_t i) { return static_cast<int64_t>(static_cast<unsigned char>(d[static_cast<size_t>(i)])); };
    for (int64_t i = 0; i < n;) {                       // [printable]{min,}
        int64_t j = i;
        while (j < n && printable_byte(at(j))) ++j;
        if (j - i >= min_len && j > i) {
            found.push_back({i, false, d.substr(static_cast<size_t>(i), static_cast<size_t>(j - i))});
            i = j;
        } else {
            i = j > i ? j : i + 1;
        }
    }
    if (wide) {
        for (int64_t i = 0; i < n;) {                   // (?:[printable]\x00){min,}
            int64_t j = i;
            while (j + 1 < n && printable_byte(at(j)) && at(j + 1) == 0) j += 2;
            if (j > i && (j - i) / 2 >= min_len) {
                std::string t;
                for (int64_t k = i; k < j; k += 2) t += d[static_cast<size_t>(k)];
                found.push_back({i, true, t});
                i = j;
            } else {
                ++i;
            }
        }
    }
    std::stable_sort(found.begin(), found.end(), [](const Found& a, const Found& b) {
        if (a.off != b.off) return a.off < b.off;
        if (a.wide != b.wide) return !a.wide;
        return a.text < b.text;
    });
    size_t count = found.size();
    if (limit) {
        int64_t lim = *limit;
        int64_t k = lim < 0 ? std::max<int64_t>(0, static_cast<int64_t>(count) + lim) : std::min<int64_t>(lim, static_cast<int64_t>(count));
        count = static_cast<size_t>(k);
    }
    py::list<StringRow> out;
    for (size_t k = 0; k < count; k++) {
        StringRow r;
        r.offset = found[k].off;
        r.address = where(found[k].off);
        r.kind = found[k].wide ? str("utf16") : str("ascii");
        r.text = str(found[k].text);
        r.category = str(categorize_ascii(found[k].text));   // (a run is printable ASCII)
        out.append(r);
    }
    return out;
}

str hexdump(const str& path, int64_t offset, int64_t length) {
    py::file fh = py::open_rb(path);
    fh.seek(std::max<int64_t>(0, offset));
    py::bytes data = fh.read(std::max<int64_t>(0, length));
    return str(hex_text(data, std::max<int64_t>(0, offset), '\n'));
}

namespace {

str version_text(const py::bytes& data, bool wide) {
    resources::Version v = resources::version(data, wide);
    py::list<str> out;
    if (v.fixed && !v.fixed->empty()) {
        py::list<int64_t> four = v.fixed->slice(2, 6);
        if (four.size() < 4) throw py::ValueError(py::percent("not enough values to unpack (expected 4, got %d)", four.size()).utf8());
        int64_t a = four[0], b = four[1], c = four[2], d = four[3];
        out.append(py::percent("FileVersion      %d.%d.%d.%d", a >> 16, a & 0xFFFF, b >> 16, b & 0xFFFF));
        out.append(py::percent("ProductVersion   %d.%d.%d.%d", c >> 16, c & 0xFFFF, d >> 16, d & 0xFFFF));
        out.append(str(""));
    }
    std::vector<std::pair<str, py::list<std::tuple<str, str>>>> langs;
    for (const auto& kv : v.strings.items()) langs.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(langs.begin(), langs.end(), [](const auto& a, const auto& b) { return a.first.u32() < b.first.u32(); });
    for (const auto& l : langs) {
        for (const auto& e : l.second) {
            str t = std::get<1>(e).strip();
            if (!t.empty()) out.append(std::get<0>(e).ljust(17) + t);
        }
    }
    str s = py::join(str("\n"), out);
    return s.empty() ? str("(no version fields could be read)") : s;
}

str string_table(const py::bytes& data, bool wide) {
    py::list<str> out;
    int64_t i = 0;
    for (const str& text : resources::strings(data, wide)) {
        if (!text.empty()) out.append(py::percent("[%2d] %s", i, text.replace(str("\r"), str(" ")).replace(str("\n"), str(" "))));
        i++;
    }
    str s = py::join(str("\n"), out);
    return s.empty() ? str("(no strings in this block)") : s;
}

// an icon or cursor resource is one image of an icon file; wrap it as a file of one
py::bytes as_icon(py::bytes image, bool cursor) {
    int64_t hot_a = 0, hot_b = 0;
    if (cursor) {
        auto hot = py::unpack_from<int64_t, int64_t>("<HH", image, 0);
        hot_a = std::get<0>(hot);
        hot_b = std::get<1>(hot);
        image = image.slice_from(4);
    }
    int64_t w, h, bits;
    if (image.slice(0, 4) == py::bytes("\x89PNG")) {
        w = h = 0;
        bits = 32;
    } else {
        auto wh = py::unpack_from<int64_t, int64_t>("<II", image, 4);
        w = std::get<0>(wh);
        h = std::get<1>(wh);
        bits = py::unpack_from1<int64_t>("<H", image, 14);
        h = py::floordiv(h, 2);                                        // picture and mask are stacked
    }
    int64_t a = cursor ? hot_a : 1, b = cursor ? hot_b : bits;
    return py::pack("<HHH", int64_t(0), int64_t(cursor ? 2 : 1), int64_t(1)) +
           py::pack("<BBBBHHII", w & 0xFF, h & 0xFF, int64_t(0), int64_t(0), a, b, image.size(), int64_t(22)) + image;
}

ResourceBody hex_body(const str& text) {
    ResourceBody b;
    b.kind = str("hex");
    b.text = text;
    return b;
}
ResourceBody text_body(const str& text) {
    ResourceBody b;
    b.kind = str("text");
    b.text = text;
    return b;
}

}  // namespace

ResourceBody resource_body(const str& path, int64_t offset, int64_t size, int64_t type_id, bool wide, int64_t limit) {
    py::bytes data;
    try {
        py::file fh = py::open_rb(path);
        fh.seek(std::max<int64_t>(0, offset));
        data = fh.read(std::max<int64_t>(0, std::min(size, limit)));
    } catch (const py::OSError& e) {
        return hex_body(py::percent("cannot be read: %s", str(std::string(e.what()))));
    }
    if (data.empty()) return hex_body(str("(empty)"));
    try {
        if (type_id == 2 && data.size() < 12) throw py::ValueError("too short to be a picture");
        if (type_id == 2) {
            ResourceBody b;
            b.kind = str("image");
            b.content_type = str("image/bmp");
            b.bytes = resources::bitmap_file(data);
            return b;
        }
        if (type_id == 1 || type_id == 3) {
            ResourceBody b;
            b.kind = str("image");
            b.content_type = str("image/x-icon");
            b.bytes = as_icon(data, type_id == 1);
            return b;
        }
        if (type_id == 16) return text_body(version_text(data, wide));
        if (type_id == 6) return text_body(string_table(data, wide));
        if (type_id == 23 || type_id == 24) return text_body(data.decode("utf-8", "replace"));
        if (type_id == 10) {
            py::bytes head = data.slice(0, 256);
            int64_t printable = 0;
            for (int64_t b : head)
                if ((9 <= b && b <= 13) || (32 <= b && b < 127)) printable++;
            if (static_cast<double>(printable) > 0.85 * static_cast<double>(head.size())) return text_body(data.decode("latin1"));
        }
    } catch (const py::Exception& e) {
        return hex_body(py::percent("could not be shown as its type (%s)\n\n%s", str(e.type()),
                                    py::join(str("\n"), _hex_lines(data.slice(0, 4096)))));
    }
    py::list<str> lines = _hex_lines(data.slice(0, 4096));
    if (data.size() > 4096) lines.append(py::percent("... %d more bytes", data.size() - 4096));
    return hex_body(py::join(str("\n"), lines));
}

// =====================================================================================================
// the command
// =====================================================================================================
namespace {

const char* MAIN_DOC =
    "python -m codebreaker info <file-or-module> [--json] [--section NAME] [--offset N] [--length N] [--min N]\n"
    "python -m codebreaker info <folder> [--json]                  every file in it, identified and tallied\n"
    "python -m codebreaker info <file-or-folder> --markdown [--out FILE.md]   a one-page report\n"
    "python -m codebreaker info <packed.exe> --unpack <out.exe>\n"
    "\n"
    "Show what a file is and holds: executables in full (NAME is one of: summary dos headers\n"
    "directories sections exports imports resources relocations tls debug certificates clr rich\n"
    "driver members strings hex, or `all`), and any other file by its format -- Xbox containers\n"
    "and assets, disc and disk images, objects, bytecode. A folder is mapped: every file in it\n"
    "identified, tallied by format. --unpack writes a packed DOS program (EXEPACK, PKLITE, ...) back\n"
    "out unpacked, as a plain MZ file that runs the same way.";

std::optional<str> option(const py::list<str>& argv, const char* name, const std::optional<str>& dflt = std::nullopt) {
    for (int64_t i = 0; i < argv.size(); i++) {
        if (argv[i] == str(name)) return i + 1 < argv.size() ? std::optional<str>(argv[i + 1]) : dflt;
    }
    return dflt;
}

bool has(const py::list<str>& argv, const char* name) { return argv.contains(str(name)); }

}  // namespace

int64_t main(const py::list<str>& argv) {
    std::vector<std::optional<str>> taken;
    for (const char* n : {"--section", "--offset", "--length", "--min", "--unpack", "--out"}) taken.push_back(option(argv, n));
    py::list<str> names;
    for (const str& a : argv) {
        if (a.startswith(str("--"))) continue;
        bool is_taken = false;
        for (const auto& t : taken)
            if (t && *t == a) is_taken = true;
        if (!is_taken) names.append(a);
    }
    if (names.empty()) {
        py::print(str(MAIN_DOC));
        return 2;
    }
    bool as_md = has(argv, "--markdown");
    std::optional<str> out_md = option(argv, "--out");

    auto emit = [&](const str& text) {
        if (out_md) {
            py::file f = py::open(*out_md, str("w"), str("utf-8"), std::nullopt, str("\n"));
            f.write(text);
            f.close();
            py::print(py::percent("wrote %s", *out_md));
        } else {
            py::print(text);
        }
    };

    if (py::os::path::isdir(names[0])) {                       // a folder: its map
        FolderMap m = folder_map(names[0], [](const str& s) { py::print(s); });
        if (has(argv, "--json")) {
            emit(dumps(to_value(m), 1));
        } else if (as_md) {
            emit(markdown(to_value(m)));
        } else {
            py::list<str> lines;
            lines.append(py::percent("%s: %d files, %s", m.folder, m.files, _size(static_cast<double>(m.bytes))));
            for (const FolderFormat& r : m.formats) {
                lines.append(py::percent("  %s %s %s files %s   %s", r.format.slice(0, 46).ljust(46), r.family.ljust(9),
                                         py::to_str(r.files).rjust(7), _size(static_cast<double>(r.bytes)).rjust(10),
                                         !r.command.empty() ? str("codebreaker ") + r.command : str("")));
            }
            if (!m.unrecognised.empty()) {
                py::list<str> parts;
                int64_t k = 0;
                for (const FolderOther& r : m.unrecognised) {
                    if (k++ >= 25) break;
                    parts.append(py::percent("%s %d", r.extension, r.files));
                }
                lines.append(str("  not recognised: ") + py::join(str(", "), parts));
            }
            emit(py::join(str("\n"), lines));
        }
        return 0;
    }
    Value xex_head = py::os::path::isfile(names[0]) ? xex_facts(names[0]) : Value::dict();
    std::optional<str> path_opt = base::corpus::retail(names[0]);
    if (!path_opt || path_opt->empty()) {
        py::print(py::percent("no file or corpus module %s", py::repr(names[0])));
        return 2;
    }
    str path = *path_opt;
    if (option(argv, "--unpack")) {
        dos::Program p = dos::read(path);
        if (!p.unpacked) {
            py::print(py::percent("%s is not packed%s", p.name,
                                  !p.packer ? str("") : py::percent(" readably (%s)", py::join(str("; "), p.notes))));
            return 1;
        }
        py::file f = py::open(*option(argv, "--unpack"), str("wb"));
        f.write(dos::unpacked_file(p));
        f.close();
        py::print(py::percent("%s (%s, unpacked %s) -> %s", p.name, p.packer ? *p.packer : str("None"), *p.unpacked,
                              *option(argv, "--unpack")));
        return 0;
    }
    std::optional<str> section = option(argv, "--section");
    bool as_json = has(argv, "--json");
    if (section && *section == str("hex")) {
        str text = hexdump(path, py::int_parse(*option(argv, "--offset", str("0")), 0),
                           py::int_parse(*option(argv, "--length", str("256")), 0));
        if (as_json) {
            Value v = Value::dict();
            v.set("hex", Value(text));
            py::print(dumps(v));
        } else {
            py::print(text);
        }
        return 0;
    }
    if (section && *section == str("strings")) {
        py::list<StringRow> rows = strings(path, py::int_parse(*option(argv, "--min", str("4")), 0));
        if (as_json) {
            Value v = Value::list();
            for (const StringRow& r : rows) v.push(to_value(r));
            py::print(dumps(v));
        } else {
            for (const StringRow& r : rows) {
                str addr = _cell(str("address"), r.address ? address_value(*r.address) : Value());
                py::print(py::percent("%08x  %s %s %s %s", r.offset, addr.ljust(10), r.kind.ljust(5), r.category.ljust(8),
                                      _cell(str(""), Value(r.text)).slice(0, 120)));
            }
        }
        return 0;
    }
    Description desc = describe(path);
    if (xex_head.truthy()) desc.extra("xex") = xex_head;
    Value info = to_value(desc);
    if (as_md) {
        emit(markdown(info, xex_head));
        return desc.ok ? 0 : 1;
    }
    auto info_part_present = [&](const str& k) { return info.has(k); };
    if (section && *section != str("all") && !info_part_present(*section)) {
        py::list<str> have;
        for (const str& k : SECTIONS) {
            const Value* v = info.get(k);
            if ((v && v->truthy()) || k == str("strings") || k == str("hex")) have.append(k);
        }
        py::print(py::percent("%s has no part %s (it has: %s)", desc.name, py::repr(*section), py::join(str(" "), have)));
        return 2;
    }
    if (as_json) {
        if (!section || *section == str("all")) py::print(dumps(info, 1));
        else py::print(dumps(info.at(*section), 1));
    } else if (section && *section != str("all")) {
        for (const str& l : render(info.at(*section), *section)) py::print(l);
    } else {
        py::print(py::percent("%s  (%s, %d bytes)", desc.path, desc.format ? *desc.format : str("not an executable"), desc.size));
        py::list<str> shortlist;
        if (!section) {
            shortlist = py::list<str>{"xex", "summary", "headers", "sections"};
        } else {
            for (size_t n = 0; n < info.keys.size(); n++) {
                const Value& v = info.items[n];
                if ((v.is_dict() || v.is_list()) && v.truthy() && info.keys[n] != str("notes")) shortlist.append(info.keys[n]);
            }
        }
        for (const str& k : shortlist) {
            const Value* v = info.get(k);
            if (v && v->truthy()) {
                py::print(str(""));
                py::print(k + str(":"));
                for (const str& l : render(*v, k, 1)) py::print(l);
            }
        }
        if (!section && desc.ok && desc.summary.has("exports")) {
            const Value& s = desc.summary;
            py::print(str(""));
            py::print(py::percent("%d exports, %d imports from %d modules, %d resources  (--section exports | imports | resources | all)",
                                  s.get_int("exports"), s.get_int("imports"), s.get_int("import_modules"), s.get_int("resources")));
        }
        for (const str& n : desc.notes) py::print(str("note: ") + n);
    }
    return desc.ok ? 0 : 1;
}

}  // namespace codebreaker::formats::inspect

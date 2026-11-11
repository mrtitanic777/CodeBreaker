// codebreaker/formats/inspect.cpp -- COMPAT shim for the old typed port of codebreaker.formats.inspect (deleted when its last
// py:: consumer is refactored). The inspector is native/cb/formats/inspect.hpp; this file converts: the old records and the old
// ordered tree (`Value`, which versions, ledger and reach still use as their JSON document) over the new typed description.
// The Value tree, its json writer and the to_value() renderings of the old records are the old port's, unchanged.
#include "codebreaker/formats/inspect.hpp"

#include <algorithm>
#include <set>
#include <string>

#include <nlohmann/json.hpp>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/inspect.hpp"
#include "codebreaker/base/corpus.hpp"
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
        case Kind::Float: return d != 0.0;
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
        case Value::Kind::Float: o += py::to_str(v.d).utf8(); return;
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

Value address_value(const Address& a) {
    if (std::holds_alternative<int64_t>(a)) return Value(std::get<int64_t>(a));
    return Value(std::get<str>(a));
}

}  // namespace

// ---- to_value
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

Value to_value(const StringRow& r) {
    Value v = Value::dict();
    v.set("offset", Value(r.offset));
    v.set("address", r.address ? address_value(*r.address) : Value());
    v.set("kind", Value(r.kind));
    v.set("category", Value(r.category));
    v.set("text", Value(r.text));
    return v;
}

// =====================================================================================================
// the conversions: the old ordered tree <-> the new document, the new records -> the old ones
// =====================================================================================================
namespace {

namespace ci = cb::formats::inspect;
using cb::compat::to_py;

Value value_of(const cb::Json& j) {
    if (j.is_null()) return Value();
    if (j.is_boolean()) return Value(j.get<bool>());
    if (j.is_number_float()) return Value(j.get<double>());
    if (j.is_number_integer()) return Value(static_cast<int64_t>(j.get<std::int64_t>()));
    if (j.is_string()) return Value(to_py(j.get<std::string>()));
    if (j.is_array()) {
        Value v = Value::list();
        for (const cb::Json& x : j) v.push(value_of(x));
        return v;
    }
    Value v = Value::dict();
    for (const auto& [k, x] : j.items()) v.set(to_py(k), value_of(x));
    return v;
}

cb::Json json_of(const Value& v) {
    switch (v.kind) {
        case Value::Kind::None: return nullptr;
        case Value::Kind::Bool: return v.b;
        case Value::Kind::Int: return v.i;
        case Value::Kind::Float: return v.d;
        case Value::Kind::Str: return v.s.utf8();
        case Value::Kind::List: {
            cb::Json a = cb::Json::array();
            for (const Value& x : v.items) a.push_back(json_of(x));
            return a;
        }
        case Value::Kind::Dict: {
            cb::Json o = cb::Json::object();
            for (size_t k = 0; k < v.keys.size(); k++) o[v.keys[k].utf8()] = json_of(v.items[k]);
            return o;
        }
    }
    return nullptr;
}

ci::Facts facts_of(const Value& v) {
    ci::Facts out;
    for (size_t k = 0; k < v.keys.size(); k++) {
        const Value& x = v.items[k];
        out.emplace_back(v.keys[k].utf8(), x.kind == Value::Kind::Str ? x.s.utf8() : x.kind == Value::Kind::Int ? std::to_string(x.i) : "");
    }
    return out;
}

Value facts_value(const ci::Facts& f) {
    Value v = Value::dict();
    for (const auto& [k, x] : f) v.set(to_py(k), Value(to_py(x)));
    return v;
}

Address old_address(const ci::Address& a) {
    if (const auto* linear = std::get_if<std::uint64_t>(&a)) return Address(static_cast<int64_t>(*linear));
    return Address(to_py(ci::address_text(a)));  // "" for none
}

detect::Detected old_detected(const cb::formats::detect::Detected& d) {
    return detect::Detected{to_py(d.format), to_py(d.detail), to_py(d.family), to_py(d.command)};
}

cb::formats::detect::Detected new_detected(const detect::Detected& d) {
    return {d.format.utf8(), d.detail.utf8(), d.family.utf8(), d.command.utf8()};
}

Section old_section(const ci::Section& s) {
    Section o;
    o.name = to_py(s.name);
    o.address = old_address(s.address);
    o.size = static_cast<int64_t>(s.size);
    o.file_offset = static_cast<int64_t>(s.file_offset);
    o.file_size = static_cast<int64_t>(s.file_size);
    for (const std::string& f : s.flags) o.flags.append(to_py(f));
    o.code = s.code;
    return o;
}

Exports old_exports(const ci::Exports& e) {
    Exports o;
    o.module = to_py(e.module);
    o.count = static_cast<int64_t>(e.symbols.size());
    if (e.ordinal_base) o.ordinal_base = static_cast<int64_t>(*e.ordinal_base);
    for (const ci::ExportSymbol& s : e.symbols) {
        ExportSymbol x;
        x.name = to_py(s.name.value_or(""));
        x.ordinal = static_cast<int64_t>(s.ordinal);
        x.hint = s.hint ? static_cast<int64_t>(*s.hint) : -1;
        x.forward = to_py(s.forward.value_or(""));
        x.address = old_address(s.address);
        x.ne = s.ne.has_value();
        if (s.ne) x.movable = s.ne->movable, x.exported = s.ne->exported;
        o.symbols.append(x);
    }
    return o;
}

ImportModule old_import_module(const ci::ImportModule& m) {
    ImportModule o;
    o.module = to_py(m.module);
    o.count = static_cast<int64_t>(m.symbols.size());
    o.delay = m.delay;
    for (const ci::ImportSymbol& s : m.symbols) {
        ImportSymbol x;
        x.name = to_py(s.name.value_or(""));
        x.ordinal = s.ordinal ? static_cast<int64_t>(*s.ordinal) : -1;
        x.hint = s.hint ? static_cast<int64_t>(*s.hint) : -1;
        if (s.slot) x.slot = static_cast<int64_t>(*s.slot);
        o.symbols.append(x);
    }
    return o;
}

Resources old_resources(const ci::Resources& r) {
    Resources o;
    o.count = static_cast<int64_t>(r.count());
    for (const ci::ResourceType& t : r.types) {
        ResourceType x;
        x.type = to_py(t.type);
        if (t.id) x.id = *t.id;
        x.count = static_cast<int64_t>(t.items.size());
        for (const ci::ResourceItem& it : t.items) {
            ResourceItem y;
            y.name = to_py(it.name.value_or(""));
            y.pe = it.from_pe;
            if (it.id) y.id = *it.id;
            y.language = it.language;
            if (it.rva) y.rva = *it.rva;
            if (it.offset) y.offset = static_cast<int64_t>(*it.offset);
            y.size = static_cast<int64_t>(it.size);
            y.type_id = it.type_id;
            y.preview = to_py(ci::preview_name(it.preview));
            y.wide = it.wide;
            x.items.append(y);
        }
        o.types.append(x);
    }
    return o;
}

Description old_description(const ci::Description& d) {
    Description o;
    o.path = to_py(d.path);
    o.name = to_py(d.name);
    o.size = static_cast<int64_t>(d.size);
    if (d.format) o.format = to_py(*d.format);
    if (d.kind) o.kind = to_py(*d.kind);
    o.ok = d.ok;
    for (const std::string& n : d.notes) o.notes.append(to_py(n));
    for (const ci::Section& s : d.sections) o.sections.append(old_section(s));
    o.exports = old_exports(d.exports);
    for (const ci::ImportModule& m : d.imports) o.imports.append(old_import_module(m));
    o.resources = old_resources(d.resources);
    // the parts whose shape differs by format are the document's own; what comes after `resources` are the extras
    const cb::Json doc = ci::to_json(d);
    o.summary = value_of(doc["summary"]);
    o.dos = value_of(doc["dos"]);
    o.headers = value_of(doc["headers"]);
    bool past_resources = false;
    for (const auto& [k, x] : doc.items()) {
        if (past_resources) o.extras.emplace_back(to_py(k), value_of(x));
        if (k == "resources") past_resources = true;
    }
    return o;
}

}  // namespace

XboxSummary& xbox_summary() {
    static XboxSummary hook;
    return hook;
}

Description describe(const str& path) {
    ci::DescribeHooks hooks;
    if (xbox_summary())
        hooks.xbox_summary = [](const std::filesystem::path& p, const cb::formats::detect::Detected& m) {
            return facts_of(xbox_summary()(to_py(cb::fs::to_text(p)), old_detected(m)));
        };
    return old_description(ci::describe(cb::fs::from_text(path.utf8()), hooks));
}

FolderMap folder_map(const str& folder, const std::function<void(const str&)>& log) {
    const ci::FolderMap m = ci::folder_map(cb::fs::from_text(folder.utf8()),
                                           log ? std::function<void(std::string_view)>([&](std::string_view s) { log(to_py(s)); })
                                               : std::function<void(std::string_view)>());
    FolderMap o;
    o.folder = to_py(m.folder);
    o.files = static_cast<int64_t>(m.files);
    o.bytes = static_cast<int64_t>(m.bytes);
    for (const ci::FolderFormat& f : m.formats)
        o.formats.append(FolderFormat{to_py(f.format), to_py(f.family), to_py(f.command), static_cast<int64_t>(f.files),
                                      static_cast<int64_t>(f.bytes), to_py(f.example)});
    for (const ci::FolderOther& f : m.unrecognised)
        o.unrecognised.append(FolderOther{to_py(f.extension), static_cast<int64_t>(f.files), static_cast<int64_t>(f.bytes), to_py(f.example)});
    return o;
}

std::optional<detect::Detected> _text_kind(const str& path, const str& name) {
    (void)name;  // the file name is the path's
    const auto k = ci::text_kind(cb::fs::from_text(path.utf8()));
    return k ? std::optional<detect::Detected>(old_detected(*k)) : std::nullopt;
}

str _size(double n) { return to_py(ci::size_text(n < 0 ? 0 : static_cast<std::uint64_t>(n))); }

str markdown(const Value& info, const std::optional<Value>& xex_header) {
    return to_py(ci::markdown(json_of(info), xex_header ? facts_of(*xex_header) : ci::Facts{}));
}

Value xex_facts(const str& path) { return facts_value(ci::xex_facts(cb::fs::from_text(path.utf8()))); }

py::list<StringRow> strings(const str& path, int64_t min_len, bool wide, std::optional<int64_t> limit) {
    ci::StringOptions o;
    o.min_length = min_len < 0 ? 0 : static_cast<std::size_t>(min_len);
    o.wide = wide;
    if (limit && *limit >= 0) o.limit = static_cast<std::size_t>(*limit);
    py::list<StringRow> out;
    for (const ci::StringRow& r : cb::compat::guard([&] { return ci::strings(cb::fs::from_text(path.utf8()), o); })) {
        StringRow x;
        x.offset = static_cast<int64_t>(r.offset);
        if (r.address) x.address = old_address(*r.address);
        x.kind = to_py(ci::string_kind_name(r.kind));
        x.category = to_py(ci::string_category_name(r.category));
        x.text = to_py(r.text);
        out.append(x);
    }
    return out;
}

str categorize(const str& text) { return to_py(ci::string_category_name(ci::categorize(text.utf8()))); }

py::list<str> _hex_lines(const py::bytes& data, int64_t base) {
    return to_py(ci::hex_lines(cb::compat::from_py(data), static_cast<std::uint64_t>(std::max<int64_t>(0, base))));
}

str hexdump(const str& path, int64_t offset, int64_t length) {
    return to_py(cb::compat::guard([&] {
        return ci::hexdump(cb::fs::from_text(path.utf8()), static_cast<std::uint64_t>(std::max<int64_t>(0, offset)),
                           static_cast<std::uint64_t>(std::max<int64_t>(0, length)));
    }));
}

ResourceBody resource_body(const str& path, int64_t offset, int64_t size, int64_t type_id, bool wide, int64_t limit) {
    const ci::ResourceBody b = ci::resource_body(cb::fs::from_text(path.utf8()), static_cast<std::uint64_t>(std::max<int64_t>(0, offset)),
                                                 static_cast<std::uint64_t>(std::max<int64_t>(0, size)), static_cast<std::int32_t>(type_id),
                                                 wide, static_cast<std::uint64_t>(std::max<int64_t>(0, limit)));
    ResourceBody o;
    switch (b.kind) {
        case ci::ResourceBody::Kind::Image:
            o.kind = str("image");
            o.content_type = to_py(b.content_type);
            o.bytes = to_py(cb::ByteView(b.bytes));
            break;
        case ci::ResourceBody::Kind::Text:
            o.kind = str("text");
            o.text = to_py(b.text);
            break;
        case ci::ResourceBody::Kind::Hex:
            o.kind = str("hex");
            o.text = to_py(b.text);
            break;
    }
    return o;
}

str _cell(const str& key, const Value& value) { return to_py(ci::cell(key.utf8(), json_of(value))); }

py::list<str> render(const Value& value, const str& key, int64_t indent) {
    return to_py(ci::render(json_of(value), key.utf8(), static_cast<int>(indent)));
}

// python -m codebreaker info ...: the new command, with the corpus resolver of the old port
int64_t main(const py::list<str>& argv) {
    ci::InfoEnvironment env;
    env.resolve = [](std::string_view name) -> std::optional<std::filesystem::path> {
        const auto p = base::corpus::retail(to_py(name));
        if (!p || p->empty()) return std::nullopt;
        return cb::fs::from_text(p->utf8());
    };
    env.hooks.xbox_summary = [](const std::filesystem::path& p, const cb::formats::detect::Detected& m) {
        return xbox_summary() ? facts_of(xbox_summary()(to_py(cb::fs::to_text(p)), old_detected(m))) : ci::Facts{};
    };
    std::vector<std::string> args = {"info"};
    for (const str& a : argv) args.push_back(a.utf8());
    return cb::compat::guard([&] { return cb::cli::Cli("codebreaker", {ci::info_command(std::move(env))}).run(args); });
}

}  // namespace codebreaker::formats::inspect

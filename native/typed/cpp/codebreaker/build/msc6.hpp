// codebreaker/build/msc6.hpp -- COMPAT shim for cb/build/msc6.hpp (docs/CPP_DESIGN.md 6.3): the old signatures in (py::str, py::dict,
// the key variant), the new functions called, the old records out; header-only, no logic. Deleted with its last old-layer
// consumer (build/module, verify/authored/match16, verify/gate16, port/pre_ansi).
//
//     msc6::available();
//     built = msc6::compile_many({{key, c_text}, ...}, header, "/AM /Gsw /Os");   // {key: (bytes, mask) or None}
//
//   * The tools are the settings (dosbox_x, msc6, msvc152, native16, native16_tools, native16_work), read when asked; DOS programs
//     run through OldDosRunner (dos_runner_adapter.hpp) and the compiled results are kept in the old cache shim's cache.
//     COMPAT(py): remove when build/dos_tools and base/cache are not old.
//   * `Key` keeps the old variant: the new compile_many numbers its sources, and the shim maps the keys to numbers and back.
//   * The old module's test hooks `_session` (one emulator session) and the regex constants have no callers in the typed tree
//     and are not carried over; `_batch_files` and `_refused` are.
//   * sdk_prototypes returns the Prototypes record (the Python's dict subclass with a `void` set); wine16 is read through the
//     old wine16 shim's folder (settings: wine16) when no lookup is given, as before.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/build/msc6.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/compat/shims/codebreaker/build/dos_runner_adapter.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "codebreaker/symbols/wine16.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::build::msc6 {

namespace nm = ::cb::build::msc6;

// the path of build/msc6.py (the Python's __file__; used by build.module to stamp its cache) -- used by: build.module
inline py::str module_file() {
    auto root = py::os::getenv(py::str("CB_CHECKOUT_ROOT"));
    return py::os::path::join(root ? *root : py::str("."), py::str("codebreaker"), py::str("build"), py::str("msc6.py"));
}

// /Gc: a function declared without a convention is pascal, as the reconstruction's own default
inline const py::list<py::str> FLAG_SETS = [] {
    py::list<py::str> out;
    for (const std::string_view f : nm::flag_sets()) out.append(cb::compat::to_py(f));
    return out;
}();   // used by: verify.authored.match16
inline constexpr int64_t BATCH = static_cast<int64_t>(nm::kBatch);   // functions to a source file when each is cut out by name

namespace shim {

inline std::optional<std::filesystem::path> setting_path(const char* key) {
    const std::optional<py::str> v = py::settings::get_opt_str(py::str(key));
    if (!v || v->empty()) return std::nullopt;
    return cb::fs::from_text(v->utf8());
}
inline nm::Tools tools() {
    nm::Tools t;
    t.dosbox_x = setting_path("dosbox_x").value_or(std::filesystem::path());
    t.msc6 = setting_path("msc6").value_or(std::filesystem::path());
    t.msvc152 = setting_path("msvc152").value_or(std::filesystem::path());
    const std::optional<py::str> native = py::settings::get_opt_str(py::str("native16"));
    if (native) {
        const std::string v = cb::text::lower(native->utf8());
        t.native16 = !(v == "off" || v == "0" || v == "false" || v == "no");
    }
    t.native16_tools = setting_path("native16_tools");
    t.native16_work = setting_path("native16_work");
    return t;
}
// nullopt: a compiler this module does not know
inline std::optional<nm::Compiler> compiler_of(const py::str& name) {
    if (name == py::str("msc6")) return nm::Compiler::Msc6;
    if (name == py::str("msvc152")) return nm::Compiler::Msvc152;
    return std::nullopt;
}
inline const OldDosRunner& runner() {
    static const OldDosRunner r;
    return r;
}
inline const std::string& ordinal_label(const std::string& s) { return s; }
inline nm::OrdinalName ordinal_names() {
    return [](std::string_view module, std::uint32_t ordinal) -> std::optional<std::string> {
        const std::optional<py::str> n = symbols::ordinals::name(cb::compat::to_py(module), static_cast<int64_t>(ordinal));
        if (!n) return std::nullopt;
        return n->utf8();
    };
}

}  // namespace shim

// (DOSBox-X, the compiler's folder); compiler: "msc6" or "msvc152"
inline std::tuple<std::optional<py::str>, std::optional<py::str>> _tools(const py::str& compiler = py::str("msc6")) {
    return {py::settings::get_opt_str(py::str("dosbox_x")), py::settings::get_opt_str(compiler)};
}

inline bool available(const py::str& compiler = py::str("msc6")) {
    const auto c = shim::compiler_of(compiler);
    return c && nm::available(shim::tools(), *c);
}

// (code, mask): the bytes of a piece of code, and 1 for every byte a fix-up record (or a hole) names
using Code = std::tuple<py::bytes, py::bytes>;

namespace shim {
inline Code old_code(const nm::Code& c) { return {cb::compat::to_py(cb::ByteView(c.code)), cb::compat::to_py(cb::ByteView(c.mask))}; }
}  // namespace shim

// (bytes, mask) of the first code segment of an OMF object file
inline Code object_code(const py::bytes& obj) {
    return cb::compat::guard([&] { return shim::old_code(nm::object_code(cb::compat::from_py(obj))); });
}

// (header, text) as Microsoft C 6 takes them.
inline std::tuple<py::str, py::str> dialect(const py::str& header, const py::str& text) {
    return {cb::compat::to_py(nm::dialect(header.utf8())), text};
}

// the functions the texts declare or define with a callee-pops (__stdcall) convention
inline py::set<py::str> pascal_names(const py::list<py::str>& texts) {
    std::vector<std::string> in;
    for (const py::str& t : texts) in.push_back(t.utf8());
    py::set<py::str> out;
    for (const std::string& n : nm::pascal_names(in)) out.add(cb::compat::to_py(n));
    return out;
}

// `text` with the arguments of every call to, and the parameters of every definition of, a function in `names` in Pascal order
inline py::str pascal_order(const py::str& text, const py::set<py::str>& names) {
    std::set<std::string> in;
    for (const py::str& n : names) in.insert(n.utf8());
    return cb::compat::to_py(nm::pascal_order(text.utf8(), in));
}

// the Windows SDK's WINDOWS.H (Visual C++ 1.52c's), or None
inline std::optional<py::str> windows_h() {
    const auto h = nm::windows_h(shim::tools());
    return h ? std::optional<py::str>(cb::compat::to_py(*h)) : std::nullopt;
}

// `header` with every import by number that is really a C function declared __cdecl, not __stdcall.
// sdk: WINDOWS.H's text (default: windows_h() or "")
inline py::str cdecl_imports(const py::str& header, const std::optional<py::str>& sdk = std::nullopt) {
    const std::optional<py::str> text = sdk ? sdk : windows_h();
    return cb::compat::to_py(nm::cdecl_imports(header.utf8(), text ? text->utf8() : std::string(), shim::ordinal_names()));
}

// (module, ordinal) -> the Export Wine's .spec has, or None (default: wine16::exports(module)[ordinal])
using ExportLookup = std::function<std::optional<symbols::wine16::Export>(const py::str&, int64_t)>;

namespace shim {
// the old lookup as the new one: the Export it answers is kept alive here for the call
inline nm::ExportLookup new_lookup(const ExportLookup& old, std::map<std::string, ::cb::symbols::wine16::Export>& keep) {
    namespace nw = ::cb::symbols::wine16;
    return [&old, &keep](std::string_view module, std::uint32_t ordinal) -> const nw::Export* {
        const py::str mod = cb::compat::to_py(module);
        const std::optional<symbols::wine16::Export> e = old ? old(mod, static_cast<int64_t>(ordinal)) : symbols::wine16::exports(mod).get(static_cast<int64_t>(ordinal));
        if (!e) return nullptr;
        nw::Export out;
        out.name = e->name.utf8();
        const std::string conv = e->convention.utf8();
        out.convention = conv == "pascal" ? nw::Convention::Pascal : conv == "cdecl" ? nw::Convention::Cdecl : conv == "varargs" ? nw::Convention::Varargs : nw::Convention::Stdcall;
        if (e->args) {
            std::vector<nw::ArgKind> kinds;
            for (const py::str& k : *e->args) {
                const std::string s = k.utf8();
                for (const nw::ArgKind kind : {nw::ArgKind::Word, nw::ArgKind::SWord, nw::ArgKind::Long, nw::ArgKind::SegPtr, nw::ArgKind::SegStr, nw::ArgKind::Ptr, nw::ArgKind::Str}) {
                    if (nw::spelling(kind) == s) kinds.push_back(kind);
                }
            }
            out.args = std::move(kinds);
        }
        out.ret16 = e->ret16;
        out.stub = e->stub;
        const std::string key = std::format("{}/{}", module, ordinal);
        return &(keep[key] = std::move(out));
    };
}
}  // namespace shim

// (header, {name: text}) with every import by number that Wine's .spec types declared with its parameters and result width
inline std::tuple<py::str, py::dict<py::str, py::str>> wine_typing(const py::str& header, const py::dict<py::str, py::str>& texts,
                                                                   const ExportLookup& exports = nullptr) {
    ::cb::OrderedMap<std::string, std::string> in;
    for (const auto& [name, text] : texts.items()) in.insert_or_assign(name.utf8(), text.utf8());
    std::map<std::string, ::cb::symbols::wine16::Export> keep;
    const nm::WineTyped typed = cb::compat::guard([&] { return nm::wine_typing(header.utf8(), in, shim::new_lookup(exports, keep)); });
    py::dict<py::str, py::str> out;
    for (const auto& [name, text] : typed.texts) out.set(cb::compat::to_py(name), cb::compat::to_py(text));
    return {cb::compat::to_py(typed.header), out};
}

// `text` with the parameters of an exported function typed as Wine's .spec has them (`export`: None -> unchanged)
inline py::str wine_definition(const py::str& text, const std::optional<symbols::wine16::Export>& export_) {
    if (!export_) return text;
    std::map<std::string, ::cb::symbols::wine16::Export> keep;
    const ExportLookup one = [&](const py::str&, int64_t) { return export_; };
    const ::cb::symbols::wine16::Export* e = shim::new_lookup(one, keep)("", 0);
    return cb::compat::to_py(nm::wine_definition(text.utf8(), e));
}

// WINDOWS.H, then `header` (whose imports by number keep their own declarations)
inline py::str sdk_header(const py::str& header, const std::optional<py::str>& sdk = std::nullopt) {
    const std::optional<py::str> text = sdk ? sdk : windows_h();
    return cb::compat::to_py(nm::sdk_header(header.utf8(), text ? std::optional<std::string>(text->utf8()) : std::nullopt));
}

// {API name: [parameter type, ...]} of every WINAPI function WINDOWS.H declares on a line, or None for one with a variable
// argument list (`params` unset); a function that returns nothing has its name in `void_`.   [Python: dict subclass _Prototypes]
struct Prototypes {
    py::dict<py::str, std::optional<py::list<py::str>>> params;
    py::set<py::str> void_;
};
inline Prototypes sdk_prototypes(const std::optional<py::str>& sdk = std::nullopt) {
    const std::optional<py::str> text = sdk ? sdk : windows_h();
    const nm::Prototypes p = nm::sdk_prototypes(text ? text->utf8() : std::string());
    Prototypes out;
    for (const auto& [name, list] : p.params) {
        std::optional<py::list<py::str>> params;
        if (list) {
            py::list<py::str> l;
            for (const std::string& s : *list) l.append(cb::compat::to_py(s));
            params = l;
        }
        out.params.set(cb::compat::to_py(name), params);
    }
    for (const std::string& n : p.no_result) out.void_.add(cb::compat::to_py(n));
    return out;
}

// `text` (in Pascal order) with each call of an import by number that the SDK declares said by its name
inline py::str sdk_calls(const py::str& text, const Prototypes& protos) {
    nm::Prototypes p;
    for (const auto& [name, params] : protos.params.items()) {
        std::optional<std::vector<std::string>> list;
        if (params) {
            std::vector<std::string> l;
            for (const py::str& s : *params) l.push_back(s.utf8());
            list = std::move(l);
        }
        p.params[name.utf8()] = std::move(list);
    }
    for (const py::str& n : protos.void_) p.no_result.insert(n.utf8());
    return cb::compat::to_py(nm::sdk_calls(text.utf8(), p, shim::ordinal_names()));
}

// `text` with each decompiler temporary (assigned once, used once, in the next statement) put back into the expression
inline py::str fold_temporaries(const py::str& text) { return cb::compat::to_py(nm::fold_temporaries(text.utf8())); }

// `text` with each pair of a function's own parameters used only as a pair made the one far pointer the source had
inline py::str join_far_parameters(const py::str& text) { return cb::compat::to_py(nm::join_far_parameters(text.utf8())); }

// `text` with the decompiler's frame structure taken apart into the locals a source declares
inline py::str split_frame(const py::str& text) { return cb::compat::to_py(nm::split_frame(text.utf8())); }

// (declarations, text): every read or write through a constant near address made one through a variable of the data segment
inline std::tuple<py::str, py::str> ds_globals(const py::str& text) {
    const nm::DsGlobals g = nm::ds_globals(text.utf8());
    return {cb::compat::to_py(g.declarations), cb::compat::to_py(g.text)};
}

// (bytes, mask) of the public function `name` in an OMF object file, or None
inline std::optional<Code> function_code(const py::bytes& obj, const py::str& name) {
    const auto c = cb::compat::guard([&] { return nm::function_code(cb::compat::from_py(obj), name.utf8()); });
    return c ? std::optional<Code>(shim::old_code(*c)) : std::nullopt;
}

// (the compiler's folder, a work folder), both by short paths, when `compiler` runs on Windows itself -- else None  [tests]
inline std::optional<std::tuple<py::str, py::str>> _native(const py::str& compiler) {
    const auto c = shim::compiler_of(compiler);
    if (!c) return std::nullopt;
    const auto n = nm::native_paths(shim::tools(), *c);
    if (!n) return std::nullopt;
    return std::make_tuple(cb::compat::to_py(cb::fs::to_text(n->tools)), cb::compat::to_py(cb::fs::to_text(n->work)));
}

// What a caller numbers its sources by (see the header notes).
using Key = std::variant<py::str, int64_t, std::tuple<std::variant<py::str, int64_t>, int64_t>>;

// Compile each of `sources` {key: C text} with `header` before it. -> {key: (bytes, mask) or None}
// names: {key: function name}; together: which keys share a source file; batch: keys to a file otherwise.
inline py::dict<Key, std::optional<Code>> compile_many(const py::dict<Key, py::str>& sources, const py::str& header,
                                                       const py::str& flags, const std::optional<py::str>& work = std::nullopt,
                                                       int64_t timeout = 900, const py::str& compiler = py::str("msc6"),
                                                       const std::optional<py::dict<Key, py::str>>& names = std::nullopt,
                                                       int64_t batch = BATCH,
                                                       const std::optional<py::list<py::list<Key>>>& together = std::nullopt) {
    const auto which = shim::compiler_of(compiler);
    if (!which) throw py::RuntimeError(cb::compat::to_py(compiler.utf8() + " is not set up (settings.json: dosbox_x, " + compiler.utf8() + ")"));
    std::vector<Key> keys;
    std::map<Key, std::size_t> index;
    std::vector<nm::Source> in;
    for (const auto& [key, text] : sources.items()) {
        index[key] = keys.size();
        keys.push_back(key);
        nm::Source s;
        s.text = text.utf8();
        if (names) {
            if (const auto n = names->get(key)) s.function = n->utf8();
        }
        in.push_back(std::move(s));
    }
    nm::CompileOptions options;
    options.compiler = *which;
    options.timeout = std::chrono::seconds(timeout);
    options.batch = static_cast<std::size_t>(batch);
    options.work = cb::fs::from_text(work ? work->utf8() : py::os::path::join(py::settings::get_str(py::str("state")), compiler, py::str("work")).utf8());
    if (together) {
        for (const py::list<Key>& g : *together) {
            std::vector<std::size_t> group;
            for (const Key& k : g) {
                const auto it = index.find(k);
                if (it != index.end()) group.push_back(it->second);
            }
            options.together.push_back(std::move(group));
        }
    }
    const nm::Context ctx{shim::tools(), shim::runner(), &base::cache::shim::any_cache()};
    const std::vector<std::optional<nm::Code>> built = cb::compat::guard([&] { return nm::compile_many(ctx, in, header.utf8(), flags.utf8(), options); });
    py::dict<Key, std::optional<Code>> out;
    for (std::size_t i = 0; i < keys.size(); ++i) out.set(keys[i], built[i] ? std::optional<Code>(shim::old_code(*built[i])) : std::nullopt);
    return out;
}

// ([(stem, text)], [[(first line, key)]]): each group's functions in one file, and the line each begins on.
// todo: {key: (cache key, text)}                                                         [tests]
inline std::tuple<py::list<std::tuple<py::str, py::str>>, py::list<py::list<std::tuple<int64_t, Key>>>>
_batch_files(const py::list<py::list<Key>>& groups, const py::dict<Key, std::tuple<py::str, py::str>>& todo, const py::str& prefix) {
    std::vector<Key> keys;
    std::map<Key, std::size_t> index;
    std::map<std::size_t, std::string> texts;
    for (const auto& [key, entry] : todo.items()) {
        index[key] = keys.size();
        texts[keys.size()] = std::get<1>(entry).utf8();
        keys.push_back(key);
    }
    std::vector<std::vector<std::size_t>> in;
    for (const py::list<Key>& g : groups) {
        std::vector<std::size_t> group;
        for (const Key& k : g) group.push_back(index.at(k));
        in.push_back(std::move(group));
    }
    const nm::BatchFiles b = nm::batch_files(in, texts, prefix.utf8());
    py::list<std::tuple<py::str, py::str>> files;
    for (const auto& f : b.files) files.append(std::make_tuple(cb::compat::to_py(f.stem), cb::compat::to_py(f.text)));
    py::list<py::list<std::tuple<int64_t, Key>>> starts;
    for (const auto& file : b.starts) {
        py::list<std::tuple<int64_t, Key>> here;
        for (const auto& [line, source] : file) here.append(std::make_tuple(static_cast<int64_t>(line), keys[source]));
        starts.append(here);
    }
    return {files, starts};
}

// the keys of the functions a refused file's errors are in -- [] when an error is not in the file itself  [tests]
inline py::list<Key> _refused(const py::str& path, const py::list<std::tuple<int64_t, Key>>& starts) {
    py::list<Key> out;
    std::optional<::cb::Bytes> said;
    std::error_code ec;
    const auto file = cb::fs::from_text(path.utf8());
    if (!std::filesystem::is_regular_file(file, ec)) return out;
    said = cb::fs::read_file(file);
    std::vector<Key> keys;
    std::vector<std::pair<std::size_t, std::size_t>> in;
    for (const auto& [line, key] : starts) {
        in.emplace_back(static_cast<std::size_t>(line), keys.size());
        keys.push_back(key);
    }
    for (const std::size_t k : nm::refused(cb::text::from_latin1(*said), in)) out.append(keys[k]);
    return out;
}

}  // namespace codebreaker::build::msc6

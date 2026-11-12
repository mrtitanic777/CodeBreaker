// codebreaker/translate/l2.hpp -- COMPAT SHIM over cb/translate/l2.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/l2.{hpp,cpp}: L2 documents over the typed tree, and the module model. This header keeps
// the old py:: API for the ports that still hold the old IR tree (py::ref nodes) and L2 documents as ir::Dict:
//
//   * the tree itself -- type_json / type_from / sym_json / sym_from / to_json / from_json over old nodes -- is the old
//     codec, moved to cb/compat/old_l2_tree.hpp (the new module cannot read an old tree);
//   * the module model (module, function, fragment, comment, units, texts, Made, Assembly) forwards to the new module through
//     the document's JSON form;
//   * validate / schema ask the L2 core library (native_l2) with the schema this file names.
//
// No logic of its own beyond that: convert, forward, and turn a failure into the exception the old code expects.
#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "cb/compat/old_l2_tree.hpp"
#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/translate/l2.hpp"
#include "cb/translate/native_l2.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

// the L2 core library's two functions (native/l2/src/api.cc), linked into the program that includes this
extern "C" {
int l2_call(const char* fn, const char* in, size_t inlen, char** out, size_t* outlen);
void l2_free(char* p);
}

namespace codebreaker::translate::l2 {

namespace newl2 = cb::translate::l2;
namespace nir = cb::translate::ir;

// jsonschema.ValidationError, as validate() raises it
struct ValidationError : py::Exception {
    ValidationError() : py::Exception() {}
    explicit ValidationError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "ValidationError"; }
};

// The path of l2.schema.json: <repo>/codebreaker/translate/l2.schema.json, found from this file's place in the tree
// (<repo>/native/typed/cpp/codebreaker/translate/l2.hpp). A variable: a caller with the schema elsewhere sets it.
inline py::str& schema_path_variable() {
    static py::str path = [] {
        std::string p = __FILE__;
        for (int up = 0; up < 6; ++up) {
            const std::size_t cut = p.find_last_of("/\\");
            if (cut == std::string::npos) break;
            p = p.substr(0, cut);
        }
        return py::bytes(p + "/codebreaker/translate/l2.schema.json").decode("utf-8");
    }();
    return path;
}
inline py::str& SCHEMA = schema_path_variable();

namespace shim_detail {

inline cb::translate::native_l2::Library& library() {
    static cb::translate::native_l2::Library lib(&l2_call, &l2_free);
    return lib;
}

inline cb::Json doc_json(const Dict& d) { return cb::compat::to_json_value(d); }
inline Dict doc_dict(const cb::Json& j) {
    const Val v = cb::compat::from_json_value(j);
    return std::get<Dict>(v.v);
}

// an old origin dict ({module, va} ...) as the new tree's
inline std::optional<nir::Origin> origin_of(const std::optional<Dict>& origin) {
    if (!origin) return std::nullopt;
    cb::Json holder = cb::Json::object();
    holder["node"] = "Empty";
    holder["origin"] = doc_json(*origin);
    cb::Arena arena;
    const nir::Node* n = newl2::from_json(arena, holder);
    if (!n->origin) return std::nullopt;
    return *n->origin;
}

}  // namespace shim_detail

// ------------------------------------------------------------------------------------ the module model
// A Module document. -> {"node": "Module", "units": [...]}
inline Dict module(const py::list<Val>& units = py::list<Val>(), const std::optional<py::str>& header = std::nullopt,
                   const std::optional<Dict>& origin = std::nullopt) {
    return cb::compat::guard([&] {
        cb::Arena arena;
        std::vector<nir::Node*> nodes;
        for (const Val& u : units) nodes.push_back(newl2::from_json(arena, cb::compat::to_json_value(u)));
        nir::Node* m = newl2::module_node(arena, nodes, cb::compat::from_py(header));
        if (const auto o = shim_detail::origin_of(origin)) m->origin = std::make_shared<const nir::Origin>(*o);
        return shim_detail::doc_dict(newl2::to_json(*m));
    });
}

// A function of a module: a MethodDecl (kind "function"), its source as `text` while it is not a tree.
// `fields` are the Python's **fields (they override what is set before them).
inline Dict function(const py::str& name, const std::optional<py::str>& text = std::nullopt, const std::optional<Dict>& origin = std::nullopt,
                     const std::optional<py::list<py::str>>& comments = std::nullopt, const Dict& fields = Dict()) {
    return cb::compat::guard([&] {
        cb::Arena arena;
        newl2::FunctionSpec spec;
        spec.name = name.utf8();
        spec.text = cb::compat::from_py(text);
        spec.origin = shim_detail::origin_of(origin);
        if (comments) spec.comments = cb::compat::from_py(*comments);
        Dict d = shim_detail::doc_dict(newl2::to_json(*newl2::function_node(arena, spec)));
        for (const auto& kv : fields.items()) d.set(std::get<0>(kv), std::get<1>(kv));
        return d;
    });
}

// What stays machine code: a MachineFragment (L2_BINARY_TYPES.md section 7).
inline Dict fragment(const std::optional<py::str>& asm_ = std::nullopt, const std::optional<py::str>& name = std::nullopt,
                     const std::optional<py::str>& syntax = std::nullopt, const std::optional<Dict>& origin = std::nullopt,
                     const std::optional<py::str>& reason = std::nullopt, const std::optional<py::str>& why = std::nullopt,
                     const std::optional<int64_t>& length = std::nullopt, const std::optional<py::str>& within = std::nullopt) {
    return cb::compat::guard([&] {
        cb::Arena arena;
        nir::MachineFragment f;
        f.assembly = cb::compat::from_py(asm_);
        f.name = cb::compat::from_py(name);
        f.syntax = cb::compat::from_py(syntax);
        f.reason = cb::compat::from_py(reason);
        f.why = cb::compat::from_py(why);
        if (length) f.length = cb::compat::int_from_py<std::uint64_t>(*length);
        f.within = cb::compat::from_py(within);
        return shim_detail::doc_dict(newl2::to_json(*newl2::fragment_node(arena, std::move(f), shim_detail::origin_of(origin))));
    });
}

inline Dict comment(const py::str& text) {
    cb::Arena arena;
    return shim_detail::doc_dict(newl2::to_json(*newl2::comment_node(arena, text.utf8())));
}

// The units of a Module document (those of one kind, given `node`).
inline py::list<Dict> units(const Dict& doc, const std::optional<py::str>& node = std::nullopt) {
    return cb::compat::guard([&] {
        cb::Arena arena;
        const nir::Node* m = newl2::from_json(arena, shim_detail::doc_json(doc));
        py::list<Dict> out;
        for (const nir::Node* u : newl2::units(m)) {
            if (node && nir::kind_name(*u) != node->utf8()) continue;
            out.append(shim_detail::doc_dict(newl2::to_json(*u)));
        }
        return out;
    });
}

// {name: text} of a module's functions whose source is text, in their order.
inline py::dict<py::str, py::str> texts(const Dict& doc) {
    return cb::compat::guard([&] {
        cb::Arena arena;
        const nir::Node* m = newl2::from_json(arena, shim_detail::doc_json(doc));
        return cb::compat::to_py(newl2::texts(m));
    });
}

// The parsed l2.schema.json
inline Val schema() {
    return cb::compat::guard([&] { return cb::compat::from_json_value(cb::json::load(cb::fs::from_text(SCHEMA.utf8()))); });
}

// Raise ValidationError if `doc` is not L2. (The L2 core library decides: it is written to decide as l2.validate does.)
inline void validate(const Dict& doc) {
    const std::string text = cb::compat::guard([&] { return cb::fs::read_text(cb::fs::from_text(SCHEMA.utf8())); });
    const auto verdict = cb::compat::guard([&] { return shim_detail::library().validate(shim_detail::doc_json(doc), text); });
    if (!verdict.valid) throw ValidationError(verdict.why);
}

// A module made compilable for a lane's build, as the one module model (decompile/c_text: compilable16, compilable32,
// compilable_powerpc): the functions with their C as text and their places, in the module's order, the header the Module's --
// and, beside it, what the lane keeps of the module as attributes (`facts`; the lanes' own structs derived from this keep them
// as typed members).
struct Made {
    py::str name;
    Dict facts;         // what the Python keeps as attributes of the object beside name and doc
    cb::translate::l2::Made impl;

    Made(const py::str& name_ = py::str(""), const py::str& header = py::str(""),
         const std::optional<py::dict<py::str, py::str>>& texts = std::nullopt,
         const std::optional<py::list<py::str>>& order = std::nullopt,
         const std::optional<py::dict<int64_t, py::str>>& by_va = std::nullopt, const Dict& facts_ = Dict())
        : name(name_), facts(facts_) {
        const auto new_texts = texts ? cb::compat::from_py(*texts) : cb::OrderedMap<std::string, std::string>();
        const std::vector<std::string> new_order = order ? cb::compat::from_py(*order) : std::vector<std::string>();
        cb::OrderedMap<std::uint64_t, std::string> places;
        if (by_va) {
            for (const auto& kv : by_va->items()) {
                places.insert_or_assign(cb::compat::int_from_py<std::uint64_t>(std::get<0>(kv)), std::get<1>(kv).utf8());
            }
        }
        impl = cb::translate::l2::Made::of(name_.utf8(), header.utf8(), new_texts, new_order, places);
    }

    void add(const py::str& name_, const std::optional<py::str>& text, const std::optional<int64_t>& va = std::nullopt) {
        std::optional<std::uint64_t> place;
        if (va) place = cb::compat::int_from_py<std::uint64_t>(*va);
        impl.add(name_.utf8(), cb::compat::from_py(text), place);
    }

    py::list<Dict> _functions() const {
        return cb::compat::guard([&] {
            cb::Arena arena;
            const nir::Node* m = impl.document(arena);
            py::list<Dict> out;
            for (const nir::Node* u : newl2::units_of<nir::MethodDecl>(m)) out.append(shim_detail::doc_dict(newl2::to_json(*u)));
            return out;
        });
    }

    // `made.texts`: {name: C text}, in the module's order (a copy; write with set_text / update_texts)
    py::dict<py::str, py::str> texts() const { return cb::compat::to_py(impl.texts()); }
    // `made.texts = new`
    void set_texts(const py::dict<py::str, py::str>& new_texts) { impl.update_texts(cb::compat::from_py(new_texts)); }
    // `made.texts[name] = text`
    void set_text(const py::str& name_, const py::str& text) { impl.set_text(name_.utf8(), text.utf8()); }
    // `made.texts.update(new)`
    void update_texts(const py::dict<py::str, py::str>& new_texts) { set_texts(new_texts); }

    py::str header() const { return cb::compat::to_py(impl.header()); }
    void set_header(const py::str& text) { impl.set_header(text.utf8()); }

    py::list<py::str> order() const { return cb::compat::to_py(impl.order()); }
    py::dict<int64_t, py::str> by_va() const { return cb::compat::to_py(impl.by_va()); }
    // {address: (name, text)}
    py::dict<int64_t, std::tuple<py::str, std::optional<py::str>>> functions() const {
        py::dict<int64_t, std::tuple<py::str, std::optional<py::str>>> out;
        for (const auto& [va, f] : impl.functions()) out.set(cb::compat::to_py(va), std::make_tuple(cb::compat::to_py(f.first), cb::compat::to_py(f.second)));
        return out;
    }
};

// A module being written as assembly language, built as its L2 Module as it goes.
struct Assembly : py::list<py::str> {
    std::optional<py::str> syntax;
    std::optional<py::str> module;
    // (start line, name, reason, origin, length) of each part
    std::vector<std::tuple<int64_t, py::str, py::str, std::optional<Dict>, std::optional<int64_t>>> _parts;

    Assembly(const py::list<py::str>& lines = py::list<py::str>(), const std::optional<py::str>& syntax_ = std::nullopt,
             const std::optional<py::str>& module_ = std::nullopt, const py::str& name = py::str("head"), const py::str& reason = py::str("head"))
        : py::list<py::str>(lines), syntax(syntax_), module(module_) {
        _parts.emplace_back(0, name, reason, std::nullopt, std::nullopt);
    }

    void part(const py::str& name, const py::str& reason = py::str("section"), const std::optional<Dict>& origin = std::nullopt,
              const std::optional<int64_t>& length = std::nullopt) {
        _parts.emplace_back(static_cast<int64_t>(this->size()), name, reason, origin, length);
    }

    Dict document() const {
        return cb::compat::guard([&] {
            const std::vector<std::string> lines = cb::compat::from_py(static_cast<const py::list<py::str>&>(*this));
            newl2::Assembly out({}, cb::compat::from_py(syntax), cb::compat::from_py(module), std::get<1>(_parts[0]).utf8(), std::get<2>(_parts[0]).utf8());
            for (std::size_t k = 0; k < _parts.size(); ++k) {
                const auto& [start, name, reason, origin, length] = _parts[k];
                if (k > 0) {
                    std::optional<std::uint64_t> n;
                    if (length) n = cb::compat::int_from_py<std::uint64_t>(*length);
                    out.part(name.utf8(), reason.utf8(), shim_detail::origin_of(origin), n);
                }
                const std::size_t from = static_cast<std::size_t>(start);
                const std::size_t to = k + 1 < _parts.size() ? static_cast<std::size_t>(std::get<0>(_parts[k + 1])) : lines.size();
                for (std::size_t i = from; i < to && i < lines.size(); ++i) out.append(lines[i]);
            }
            cb::Arena arena;
            return shim_detail::doc_dict(newl2::to_json(*out.document(arena)));
        });
    }
};

}  // namespace codebreaker::translate::l2

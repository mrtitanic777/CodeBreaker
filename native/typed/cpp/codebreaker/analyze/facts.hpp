// codebreaker/analyze/facts.hpp -- COMPAT SHIM over cb/analyze/facts.hpp (docs/CPP_DESIGN.md 6.3), for the old
// codebreaker/analyze/facts.py port. Deleted with its last py:: consumer (none today: the analyses that write facts
// are the not-yet-refactored classes/observe/structures/dispatch).
//
// The fact table is native/cb/analyze/facts.hpp. The old API is kept as it was: a FactStore opened on a path (the function
// store's own file when none), put(module, analysis, rows), get(...), tally(...), key(), and dumps/loads of a fact's
// JSON value, which is the old tagged `Json` here and a knowledge::Value there (conversion only). The old `_plain`
// hook (what json.dumps's `default=` did for a set or an object) is not carried: a fact's value is built as a
// document by the analysis that makes it.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/analyze/facts.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"

namespace codebreaker::analyze::facts {

using Json = knowledge::sources::Json;

namespace detail {

// The old tagged Json as a document, and back.
inline ::cb::knowledge::Value to_value(const Json& j) {
    using V = ::cb::knowledge::Value;
    switch (j.kind) {
        case Json::None: return V();
        case Json::Bool: return V(j.b);
        case Json::Int: return V(static_cast<std::int64_t>(j.i));
        case Json::Float: return V(j.d);
        case Json::Str: return V(j.s.utf8());
        case Json::List: {
            V::List items;
            for (const Json& x : j.list) items.push_back(to_value(x));
            return V(std::move(items));
        }
        case Json::Dict: {
            V::Map items;
            for (const auto& kv : j.dict.items()) items.insert_or_assign(std::get<0>(kv).utf8(), to_value(std::get<1>(kv)));
            return V(std::move(items));
        }
    }
    return V();
}

inline Json from_value(const ::cb::knowledge::Value& v) {
    using K = ::cb::knowledge::Value::Kind;
    Json j;
    switch (v.kind()) {
        case K::Null: break;
        case K::Bool: j.kind = Json::Bool; j.b = *v.boolean(); break;
        case K::Int: j.kind = Json::Int; j.i = *v.integer(); break;
        case K::Double: j.kind = Json::Float; j.d = *v.number(); break;
        case K::Text: j.kind = Json::Str; j.s = py::str(*v.text()); break;
        case K::List:
            j.kind = Json::List;
            for (const auto& x : *v.list()) j.list.push_back(from_value(x));
            break;
        case K::Map:
            j.kind = Json::Dict;
            for (const auto& kv : *v.map()) j.dict.set(py::str(kv.first), from_value(kv.second));
            break;
    }
    return j;
}

}  // namespace detail

// The function store's key for an address of an image kind ("ne", "pe", "le"); '' for None.
inline py::str key(const py::str& kind, const std::optional<int64_t>& va) {
    return py::str(::cb::analyze::facts::key(kind == py::str("ne") ? ::cb::analyze::ImageKind::Ne : ::cb::analyze::ImageKind::Pe, va));
}
inline py::str key(const image::Image& img, const std::optional<int64_t>& va) { return key(img.kind, va); }

// (addr key, kind, value): one row of what an analysis has to say about a module
using Row = std::tuple<py::str, py::str, Json>;

// {analysis, addr, kind, value} of one stored fact
struct Fact {
    py::str analysis;
    py::str addr;
    py::str kind;
    Json value;
};

class FactStore {
  public:
    explicit FactStore(const std::optional<py::str>& path = std::nullopt)
        : impl_(std::make_shared<::cb::analyze::facts::FactStore>(::cb::analyze::facts::FactStore::open(
              path && !path->empty() ? std::filesystem::path(path->utf8())
                                     : ::cb::analyze::facts::default_path(::cb::config::current())))),
          path(py::str(impl_->path().generic_string())) {}
    FactStore(const FactStore&) = delete;
    FactStore& operator=(const FactStore&) = delete;

    void close() { impl_.reset(); }

    void put(const py::str& module, const py::str& analysis, const py::list<Row>& rows) {
        ::cb::compat::guard([&] {
            std::vector<::cb::analyze::facts::Row> out;
            for (const Row& r : rows) out.push_back({std::get<0>(r).utf8(), std::get<1>(r).utf8(), detail::to_value(std::get<2>(r))});
            impl_->put(module.utf8(), analysis.utf8(), out);
        });
    }

    py::list<Fact> get(const py::str& module, const std::optional<py::str>& analysis = std::nullopt,
                       const std::optional<py::str>& addr = std::nullopt, const std::optional<py::str>& kind = std::nullopt) {
        return ::cb::compat::guard([&] {
            const auto narrow = [](const std::optional<py::str>& s) { return s ? std::optional<std::string>(s->utf8()) : std::nullopt; };
            py::list<Fact> out;
            for (auto& f : impl_->get(module.utf8(), {.analysis = narrow(analysis), .addr = narrow(addr), .kind = narrow(kind)}))
                out.append(Fact{py::str(f.analysis), py::str(f.addr), py::str(f.kind), detail::from_value(f.value)});
            return out;
        });
    }

    py::dict<py::str, py::dict<py::str, int64_t>> tally(const py::str& module) {
        return ::cb::compat::guard([&] {
            py::dict<py::str, py::dict<py::str, int64_t>> out;
            for (const auto& [analysis, kinds] : impl_->tally(module.utf8())) {
                py::dict<py::str, int64_t> row;
                for (const auto& [kind, n] : kinds) row.set(py::str(kind), n);
                out.set(py::str(analysis), row);
            }
            return out;
        });
    }

  private:
    std::shared_ptr<::cb::analyze::facts::FactStore> impl_;

  public:
    py::str path;
};

// json.dumps(value, sort_keys=True) / json.loads(text)
inline py::str dumps(const Json& value) { return py::str(::cb::analyze::facts::to_text(detail::to_value(value))); }
inline Json loads(const py::str& text) {
    return ::cb::compat::guard([&] { return detail::from_value(::cb::analyze::facts::from_text(text.utf8())); });
}

inline std::shared_ptr<FactStore> open(const std::optional<py::str>& path = std::nullopt) { return std::make_shared<FactStore>(path); }

}  // namespace codebreaker::analyze::facts

// codebreaker/knowledge/show.hpp -- SHIM over cb/knowledge/show.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
//     codebreaker knowledge [--load] [<platform> [<kind> [<key>]]]
//
// The command itself is the new module's (a pure function of what a `Base` answers). This shim is the Base over the
// process's knowledge base (store::open(), knowledge::table): it converts the old base's cells and values, and the
// old authored-source Json values to the new knowledge::Value.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>

#include <nlohmann/json.hpp>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/json.hpp"
#include "cb/knowledge/show.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "py.hpp"

namespace codebreaker::knowledge::show {

namespace shim_ {

namespace cbs = cb::knowledge::show;
using engine::native_engine::Cell;
using engine::native_engine::Cells;

inline cb::knowledge::Value value_of(const sources::Json& j) {
    using Json = sources::Json;
    switch (j.kind) {
        case Json::None: return cb::knowledge::Value();
        case Json::Bool: return cb::knowledge::Value(j.b);
        case Json::Int: return cb::knowledge::Value(j.i);
        case Json::Float: return cb::knowledge::Value(j.d);
        case Json::Str: return cb::knowledge::Value(cb::compat::from_py(j.s));
        case Json::List: {
            cb::knowledge::Value::List items;
            for (const Json& x : j.list) items.push_back(value_of(x));
            return cb::knowledge::Value(std::move(items));
        }
        case Json::Dict: {
            cb::knowledge::Value::Map items;
            for (const auto& kv : j.dict.items()) items.insert_or_assign(cb::compat::from_py(std::get<0>(kv)), value_of(std::get<1>(kv)));
            return cb::knowledge::Value(std::move(items));
        }
    }
    return cb::knowledge::Value();
}

inline cbs::Stored stored_of(const engine::facts::Value& v) {
    cbs::Stored out;
    if (std::holds_alternative<sources::Json>(v)) {
        out.kind = cbs::Stored::Kind::Structured;
        out.structured = value_of(std::get<sources::Json>(v));
    } else if (std::holds_alternative<py::str>(v)) {
        out.kind = cbs::Stored::Kind::Text;
        out.text = cb::compat::from_py(std::get<py::str>(v));
    } else {
        out.kind = cbs::Stored::Kind::Bytes;
        out.bytes = cb::compat::from_py(std::get<py::bytes>(v));
    }
    return out;
}

// "%s" of a cell (None, a number, text)
inline std::string cell_text(const Cell& c) {
    if (std::holds_alternative<std::monostate>(c)) return "None";
    if (std::holds_alternative<int64_t>(c)) return std::to_string(std::get<int64_t>(c));
    if (std::holds_alternative<double>(c)) return cb::compat::from_py(py::to_str(std::get<double>(c)));
    if (std::holds_alternative<py::str>(c)) return cb::compat::from_py(std::get<py::str>(c));
    return cb::compat::from_py(py::repr(std::get<py::bytes>(c)));
}

inline std::int64_t cell_int(const Cell& c) {
    if (std::holds_alternative<int64_t>(c)) return std::get<int64_t>(c);
    if (std::holds_alternative<double>(c)) return static_cast<std::int64_t>(std::get<double>(c));
    return 0;
}

inline double cell_double(const Cell& c) {
    if (std::holds_alternative<int64_t>(c)) return static_cast<double>(std::get<int64_t>(c));
    if (std::holds_alternative<double>(c)) return std::get<double>(c);
    return 0.0;
}

// The knowledge base of this process, as the command asks of it.
class OldBase : public cbs::Base {
  public:
    OldBase() : kb_(store::open()) {}

    std::string path() const override { return cb::compat::from_py(kb_->path); }
    std::string platform(std::string_view name) const override {
        return cb::compat::from_py(knowledge::platform(cb::compat::to_py(name)));
    }
    void load_authored() override {
        for (const auto& pk : sources::AUTHORED().keys()) sources::authored(std::get<0>(pk), std::get<1>(pk), kb_);
    }
    std::vector<cbs::KindRow> kinds(std::optional<std::string_view> platform) override {
        std::optional<py::str> want;
        if (platform) want = cb::compat::to_py(*platform);
        std::vector<cbs::KindRow> out;
        for (const Cells& row : kb_->kinds(want)) {
            cbs::KindRow r;
            r.platform = cell_text(row[0]);
            r.kind = cell_text(row[1]);
            r.keys = cell_int(row[2]);
            r.bytes = static_cast<std::int64_t>(cell_double(row[3]));
            const cb::Json parsed = cb::json::parse(std::get<py::str>(row[4]).utf8());   // {origin: count}
            for (const auto& [origin, count] : parsed.items()) r.origins[origin] = count.get<std::int64_t>();
            r.source = std::holds_alternative<py::str>(row[5]) ? cell_text(row[5]) : std::string();
            out.push_back(std::move(r));
        }
        return out;
    }
    cbs::SharedBodies shared_bodies() override {
        const Cells c = kb_->shared_bodies();
        return {cell_int(c[0]), cell_int(c[1]), cell_int(c[2])};
    }
    std::vector<std::string> first_keys(std::string_view platform, std::string_view kind, std::size_t limit, std::size_t& total) override {
        const py::dict<py::str, engine::facts::Value> t = knowledge::table(cb::compat::to_py(platform), cb::compat::to_py(kind));
        total = static_cast<std::size_t>(t.size());
        std::vector<std::string> keys;
        for (const py::str& key : t.keys()) {
            if (keys.size() >= limit) break;
            keys.push_back(cb::compat::from_py(key));
        }
        return keys;
    }
    std::optional<cbs::Stored> value(std::string_view platform, std::string_view kind, std::string_view key) override {
        const py::str p = cb::compat::to_py(platform), k = cb::compat::to_py(kind), kk = cb::compat::to_py(key);
        const py::dict<py::str, engine::facts::Value> t = knowledge::table(p, k);
        const std::optional<engine::facts::Value> fallback = kb_->get(p, k, kk);   // (the default is evaluated first, as the call's argument)
        if (t.contains(kk)) return stored_of(t.at(kk));
        if (fallback) return stored_of(*fallback);
        return std::nullopt;
    }
    std::optional<cbs::Provenance> about(std::string_view platform, std::string_view kind, std::string_view key) override {
        const std::optional<Cells> a = kb_->about(cb::compat::to_py(platform), cb::compat::to_py(kind), cb::compat::to_py(key));
        if (!a) return std::nullopt;
        cbs::Provenance p;
        p.source = cell_text((*a)[0]);
        p.origin = cell_text((*a)[1]);
        if (!std::holds_alternative<std::monostate>((*a)[2])) p.made_at = cell_double((*a)[2]);
        p.hash = cell_text((*a)[3]);
        return p;
    }

  private:
    std::shared_ptr<engine::native_engine::KnowledgeBase> kb_;
};

}  // namespace shim_

// json.dumps(value, indent=1, default=repr)
inline py::str dumps(const engine::facts::Value& value) {
    return cb::compat::guard([&] { return cb::compat::to_py(cb::knowledge::show::dump(shim_::stored_of(value))); });
}

inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        shim_::OldBase base;
        const cb::knowledge::show::Outcome out = cb::knowledge::show::run(base, args);
        py::print(py::str(out.text.substr(0, out.text.empty() ? 0 : out.text.size() - 1)));   // (print adds the last newline)
        return static_cast<int64_t>(out.exit_code);
    });
}

}  // namespace codebreaker::knowledge::show

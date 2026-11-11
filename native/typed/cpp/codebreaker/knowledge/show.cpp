// codebreaker/knowledge/show.cpp -- typed C++ port of codebreaker.knowledge.show.

#include "codebreaker/knowledge/show.hpp"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "json.hpp"
#include "py.hpp"
#include "py_json.hpp"

namespace codebreaker::knowledge::show {

namespace {

using engine::facts::Value;
using engine::native_engine::Cell;
using engine::native_engine::Cells;
using sources::Json;

// "%s" of a cell (None, a number, text)
py::str cell_str(const Cell& c) {
    if (std::holds_alternative<std::monostate>(c)) return py::str("None");
    if (std::holds_alternative<int64_t>(c)) return py::to_str(std::get<int64_t>(c));
    if (std::holds_alternative<double>(c)) return py::to_str(std::get<double>(c));
    if (std::holds_alternative<py::str>(c)) return std::get<py::str>(c);
    return py::repr(std::get<py::bytes>(c));
}

// a cell holding a number, as a double / an int (the base keeps counts and sizes as integers)
double cell_double(const Cell& c) {
    if (std::holds_alternative<int64_t>(c)) return static_cast<double>(std::get<int64_t>(c));
    if (std::holds_alternative<double>(c)) return std::get<double>(c);
    throw py::TypeError("unsupported operand type(s) for /: 'NoneType' and 'float'");
}

int64_t cell_int(const Cell& c) {
    if (std::holds_alternative<int64_t>(c)) return std::get<int64_t>(c);
    if (std::holds_alternative<double>(c)) return static_cast<int64_t>(std::get<double>(c));
    throw py::TypeError("%d format: a real number is required");
}

// ---- json.dumps(indent=1) ----
void put_json(std::string& o, const Json& v, int64_t level) {
    const std::string pad(static_cast<size_t>(level + 1), ' ');
    const std::string close_pad(static_cast<size_t>(level), ' ');
    switch (v.kind) {
        case Json::None: o += "null"; break;
        case Json::Bool: o += v.b ? "true" : "false"; break;
        case Json::Int: o += std::to_string(v.i); break;
        case Json::Float:
            if (std::isnan(v.d)) o += "NaN";
            else if (std::isinf(v.d)) o += v.d > 0 ? "Infinity" : "-Infinity";
            else o += py::to_str(v.d).utf8();
            break;
        case Json::Str: py::json::put(o, v.s); break;
        case Json::List:
            if (v.list.empty()) {
                o += "[]";
                break;
            }
            o += "[\n";
            for (size_t i = 0; i < v.list.size(); ++i) {
                if (i) o += ",\n";
                o += pad;
                put_json(o, v.list[i], level + 1);
            }
            o += "\n" + close_pad + "]";
            break;
        case Json::Dict: {
            if (v.dict.empty()) {
                o += "{}";
                break;
            }
            o += "{\n";
            bool first = true;
            for (const auto& kv : v.dict.items()) {
                if (!first) o += ",\n";
                first = false;
                o += pad;
                py::json::put(o, std::get<0>(kv));
                o += ": ";
                put_json(o, std::get<1>(kv), level + 1);
            }
            o += "\n" + close_pad + "}";
            break;
        }
    }
}

// (the first of two items sorted as Python sorts (str, int) pairs)
bool pair_less(const std::pair<py::str, int64_t>& a, const std::pair<py::str, int64_t>& b) {
    const std::u32string x = a.first.u32(), y = b.first.u32();
    if (x != y) return x < y;
    return a.second < b.second;
}

// text[:72] + ("..." if len(text) > 72 else "")
py::str clip_source(const py::str& source) {
    const std::u32string u = source.u32();
    if (u.size() > 72) return py::str(u.substr(0, 72)) + py::str("...");
    return source;
}

// time.strftime("%Y-%m-%d %I:%M %p", time.localtime(made))
py::str when(const Cell& made) {
    std::time_t t;
    if (std::holds_alternative<std::monostate>(made)) t = std::time(nullptr);   // localtime(None) is now
    else t = static_cast<std::time_t>(std::floor(cell_double(made)));
    std::tm tm_{};
    localtime_s(&tm_, &t);
    char buf[64];
    std::strftime(buf, sizeof buf, "%Y-%m-%d %I:%M %p", &tm_);
    return py::str(std::string(buf));
}

}  // namespace

py::str dumps(const Value& value) {
    std::string o;
    if (std::holds_alternative<Json>(value)) put_json(o, std::get<Json>(value), 0);
    else if (std::holds_alternative<py::str>(value)) py::json::put(o, std::get<py::str>(value));
    else py::json::put(o, py::repr(std::get<py::bytes>(value)));   // default=repr
    return py::str(o);
}

int64_t main(const py::list<py::str>& argv_in) {
    auto kb = store::open();
    py::list<py::str> argv = argv_in;
    bool load = false;
    for (const py::str& a : argv) {
        if (a == py::str("--load")) load = true;
    }
    if (load) {
        py::list<py::str> rest;
        for (const py::str& a : argv) {
            if (!(a == py::str("--load"))) rest.append(a);
        }
        argv = rest;
        for (const auto& pk : sources::AUTHORED().keys()) sources::authored(std::get<0>(pk), std::get<1>(pk), kb);
    }
    if (py::len(argv) <= 1) {
        std::optional<py::str> want;
        if (py::len(argv) > 0) want = knowledge::platform(argv[0]);
        py::print(py::percent(py::str("%s"), kb->path));
        for (const Cells& row : kb->kinds(want)) {
            const py::str p = std::get<py::str>(row[0]);
            const py::str k = std::get<py::str>(row[1]);
            std::vector<std::pair<py::str, int64_t>> origins;
            const py::V parsed = py::json_parse(std::get<py::str>(row[4]).utf8());
            for (const auto& kv : parsed.items()) origins.emplace_back(py::str(kv.first.str()), kv.second.as_int());
            std::sort(origins.begin(), origins.end(), pair_less);
            py::list<py::str> parts;
            for (const auto& o : origins) parts.append(py::percent(py::str("%s %d"), o.first, o.second));
            const py::str source = std::holds_alternative<py::str>(row[5]) ? std::get<py::str>(row[5]) : py::str("");
            py::print(py::percent(py::str("  %-8s %-24s %7d  %8.1f KB  %-28s %s"), p, k, cell_int(row[2]),
                                  cell_double(row[3]) / 1024.0, py::join(py::str(", "), parts), clip_source(source)));
        }
        const Cells shared = kb->shared_bodies();
        py::print(py::percent(py::str("  (bodies) %d recorded, %d distinct, %d in two or more modules"), cell_int(shared[0]),
                              cell_int(shared[1]), cell_int(shared[2])));
        return 0;
    }
    const py::str p = knowledge::platform(argv[0]);
    const py::str kind = argv[1];
    if (py::len(argv) == 2) {
        const py::dict<py::str, Value> t = knowledge::table(p, kind);
        int64_t shown = 0;
        for (const py::str& key : t.keys()) {
            if (shown++ >= 200) break;
            py::print(key);
        }
        py::print(py::percent(py::str("(%d)"), t.size()));
        return 0;
    }
    const py::str key = argv[2];
    const py::dict<py::str, Value> t = knowledge::table(p, kind);
    const std::optional<Value> fallback = kb->get(p, kind, key);     // (the default is evaluated first, as the call's argument)
    std::optional<Value> value = fallback;
    if (t.contains(key)) value = t.at(key);
    const std::optional<Cells> about = kb->about(p, kind, key);
    if (!about) {
        py::print(py::percent(py::str("nothing known: %s %s %s"), p, kind, key));
        return 1;
    }
    py::str text = value ? dumps(*value) : py::str("null");
    if (py::len(text) > 20000) text = text.slice(0, 20000);
    py::print(text);
    const Cells& a = *about;
    py::print(py::percent(py::str("source: %s\norigin: %s\nmade:   %s\nhash:   %s"), cell_str(a[0]), cell_str(a[1]), when(a[2]),
                          cell_str(a[3])));
    return 0;
}

}  // namespace codebreaker::knowledge::show

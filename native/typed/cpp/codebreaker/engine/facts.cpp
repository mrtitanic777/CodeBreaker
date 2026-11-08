// codebreaker/engine/facts.cpp -- typed C++ port of codebreaker.engine.facts (see facts.hpp).

#include "codebreaker/engine/facts.hpp"

#include <cmath>
#include <mutex>

#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/symbols/native.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::engine::facts {

const py::str GIVEN("given");

namespace {

std::mutex& open_lock() {
    static std::mutex m;
    return m;
}
py::dict<py::str, std::shared_ptr<native_engine::Database>>& open_dbs() {
    static py::dict<py::str, std::shared_ptr<native_engine::Database>> d;     // path -> Database, of this process
    return d;
}

// settings.state(*parts): a path inside the state folder, which is created if it is not there
py::str state(const py::list<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(parts.empty() ? path : py::os::path::dirname(path), 0777, true);
    return path;
}

// json.dumps(value, sort_keys=True, separators=(",", ":")): false when the value holds a NaN or an infinity
// (json.dumps writes them, but json.loads(text) == value is then false: Python pickles such a value)
bool dumps_into(std::string& o, const Json& v) {
    switch (v.kind) {
        case Json::None: o += "null"; return true;
        case Json::Bool: o += v.b ? "true" : "false"; return true;
        case Json::Int: o += std::to_string(v.i); return true;
        case Json::Float:
            if (std::isnan(v.d) || std::isinf(v.d)) return false;
            py::json::put(o, v.d);
            return true;
        case Json::Str: py::json::put(o, v.s); return true;
        case Json::List: {
            o += '[';
            bool first = true;
            for (const Json& x : v.list) {
                if (!first) o += ',';
                first = false;
                if (!dumps_into(o, x)) return false;
            }
            o += ']';
            return true;
        }
        case Json::Dict: {
            o += '{';
            py::list<py::str> keys;
            for (const py::str& k : v.dict.keys()) keys.append(k);
            bool first = true;
            for (const py::str& k : py::sorted(keys)) {
                if (!first) o += ',';
                first = false;
                py::json::put(o, k);
                o += ':';
                if (!dumps_into(o, v.dict.at(k))) return false;
            }
            o += '}';
            return true;
        }
    }
    return false;
}

}  // namespace

Json from_native(const symbols::native::Json& j) {
    Json out;
    const auto& v = j.value;
    if (std::holds_alternative<std::monostate>(v)) {
        out.kind = Json::None;
    } else if (const bool* b = std::get_if<bool>(&v)) {
        out.kind = Json::Bool;
        out.b = *b;
    } else if (const int64_t* i = std::get_if<int64_t>(&v)) {
        out.kind = Json::Int;
        out.i = *i;
    } else if (const double* d = std::get_if<double>(&v)) {
        out.kind = Json::Float;
        out.d = *d;
    } else if (const py::str* s = std::get_if<py::str>(&v)) {
        out.kind = Json::Str;
        out.s = *s;
    } else if (const auto* a = std::get_if<symbols::native::Json::Array>(&v)) {
        out.kind = Json::List;
        for (const auto& x : *a) out.list.push_back(from_native(x));
    } else {
        out.kind = Json::Dict;
        for (const auto& kv : std::get<symbols::native::Json::Object>(v)) out.dict.set(kv.first, from_native(kv.second));
    }
    return out;
}

py::str json_dumps(const Json& value) {
    std::string o;
    if (!dumps_into(o, value)) throw py::ValueError("Out of range float values are not JSON compliant");
    return py::str(o);
}

Json json_loads(const py::str& text) { return from_native(symbols::native::loads(text)); }

std::tuple<py::str, py::bytes> encode(const Value& value) {
    if (const py::bytes* b = std::get_if<py::bytes>(&value)) return {py::str("B"), *b};
    if (const py::str* s = std::get_if<py::str>(&value)) return {py::str("S"), s->encode("utf-8", "surrogatepass")};
    const Json& j = std::get<Json>(value);
    if (j.kind == Json::Str) return {py::str("S"), j.s.encode("utf-8", "surrogatepass")};   // (a str is kept as text)
    std::string o;
    if (!dumps_into(o, j)) {
        // Python would pickle it (protocol 4): a typed program has no pickle
        throw py::NotImplementedError("facts.encode: a value only a pickle can keep (NaN or infinity)");
    }
    return {py::str("J"), py::str(o).encode("utf-8")};
}

Value decode(const py::str& tag, const py::bytes& data) {
    if (tag == "B") return data;
    if (tag == "S") return data.decode("utf-8", "surrogatepass");
    if (tag == "J") return json_loads(data.decode("utf-8"));
    throw py::NotImplementedError("facts.decode: a pickled value has no typed form");
}

// ---- Row ----
Row::Row(native_engine::Database* db_, const py::str& kind_, const py::str& key_, const py::str& hash_,
         const py::str& tag_, const std::optional<py::bytes>& value, const std::optional<py::str>& blob,
         int64_t size_, const py::str& step_, const py::str& version_, const std::optional<py::str>& inputs_,
         double made_, double took_)
    : db(db_), kind(kind_), key(key_), hash(hash_), tag(tag_), size(size_), step(step_), version(version_),
      made(made_), took(took_), value_(value), blob_(blob) {
    if (inputs_ && !inputs_->empty()) {
        Json rows = json_loads(*inputs_);
        for (const Json& r : rows.list) {
            // tuple(i): each input is [kind, key, hash]
            inputs.append(Input{r.list.at(0).s, r.list.at(1).s, r.list.at(2).s});
        }
    }
}

py::bytes Row::data() const {
    if (value_) return *value_;
    return db->blob(*blob_);
}

Value Row::value() const { return decode(tag, data()); }

bool Row::given() const { return step == GIVEN; }

std::shared_ptr<native_engine::Database> Database(const py::str& path) {
    return std::make_shared<native_engine::Database>(path);
}

// --- where the databases are ---
py::str root() { return state(py::list<py::str>{py::str("db")}); }

py::str sha(const py::str& path) { return native_engine::sha(path); }

std::shared_ptr<native_engine::Database> open_at(const py::str& path_in) {
    py::str path = py::os::path::abspath(path_in);
    std::lock_guard<std::mutex> g(open_lock());
    auto found = open_dbs().get(path);
    if (found) return found;
    auto db = Database(path);
    open_dbs().set(path, db);
    return db;
}

void forget_open(const py::str& path) {
    std::lock_guard<std::mutex> g(open_lock());
    open_dbs().pop(path, nullptr);
}

std::shared_ptr<native_engine::Database> open_for(const py::str& binary, const std::optional<py::str>& folder) {
    py::str s = sha(binary);
    auto db = open_at(py::os::path::join(folder ? *folder : root(), s + ".sqlite"));
    db->sha = s;
    if (!db->about(py::str("name"))) db->about(py::str("name"), py::os::path::basename(binary));
    db->about(py::str("path"), py::os::path::abspath(binary));
    return db;
}

py::str work(native_engine::Database& db, const py::list<py::str>& parts) {
    py::str p = std::get<0>(py::os::path::splitext(db.path)) + ".work";
    for (const py::str& x : parts) p = py::os::path::join(p, x);
    py::os::makedirs(p, 0777, true);
    return p;
}

int64_t max_bytes() {
    double gb;
    try {
        py::str v = py::settings::get_str(py::str("db_max_gb"));
        gb = v.empty() ? static_cast<double>(DEFAULT_MAX_GB) : std::stod(v.utf8());
    } catch (const std::invalid_argument&) {
        gb = static_cast<double>(DEFAULT_MAX_GB);
    } catch (const py::TypeError&) {
        // settings.json holds it as a number: read as one
        try {
            gb = py::settings::get_float(py::str("db_max_gb"));
            if (gb == 0) gb = static_cast<double>(DEFAULT_MAX_GB);
        } catch (const py::Exception&) {
            gb = static_cast<double>(DEFAULT_MAX_GB);
        }
    }
    return static_cast<int64_t>(gb * static_cast<double>(int64_t(1) << 30));
}

py::list<std::tuple<py::str, int64_t, double>> databases(const std::optional<py::str>& folder) {
    return native_engine::databases(folder ? *folder : root());
}

py::list<py::str> hold(const std::optional<int64_t>& cap, const std::optional<py::str>& folder) {
    return native_engine::hold(cap ? *cap : max_bytes(), folder ? *folder : root());
}

}  // namespace codebreaker::engine::facts

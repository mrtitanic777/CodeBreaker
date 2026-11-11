// codebreaker/engine/facts.cpp -- typed C++ port of codebreaker.engine.facts (see facts.hpp).

#include "codebreaker/engine/facts.hpp"

#include <cmath>
#include <cstring>
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


// ---- the pickles (protocol 4) of the values a typed program can hold ----
// Python keeps a value json cannot give back unchanged (a tuple, say) as pickle.dumps(value, protocol=4). The
// subset here is what the knowledge base holds: None, bool, int, float, str, list, tuple, dict with str keys.
// A str is memoized and a later equal `shared` str is written as a reference to it, as the pickler does for one
// str object met twice (the Python's constants, such as the "uint4" of a pointer field).

bool has_tuple(const Json& v) {
    if (v.kind == Json::List) {
        if (v.tuple) return true;
        for (const Json& x : v.list)
            if (has_tuple(x)) return true;
    } else if (v.kind == Json::Dict) {
        for (const auto& kv : v.dict.items())
            if (has_tuple(std::get<1>(kv))) return true;
    }
    return false;
}

struct PickleOut {
    std::string o;
    int64_t memos = 0;
    std::vector<std::pair<std::string, int64_t>> shared;     // (utf-8 text, memo index) of the shared strs met

    void u32le(uint32_t v) {
        for (int b = 0; b < 4; ++b) o += static_cast<char>((v >> (8 * b)) & 0xff);
    }
    void memoize() {
        o += '\x94';
        ++memos;
    }
    void integer(int64_t v) {
        if (v >= 0 && v < 256) {
            o += 'K';
            o += static_cast<char>(v);
        } else if (v >= 0 && v < 65536) {
            o += 'M';
            o += static_cast<char>(v & 0xff);
            o += static_cast<char>(v >> 8);
        } else if (v >= INT32_MIN && v <= INT32_MAX) {
            o += 'J';
            u32le(static_cast<uint32_t>(static_cast<int32_t>(v)));
        } else {
            // LONG1: the shortest little-endian two's complement
            std::string b;
            int64_t x = v;
            while (true) {
                b += static_cast<char>(x & 0xff);
                x >>= 8;      // (arithmetic)
                bool sign = (b.back() & 0x80) != 0;
                if ((x == 0 && !sign) || (x == -1 && sign)) break;
            }
            o += '\x8a';
            o += static_cast<char>(b.size());
            o += b;
        }
    }
    void put(const Json& v) {
        switch (v.kind) {
            case Json::None: o += 'N'; return;
            case Json::Bool: o += v.b ? '\x88' : '\x89'; return;
            case Json::Int: integer(v.i); return;
            case Json::Float: {
                uint64_t bits;
                std::memcpy(&bits, &v.d, 8);
                o += 'G';
                for (int b = 7; b >= 0; --b) o += static_cast<char>((bits >> (8 * b)) & 0xff);
                return;
            }
            case Json::Str: str(v.s, v.shared); return;
            case Json::List: {
                size_t n = v.list.size();
                if (v.tuple) {
                    if (n == 0) {
                        o += ')';
                        return;
                    }
                    if (n <= 3) {
                        for (const Json& x : v.list) put(x);
                        o += static_cast<char>('\x85' + (n - 1));
                    } else {
                        o += '(';
                        for (const Json& x : v.list) put(x);
                        o += 't';
                    }
                    memoize();
                    return;
                }
                o += ']';
                memoize();
                for (size_t at = 0; at < n; at += 1000) {
                    size_t end = std::min(n, at + 1000);
                    if (end - at == 1) {
                        put(v.list[at]);
                        o += 'a';
                    } else {
                        o += '(';
                        for (size_t k = at; k < end; ++k) put(v.list[k]);
                        o += 'e';
                    }
                }
                return;
            }
            case Json::Dict: {
                o += '}';
                memoize();
                std::vector<std::pair<py::str, const Json*>> items;
                for (const auto& kv : v.dict.items()) items.emplace_back(std::get<0>(kv), &std::get<1>(kv));
                size_t n = items.size();
                for (size_t at = 0; at < n; at += 1000) {
                    size_t end = std::min(n, at + 1000);
                    if (end - at == 1) {
                        str(items[at].first, false);
                        put(*items[at].second);
                        o += 's';
                    } else {
                        o += '(';
                        for (size_t k = at; k < end; ++k) {
                            str(items[k].first, false);
                            put(*items[k].second);
                        }
                        o += 'u';
                    }
                }
                return;
            }
        }
    }
    void str(const py::str& s, bool is_shared) {
        std::string u = s.utf8();
        if (is_shared) {
            for (const auto& seen : shared) {
                if (seen.first != u) continue;
                if (seen.second < 256) {
                    o += 'h';
                    o += static_cast<char>(seen.second);
                } else {
                    o += 'j';
                    u32le(static_cast<uint32_t>(seen.second));
                }
                return;
            }
            shared.emplace_back(u, memos);
        }
        if (u.size() < 256) {
            o += '\x8c';
            o += static_cast<char>(u.size());
        } else {
            o += 'X';
            u32le(static_cast<uint32_t>(u.size()));
        }
        o += u;
        memoize();
    }
};

// pickle.dumps(value, protocol=4) for a value of that subset
py::bytes pickle_dumps(const Json& value) {
    PickleOut out;
    out.put(value);
    out.o += '.';
    if (out.o.size() >= (1u << 16)) throw py::NotImplementedError("facts.encode: a pickle of more than one frame");
    std::string framed = "\x80\x04";
    framed += '\x95';
    uint64_t n = out.o.size();
    for (int b = 0; b < 8; ++b) framed += static_cast<char>((n >> (8 * b)) & 0xff);
    framed += out.o;
    return py::bytes(framed);
}

// pickle.loads(data) of a pickle in that subset (what pickle_dumps writes, or the Python's of such a value)
Json pickle_loads(const std::string& d) {
    size_t at = 0;
    std::vector<Json> stack, memo;
    std::vector<size_t> marks;
    auto bad = [](const char* why) { return py::ValueError(std::string("facts.decode: ") + why); };
    auto need = [&](size_t k) {
        if (at + k > d.size()) throw bad("pickle cut short");
    };
    auto num = [&](size_t k) {
        need(k);
        uint64_t v = 0;
        for (size_t b = 0; b < k; ++b) v |= static_cast<uint64_t>(static_cast<uint8_t>(d[at + b])) << (8 * b);
        at += k;
        return v;
    };
    auto top = [&]() -> Json& {
        if (stack.empty()) throw bad("pickle stack empty");
        return stack.back();
    };
    auto push_text = [&](size_t len) {
        need(len);
        Json j;
        j.kind = Json::Str;
        j.s = py::str(d.substr(at, len));
        at += len;
        stack.push_back(j);
    };
    auto make_tuple = [&](size_t from) {
        Json t;
        t.kind = Json::List;
        t.tuple = true;
        for (size_t k = from; k < stack.size(); ++k) t.list.push_back(stack[k]);
        stack.resize(from);
        stack.push_back(t);
    };
    auto take_mark = [&]() {
        if (marks.empty()) throw bad("pickle mark missing");
        size_t m = marks.back();
        marks.pop_back();
        return m;
    };
    auto fixed_tuple = [&](size_t n) {
        if (stack.size() < n) throw bad("pickle stack short");
        make_tuple(stack.size() - n);
    };
    while (true) {
        need(1);
        uint8_t op = static_cast<uint8_t>(d[at++]);
        switch (op) {
            case 0x80: num(1); break;                                    // PROTO
            case 0x95: num(8); break;                                    // FRAME
            case 'N': stack.push_back(Json()); break;
            case 0x88: case 0x89: { Json j; j.kind = Json::Bool; j.b = op == 0x88; stack.push_back(j); break; }
            case 'K': case 'M': case 'J': {
                Json j;
                j.kind = Json::Int;
                j.i = op == 'K' ? static_cast<int64_t>(num(1)) : op == 'M' ? static_cast<int64_t>(num(2))
                                                                           : static_cast<int64_t>(static_cast<int32_t>(num(4)));
                stack.push_back(j);
                break;
            }
            case 0x8a: {                                                 // LONG1
                size_t n = static_cast<size_t>(num(1));
                need(n);
                if (n > 8) throw bad("pickle integer past int64");
                int64_t v = n && (d[at + n - 1] & 0x80) ? -1 : 0;
                for (size_t b = n; b-- > 0;) v = static_cast<int64_t>((static_cast<uint64_t>(v) << 8) | static_cast<uint8_t>(d[at + b]));
                at += n;
                Json j;
                j.kind = Json::Int;
                j.i = v;
                stack.push_back(j);
                break;
            }
            case 'G': {
                need(8);
                uint64_t bits = 0;
                for (int b = 0; b < 8; ++b) bits = (bits << 8) | static_cast<uint8_t>(d[at + b]);
                at += 8;
                Json j;
                j.kind = Json::Float;
                std::memcpy(&j.d, &bits, 8);
                stack.push_back(j);
                break;
            }
            case 0x8c: push_text(static_cast<size_t>(num(1))); break;     // SHORT_BINUNICODE
            case 'X': push_text(static_cast<size_t>(num(4))); break;      // BINUNICODE
            case 0x94: memo.push_back(top()); break;                       // MEMOIZE
            case 'h': case 'j': {
                size_t idx = static_cast<size_t>(num(op == 'h' ? 1 : 4));
                if (idx >= memo.size()) throw bad("pickle memo missing");
                Json j = memo[idx];
                stack.push_back(j);
                break;
            }
            case ']': { Json j; j.kind = Json::List; stack.push_back(j); break; }
            case '}': { Json j; j.kind = Json::Dict; stack.push_back(j); break; }
            case ')': { Json j; j.kind = Json::List; j.tuple = true; stack.push_back(j); break; }
            case '(': marks.push_back(stack.size()); break;
            case 'a': {
                if (stack.size() < 2) throw bad("pickle stack short");
                Json item = stack.back();
                stack.pop_back();
                stack.back().list.push_back(item);
                break;
            }
            case 'e': {
                size_t m = take_mark();
                if (m < 1) throw bad("pickle stack short");
                Json& list = stack[m - 1];
                for (size_t k = m; k < stack.size(); ++k) list.list.push_back(stack[k]);
                stack.resize(m);
                break;
            }
            case 's': {
                if (stack.size() < 3) throw bad("pickle stack short");
                Json val = stack.back();
                stack.pop_back();
                Json key = stack.back();
                stack.pop_back();
                stack.back().dict.set(key.s, val);
                break;
            }
            case 'u': {
                size_t m = take_mark();
                if (m < 1) throw bad("pickle stack short");
                Json& dict = stack[m - 1];
                for (size_t k = m; k + 1 < stack.size(); k += 2) dict.dict.set(stack[k].s, stack[k + 1]);
                stack.resize(m);
                break;
            }
            case 't': make_tuple(take_mark()); break;
            case 0x85: fixed_tuple(1); break;
            case 0x86: fixed_tuple(2); break;
            case 0x87: fixed_tuple(3); break;
            case '.':
                if (stack.size() != 1) throw bad("pickle stack at its end");
                return stack.back();
            default: throw py::NotImplementedError("facts.decode: a pickle opcode outside the typed subset");
        }
    }
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
    if (has_tuple(j)) return {py::str("P"), pickle_dumps(j)};     // (json gives a list back, which is not equal to a tuple)
    if (!dumps_into(o, j)) {
        // Python would pickle it (protocol 4): the subset above has no NaN or infinity
        throw py::NotImplementedError("facts.encode: a value only a pickle can keep (NaN or infinity)");
    }
    return {py::str("J"), py::str(o).encode("utf-8")};
}

Value decode(const py::str& tag, const py::bytes& data) {
    if (tag == "B") return data;
    if (tag == "S") return data.decode("utf-8", "surrogatepass");
    if (tag == "J") return json_loads(data.decode("utf-8"));
    return pickle_loads(data.raw());     // (tag P: a pickle in the typed subset; any other opcode is NotImplementedError)
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

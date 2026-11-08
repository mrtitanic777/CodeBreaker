// codebreaker/knowledge/sources.cpp -- C++ of codebreaker.knowledge.sources (see sources.hpp).

#include "codebreaker/knowledge/sources.hpp"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace codebreaker::knowledge::sources {

namespace {

// ---------------------------------------------------------------------------------- json.load
// The parser json.load is: strict (no control characters in strings), NaN / Infinity / -Infinity
// accepted, a repeated key keeps its first place and its last value, anything after the value is an error.
// A bad document is a ValueError (Python's JSONDecodeError is one).
class Parser {
  public:
    explicit Parser(const std::u32string& t) : t_(t) {}

    Json document() {
        skip();
        Json v = value();
        skip();
        if (p_ != t_.size()) fail("Extra data");
        return v;
    }

  private:
    const std::u32string& t_;
    size_t p_ = 0;

    [[noreturn]] void fail(const char* what) const {
        throw py::ValueError(std::string(what) + ": char " + std::to_string(p_));
    }
    void skip() {
        while (p_ < t_.size() && (t_[p_] == U' ' || t_[p_] == U'\t' || t_[p_] == U'\n' || t_[p_] == U'\r')) ++p_;
    }
    bool word(const char* w) {
        size_t n = std::char_traits<char>::length(w);
        if (t_.size() - p_ < n) return false;
        for (size_t k = 0; k < n; ++k)
            if (t_[p_ + k] != static_cast<char32_t>(w[k])) return false;
        p_ += n;
        return true;
    }
    static Json make_float(double d) {
        Json j;
        j.kind = Json::Float;
        j.d = d;
        return j;
    }
    static bool digit(char32_t c) { return c >= U'0' && c <= U'9'; }

    Json value() {
        if (p_ >= t_.size()) fail("Expecting value");
        char32_t c = t_[p_];
        Json j;
        if (c == U'"') {
            j.kind = Json::Str;
            j.s = string();
            return j;
        }
        if (c == U'{') return object();
        if (c == U'[') return array();
        if (c == U'n' && word("null")) return j;
        if (c == U't' && word("true")) {
            j.kind = Json::Bool;
            j.b = true;
            return j;
        }
        if (c == U'f' && word("false")) {
            j.kind = Json::Bool;
            return j;
        }
        if (c == U'N' && word("NaN")) return make_float(std::nan(""));
        if (c == U'I' && word("Infinity")) return make_float(HUGE_VAL);
        if (c == U'-' && word("-Infinity")) return make_float(-HUGE_VAL);
        return number();
    }

    // -?(0|[1-9]\d*)(\.\d+)?([eE][-+]?\d+)?
    Json number() {
        size_t s = p_, q = p_;
        if (q < t_.size() && t_[q] == U'-') ++q;
        if (q < t_.size() && t_[q] == U'0') ++q;
        else if (q < t_.size() && t_[q] >= U'1' && t_[q] <= U'9') {
            while (q < t_.size() && digit(t_[q])) ++q;
        } else fail("Expecting value");
        size_t int_end = q;
        bool is_float = false;
        if (q + 1 < t_.size() && t_[q] == U'.' && digit(t_[q + 1])) {
            q += 1;
            while (q < t_.size() && digit(t_[q])) ++q;
            is_float = true;
        }
        if (q < t_.size() && (t_[q] == U'e' || t_[q] == U'E')) {
            size_t r = q + 1;
            if (r < t_.size() && (t_[r] == U'+' || t_[r] == U'-')) ++r;
            if (r < t_.size() && digit(t_[r])) {
                while (r < t_.size() && digit(t_[r])) ++r;
                q = r;
                is_float = true;
            }
        }
        std::string text;
        for (size_t k = s; k < q; ++k) text += static_cast<char>(t_[k]);
        p_ = q;
        Json j;
        if (is_float) return make_float(std::strtod(text.c_str(), nullptr));
        (void)int_end;
        errno = 0;
        long long v = std::strtoll(text.c_str(), nullptr, 10);
        if (errno == ERANGE) throw py::OverflowError("integer does not fit int64");
        j.kind = Json::Int;
        j.i = v;
        return j;
    }

    static int hex4(const std::u32string& t, size_t at) {
        if (at + 4 > t.size()) return -1;
        int v = 0;
        for (size_t k = 0; k < 4; ++k) {
            char32_t c = t[at + k];
            int d;
            if (c >= U'0' && c <= U'9') d = static_cast<int>(c - U'0');
            else if (c >= U'a' && c <= U'f') d = static_cast<int>(c - U'a') + 10;
            else if (c >= U'A' && c <= U'F') d = static_cast<int>(c - U'A') + 10;
            else return -1;
            v = v * 16 + d;
        }
        return v;
    }

    py::str string() {
        ++p_;   // the opening quote
        std::u32string out;
        for (;;) {
            if (p_ >= t_.size()) fail("Unterminated string");
            char32_t c = t_[p_];
            if (c == U'"') {
                ++p_;
                return py::str(out);
            }
            if (c < 0x20) fail("Invalid control character");
            if (c != U'\\') {
                out.push_back(c);
                ++p_;
                continue;
            }
            if (p_ + 1 >= t_.size()) fail("Unterminated string");
            char32_t e = t_[p_ + 1];
            switch (e) {
                case U'"': out.push_back(U'"'); p_ += 2; break;
                case U'\\': out.push_back(U'\\'); p_ += 2; break;
                case U'/': out.push_back(U'/'); p_ += 2; break;
                case U'b': out.push_back(U'\b'); p_ += 2; break;
                case U'f': out.push_back(U'\f'); p_ += 2; break;
                case U'n': out.push_back(U'\n'); p_ += 2; break;
                case U'r': out.push_back(U'\r'); p_ += 2; break;
                case U't': out.push_back(U'\t'); p_ += 2; break;
                case U'u': {
                    int u = hex4(t_, p_ + 2);
                    if (u < 0) fail("Invalid \\uXXXX escape");
                    p_ += 6;
                    if (u >= 0xD800 && u <= 0xDBFF && p_ + 1 < t_.size() && t_[p_] == U'\\' && t_[p_ + 1] == U'u') {
                        int lo = hex4(t_, p_ + 2);
                        if (lo >= 0xDC00 && lo <= 0xDFFF) {   // a surrogate pair is one character
                            out.push_back(static_cast<char32_t>(0x10000 + (((u - 0xD800) << 10) | (lo - 0xDC00))));
                            p_ += 6;
                            break;
                        }
                    }
                    out.push_back(static_cast<char32_t>(u));   // (a lone surrogate stays, as Python's)
                    break;
                }
                default: fail("Invalid \\escape");
            }
        }
    }

    Json array() {
        ++p_;
        Json j;
        j.kind = Json::List;
        skip();
        if (p_ < t_.size() && t_[p_] == U']') {
            ++p_;
            return j;
        }
        for (;;) {
            skip();
            j.list.push_back(value());
            skip();
            if (p_ < t_.size() && t_[p_] == U']') {
                ++p_;
                return j;
            }
            if (p_ >= t_.size() || t_[p_] != U',') fail("Expecting ',' delimiter");
            ++p_;
        }
    }

    Json object() {
        ++p_;
        Json j;
        j.kind = Json::Dict;
        skip();
        if (p_ < t_.size() && t_[p_] == U'}') {
            ++p_;
            return j;
        }
        for (;;) {
            skip();
            if (p_ >= t_.size() || t_[p_] != U'"') fail("Expecting property name enclosed in double quotes");
            py::str key = string();
            skip();
            if (p_ >= t_.size() || t_[p_] != U':') fail("Expecting ':' delimiter");
            ++p_;
            skip();
            j.dict.set(key, value());
            skip();
            if (p_ < t_.size() && t_[p_] == U'}') {
                ++p_;
                return j;
            }
            if (p_ >= t_.size() || t_[p_] != U',') fail("Expecting ',' delimiter");
            ++p_;
        }
    }
};

// ---------------------------------------------------------------------------------- Python's reading of a JSON value
bool truthy(const Json& v) {
    switch (v.kind) {
        case Json::None: return false;
        case Json::Bool: return v.b;
        case Json::Int: return v.i != 0;
        case Json::Float: return v.d != 0.0;
        case Json::Str: return !v.s.u32().empty();
        case Json::List: return !v.list.empty();
        case Json::Dict: return !v.dict.empty();
    }
    return false;
}

Json text_of(const std::u32string& chars) {
    Json j;
    j.kind = Json::Str;
    j.s = py::str(chars);
    return j;
}

// `for x in v` (a dict is its keys, a str its characters), or the TypeError of a value that is not iterable
std::vector<Json> iterate(const Json& v) {
    std::vector<Json> out;
    switch (v.kind) {
        case Json::List: return v.list;
        case Json::Dict:
            for (const py::str& k : v.dict.keys()) out.push_back(text_of(k.u32()));
            return out;
        case Json::Str:
            for (char32_t c : v.s.u32()) out.push_back(text_of(std::u32string(1, c)));
            return out;
        default: throw py::TypeError("object is not iterable");
    }
}

// x.get(name) of a value that must be a dict (else the AttributeError of a value without .get); nullopt: None
std::optional<Json> get_of(const Json& v, const char* name) {
    if (v.kind != Json::Dict) throw py::AttributeError("object has no attribute 'get'");
    py::str key(name);
    if (!v.dict.contains(key)) return std::nullopt;
    return v.dict.at(key);
}

// container[index]
Json item_of(const Json& container, const Json& index) {
    if (container.kind == Json::List || container.kind == Json::Str) {
        int64_t n;
        if (index.kind == Json::Int) n = index.i;
        else if (index.kind == Json::Bool) n = index.b ? 1 : 0;
        else throw py::TypeError("indices must be integers or slices");
        int64_t len = container.kind == Json::List ? static_cast<int64_t>(container.list.size())
                                                    : static_cast<int64_t>(container.s.u32().size());
        if (n < 0) n += len;
        if (n < 0 || n >= len) throw py::IndexError("index out of range");
        if (container.kind == Json::List) return container.list[static_cast<size_t>(n)];
        return text_of(std::u32string(1, container.s.u32()[static_cast<size_t>(n)]));
    }
    if (container.kind == Json::Dict) {
        if (index.kind == Json::List || index.kind == Json::Dict) throw py::TypeError("unhashable type");
        // (a JSON object's keys are all str: only a str index can be one)
        if (index.kind == Json::Str && container.dict.contains(index.s)) return container.dict.at(index.s);
        throw py::KeyError("key not found");
    }
    throw py::TypeError("object is not subscriptable");
}

py::str zero_padded_6(int64_t i) {   // "%06d" % i
    char b[32];
    std::snprintf(b, sizeof b, "%06lld", static_cast<long long>(i));
    return py::str(b);
}

}  // namespace

// ---------------------------------------------------------------------------------- where the files are
std::function<std::shared_ptr<KnowledgeBase>()>& store_open() {
    static std::function<std::shared_ptr<KnowledgeBase>()> open;
    return open;
}

namespace {
std::optional<py::str>& here_() {
    static std::optional<py::str> h;
    return h;
}
std::optional<py::dict<std::tuple<py::str, py::str>, Source>>& authored_table_() {
    static std::optional<py::dict<std::tuple<py::str, py::str>, Source>> t;
    return t;
}
}  // namespace

py::str _HERE() {
    if (here_()) return *here_();
    if (const char* root = std::getenv("CB_CHECKOUT_ROOT"))
        return py::os::path::abspath(py::os::path::join(py::str(root), py::str("codebreaker"), py::str("knowledge")));
    return py::os::path::abspath(py::os::path::join(py::str("codebreaker"), py::str("knowledge")));
}

void set_here(const py::str& here) {
    here_() = py::os::path::abspath(here);
    authored_table_().reset();   // (AUTHORED's files are _HERE's)
}

py::str _SYMBOLS() { return py::os::path::join(py::os::path::dirname(_HERE()), py::str("symbols")); }
py::str _MODELS() { return py::os::path::join(_HERE(), py::str("models")); }

// ---------------------------------------------------------------------------------- the readers
Json _json(const py::str& path) {
    py::str text = py::open_rb(path).read().decode("utf-8");
    return Parser(text.u32()).document();
}

Reader _signatures(const py::str& bits) {
    return [bits](const py::str& path) -> Rows {
        Json data = _json(path);
        std::optional<Json> libs = get_of(data, "libraries");
        Json labels;
        if (libs && truthy(*libs)) labels = *libs;
        else {
            labels.kind = Json::List;
        }
        std::optional<Json> rows_of = get_of(data, bits.utf8().c_str());
        std::vector<Json> rows;
        if (rows_of && truthy(*rows_of)) rows = iterate(*rows_of);
        Rows out;
        int64_t i = 0;
        for (const Json& row : rows) {
            std::vector<Json> f = iterate(row);       // lead, size, mask, digest, name, lib = row
            if (f.size() < 6) throw py::ValueError("not enough values to unpack");
            if (f.size() > 6) throw py::ValueError("too many values to unpack");
            Json entry;
            entry.kind = Json::List;
            entry.list = {f[0], f[1], f[2], f[3], f[4], item_of(labels, f[5])};
            out.set(zero_padded_6(i), entry);
            ++i;
        }
        return out;
    };
}

Reader _part(const py::str& name, const std::function<py::str(const py::str&)>& keyed) {
    return [name, keyed](const py::str& path) -> Rows {
        Json data = _json(path);
        std::optional<Json> part = get_of(data, name.utf8().c_str());
        Rows out;
        if (!part || !truthy(*part)) return out;
        if (part->kind != Json::Dict) throw py::AttributeError("object has no attribute 'items'");
        for (const auto& kv : part->dict.items()) {
            const py::str& k = std::get<0>(kv);
            out.set(keyed ? keyed(k) : k, std::get<1>(kv));
        }
        return out;
    };
}

Rows _whole(const py::str& path) {
    Json data = _json(path);
    if (data.kind == Json::Dict) return data.dict.copy();
    // dict(x) of a sequence of pairs; a pair's key that is not a str cannot be a Rows key (the data files
    // are objects, so this is only for the errors dict() gives)
    Rows out;
    for (const Json& pair : iterate(data)) {
        std::vector<Json> kv = iterate(pair);
        if (kv.size() != 2) throw py::ValueError("dictionary update sequence element has the wrong length");
        if (kv[0].kind == Json::List || kv[0].kind == Json::Dict) throw py::TypeError("unhashable type");
        if (kv[0].kind != Json::Str) throw py::TypeError("a key that is not text is not representable");
        out.set(kv[0].s, kv[1]);
    }
    return out;
}

// ---------------------------------------------------------------------------------- AUTHORED
const py::dict<std::tuple<py::str, py::str>, Source>& AUTHORED() {
    if (!authored_table_()) {
        using Key = std::tuple<py::str, py::str>;
        py::dict<Key, Source> t;
        py::str symbols = _SYMBOLS(), models = _MODELS();
        auto j = [](const py::str& dir, const char* file) { return py::os::path::join(dir, py::str(file)); };
        auto add = [&](const char* platform, const char* kind, const py::str& file, const Reader& read, const char* what) {
            t.set(Key(py::str(platform), py::str(kind)), Source{file, read, py::str(what)});
        };
        Reader whole = _whole;
        add("windows", "ordinal_names", j(symbols, "ordinal_names.json"), whole,
            "names of numbered exports from their makers' definition files and import "
            "libraries: {module: {from, names: {ordinal: name}}}");
        add("win32", "argument_bytes.header", j(symbols, "win32_arguments.json"), whole,
            "bytes of arguments as the Win32 headers declare them (tools/make_api_table.py)");
        add("win32", "signature", j(symbols, "runtime_signatures.json"), _signatures("32"),
            "32-bit runtime-library functions: [leading bytes, size, mask, digest, name, library]");
        add("win16", "signature", j(symbols, "runtime_signatures.json"), _signatures("16"),
            "16-bit runtime-library functions: [leading bytes, size, mask, digest, name, library]");
        add("vxd", "device", j(symbols, "driver_services.json"), _part("devices"), "device numbers the driver kit names");
        add("vxd", "service_table", j(symbols, "driver_services.json"), _part("services"),
            "each device's services in order (service N is the Nth)");
        add("vxd", "driver", j(symbols, "driver_services.json"), _part("drivers"),
            "device numbers corpus drivers give themselves");
        add("vxd", "message", j(symbols, "driver_services.json"), _part("messages"),
            "messages a driver's control procedure is sent");
        add("vxd", "api_model", j(models, "vxd.json"), _part("services"),
            "what a service does that the emulator's stand-in must do too");
        add("dos", "interrupt", j(symbols, "dos_interrupts.json"), whole, "DOS and BIOS interrupt functions: names and arguments");
        add("xbox360", "ordinal_names", j(symbols, "console_ordinals.json"), whole,
            "numbered exports of the console's system libraries");
        add("win16", "api_model", j(models, "win16.json"), _part("functions"),
            "what the emulator's stand-in for a Windows function returns and removes");
        add("win32", "api_model", j(models, "win32.json"), _part("functions"),
            "documented argument counts for the emulator's stand-in of a common import");
        add("win16", "kernel_ordinal", j(models, "win16.json"), _part("kernel_ordinals"),
            "KERNEL's numbers for the functions the stand-ins model");
        authored_table_() = t;
    }
    return *authored_table_();
}

py::str file_stamp(const py::str& path) {
    try {
        py::file f;
        {
            f = py::open_rb(path);
            return py::hashlib::sha1(f.read()).hexdigest();
        }
    } catch (const py::OSError&) {
        return "missing";
    }
}

py::dict<py::str, py::str>& _stamps() {
    static py::dict<py::str, py::str> stamps;
    return stamps;
}

void changed(const py::str& path_in) {
    py::str path = py::os::path::normcase(py::os::path::abspath(path_in));
    if (!store_open()) throw py::RuntimeError("the knowledge base is not open");
    std::shared_ptr<KnowledgeBase> kb = store_open()();
    for (const auto& entry : AUTHORED().items()) {
        const auto& key = std::get<0>(entry);
        const Source& src = std::get<1>(entry);
        if (py::os::path::normcase(src.file) == path) {
            _stamps().pop(src.file, py::str());   // (a pop with a default: absent is fine)
            kb->forget_ensured(std::get<0>(key), std::get<1>(key));
        }
    }
}

Rows authored(const py::str& platform, const py::str& kind, const std::shared_ptr<KnowledgeBase>& kb_in) {
    const Source& src = AUTHORED().at(std::make_tuple(platform, kind));   // (an unknown kind: KeyError)
    py::str path = src.file;
    Reader read = src.read;
    std::shared_ptr<KnowledgeBase> kb = kb_in;
    if (!kb) {
        if (!store_open()) throw py::RuntimeError("the knowledge base is not open");
        kb = store_open()();
    }
    py::str stamp;
    if (_stamps().contains(path)) stamp = _stamps().at(path);
    else {
        stamp = file_stamp(path);
        _stamps().set(path, stamp);
    }
    auto make = [read, path]() -> Rows {
        try {
            return read(path);
        } catch (const py::OSError&) {
            return Rows();
        } catch (const py::ValueError&) {
            return Rows();
        }
    };
    py::str root = py::os::path::dirname(py::os::path::dirname(_HERE()));
    return kb->ensure(platform, kind, stamp, make, py::os::path::relpath(path, root));
}

}  // namespace codebreaker::knowledge::sources

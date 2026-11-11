// codebreaker/symbols/dos_interrupts.cpp -- C++ from codebreaker.symbols.dos_interrupts.

#include "py_settings.hpp"
#include "codebreaker/symbols/dos_interrupts.hpp"

namespace codebreaker::symbols::dos_interrupts {

py::str TABLE_PATH = "codebreaker/symbols/dos_interrupts.json";

namespace {

// ---- a small JSON reader for the table's file (strings, arrays and objects only) -------------------------------
struct Json {
    enum Kind { String, Array, Object } kind = String;
    py::str text;
    std::vector<Json> items;
    std::vector<std::pair<py::str, Json>> members;
};

class Reader {
public:
    explicit Reader(const py::str& text) : s_(text.u32()) {}

    Json document() {
        Json v = value();
        space();
        if (i_ != s_.size()) fail("extra data");
        return v;
    }

private:
    const std::u32string s_;
    size_t i_ = 0;

    [[noreturn]] void fail(const char* why) const { throw py::ValueError(std::string("dos_interrupts.json: ") + why); }
    void space() {
        while (i_ < s_.size() && (s_[i_] == ' ' || s_[i_] == '\t' || s_[i_] == '\n' || s_[i_] == '\r')) ++i_;
    }
    void expect(char32_t c) {
        space();
        if (i_ >= s_.size() || s_[i_] != c) fail("unexpected character");
        ++i_;
    }
    unsigned hex4() {
        if (i_ + 4 > s_.size()) fail("bad \\u escape");
        unsigned v = 0;
        for (int k = 0; k < 4; ++k) {
            char32_t c = s_[i_++];
            v <<= 4;
            if (c >= '0' && c <= '9') v |= c - '0';
            else if (c >= 'a' && c <= 'f') v |= c - 'a' + 10;
            else if (c >= 'A' && c <= 'F') v |= c - 'A' + 10;
            else fail("bad \\u escape");
        }
        return v;
    }
    py::str string() {
        expect('"');
        std::u32string out;
        while (true) {
            if (i_ >= s_.size()) fail("unterminated string");
            char32_t c = s_[i_++];
            if (c == '"') break;
            if (c != '\\') {
                out.push_back(c);
                continue;
            }
            if (i_ >= s_.size()) fail("unterminated string");
            char32_t e = s_[i_++];
            switch (e) {
                case '"': out.push_back('"'); break;
                case '\\': out.push_back('\\'); break;
                case '/': out.push_back('/'); break;
                case 'b': out.push_back('\b'); break;
                case 'f': out.push_back('\f'); break;
                case 'n': out.push_back('\n'); break;
                case 'r': out.push_back('\r'); break;
                case 't': out.push_back('\t'); break;
                case 'u': {
                    unsigned u = hex4();
                    if (u >= 0xD800 && u < 0xDC00 && i_ + 1 < s_.size() && s_[i_] == '\\' && s_[i_ + 1] == 'u') {
                        i_ += 2;
                        unsigned lo = hex4();
                        u = 0x10000 + ((u - 0xD800) << 10) + (lo - 0xDC00);
                    }
                    out.push_back(static_cast<char32_t>(u));
                    break;
                }
                default: fail("bad escape");
            }
        }
        return py::str(out);
    }
    Json value() {
        space();
        if (i_ >= s_.size()) fail("unexpected end");
        Json v;
        if (s_[i_] == '"') {
            v.kind = Json::String;
            v.text = string();
        } else if (s_[i_] == '[') {
            ++i_;
            v.kind = Json::Array;
            space();
            if (i_ < s_.size() && s_[i_] == ']') {
                ++i_;
                return v;
            }
            while (true) {
                v.items.push_back(value());
                space();
                if (i_ < s_.size() && s_[i_] == ',') {
                    ++i_;
                    continue;
                }
                expect(']');
                break;
            }
        } else if (s_[i_] == '{') {
            ++i_;
            v.kind = Json::Object;
            space();
            if (i_ < s_.size() && s_[i_] == '}') {
                ++i_;
                return v;
            }
            while (true) {
                space();
                py::str k = string();
                expect(':');
                v.members.emplace_back(k, value());
                space();
                if (i_ < s_.size() && s_[i_] == ',') {
                    ++i_;
                    continue;
                }
                expect('}');
                break;
            }
        } else {
            fail("a value the table does not use");
        }
        return v;
    }
};

const Json& member(const Json& obj, const char* key) {
    for (const auto& kv : obj.members) {
        if (kv.first == key) return kv.second;
    }
    throw py::ValueError(std::string("dos_interrupts.json: missing ") + key);
}
const Json* find_member(const Json& obj, const char* key) {
    for (auto it = obj.members.rbegin(); it != obj.members.rend(); ++it) {   // (a repeated key: the last one counts)
        if (it->first == key) return &it->second;
    }
    return nullptr;
}
const py::str& as_string(const Json& v) {
    if (v.kind != Json::String) throw py::ValueError("dos_interrupts.json: a string was expected");
    return v.text;
}
const Json& as_kind(const Json& v, Json::Kind k) {
    if (v.kind != k) throw py::ValueError("dos_interrupts.json: unexpected shape");
    return v;
}
Hit as_hit(const Json& v) {
    as_kind(v, Json::Array);
    if (v.items.size() != 2) throw py::ValueError("dos_interrupts.json: a service is [name, arguments]");
    return {as_string(v.items[0]), as_string(v.items[1])};
}
py::dict<py::str, Hit> as_services(const Json& v) {
    py::dict<py::str, Hit> out;
    for (const auto& kv : as_kind(v, Json::Object).members) out.set(kv.first, as_hit(kv.second));
    return out;
}

std::optional<py::dict<int64_t, Entry>> _TABLE;   // the table, read once

}  // namespace

Service::Service(const py::str& name_, const py::str& arguments_, bool exact_) : name(name_), arguments(arguments_), exact(exact_) {}

py::str Service::__repr__() const {
    return py::percent("Service(%s%s)", name, exact ? "" : ", not exact");
}

py::dict<int64_t, Entry> load_table(const py::str& path) {
    py::file f = py::open_rb(path);
    py::str text = f.read().decode("utf-8");
    f.close();
    Json doc = Reader(text).document();
    py::dict<int64_t, Entry> out;
    for (const auto& kv : as_kind(doc, Json::Object).members) {
        const Json& row = as_kind(kv.second, Json::Object);
        Entry e;
        e.name = as_string(member(row, "name"));
        if (const Json* a = find_member(row, "arguments")) e.arguments = as_string(*a);
        if (const Json* s = find_member(row, "services")) {
            e.has_services = true;
            e.services = as_services(*s);
        }
        if (const Json* s = find_member(row, "select")) e.select = as_string(*s);
        if (const Json* a = find_member(row, "al")) {
            for (const auto& ak : as_kind(*a, Json::Object).members) e.al.set(ak.first, as_services(ak.second));
        }
        out.set(py::int_from_str(kv.first, 16), e);
    }
    return out;
}

py::dict<int64_t, Entry> table() {
    if (!_TABLE) {
        _TABLE = load_table(py::os::path::isabs(TABLE_PATH) ? TABLE_PATH : py::os::path::join(py::settings::checkout_root(), TABLE_PATH));   // (the Python reads it beside the module: from the checkout)
    }
    return *_TABLE;
}

Service name(int64_t number, std::optional<int64_t> ah, std::optional<int64_t> al) {
    py::dict<int64_t, Entry> t = table();
    if (!t.contains(number)) {
        return Service(py::percent("int_%02Xh", number), "", false);
    }
    Entry entry = t.at(number);
    py::str group = entry.name;
    if (!entry.has_services) {
        return Service(group, entry.arguments, true);
    }
    if (entry.select && *entry.select == "AX") {   // the mouse: the number is all of AX
        if (ah && *ah == 0) {
            ah = al;
        } else {
            ah = std::nullopt;
        }
    }
    if (!ah) {
        return Service(group, "service number not a constant here", false);
    }
    std::optional<Hit> sub;
    if (al) {
        py::str ah_key = py::percent("%02X", *ah);
        if (entry.al.contains(ah_key)) {
            py::dict<py::str, Hit> by_al = entry.al.at(ah_key);
            py::str al_key = py::percent("%02X", *al);
            if (by_al.contains(al_key)) sub = by_al.at(al_key);
        }
    }
    std::optional<Hit> hit = sub;
    if (!hit) {
        py::str ah_key = py::percent("%02X", *ah);
        if (entry.services.contains(ah_key)) hit = entry.services.at(ah_key);
    }
    if (!hit) {
        return Service(py::percent("%s_AH_%02Xh", group, *ah), "", false);
    }
    return Service(py::percent("%s_%s", group, std::get<0>(*hit)), std::get<1>(*hit), true);
}

py::str describe(const Interrupt& interrupt) {
    Service s = name(interrupt.number, interrupt.ah, interrupt.al);
    py::str regs;
    if (interrupt.ah) regs += py::percent(" %s=%02Xh", "AH", *interrupt.ah);
    if (interrupt.al) regs += py::percent(" %s=%02Xh", "AL", *interrupt.al);
    py::str args = s.arguments.empty() ? py::str("") : py::percent("(%s)", s.arguments);
    return py::percent("int %02Xh%s  %s%s", interrupt.number, regs, s.name, args);
}

static const py::re::Pattern& _swi() {
    static const py::re::Pattern pattern = py::re::compile(R"(\bswi\(0x([0-9a-fA-F]+)\))");   // _SWI
    return pattern;
}

py::str annotate(const py::str& text, const py::list<Interrupt>& interrupts) {
    if (interrupts.empty()) {
        return text;
    }
    py::dict<int64_t, py::set<py::str>> by_number;
    for (const Interrupt& i : interrupts) {
        by_number.setdefault(i.number, py::set<py::str>()).add(name(i.number, i.ah, i.al).name);
    }
    auto inline_ = [&](const py::re::Match& m) -> py::str {
        py::set<py::str> names = by_number.get(py::int_from_str(m.group(1), 16), py::set<py::str>());
        return py::len(names) == 1 ? py::percent("%s /* %s */", m.group(0), *names.begin()) : m.group(0);
    };
    py::list<py::str> lines = {"/* interrupt calls, in address order:"};
    for (const Interrupt& i : interrupts) {
        lines.append(py::percent("     %05X  %s", i.address, describe(i)));
    }
    return py::join("\n", lines) + " */\n" + _swi().sub(inline_, text);
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        py::dict<int64_t, Entry> t = table();
        int64_t services = 0;
        for (const Entry& e : t.values()) {
            int64_t n = py::len(e.services);
            for (const auto& x : e.al.values()) n += py::len(x);
            services += n ? n : 1;
        }
        py::print(py::percent("%d interrupts, %d services", py::len(t), services));
        return 0;
    }
    py::list<int64_t> numbers;
    for (int64_t k = 0; k < py::min<int64_t>(3, py::len(argv)); ++k) numbers.append(py::int_from_str(argv[k], 16));
    Service s = name(numbers[0], numbers.size() > 1 ? std::optional<int64_t>(numbers[1]) : std::nullopt,
                     numbers.size() > 2 ? std::optional<int64_t>(numbers[2]) : std::nullopt);
    py::print(py::percent("%s%s%s", s.name, s.arguments.empty() ? py::str("") : py::percent("(%s)", s.arguments),
                          s.exact ? "" : "   [not exact]"));
    return 0;
}

}  // namespace codebreaker::symbols::dos_interrupts

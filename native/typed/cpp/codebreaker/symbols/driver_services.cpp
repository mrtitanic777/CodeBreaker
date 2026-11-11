// codebreaker/symbols/driver_services.cpp -- typed C++ of codebreaker.symbols.driver_services (see driver_services.hpp).

#include "codebreaker/symbols/driver_services.hpp"

#include <algorithm>
#include <map>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/le.hpp"
#include "py_json.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::driver_services {

namespace {

namespace ks = knowledge::sources;
using Json = ks::Json;

// the base's store, as far as sources.authored asks of it: a kind's rows, made again when the file's stamp is new
class MemoryBase : public ks::KnowledgeBase {
  public:
    ks::Rows ensure(const py::str& platform, const py::str& kind, const py::str& stamp, const std::function<ks::Rows()>& make,
                    const py::str&) override {
        auto key = std::make_pair(platform.utf8(), kind.utf8());
        auto it = cache_.find(key);
        if (it != cache_.end() && it->second.first == stamp.utf8()) return it->second.second;
        ks::Rows rows = make();
        cache_[key] = {stamp.utf8(), rows};
        return rows;
    }
    void forget_ensured(const py::str& platform, const py::str& kind) override { cache_.erase(std::make_pair(platform.utf8(), kind.utf8())); }

  private:
    std::map<std::pair<std::string, std::string>, std::pair<std::string, ks::Rows>> cache_;
};

// the process's knowledge base, when the program has set none: the in-memory stand-in above
void ensure_store() {
    if (!ks::store_open()) {
        static const std::shared_ptr<ks::KnowledgeBase> base = std::make_shared<MemoryBase>();
        ks::store_open() = [] { return base; };
    }
}

py::re::Pattern re_(const char* pattern, int64_t flags = 0) { return py::re::compile(py::str(pattern), flags); }

const py::re::Pattern& BEGIN() { static const py::re::Pattern p = re_(R"(^\s*Begin_Service_Table\s+(\w+))", py::re::I); return p; }
const py::re::Pattern& END() { static const py::re::Pattern p = re_(R"(^\s*End_Service_Table\b)", py::re::I); return p; }
const py::re::Pattern& ID_ASM() { static const py::re::Pattern p = re_(R"(^\s*(\w+?)_Device_ID\s+(?:equ|=)\s+([^;]+))", py::re::I); return p; }
const py::re::Pattern& ID_C() { static const py::re::Pattern p = re_(R"(^\s*#define\s+(\w+?)_Device_ID\s+(\w+))", py::re::I); return p; }
const py::re::Pattern& EQU() { static const py::re::Pattern p = re_(R"(^\s*(\w+)\s+(?:equ|=)\s+([0-9][0-9A-Fa-f]*h?)\s*(?:;.*)?$)", py::re::I); return p; }

std::string hex_pad(int64_t v, int width) {   // "%0*X" of Python (the width counts a minus sign)
    char b[40];
    std::snprintf(b, sizeof b, "%llX", static_cast<unsigned long long>(v < 0 ? -v : v));
    std::string digits = b;
    std::string sign = v < 0 ? "-" : "";
    while (static_cast<int>(sign.size() + digits.size()) < width) digits = "0" + digits;
    return sign + digits;
}

int64_t hex_key(const py::str& k) { return py::int_parse(k, 16); }

py::str name_of(const Json& j) {
    if (j.kind != Json::Str) throw py::TypeError("a name that is not text in the driver table");
    return j.s;
}

py::dict<int64_t, py::str> names_table(const ks::Rows& rows) {
    py::dict<int64_t, py::str> out;
    for (const auto& kv : rows.items()) out.set(hex_key(std::get<0>(kv)), name_of(std::get<1>(kv)));
    return out;
}

}  // namespace

py::str TABLE() { return py::os::path::join(ks::_SYMBOLS(), py::str("driver_services.json")); }

std::function<ks::Rows(const py::str&, const py::str&)>& knowledge_table() {
    static std::function<ks::Rows(const py::str&, const py::str&)> t = [](const py::str& platform, const py::str& kind) {
        ensure_store();
        return ks::authored(platform, kind);
    };
    return t;
}

std::optional<Table>& _loaded() {
    static std::optional<Table> t;
    return t;
}

const Table& _table() {
    if (!_loaded()) {
        Table t;
        auto& table = knowledge_table();
        t.devices = names_table(table(py::str("vxd"), py::str("device")));
        for (const auto& kv : table(py::str("vxd"), py::str("service_table")).items()) {
            py::list<py::str> names;
            const Json& v = std::get<1>(kv);
            if (v.kind != Json::List) throw py::TypeError("a service table that is not a list");
            for (const Json& n : v.list) names.append(name_of(n));
            t.services.set(hex_key(std::get<0>(kv)), names);
        }
        t.drivers = names_table(table(py::str("vxd"), py::str("driver")));
        t.messages = names_table(table(py::str("vxd"), py::str("message")));
        _loaded() = t;
    }
    return *_loaded();
}

std::optional<py::str> device_name(int64_t device) {
    const Table& t = _table();
    std::optional<py::str> kit = t.devices.get(device);
    if (kit && !kit->empty()) return kit;
    return t.drivers.get(device);
}

std::optional<py::str> service_name(int64_t device, int64_t service) {
    const Table& t = _table();
    std::optional<py::list<py::str>> names = t.services.get(device);
    int64_t n = service & 0x7FFF;
    if (names && n < names->size() && !(*names)[n].empty()) return (*names)[n];
    return std::nullopt;
}

py::str label(int64_t device, int64_t service) {
    std::optional<py::str> real = service_name(device, service);
    if (real && !real->empty()) return *real;
    int64_t n = service & 0x7FFF;
    std::optional<py::str> dev = device_name(device);
    if (dev && !dev->empty()) return py::str(py::re::sub(py::str(R"(\W)"), py::str("_"), *dev).utf8() + "_Service_" + hex_pad(n, 4));
    return device == 1 ? py::str("VMM_" + hex_pad(n, 4)) : py::str("VxD_" + hex_pad(device, 4) + "_" + hex_pad(n, 4));
}

py::dict<int64_t, py::str> messages() { return _table().messages.copy(); }

std::optional<py::str> message_name(int64_t number) { return _table().messages.get(number); }

std::tuple<int64_t, int64_t, int64_t> size() {
    const Table& t = _table();
    int64_t named = 0;
    for (const auto& kv : t.services.items())
        for (const py::str& n : std::get<1>(kv))
            if (!n.empty()) ++named;
    py::set<int64_t> numbers;
    for (int64_t k : t.devices.keys()) numbers.add(k);
    for (int64_t k : t.drivers.keys()) numbers.add(k);
    return std::make_tuple(t.services.size(), named, numbers.size());
}

// ---- Making the table
py::str include_dir() {
    std::vector<py::str> cands;
    auto kit = py::os::environ_::get(py::str("CB_DRIVER_KIT"));
    cands.push_back(kit ? *kit : py::str(""));
    py::str ref = py::os::path::join(base::corpus::design_dir(), py::str("reference"));
    cands.push_back(py::os::path::join(py::os::path::join(ref, py::str("Microsoft Windows 95 DDK")), py::str("INC32")));
    cands.push_back(py::os::path::join(py::os::path::join(ref, py::str("win95ddk")), py::str("INC32")));
    for (const py::str& c : cands)
        if (!c.empty() && py::os::path::isfile(py::os::path::join(c, py::str("VMM.INC")))) return c;
    return py::str("");
}

std::optional<int64_t> _number(const py::str& text, const py::dict<py::str, int64_t>& known) {
    static const py::re::Pattern hex0x = re_("0x([0-9A-Fa-f]+)");
    static const py::re::Pattern hexh = re_("([0-9][0-9A-Fa-f]*)h", py::re::I);
    int64_t total = 0;
    for (py::str part : py::split(text.replace(py::str("("), py::str(" ")).replace(py::str(")"), py::str(" ")), py::str("+"))) {
        part = part.strip();
        std::optional<py::re::Match> m = hex0x.fullmatch(part);
        if (!m) m = hexh.fullmatch(part);
        if (m) total += py::int_parse(m->group(1), 16);
        else if (part.isdigit()) total += py::int_parse(part, 10);
        else if (known.contains(part.upper())) total += known.at(part.upper());
        else return std::nullopt;
    }
    return total;
}

std::tuple<py::dict<int64_t, py::str>, py::dict<int64_t, py::list<py::str>>> read_include_files(const py::str& folder) {
    auto upper_ends = [](const py::str& n, const char* suffix) { return n.upper().endswith(py::str(suffix)); };
    std::vector<py::str> files;
    for (const py::str& n : py::os::listdir(folder)) files.push_back(n);
    // sorted(key=lambda n: (not n.upper().endswith(".INC"), n.upper())) -- stable
    std::stable_sort(files.begin(), files.end(), [&](const py::str& a, const py::str& b) {
        bool na = !upper_ends(a, ".INC"), nb = !upper_ends(b, ".INC");
        if (na != nb) return !na && nb;
        return a.upper().u32() < b.upper().u32();
    });
    std::vector<std::pair<py::str, py::list<py::str>>> texts;
    for (const py::str& fn : files)
        if (upper_ends(fn, ".INC") || upper_ends(fn, ".H")) {
            py::file f = py::open(py::os::path::join(folder, fn), py::str("r"), py::str("latin1"));
            texts.emplace_back(fn, py::splitlines(f.read_text()));
            f.close();
        }
    py::dict<py::str, int64_t> equates;
    py::dict<py::str, py::str> pending, spelled, first;
    for (const auto& ft : texts) {
        const py::str& fn = ft.first;
        for (const py::str& line : ft.second) {
            std::optional<py::re::Match> m = ID_ASM().match(line);
            if (!m) m = ID_C().match(line);
            if (m) {
                pending.setdefault(m->group(1).upper(), m->group(2).strip());
                spelled.setdefault(m->group(1).upper(), m->group(1));
                first.setdefault(m->group(1).upper(), fn.upper());
                continue;
            }
            m = EQU().match(line);
            if (m) {
                std::optional<int64_t> v = _number(m->group(2), py::dict<py::str, int64_t>());
                if (v) equates.setdefault(m->group(1).upper(), *v);
            }
        }
    }
    py::dict<py::str, int64_t> ids;
    for (int round = 0; round < 4; ++round) {          // a number may be another device's name
        py::dict<py::str, int64_t> known = equates.copy();
        for (const auto& kv : ids.items()) known.set(std::get<0>(kv) + py::str("_DEVICE_ID"), std::get<1>(kv));
        for (const auto& kv : pending.items()) {
            const py::str& name = std::get<0>(kv);
            if (!ids.contains(name)) {
                std::optional<int64_t> v = _number(std::get<1>(kv), known);
                if (v) ids.set(name, *v);
            }
        }
    }
    // tables: {table name upper: {file: [service names]}}, in the order met
    py::dict<py::str, py::dict<py::str, py::list<py::str>>> tables;
    for (const auto& ft : texts) {
        const py::str& fn = ft.first;
        std::optional<py::str> cur;
        std::optional<py::re::Pattern> macro;
        for (const py::str& line : ft.second) {
            py::str code = py::split(line, py::str(";"), 1)[0];
            std::optional<py::re::Match> m = BEGIN().match(code);
            if (m && !code.upper().contains(py::str("MACRO"))) {
                cur = m->group(1);
                macro = py::re::compile(py::str("^\\s*") + py::re::escape(*cur) + py::str("_Service\\s+(\\w+)"), py::re::I);
                py::dict<py::str, py::list<py::str>> by_file = tables.setdefault(cur->upper(), py::dict<py::str, py::list<py::str>>());
                by_file.setdefault(fn, py::list<py::str>());
                continue;
            }
            if (cur && END().match(code)) cur.reset();
            else if (cur) {
                m = macro->match(code);
                if (m) tables.at(cur->upper()).at(fn).append(m->group(1));
            }
        }
    }
    py::dict<int64_t, py::str> devices;
    py::dict<int64_t, py::list<py::str>> services;
    std::vector<py::str> names = ids.keys();
    std::stable_sort(names.begin(), names.end(), [&](const py::str& a, const py::str& b) {
        bool na = !(first.at(a) == py::str("VMM.INC")), nb = !(first.at(b) == py::str("VMM.INC"));
        if (na != nb) return !na && nb;
        const std::u32string &fa = first.at(a).u32(), &fb = first.at(b).u32();
        if (fa != fb) return fa < fb;
        return a.u32() < b.u32();
    });
    for (const py::str& name : names)
        if (0 < ids.at(name) && ids.at(name) < 0x10000) devices.setdefault(ids.at(name), spelled.at(name));   // several names, one number: the kit's main list names it
    for (const auto& kv : tables.items()) {
        const py::str& name = std::get<0>(kv);
        std::optional<int64_t> number = ids.get(name);
        std::optional<py::list<py::str>> best;
        for (const auto& fl : std::get<1>(kv).items())                  // max(by_file.values(), key=len): the first longest
            if (!best || std::get<1>(fl).size() > best->size()) best = std::get<1>(fl);
        if (!number || best->empty()) continue;
        std::optional<py::list<py::str>> have = services.get(*number);
        if (best->size() > (have ? have->size() : 0)) {
            services.set(*number, *best);
            std::optional<py::str> sp = spelled.get(name);
            devices.set(*number, sp ? *sp : name);
        }
    }
    return std::make_tuple(devices, services);
}

py::dict<int64_t, py::str> read_messages(const py::str& folder) {
    static const py::re::Pattern line_re = re_(R"(^(\w+)\s+EQU\s+(00[0-9A-F]{2})H\s*$)", py::re::I);
    py::dict<int64_t, py::str> out;
    bool inside = false;
    py::file f = py::open(py::os::path::join(folder, py::str("VMM.INC")), py::str("r"), py::str("latin1"));
    py::str text = f.read_text();
    f.close();
    const std::u32string& u = text.u32();
    size_t i = 0;
    while (i < u.size()) {                                          // for line in f: each line keeps its "\n"
        size_t j = u.find(U'\n', i);
        size_t stop = j == std::u32string::npos ? u.size() : j + 1;
        py::str line{u.substr(i, stop - i)};
        i = stop;
        std::optional<py::re::Match> m = line_re.match(line);
        if (m && m->group(1).upper() == py::str("SYS_CRITICAL_INIT")) inside = true;
        if (m && m->group(1).upper() == py::str("MAX_SYSTEM_CONTROL")) break;
        if (m && inside) out.setdefault(py::int_parse(m->group(2), 16), m->group(1).upper());
    }
    return out;
}

py::dict<int64_t, py::str> corpus_drivers() {
    py::dict<int64_t, py::str> out;
    for (const py::str& path : formats::le::corpus_files()) {
        try {
            for (const auto& member : formats::le::members(path)) {
                std::optional<formats::le::Descriptor> d = formats::le::descriptor(std::get<1>(member));
                if (d && d->device_id && !d->name.empty()) out.setdefault(d->device_id, d->name);
            }
        } catch (const py::Exception&) {
            continue;
        }
    }
    return out;
}

namespace {
// json.dump(...)'s text for {"key": {"0001": value, ...}} (indent=1)
template <class V, class Put>
void put_table(std::string& o, const py::dict<int64_t, V>& d, const Put& put) {
    std::vector<int64_t> keys = d.keys();
    std::sort(keys.begin(), keys.end());
    if (keys.empty()) {
        o += "{}";
        return;
    }
    o += "{\n";
    for (size_t i = 0; i < keys.size(); ++i) {
        o += "  ";
        py::json::put(o, py::str(hex_pad(keys[i], 4)));
        o += ": ";
        put(o, d.at(keys[i]));
        o += i + 1 < keys.size() ? ",\n" : "\n";
    }
    o += " }";
}
}  // namespace

std::tuple<int64_t, int64_t, int64_t> generate(const std::optional<py::str>& folder_, const std::optional<py::str>& out_) {
    py::str folder = folder_ && !folder_->empty() ? *folder_ : include_dir();
    py::str out = out_ ? *out_ : TABLE();
    if (folder.empty()) throw py::SystemExit(py::str("the driver kit's include files were not found; set CB_DRIVER_KIT to its INC32 folder"));
    auto [devices, services] = read_include_files(folder);
    py::dict<int64_t, py::str> drivers;
    for (const auto& kv : corpus_drivers().items())
        if (!devices.contains(std::get<0>(kv))) drivers.set(std::get<0>(kv), std::get<1>(kv));
    {
        py::file f = py::open(out, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));      // (opened, so truncated, before the rest is read)
        std::string o = "{\n \"source\": ";
        py::json::put(o, py::str("Windows 95 driver kit include files (INC32): Begin_Service_Table lists and "
                                 "*_Device_ID equates; \"drivers\" from the corpus drivers' own descriptors"));
        auto put_name = [](std::string& s, const py::str& v) { py::json::put(s, v); };
        auto put_names = [](std::string& s, const py::list<py::str>& v) {
            if (v.empty()) {
                s += "[]";
                return;
            }
            s += "[\n";
            for (int64_t i = 0; i < v.size(); ++i) {
                s += "   ";
                py::json::put(s, v[i]);
                s += i + 1 < v.size() ? ",\n" : "\n";
            }
            s += "  ]";
        };
        o += ",\n \"devices\": ";
        put_table(o, devices, put_name);
        o += ",\n \"services\": ";
        put_table(o, services, put_names);
        o += ",\n \"drivers\": ";
        put_table(o, drivers, put_name);
        o += ",\n \"messages\": ";
        put_table(o, read_messages(folder), put_name);
        o += "\n}";
        f.write(py::str(o));
        f.write(py::str("\n"));
        f.close();
    }
    ensure_store();
    ks::changed(out);                                               // the knowledge base loads it again
    _loaded().reset();
    return size();
}

std::tuple<int64_t, int64_t, py::dict<std::tuple<int64_t, int64_t>, int64_t>> coverage(const py::str& path) {
    int64_t calls = 0, named = 0;
    py::dict<std::tuple<int64_t, int64_t>, int64_t> missing;
    for (const auto& member : formats::le::members(path)) {
        for (const auto& c : formats::le::service_calls(std::get<1>(member))) {
            ++calls;
            std::optional<py::str> nm = service_name(std::get<2>(c), std::get<3>(c));
            if (nm && !nm->empty()) ++named;
            else {
                auto key = std::make_tuple(std::get<2>(c), std::get<3>(c) & 0x7FFF);
                missing.set(key, missing.get(key, 0) + 1);
            }
        }
    }
    return std::make_tuple(calls, named, missing);
}

int64_t main(const py::list<py::str>& argv) {
    if (!argv.empty() && argv[0] == py::str("--generate")) {
        auto [tables, names, devices] = generate(argv.size() > 1 ? std::optional<py::str>(argv[1]) : std::nullopt);
        py::print(TABLE() + py::str(": " + std::to_string(tables) + " service tables, " + std::to_string(names) + " services, " +
                                    std::to_string(devices) + " device numbers"));
        return 0;
    }
    auto [tables, names, devices] = size();
    py::print(py::str(std::to_string(tables) + " service tables, " + std::to_string(names) + " services named, " + std::to_string(devices) +
                      " device numbers named"));
    for (const py::str& arg : argv) {
        std::optional<py::str> path = base::corpus::retail(arg);
        if (!path || path->empty()) {
            py::print(arg + py::str(": not found"));
            continue;
        }
        auto [calls, named, missing] = coverage(*path);
        auto items = missing.items();
        std::sort(items.begin(), items.end());                      // sorted(missing.items())
        std::string tail;
        for (size_t i = 0; i < items.size() && i < 6; ++i) {
            const auto& it = items[i];
            tail += "  " + label(std::get<0>(std::get<0>(it)), std::get<1>(std::get<0>(it))).utf8() + " x" + std::to_string(std::get<1>(it));
        }
        char counts[80];
        std::snprintf(counts, sizeof counts, " %4lld service calls, %4lld named", static_cast<long long>(calls), static_cast<long long>(named));
        py::print(py::os::path::basename(*path).ljust(16) + py::str(counts) + py::str(tail));
    }
    return 0;
}

}  // namespace codebreaker::symbols::driver_services

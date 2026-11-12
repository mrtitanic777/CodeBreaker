// codebreaker/symbols/driver_services.cpp -- the shim's translation unit (see driver_services.hpp). The new module
// (cb/symbols/driver_services) holds the table and reads the kit's include files; this file gives it the knowledge base's rows
// (through the old knowledge reader) and the corpus's drivers (through the old LE reader), and converts types.

#include "codebreaker/symbols/driver_services.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/driver_services.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/le.hpp"
#include "py_json.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::driver_services {

namespace {

namespace ks = knowledge::sources;
namespace nds = cb::symbols::driver_services;
using Json = ks::Json;
using cb::compat::from_py;
using cb::compat::to_py;

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

// The rows the new module reads, made from the table the old layer loaded.
cb::knowledge::Rows rows_of(const py::dict<int64_t, py::str>& names) {
    cb::knowledge::Rows rows;
    for (const auto& kv : names.items()) rows.insert_or_assign(std::format("{:04X}", static_cast<std::uint32_t>(std::get<0>(kv))), cb::knowledge::Value(from_py(std::get<1>(kv))));
    return rows;
}

cb::knowledge::Rows rows_of(const py::dict<int64_t, py::list<py::str>>& lists) {
    cb::knowledge::Rows rows;
    for (const auto& kv : lists.items()) {
        cb::knowledge::Value::List names;
        for (const py::str& n : std::get<1>(kv)) names.emplace_back(from_py(n));
        rows.insert_or_assign(std::format("{:04X}", static_cast<std::uint32_t>(std::get<0>(kv))), cb::knowledge::Value(std::move(names)));
    }
    return rows;
}

// the table the new module reads, rebuilt with `_loaded()`
std::optional<nds::Table>& loaded_new() {
    static std::optional<nds::Table> t;
    return t;
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
        loaded_new() = cb::compat::guard([&] { return nds::Table::from_rows(rows_of(t.devices), rows_of(t.services), rows_of(t.drivers), rows_of(t.messages)); });
        _loaded() = t;
    }
    return *_loaded();
}

namespace {
const nds::Table& table_() {
    _table();
    return *loaded_new();
}
std::optional<py::str> to_old(const std::optional<std::string>& v) { return v ? std::optional<py::str>(to_py(*v)) : std::nullopt; }
std::optional<std::uint32_t> as_number(int64_t v) { return v >= 0 && v <= 0xFFFFFFFFLL ? std::optional<std::uint32_t>(static_cast<std::uint32_t>(v)) : std::nullopt; }
}  // namespace

std::optional<py::str> device_name(int64_t device) {
    const auto n = as_number(device);
    return n ? to_old(table_().device_name(*n)) : std::nullopt;
}

std::optional<py::str> service_name(int64_t device, int64_t service) {
    const auto d = as_number(device);
    const auto s = as_number(service & 0xFFFFFFFFLL);
    return d && s ? to_old(table_().service_name(*d, *s)) : std::nullopt;
}

py::str label(int64_t device, int64_t service) {
    const auto d = as_number(device);
    return to_py(table_().label(d.value_or(0), static_cast<std::uint32_t>(service & 0xFFFFFFFFLL)));
}

py::dict<int64_t, py::str> messages() { return _table().messages.copy(); }

std::optional<py::str> message_name(int64_t number) {
    const auto n = as_number(number);
    return n ? to_old(table_().message_name(*n)) : std::nullopt;
}

std::tuple<int64_t, int64_t, int64_t> size() {
    const nds::Table::Size s = table_().size();
    return std::make_tuple(static_cast<int64_t>(s.service_tables), static_cast<int64_t>(s.services_named), static_cast<int64_t>(s.devices_named));
}

// ---- Making the table
py::str include_dir() {
    auto kit = py::os::environ_::get(py::str("CB_DRIVER_KIT"));
    const std::filesystem::path found =
        nds::find_include_dir(kit ? from_py(*kit) : std::string(), cb::fs::from_text(from_py(py::os::path::join(base::corpus::design_dir(), py::str("reference")))));
    return found.empty() ? py::str("") : py::str(found.string());
}

std::optional<int64_t> _number(const py::str& text, const py::dict<py::str, int64_t>& known) {
    std::map<std::string, std::uint32_t> names;
    for (const auto& kv : known.items()) names[from_py(std::get<0>(kv))] = static_cast<std::uint32_t>(std::get<1>(kv));
    const auto n = nds::parse_device_number(from_py(text), names);
    return n ? std::optional<int64_t>(*n) : std::nullopt;
}

std::tuple<py::dict<int64_t, py::str>, py::dict<int64_t, py::list<py::str>>> read_include_files(const py::str& folder) {
    const nds::KitTables kit = cb::compat::guard([&] { return nds::read_include_files(cb::fs::from_text(from_py(folder))); });
    py::dict<int64_t, py::str> devices;
    py::dict<int64_t, py::list<py::str>> services;
    for (const auto& [n, name] : kit.devices) devices.set(n, to_py(name));
    for (const auto& [n, names] : kit.services) {
        py::list<py::str> list;
        for (const std::string& s : names) list.append(to_py(s));
        services.set(n, list);
    }
    return std::make_tuple(devices, services);
}

py::dict<int64_t, py::str> read_messages(const py::str& folder) {
    const auto found = cb::compat::guard([&] { return nds::read_messages(cb::fs::from_text(from_py(folder))); });
    py::dict<int64_t, py::str> out;
    for (const auto& [n, name] : found) out.set(n, to_py(name));
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

std::tuple<int64_t, int64_t, int64_t> generate(const std::optional<py::str>& folder_, const std::optional<py::str>& out_) {
    py::str folder = folder_ && !folder_->empty() ? *folder_ : include_dir();
    py::str out = out_ ? *out_ : TABLE();
    if (folder.empty()) throw py::SystemExit(py::str("the driver kit's include files were not found; set CB_DRIVER_KIT to its INC32 folder"));
    std::map<std::uint32_t, std::string> drivers;
    for (const auto& kv : corpus_drivers().items()) drivers[static_cast<std::uint32_t>(std::get<0>(kv))] = from_py(std::get<1>(kv));
    cb::compat::guard([&] {
        const std::filesystem::path kit_folder = cb::fs::from_text(from_py(folder));
        const nds::KitTables kit = nds::read_include_files(kit_folder);
        cb::fs::write_text(cb::fs::from_text(from_py(out)), nds::format_table(kit, drivers, nds::read_messages(kit_folder)));
    });
    ensure_store();
    ks::changed(out);                                               // the knowledge base loads it again
    _loaded().reset();
    return size();
}

std::tuple<int64_t, int64_t, py::dict<std::tuple<int64_t, int64_t>, int64_t>> coverage(const py::str& path) {
    std::vector<nds::ServiceCall> calls;
    for (const auto& member : formats::le::members(path)) {
        for (const auto& c : formats::le::service_calls(std::get<1>(member)))
            calls.push_back({static_cast<std::uint32_t>(std::get<2>(c)), static_cast<std::uint32_t>(std::get<3>(c))});
    }
    const nds::Coverage found = nds::coverage(table_(), calls);
    py::dict<std::tuple<int64_t, int64_t>, int64_t> missing;
    for (const auto& [key, n] : found.missing) missing.set(std::make_tuple(static_cast<int64_t>(key.first), static_cast<int64_t>(key.second)), static_cast<int64_t>(n));
    return std::make_tuple(static_cast<int64_t>(found.calls), static_cast<int64_t>(found.named), missing);
}

int64_t main(const py::list<py::str>& argv) {
    if (!argv.empty() && argv[0] == py::str("--generate")) {
        auto [tables, names, devices] = generate(argv.size() > 1 ? std::optional<py::str>(argv[1]) : std::nullopt);
        py::print(TABLE() + py::str(": " + std::to_string(tables) + " service tables, " + std::to_string(names) + " services, " +
                                    std::to_string(devices) + " device numbers"));
        return 0;
    }
    py::print(to_py(nds::size_line(table_().size())));
    for (const py::str& arg : argv) {
        std::optional<py::str> path = base::corpus::retail(arg);
        if (!path || path->empty()) {
            py::print(arg + py::str(": not found"));
            continue;
        }
        std::vector<nds::ServiceCall> calls;
        for (const auto& member : formats::le::members(*path)) {
            for (const auto& c : formats::le::service_calls(std::get<1>(member)))
                calls.push_back({static_cast<std::uint32_t>(std::get<2>(c)), static_cast<std::uint32_t>(std::get<3>(c))});
        }
        py::print(to_py(nds::coverage_line(table_(), from_py(py::os::path::basename(*path)), nds::coverage(table_(), calls))));
    }
    return 0;
}

}  // namespace codebreaker::symbols::driver_services

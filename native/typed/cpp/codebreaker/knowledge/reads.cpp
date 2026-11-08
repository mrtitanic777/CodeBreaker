// codebreaker/knowledge/reads.cpp -- typed C++ port of codebreaker.knowledge.reads (see reads.hpp).

#include "codebreaker/knowledge/reads.hpp"

#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "codebreaker/symbols/sdk_structs.hpp"
#include "codebreaker/symbols/xdk_libraries.hpp"
#include "py_os.hpp"

namespace codebreaker::knowledge::reads {

const py::str KIND("kb");        // the engine's name for an input that is a piece of the base
const py::str ABSENT("absent");

namespace {
// _CURRENT: the Reads in force in this context (contextvars.ContextVar; here, a thread-local pointer)
thread_local std::shared_ptr<Reads> t_current;
}  // namespace

// ---- Reads ----
void Reads::add(const py::str& key) {
    std::lock_guard<std::mutex> g(lock_);
    keys_.add(key);
}

void Reads::update(const py::list<py::str>& keys) {
    std::lock_guard<std::mutex> g(lock_);
    keys_.update(keys);
}

py::list<py::str> Reads::sorted_keys() const {
    std::lock_guard<std::mutex> g(lock_);
    py::list<py::str> out;
    for (const py::str& k : keys_) out.append(k);
    return py::sorted(out);
}

int64_t Reads::size() const { return keys_.size(); }

bool Reads::contains(const py::str& key) const {
    std::lock_guard<std::mutex> g(lock_);
    return keys_.contains(key);
}

py::set<py::str> Reads::keys() const { return keys_; }

// ---- recording ----
recording::recording() {
    reads_ = std::make_shared<Reads>();
    previous_ = t_current;
    t_current = reads_;
}

recording::~recording() { t_current = previous_; }

std::shared_ptr<Reads> capture() { return t_current; }

Adopt::Adopt(std::shared_ptr<Reads> reads) {
    previous_ = t_current;
    t_current = std::move(reads);
}

Adopt::~Adopt() { t_current = previous_; }

bool active() { return t_current != nullptr; }

py::str join(const py::str& platform, const py::str& kind, const py::str& key) {
    return platform + py::str(":") + kind + py::str(":") + key;
}

std::tuple<py::str, py::str, py::str> split(const py::str& read) {
    py::list<py::str> parts = py::split(read, py::str(":"), 2);
    if (parts.size() < 3) throw py::ValueError("not enough values to unpack (expected 3, got " + std::to_string(parts.size()) + ")");
    return {parts[0], parts[1], parts[2]};
}

void record(const py::str& platform, const py::str& kind, const py::str& key) {
    std::shared_ptr<Reads> r = t_current;
    if (r) r->add(join(platform, kind, key));
}

void record_kind(const py::str& platform, const py::str& kind) { record(platform, kind, py::str("*")); }

void merge(const py::list<py::str>& keys) {
    std::shared_ptr<Reads> r = t_current;
    if (r && !keys.empty()) r->update(keys);
}

// ---- what a read is now ----
namespace {

// Bring an authored or derived kind up to date with what it is made from (its file changed, the headers
// changed) before its hashes are read: a staleness check in a new process sees the piece as it would now be
// made.
void fresh(const py::str& platform, const py::str& kind) {
    if (sources::AUTHORED().contains(std::make_tuple(platform, kind))) {
        sources::authored(platform, kind);
        return;
    }
    try {
        if (kind == "prototype" && (platform == "win16" || platform == "win32")) {
            symbols::prototypes::table();
        } else if ((kind == "struct" || kind == "struct.pointer") && platform == "win32") {
            symbols::sdk_structs::_read();
        } else if (kind == "library_signatures" && platform == "xbox360") {
            py::str folder = symbols::xdk_libraries::lib_folder();
            if (!folder.empty() && py::os::path::isdir(folder)) symbols::xdk_libraries::signatures(folder);
        }
    } catch (const py::Exception&) {
    }
}

std::mutex& fresh_lock() {
    static std::mutex m;
    return m;
}
py::set<std::tuple<py::str, py::str>>& fresh_done() {
    static py::set<std::tuple<py::str, py::str>> s;     // (platform, kind) brought up to date by this process
    return s;
}

}  // namespace

py::str digest(const py::str& read) {
    auto [platform, kind, key] = reads::split(read);
    auto kb = store::open();
    (void)key;
    bool todo = false;
    if (platform != "bodies") {      // (the C++ base answers, once the kind is fresh)
        std::lock_guard<std::mutex> g(fresh_lock());
        todo = !fresh_done().contains(std::make_tuple(platform, kind));
    }
    if (todo) {
        fresh(platform, kind);
        std::lock_guard<std::mutex> g(fresh_lock());
        fresh_done().add(std::make_tuple(platform, kind));
    }
    return kb->digest(read);
}

}  // namespace codebreaker::knowledge::reads

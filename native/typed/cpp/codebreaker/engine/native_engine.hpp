// codebreaker/engine/native_engine.hpp -- typed C++ port of codebreaker.engine.native_engine.
//
// native_engine.py -- the engine and the store in C++ (native/engine: cbengine.dll), through ctypes.
//
// The shim: the program database (facts.py's Database), the engine's core (engine.py's Run and Context:
// which facts are current, the inputs recorded, what is written, kept or remade, the order and the worker
// threads), the function store (store/functions.py), the knowledge base's database (knowledge/store.py,
// reads.py's digests) and the verdict vocabulary (verify/verdicts.py) are C++; the Python module only
// hands Python's calls across, and calls back the Python that remains (a step's code, a step's version,
// a knowledge piece's hash now, what a value IS).
//
// Notes on the port:
//   * ctypes has nothing to do in C++: native/engine/*.cc and SQLite are LINKED into the program, and the
//     cbe_* functions of cbengine.h are called directly. enabled() is therefore always true, why_not()
//     None, and dll_path() only says where the DLL would be (the `engine_bin` setting). The callbacks
//     that the Python registers with cbe_set_callbacks are this file's own functions, which call the
//     steps registered in engine.hpp (std::function in place of the Python function objects).
//   * An exception a step raises inside a worker thread crosses the C engine as a number (the shim's
//     table _ERRS) and is raised again at the call that started the run, as in the Python: here it is
//     kept as a std::exception_ptr.
//   * A hint handed to a step (need_many's) is an opaque std::any. The asker's context handed to worker
//     threads (contextvars in Python) is the knowledge::reads recording in force, carried the same way.
//   * Row buffers: a field is a Cell (None, int, float, text or bytes), as the shim's pack() makes them.
//   * sqlite3 connections (Database.conn, FunctionStore.db, FunctionStore.module_db, KnowledgeBase.conn)
//     are not ported: the sqlite3 module is Python's. module_db() still makes a module's tables (what
//     _m does), then returns the path of the file for a caller that opens it itself.
//   * The sqlite3 exceptions are declared here (sqlite3_Error and the three the shim raises).
#pragma once

#include <any>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/engine/facts.hpp"
#include "py.hpp"

namespace codebreaker::engine {
struct Step;
}

namespace codebreaker::engine::native_engine {

// sqlite3.Error and the two the shim raises (IntegrityError for SQLite code 19, else OperationalError),
// and sqlite3.InterfaceError for a value pack() cannot bind.
struct sqlite3_Error : py::Exception {
    sqlite3_Error() : py::Exception() {}
    explicit sqlite3_Error(const std::string& m) : py::Exception(m) {}
    explicit sqlite3_Error(const char* m) : py::Exception(m) {}
    const char* type() const override { return "Error"; }
};
struct sqlite3_IntegrityError : sqlite3_Error {
    explicit sqlite3_IntegrityError(const std::string& m) : sqlite3_Error(m) {}
    const char* type() const override { return "IntegrityError"; }
};
struct sqlite3_OperationalError : sqlite3_Error {
    explicit sqlite3_OperationalError(const std::string& m) : sqlite3_Error(m) {}
    const char* type() const override { return "OperationalError"; }
};
struct sqlite3_InterfaceError : sqlite3_Error {
    explicit sqlite3_InterfaceError(const std::string& m) : sqlite3_Error(m) {}
    const char* type() const override { return "InterfaceError"; }
};

// Where cbengine.dll is: the `engine_bin` setting (default native/engine/build/bin).
py::str dll_path();
// The engine is linked in: true.
bool enabled();
std::optional<py::str> why_not();

// ---- row buffers ----
// A field of a row buffer: None, an int, a float, text or bytes.
using Cell = std::variant<std::monostate, int64_t, double, py::str, py::bytes>;
using Cells = std::vector<Cell>;

py::bytes pack(const Cells& values);
Cells unpack(const py::bytes& data);
// A store's or knowledge base's answer: [tuple] of its rows.
std::vector<Cells> _rows(const py::bytes& data);

// Cell accessors the callers need (a None reads as the default).
py::str cell_str(const Cell& c);                    // text (or bytes decoded); None -> ""
std::optional<py::str> cell_opt_str(const Cell& c);  // None -> nullopt
int64_t cell_int(const Cell& c);                    // int (or float truncated); None -> 0
double cell_double(const Cell& c);                  // float or int; None -> 0.0
py::bytes cell_bytes(const Cell& c);

// ---- the program database (facts.py) ----
// The steps whose other versions' facts are gone in this process (facts.Database.cleaned).
class Cleaned {
  public:
    explicit Cleaned(class Database& db) : db_(db) {}
    bool contains(const py::str& step) const;
    void add(const py::str& step);
    void clear();

  private:
    Database& db_;
};

// facts.Database on the C++ fact store (the same file, the same schema).
class Database {
  public:
    explicit Database(const py::str& path);
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    py::str path;
    std::optional<py::str> sha;       // set by facts.open_for
    Cleaned cleaned;
    void* handle() const { return h_; }

    bool native() const { return true; }

    std::unique_ptr<facts::Row> row(const py::str& kind, const py::str& key = py::str("")) const;
    py::list<facts::Row> rows(const std::optional<py::str>& kind = std::nullopt) const;
    // (kind, count, bytes, ...) of every kind: four fields each
    std::vector<Cells> kinds() const;
    py::bytes blob(const py::str& hash) const;
    std::optional<py::str> about(const py::str& name) const;
    py::str about(const py::str& name, const py::str& value) const;
    py::str write(const py::str& kind, const py::str& key, const facts::Value& value, const py::str& step,
                  const py::str& version = py::str(""), const py::list<facts::Input>& inputs = {},
                  double took = 0.0,
                  const std::optional<std::tuple<py::str, py::bytes>>& encoded = std::nullopt) const;
    void write_raw(const py::str& kind, const py::str& key, const py::str& hash, const py::str& tag,
                   const py::bytes& data, const py::str& step, const py::str& version, const py::str& inputs_json,
                   double took = 0) const;
    void remake(const py::str& kind, const py::str& key, const py::str& version,
                const py::list<facts::Input>& inputs) const;
    int64_t forget(const py::str& kind, const std::optional<py::str>& key = std::nullopt) const;
    int64_t drop_other_versions(const py::str& step, const py::str& version) const;
    void collect() const;
    void close();

  private:
    void* h_ = nullptr;
};

py::str sha(const py::str& path);
py::list<std::tuple<py::str, int64_t, double>> databases(const py::str& folder);
// The names of the databases gone, as hold() lists them.
py::list<py::str> hold(int64_t cap, const py::str& folder);

// ---- a run (engine.py's Run) and the context a step is given ----
class Run;

// engine.Context on the C++ core: what a step is given, its reads recorded as it asks.
class Context {
  public:
    Context(Run& run, const Step& step, void* p, const std::any& hint);

    Run& run;
    const Step& step;
    void* p;
    std::any hint;
    std::shared_ptr<Database> db;
    py::str key;

    py::list<facts::Input> inputs() const;
    facts::Value need(const py::str& kind, const py::str& key = py::str(""));
    py::dict<py::str, facts::Value> need_many(const py::str& kind, const py::list<py::str>& keys,
                                              const std::any& hint = {});
    std::optional<facts::Value> peek(const py::str& kind, const py::str& key = py::str(""));
    facts::Value file(const py::str& path);
    void knowledge(const py::list<py::str>& keys);
    std::optional<facts::Value> previous() const;
    py::str work(const py::list<py::str>& parts = py::list<py::str>()) const;
};

// engine.Run on the C++ core: each fact decided current or made again at most once in it.
class Run {
  public:
    Run(std::shared_ptr<Database> db, bool log_on);   // log: whether a log callback is wanted
    Run(std::shared_ptr<Database> db, std::function<void(const py::str&)> log);
    ~Run();
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;

    std::shared_ptr<Database> db;
    std::function<void(const py::str&)> log;
    void* handle() const { return p_; }

    // {kind: [kept, made]}
    py::dict<py::str, py::list<int64_t>> counts() const;
    std::optional<py::str> hash_of(const py::str& kind, const py::str& key);
    facts::Value value(const py::str& kind, const py::str& key);
    facts::Value _value(const py::str& kind, const py::str& key);
    py::list<py::str> stale(const py::str& kind, const py::list<py::str>& keys);
    py::dict<py::str, facts::Value> many(const py::str& kind, const py::list<py::str>& keys,
                                         const std::any& hint = {});

    // values made or read in this run: (kind, key) -> value (the steps write it from their threads)
    void set_value(const py::str& kind, const py::str& key, const facts::Value& v);

  private:
    void* p_ = nullptr;
    std::recursive_mutex mu_;
    py::dict<std::tuple<py::str, py::str>, facts::Value> values_;
    friend class Context;
};

// A step declared (engine.step): the C++ registry knows its kind, what it reads, batch, always.
void declared(const Step& s);

// ---- the function store (store/functions.py) ----
// One row of candidates(), judged() and best(): addr, engine, rev, sig, name, source, problem, verdict,
// reason, limits, detail, judged_sig (a SQL value each: text, an int, or None).
struct Candidate {
    Cell addr, engine, rev, sig, name, source, problem, verdict, reason, limits, detail, judged_sig;
    py::list<py::str> weak;     // best(): the weak reasons of its class
    Cell klass;                  // best(): its class
};

struct Verdict {
    Cell profile, verdict, reason, limits;
    bool carried = false;
};
struct Stored {
    Cell engine, rev, name, source, problem;
    py::list<Verdict> verdicts;
};
struct Summary {
    int64_t functions = 0, identified = 0, judged = 0;
    Cell last, last_profile;
    py::list<py::str> profiles;
    py::dict<py::str, int64_t> latest;
};
struct Variant {
    Cell addr, compiler, source, parent, knobs, distance, total, verdict, behaves;
};
struct Overview {
    py::str sha, name, kind, path;
    int64_t functions = 0, identified = 0;
    py::dict<py::str, int64_t> latest;
};
// a row of the store's answers that is a record of cells
struct ModuleRow {
    Cells fields;      // sha, name, path, kind (module_rows) / sha, name, path (others)
};

class KnowledgeBase;

// functions.FunctionStore on the C++ store (the same files, the same schema).
class FunctionStore {
  public:
    FunctionStore(const std::optional<py::str>& path = std::nullopt, bool readonly = false);
    ~FunctionStore();
    FunctionStore(const FunctionStore&) = delete;
    FunctionStore& operator=(const FunctionStore&) = delete;

    bool own_path;
    py::str path;
    bool readonly;
    py::str folder;

    void close();
    py::str database_path(const py::str& module) const;
    // A module's tables made (as _m does); the file's path, for a caller that opens it itself. In a
    // read-only store: the path if the file exists, else None.
    std::optional<py::str> module_db(const py::str& module);
    py::dict<py::str, Summary> summaries();

    // ---- writing
    void add_module(const py::str& sha, const py::str& name, const py::str& path = py::str(""),
                    const py::str& kind = py::str(""));
    void add_profile(const py::str& digest, const py::str& settings);
    void add_functions(const py::str& module, const py::list<py::str>& addrs);
    struct RecordRow {
        py::str addr;
        Cell name, source, sig;
        py::str verdict;
        Cell reason, limits, detail, problem;
    };
    void record_many(const py::str& module, const py::str& engine, const Cell& rev, const py::str& profile,
                     const py::list<RecordRow>& rows);
    void put_text(const py::str& module, const py::str& engine, const py::str& kind, const py::str& body);
    Cell get_text(const py::str& module, const py::str& engine, const py::str& kind);
    void record(const py::str& module, const py::str& addr, const py::str& engine, const Cell& rev, const Cell& sig,
                const Cell& name, const Cell& source, const py::str& profile, const py::str& verdict,
                const Cell& reason = {}, const Cell& limits = {}, const Cell& detail = {},
                const Cell& problem = {});

    // ---- reading
    py::list<Candidate> candidates(const py::str& module, const py::str& profile,
                                   const std::optional<py::str>& engine = std::nullopt);
    py::list<Stored> stored(const py::str& module, const py::str& addr);
    py::dict<py::str, Cell> built(const py::str& module, const py::str& profile);
    void set_built(const py::str& module, const py::str& profile, const py::dict<py::str, py::str>& sigs);
    py::dict<py::str, Candidate> judged(const py::str& module, const py::str& profile, const py::str& engine);
    py::dict<py::str, Candidate> best(const py::str& module, const py::str& profile,
                                      const py::list<py::str>& prefer = py::list<py::str>());
    py::dict<py::str, Cell> latest(const py::str& module);
    py::list<py::str> addresses(const py::str& module);
    py::dict<py::str, Cell> tally(const py::str& profile, const py::str& module,
                                  const py::list<py::str>& prefer = py::list<py::str>());
    void set_identified(const py::str& module,
                        const py::dict<py::str, std::tuple<py::str, py::str>>& found);
    py::dict<py::str, std::tuple<Cell, Cell>> identified(const py::str& module);
    py::dict<py::str, Cell> scores(const py::str& profile, const py::str& module,
                                   const py::list<py::str>& prefer = py::list<py::str>());

    // ---- bodies and their names: the knowledge base's
    std::shared_ptr<KnowledgeBase> knowledge();
    void set_bodies(const py::str& module, const py::dict<py::str, std::tuple<py::str, Cell, Cell>>& rows);
    py::dict<py::str, std::tuple<py::str, Cell, Cell>> bodies(const py::str& module);
    void name_body(const py::str& digest, const py::str& name, const py::str& source = py::str("named by hand"));
    py::dict<py::str, std::tuple<Cell, Cell>> names_from_bodies(const py::str& module);
    Cells shared_bodies();
    Cell modules_with_body(const py::str& digest);

    // ---- byte-match variants
    Cell record_variant(const py::str& module, const py::str& addr, const py::str& compiler, const py::str& source,
                        const Cell& distance, const Cell& total, const py::str& verdict,
                        const py::str& knobs = py::str(""), const Cell& parent = {},
                        const py::str& behaves = py::str(""));
    py::list<Variant> variants(const py::str& module, const std::optional<py::str>& compiler = std::nullopt);
    py::dict<py::str, Variant> best_variant(const py::str& module, const py::str& compiler);
    Cells match_tally(const py::str& module, const py::str& compiler);

    // ---- across modules: the index
    std::vector<Cells> modules(const std::optional<py::str>& profile = std::nullopt);
    std::vector<Cells> module_rows(const std::optional<py::str>& kind = std::nullopt);
    std::optional<py::str> module_path(const py::str& name);
    // sha -> (functions, judged, last)
    py::dict<py::str, std::tuple<int64_t, int64_t, Cell>> stamps();
    std::optional<py::str> latest_profile(const py::str& kind);
    std::vector<Cells> recent(int64_t limit = 8);
    Cell verdicts_since(double t);
    py::list<Overview> overview();

    // ---- the store from before the databases
    std::optional<py::str> _about(const py::str& name);
    void _about(const py::str& name, const py::str& value);
    // -> {name: count} of what was carried this time
    py::dict<py::str, Cell> carry(const std::function<void(const py::str&)>& log = nullptr);

    std::vector<Cells> _c(const py::str& method, const Cells& args);

  private:
    void* p_ = nullptr;
    std::shared_ptr<KnowledgeBase> knowledge_;
};

// ---- the knowledge base (knowledge/store.py) ----
// knowledge.store.KnowledgeBase on the C++ database: values decoded here, as Python's.
class KnowledgeBase : public knowledge::sources::KnowledgeBase {
  public:
    explicit KnowledgeBase(const py::str& path);
    ~KnowledgeBase();
    KnowledgeBase(const KnowledgeBase&) = delete;
    KnowledgeBase& operator=(const KnowledgeBase&) = delete;

    py::str path;
    void close();

    std::optional<facts::Value> get(const py::str& platform, const py::str& kind, const py::str& key);
    bool has(const py::str& platform, const py::str& kind, const py::str& key);
    void put(const py::str& platform, const py::str& kind, const py::str& key, const facts::Value& value,
             const py::str& source = py::str(""), const py::str& origin = py::str("learned"));
    void put_many(const py::str& platform, const py::str& kind, const py::dict<py::str, facts::Value>& values,
                  const py::str& source = py::str(""), const py::str& origin = py::str("learned"));
    void forget(const py::str& platform, const py::str& kind, const py::str& key);
    // (source, origin, ...) of a piece, or None
    std::optional<Cells> about(const py::str& platform, const py::str& kind, const py::str& key);
    // {key: value} of a kind, read once per process
    py::dict<py::str, facts::Value> table(const py::str& platform, const py::str& kind);
    py::dict<py::str, Cell> hashes(const py::str& platform, const py::str& kind);
    std::optional<py::str> hash_of(const py::str& platform, const py::str& kind, const py::str& key);
    std::optional<py::str> kind_digest(const py::str& platform, const py::str& kind);
    // reads.digest's answer from the base (the kind brought up to date by the caller)
    py::str digest(const py::str& read);
    Cell stamp(const py::str& platform, const py::str& kind);
    // a whole kind made again when its stamp changes
    py::dict<py::str, facts::Value> ensure_values(const py::str& platform, const py::str& kind, const py::str& stamp,
                                                  const std::function<py::dict<py::str, facts::Value>()>& make,
                                                  const py::str& source, const py::str& origin = py::str("authored"));
    // knowledge::sources::KnowledgeBase: rows of Json values (the authored kinds)
    knowledge::sources::Rows ensure(const py::str& platform, const py::str& kind, const py::str& stamp,
                                    const std::function<knowledge::sources::Rows()>& make,
                                    const py::str& source) override;
    void forget_ensured(const py::str& platform, const py::str& kind) override;
    // [[platform, kind, count, size, {origin: n}, source]]: origins as the JSON text the base keeps
    std::vector<Cells> kinds(const std::optional<py::str>& platform = std::nullopt);
    Cell carried(const py::str& what, const std::optional<py::str>& mark = std::nullopt);

    void add_module(const py::str& sha, const py::str& name, const py::str& platform = py::str(""));
    void set_bodies(const py::str& module, const py::dict<py::str, std::tuple<py::str, Cell, Cell>>& rows);
    py::dict<py::str, std::tuple<py::str, Cell, Cell>> bodies(const py::str& module);
    void name_body(const py::str& body_id, const py::str& name, const py::str& source = py::str("named by hand"));
    std::optional<Cells> name(const py::str& body_id, bool record = true);
    py::dict<py::str, std::tuple<Cell, Cell>> names_from_bodies(const py::str& module, bool record = true);
    Cells shared_bodies();
    Cell modules_with_body(const py::str& digest);
    void carry_bodies(const py::str& store_db, const py::str& label);

    std::vector<Cells> _c(const py::str& method, const Cells& args);

  private:
    void _read(const py::str& kind, const py::str& key);
    void* p_ = nullptr;
    std::recursive_mutex mu_;
    py::dict<std::tuple<py::str, py::str>, py::dict<py::str, facts::Value>> tables_;
    py::dict<std::tuple<py::str, py::str>, py::str> ensured_;
};

// ---- the verdict vocabulary ----
// verdicts_call("verdict", word) and the others: the answer's fields.
Cells verdicts_call(const py::str& method, const Cells& args);

}  // namespace codebreaker::engine::native_engine

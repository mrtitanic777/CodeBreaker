// cb/compare/versions.hpp -- two builds of a module, function by function: what is the same, what changed, what is new, and what
// is already known about the new one because of the old one.
//
//     codebreaker versions <module> --from CORPUS --to CORPUS       one module, two products
//     codebreaker versions <fileA> <fileB>                          any two files
//     codebreaker versions --all --from CORPUS --to CORPUS          every module of a product: how much of it is known
//                          [--sample N] [--jobs N]
//     codebreaker versions --shared A B C ... [--level shape]       the functions several different programs share: a
//                                                                   source module linked into each
//
//     Side a = load(path_a), b = load(path_b);                      the functions of a build, fingerprinted
//     Comparison c = compare(path_a, path_b);                       pairs, added, removed, counts(), code(), report()
//
// HOW. A function is everything from its start (the starts the decompiler would use) to the return after which nothing of it
// is left. It is fingerprinted at three strictnesses (bytes: its machine code with only what the loader or linker fills in
// blanked; text: its instructions with every address replaced by what it stands for; shape: instruction kinds and operand
// kinds only). Matching goes from certain to likely -- the same bytes, the same exported name, the same text, the same shape,
// then the call graph, then likeness of the shapes, then position between two matched neighbours -- and each step only
// considers what is still unmatched. A pair is `identical` when bytes or text agree, else `changed` with a score; functions
// of the old build with no partner are `removed`, of the new one `added`.
//
// Work already done on the old build reaches the new one through `Ledger` (what the function store knows: the verified
// reconstructions, the names of bodies): a function identical in both carries its name and its reconstruction across.
//
// The tie-breaks of the matching are fixed: candidates are looked up in a fixed order (sorted marks, then sorted run ids), and
// a run of shapes is the BLAKE2b digest of its text, so two runs of the same pair agree with each other and across processes.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::base {
class Corpus;
}

namespace cb::compare::versions {

// The three strictnesses a function is fingerprinted at, strictest first.
enum class Level { Bytes, Text, Shape };
inline constexpr std::array<Level, 3> kLevels{Level::Bytes, Level::Text, Level::Shape};
std::string_view level_name(Level level);                      // "bytes" "text" "shape"
std::optional<Level> level_from_name(std::string_view name);

// The kinds of file whose functions are compared.
enum class Kind { Pe, Ne, Dos };
std::string_view kind_name(Kind kind);                         // "pe" "ne" "dos"

// What a call goes to: another function of the module (by key), or an imported one (by "MODULE.name").
struct Call {
    enum class To { Function, Import } to = To::Function;
    std::string key;
    friend bool operator==(const Call&, const Call&) = default;
};

struct Fingerprints {
    std::string bytes, text, shape;
    const std::string& at(Level level) const { return level == Level::Bytes ? bytes : level == Level::Text ? text : shape; }
};

// One instruction as listed: its address and "mnemonic operands".
struct Row {
    std::uint64_t address = 0;
    std::string text;
};

// One function of one build.
struct Function {
    std::string key, name;               // key: the address as the build writes it ("1:04a2", "bfea1000", "10a3c")
    bool named = false;                  // the build names it (an export, a symbol); else `name` is sub_...
    std::int64_t at = 0, size = 0;
    std::vector<Row> rows;
    std::vector<std::string> text, shape;   // per instruction, address-free and kind-only
    Fingerprints ids;
    std::vector<Call> calls;
    std::set<std::string> marks;         // sorted: what is distinctive (an import it calls, a constant it uses)
    std::int64_t jumps = 0;
    std::size_t count() const { return shape.size(); }
};

// Lookup by text without making a string of the key.
struct KeyHash {
    using is_transparent = void;
    std::size_t operator()(std::string_view s) const { return std::hash<std::string_view>{}(s); }
};

// One build of a module.
class Side {
public:
    Side(std::filesystem::path path, Kind kind, std::int64_t code) : path_(std::move(path)), kind_(kind), code_(code) { fill_name(); }

    const std::filesystem::path& path() const { return path_; }
    Kind kind() const { return kind_; }
    const std::string& name() const { return name_; }            // the file's name in lower case
    std::int64_t code() const { return code_; }                  // bytes of code the module has
    std::size_t count() const { return functions_.size(); }
    std::int64_t size() const;                                   // bytes of the functions
    std::span<const Function> functions() const { return functions_; }   // in address order
    const Function& function(std::size_t index) const { return functions_[index]; }
    std::optional<std::size_t> index_of(std::string_view key) const;
    const Function* find(std::string_view key) const;
    // The functions that call `index` (indexes, ascending).
    const std::vector<std::size_t>& callers_of(std::size_t index) const { return callers_[index]; }

    void add(Function f);
    void finish();                       // after the last add(): the callers of each function

private:
    void fill_name();
    std::filesystem::path path_;
    Kind kind_;
    std::int64_t code_;
    std::string name_;
    std::vector<Function> functions_;
    std::unordered_map<std::string, std::size_t, KeyHash, std::equal_to<>> index_;   // lookup only
    std::vector<std::vector<std::size_t>> callers_;
};

// Read a module and fingerprint its functions. UnsupportedError for a file whose kind is not compared.
Side load(const std::filesystem::path& path);

// The functions several programs have in common: each with its size in instructions and where it is (program name -> keys).
struct Shared {
    std::size_t instructions = 0;
    std::vector<std::pair<std::string, std::vector<std::string>>> where;   // in the order the programs were met
};
// Those that are in two programs or more, largest first; `least`: the fewest instructions worth reporting.
std::vector<Shared> shared(std::span<const std::filesystem::path> paths, Level level = Level::Text, std::size_t least = 8);

// 0..1: how alike two functions' shapes are (1 = the same sequence).
double likeness(const Function& a, const Function& b);

// How a pair was found, from the surest to the least sure.
enum class How { Name, Bytes, Text, Shape, Calls, Likeness, Position };
std::string_view how_name(How how);                            // "name" "bytes" "text" "shape" "calls" "likeness" "position"

// The pairing of two builds: indexes into a and b, in the order the pairs were found.
struct Pairing {
    struct Link {
        std::size_t from, to;
        How how;
    };
    std::vector<Link> links;
};
Pairing match(const Side& a, const Side& b, double least = 0.6);

// ---- what the comparison found ------------------------------------------------------------------------------------
struct Pair {
    std::string from, to, from_name, to_name;
    bool identical = false;              // bytes or text agree; else changed
    std::optional<Level> level;          // the strictest level at which they agree
    How how = How::Name;
    double score = 0.0;                  // 1 for identical, else the likeness of the shapes (to three places)
    std::int64_t size_from = 0, size_to = 0;
};

struct Counts {
    std::int64_t identical = 0, identical_bytes = 0, identical_text = 0, changed = 0, added = 0, removed = 0;
};
struct CodeBytes {
    std::int64_t to_bytes = 0, identical = 0, changed = 0, added = 0;
};
struct NameRow {                         // a name the old build has for a function the new build has none for
    std::string to, name, from;
    bool identical = false;
};
struct Reconstruction {                  // a verified or weak reconstruction of a function the new build has unchanged
    std::string from, to, klass;
    std::optional<std::string> engine, name;
    std::string source;
};

// What knowing more of a build than its bytes asks of the rest of CodeBreaker: the corpus (which product a file is in, its
// content hash), the function store (what is reconstructed of the old build, where the new one's results are filed) and the
// knowledge base's bodies (the names a body carries). The command and the matching read only through it.
class Ledger {
public:
    virtual ~Ledger() = default;
    struct Identity {                    // what results for a module are filed under
        std::string sha, profile;
    };
    struct Known {                       // the best candidate of a function and the class its verdict makes it
        std::string klass;               // "verified", "weak", ...
        std::optional<std::string> engine, name;
        std::string source;
    };
    virtual std::optional<std::string> sha(const std::filesystem::path& module) = 0;     // content hash; nullopt: not known
    virtual std::optional<std::string> owner(const std::filesystem::path& module) = 0;   // the corpus the file is in
    virtual std::optional<Identity> identity(const std::filesystem::path& module) = 0;
    virtual std::map<std::string, Known> best(const Identity& identity) = 0;             // {address: best candidate}
    // {address: the body's digest} of a module's recorded bodies; empty when none are recorded.
    virtual std::map<std::string, std::string> bodies(const std::string& sha) = 0;
    virtual void name_body(const std::string& digest, const std::string& name, const std::string& source) = 0;
    // File a module and carried reconstructions for it (to be judged there): `rows` are {address, name, source, reason}.
    struct Carried {
        std::string address;
        std::optional<std::string> name;
        std::string source, reason;
    };
    virtual void file_carried(const Identity& identity, const std::string& module_name, const std::filesystem::path& path, Kind kind,
                              std::span<const std::string> addresses, const std::string& rev, std::span<const Carried> rows) = 0;
};

// The product of comparing two builds.
class Comparison {
public:
    Comparison(std::shared_ptr<const Side> a, std::shared_ptr<const Side> b, const Pairing& pairing, double seconds);

    const Side& from() const { return *a_; }
    const Side& to() const { return *b_; }
    double seconds() const { return seconds_; }
    const std::vector<Pair>& pairs() const { return pairs_; }            // in the old build's address order
    const std::vector<std::size_t>& removed() const { return removed_; }  // indexes in the old build
    const std::vector<std::size_t>& added() const { return added_; }      // indexes in the new build

    Counts counts() const;
    // Bytes of the new build's code: all of it, and what is identical / changed / added.
    CodeBytes code() const;
    // Names the old build has for functions the new build has none for.
    std::vector<NameRow> names() const;
    // The old build's verified reconstructions of functions the new build has unchanged.
    std::vector<Reconstruction> reconstructions(Ledger& ledger) const;
    // Give the carried names to the new build's bodies in the store (identical pairs only): how many, or nullopt when the
    // store has no bodies for the new build.
    std::optional<std::size_t> carry_names(Ledger& ledger) const;
    // File those reconstructions for the new build, to be judged there: how many.
    std::size_t carry(Ledger& ledger) const;
    std::string report() const;
    // The --json document: the pairs, what is added and removed, the names and reconstructions that carry, and with `listings`
    // each changed function's instructions side by side. `ledger` null: no reconstructions are listed.
    std::string json(bool listings = true, Ledger* ledger = nullptr) const;

private:
    std::shared_ptr<const Side> a_, b_;
    double seconds_;
    std::vector<Pair> pairs_;
    std::vector<std::size_t> removed_, added_;
    std::vector<std::size_t> pair_from_, pair_to_;   // the indexes each pair stands for
};

// Match the functions of two builds of a module.
Comparison compare(const std::filesystem::path& a, const std::filesystem::path& b, double least = 0.6);

// ---- a whole product -----------------------------------------------------------------------------------------------
enum class ModuleStatus { Compared, SameFile, NewModule, Failed };
std::string_view status_name(ModuleStatus status);             // "compared" "same file" "new module" "failed"

struct ModuleRow {
    std::string name;
    ModuleStatus status = ModuleStatus::Compared;
    std::optional<std::int64_t> functions, bytes, identical, identical_code, changed, changed_code, added, removed;
    std::optional<double> seconds;
    std::optional<std::string> error;
};

struct Product {
    std::string from, to;
    std::vector<ModuleRow> modules;
    // The sums over the modules (modules, each status, functions, bytes, identical, identical_code, changed, changed_code,
    // added, removed), in the order their keys were first met.
    std::vector<std::pair<std::string, std::int64_t>> totals;
    std::int64_t total(std::string_view key) const;
};

// How much of product `to` is already known from product `from`: every 16- and 32-bit module of `to` (or an evenly spread
// `sample` of them) against the same-named module of `from`. `log` is told each module's line.
Product product(base::Corpus& corpus, const std::string& from, const std::string& to, std::size_t sample = 0,
                std::optional<int> jobs = std::nullopt, double least = 0.6, const std::function<void(const std::string&)>& log = {});
std::string product_json(const Product& p, std::optional<double> seconds = std::nullopt);

// `codebreaker versions ...`: the command line after the command name. Returns the exit code.
int run(const cli::Args& args, base::Corpus& corpus, Ledger& ledger);
// The command as the command table declares it (`codebreaker versions`).
cli::Command command(base::Corpus& corpus, Ledger& ledger);
// The usage text printed for a command line that names neither modules nor files.
extern const char* const kUsage;

}  // namespace cb::compare::versions

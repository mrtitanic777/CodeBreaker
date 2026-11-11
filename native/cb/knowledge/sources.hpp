// cb/knowledge/sources.hpp -- where each authored kind of the knowledge base comes from, and how it is loaded.
//
// Some knowledge is written by people and kept in the repository as data files: the names of numbered exports, the
// driver kit's services, the DOS interrupts, the stand-ins' models. For such a kind the file IS the knowledge and the
// rows in the base are its copy, made again when the file's hash changes. (The derived and learned kinds are made by
// the modules that know how -- the headers' reader, an export's code -- and keep what they make in the base through
// its own calls; they are not here.)
//
//     AuthoredSources sources(DataFolders::from_environment());
//     const Rows rows = sources.authored({"win16", "api_model"}, store);   // loaded first when the file changed
//
// `catalogue()` is the table of the fourteen kinds, as data: which file, and how its rows are cut from it (the whole
// object, one part of it, or the runtime-library signature tables). `store` is the base as this module needs it
// (`KindStore`): the knowledge store implements it over its database.
#pragma once

#include <filesystem>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/knowledge/value.hpp"

namespace cb::knowledge {

// {key: value} of a kind, keys in the order the file gives them.
using Rows = OrderedMap<std::string, Value>;

// A kind of the base: a platform ("win32", "vxd", "windows", "dos", "xbox360", "win16") and the kind's name.
struct KindId {
    std::string platform;
    std::string kind;
    friend auto operator<=>(const KindId&, const KindId&) = default;
};

// The part of the knowledge base that loading an authored kind needs.
class KindStore {
public:
    virtual ~KindStore() = default;
    // The kind's rows, made again by `make` when `stamp` differs from the one they were made under (else the stored
    // rows, as they are). `source` says where they came from, for whoever lists the base.
    virtual Rows ensure(const KindId& kind, std::string_view stamp, const std::function<Rows()>& make,
                        std::string_view source) = 0;
    // Make the next ensure() of this kind compare stamps again, whatever it saw before.
    virtual void forget_ensured(const KindId& kind) = 0;
};

// Which folder of the data a file is in.
enum class DataFolder { Symbols, Models };

// How a kind's rows are cut from its file.
enum class Layout {
    Whole,        // the file is one object; each entry is a row
    Part,         // one entry of the file's object (`part`) is the object whose entries are the rows
    Signatures,   // `part` is "16" or "32": rows "000000", "000001"... of [lead, size, mask, digest, name, library]
};

struct AuthoredKind {
    KindId id;
    DataFolder folder = DataFolder::Symbols;
    std::string file;      // the file's name in its folder
    Layout layout = Layout::Whole;
    std::string part;      // see Layout
    std::string what;      // one line: what the kind holds
};

// The folders the authored files are in.
struct DataFolders {
    std::filesystem::path symbols;   // <checkout>/codebreaker/symbols
    std::filesystem::path models;    // <checkout>/codebreaker/knowledge/models

    static DataFolders under(const std::filesystem::path& checkout);
    // CB_CHECKOUT_ROOT when set, else the working folder.
    static DataFolders from_environment();
};

class AuthoredSources {
public:
    explicit AuthoredSources(DataFolders folders) : folders_(std::move(folders)) {}

    // Every authored kind, in the order the catalogue lists them.
    static const std::vector<AuthoredKind>& catalogue();
    static const AuthoredKind* find(const KindId& id);

    std::filesystem::path path_of(const AuthoredKind& kind) const;

    // Cut the file's rows. Throws IoError (cannot read), ParseError (not JSON) or FormatError (JSON of another shape).
    Rows read(const AuthoredKind& kind) const;

    // The hash of the kind's file as it is now (cached until changed() is told otherwise); "missing" when it cannot be
    // read.
    std::string stamp(const AuthoredKind& kind);

    // The rows of an authored kind, loaded first when its file changed. A file that cannot be read or is not the shape
    // the kind says makes the kind empty and is logged. Throws UsageError for a kind that is not authored.
    Rows authored(const KindId& id, KindStore& store);

    // A file of the catalogue was written by this process: its kinds are loaded again at the next ask. Returns those
    // kinds.
    std::vector<KindId> changed(const std::filesystem::path& file, KindStore& store);

    // The file's name relative to the checkout, as the base shows it ("codebreaker/symbols/ordinal_names.json").
    std::string source_label(const AuthoredKind& kind) const;

private:
    DataFolders folders_;
    std::mutex mutex_;
    std::unordered_map<std::string, std::string> stamps_;   // lookup only: file path text -> hash
};

}  // namespace cb::knowledge

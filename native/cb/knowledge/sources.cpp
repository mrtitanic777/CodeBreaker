#include "cb/knowledge/sources.hpp"

#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

namespace cb::knowledge {

namespace fs = cb::fs;

namespace {


AuthoredKind kind(std::string platform, std::string name, DataFolder folder, std::string file, Layout layout,
                  std::string part, std::string what) {
    return AuthoredKind{{std::move(platform), std::move(name)}, folder, std::move(file), layout, std::move(part),
                        std::move(what)};
}

// The object a part of the file names, as rows; an absent or empty part is no rows.
Rows rows_of_object(const Value& object, std::string_view file) {
    Rows rows;
    const Value::Map* map = object.map();
    if (map == nullptr) throw FormatError(file, 0, "an object was expected");
    rows.reserve(map->size());
    for (const auto& [key, value] : *map) rows.insert_or_assign(key, value);
    return rows;
}

Rows signature_rows(const Value& document, std::string_view bits, std::string_view file) {
    const Value* labels = document.get("libraries");
    const Value::List* names = labels != nullptr ? labels->list() : nullptr;
    const Value* table = document.get(bits);
    const Value::List* entries = table != nullptr ? table->list() : nullptr;
    Rows rows;
    if (entries == nullptr) return rows;
    rows.reserve(entries->size());
    std::size_t index = 0;
    for (const Value& entry : *entries) {
        const Value::List* f = entry.list();
        if (f == nullptr || f->size() != 6) throw FormatError(file, 0, std::format("signature {} is not six fields", index));
        const std::optional<std::int64_t> library = (*f)[5].integer();
        if (names == nullptr || !library || *library < 0 || static_cast<std::uint64_t>(*library) >= names->size())
            throw FormatError(file, 0, std::format("signature {} names a library that is not listed", index));
        rows.insert_or_assign(std::format("{:06}", index),
                              Value(Value::List{(*f)[0], (*f)[1], (*f)[2], (*f)[3], (*f)[4],
                                                (*names)[static_cast<std::size_t>(*library)]}));
        ++index;
    }
    return rows;
}

}  // namespace

DataFolders DataFolders::under(const std::filesystem::path& checkout) {
    return {checkout / "codebreaker" / "symbols", checkout / "codebreaker" / "knowledge" / "models"};
}

DataFolders DataFolders::from_environment() {
    const std::optional<std::string> root = fs::get_env("CB_CHECKOUT_ROOT");
    return under(root && !root->empty() ? fs::from_text(*root) : std::filesystem::current_path());
}

const std::vector<AuthoredKind>& AuthoredSources::catalogue() {
    static const std::vector<AuthoredKind> kinds = [] {
        constexpr auto S = DataFolder::Symbols;
        constexpr auto M = DataFolder::Models;
        return std::vector<AuthoredKind>{
            kind("windows", "ordinal_names", S, "ordinal_names.json", Layout::Whole, "",
                 "names of numbered exports from their makers' definition files and import libraries: "
                 "{module: {from, names: {ordinal: name}}}"),
            kind("win32", "argument_bytes.header", S, "win32_arguments.json", Layout::Whole, "",
                 "bytes of arguments as the Win32 headers declare them (tools/make_api_table.py)"),
            kind("win32", "signature", S, "runtime_signatures.json", Layout::Signatures, "32",
                 "32-bit runtime-library functions: [leading bytes, size, mask, digest, name, library]"),
            kind("win16", "signature", S, "runtime_signatures.json", Layout::Signatures, "16",
                 "16-bit runtime-library functions: [leading bytes, size, mask, digest, name, library]"),
            kind("vxd", "device", S, "driver_services.json", Layout::Part, "devices", "device numbers the driver kit names"),
            kind("vxd", "service_table", S, "driver_services.json", Layout::Part, "services",
                 "each device's services in order (service N is the Nth)"),
            kind("vxd", "driver", S, "driver_services.json", Layout::Part, "drivers",
                 "device numbers corpus drivers give themselves"),
            kind("vxd", "message", S, "driver_services.json", Layout::Part, "messages",
                 "messages a driver's control procedure is sent"),
            kind("vxd", "api_model", M, "vxd.json", Layout::Part, "services",
                 "what a service does that the emulator's stand-in must do too"),
            kind("dos", "interrupt", S, "dos_interrupts.json", Layout::Whole, "",
                 "DOS and BIOS interrupt functions: names and arguments"),
            kind("xbox360", "ordinal_names", S, "console_ordinals.json", Layout::Whole, "",
                 "numbered exports of the console's system libraries"),
            kind("win16", "api_model", M, "win16.json", Layout::Part, "functions",
                 "what the emulator's stand-in for a Windows function returns and removes"),
            kind("win32", "api_model", M, "win32.json", Layout::Part, "functions",
                 "documented argument counts for the emulator's stand-in of a common import"),
            kind("win16", "kernel_ordinal", M, "win16.json", Layout::Part, "kernel_ordinals",
                 "KERNEL's numbers for the functions the stand-ins model"),
        };
    }();
    return kinds;
}

const AuthoredKind* AuthoredSources::find(const KindId& id) {
    for (const AuthoredKind& k : catalogue())
        if (k.id == id) return &k;
    return nullptr;
}

std::filesystem::path AuthoredSources::path_of(const AuthoredKind& k) const {
    return (k.folder == DataFolder::Symbols ? folders_.symbols : folders_.models) / k.file;
}

std::string AuthoredSources::source_label(const AuthoredKind& k) const {
    return std::string(k.folder == DataFolder::Symbols ? "codebreaker/symbols/" : "codebreaker/knowledge/models/") + k.file;
}

Rows AuthoredSources::read(const AuthoredKind& k) const {
    const std::filesystem::path path = path_of(k);
    const std::string name = fs::to_text(path);
    const Value document = Value::from_json_text(fs::read_text(path), name);
    if (document.map() == nullptr) throw FormatError(name, 0, "an object was expected at the top");
    switch (k.layout) {
        case Layout::Whole: return rows_of_object(document, name);
        case Layout::Part: {
            const Value* part = document.get(k.part);
            return part == nullptr ? Rows() : rows_of_object(*part, name);
        }
        case Layout::Signatures: return signature_rows(document, k.part, name);
    }
    throw InternalError("AuthoredSources::read: unknown layout");
}

std::string AuthoredSources::stamp(const AuthoredKind& k) {
    const std::filesystem::path path = path_of(k);
    const std::string key = fs::to_text(path);
    {
        const std::lock_guard<std::mutex> lock(mutex_);
        if (auto it = stamps_.find(key); it != stamps_.end()) return it->second;
    }
    std::string hash;
    try {
        hash = crypto::to_hex(crypto::sha1(fs::read_file(path)));
    } catch (const IoError&) {
        hash = "missing";   // a file that is not there is a stamp too: the kind is empty until it is
    }
    const std::lock_guard<std::mutex> lock(mutex_);
    stamps_[key] = hash;
    return hash;
}

Rows AuthoredSources::authored(const KindId& id, KindStore& store) {
    const AuthoredKind* k = find(id);
    if (k == nullptr) throw UsageError(std::format("{}:{} is not an authored kind", id.platform, id.kind));
    const std::string stamp_now = stamp(*k);
    // An isolation boundary: one broken data file empties its kinds, loudly, and the run goes on.
    auto make = [this, k]() -> Rows {
        try {
            return read(*k);
        } catch (const Error& e) {
            log::warn(std::format("knowledge {}:{}: {}: the kind is empty", k->id.platform, k->id.kind, e.what()));
            return Rows();
        }
    };
    return store.ensure(id, stamp_now, make, source_label(*k));
}

std::vector<KindId> AuthoredSources::changed(const std::filesystem::path& file, KindStore& store) {
    std::vector<KindId> touched;
    for (const AuthoredKind& k : catalogue()) {
        if (!fs::same_path(path_of(k), file)) continue;
        {
            const std::lock_guard<std::mutex> lock(mutex_);
            stamps_.erase(fs::to_text(path_of(k)));
        }
        store.forget_ensured(k.id);
        touched.push_back(k.id);
    }
    return touched;
}

}  // namespace cb::knowledge

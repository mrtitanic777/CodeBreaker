// exact/mspascal.hpp -- the Microsoft Pascal compiler (PAS1/PAS2; 1.10 of November 1981 to 3.11), as a toolchain.
//
// A profile names this module (toolchains/mspascal.json: "plugin": "codebreaker.exact.mspascal:MSPascal"). The passes run under the DOS
// emulator: `PAS1 T,T,T,T;` (source, object, listing, code listing), then `PAS2`. The object is OMF. Most of a compiland's procedures are
// not public; the plugin names them, so that the engine sees them as it sees a C file's functions: from the code listing (T.COD:
// "Procedure/Function : NAME" and the first address of its code) -- or, for a compiler that writes none (3.04, 3.11), from where the code
// calls the compiler's entry helper (or sets up a frame in line), in the order the source emits its procedures (each at the end of its body:
// after those declared inside it). The names are published in the object (a PUBDEF of the code segment): exact/omf and the ledger see them
// like a C file's functions.
//
// Every compiland ends with the compiler's entry helper (a near call to it opens each procedure); it is published as "(helper)".
//
// Variants: the kit's version ("version": a folder of the kit, or "1.10": the pre-release kit's zips).
//
//     toolchain::Toolchains toolchains(locations, &runner, &cache);
//     mspascal::register_plugin(toolchains, {.extract_zip = ...});
//     auto tc = std::dynamic_pointer_cast<mspascal::MsPascal>(toolchains.load("mspascal", "3.11"));
//     auto r = tc->compile(source, "T.PAS");              // as pascal_join asks;   modules::Toolchain::compile(source, options) too
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/json_fwd.hpp"
#include "core/ordered_map.hpp"
#include "exact/pascal_join.hpp"
#include "exact/toolchain.hpp"

namespace cb::exact::mspascal {

inline constexpr std::string_view kCacheVersion = "pas1";
inline constexpr std::string_view kPluginName = "codebreaker.exact.mspascal:MSPascal";

// The compiler's entry helper in each of its versions (a near call to it opens every procedure) and the frame set up in line (3.11).
const std::vector<Bytes>& entry_helpers();
const Bytes& inline_frame();

// The procedures of a source in the order their code is written: each at the end of its body, after the procedures declared inside it.
std::vector<std::string> emit_order(std::string_view text);

// The object with a PUBDEF of the code segment `code_seg` naming `procs` {name: offset} (before its MODEND), the names in the order
// of their offsets (equal offsets in the order given).
Bytes publish(ByteView object, const OrderedMap<std::string, std::uint32_t>& procs, std::string_view code_seg);

// The object with the procedures named: `listing` is T.COD's text ("" when the compiler wrote none), `text` the source. An object with
// no code, or with nothing to name, is returned as it is.
Bytes name_procedures(ByteView object, std::string_view listing, std::string_view text);

// What the plugin takes from outside the process.
struct Hooks {
    // A zip's files extracted into a folder (the 1.10 pre-release kit is kept as zips). Without it, that kit is an UnsupportedError.
    std::function<void(const std::filesystem::path& zip, const std::filesystem::path& dest)> extract_zip;
};

class MsPascal final : public toolchain::Toolchain, public pascal_join::Toolchain {
public:
    // UsageError for a variant the profile does not have; NotFoundError when the profile's file is not there.
    MsPascal(toolchain::Profile profile, std::optional<std::string> variant, const toolchain::Context& context, Hooks hooks = {});
    ~MsPascal() override;

    const std::string& version() const { return version_; }
    // sha1(name + version + kCacheVersion), 12 hex digits: names this toolchain's compiles in the cache.
    const std::string& plugin_key() const { return plugin_key_; }
    // The bytes after the entry helper's call: the variant's "helper_data", else the profile's, else 3 (a byte and a word in 1.x, the
    // frame size alone in 3.x).
    std::uint32_t helper_data() const override { return helper_data_; }

    // The kit's folder for this version: the version's folder of the kit, or for 1.10 the zips of the pre-release kit extracted once
    // under <state>/exact/stage/mspascal110.
    std::filesystem::path kit() const;

    // The object (its procedures named) and what the passes printed. `file_name` is the source's name (T.PAS by default). Cached under the
    // source, its name and plugin_key.
    pascal_join::CompileResult compile(ByteView source, std::string_view file_name) const override;
    modules::CompileResult compile(ByteView source, const modules::CompileOptions& options) const override;

    // What compile() keeps its answer under, and the compile itself without the cache (a caller with a cache of its own).
    std::string cache_key(ByteView source, std::string_view file_name) const;
    pascal_join::CompileResult build(ByteView source, std::string_view file_name) const;

private:
    modules::CompileResult run_passes(const Bytes& source, const std::string& file_name) const;

    toolchain::Context ctx_;
    Hooks hooks_;
    std::string version_, plugin_key_;
    std::uint32_t helper_data_ = 3;
    std::unique_ptr<Json> raw_;   // the profile's own JSON
};

void register_plugin(toolchain::Toolchains& toolchains, Hooks hooks = {});

}  // namespace cb::exact::mspascal

// engine/lane_kit_internal.hpp -- what the files of lane_kit share and nobody else needs.
#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "analyze/image.hpp"
#include "base/cache.hpp"
#include "base/config.hpp"
#include "base/corpus.hpp"
#include "build/functions.hpp"
#include "build/watcom.hpp"
#include "decompile/decompiler.hpp"
#include "engine/lane_kit.hpp"
#include "formats/pe.hpp"
#include "emulate/windows16.hpp"
#include "knowledge/package.hpp"
#include "source/assembly.hpp"
#include "symbols/api_arguments.hpp"
#include "symbols/driver_services.hpp"
#include "translate/native_l2.hpp"

namespace cb::engine::kit_detail {

// The corpus over the configuration's products, the lane of a file from formats/detect.
std::shared_ptr<base::Corpus> corpus_of(const config::Config& config);

// The shared cache as the function builder's build cache.
class KitBuildCache final : public build::functions::BuildCache {
public:
    explicit KitBuildCache(base::cache::Cache& cache) : cache_(cache) {}
    std::optional<Bytes> get(std::string_view kind, std::string_view key) override { return cache_.get(kind, key); }
    void put(std::string_view kind, std::string_view key, ByteView value) override { cache_.put(kind, key, value); }

private:
    base::cache::Cache& cache_;
};

// How many bytes of arguments the functions a module calls remove, read from the headers and the code of the modules that export them
// (symbols/api_arguments), and what is found of a module from them: the 32-bit and the 16-bit tables, the stubs of a flat thunk.
class KitSizes {
public:
    KitSizes(base::Corpus& corpus, knowledge::Knowledge& knowledge);
    ~KitSizes();

    std::optional<std::uint32_t> stack_bytes(std::string_view dll, std::string_view name);
    std::optional<std::int64_t> argc(std::string_view dll, std::string_view name);
    std::optional<std::uint32_t> header_bytes(std::string_view name);
    // Bytes the function at `rva` of a PE module removes (read from its returns).
    std::optional<std::uint32_t> pe_function_pop(const std::filesystem::path& module, std::uint32_t rva);
    // The 16-bit table: "USER.@171" or "USER_171".
    std::optional<std::uint32_t> win16_bytes(std::string_view api);
    // The sizes found so far for the 16-bit exports, as text (what a cache of the gate's verdicts is keyed by).
    std::string win16_table_text();
    // The starts of the functions the thunk compiler generated in a flat-thunk module.
    std::vector<std::uint64_t> flat_thunk_stubs(const std::filesystem::path& module);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// The 16-bit runner's surroundings (lane_kit_win16.cpp).
emulate::windows16::Environment win16_environment_of(knowledge::Knowledge& kb, KitSizes& sizes, emulate::native_trace::Governor& governor);

// What the decompiler asks of the rest of CodeBreaker, answered from the symbol tables, the analyses and the readers.
class KitEnvironment final : public decompile::decompiler::Environment {
public:
    KitEnvironment(const config::Config& config, base::Corpus& corpus, knowledge::Knowledge& knowledge);
    ~KitEnvironment() override;

    KitSizes& sizes();

    decompile::describe::Knowledge& knowledge() override;
    const translate::native_l2::Library& library() override;
    std::map<std::string, std::string> import_dlls(const std::filesystem::path& module) override;
    std::optional<std::uint32_t> argument_bytes(std::string_view dll, std::string_view name) override;
    std::optional<std::vector<decompile::struct_text::Structure>> recover_structures(const std::filesystem::path& module) override;
    decompile::describe::PowerPcFacts powerpc_facts(const std::filesystem::path& module) override;
    std::vector<decompile::decompiler::Va> powerpc_functions(const std::filesystem::path& module) override;
    void described(const std::filesystem::path& module, const decompile::describe::PeDescription& description) override;
    std::map<decompile::decompiler::Va, decompile::decompiler::LibraryFunction> dos_library(const formats::dos::Program& program,
                                                                                            std::span<const decompile::decompiler::Va> starts) override;

    // The link lines an object file of `module` needs for the imports its code calls (`used`: the imports its C calls).
    std::function<std::vector<std::string>(ByteView object)> import_lines(const std::filesystem::path& module,
                                                                          const std::vector<std::string>& used,
                                                                          const build::watcom::OpenWatcom& openwatcom);
    // {function key: {name, library}} of a module's functions that belong to a runtime library.
    std::map<std::string, std::pair<std::string, std::string>> identify(const std::filesystem::path& module);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// The machine a cartridge, snapshot or tape is made for ("nes", "sms", "gb", "genesis", "ps1", "gba", "zx", "msx", "coleco", "cpc"); empty when the file is none (lane_kit_machines).
std::string machine_of_file(const std::filesystem::path& path);

}  // namespace cb::engine::kit_detail

namespace cb::engine {

struct LaneKit::Impl {
    Impl(config::Config c, std::shared_ptr<base::Corpus> k);

    config::Config config;
    std::shared_ptr<base::Corpus> corpus;
    std::mutex lock;

    base::cache::Cache& cache();
    knowledge::Knowledge& knowledge();
    decompile::decompiler::Decompiler& decompiler();
    build::functions::FunctionBuilder& builder();
    emulate::pcode::LifterSet* lifters();
    kit_detail::KitEnvironment& environment();
    build::watcom::OpenWatcom openwatcom() const;

    std::unique_ptr<base::cache::Cache> cache_;
    std::unique_ptr<knowledge::Store> store_;
    std::unique_ptr<knowledge::Knowledge> knowledge_;
    std::unique_ptr<kit_detail::KitBuildCache> build_cache_;
    std::unique_ptr<build::functions::FunctionBuilder> builder_;
    std::unique_ptr<kit_detail::KitEnvironment> environment_;
    std::unique_ptr<decompile::decompiler::Decompiler> decompiler_;
    std::unique_ptr<source::assembly::Prober> prober_;
    std::optional<jobs::WorkerLauncher> launcher;
    std::unique_ptr<emulate::pcode::LifterSet> lifters_;
    std::shared_ptr<const verify::lanes::Kind> win32_, win16_, driver_, java_, powerpc_, dos_;
    std::map<std::string, std::shared_ptr<const verify::lanes::Kind>> machines_;   // "nes", "sms", "gb", "genesis", "ps1", "gba", "zx", "msx", "coleco", "cpc"
    std::shared_ptr<const symbols::driver_services::Table> driver_tables_;   // what the driver lane's spec points at
};

}  // namespace cb::engine

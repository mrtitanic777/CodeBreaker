// cb/app/native_commands.cpp -- see native_commands.hpp.
#include "cb/app/native_commands.hpp"

#include <algorithm>
#include <array>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/build/driver.hpp"
#include "cb/build/flat_thunk.hpp"
#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/formats/xbox/heavyiron.hpp"
#include "cb/symbols/demangle.hpp"
#include "cb/symbols/dos_interrupts.hpp"
#include "cb/symbols/driver_services.hpp"
#include "cb/symbols/prototypes.hpp"

namespace cb::app {

namespace {

constexpr std::array<std::string_view, 6> kNative{"info", "flat-thunk", "driver-build", "demangle", "dos-interrupts", "heavyiron"};

// The corpus over the configuration's products: the lane of a file from cb::formats::detect.
std::shared_ptr<base::Corpus> corpus_of(const config::Config& config) {
    base::FormatProbes probes;
    probes.lane = [](const std::filesystem::path& p) -> std::optional<base::Lane> {
        const std::optional<formats::detect::Lane> l = formats::detect::lane(p);
        if (!l) return std::nullopt;
        switch (*l) {
        case formats::detect::Lane::Pe: return base::Lane::Pe;
        case formats::detect::Lane::Ne: return base::Lane::Ne;
        case formats::detect::Lane::Le: return base::Lane::Le;
        case formats::detect::Lane::Dos: return base::Lane::Dos;
        }
        return std::nullopt;
    };
    return std::make_shared<base::Corpus>(config.products(), std::move(probes));
}

// The first word that is not `--corpus` or its name: the command.
std::string_view command_of(std::span<const std::string> args) {
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--corpus") {
            ++i;
            continue;
        }
        return args[i];
    }
    return {};
}

build::watcom::OpenWatcom ft_openwatcom_of(const config::Config& config) {
    return build::watcom::OpenWatcom::from_settings(config.text("openwatcom"));
}

}  // namespace

void register_native_commands(CommandTable& table, std::shared_ptr<base::Corpus> corpus) {
    formats::inspect::InfoEnvironment env;
    env.resolve = [corpus](std::string_view name) -> std::optional<std::filesystem::path> { return corpus->retail(name); };
    table.set(formats::inspect::info_command(std::move(env)));

    // flat-thunk: the period tools from the settings, the SDK prototypes read from the headers when first asked
    const config::Config& config = config::current();
    build::flat_thunk::Services ft;
    std::filesystem::path reference;
    try {
        reference = corpus->reference();
    } catch (const Error&) {   // no reference folder: flat-thunk says so when it needs one; other commands do not care
    }
    if (!reference.empty()) ft.locations.reference_roots.push_back(reference);
    ft.locations.period_compiler = config.path("period_compiler").value_or(std::filesystem::path{});
    ft.locations.openwatcom = build::watcom::OpenWatcom::from_settings(config.text("openwatcom"));
    ft.retail = [corpus](std::string_view name) -> std::optional<std::filesystem::path> { return corpus->retail(name); };
    const std::filesystem::path ow_root = ft.locations.openwatcom.root();
    ft.sdk = [ow_root, reference] {
        const auto folders = symbols::prototypes::HeaderFolders::standard(ow_root, reference);
        return build::flat_thunk::sdk_prototypes(symbols::prototypes::PrototypeTable(symbols::prototypes::read_headers(folders)));
    };
    table.set(build::flat_thunk::command(std::move(ft)));

    // driver-build: OpenWatcom's wasm/wlink against the Windows 95 DDK's 32-bit includes; originals from the corpus
    build::driver::Environment dr;
    std::filesystem::path design_reference;
    try {
        design_reference = corpus->design_dir() / "reference";
    } catch (const Error&) {
    }
    const std::filesystem::path kit = symbols::driver_services::find_include_dir(config.text("driver_kit"), design_reference);
    dr.tools = build::driver::find_tools(ft_openwatcom_of(config), kit, config.state());
    dr.retail = [corpus](std::string_view name) -> std::optional<std::filesystem::path> { return corpus->retail(name); };
    for (const auto& [name, copies] : *corpus->index())
        if (!copies.empty() && formats::detect::holds_drivers(copies.front())) dr.corpus_drivers.push_back(copies.front());
    std::sort(dr.corpus_drivers.begin(), dr.corpus_drivers.end(), [](const auto& a, const auto& b) {
        std::error_code ea, eb;
        const auto sa = std::filesystem::file_size(a, ea), sb = std::filesystem::file_size(b, eb);
        return sa != sb ? sa < sb : fs::to_text(a) < fs::to_text(b);
    });
    dr.state = config.state();
    table.set(build::driver::command(dr));

    // demangle <name> ...: each name readable, or said to be not readable
    table.set("demangle", [](std::span<const std::string> args) -> int {
        if (args.empty()) {
            out("usage: codebreaker demangle <name> [<name> ...]\n");
            return 2;
        }
        for (const std::string& name : args) {
            if (symbols::demangle::parse(name) || !name.starts_with('?')) out("{}\n", symbols::demangle::readable(name));
            else out("{}  (not readable)\n", name);
        }
        return 0;
    });

    // dos-interrupts [<interrupt> [<AH> [<AL>]]]: the checkout's own dos_interrupts.json
    const std::filesystem::path interrupts = config.checkout_root() / "codebreaker" / "symbols" / "dos_interrupts.json";
    table.set("dos-interrupts", [interrupts](std::span<const std::string> args) -> int {
        return symbols::dos_interrupts::run(symbols::dos_interrupts::ServiceTable::load(interrupts), args);
    });

    // heavyiron <archive> ...: Heavy Iron Studios' .HO/.HIP archives
    table.set("heavyiron", [](std::span<const std::string> args) -> int {
        return formats::xbox::heavyiron::command(std::vector<std::string>(args.begin(), args.end()));
    });
}

bool is_native_command(std::string_view name) {
    return std::find(kNative.begin(), kNative.end(), name) != kNative.end();
}

int run_native_command(std::span<const std::string> args) {
    std::shared_ptr<base::Corpus> corpus = corpus_of(config::current());
    Environment env;
    env.use_corpus = [corpus](const std::string& name) { corpus->use(name); };
    env.corpus_names = [corpus] { return corpus->names(); };
    CommandTable table(std::move(env));
    register_native_commands(table, corpus);
    const int code = table.main(std::vector<std::string>(args.begin(), args.end()));
    flush_out();
    return code;
}

}  // namespace cb::app

extern "C" int cb_native_command(int argc, const char* const* argv, int* handled) {
    if (handled) *handled = 0;
    if (argc < 1 || !argv) return 2;
    std::vector<std::string> args(argv, argv + argc);
    if (!cb::app::is_native_command(cb::app::command_of(args))) return 2;
    if (handled) *handled = 1;
    return cb::app::run_native_command(args);
}

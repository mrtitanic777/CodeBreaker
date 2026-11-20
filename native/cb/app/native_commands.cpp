// cb/app/native_commands.cpp -- see native_commands.hpp.
#include "cb/app/native_commands.hpp"

#include <algorithm>
#include <array>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/app/convert.hpp"
#include "cb/app/verbs.hpp"
#include "cb/app/window.hpp"
#include "cb/analyze/dos_functions.hpp"
#include "cb/base/config.hpp"
#include "cb/base/settings.hpp"
#include "cb/base/corpus.hpp"
#include "cb/base/converted.hpp"
#include "cb/base/cache_command.hpp"
#include "cb/base/corpus_executables.hpp"
#include "cb/base/tidy_state_command.hpp"
#include "cb/verify/regress.hpp"
#include "cb/app/state_commands.hpp"
#include "cb/translate/judge_c_java_command.hpp"
#include "cb/translate/judge_cs_cpp_command.hpp"
#include "cb/translate/judge_py_cpp_command.hpp"
#include "cb/analyze/ppc_calls.hpp"
#include "cb/build/disc_image.hpp"
#include "cb/build/x86asm/command.hpp"
#include "cb/exact/emu/dos_run_command.hpp"
#include "cb/build/period16.hpp"
#include "cb/build/xex_edit.hpp"
#include "cb/core/json.hpp"
#include "cb/formats/fonts.hpp"
#include "cb/formats/gfx.hpp"
#include "cb/formats/media.hpp"
#include "cb/formats/xbox/system_ui.hpp"
#include "cb/formats/xex_imports.hpp"
#include "cb/build/driver.hpp"
#include "cb/build/resources.hpp"
#include "cb/coverage/reach_command.hpp"
#include "cb/build/flat_thunk.hpp"
#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/exact/command.hpp"
#include "cb/jobs/workers.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/cab.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/formats/xbox/crypto/tools.hpp"
#include "cb/formats/xbox/disc_command.hpp"
#include "cb/formats/xbox/heavyiron.hpp"
#include "cb/formats/xex.hpp"
#include "cb/invert/production.hpp"
#include "cb/java/tools.hpp"
#include "cb/recompile/command.hpp"
#include "cb/recompile/xenon_tools.hpp"
#include "cb/source/commands.hpp"
#include "cb/source/dos_writer.hpp"
#include "cb/source/pipeline.hpp"
#include "cb/source/le_writer.hpp"
#include "cb/source/ne_writer.hpp"
#include "cb/source/pe_writer.hpp"
#include "cb/symbols/demangle.hpp"
#include "cb/symbols/dos_interrupts.hpp"
#include "cb/symbols/driver_services.hpp"
#include "cb/store/analyze_command.hpp"
#include "cb/store/versions_command.hpp"
#include "cb/symbols/identify.hpp"
#include "cb/symbols/program_commands.hpp"
#include "cb/ui/mcp_command.hpp"
#include "cb/ui/serve.hpp"
#include "cb/xna/command.hpp"
#include "cb/symbols/prototypes.hpp"

namespace cb::app {

namespace {

constexpr auto kNative = std::to_array<std::string_view>({"info", "flat-thunk", "driver-build", "demangle", "dos-interrupts", "heavyiron", "dos-functions", "cab", "xex", "le-source", "pe-source", "ne-source", "resources", "identify", "settings", "versions", "reach", "vxd", "ne-asm", "driver-source", "dos-source", "dos-build", "driver-judge", "exact", "analyze", "decompile", "place", "recompile", "recompile-project", "xenon-recompile", "classes", "xdk-match", "java", "xna", "build", "score16", "crypto", "xbox-disc", "xex-basic", "xenon-config", "switch-tables", "corpus-executables", "module", "scoreboard", "measure", "teeth", "sweep", "match", "repair", "invert", "mcp", "serve", "cache", "tidy-state", "regress", "xex-imports", "xex-edit", "ppc-calls", "system-ui", "disc", "media", "gfx", "fonts", "build16", "names", "knowledge", "coverage", "progress", "facts", "origin", "driver-services", "migrate-state", "boot-floppy", "judge-c-java", "judge-cs-cpp", "judge-py-cpp", "nasm", "dos-run", "convert", "app", "inspect", "extract", "check", "compare", "corpus", "lookup", "xbox"});   // (counted by the compiler)

// The console's ordinal names (data/symbols/console_ordinals.json, {library: {ordinal: name}}), read the first time a name is asked for.
formats::xex_imports::OrdinalNames console_ordinal_names(const config::Config& config) {
    const std::filesystem::path file = config.data_root() / "symbols" / "console_ordinals.json";
    auto table = std::make_shared<std::optional<Json>>();
    return [file, table](std::string_view library, std::uint32_t ordinal) -> std::optional<std::string> {
        if (!table->has_value()) {
            std::error_code ec;
            *table = std::filesystem::is_regular_file(file, ec) ? json::load(file) : Json::object();
        }
        const Json& root = **table;
        const auto lib = root.find(std::string(library));
        if (lib == root.end() || !lib->is_object()) return std::nullopt;
        const auto name = lib->find(std::to_string(ordinal));
        if (name == lib->end() || !name->is_string()) return std::nullopt;
        return name->get<std::string>();
    };
}

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

std::string lower(std::string_view s) {
    std::string r(s);
    for (char& c : r)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return r;
}

// The files a cabinet is made from: a file under its own name; a folder's files, folder by folder and name by name,
// under their paths inside it (backslash-separated).
struct GatheredFile {
    std::string name;
    Bytes data;
};
std::vector<GatheredFile> gather(std::span<const std::string> paths) {
    std::vector<GatheredFile> files;
    for (const std::string& given : paths) {
        const std::filesystem::path root = fs::from_text(given);
        if (!std::filesystem::is_directory(root)) {
            files.push_back({fs::to_text(root.filename()), fs::read_file(root)});
            continue;
        }
        std::vector<std::pair<std::string, std::filesystem::path>> found;   // (folder, file)
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
            if (entry.is_regular_file()) found.emplace_back(fs::to_text(entry.path().parent_path()), entry.path());
        std::sort(found.begin(), found.end(), [](const auto& a, const auto& b) {
            return a.first != b.first ? a.first < b.first : fs::to_text(a.second.filename()) < fs::to_text(b.second.filename());
        });
        for (const auto& [folder, file] : found) {
            std::string name = fs::to_text(std::filesystem::relative(file, root));
            std::replace(name.begin(), name.end(), '/', '\\');
            files.push_back({std::move(name), fs::read_file(file)});
        }
    }
    return files;
}

constexpr std::string_view kCabUsage =
    "usage: codebreaker cab list <file.cab>\n"
    "       codebreaker cab extract <file.cab> <outdir> [names...]\n"
    "       codebreaker cab create <out.cab> <file-or-folder>... [--store]\n";

int cab_command(std::span<const std::string> argv) {
    namespace cab = formats::cab;
    const bool store = std::find(argv.begin(), argv.end(), "--store") != argv.end();
    std::vector<std::string> args;
    for (const std::string& a : argv)
        if (a != "--store") args.push_back(a);
    try {
        if (args.size() == 2 && args[0] == "list") {
            const cab::Cabinet c = cab::read(fs::from_text(args[1]));
            std::string folders;
            for (const cab::FolderEntry& f : c.folders) folders += (folders.empty() ? "" : ", ") + f.compression();
            out("{}: set {}, cabinet {}, {} bytes, folders: {}\n", fs::to_text(fs::from_text(args[1]).filename()), c.set_id,
                c.index, c.size, folders.empty() ? "none" : folders);
            if (c.prev && !c.prev->cabinet.empty()) out("  prev cabinet: {}\n", c.prev->cabinet);
            if (c.next && !c.next->cabinet.empty()) out("  next cabinet: {}\n", c.next->cabinet);
            std::uint64_t total = 0;
            for (const cab::FileEntry& e : c.files) {
                std::string_view mark;
                if (e.folder == cab::kFromPrev) mark = "  (from previous cabinet)";
                else if (e.folder == cab::kToNext) mark = "  (into next cabinet)";
                else if (e.folder == cab::kPrevAndNext) mark = "  (from previous, into next)";
                out("{:>10}  {:04}-{:02}-{:02}  {:<8} {}{}\n", e.size, 1980 + (e.date >> 9), (e.date >> 5) & 15, e.date & 31,
                    c.compression_of(e), e.name, mark);
                total += e.size;
            }
            out("{} files, {} bytes\n", c.files.size(), total);
            return 0;
        }
        if (args.size() >= 3 && args[0] == "extract") {
            std::optional<std::vector<std::string>> names;
            if (args.size() > 3) names = std::vector<std::string>(args.begin() + 3, args.end());
            const std::vector<cab::Extracted> done = cab::extract(fs::from_text(args[1]), fs::from_text(args[2]), names);
            std::vector<std::string> got;
            for (const cab::Extracted& d : done) got.push_back(lower(d.name));
            bool missing = false;
            for (std::size_t i = 3; i < args.size(); ++i)
                if (std::find(got.begin(), got.end(), lower(args[i])) == got.end()) {
                    flush_out();
                    err("not in the cabinet: {}\n", args[i]);
                    missing = true;
                }
            out("extracted {} files to {}\n", done.size(), args[2]);
            return missing ? 1 : 0;
        }
        if (args.size() >= 3 && args[0] == "create") {
            const std::vector<GatheredFile> gathered = gather(std::span(args).subspan(2));
            std::vector<cab::SourceFile> files;
            for (const GatheredFile& g : gathered) files.push_back({g.name, ByteView(g.data)});
            cab::WriteOptions options;
            options.compression = store ? cab::Compression::None : cab::Compression::MsZip;
            const std::uint64_t size = cab::write(files, fs::from_text(args[1]), options);
            out("wrote {}: {} files, {} bytes\n", args[1], files.size(), size);
            return 0;
        }
    } catch (const Error& problem) {
        flush_out();
        err("cab: {}\n", problem.what());
        return 1;
    }
    err_text(kCabUsage);
    return 2;
}

// A command whose surroundings (tools, corpus scans) are found only when it runs: `make` builds it then, and it runs as
// its own table would run it.
void lazy_command(CommandTable& table, std::string name, std::function<cli::Command()> make) {
    table.set(name, [name, make = std::move(make)](std::span<const std::string> args) -> int {
        CommandTable one{Environment{}};
        one.set(make());
        std::vector<std::string> line{name};
        line.insert(line.end(), args.begin(), args.end());
        return one.run(line);
    });
}

// The same for a command that reads its own command line: `make` gives its runner when it runs.
void lazy_runner(CommandTable& table, std::string name, std::function<std::function<int(std::span<const std::string>)>()> make) {
    table.set(name, [make = std::move(make)](std::span<const std::string> args) -> int { return make()(args); });
}

}  // namespace

void register_native_commands(CommandTable& table, std::shared_ptr<base::Corpus> corpus) {
    formats::inspect::InfoEnvironment env;
    env.resolve = [corpus](std::string_view name) -> std::optional<std::filesystem::path> { return corpus->retail(name); };
    table.set(formats::inspect::info_command(std::move(env)));

    const config::Config& config = config::current();

    // flat-thunk: the period tools from the settings, the SDK prototypes read from the headers when first asked.
    // (Each command's surroundings are found when that command runs, not for every command: no start-up cost for the others.)
    lazy_command(table, "flat-thunk", [corpus, &config] {
        build::flat_thunk::Services ft;
        std::filesystem::path reference;
        try {
            reference = corpus->reference();
        } catch (const Error&) {   // no reference folder: flat-thunk says so when it needs one
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
        return build::flat_thunk::command(std::move(ft));
    });

    // driver-build: OpenWatcom's wasm/wlink against the Windows 95 DDK's 32-bit includes; originals from the corpus
    lazy_command(table, "driver-build", [corpus, &config] {
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
        return build::driver::command(dr);
    });

    // convert FILE [--out DIR]: the window's pipeline on the command line; app: the window itself
    table.set("convert", [](std::span<const std::string> args) -> int { return convert::run_command(args); });
    table.set("app", [](std::span<const std::string> args) -> int { return window::run(args, false); });

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
    const std::filesystem::path interrupts = config.checkout_root() / "data" / "symbols" / "dos_interrupts.json";
    table.set("dos-interrupts", [interrupts](std::span<const std::string> args) -> int {
        return symbols::dos_interrupts::run(symbols::dos_interrupts::ServiceTable::load(interrupts), args);
    });

    // dos-functions <program> [--all]: the functions found in a real-mode program, and the interrupt services it calls
    table.set("dos-functions", [corpus, interrupts](std::span<const std::string> args) -> int {
        if (args.empty()) {
            out("usage: codebreaker dos-functions <program> [--all]\n");
            return 2;
        }
        const std::filesystem::path path = corpus->resolve(args[0]);
        const std::string name = fs::to_text(path.filename());
        const formats::dos::Program program = formats::dos::read_program(fs::read_file(path), name);
        const analyze::dos_functions::Analysis analysis = analyze::dos_functions::discover(program);
        const bool all = std::find(args.begin(), args.end(), "--all") != args.end();
        const auto services = symbols::dos_interrupts::ServiceTable::load(interrupts);
        for (const std::string& line : analyze::dos_functions::report(name, program, analysis, all, services)) out("{}\n", line);
        return 0;
    });

    // cab list|extract|create: Microsoft cabinets, with no outside tool
    table.set("cab", [](std::span<const std::string> argv) -> int { return cab_command(argv); });

    // le-source <driver> [--out DIR]: a VxD as NASM source that reassembles to it (built by x86asm; out under state/le_source)
    source::le_writer::Environment le_env;
    le_env.state = config.state();
    table.set("le-source", [le_env](std::span<const std::string> args) -> int {
        return source::le_writer::command(args, le_env, [](std::string_view line) { out("{}\n", line); });
    });

    // pe-source / ne-source <program> [--out DIR] [--keep-stub]: a Windows module as NASM source that reassembles to it; its DOS
    // stub written by the DOS writer (OpenWatcom's wasm/wlink, the probes kept in state/assembly_probes.json)
    const auto stub_finder = [&config] {
        return source::dos_writer::stub_finder(
            source::dos_writer::options_for(ft_openwatcom_of(config), config.state() / "assembly_probes.json"));
    };
    table.set("pe-source", [le_env, stub_finder](std::span<const std::string> args) -> int {
        source::pe_writer::Environment env{le_env.state, stub_finder()};
        return source::pe_writer::command(args, env, [](std::string_view line) { out("{}\n", line); });
    });
    table.set("ne-source", [le_env, stub_finder](std::span<const std::string> args) -> int {
        source::ne_writer::Environment env{le_env.state, stub_finder()};
        return source::ne_writer::command(args, env, [](std::string_view line) { out("{}\n", line); });
    });

    // resources <module> ... [--out DIR]: a module's resources as .rc and files, compiled with OpenWatcom's wrc and compared;
    // without --out into the module's project folder (Converted/<name>/assets/resources/)
    lazy_command(table, "resources", [corpus, &config] {
        build::resources::Environment env;
        env.open_watcom = ft_openwatcom_of(config);
        env.retail = [corpus](std::string_view name) -> std::optional<std::filesystem::path> { return corpus->retail(name); };
        const std::filesystem::path converted = config.path("converted").value_or(config.checkout_root() / "Converted");
        env.project = [converted](const std::filesystem::path& binary) { return base::converted::Store(converted).project(binary); };
        return build::resources::command(env);
    });

    // identify <module> | --learn WHAT=FILE...: a module's runtime-library functions and its compiler (it opens the knowledge base and
    // the corpus's modules when it runs)
    table.set(symbols::identify::command(symbols::Environment{corpus.get(), &config}));

    // versions <old> <new> ...: two builds compared function by function, work carried from the old build's results
    table.set(store::versions_command::command(store::versions_command::Environment{config, corpus}));

    // reach ...: how far CodeBreaker gets with every program of the corpora
    table.set(coverage::reach_command::command(coverage::reach_command::Environment{config, corpus}));

    // vxd, ne-asm, driver-source, dos-source, dos-build, driver-judge: the source lanes' commands (source::commands::Production
    // finds its tools when a command runs)
    const auto production = std::make_shared<source::commands::Production>(config, corpus);
    table.set("vxd", [production](std::span<const std::string> args) -> int { return production->vxd()(args); });
    table.set("ne-asm", [production](std::span<const std::string> args) -> int { return production->ne_asm()(args); });
    table.set("driver-source", [production](std::span<const std::string> args) -> int { return production->driver_source()(args); });
    table.set("dos-source", [production](std::span<const std::string> args) -> int { return production->dos_source()(args); });
    // x86asm, CodeBreaker's own assembler, as nasm.exe was (NASM 3.02 with the MASM mode): `codebreaker nasm -f bin x.asm`
    table.set("nasm", [](std::span<const std::string> args) -> int { return build::x86asm::run_nasm_command(args); });
    table.set(production->dos_build());
    table.set(production->boot_floppy());
    table.set(production->driver_judge());

    // exact <PROJECT> <stage> ...: the exact-match stages (toolchains, library, carve, names, ledger, draft, ...)
    table.set("exact", exact::command::runner(exact::command::Environment{config}));
    table.set(exact::emu::dos_run::command());   // (CodeBreaker's own DOS executive: the seed of the DOSBox-X replacement)

    // analyze <module> ...: the analyses of a module and the facts they file
    {   // observe's runs go out to worker processes when this program hosts them
        std::optional<jobs::WorkerLauncher> workers;
        if (engine::hosting_workers()) workers = jobs::WorkerLauncher::for_this_program();
        table.set(store::analyze_command::command(store::analyze_command::Environment{config, corpus, std::move(workers)}));
    }

    // decompile, build, place: the pipeline commands that are native now (module, scoreboard, measure, teeth, sweep wait for the
    // lane kit's worker processes and its other lanes; match, repair, score16 for their native compile/compare path)
    for (cli::Command& c : source::pipeline::commands(source::pipeline::Environment{config, corpus}))
        table.set(std::move(c));

    // crypto <tool> ...: the Xbox 360 crypto and flash tools (keys from the xbox360_* settings)
    {
        formats::xbox::crypto::Environment crypto_env;
        if (const auto keys = config.path("xbox360_keys")) crypto_env.keys = formats::xbox::crypto::keystore::KeyStore(*keys);
        if (const auto bin = config.path("xbox360_crypto_bin")) crypto_env.bin_dir = *bin;
        if (const auto sdk = config.path("xbox360_sdk")) crypto_env.sdk_bin_dir = *sdk / "bin" / "win32";
        table.set(formats::xbox::crypto::command(crypto_env));
    }
    // xbox-disc, xex-basic, xenon-config, switch-tables, corpus-executables (were the codebreaker_native extension's)
    table.set(formats::xbox::disc_command::command());
    for (cli::Command& c : recompile::xenon_tools::commands(recompile::xenon_tools::Environment{config})) table.set(std::move(c));
    table.set(base::corpus_executables::command(base::corpus_executables::Environment{corpus}));

    // recompile, recompile-project, xenon-recompile: Xbox 360 PowerPC code as C/C++ and the projects around it
    table.set("recompile", recompile::command::recompile_runner(recompile::command::Environment{config}));
    table.set("recompile-project", recompile::command::recompile_project_runner(recompile::command::Environment{config}));
    table.set("xenon-recompile", recompile::command::xenon_recompile_runner(recompile::command::Environment{config}));

    // classes, xdk-match: what a PowerPC program's run-time type information and the XDK's libraries name
    table.set(symbols::program_commands::classes_command(symbols::program_commands::Environment{config}));
    table.set("xdk-match", symbols::program_commands::xdk_match_runner(symbols::program_commands::Environment{config}));

    // java <tool> ..., xna ...: the Java and XNA lanes' tools
    table.set("java", java::tools::runner(java::tools::Environment{config}));
    table.set("xna", xna::command::runner(xna::command::Environment{config}));

    // mcp, serve: the MCP server and the local service the terminal UI talks to (the wire protocol the UI client speaks)
    table.set(ui::mcp_command::command(ui::mcp_command::Environment{config, corpus, {}}));
    table.set(ui::serve::command(ui::serve::Environment{config, corpus, {}}));

    // invert ...: the inverse models (Microsoft C 6, MSVC 2, early C) and the module pass, with the period compilers
    lazy_runner(table, "invert", [&config, corpus] { return invert::command::runner(invert::command::production(config, corpus)); });

    // cache, tidy-state: the result cache (native and older state files) and state/'s clean-up
    table.set(base::cache::command(base::cache::CommandEnvironment{base::cache::settings_of(config), {}}));
    table.set(base::tidy_state::command(base::tidy_state::environment_of(config)));
    table.set(verify::regress::command(config));

    // settings: the effective settings, where each comes from
    table.set(base::settings::command(base::settings::Environment{&config}));

    // xex <command> <file.xex> [args]: the xex engine's command line, in-process
    formats::xex::Tools xex_tools;
    xex_tools.unpacked_dir = config.state() / "unpacked" / "xex";
    table.set("xex", [](std::span<const std::string> args) -> int {
        flush_out();
        return formats::xex::run(std::vector<std::string>(args.begin(), args.end()));
    });

    // xex-imports, xex-edit, ppc-calls, system-ui: Xbox 360 modules through the xex engine; imports named from the console's ordinals
    table.set(formats::xex_imports::command({xex_tools, console_ordinal_names(config)}));
    table.set(build::xex_edit::command({xex_tools}));
    table.set(analyze::ppc_calls::command({xex_tools, console_ordinal_names(config)}));
    table.set(formats::xbox::system_ui::command({}));

    // disc, media, gfx, fonts: disc images, console media packages, graphics and font files
    table.set(build::disc_image::command());
    table.set(formats::media::command());
    table.set(formats::gfx::command());
    table.set(formats::fonts::command(formats::fonts::Environment{
        [corpus](std::string_view name) -> std::optional<std::filesystem::path> { return corpus->retail(name); }}));

    // build16: a Win16 module built with the period toolchain (Microsoft C 6 under DOSBox-X), compared with the original
    table.set(build::period16::command(build::period16::Environment{config, corpus}));

    // names, knowledge, coverage, progress, facts, origin, driver-services, migrate-state: over the state folder (state_commands.hpp)
    for (cli::Command& c : state_commands(config, corpus)) table.set(std::move(c));
    table.set(translate::judge_c_java::command(translate::judge_c_java::Environment{config}));
    table.set(translate::judge_cs_cpp::command(translate::judge_cs_cpp::Environment{config}));
    table.set(translate::judge_py_cpp::command(translate::judge_py_cpp::Environment{config}));

    // heavyiron <archive> ...: Heavy Iron Studios' .HO/.HIP archives
    table.set("heavyiron", [](std::span<const std::string> args) -> int {
        return formats::xbox::heavyiron::command(std::vector<std::string>(args.begin(), args.end()));
    });

    // the verbs over everything above: convert, inspect, extract, build, check, compare, corpus, lookup, xbox (and cache taking in tidy-state, migrate-state)
    verbs::register_verbs(table);
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

// cb/app/native_commands.cpp -- see native_commands.hpp.
#include "cb/app/native_commands.hpp"

#include <algorithm>
#include <array>
#include <vector>

#include "cb/analyze/dos_functions.hpp"
#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/build/driver.hpp"
#include "cb/build/flat_thunk.hpp"
#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/cab.hpp"
#include "cb/formats/detect.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/formats/xbox/heavyiron.hpp"
#include "cb/formats/xex.hpp"
#include "cb/source/le_writer.hpp"
#include "cb/symbols/demangle.hpp"
#include "cb/symbols/dos_interrupts.hpp"
#include "cb/symbols/driver_services.hpp"
#include "cb/symbols/prototypes.hpp"

namespace cb::app {

namespace {

constexpr std::array<std::string_view, 10> kNative{"info", "flat-thunk", "driver-build", "demangle", "dos-interrupts", "heavyiron", "dos-functions", "cab", "xex", "le-source"};

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

    // le-source <driver> [--out DIR]: a VxD as NASM source that reassembles to it (the nasm setting; out under state/le_source)
    source::le_writer::Environment le_env;
    if (const auto nasm = config.path("nasm"); nasm && std::filesystem::is_regular_file(*nasm)) le_env.nasm = *nasm;
    le_env.state = config.state();
    table.set("le-source", [le_env](std::span<const std::string> args) -> int {
        return source::le_writer::command(args, le_env, [](std::string_view line) { out("{}\n", line); });
    });

    // xex <command> <file.xex> [args]: the xex program, with this process's stdout and stderr
    formats::xex::Tools xex_tools;
    xex_tools.xex_program = config.path("xex_bin").value_or(std::filesystem::path{}) / "xex.exe";
    xex_tools.unpacked_dir = config.state() / "unpacked" / "xex";
    table.set("xex", [xex_tools](std::span<const std::string> args) -> int {
        flush_out();
        return formats::xex::run(xex_tools, std::vector<std::string>(args.begin(), args.end()));
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

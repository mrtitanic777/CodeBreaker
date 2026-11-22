// `module <module>...` and `scoreboard`: reconstruct, judge and write a module's source file (source/module_writer writes it; this is the preparation
// the writer is handed -- finding the module, reconstructing it, naming what the binary names, the readable pass -- and the files' place in the tree).
#include <chrono>
#include <ctime>
#include <fstream>

#include "analyze/call_graph.hpp"
#include "analyze/classes.hpp"
#include "analyze/conventions.hpp"
#include "analyze/globals.hpp"
#include "analyze/report_internal.hpp"
#include "analyze/strings.hpp"
#include "analyze/structures.hpp"
#include "core/fs.hpp"
#include "core/sys.hpp"
#include "core/log.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"
#include "formats/powerpc_image.hpp"
#include "java/production.hpp"
#include "source/java_writer.hpp"
#include "verify/lanes/java.hpp"
#include "formats/xex.hpp"
#include "source/classes_writer.hpp"
#include "source/module_writer.hpp"
#include "source/pipeline_world.hpp"
#include "source/placement.hpp"
#include "source/readable.hpp"
#include "symbols/workbench.hpp"

namespace cb::source::pipeline {

namespace stdfs = std::filesystem;
namespace mw = source::module_writer;
namespace mf = translate::backends::module_file;

namespace {

// ---- small things ------------------------------------------------------------------------------------------------------------

std::string local_time(const char* format) {
    const std::time_t now = std::time(nullptr);
    std::tm local{};
    local = cb::sys::local_tm(now);
    char buffer[64];
    std::strftime(buffer, sizeof buffer, format, &local);
    return buffer;
}

// A path as compared on Windows: absolute, one kind of slash, no case.
std::string compared_path(const stdfs::path& p) {
    std::error_code ec;
    std::string text = text::lower(fs::to_text(stdfs::absolute(p, ec).lexically_normal()));
    std::replace(text.begin(), text.end(), '\\', '/');
    return text;
}

// Is the file one of the selected product's originals?
bool in_corpus(base::Corpus& corpus, const stdfs::path& file) {
    if (file.empty()) return false;
    std::string root = compared_path(corpus.root());
    if (root.empty()) return false;
    if (!root.ends_with('/')) root += '/';
    return compared_path(file).starts_with(root);
}

// Is the file one CodeBreaker wrote (this tool or the earlier one) -- and so may be written again? A file anyone wrote by hand is never replaced. A
// file that is not there is ours to write.
bool made_by_us(const stdfs::path& file) {
    std::ifstream in(file, std::ios::binary);
    if (!in) return true;
    std::string head(600, '\0');
    in.read(head.data(), static_cast<std::streamsize>(head.size()));
    head.resize(static_cast<std::size_t>(in.gcount()));
    static constexpr std::string_view kOurs[] = {"reconstructed source", "behavioral reconstruction", "authored-source reconstruction",
                                                 "(functional group, reconstructed)", "decompiled by CodeBreaker", "by CodeBreaker"};
    return std::any_of(std::begin(kOurs), std::end(kOurs), [&](std::string_view mark) { return head.find(mark) != std::string::npos; });
}

// A module written as its assembly file: a driver, a real-mode program, a cartridge, a snapshot or a tape.
bool written_as_assembly(std::string_view kind) {
    if (kind == "le" || kind == "dos") return true;
    engine::register_all_lanes();
    const auto lanes = engine::registered_lanes();   // (every platform lane is a machine whose programs come back as assembly: engine/lane_registry.hpp)
    return std::any_of(lanes.begin(), lanes.end(), [&](const auto& lane) { return lane->name == kind; }) || engine::machine_lanes().find(kind) != nullptr;
}

mf::Lane lane_of_kind(std::string_view kind) {
    if (kind == "ne") return mf::Lane::Ne;
    if (kind == "powerpc") return mf::Lane::PowerPc;
    if (kind == "le") return mf::Lane::Le;
    if (written_as_assembly(kind)) return mf::Lane::Dos;   // (the head of an assembly file: nothing in it says which machine)
    return mf::Lane::Pe;
}

// ---- what a module's preparation reads -------------------------------------------------------------------------------------------

// A comment block describing the module's global variables, from the globals analysis: for each global the functions refer to by its address, the
// type recovered from how it is used, its initial value, and how many functions read and write it. Nothing is renamed -- the globals keep the
// macros the decompiler bound to their original addresses -- so this only adds information. "" when there is nothing to say.
std::string globals_note(analyze::Image& image, std::string_view header) {
    static const Regex macro(R"(\b[a-z]+Ram([0-9a-f]{8})\b)");
    std::set<std::uint64_t> addresses;
    macro.for_each_match(header, [&](const RegexMatch& m) {
        if (const auto a = text::parse_int<std::uint64_t>(m.group(1), 16)) addresses.insert(*a);
        return true;
    });
    if (addresses.empty()) return "";
    std::string lines;
    try {
        const analyze::call_graph::Graph graph = analyze::call_graph::build(image);
        const analyze::strings::Table table = analyze::strings::build_table(image, graph);
        const std::map<analyze::Va, analyze::globals::Variable> variables = analyze::globals::variables(image, graph, table);
        for (const std::uint64_t a : addresses) {
            const auto found = variables.find(static_cast<analyze::Va>(a));
            if (found == variables.end()) continue;
            const analyze::globals::Variable& g = found->second;
            std::string initial;   // a label, "0x.." for a value, "uninitialised"
            switch (g.initial.kind) {
                case analyze::globals::Initial::Kind::Uninitialised: initial = "uninitialised"; break;
                case analyze::globals::Initial::Kind::Value: initial = std::format("0x{:x}", g.initial.value); break;
                case analyze::globals::Initial::Kind::Address: initial = g.initial.label; break;
            }
            std::string tail;
            if (initial == "uninitialised") tail = "  uninitialised";
            else if (!initial.empty()) tail = "  initially " + initial.substr(0, 24);
            lines += std::format(" *   {:08x}  {:<34} read by {}, written by {}{}\n", a, g.type, g.readers.size(), g.writers.size(), tail);
        }
    } catch (const Error&) {   // an analysis that cannot read the module says nothing
        return "";
    }
    if (lines.empty()) return "";
    lines.pop_back();
    return "\n/* globals (type recovered from use; still at the original's address):\n" + lines + " */\n";
}

// The imports of a PowerPC image that can be written by their real name: the libraries it imports from are in the .xex header when the image came
// from one, else the kernel and the system library, in the order every image seen has them; the numbers of the console's system libraries are the
// knowledge base's.
std::map<std::string, std::string> console_imports(knowledge::Knowledge& kb, const stdfs::path& module) {
    const std::vector<std::string> libraries = formats::xex::import_libraries(module).value_or(std::vector<std::string>{"xboxkrnl.exe", "xam.xex"});
    std::vector<std::map<std::string, std::string>> numbers;
    for (const std::string& library : libraries) numbers.push_back(kb.export_names("xbox360", text::lower(library)));
    std::set<std::string> distinct;
    const formats::powerpc_image::Image image = formats::powerpc_image::Image::read(module);
    for (const auto& [address, symbol] : image.imports()) distinct.insert(symbol);
    const std::vector<std::string> symbols(distinct.begin(), distinct.end());
    return mw::console_import_names(symbols, numbers, {});
}

}  // namespace

// ---- module --------------------------------------------------------------------------------------------------------------------

namespace {

// What the description of a 16-bit module asks of the symbol tables, answered by the ordinals: the C name an export of a module has by its number.
class OrdinalKnowledge final : public decompile::describe::Knowledge {
public:
    explicit OrdinalKnowledge(const symbols::ordinals::Ordinals& ordinals) : ordinals_(ordinals) {}
    std::optional<std::string> import_name(std::string_view module, std::uint32_t ordinal) override { return ordinals_.identifier(module, ordinal); }

private:
    const symbols::ordinals::Ordinals& ordinals_;
};

struct ModuleRun {
    World& world;
    stdfs::path path;
    std::string kind;
    mw::Log log;
    bool verify = true;
    bool build_whole = false;

    // The symbol tables of the run (the knowledge base, the ordinals, the namer): opened when first asked for.
    std::shared_ptr<symbols::Workbench> workbench() {
        if (!bench_) bench_ = std::make_shared<symbols::Workbench>(symbols::Environment{&world.corpus(), &world.config()});
        return bench_;
    }

    // The readable pass needs the SDK's headers: read when a module with C to read first asks, once for the run.
    std::shared_ptr<readable::Sdk> sdk() {
        if (!sdk_) {
            const symbols::prototypes::HeaderFolders folders = symbols::prototypes::HeaderFolders::standard(
                build::watcom::OpenWatcom::from_settings(world.config().text("openwatcom")).root(), world.corpus().reference());
            sdk_ = std::make_shared<readable::Sdk>(readable::Sdk::read(folders, world.kit().knowledge().prototypes()));
        }
        return sdk_;
    }

    // The names a binary gives its functions that the decompiler left as `sub_...`, and the rest of what the writer is handed.
    mw::Prepared prepare(std::string_view sha, std::string_view profile) {
        const base::Corpus& corpus = world.corpus();
        const std::optional<base::Lane> lane = corpus.lane(path);
        mw::Prepared p;
        p.name = text::lower(fs::to_text(path.filename()));
        p.stem = fs::to_text(path.stem());
        p.stem = text::lower(p.stem);
        p.module = std::string(sha);
        p.profile = std::string(profile);
        p.date = local_time("%Y-%m-%d");
        p.source_engine = std::string(verify::reconstruct::kSource);
        p.lane = lane_of_kind(kind);
        p.flat = kind == "pe";
        if (written_as_assembly(kind)) return p;

        const base::Lane key_lane = lane.value_or(base::Lane::Pe);
        const auto keyed = [key_lane](core16::Va address) { return symbols::names::function_key(key_lane, static_cast<symbols::names::Address>(address)); };
        p.address_key = keyed;

        // the decorated names of C++ exports, by the store's key
        try {
            if (lane == base::Lane::Pe) {
                for (const auto& [address, name] : decompile::describe::decorated_names(formats::pe::Image::open(path))) p.exported[keyed(address)] = name;
            } else if (lane == base::Lane::Ne) {
                for (const auto& [address, name] : decompile::describe::decorated_names(formats::ne::Module::open_file(path))) p.exported[keyed(address)] = name;
            }
        } catch (const Error&) {   // a file whose export table cannot be read has none
        }

        // The symbol tables of this run: the names the shipped tables and the corpus's own exports give a function by its number, the names of
        // functions that are the same body in another module.
        const std::shared_ptr<symbols::Workbench> bench = workbench();

        // imports by number that can be written by their real name (a 32-bit module's are already named in the decompiled text)
        try {
            if (kind == "powerpc") {
                p.found_imports = console_imports(world.kit().knowledge(), path);
            } else if (lane == base::Lane::Ne) {
                OrdinalKnowledge names(bench->ordinals());
                p.found_imports = decompile::describe::import_names16(formats::ne::Module::open_file(path), names);
            }
        } catch (const Error&) {
        }

        // a recovered name for an internal function (a name it shares with a named function in another module, a runtime-library name, or one
        // from a debug symbol file) replaces the decompiler's sub_... placeholder everywhere the module uses it
        try {
            for (const symbols::names::FunctionName& f : bench->namer().of_module(path).functions)
                if (f.source != "(none)" && !f.name.starts_with("sub_")) p.recovered[f.key] = f.name;
        } catch (const Error&) {
        }

        // the module as the analyses see it: its strings, its globals, its classes
        const analyze::report::Environment analysis_environment = [this] {
            analyze::report::Environment e;
            e.state = world.config().state();
            e.knowledge = &world.kit().decompiler().context().environment->knowledge();
            return e;
        }();
        std::shared_ptr<analyze::Image> image;
        if (kind == "pe" || kind == "ne") {
            try {
                image = analyze::report::detail::load_module(path, analysis_environment).image;
            } catch (const Error&) {   // a module the analyses cannot read gets no strings, no note on its globals, no classes
            }
        }
            readable::StringAt string_at = [image](std::uint32_t address) -> std::optional<std::string> {
            if (!image) return std::nullopt;
            try {
                if (const auto found = analyze::strings::string_at(*image, static_cast<analyze::Va>(address))) return found->text;
            } catch (const Error&) {
            }
            return std::nullopt;
        };
        if (image) p.globals_note = [image](const std::string& header) { return globals_note(*image, header); };

        // the C++ classes
        if (kind == "pe" && image) {
            try {
                const analyze::call_graph::Graph graph = analyze::call_graph::build(*image);
                const analyze::conventions::Report conventions = analyze::conventions::analyse(*image, graph);
                const analyze::ConventionFacts summary = analyze::conventions::summary(conventions.facts);
                const std::vector<analyze::structures::Structure> structures = analyze::structures::recover(*image, graph, summary);
                const analyze::classes::Model model = analyze::classes::build_model(analyze::classes::Inputs{*image, graph, summary});
                for (const analyze::classes::Class& c : analyze::classes::hierarchy(*image, model, structures)) p.classes.push_back(c);
            } catch (const Error&) {
            }
            try {
                auto pe = std::make_shared<formats::pe::Image>(formats::pe::Image::open(path));
                auto thunks = std::make_shared<classes_writer::ImportThunks>(*pe);
                p.import_behind = [pe, thunks, bench](core16::Va address) { return thunks->behind(address, bench->ordinals()); };
            } catch (const Error&) {
            }
        }

        // the readable pass over the module as the store holds it
        const std::shared_ptr<readable::Sdk> shared_sdk = sdk();
        p.readable = [shared_sdk, string_at](const translate::l2::Made& stored) { return readable::make_pass(*shared_sdk, stored, string_at); };

        if (build_whole) {
            const build::module::ModuleBuilder builder(world.kit().module_services());
            build::module::BuildOptions options;
            options.outdir = out_dir / "build";
            options.call = false;
            options.log = [](std::string_view) {};
            const build::module::Built b = builder.build(fs::to_text(path), options);
            mf::Whole whole;
            whole.linked = b.linked();
            whole.error = b.error;
            whole.exports_same = b.exports_same;
            whole.exports_total = b.exports_total;
            whole.rejected = b.rejected.size();
            whole.absent = b.absent.size();
            whole.built = fs::to_text(out_dir / "build" / (p.stem + fs::to_text(path.extension())));
            p.whole = whole;
        }
        return p;
    }

    stdfs::path out_dir;

    std::shared_ptr<readable::Sdk> sdk_;
    std::shared_ptr<symbols::Workbench> bench_;
    std::vector<std::shared_ptr<void>> keep_;
};

void write_one(World& world, const std::string& module, bool verify, std::optional<stdfs::path> out_dir, std::size_t split, bool build_whole) {
    base::Corpus& corpus = world.corpus();
    const mw::Log log = [](std::string_view line) { cb::out("{}\n", line); };
    const std::optional<stdfs::path> path = world.find(module);
    if (!path) throw UsageError("no module '" + module + "' in the corpus");
    std::error_code ec;
    if (stdfs::is_directory(*path, ec) || text::iends_with(fs::to_text(*path), ".java") || text::iends_with(fs::to_text(*path), ".jar")) {
        // a Java program: the C++ it transpiles to, class by class, each with what the judge found (source/java_writer)
        const verify::reconstruct::Reconstructor& rec = world.reconstructor();
        std::string sha, profile;
        if (verify) {
            const verify::reconstruct::Outcome done = rec.run(*path);
            sha = done.sha;
            profile = done.profile;
        } else {
            const verify::reconstruct::Identity id = rec.identity(*path);
            sha = id.sha;
            profile = id.profile;
        }
        stdfs::path folder = *path;
        while (!folder.empty() && !folder.has_filename()) folder = folder.parent_path();
        const std::string name = fs::to_text(folder.filename());
        const stdfs::path state_folder = world.config().state(stdfs::path("source") / name);
        const stdfs::path out = out_dir.value_or(state_folder);
        const java::JavaData data(world.config().checkout_root() / "config" / "java");
        const std::unique_ptr<java::assembly::Assembly> assembly = java::production::make_assembly(world.config(), data);
        const stdfs::path java_dir = verify::lanes::JavaLane::source_dir(*path);
        const source::java_writer::Conversion convert = [&](const stdfs::path& into) { (void)assembly->converter(std::string(verify::lanes::JavaLane::kCppStandard))(java_dir, into); };
        (void)source::java_writer::write_java(name, world.store(), sha, profile, out, state_folder, convert, log);
        return;
    }

    const std::string name = text::lower(fs::to_text(path->filename()));
    const std::string stem = text::lower(fs::to_text(path->stem()));
    // A product's source tree is for that product's modules. A file from anywhere else (given by path: another product's, a PowerPC image) is
    // written under <state>/source/ here.
    stdfs::path out;
    if (out_dir) {
        out = *out_dir;
    } else if (in_corpus(corpus, *path)) {
        const stdfs::path table_file = world.config().checkout_root() / "config" / "source" / "placement.json";
        const placement::Placement where(corpus.design_dir().parent_path(), placement::load_table(table_file, corpus.current()));
        out = where.folder(name, *path, corpus.lane(*path));
    } else {
        out = world.config().state(stdfs::path("source") / stem);
    }
    std::vector<std::string> held;   // someone wrote these by hand: not replaced
    for (const std::string& f : {stem + ".c", stem + ".h"})
        if (!made_by_us(out / f)) held.push_back(f);
    if (!held.empty()) {
        log(std::format("{}: {} written by hand -- the reconstruction goes to the state folder instead", fs::to_text(out), text::join(held, ", ")));
        out = world.config().state(stdfs::path("source") / stem);
    }
    fs::make_dirs(out);

    const verify::reconstruct::Reconstructor& rec = world.reconstructor();
    std::string sha, profile;
    if (verify) {
        const verify::reconstruct::Outcome done = rec.run(*path);
        sha = done.sha;
        profile = done.profile;
    } else {
        const verify::reconstruct::Identity id = rec.identity(*path);
        sha = id.sha;
        profile = id.profile;
    }
    ModuleRun run{world, *path, rec.kind_name(*path).value_or(""), log, verify, build_whole, out};
    const mw::Prepared prepared = run.prepare(sha, profile);
    if (written_as_assembly(run.kind)) {
        (void)mw::write_assembly(prepared, world.store(), out, log);
        return;
    }
    (void)mw::write_module(prepared, world.store(), out, split, log);
}

// SCOREBOARD.md from the store: modules judged under the CURRENT verifier settings of their kind (32-bit, 16-bit, PowerPC, driver).
stdfs::path write_scoreboard(World& world, const std::optional<stdfs::path>& out_dir, const mw::Log& log) {
    base::Corpus& corpus = world.corpus();
    engine::FunctionStore& store = world.store();
    std::map<std::string, std::string> current;   // digest -> kind
    for (const auto& [kind, digest] : world.reconstructor().digests())
        if (digest && !digest->empty()) current[*digest] = kind;
    std::vector<std::pair<std::string, std::string>> by_kind;   // (kind, digest), sorted by kind as the heading says them
    for (const auto& [digest, kind] : current) by_kind.emplace_back(kind, digest);
    std::sort(by_kind.begin(), by_kind.end());
    std::vector<std::string> settings;
    for (const auto& [kind, digest] : by_kind) settings.push_back(kind + " " + digest);

    std::map<std::string, std::string> where;   // module hash -> path
    for (const engine::ModuleRow& row : store.module_rows()) where[row.sha] = row.path;
    struct Row {
        std::string name, kind, sha, digest;
    };
    std::vector<Row> rows;
    for (const auto& [digest, kind] : current)
        for (const engine::ModuleRow& m : store.modules(digest)) {
            const auto path = where.find(m.sha);
            if (in_corpus(corpus, path == where.end() ? stdfs::path() : fs::from_text(path->second))) rows.push_back({m.name, kind, m.sha, digest});
        }
    std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) { return std::tie(a.name, a.kind, a.sha, a.digest) < std::tie(b.name, b.kind, b.sha, b.digest); });

    mw::ScoreboardInput in;
    in.generated = local_time("%Y-%m-%d %H:%M");
    in.settings = text::join(settings, ", ");
    for (const Row& r : rows) in.modules.push_back({r.name, r.kind, store.scores(r.digest, r.sha)});
    in.corpus_32 = corpus.modules(base::Lane::Pe).size();
    in.corpus_16 = corpus.modules(base::Lane::Ne).size();
    const stdfs::path file = (out_dir ? *out_dir : corpus.design_dir().parent_path()) / "SCOREBOARD.md";
    fs::write_text(file, mw::scoreboard(in));
    log(std::format("wrote {} ({} modules)", fs::to_text(file), rows.size()));
    return file;
}

}  // namespace

cli::Command module(const Environment& environment) {
    cli::Command declared{"module",
                          "reconstruct, judge and write a module's source file, every function labelled with what is known",
                          {cli::Positional{"module", "a corpus name, or a path", true, true},
                           cli::Option{"--out", "", "write there instead of the source tree", cli::Arg::Value, "DIR"},
                           cli::Option{"--split", "", "at most this many functions to a file", cli::Arg::Value, "N"},
                           cli::Option{"--no-verify", "", "write from what the store already holds", cli::Arg::Flag},
                           cli::Option{"--build", "", "also build the whole module and report it", cli::Arg::Flag},
                           cli::Option{"--scoreboard", "", "also rewrite the scoreboard", cli::Arg::Flag}},
                          {},
                          {},
                          "Reconstruct",
                          "Each function carries one status line: VERIFIED, VERIFIED (weak), UNVERIFIED, NOT JUDGED, DOES NOT COMPILE, NO SOURCE, or IDENTIFIED "
                          "(runtime library). Imports by number are written with their real names where the exporting module gives one; a C++ export is shown "
                          "with the declaration its decorated name stands for. A driver or a real-mode program is written as its assembly file. Nothing is "
                          "counted as progress except what these labels say."};
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) {
        world.verbose = true;
        std::optional<stdfs::path> out;
        if (const auto v = args.value("out")) out = fs::from_text(*v);
        const std::size_t split = args.integer<std::size_t>("split").value_or(0);
        bool scoreboard = args.flag("scoreboard");
        for (const std::string& module : args.values("module")) {
            write_one(world, module, !args.flag("no-verify"), out, split, args.flag("build"));
            std::error_code ec;
            if (!stdfs::exists(fs::from_text(module), ec)) scoreboard = true;                       // a corpus name
            else if (in_corpus(world.corpus(), fs::from_text(module))) scoreboard = true;           // (not for a file that is not the product's)
        }
        if (scoreboard) (void)write_scoreboard(world, out, [](std::string_view line) { cb::out("{}\n", line); });
        return 0;
    });
}

cli::Command scoreboard(const Environment& environment) {
    cli::Command declared{"scoreboard", "rewrite SCOREBOARD.md from the function store", {cli::Option{"--out", "", "write there instead of beside the tree", cli::Arg::Value, "DIR"}},
                          {}, {}, "Reconstruct",
                          "Only modules measured on the current verifier are listed. Nothing is counted as progress except what the labels say."};
    return in_world(std::move(declared), environment, [](World& world, const cli::Args& args) {
        std::optional<stdfs::path> out;
        if (const auto v = args.value("out")) out = fs::from_text(*v);
        (void)write_scoreboard(world, out, [](std::string_view line) { cb::out("{}\n", line); });
        return 0;
    });
}

}  // namespace cb::source::pipeline

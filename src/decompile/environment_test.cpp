// cb-unity: alone (helpers named for what they do here)
// Tests of cb/decompile/environment.hpp on real modules (formats/testdata, symbols/testdata): LZ32.DLL (a 32-bit library that exports 12
// functions by name, with 10 imports), LZEXPAND.DLL (a 16-bit library), and natalsu.exe (a PowerPC image). The expected values were
// recorded from the same files (the argument tables, the OpenWatcom table, the analysis, the names and the driver services)
// when this was written.
//
// The last test is the proof the module was written for: with the environment over the knowledge base, the Decompiler writes the C of five
// real modules byte for byte as recorded. It runs only where CB_PROOF_EXPECTED names the folder of the expected files
// (<module>.c, from `codebreaker decompile <module> --out`), and the checkout's own settings find the corpus and the programs.
#include "decompile/environment.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>

#include "base/config.hpp"
#include "base/converted.hpp"
#include "build/watcom.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/detect.hpp"
#include "knowledge/store.hpp"

namespace cb::decompile::environment {
namespace {

namespace api = symbols::api_arguments;
namespace dsc = describe;
namespace stdfs = std::filesystem;

// ---- the corpus of the tests --------------------------------------------------------------------------------------------------------

struct EnvWorld {
    fs::ScopedTempDir dir{"cb-envir"};
    stdfs::path folder = dir.path() / "files";
    std::unique_ptr<base::Corpus> corpus;
    std::unique_ptr<base::Corpus::Selected> selected;

    EnvWorld() {
        fs::make_dirs(folder);
        for (const char* name : {"lz32.dll", "lzexpand.dll"}) fs::write_file(folder / name, fs::read_file(testing::data_path(std::string("formats/testdata/") + name)));
        base::Products products;
        products.target = "test";
        products.target_folder = folder;
        products.listed["test"] = folder;
        products.checkout_root = stdfs::path(CB_SOURCE_DIR).parent_path();
        products.state = dir.path() / "state";
        base::FormatProbes probes;
        probes.lane = [](const stdfs::path& p) -> std::optional<base::Lane> {
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
        corpus = std::make_unique<base::Corpus>(products, probes);
        selected = std::make_unique<base::Corpus::Selected>(*corpus, "test");
    }
    stdfs::path lz32() const { return folder / "lz32.dll"; }
};

// The sizes the headers and earlier scans gave, in memory.
class EnvWin32Memory final : public api::Win32Memory {
public:
    std::map<std::string, std::uint32_t> headers;
    std::map<std::string, api::Known> found;
    std::optional<std::uint32_t> header_bytes(std::string_view name) override {
        const auto it = headers.find(std::string(name));
        return it == headers.end() ? std::nullopt : std::optional<std::uint32_t>(it->second);
    }
    std::optional<api::Known> scanned(std::string_view stem, std::string_view name) override {
        const auto it = found.find(std::string(stem) + "." + std::string(name));
        return it == found.end() ? std::nullopt : std::optional<api::Known>(it->second);
    }
    void remember(std::string_view stem, std::string_view name, std::optional<std::uint32_t> bytes) override {
        found[std::string(stem) + "." + std::string(name)] = api::Known{bytes};
    }
};

class EnvWin16Memory final : public api::Win16Memory {
public:
    std::map<std::string, std::optional<std::uint32_t>> kept;
    std::optional<api::Known> remembered(std::string_view key) override {
        const auto it = kept.find(std::string(key));
        return it == kept.end() ? std::nullopt : std::optional<api::Known>(api::Known{it->second});
    }
    std::map<std::string, std::optional<std::uint32_t>> table() override { return kept; }
    void remember(std::string_view key, std::optional<std::uint32_t> bytes, std::string_view) override { kept[std::string(key)] = bytes; }
};

const translate::native_l2::Library& unused_library() {
    static const translate::native_l2::Library library(
        [](const char*, const char*, std::size_t, char**, std::size_t*) -> int { return 1; }, [](char*) {});
    return library;
}

// ---- the modules ---------------------------------------------------------------------------------------------------------------------

CB_TEST("decompile.environment.scope_of_the_sixteen_bit_sizes", [] {
    EnvWorld w;
    const std::string scope = win16_scope(*w.corpus);
    CB_CHECK(scope.starts_with("test:"));
    CB_CHECK(scope.ends_with("|"));
    CB_CHECK_EQ(scope.size(), std::string("test:").size() + 10 + 1);
    CB_CHECK_EQ(scope, win16_scope(*w.corpus));
});

CB_TEST("decompile.environment.modules_of_the_corpus", [] {
    EnvWorld w;
    CorpusModules modules(*w.corpus);
    // a 32-bit module by its stem
    const auto lz = modules.find("LZ32");
    CB_CHECK(lz != nullptr);
    CB_CHECK(lz->export_by_name("LZRead").has_value());
    CB_CHECK_EQ(lz->export_by_name("LZRead").value_or(0), 0x1363u);
    CB_CHECK(modules.find("LZ32") == lz);   // opened once
    CB_CHECK(modules.find("NOSUCH") == nullptr);
    CB_CHECK(modules.find("LZEXPAND") == nullptr);   // (a 16-bit module does not read as a PE)
    // how a 16-bit export returns
    const auto far_export = modules.find("LZEXPAND", 1);
    CB_CHECK(far_export.has_value());
    CB_CHECK(far_export->what.has_value());
    CB_CHECK(far_export->what->far_return);
    CB_CHECK_EQ(far_export->what->pop, 4);
    CB_CHECK(far_export->file.ends_with("lzexpand.dll"));
    const auto missing_entry = modules.find("LZEXPAND", 99);
    CB_CHECK(missing_entry.has_value());
    CB_CHECK(!missing_entry->what.has_value());   // the module is there, the export is not
    CB_CHECK(!modules.find("NOSUCH", 1).has_value());
    // the names the modules' own tables give their exports
    const auto names16 = modules.table("LZEXPAND");
    CB_CHECK(names16.has_value());
    CB_CHECK_EQ(names16->names.at(11), "WEP");
    CB_CHECK_EQ(names16->file, "lzexpand.dll");
    const auto names32 = modules.table("LZ32");
    CB_CHECK(names32.has_value());
    CB_CHECK_EQ(names32->names.size(), 12u);
    CB_CHECK_EQ(names32->names.at(5), "LZRead");
    CB_CHECK_EQ(names32->names.at(12), "GetExpandedNameW");
    CB_CHECK_EQ(names32->file, "lz32.dll");
    CB_CHECK(!modules.table("NOSUCH").has_value());
});

// ---- what describing a module asks -----------------------------------------------------------------------------------------------------

struct EnvOver {
    EnvWorld world;
    EnvWin32Memory win32;
    EnvWin16Memory win16;
    std::shared_ptr<CorpusModules> modules = std::make_shared<CorpusModules>(*world.corpus);
    symbols::ordinals::Ordinals ordinals{*modules, {}};
    CorpusEnvironment environment{Sources{*world.corpus, unused_library(), win32, win16, ordinals, nullptr, modules}};
};

CB_TEST("decompile.environment.argument_sizes_as_recorded", [] {
    EnvOver o;
    // what LZ32.DLL's own code removes on return, export by export (api_arguments.stack_bytes)
    const std::pair<const char*, std::uint32_t> exports[] = {{"LZCopy", 8},   {"LZOpenFileA", 12}, {"LZInit", 4},         {"LZSeek", 12},
                                                              {"LZRead", 12},  {"LZClose", 4},       {"LZStart", 0},        {"CopyLZFile", 8},
                                                              {"LZDone", 0},   {"GetExpandedNameA", 8}, {"LZOpenFileW", 12}, {"GetExpandedNameW", 8}};
    for (const auto& [name, bytes] : exports) {
        CB_CHECK_EQ(o.environment.argument_bytes("LZ32", name).value_or(999), bytes);
        CB_CHECK_EQ(o.environment.knowledge().win32_argument_bytes("LZ32", name).value_or(999), static_cast<std::int64_t>(bytes));
    }
    CB_CHECK(!o.environment.argument_bytes("LZ32", "NoSuchExport").has_value());
    CB_CHECK(!o.environment.argument_bytes("KERNEL32", "GlobalAlloc").has_value());   // (the corpus here has no KERNEL32)
    CB_CHECK(o.win32.found.contains("LZ32.LZRead"));                                  // remembered for the next ask
    // the headers win where they apply
    o.win32.headers["GlobalAlloc"] = 8;
    CB_CHECK_EQ(o.environment.knowledge().declared_argument_bytes("GlobalAlloc").value_or(0), 8);
    CB_CHECK(!o.environment.knowledge().declared_argument_bytes("Nothing").has_value());
    CB_CHECK_EQ(o.environment.argument_bytes("KERNEL32", "GlobalAlloc").value_or(0), 8u);
    // a 16-bit export, by the name the emulator reports it
    const std::int64_t sixteen[] = {4, 10, 2, 8, 8, 2, 0, 4, 0, 8, 2, 0};   // LZEXPAND ordinals 1 to 12 (api_arguments.arg_bytes)
    for (int ordinal = 1; ordinal <= 12; ++ordinal) {
        CB_CHECK_EQ(o.environment.knowledge().win16_argument_bytes("LZEXPAND_" + std::to_string(ordinal)).value_or(-1), sixteen[ordinal - 1]);
        CB_CHECK_EQ(o.environment.knowledge().win16_argument_bytes("LZEXPAND.@" + std::to_string(ordinal)).value_or(-1), sixteen[ordinal - 1]);
    }
    CB_CHECK(o.win16.kept.contains("LZEXPAND.5"));
    CB_CHECK(!o.environment.knowledge().win16_argument_bytes("NOSUCH_1").has_value());
    CB_CHECK(!o.environment.knowledge().win16_argument_bytes("not an api name").has_value());
});

CB_TEST("decompile.environment.a_module_as_recorded", [] {
    EnvOver o;
    // imports by name: {C name: DLL}
    const std::map<std::string, std::string> imports = o.environment.import_dlls(o.world.lz32());
    const std::map<std::string, std::string> want = {{"DisableThreadLibraryCalls", "KERNEL32"}, {"GetShortPathNameA", "KERNEL32"}, {"GlobalAlloc", "KERNEL32"},
                                                     {"GlobalFree", "KERNEL32"},                {"OpenFile", "KERNEL32"},          {"SMapLS_IP_EBP_12", "KERNEL32"},
                                                     {"SMapLS_IP_EBP_8", "KERNEL32"},           {"SUnMapLS_IP_EBP_12", "KERNEL32"}, {"SUnMapLS_IP_EBP_8", "KERNEL32"},
                                                     {"SetLastError", "KERNEL32"}};
    CB_CHECK(imports == want);
    const std::set<std::string> names = o.environment.knowledge().imported_names(o.world.lz32());
    CB_CHECK_EQ(names.size(), 10u);
    CB_CHECK(names.contains("SMapLS_IP_EBP_8"));
    // an unreadable file has no imports
    CB_CHECK(o.environment.import_dlls(o.world.folder / "no_such.dll").empty());
    // how its functions are called (analyze.conventions): no `this` and no register calls here
    const dsc::Knowledge::Conventions conventions = o.environment.knowledge().conventions(o.world.lz32());
    CB_CHECK(conventions.thiscall.empty());
    CB_CHECK(conventions.fastcall.empty());
    // what an export removes on return (api_arguments._scan on the module being described)
    CB_CHECK_EQ(o.environment.knowledge().pe_function_pop(o.world.lz32(), 0x1123).value_or(-1), 0);
    CB_CHECK_EQ(o.environment.knowledge().pe_function_pop(o.world.lz32(), 0x1198).value_or(-1), 8);
    CB_CHECK_EQ(o.environment.knowledge().pe_function_pop(o.world.lz32(), 0x11a1).value_or(-1), 12);
    CB_CHECK_EQ(o.environment.knowledge().pe_function_pop(o.world.lz32(), 0x11dd).value_or(-1), 4);
    // structures: this module has none (analyze.structures._recover); a file that is no module has no analysis
    const auto structures = o.environment.recover_structures(o.world.lz32());
    CB_CHECK(structures.has_value());
    CB_CHECK(structures->empty());
    CB_CHECK(!o.environment.recover_structures(o.world.folder / "lzexpand.dll").has_value());   // (a 16-bit module: not for this analysis)
    // a driver's service without a table is a placeholder
    CB_CHECK_EQ(o.environment.knowledge().driver_service_label(1, 0), "vxd_1_0");
    CB_CHECK(&o.environment.library() == &unused_library());
});

CB_TEST("decompile.environment.a_powerpc_image", [] {
    EnvOver o;
    const stdfs::path image = testing::data_path("formats/testdata/natalsu.exe");
    const dsc::PowerPcFacts facts = o.environment.powerpc_facts(image);
    CB_CHECK_EQ(facts.spans.size(), 1u);
    CB_CHECK_EQ(facts.spans[0].va, 0x98000000);
    CB_CHECK_EQ(facts.spans[0].length, 0x9000u);
    CB_CHECK_EQ(facts.imports.size(), 28u);
    CB_CHECK_EQ(facts.imports.at(2550162020), "import0_ord_492");
    CB_CHECK_EQ(facts.inlined.size(), 18u);
    CB_CHECK_EQ(facts.inlined.at(2550146960), "__savegprlr_14");
    // what run-time type information and the entry point name (symbols.names.powerpc_names): only the entry point in this image
    CB_CHECK_EQ(facts.names.size(), 1u);
    CB_CHECK_EQ(facts.names.at(2550146728), "entry");
    const std::vector<decompiler::Va> functions = o.environment.powerpc_functions(image);
    CB_CHECK_EQ(functions.size(), 87u);
    CB_CHECK_EQ(functions.front(), 2550145024);
    CB_CHECK(std::is_sorted(functions.begin(), functions.end()));
    CB_CHECK_THROWS_AS(NotFoundError, (void)o.environment.powerpc_facts(o.world.folder / "no_such_image.exe"));
    CB_CHECK_THROWS_AS(FormatError, (void)o.environment.powerpc_functions(o.world.lz32()));   // (an i386 module)
});

// ---- over the knowledge base -------------------------------------------------------------------------------------------------------------

CB_TEST("decompile.environment.over_the_knowledge_base", [] {
    EnvWorld w;
    fs::ScopedTempDir kb_dir("cb-envkb");
    knowledge::Store store(kb_dir.path() / "k.sqlite", knowledge::DataFolders::under(stdfs::path(CB_SOURCE_DIR).parent_path()));
    knowledge::Knowledge kb(store);
    const std::unique_ptr<CorpusEnvironment> environment = CorpusEnvironment::over_knowledge(*w.corpus, unused_library(), kb);
    // a driver's services, as the kit names them (symbols.driver_services.label)
    dsc::Knowledge& known = environment->knowledge();
    CB_CHECK_EQ(known.driver_service_label(1, 0), "Get_VMM_Version");
    CB_CHECK_EQ(known.driver_service_label(1, 1), "Get_Cur_VM_Handle");
    CB_CHECK_EQ(known.driver_service_label(0x1D, 1), "WINDEBUG_Service_0001");
    CB_CHECK_EQ(known.driver_service_label(0x44, 3), "PELOADER_Service_0003");
    CB_CHECK_EQ(known.driver_service_label(0x9999, 7), "VxD_9999_0007");
    CB_CHECK_EQ(known.driver_service_label(1, 0xFFFF), "VMM_Service_7FFF");
    // 32-bit sizes: the headers' where they apply (LZRead is declared: nothing is scanned), else what the exporting module's code says,
    // which goes to the base and is there for the next ask
    CB_CHECK_EQ(environment->argument_bytes("LZ32", "LZRead").value_or(0), 12u);
    CB_CHECK(!store.get("win32", "argument_bytes.code", "LZ32.LZRead").has_value());
    CB_CHECK(environment->knowledge().declared_argument_bytes("LZRead").has_value());
    CB_CHECK_EQ(known.win16_argument_bytes("LZEXPAND_5").value_or(0), 8);
    CB_CHECK(store.get("win16", "argument_bytes", win16_scope(*w.corpus) + "LZEXPAND.5").has_value());
    CB_CHECK_EQ(known.win16_argument_bytes("LZEXPAND_5").value_or(0), 8);   // (the second ask is the base's)
    // the tables of the base as the readers take them
    CB_CHECK(!shipped_tables(kb).empty());
    CB_CHECK(!driver_table(kb).messages().empty());
});

// ---- the proof against the recorded output ---------------------------------------------------------------------------------------------------------

CB_TEST("decompile.environment.proof_decompile_as_recorded", [] {
    const std::optional<std::string> expected_dir = fs::get_env("CB_PROOF_EXPECTED");
    if (!expected_dir) return;   // (needs the real corpus and the native program: gated)
    const config::Config main = config::Config::load(config::current().main_checkout());
    base::FormatProbes probes;
    probes.lane = [](const stdfs::path& p) -> std::optional<base::Lane> {
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
    base::Corpus corpus(main.products(), probes);
    const std::string where = fs::get_env("CB_PROOF_WORK").value_or(fs::to_text(stdfs::temp_directory_path() / "cb-proof"));
    fs::make_dirs(fs::from_text(where));
    const stdfs::path reference = [&] {
        try {
            return corpus.reference();
        } catch (const Error&) {
            return stdfs::path();
        }
    }();
    knowledge::Store store(fs::from_text(where) / "knowledge.sqlite", knowledge::DataFolders::under(main.checkout_root()));
    knowledge::Knowledge kb(store, symbols::prototypes::HeaderFolders::standard(build::watcom::OpenWatcom::from_settings(main.text("openwatcom")).root(), reference));
    const translate::native_l2::Library library = translate::native_l2::Library::builtin();
    const std::unique_ptr<CorpusEnvironment> environment = CorpusEnvironment::over_knowledge(corpus, library, kb);
    decompiler::Context context;
    context.settings = decompiler::Settings::from_config(main);
    context.policy = decompiler::Policy::from_environment();
    context.environment = environment.get();
    context.limits = main.budget_limits();
    context.scratch = fs::from_text(where) / "scratch";
    decompiler::Decompiler decompiler(context);
    base::converted::Store converted(fs::from_text(where) / "Converted");
    for (const char* module : {"lz32.dll", "version.dll", "lzexpand.dll", "find.exe", "vmouse.vxd"}) {
        const stdfs::path out = fs::from_text(where) / (std::string(module) + ".c");
        const auto started = std::chrono::steady_clock::now();
        const std::vector<std::string> args = {module, "--out", fs::to_text(out)};
        CB_CHECK_EQ(decompiler::command(decompiler, corpus, converted, args), 0);
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
        const Bytes got = fs::read_file(out);
        const Bytes want = fs::read_file(fs::from_text(*expected_dir) / (std::string(module) + ".c"));
        std::cout << "proof " << module << " " << (got == want ? "identical" : "DIFFERENT") << " " << got.size() << " bytes " << seconds << " s\n";
        CB_CHECK(got == want);
    }
});

}  // namespace
}  // namespace cb::decompile::environment

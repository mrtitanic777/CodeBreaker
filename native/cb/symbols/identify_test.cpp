// cb-unity: alone (test helpers named for this module)
// Tests of `identify` and `names` as commands (symbols/identify, symbols/workbench, symbols/names_command, symbols/ordinal_collect,
// symbols/body_store) over real modules: LZ32.DLL (a flat thunk), LZEXPAND.DLL (16-bit), FIND.EXE (DOS) and NATALSU.EXE (a console image) from
// formats/testdata, through a corpus made here, the real signature and ordinal files of the checkout, and a knowledge base of the test's own.
//
// The goldens in symbols/testdata/identify are the recorded output of `codebreaker identify lz32.dll`, `codebreaker names natalsu.exe` ...
// (2026-11-12), carriage returns taken out. The corpus of the test lacks the 16-bit system modules whose export names LZEXPAND's imports
// are called by and the reference folder's symbol files, which is why LZEXPAND's import lines are left out of the comparison.
#include <algorithm>
#include <sstream>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/describe.hpp"
#include "cb/formats/detect.hpp"
#include "cb/symbols/identify.hpp"
#include "cb/symbols/names.hpp"
#include "cb/symbols/ordinals.hpp"
#include "cb/symbols/workbench.hpp"

namespace sy = cb::symbols;
namespace stdfs = std::filesystem;

namespace {

stdfs::path idn_testdata(std::string_view name) { return cb::testing::data_path("symbols/testdata/identify/" + std::string(name)); }
stdfs::path idn_format_data(std::string_view name) { return cb::testing::data_path("formats/testdata/" + std::string(name)); }
// (a checkout may have turned the golden's line ends into carriage return and line feed: the commands write line feeds)
std::string idn_golden(std::string_view name) {
    std::string text = cb::fs::read_text(idn_testdata(name));
    text.erase(std::remove(text.begin(), text.end(), '\r'), text.end());
    return text;
}

// A corpus holding the files of formats/testdata that these tests ask about, the configuration of the checkout, and an Environment over them.
struct IdnWorld {
    cb::fs::ScopedTempDir dir{"cbidn"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    std::optional<cb::fs::ScopedEnv> root_env;
    std::unique_ptr<cb::base::Corpus> corpus;
    std::unique_ptr<cb::config::Config> config;
    sy::Environment environment;

    IdnWorld() {
        cb::fs::make_dirs(dir.path() / "corpus");
        cb::fs::make_dirs(dir.path() / "reference");
        root_env.emplace("CB_CORPUS_ROOT", cb::fs::to_text(dir.path() / "corpus"));
        for (const char* name : {"lz32.dll", "lzexpand.dll", "find.exe", "natalsu.exe", "natalsu.exe.json"})
            cb::fs::write_file(dir.path() / "corpus" / name, cb::fs::read_file(idn_format_data(name)));
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = dir.path() / "corpus";
        products.checkout_root = stdfs::path(CB_SOURCE_DIR).parent_path();
        products.state = dir.path() / "state";
        products.reference = dir.path() / "reference";
        cb::base::FormatProbes probes;
        probes.lane = [](const stdfs::path& p) -> std::optional<cb::base::Lane> {
            const std::optional<cb::formats::detect::Lane> l = cb::formats::detect::lane(p);
            if (!l) return std::nullopt;
            switch (*l) {
            case cb::formats::detect::Lane::Pe: return cb::base::Lane::Pe;
            case cb::formats::detect::Lane::Ne: return cb::base::Lane::Ne;
            case cb::formats::detect::Lane::Le: return cb::base::Lane::Le;
            case cb::formats::detect::Lane::Dos: return cb::base::Lane::Dos;
            }
            return std::nullopt;
        };
        corpus = std::make_unique<cb::base::Corpus>(products, probes);
        config = std::make_unique<cb::config::Config>(cb::config::Config::load(products.checkout_root));
        environment.corpus = corpus.get();
        environment.config = config.get();
        environment.knowledge_file = dir.path() / "knowledge.sqlite";
        environment.signature_file = dir.path() / "runtime_signatures.json";
        environment.ordinal_file = dir.path() / "ordinal_names.json";
    }

    // The command's output for `args`, its exit code in `code`.
    std::string run(const cb::cli::Command& command, std::vector<std::string> args, int* code = nullptr) {
        const std::string name = command.name;
        const cb::cli::Cli program("codebreaker", {command});
        args.insert(args.begin(), name);
        cb::testing::CapturedStd captured;
        const int rc = program.run(args);
        if (code != nullptr) *code = rc;
        return captured.out();
    }
};

// The lines of `text` without those `drop` says, one per line.
std::string idn_without(const std::string& text, bool (*drop)(const std::string&)) {
    std::string out;
    std::istringstream in(text);
    for (std::string line; std::getline(in, line);)
        if (!drop(line)) out += line + "\n";
    return out;
}

void idn_write_text(const stdfs::path& file, std::string_view text) {
    cb::fs::write_file(file, cb::ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
}

bool idn_import_line(const std::string& line) { return line.starts_with("import ") || line.find(" imports -- ") != std::string::npos; }

}  // namespace

// ---- identify ---------------------------------------------------------------------------------------------------------

CB_TEST("symbols.identify.a_flat_thunk_is_identified_stub_by_stub_and_its_compiler_named", [] {
    IdnWorld w;
    int code = -1;
    const std::string text = w.run(sy::identify::command(w.environment), {"lz32.dll"}, &code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK_EQ(text, idn_golden("identify_lz32.dll.txt"));   // (19 of 39 functions, all the thunk compiler's)
});

CB_TEST("symbols.identify.a_16_bit_module_is_identified_by_signature_with_segment_and_offset_addresses", [] {
    IdnWorld w;
    int code = -1;
    const std::string text = w.run(sy::identify::command(w.environment), {"lzexpand.dll"}, &code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK_EQ(text, idn_golden("identify_lzexpand.dll.txt"));   // (four functions of the C runtime, MLIBCEW.LIB)
});

CB_TEST("symbols.identify.a_console_image_has_no_library_but_says_what_its_tool_block_says", [] {
    IdnWorld w;
    CB_CHECK_EQ(w.run(sy::identify::command(w.environment), {"natalsu.exe"}), idn_golden("identify_natalsu.exe.txt"));
});

CB_TEST("symbols.identify.a_dos_program_is_matched_in_its_image", [] {
    IdnWorld w;
    // (a DOS program of Windows 95 holds none of a period library's functions, but its functions are found and counted: they are matched)
    const std::string text = w.run(sy::identify::command(w.environment), {"find.exe"});
    CB_CHECK(text.starts_with("find.exe: 0 of "));
    CB_CHECK(!text.starts_with("find.exe: 0 of 0 functions"));
    CB_CHECK(text.ends_with("built with: not established (linker ?)\n"));
});

CB_TEST("symbols.identify.the_identifier_answers_in_the_addresses_the_decompiler_uses", [] {
    IdnWorld w;
    sy::Workbench bench(w.environment);
    sy::identify::Identifier& identifier = bench.identifier();
    const sy::identify::Found lz32 = identifier.identify(w.dir.path() / "corpus" / "lz32.dll");
    CB_CHECK_EQ(lz32.size(), 19U);
    CB_CHECK_EQ(lz32.begin()->first, 0xBFEA1075ULL);
    CB_CHECK_EQ(lz32.begin()->second.name, "thunk stub");
    CB_CHECK_EQ(identifier.function_count(w.dir.path() / "corpus" / "lz32.dll"), 39U);
    const sy::identify::Found lzexpand = identifier.identify(w.dir.path() / "corpus" / "lzexpand.dll");
    CB_CHECK_EQ(lzexpand.size(), 4U);
    const sy::identify::Address first = static_cast<sy::identify::Address>(cb::decompile::describe::address(11, 0x10));
    CB_CHECK(lzexpand.contains(first));
    CB_CHECK_EQ(lzexpand.at(first).name, "_dos_getftime");
    const cb::symbols::runtime_library::CompilerInfo info = identifier.compiler(w.dir.path() / "corpus" / "lz32.dll", lz32);
    CB_CHECK_EQ(info.compiler, "Microsoft Visual C++ 2.x");
    CB_CHECK_EQ(info.linker, "2.50");
});

CB_TEST("symbols.identify.the_command_refuses_what_it_cannot_do", [] {
    IdnWorld w;
    cb::testing::CapturedStd captured;
    const cb::cli::Command command = sy::identify::command(w.environment);
    const cb::cli::Cli program("codebreaker", {command});
    CB_CHECK_THROWS_AS(cb::UsageError, (void)program.run(std::vector<std::string>{"identify"}));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)program.run(std::vector<std::string>{"identify", "nosuchmodule.dll"}));
});

CB_TEST("symbols.identify.learn_makes_the_signature_file_from_a_library", [] {
    IdnWorld w;
    const stdfs::path library = cb::testing::data_path("symbols/libraries/testdata/atof.obj");
    int code = -1;
    const std::string text = w.run(sy::identify::command(w.environment), {"--learn", "A test object=" + cb::fs::to_text(library), "not a source"}, &code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK(text.find("A test object (ATOF.OBJ)") != std::string::npos);
    CB_CHECK(text.find(" 32-bit and ") != std::string::npos);
    const std::string file = cb::fs::read_text(w.environment.signature_file);
    CB_CHECK(file.starts_with("{\"libraries\": [\"A test object (ATOF.OBJ)\"]"));
    CB_CHECK(file.find("\"32\": [") != std::string::npos);
    CB_CHECK(file.find("\"16\": [") != std::string::npos);
});

// ---- names ---------------------------------------------------------------------------------------------------------------

CB_TEST("symbols.names.the_command_names_a_32_bit_module", [] {
    IdnWorld w;
    int code = -1;
    CB_CHECK_EQ(w.run(cb::symbols::names::command(w.environment), {"lz32.dll"}, &code), idn_golden("names_lz32.dll.txt"));
    CB_CHECK_EQ(code, 0);
});

CB_TEST("symbols.names.the_command_names_a_16_bit_modules_functions", [] {
    IdnWorld w;
    const std::string text = w.run(cb::symbols::names::command(w.environment), {"lzexpand.dll"});
    CB_CHECK_EQ(idn_without(text, idn_import_line), idn_without(idn_golden("names_lzexpand.dll.txt"), idn_import_line));
    CB_CHECK(text.find("import USER") != std::string::npos);   // (the imports are listed: named only where the corpus holds USER)
});

CB_TEST("symbols.names.a_console_images_functions_and_imports_are_named", [] {
    IdnWorld w;
    CB_CHECK_EQ(w.run(cb::symbols::names::command(w.environment), {"natalsu.exe"}), idn_golden("names_natalsu.exe.txt"));
});

CB_TEST("symbols.names.a_dos_program_ends_the_report_with_an_empty_line", [] {
    IdnWorld w;
    CB_CHECK_EQ(w.run(cb::symbols::names::command(w.environment), {"find.exe"}), idn_golden("names_find.exe.txt"));
});

CB_TEST("symbols.names.naming_a_body_is_kept_in_the_knowledge_base_and_counted", [] {
    IdnWorld w;
    const cb::cli::Command command = cb::symbols::names::command(w.environment);
    CB_CHECK_EQ(w.run(command, {"--shared"}), "0 bodies recorded, 0 distinct; 0 bodies are in two or more modules (0 functions are such copies)\n");
    int code = -1;
    const std::string first = w.run(command, {"lzexpand.dll", "--set", "2:0000", "InitTheExpander"}, &code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK(first.starts_with("body "));
    CB_CHECK(first.ends_with(" is now InitTheExpander\n"));
    // (the module's bodies are recorded now, and the name is read back the next time)
    const std::string shown = w.run(command, {"lzexpand.dll"});
    CB_CHECK(shown.find("2:0000     LZINIT") != std::string::npos);   // (the module's own export name still comes first)
    const std::string counts = w.run(command, {"--shared"});
    CB_CHECK(counts.starts_with("46 bodies recorded") || counts.find(" bodies recorded, ") != std::string::npos);
    CB_CHECK(counts.find("0 bodies recorded") == std::string::npos);
    int none = -1;
    CB_CHECK_EQ(w.run(command, {"lzexpand.dll", "--set", "1:ffff", "Nothing"}, &none), "that function has no body worth naming\n");
    CB_CHECK_EQ(none, 1);
});

CB_TEST("symbols.names.the_command_refuses_what_it_cannot_do", [] {
    IdnWorld w;
    cb::testing::CapturedStd captured;
    const cb::cli::Command command = cb::symbols::names::command(w.environment);
    const cb::cli::Cli program("codebreaker", {command});
    CB_CHECK_THROWS_AS(cb::UsageError, (void)program.run(std::vector<std::string>{"names"}));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)program.run(std::vector<std::string>{"names", "nosuchmodule.dll"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)program.run(std::vector<std::string>{"names", "lz32.dll", "--set", "zz", "Name"}));
});

CB_TEST("symbols.names.the_ordinal_table_is_remade_from_a_definition_file_the_corpus_bears_out", [] {
    IdnWorld w;
    // LZEXPAND imports USER by number (and the test's corpus holds no USER), so a definition file for USER is kept: its names stand for numbers
    // something imports. LZEXPAND's own exports are all named by the module itself, so a file naming them gives nothing.
    idn_write_text(w.dir.path() / "user.def", "LIBRARY USER\nEXPORTS\n    Foo    @7\n    IsCharLower   @436\n");
    idn_write_text(w.dir.path() / "lzexpand.def", "LIBRARY LZEXPAND\nEXPORTS\n    Spare   @1\n");
    int code = -1;
    const std::string text = w.run(cb::symbols::names::command(w.environment),
                                   {"--ordinals", cb::fs::to_text(w.dir.path() / "user.def"), cb::fs::to_text(w.dir.path() / "lzexpand.def")}, &code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK_EQ(text, "USER           2 names from user.def       (0 checked against the code, 0 dropped)\n");
    const cb::symbols::ordinals::ShippedTables tables = cb::symbols::ordinals::parse_shipped(cb::fs::read_text(w.environment.ordinal_file));
    CB_CHECK_EQ(tables.size(), 1U);
    CB_CHECK_EQ(tables.at("USER").names.at(436), "IsCharLower");
    CB_CHECK_EQ(tables.at("USER").names.at(7), "Foo");
});

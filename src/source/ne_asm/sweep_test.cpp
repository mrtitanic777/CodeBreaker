// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/ne_asm/sweep: which modules of a corpus are NE and in what order, a module's record (with the assembler given by the test), and the
// command's lines and totals, over LZEXPAND.DLL (the one NE module of the corpus the tests make) with the listing of scenario_a (a canned objdump).
#include "source/ne_asm/sweep.hpp"

#include <nlohmann/json.hpp>
#include <sstream>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"

namespace sw = cb::source::ne_asm::sweep;
namespace id = cb::source::ne_asm::idiom;
namespace rc = cb::source::ne_asm::recon;
namespace sg = cb::source::ne_asm::segment;
namespace fx = cb::source::ne_asm::fixups;

namespace {

fx::Objdump sw_objdump() {
    std::vector<std::pair<std::uint32_t, std::string>> rows;
    std::istringstream in(cb::text::replace_all(cb::fs::read_text(cb::testing::data_path("source/ne_asm/testdata/scenario_a.rows")), "\r\n", "\n"));
    for (std::string line; std::getline(in, line);) {
        if (line.empty()) continue;
        const std::size_t tab = line.find('\t');
        rows.emplace_back(*cb::text::parse_int<std::uint32_t>(line.substr(0, tab), 16), line.substr(tab + 1));
    }
    return [rows = std::move(rows)](cb::ByteView, std::uint32_t) {
        std::string out = "\r\nsegment.bin:     file format binary\r\n\r\n\r\nDisassembly of section .data:\r\n\r\n00000000 <.data>:\r\n";
        for (const auto& [offset, text] : rows) out += std::format("{:4x}:\t00 00\t{}\r\n", offset, text);
        return out;
    };
}

// A corpus root as Setup installs it: the NE module in SYSTEM, a copy of it under another name in WINDOWS (the same name is taken once), and what is
// not NE.
struct SweepWorld {
    cb::fs::ScopedTempDir dir{"sweeptest"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedEnv root{"CB_CORPUS_ROOT", cb::fs::to_text(dir.path() / "corpus")};
    cb::fs::ScopedEnv no_data{"WT_DATA_ROOT", std::nullopt};
    std::unique_ptr<cb::base::Corpus> corpus;
    SweepWorld() {
        const std::filesystem::path folder = dir.path() / "corpus";
        cb::fs::make_dirs(folder / "WINDOWS" / "SYSTEM");
        cb::fs::make_dirs(folder / "WINDOWS" / "COMMAND");
        std::filesystem::copy_file(cb::testing::data_path("formats/testdata/lzexpand.dll"), folder / "WINDOWS" / "SYSTEM" / "LZEXPAND.DLL");
        std::filesystem::copy_file(cb::testing::data_path("formats/testdata/lzexpand.dll"), folder / "WINDOWS" / "lzexpand.dll");   // (a second copy of the name)
        std::filesystem::copy_file(cb::testing::data_path("formats/testdata/lzexpand.dll"), folder / "WINDOWS" / "COMMAND" / "other.drv");
        std::filesystem::copy_file(cb::testing::data_path("formats/testdata/find.exe"), folder / "WINDOWS" / "COMMAND" / "find.exe");   // a DOS program
        std::filesystem::copy_file(cb::testing::data_path("formats/testdata/lz32.dll"), folder / "WINDOWS" / "SYSTEM" / "lz32.dll");   // a PE
        cb::fs::write_text(folder / "WINDOWS" / "SYSTEM" / "readme.txt", "x");
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = folder;
        products.checkout_root = dir.path() / "CodeBreaker";
        products.state = dir.path() / "state";
        cb::base::FormatProbes probes;
        probes.lane = [](const std::filesystem::path& p) -> std::optional<cb::base::Lane> {
            const auto lane = cb::formats::detect::lane(p);
            if (!lane) return std::nullopt;
            switch (*lane) {
            case cb::formats::detect::Lane::Pe: return cb::base::Lane::Pe;
            case cb::formats::detect::Lane::Ne: return cb::base::Lane::Ne;
            case cb::formats::detect::Lane::Le: return cb::base::Lane::Le;
            case cb::formats::detect::Lane::Dos: return cb::base::Lane::Dos;
            }
            return std::nullopt;
        };
        corpus = std::make_unique<cb::base::Corpus>(products, probes);
    }
};

}  // namespace

CB_TEST("source.ne_asm.sweep.the_ne_modules_of_the_system_tree", [] {
    const SweepWorld world;
    const auto modules = sw::ne_modules(*world.corpus);
    // SYSTEM before WINDOWS itself before COMMAND within each root; the name once (the first copy), in lower case; a PE and a DOS program are not NE
    CB_CHECK_EQ(modules.size(), std::size_t{2});
    CB_CHECK_EQ(modules[0].first, "lzexpand.dll");
    CB_CHECK(cb::fs::to_text(modules[0].second).find("SYSTEM") != std::string::npos);
    CB_CHECK_EQ(modules[1].first, "other.drv");
});

CB_TEST("source.ne_asm.sweep.a_module_is_swept_segment_by_segment", [] {
    const SweepWorld world;
    std::size_t asked = 0;
    id::Assemblers assemblers;
    assemblers.nasm = [&](std::string_view) {
        ++asked;
        return rc::Assembled{};   // makes nothing: every instruction ends up db
    };
    const auto ordinals = std::make_shared<const sg::Ordinals>(sg::load_ordinals(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md")));
    const auto modules = sw::ne_modules(*world.corpus);
    const sw::ModuleRecord record = sw::sweep_module(modules[0].first, modules[0].second, assemblers, id::Backend::Nasm, world.dir.path() / "state", ordinals, std::nullopt, sw_objdump());
    CB_CHECK_EQ(record.module, "lzexpand.dll");
    CB_CHECK(!record.segments.empty());
    CB_CHECK(!record.ok);   // nothing assembled
    CB_CHECK_EQ(record.exact_segments, std::size_t{0});
    CB_CHECK_EQ(record.live, std::size_t{0});
    CB_CHECK(record.code_instructions > 0);
    CB_CHECK(asked > 0);
    for (const sw::SegmentRecord& s : record.segments) {
        CB_CHECK(s.segment >= 1);
        CB_CHECK(s.bytes > 0);
        CB_CHECK(!s.ok);
        CB_CHECK_EQ(s.pinned, s.code_instructions);
    }
});

CB_TEST("source.ne_asm.sweep.the_command_lists_and_totals", [] {
    const SweepWorld world;
    id::Assemblers assemblers;
    assemblers.nasm = [](std::string_view) { return rc::Assembled{}; };
    const sw::Environment env{*world.corpus, assemblers, world.dir.path() / "state", sw_objdump()};
    const std::string ords = cb::fs::to_text(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md"));
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(sw::main(env, std::vector<std::string>{"--list"}), cb::kExitOk);
        CB_CHECK_EQ(cap.out(), "lzexpand.dll\nother.drv\n");
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(sw::main(env, std::vector<std::string>{"--limit", "1", "--ords", ords, "--emit", cb::fs::to_text(world.dir.path() / "emit")}), cb::kExitOk);
        const std::string said = cap.out();
        CB_CHECK(said.starts_with("NE idiomatic sweep: 1 NE modules (backend=nasm)\n\n  lzexpand.dll "));
        CB_CHECK(said.find("(partial)") != std::string::npos);
        CB_CHECK(said.find("fully byte-exact modules : 0/1") != std::string::npos);
        CB_CHECK(said.find("module errors            : 0") != std::string::npos);
        CB_CHECK(said.find("wrote _ne_idiom_summary.json") != std::string::npos);
    }
    const nlohmann::ordered_json summary = nlohmann::ordered_json::parse(cb::fs::read_text(world.dir.path() / "emit" / "_ne_idiom_summary.json"));
    CB_CHECK(summary["totals"]["mods"] == 1);
    CB_CHECK(summary["totals"]["full"] == 0);
    CB_CHECK(summary["modules"][0]["module"] == "lzexpand.dll");
    {
        cb::testing::CapturedStd cap;   // --start skips modules; a limit of 0 is no limit
        CB_CHECK_EQ(sw::main(env, std::vector<std::string>{"--start", "1", "--limit", "0", "--ords", ords}), cb::kExitOk);
        CB_CHECK(cap.out().starts_with("NE idiomatic sweep: 1 NE modules (backend=nasm)\n\n  other.drv "));
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_THROWS_AS(cb::UsageError, sw::main(env, std::vector<std::string>{"--backend", "gas"}));
    }
});

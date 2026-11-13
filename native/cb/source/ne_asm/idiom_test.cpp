// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/ne_asm/idiom: the cMacros source of a range (names, near and far, labels, extern references), the loop that pins what the assembler
// will not encode, the two assemblers' wrappers (nasm as a process, ML as a DOS runner) and the command. The assembler is given by the test, over
// LZEXPAND.DLL's first code segment with the listing of scenario_a (a canned objdump). The source of a real range, byte for byte against the Python
// module and its ML, is in the report of the change.
#include "cb/source/ne_asm/idiom.hpp"

#include <format>
#include <set>
#include <sstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace id = cb::source::ne_asm::idiom;
namespace rc = cb::source::ne_asm::recon;
namespace sg = cb::source::ne_asm::segment;
namespace fx = cb::source::ne_asm::fixups;

namespace {

using IdRows = std::vector<std::pair<std::uint32_t, std::string>>;

IdRows id_rows(std::string_view name) {
    IdRows rows;
    std::istringstream in(cb::text::replace_all(cb::fs::read_text(cb::testing::data_path("source/ne_asm/testdata/" + std::string(name))), "\r\n", "\n"));
    for (std::string line; std::getline(in, line);) {
        if (line.empty()) continue;
        const std::size_t tab = line.find('\t');
        rows.emplace_back(*cb::text::parse_int<std::uint32_t>(line.substr(0, tab), 16), line.substr(tab + 1));
    }
    return rows;
}

fx::Objdump id_objdump(IdRows rows) {
    return [rows = std::move(rows)](cb::ByteView, std::uint32_t) {
        std::string out = "\r\nsegment.bin:     file format binary\r\n\r\n\r\nDisassembly of section .data:\r\n\r\n00000000 <.data>:\r\n";
        for (const auto& [offset, text] : rows) out += std::format("{:4x}:\t00 00\t{}\r\n", offset, text);
        return out;
    };
}

sg::Seg id_segment(sg::NameOverrides overrides = {}) {
    const cb::formats::ne::Module module = fx::open_module(cb::testing::data_path("formats/testdata/lzexpand.dll"));
    const auto ordinals = std::make_shared<const sg::Ordinals>(sg::load_ordinals(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md")));
    return sg::Seg(module, 1, ordinals, std::move(overrides), id_objdump(id_rows("scenario_a.rows")));
}

bool id_has(std::string_view text, std::string_view part) { return text.find(part) != std::string_view::npos; }

// A runner of DOS programs that makes what ML would, and keeps what it was asked.
class IdFakeRunner final : public cb::build::DosRunner {
public:
    mutable std::vector<cb::build::DosRunSpec> specs;
    mutable cb::build::DosRunResult answer;
    cb::build::DosRunResult run(const cb::build::DosRunSpec& spec) const override {
        specs.push_back(spec);
        return answer;
    }
};

}  // namespace

CB_TEST("source.ne_asm.idiom.a_name_cproc_accepts", [] {
    const sg::Seg plain = id_segment();
    CB_CHECK_EQ(id::procedure_name(plain, 0x90), "sub_0090");            // no name known: sub_<offset>
    const sg::Seg named = id_segment({{0x90, "Worker"}, {0xa0, "1st-Proc.Name"}, {0xc0, "Ok_$?@9"}});
    CB_CHECK_EQ(id::procedure_name(named, 0x90), "Worker");
    CB_CHECK_EQ(id::procedure_name(named, 0xa0), "_1st_Proc_Name");      // a digit first gets an underscore; everything else cProc does not take becomes one
    CB_CHECK_EQ(id::procedure_name(named, 0xc0), "Ok_$?@9");             // what it does take stays
});

CB_TEST("source.ne_asm.idiom.the_source_of_a_range", [] {
    const sg::Seg seg = id_segment();
    // [0x90, 0xc8): a near function (ret 4), a far one (retf) and the thunk that jumps back to the first
    const id::Emitted emitted = id::emit(seg, 0x90, 0xc8);
    const std::string& text = emitted.source;
    CB_CHECK(text.starts_with("; idiomatic recovered source -- Microsoft SDK cMacros (ne_idiom.py)\nOPTION M510\n?PLM = 1\n?WIN = 1\n.xlist\ninclude cmacros.inc\n.list\n\nsBegin  CODE\n.386p\n"));
    CB_CHECK(id_has(text, "        assumes cs,CODE\n        assumes ds,nothing\n        assumes es,nothing\n\n"));
    CB_CHECK(id_has(text, "cProc   sub_0090,<NEAR>\ncBegin  nogen\n"));
    CB_CHECK(id_has(text, "cProc   sub_00a0,<FAR>\ncBegin  nogen\n"));      // it ends in retf
    CB_CHECK(id_has(text, "cEnd    nogen\n"));
    CB_CHECK(id_has(text, "\nsEnd    CODE\n        end\n"));
    CB_CHECK(text.ends_with("        end\n"));
    // the jump of the thunk to the first function is symbolic; a branch to a function is by name, the bytes it carries are the displacement's
    CB_CHECK(id_has(text, "jmp     sub_0090"));
    CB_CHECK(emitted.unresolved.empty());   // (a branch inside the range: its displacement is resolved)
    // every instruction line carries its offset
    CB_CHECK(id_has(text, "; 0090"));
    CB_CHECK(id_has(text, "; 00c4"));
});

CB_TEST("source.ne_asm.idiom.a_branch_out_of_the_range_is_extern_and_its_bytes_are_unresolved", [] {
    const sg::Seg seg = id_segment();
    // [0xc0, 0xc8) holds `jmp 0x90`, which is outside: the displacement bytes are masked as a relocation is, and the target is declared
    const id::Emitted emitted = id::emit(seg, 0xc0, 0xc8);
    CB_CHECK(id_has(emitted.source, "jmp     sub_0090"));
    CB_CHECK(id_has(emitted.source, "        externNP sub_0090\n"));
    CB_CHECK(emitted.unresolved == (std::set<std::uint32_t>{0xc5, 0xc6}));   // the two bytes after the opcode at 0xc4
});

CB_TEST("source.ne_asm.idiom.local_labels_and_short_jumps", [] {
    const sg::Seg seg = id_segment();
    const id::Emitted emitted = id::emit(seg, 0, 0x60, std::vector<std::uint32_t>{0});   // the range as one function
    const std::string& text = emitted.source;
    CB_CHECK(id_has(text, "L_0020:\n"));
    CB_CHECK(id_has(text, "L_0040:\n"));
    CB_CHECK(id_has(text, "jne     L_0020") || id_has(text, "jne     short L_0020"));
    CB_CHECK(id_has(text, "cProc   sub_0000,<"));
    // far by its last retf: [0xa0, 0xb0) ends in retf
    CB_CHECK(id_has(id::emit(seg, 0xa0, 0xb0).source, "cProc   sub_00a0,<FAR>"));
    // rows pinned as db
    const id::Emitted pinned = id::emit(seg, 0x90, 0xa0, std::nullopt, {0x93});
    CB_CHECK(id_has(pinned.source, "db      "));
    CB_CHECK(id_has(pinned.source, "; 0093 mov ax,word ptr [bp+6h]"));
});

CB_TEST("source.ne_asm.idiom.no_function_start_makes_the_range_one_function", [] {
    const sg::Seg seg = id_segment();
    // a range the carve found no start in: the whole range is one function, named for its first offset
    const std::string text = id::emit(seg, 0x56, 0x60).source;
    CB_CHECK(id_has(text, "cProc   sub_0056,<"));
    // a range that opens mid-function: the lead-in before the first function start is db
    const std::string lead = id::emit(seg, 0x4a, 0x60, std::vector<std::uint32_t>{0x50}).source;
    CB_CHECK(id_has(lead, "data/lead-in"));
    CB_CHECK(id_has(lead, "cProc   sub_0050,<"));
});

CB_TEST("source.ne_asm.idiom.an_assembler_that_refuses_pins_the_instruction", [] {
    const sg::Seg seg = id_segment();
    std::vector<std::string> sources;
    const rc::Assemble refuses = [&](std::string_view source) {
        sources.emplace_back(source);
        const std::vector<std::string_view> lines = cb::text::lines(source);
        rc::Assembled made;
        for (std::size_t i = 0; i < lines.size(); ++i)
            if (lines[i].find("; 0093") != std::string_view::npos && lines[i].find("        mov") == 0) made.log = std::format("m.asm:{}: error: parser: instruction expected\n", i + 1);
        return made;
    };
    id::Assemblers assemblers;
    assemblers.nasm = refuses;
    const id::Reconciled r = id::reconcile_range(seg, 0x90, 0xa0, assemblers, id::Backend::Nasm);
    CB_CHECK(!r.verdict.has_value());
    CB_CHECK(sources.size() >= 3);
    CB_CHECK(id_has(sources.front(), "        mov ax,word ptr [bp+6h]"));
    CB_CHECK(!id_has(sources[1], "        mov ax,word ptr [bp+6h]"));   // db from the second time on
    CB_CHECK(!r.force_db.contains(0x93));                                  // it is data (the assembler refused it), not a pinned instruction
    const id::Verified v = id::verify_range(seg, 0x90, 0xa0, assemblers, id::Backend::Nasm);
    CB_CHECK(!v.ok);
    CB_CHECK_EQ(v.bytes, std::size_t{0x10});
    CB_CHECK_EQ(v.live, std::size_t{0});
    CB_CHECK(!v.verdict.has_value());
    // an assembler that is not set up
    CB_CHECK_THROWS_AS(cb::UsageError, id::reconcile_range(seg, 0x90, 0xa0, assemblers, id::Backend::Ml));
    CB_CHECK_EQ(std::string(id::backend_name(id::Backend::Ml)), "ml");
    CB_CHECK_EQ(std::string(id::backend_name(id::Backend::Nasm)), "nasm");
});

CB_TEST("source.ne_asm.idiom.nasm_is_run_on_the_source_and_its_words_are_kept", [] {
    cb::fs::ScopedTempDir dir("idiomtest");
    // a program that is not there: the failure is the log, and nothing was made
    const rc::Assemble none = id::nasm_assembler(dir.path() / "no-such-nasm.exe", dir.path() / "inc");
    const rc::Assembled made = none("sBegin CODE\n");
    CB_CHECK(!made.object.has_value());
    CB_CHECK(!made.log.empty());
    CB_CHECK(made.listing.empty());
    // the shim is looked for in the nasm source tree: beside the program, or above it where the program is in a build folder
    cb::fs::make_dirs(dir.path() / "nasm" / "test" / "masm");
    cb::fs::make_dirs(dir.path() / "nasm" / "build" / "bin");
    cb::fs::write_text(dir.path() / "nasm" / "test" / "masm" / "cmacros_shim.inc", "; the shim\n");
    const rc::Assemble again = id::nasm_assembler(dir.path() / "nasm" / "build" / "bin" / "nasm.exe", dir.path() / "shim-copy");
    again("x");
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "shim-copy" / "cmacros_shim.inc"));
    CB_CHECK_EQ(cb::fs::read_text(dir.path() / "shim-copy" / "cmacros_shim.inc"), "; the shim\n");
});

CB_TEST("source.ne_asm.idiom.ml_is_run_as_a_dos_program_over_a_staged_folder", [] {
    cb::fs::ScopedTempDir dir("idiomtest");
    cb::fs::make_dirs(dir.path() / "ml");
    cb::fs::make_dirs(dir.path() / "inc16");
    cb::fs::make_dirs(dir.path() / "extender");
    cb::fs::write_text(dir.path() / "ml" / "ML.EXE", "ml");
    cb::fs::write_text(dir.path() / "ml" / "ML.ERR", "err");
    cb::fs::write_text(dir.path() / "extender" / "DOSXNT.EXE", "x");
    cb::fs::write_text(dir.path() / "inc16" / "CMACROS.INC", "inc");
    cb::fs::write_text(dir.path() / "inc16" / "README.TXT", "not an include");
    IdFakeRunner runner;
    runner.answer.made["m.obj"] = cb::Bytes{1, 2, 3};
    runner.answer.made["m.lst"] = cb::Bytes{'l', 's', 't'};
    runner.answer.made["a.txt"] = cb::Bytes{'l', 'o', 'g'};
    const rc::Assemble assemble = id::ml_assembler(runner, {dir.path() / "ml", dir.path() / "extender" / "DOSXNT.EXE", dir.path() / "inc16", dir.path() / "stage"});
    const rc::Assembled made = assemble("a source");
    CB_CHECK(made.object == std::optional<cb::Bytes>(cb::Bytes{1, 2, 3}));
    CB_CHECK_EQ(made.listing, "lst");
    CB_CHECK_EQ(made.log, "log");
    CB_CHECK_EQ(runner.specs.size(), std::size_t{1});
    const cb::build::DosRunSpec& spec = runner.specs.front();
    CB_CHECK_EQ(spec.mounts.size(), std::size_t{1});
    CB_CHECK_EQ(spec.mounts.front().drive, "c");
    CB_CHECK(spec.mounts.front().folder == dir.path() / "stage");
    CB_CHECK(spec.commands == (std::vector<std::string>{"c:\\ml.exe /c /Ic:\\inc /Fom.obj /Flm.lst m.asm > a.txt"}));
    CB_CHECK(spec.artifacts == (std::vector<std::string>{"m.obj", "m.lst", "a.txt"}));
    CB_CHECK_EQ(std::get<std::string>(spec.files.at("m.asm")), "a source");
    // the programs and the includes were put in the staged folder
    for (const char* name : {"ML.EXE", "ML.ERR", "DOSXNT.EXE"}) CB_CHECK(std::filesystem::is_regular_file(dir.path() / "stage" / name));
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "stage" / "inc" / "CMACROS.INC"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "stage" / "inc" / "README.TXT"));
    // an assembly that made nothing: nothing
    runner.answer.made.clear();
    const rc::Assembled nothing = assemble("b");
    CB_CHECK(!nothing.object.has_value());
    CB_CHECK(nothing.listing.empty());
    CB_CHECK(nothing.log.empty());
});

CB_TEST("source.ne_asm.idiom.the_command_names_a_segment_without_code", [] {
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedTempDir dir("idiomtest");
    cb::fs::ScopedEnv root{"CB_CORPUS_ROOT", cb::fs::to_text(dir.path())};
    cb::base::Products products;
    products.target = "win95";
    products.target_folder = dir.path();
    products.checkout_root = dir.path() / "CodeBreaker";
    products.state = dir.path() / "state";
    const cb::base::Corpus corpus(products);
    id::Assemblers assemblers;
    assemblers.nasm = [](std::string_view) { return rc::Assembled{}; };
    const id::Environment env{corpus, assemblers, dir.path() / "state", id_objdump(id_rows("scenario_a.rows"))};
    const std::string lz = cb::fs::to_text(cb::testing::data_path("formats/testdata/lzexpand.dll"));
    const std::string ords = cb::fs::to_text(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md"));
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_EQ(id::main(env, std::vector<std::string>{lz, "99", "0", "whole", "--backend", "nasm", "--ords", ords}), cb::kExitFailed);
        CB_CHECK_EQ(cap.out(), "no code segment 99\n");
    }
    {
        cb::testing::CapturedStd cap;   // a range of the first segment, with an assembler that makes nothing: it is not exact
        CB_CHECK_EQ(id::main(env, std::vector<std::string>{lz, "1", "0x90", "0xa0", "--backend", "nasm", "--ords", ords}), cb::kExitOk);
        const std::string said = cap.out();
        CB_CHECK(said.starts_with(std::format("=== {} seg1 [0x90,0xa0) ===\nNOT exact: ok=false n=16 ", lz)));
    }
    {
        cb::testing::CapturedStd cap;
        CB_CHECK_THROWS_AS(cb::UsageError, id::main(env, std::vector<std::string>{lz, "1", "0", "0x10", "--backend", "gas"}));
    }
});

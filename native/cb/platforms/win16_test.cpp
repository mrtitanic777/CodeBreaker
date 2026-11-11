#include "cb/platforms/win16.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from codebreaker/platforms/win16.py:
//   * the descriptor is data only: the Python's Ref slots (which module's `NEFile`, `ow_tool` ... to call) are the
//     layers' own tables now, keyed on Id::Win16; tools() and library() are the build layer's (build::Watcom);
//   * call(): "error: <kind>" names the cb::Error that stopped the run, not a Python exception class.

using namespace cb;
using namespace cb::platforms;

namespace {
std::filesystem::path lzexpand() { return testing::data_path("formats/testdata/lzexpand.dll"); }
}  // namespace

CB_TEST("platforms.win16.descriptor", [] {
    const Platform p = win16();
    CB_CHECK(p.id == Id::Win16);
    CB_CHECK_EQ(p.name, "ne");
    CB_CHECK_EQ(p.title, "16-bit Windows");
    CB_CHECK_EQ(p.family, "x86");
    CB_CHECK(*p.source == SourceLanguage::CorAsm);
    CB_CHECK_EQ(*p.lane, "win16");
    CB_CHECK(p.steps == Steps::Win16);
    CB_CHECK_EQ(p.loader.container, "NE");
    CB_CHECK_EQ(p.cpu.language, "x86:LE:16:Protected Mode:default");
    CB_CHECK_EQ(*p.cpu.bits, 16u);
    CB_CHECK(*p.cpu.endian == Endian::Little);
    const auto& cs = std::get<CapstoneDecoder>(p.cpu.decoder);
    CB_CHECK(cs.arch == Arch::X86);
    CB_CHECK_EQ(cs.bits, 16u);
    CB_CHECK_EQ(p.cpu.registers.size(), 12u);
    CB_CHECK_EQ(p.cpu.registers.front(), "AX");
    CB_CHECK_EQ(p.cpu.registers.back(), "SS");
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK_EQ(p.convention.names.size(), 6u);
    CB_CHECK_EQ(p.convention.names[0], "__stdcall16far");
    CB_CHECK_EQ(std::get<unsigned>(p.convention.arguments), 8u);
    CB_CHECK_EQ(*p.convention.word, 2u);
    CB_CHECK(*p.convention.order == CallOrder::Pascal);
    CB_CHECK((p.convention.results == std::vector<std::string>{"AX", "DX:AX"}));
    CB_CHECK(*p.convention.pops == Pops::Either);
    CB_CHECK((p.convention.preserved == std::vector<std::string>{"SI", "DI", "DS"}));
    CB_CHECK(p.emulator.engine == Engine::Unicorn);
    CB_CHECK(p.emulator.runs_code);
    CB_CHECK(*p.compiler.language == Language::C);
    CB_CHECK_EQ(p.compiler.toolchain, "openwatcom");
});

CB_TEST("platforms.win16.is_what_the_registry_recognises_by_an_ne_header", [] {
    Probes probes;
    probes.executable_class = [](const std::filesystem::path& p) -> std::optional<ExeKind> {
        return p.filename() == "lzexpand.dll" ? std::optional<ExeKind>(ExeKind::Ne) : std::nullopt;
    };
    Registry registry(probes);
    registry.add(win16());
    CB_CHECK(registry.identify(lzexpand()) != nullptr);
    CB_CHECK_EQ(registry.identify(lzexpand())->name, "ne");
    CB_CHECK(registry.identify("other.exe") == nullptr);
    CB_CHECK(registry.named("ne") == registry.named("win16"));
    const auto steps = registry.step_sets();
    CB_CHECK_EQ(steps.size(), 1u);
    CB_CHECK(steps[0].second == Steps::Win16);
});

CB_TEST("platforms.win16.segments_and_entries_of_a_real_module", [] {
    const auto segs = ne_segments(lzexpand());
    CB_CHECK_EQ(segs.size(), 12u);
    CB_CHECK_EQ(segs[0].index, 1);
    CB_CHECK_EQ(segs[0].file_offset, std::uint64_t{768});
    CB_CHECK_EQ(segs[0].length, 456u);
    CB_CHECK(segs[11].is_data());
    const auto entries = ne_entries(lzexpand());
    CB_CHECK_EQ(entries.size(), 30u);
    CB_CHECK_EQ(entries[0].ordinal, 1u);
    CB_CHECK_EQ(entries[0].segment, 4);
    CB_CHECK_EQ(entries[0].offset, 0x15C);
});

CB_TEST("platforms.win16.not_an_ne_module_is_an_error", [] {
    const fs::ScopedTempDir dir("cbwin16");
    const auto text = dir.path() / "text.dll";
    fs::write_file(text, as_bytes("this is not an executable at all, only text"));
    CB_CHECK_THROWS_AS(FormatError, (void)ne_segments(text));
    const auto empty = dir.path() / "empty.exe";
    fs::write_file(empty, {});
    CB_CHECK_THROWS_AS(FormatError, (void)ne_entries(empty));
    CB_CHECK_THROWS_AS(NotFoundError, (void)ne_segments(dir.path() / "missing.dll"));
    // an LE driver is not an NE module either
    CB_CHECK_THROWS_AS(FormatError, (void)ne_segments(testing::data_path("formats/testdata/splitter.vxd")));
});

CB_TEST("platforms.win16.call_hands_the_runner_eight_zero_words_and_reads_ax", [] {
    std::vector<std::int64_t> seen_args;
    std::uint32_t seen_ordinal = 0;
    const Win16Runner ok = [&](const std::filesystem::path&, std::uint32_t ordinal, std::span<const std::int64_t> args) {
        seen_ordinal = ordinal;
        seen_args.assign(args.begin(), args.end());
        return Win16Run{"returned", {{"AX", 0x1234}, {"DX", 5}}};
    };
    const Win16Call got = call_win16_export(lzexpand(), 7, ok);
    CB_CHECK_EQ(seen_ordinal, 7u);
    CB_CHECK((seen_args == std::vector<std::int64_t>(8, 0)));
    CB_CHECK_EQ(got.ended, "returned");
    CB_CHECK_EQ(*got.ax, 0x1234);

    const Win16Runner no_ax = [](const std::filesystem::path&, std::uint32_t, std::span<const std::int64_t>) {
        return Win16Run{"hang", {}};
    };
    const Win16Call hung = call_win16_export(lzexpand(), 1, no_ax);
    CB_CHECK_EQ(hung.ended, "hang");
    CB_CHECK(!hung.ax.has_value());
});

CB_TEST("platforms.win16.call_turns_an_emulator_failure_into_a_result", [] {
    const Win16Runner gives_up = [](const std::filesystem::path&, std::uint32_t, std::span<const std::int64_t>) -> Win16Run {
        throw EmulationError("unmapped read");
    };
    const Win16Call got = call_win16_export(lzexpand(), 1, gives_up);
    CB_CHECK_EQ(got.ended, "error: EmulationError");
    CB_CHECK(!got.ax.has_value());

    // a broken setup or a bug is not a result
    const Win16Runner unset = [](const std::filesystem::path&, std::uint32_t, std::span<const std::int64_t>) -> Win16Run {
        throw UsageError("the emulator is not wired");
    };
    CB_CHECK_THROWS_AS(UsageError, (void)call_win16_export(lzexpand(), 1, unset));
    const Win16Runner bug = [](const std::filesystem::path&, std::uint32_t, std::span<const std::int64_t>) -> Win16Run {
        throw InternalError("invariant");
    };
    CB_CHECK_THROWS_AS(InternalError, (void)call_win16_export(lzexpand(), 1, bug));
});

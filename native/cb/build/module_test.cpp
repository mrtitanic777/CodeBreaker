// Tests of the module builder: what it decides without the tools (which functions go where, what a link line says, how an image is
// compared with the original, how the gate's verdicts are kept), with the pieces it is given replaced by recorded answers. A build with
// OpenWatcom is the product check of the area (the 15 MINI.CAB modules, byte for byte), not a unit test.
#include "cb/build/module.hpp"

#include <format>
#include <map>

#include "cb/base/cache.hpp"
#include "cb/build/module_internal.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/l2.hpp"

namespace {

using namespace cb;
using namespace cb::build::module;
namespace ir = cb::translate::ir;
namespace definitions = cb::build::definitions;

// A cache that holds what it is given.
class MtMemoryCache : public build::functions::BuildCache {
public:
    std::optional<Bytes> get(std::string_view kind, std::string_view key) override {
        ++gets;
        const auto it = held.find(std::string(kind) + "/" + std::string(key));
        return it == held.end() ? std::nullopt : std::optional<Bytes>(it->second);
    }
    void put(std::string_view kind, std::string_view key, ByteView value) override {
        held[std::string(kind) + "/" + std::string(key)] = Bytes(value.begin(), value.end());
    }
    std::map<std::string, Bytes> held;
    int gets = 0;
};

definitions::Export mt_export(std::uint32_t ordinal, std::optional<std::string> name, std::uint64_t address) {
    definitions::Export e;
    e.ordinal = ordinal;
    e.name = std::move(name);
    e.address = address;
    return e;
}

bool mt_has(const std::string& s, std::string_view part) { return s.find(part) != std::string::npos; }

void mt_write(const std::filesystem::path& p, const Bytes& bytes) { fs::write_file(p, bytes); }

}  // namespace

CB_TEST("build.module.kept_as_asm_reads_module_function_and_reason", [] {
    const std::string text =
        "# functions the gate passes that a run of the whole module showed wrong\n"
        "user.exe sub_1_222B   leaves DS   # trailing comment\n"
        "USER.EXE f2\n"
        "other.dll g why not\n"
        "\n"
        "badline\n";
    const auto user = kept_as_asm(text, "user.exe");
    CB_CHECK_EQ(user.size(), 2u);
    CB_CHECK_EQ(user.at("sub_1_222B"), "leaves DS");
    CB_CHECK_EQ(user.at("f2"), "");
    CB_CHECK_EQ(kept_as_asm(text, "other.dll").at("g"), "why not");
    CB_CHECK(kept_as_asm(text, "kernel.exe").empty());
    CB_CHECK(kept_as_asm("", "user.exe").empty());
});

CB_TEST("build.module.name_of_stem_finds_the_function_that_has_it", [] {
    const std::map<std::string, std::string> stems = {{"alpha", "s01/f1"}, {"beta", "s01/f2"}};
    CB_CHECK_EQ(name_of_stem("s01/f2", stems).value(), "beta");
    CB_CHECK(!name_of_stem("s02/f1", stems));
});

CB_TEST("build.module.pack_holes_puts_each_function_in_its_own_hole_when_it_fits", [] {
    using namespace internal;
    OrderedMap<std::string, std::uint32_t> size;
    size.insert_or_assign("a", 8);
    size.insert_or_assign("b", 0x20);
    // a fits its hole; b does not fit its own and takes the room the unclaimed hole has left
    const std::vector<HoleSpec> holes = {{0x10, 0x20, "a"}, {0x20, 0x30, "b"}, {0x30, 0x50, std::nullopt}};
    const Packed packed = pack_holes(holes, size, {}, {}, std::nullopt);
    CB_CHECK_EQ(packed.bins.size(), 3u);
    CB_CHECK_EQ(packed.bins[0].objects.size(), 1u);
    CB_CHECK_EQ(packed.bins[0].used, 0x18u);
    CB_CHECK(packed.bins[1].objects.empty());
    CB_CHECK_EQ(packed.bins[2].objects.size(), 1u);
    CB_CHECK_EQ(packed.bins[2].objects[0].second, "b");
    CB_CHECK(packed.no_room_far.empty() && packed.no_room_near.empty());
});

CB_TEST("build.module.pack_holes_aligns_to_a_word_and_says_who_found_no_room", [] {
    using namespace internal;
    OrderedMap<std::string, std::uint32_t> size;
    size.insert_or_assign("odd", 4);
    size.insert_or_assign("big", 0x100);
    size.insert_or_assign("near_big", 0x100);
    const std::set<std::string> near = {"near_big"};
    const std::vector<HoleSpec> holes = {{0x11, 0x20, "odd"}, {0x20, 0x30, "big"}, {0x30, 0x40, "near_big"}};
    const Packed packed = pack_holes(holes, size, near, {}, std::nullopt);
    CB_CHECK_EQ(packed.bins[0].objects[0].first, 1u);   // (a hole that starts on an odd byte: the object is aligned)
    CB_CHECK_EQ(packed.bins[0].used, 0x16u);
    CB_CHECK_EQ(packed.no_room_far.size(), 1u);
    CB_CHECK_EQ(packed.no_room_far[0], "big");
    CB_CHECK_EQ(packed.no_room_near.size(), 1u);
    CB_CHECK_EQ(packed.no_room_near[0], "near_big");
});

CB_TEST("build.module.pack_holes_keeps_what_was_below_the_cut_below_it", [] {
    using namespace internal;
    OrderedMap<std::string, std::uint32_t> size;
    size.insert_or_assign("low", 0x30);   // too big for its own hole
    size.insert_or_assign("tiny", 0x4);
    const std::vector<HoleSpec> holes = {{0x00, 0x10, "low"}, {0x10, 0x18, "tiny"}, {0x40, 0x80, std::nullopt}};
    // without a limit `low` goes to the free hole; with the segment cut at 0x20 it may not (the hole ends past the cut)
    CB_CHECK(pack_holes(holes, size, {}, {}, std::nullopt).no_room_far.empty());
    const Packed cut = pack_holes(holes, size, {}, {}, 0x20u);
    CB_CHECK_EQ(cut.no_room_far.size(), 1u);
    CB_CHECK_EQ(cut.no_room_far[0], "low");
});

CB_TEST("build.module.resident_limit_of_finds_the_realloc_of_cs", [] {
    using namespace internal;
    // mov cx, 0A940h / push cs / push dx / push cx / push dx
    const Bytes code = {0x90, 0x90, 0xB9, 0x40, 0xA9, 0x0E, 0x52, 0x51, 0x52, 0x0E, 0x90, 0x90};
    CB_CHECK_EQ(resident_limit_of(code).value(), 0xA940u);
    CB_CHECK(!resident_limit_of(Bytes{0x90, 0x90, 0x90}));
    CB_CHECK(!resident_limit_of(Bytes{}));
});

CB_TEST("build.module.result_in_ax_is_where_the_function_loads_what_it_returns", [] {
    using namespace internal;
    const Bytes loads = {0xB8, 0x05, 0x00, 0xC3};   // mov ax, 5 / ret
    CB_CHECK_EQ(result_in_ax(loads, 0, 4).value(), 0u);
    const Bytes gives_back = {0x50, 0x58, 0xC3};     // push ax / pop ax / ret: its own AX given back
    CB_CHECK(!result_in_ax(gives_back, 0, 3));
});

CB_TEST("build.module.stand_in_from_header_declares_what_the_header_declares", [] {
    using namespace internal;
    const std::string header = "#define X 1\nint __stdcall Foo(int a, int b);\nextern long Bar(void);\n";
    CB_CHECK_EQ(stand_in_from_header(header, "_Foo@8").value(), "int __stdcall Foo(int a, int b)\n{\n  return 0;\n}\n");
    CB_CHECK_EQ(stand_in_from_header(header, "Bar_").value(), "long Bar(void)\n{\n  return 0;\n}\n");
    // never declared: what a call to an undeclared name assumes, or __stdcall with the words its symbol says
    CB_CHECK_EQ(stand_in_from_header(header, "_Baz@8").value(), "int __stdcall Baz(int a0, int a1)\n{\n  return 0;\n}\n");
    CB_CHECK_EQ(stand_in_from_header(header, "_Baz@0").value(), "int __stdcall Baz(void)\n{\n  return 0;\n}\n");
    CB_CHECK_EQ(stand_in_from_header(header, "Qux").value(), "int Qux()\n{\n  return 0;\n}\n");
    CB_CHECK(!stand_in_from_header(header, "?Odd@@YAXXZ"));
});

CB_TEST("build.module.whole_file_image_says_the_whole_file_is_the_image", [] {
    fs::ScopedTempDir dir("cb-module");
    // an original whose MZ header says its whole file (pages * 512 covers the size), and a stub alone
    Bytes whole(0x400, 0);
    whole[0] = 'M';
    whole[1] = 'Z';
    whole[2] = 0;
    whole[4] = 2;   // 2 pages
    Bytes stub = whole;
    stub[4] = 1;    // 1 page: the header covers less than the file
    Bytes image(0x1234, 7);
    image[0] = 'M';
    image[1] = 'Z';
    mt_write(dir.path() / "whole.exe", whole);
    mt_write(dir.path() / "stub.exe", stub);
    mt_write(dir.path() / "image.bin", image);
    whole_file_image(dir.path() / "stub.exe", dir.path() / "image.bin");
    CB_CHECK(fs::read_file(dir.path() / "image.bin") == image);   // (a stub alone: nothing to do)
    whole_file_image(dir.path() / "whole.exe", dir.path() / "image.bin");
    const Bytes patched = fs::read_file(dir.path() / "image.bin");
    CB_CHECK_EQ(patched.size(), image.size());
    CB_CHECK_EQ(patched[2] | patched[3] << 8, 0x1234 % 512);
    CB_CHECK_EQ(patched[4] | patched[5] << 8, (0x1234 + 511) / 512);
    CB_CHECK(Bytes(patched.begin() + 6, patched.end()) == Bytes(image.begin() + 6, image.end()));
});

CB_TEST("build.module.report_says_why_nothing_linked", [] {
    Built b("C:/corpus/Lz32.DLL", "pe");
    CB_CHECK_EQ(b.name, "lz32.dll");
    b.error = "link: nothing";
    CB_CHECK_EQ(b.report(), "lz32.dll: NOT BUILT: link: nothing");
    b.carried_report = "as another process wrote it";
    CB_CHECK_EQ(b.report(), "as another process wrote it");
});

CB_TEST("build.module.report_counts_stand_ins_and_names_what_was_not_carried", [] {
    fs::ScopedTempDir dir("cb-module");
    mt_write(dir.path() / "orig.dll", Bytes(1000, 1));
    mt_write(dir.path() / "built.dll", Bytes(500, 1));
    Built b(dir.path() / "orig.dll", "ne");
    b.image = dir.path() / "built.dll";
    b.functions = 10;
    b.with_text = 8;
    b.rejected.insert_or_assign("f", "E1: no");
    b.absent = {"g"};
    b.data_error = "the data does not compile: x";
    b.entry = "none, as in the original";
    b.format_same = true;
    b.exports_total = 3;
    b.exports_same = 2;
    b.exports_missing.push_back({mt_export(3, std::nullopt, 0), "in the build as None"});
    b.imports_built = 1;
    b.imports_original = 2;
    const std::string report = b.report();
    CB_CHECK(report.starts_with("orig.dll: linked, 500 bytes (the original is 1000: 50%)\n"));
    CB_CHECK(mt_has(report, "8 with reconstructed C, 7 compiled, 1 rejected by the compiler (stand-ins)"));
    CB_CHECK(mt_has(report, "1 called but not reconstructed (stand-ins), 0 left unresolved"));
    CB_CHECK(mt_has(report, "data        NOT CARRIED: the data does not compile: x"));
    CB_CHECK(mt_has(report, "missing: (no name) @3 -- in the build as None"));
    CB_CHECK(mt_has(report, "resources   the original has none"));
});

CB_TEST("build.module.check_compares_format_exports_and_imports", [] {
    fs::ScopedTempDir dir("cb-module");
    mt_write(dir.path() / "orig.dll", Bytes(64, 1));
    mt_write(dir.path() / "built.dll", Bytes(64, 2));
    definitions::Interface original, built;
    original.library = built.library = true;
    original.exports = {mt_export(1, "Alpha", 0x100), mt_export(2, "Beta", 0x200), mt_export(3, std::nullopt, 0x300), mt_export(4, "Delta", 0x400)};
    built.exports = {mt_export(1, "Alpha", 0x100), mt_export(2, "Gamma", 0x200), mt_export(3, "ordinal_3", 0x300)};
    original.imports = {{"KERNEL32", std::string("Sleep")}};
    built.imports = {{"KERNEL32", std::string("Sleep")}, {"USER32", std::uint32_t{5}}, {"GDI32", std::string("Line")}};
    Services services;
    services.interface_of = [&](const std::filesystem::path& p) { return p.filename() == "orig.dll" ? original : built; };
    Built b(dir.path() / "orig.dll", "pe");
    b.image = dir.path() / "built.dll";
    b.stand_in_addresses.insert(Address(std::uint64_t{0x100}));
    ModuleBuilder(services).check(b);
    CB_CHECK(b.format_same);
    CB_CHECK_EQ(b.exports_total, 4u);
    CB_CHECK_EQ(b.exports_same, 2u);          // Alpha, and the one with no name that the linker names ordinal_3
    CB_CHECK_EQ(b.exports_on_stand_ins, 1u);  // Alpha is at a function the compiler rejected
    CB_CHECK_EQ(b.exports_missing.size(), 2u);
    CB_CHECK_EQ(b.exports_missing[0].what.ordinal, 2u);
    CB_CHECK_EQ(b.exports_missing[0].why, "in the build as 'Gamma'");
    CB_CHECK_EQ(b.exports_missing[1].why, "in the build as None");
    CB_CHECK_EQ(b.imports_original, 1u);
    CB_CHECK_EQ(b.imports_built, 3u);
    CB_CHECK_EQ(b.imports_extra.size(), 2u);
    CB_CHECK_EQ(b.imports_extra[0].module, "GDI32");   // (sorted)
    CB_CHECK_EQ(b.imports_extra[1].module, "USER32");
    // a library is not a program
    built.library = false;
    Built c(dir.path() / "orig.dll", "pe");
    c.image = dir.path() / "built.dll";
    ModuleBuilder(services).check(c);
    CB_CHECK(!c.format_same);
});

CB_TEST("build.module.run_calls_each_code_export_in_the_build_and_in_the_original", [] {
    fs::ScopedTempDir dir("cb-module");
    mt_write(dir.path() / "orig.dll", Bytes(64, 1));
    mt_write(dir.path() / "built.dll", Bytes(64, 2));
    definitions::Interface original, built;
    original.exports = {mt_export(1, "Same", 0x100), mt_export(2, "Differs", 0x200), mt_export(3, std::nullopt, 0x300), mt_export(4, "NotBuilt", 0x400)};
    built.exports = {mt_export(1, "Same", 0x1100), mt_export(2, "Differs", 0x1200), mt_export(3, "ordinal_3", 0x1300)};
    Services services;
    services.interface_of = [&](const std::filesystem::path& p) { return p.filename() == "orig.dll" ? original : built; };
    std::vector<std::string> called;
    services.call_export = [&](const std::filesystem::path& module, std::string_view kind, const definitions::Export& e) {
        called.push_back(std::format("{}:{}:{}", module.filename().string(), kind, e.ordinal));
        const bool mine = module.filename() == "built.dll";
        return CallOutcome{"ret", e.ordinal == 2 && mine ? std::optional<std::int64_t>(1) : std::optional<std::int64_t>(0)};
    };
    Built b(dir.path() / "orig.dll", "ne");
    b.image = dir.path() / "built.dll";
    ModuleBuilder(services).run(b);
    CB_CHECK_EQ(b.ran.size(), 3u);   // (NotBuilt is not in the build)
    CB_CHECK_EQ(b.ran[0].name, "Same");
    CB_CHECK(b.ran[0].same);
    CB_CHECK(!b.ran[1].same);        // the same end, another result
    CB_CHECK_EQ(b.ran[2].name, "@3");
    CB_CHECK_EQ(called.size(), 6u);
    CB_CHECK_EQ(called[0], "built.dll:ne:1");
    CB_CHECK_EQ(called[1], "orig.dll:ne:1");
    // the limit
    Built c(dir.path() / "orig.dll", "ne");
    c.image = dir.path() / "built.dll";
    ModuleBuilder(services).run(c, 1);
    CB_CHECK_EQ(c.ran.size(), 1u);
});

CB_TEST("build.module.the_gate_runs_a_function_once_and_keeps_the_verdict", [] {
    fs::ScopedTempDir dir("cb-module");
    mt_write(dir.path() / "orig.dll", Bytes(64, 9));
    decompile::c_text::Module16 m;
    m.made = translate::l2::Made("orig.dll", "typedef int int2;\n");
    m.made.add("same", "int2 same() { return 1; }", 0x10000);
    m.made.add("differs", "int2 differs() { return 2; }", 0x10100);
    m.made.add("unsure", "int2 unsure() { return 3; }", 0x10200);
    MtMemoryCache cache;
    Services services;
    services.cache = &cache;
    int checked = 0;
    services.gate_check = [&](const std::filesystem::path&, const std::filesystem::path&, const std::vector<std::string>& names) {
        checked += static_cast<int>(names.size());
        std::vector<GateResult> out;
        for (const std::string& n : names) {
            out.push_back({n, n == "differs" ? GateResult::Verdict::Divergent : n == "unsure" ? GateResult::Verdict::Inconclusive : GateResult::Verdict::Same,
                           n == "differs" ? "returns 2, the original 1" : n == "unsure" ? "the checker failed: x" : ""});
        }
        return out;
    };
    const ModuleBuilder builder(services);
    const std::vector<std::string> names = {"same", "differs", "unsure"};
    CB_CHECK(!builder.gate_known(dir.path() / "orig.dll", m));   // (nothing recorded yet)
    std::vector<std::pair<std::string, std::string>> not_run;
    const auto first = builder.gate(dir.path() / "orig.dll", dir.path(), names, m, &not_run);
    CB_CHECK_EQ(checked, 3);
    CB_CHECK_EQ(first.size(), 1u);
    CB_CHECK_EQ(first.at("differs"), "returns 2, the original 1");
    CB_CHECK_EQ(not_run.size(), 1u);
    CB_CHECK_EQ(not_run[0].first, "unsure");
    // again: the two verdicts are kept (and the divergence comes back with its reason), only the one with none is run
    checked = 0;
    const auto second = builder.gate(dir.path() / "orig.dll", dir.path(), names, m, &not_run);
    CB_CHECK_EQ(checked, 1);
    CB_CHECK_EQ(second.at("differs"), "returns 2, the original 1");
    // the build records which functions it ran; with a verdict kept for each of them the module is built once
    const std::string kept = "same\ndiffers";
    cache.put("gate16_set", base::cache::key({fs::read_file(dir.path() / "orig.dll"), m.made.header()}),
              ByteView(reinterpret_cast<const std::uint8_t*>(kept.data()), kept.size()));
    const auto known = builder.gate_known(dir.path() / "orig.dll", m);
    CB_CHECK(known.has_value());
    CB_CHECK_EQ(known->size(), 1u);
    CB_CHECK_EQ(known->at("differs"), "returns 2, the original 1");
    // a changed text is a verdict to make again
    m.made.set_text("same", "int2 same() { return 4; }");
    CB_CHECK(!builder.gate_known(dir.path() / "orig.dll", m));
});

CB_TEST("build.module.document32_places_each_function_by_its_address", [] {
    decompile::c_text::Compilable32 m;
    m.by_va = {{0x6E002000, "second"}, {0x6E001000, "first"}};
    Arena arena;
    ir::Node* doc = build::module::module_document32(arena, "lz32.dll", m);
    const std::vector<ir::Node*> units = translate::l2::units(doc);
    CB_CHECK_EQ(units.size(), 2u);
    const auto* first = ir::get<ir::MethodDecl>(units[0]);
    CB_CHECK(first != nullptr);
    CB_CHECK_EQ(first->name.value_or(""), "first");   // (ascending addresses, whatever order they were given in)
    CB_CHECK(units[0]->origin != nullptr);
    const auto* origin = std::get_if<ir::OriginVa>(units[0]->origin.get());
    CB_CHECK(origin != nullptr);
    CB_CHECK_EQ(origin->va, 0x6E001000u);
    CB_CHECK_EQ(origin->module, "lz32.dll");
});

CB_TEST("build.module.document16_holds_assembled_functions_as_fragments_that_share_their_file", [] {
    fs::ScopedTempDir dir("cb-module");
    fs::make_dirs(dir.path() / "asm");
    fs::write_text(dir.path() / "asm" / "run.asm", "CODE1 segment\nend\n");
    decompile::c_text::Module16 m;
    m.made = translate::l2::Made("user.exe", "");
    m.made.add("c_function", "int c_function() { return 0; }", 0x10000);
    m.made.add("asm_one", std::nullopt, 0x10010);
    m.made.add("asm_two", std::nullopt, 0x10030);
    m.where = {{0x10000, {1, 0}}, {0x10010, {1, 0x10}}, {0x10030, {1, 0x30}}};
    OrderedMap<std::string, std::string> assembled;
    assembled.insert_or_assign("asm_one", "written in assembly: x");
    assembled.insert_or_assign("asm_two", "its C does not compile: y");
    const std::map<std::string, std::string> stems = {{"asm_one", "asm/run"}, {"asm_two", "asm/run"}, {"c_function", "s01/f1"}};
    Arena arena;
    ir::Node* doc = build::module::module_document(arena, "user.exe", m, assembled, stems, dir.path(), {{1, 0x50}});
    const std::vector<ir::Node*> units = translate::l2::units(doc);
    CB_CHECK_EQ(units.size(), 3u);
    CB_CHECK(ir::get<ir::MethodDecl>(units[0]) != nullptr);
    const auto* one = ir::get<ir::MachineFragment>(units[1]);
    const auto* two = ir::get<ir::MachineFragment>(units[2]);
    CB_CHECK(one != nullptr && two != nullptr);
    CB_CHECK_EQ(one->length.value_or(0), 0x20u);        // up to the next function
    CB_CHECK_EQ(two->length.value_or(0), 0x20u);        // up to the segment's end
    CB_CHECK_EQ(one->assembly.value_or(""), "CODE1 segment\nend\n");   // the text is on the first
    CB_CHECK(!two->assembly.has_value());
    CB_CHECK_EQ(two->within.value_or(""), "asm_one");                    // and the second points at it
    const auto* origin = std::get_if<ir::OriginSegment>(units[1]->origin.get());
    CB_CHECK(origin != nullptr);
    CB_CHECK_EQ(origin->offset, 0x10);
});

CB_TEST("build.module.build_names_a_module_that_is_not_in_the_corpus", [] {
    Services services;
    services.retail = [](std::string_view) { return std::nullopt; };
    CB_CHECK_THROWS_AS(UsageError, ModuleBuilder(services).build("nothing.dll"));
});

CB_TEST("build.module.build_without_openwatcom_is_a_usage_error_and_a_foreign_kind_is_refused", [] {
    fs::ScopedTempDir dir("cb-module");
    mt_write(dir.path() / "m.dll", Bytes(64, 1));
    Services services;
    services.retail = [&](std::string_view) { return std::optional<std::filesystem::path>(dir.path() / "m.dll"); };
    services.build_folder = [&](std::string_view name) { return dir.path() / "build" / std::string(name); };
    services.kind_of = [](const std::filesystem::path&) { return std::optional<std::string>("dos"); };
    CB_CHECK_THROWS_AS(UsageError, ModuleBuilder(services).build("m.dll"));   // a program that is not linked whole here
    services.kind_of = [](const std::filesystem::path&) { return std::optional<std::string>("ne"); };
    int prepared = 0;
    services.prepare = [&](const std::filesystem::path&, const std::filesystem::path&) {
        ++prepared;
        Prepared p;
        p.functions = 1;
        p.win16 = std::make_shared<decompile::c_text::Module16>();
        p.win16->description = std::make_shared<decompile::describe::NeDescription>();
        return p;
    };
    // (no OpenWatcom root: the compiler is not set up)
    CB_CHECK_THROWS_AS(UsageError, ModuleBuilder(services).build("m.dll", {.outdir = dir.path() / "out"}));
    CB_CHECK_EQ(prepared, 1);
});

CB_TEST("build.module.build_many_gives_each_module_its_own_error", [] {
    fs::ScopedTempDir dir("cb-module");
    mt_write(dir.path() / "a.dll", Bytes(64, 1));
    mt_write(dir.path() / "b.dll", Bytes(64, 1));
    Services services;
    services.retail = [&](std::string_view name) -> std::optional<std::filesystem::path> {
        if (name == "gone.dll") return std::nullopt;
        return dir.path() / std::string(name);
    };
    services.build_folder = [&](std::string_view name) { return dir.path() / "build" / std::string(name); };
    services.kind_of = [](const std::filesystem::path& p) { return std::optional<std::string>(p.filename() == "a.dll" ? "dos" : "pe"); };
    services.workers = [] { return 1; };
    services.prepare = [](const std::filesystem::path& m, const std::filesystem::path&) -> Prepared { throw FormatError(m.filename().string(), 0, "cannot open for writing"); };
    const auto made = ModuleBuilder(services).build_many({"a.dll", "b.dll", "gone.dll"}, {}, false);
    CB_CHECK_EQ(made.size(), std::size_t{3});
    CB_CHECK_EQ(made[0].first, "a.dll");
    CB_CHECK(made[0].second.error.find("dos") != std::string::npos);        // a's own: a dos module is not linked whole
    CB_CHECK(made[1].second.error.find("cannot open for writing") != std::string::npos);   // b's own: its prepare failed
    CB_CHECK(made[1].second.error.find("dos") == std::string::npos);
    CB_CHECK(made[2].second.error.find("no module") != std::string::npos);   // the third is not in the corpus
    for (const auto& [name, b] : made) CB_CHECK(!b.linked());
});

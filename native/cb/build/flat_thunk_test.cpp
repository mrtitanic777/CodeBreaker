#include "cb/build/flat_thunk.hpp"

#include <algorithm>
#include <cstring>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::build;
namespace ft = cb::build::flat_thunk;
namespace stdfs = std::filesystem;

namespace {

Bytes bytes_of(std::initializer_list<unsigned> v) {
    Bytes out;
    for (const unsigned b : v) out.push_back(static_cast<std::uint8_t>(b));
    return out;
}

stdfs::path formats_data(std::string_view name) { return testing::data_path("formats/testdata/" + std::string(name)); }
stdfs::path golden_file(std::string_view name) { return testing::data_path("build/testdata/flat_thunk/" + std::string(name)); }

std::string lf(std::string s) { return text::replace_all(s, "\r\n", "\n"); }

// The small SDK table the golden files were made with (testdata/flat_thunk/sdk.txt: name|returns|params|convention|header).
std::map<std::string, ft::SdkPrototype> small_sdk() {
    std::map<std::string, ft::SdkPrototype> out;
    const std::string sdk_text = fs::read_text(golden_file("sdk.txt"));
    for (const std::string_view line : text::lines(sdk_text)) {
        const auto f = text::split(line, '|');
        if (f.size() == 5) out[std::string(f[0])] = {std::string(f[1]), std::string(f[2]), std::string(f[3]), std::string(f[4])};
    }
    return out;
}

}  // namespace

// ---- the stub reader ----------------------------------------------------------------------------------------------

CB_TEST("build.flat_thunk.instruction_length_of_what_a_stub_uses", [] {
    const auto len = [](std::initializer_list<unsigned> v) {
        const Bytes b = bytes_of(v);
        return ft::instruction_length(b, 0);
    };
    CB_CHECK_EQ(len({0x55}), std::size_t{1});                          // push ebp
    CB_CHECK_EQ(len({0x8B, 0xEC}), std::size_t{2});                    // mov ebp, esp
    CB_CHECK_EQ(len({0x8B, 0x45, 0x08}), std::size_t{3});              // mov eax, [ebp+8]
    CB_CHECK_EQ(len({0x8B, 0x85, 1, 2, 3, 4}), std::size_t{6});        // mov eax, [ebp+disp32]
    CB_CHECK_EQ(len({0x8B, 0x04, 0x24}), std::size_t{3});              // mov eax, [esp]   (SIB, no displacement)
    CB_CHECK_EQ(len({0x8B, 0x04, 0x25, 0, 0, 0, 0}), std::size_t{7});  // SIB with base 5 and mod 0: disp32
    CB_CHECK_EQ(len({0x8B, 0x05, 0, 0, 0, 0}), std::size_t{6});        // mov eax, [abs32]
    CB_CHECK_EQ(len({0x68, 1, 2, 3, 4}), std::size_t{5});              // push imm32
    CB_CHECK_EQ(len({0x66, 0x68, 1, 2}), std::size_t{4});              // operand-size prefix: imm16
    CB_CHECK_EQ(len({0x6A, 0x01}), std::size_t{2});                    // push imm8
    CB_CHECK_EQ(len({0xE8, 1, 2, 3, 4}), std::size_t{5});              // call rel32
    CB_CHECK_EQ(len({0xEB, 0x05}), std::size_t{2});                    // jmp rel8
    CB_CHECK_EQ(len({0x74, 0x05}), std::size_t{2});                    // je rel8
    CB_CHECK_EQ(len({0x0F, 0x84, 1, 2, 3, 4}), std::size_t{6});        // je rel32
    CB_CHECK_EQ(len({0x0F, 0xB6, 0xC0}), std::size_t{3});              // movzx eax, al
    CB_CHECK_EQ(len({0xC2, 0x08, 0x00}), std::size_t{3});              // ret 8
    CB_CHECK_EQ(len({0xC3}), std::size_t{1});
    CB_CHECK_EQ(len({0xF7, 0xC0, 1, 2, 3, 4}), std::size_t{6});        // test eax, imm32 (group 3 /0 has an immediate)
    CB_CHECK_EQ(len({0xF7, 0xD8}), std::size_t{2});                    // neg eax (/3 has none)
    CB_CHECK_EQ(len({0xF6, 0xC0, 1}), std::size_t{3});                 // test al, imm8
    CB_CHECK_EQ(len({0x83, 0xC4, 0x04}), std::size_t{3});              // add esp, 4
    CB_CHECK_EQ(len({0x81, 0xC4, 1, 2, 3, 4}), std::size_t{6});        // add esp, imm32
    CB_CHECK_EQ(len({0xFF, 0x15, 0, 0x40, 0, 0}), std::size_t{6});     // call [abs32]
    CB_CHECK_EQ(len({0xB8, 1, 2, 3, 4}), std::size_t{5});              // mov eax, imm32
    CB_CHECK_EQ(len({0xB0, 1}), std::size_t{2});                       // mov al, imm8
    CB_CHECK_EQ(len({0x05, 1, 2, 3, 4}), std::size_t{5});              // add eax, imm32
    CB_CHECK_EQ(len({0x04, 1}), std::size_t{2});                       // add al, imm8
    CB_CHECK_EQ(len({0x3C, 1}), std::size_t{2});                       // cmp al, imm8
    CB_CHECK_EQ(len({0xA1, 1, 2, 3, 4}), std::size_t{5});              // mov eax, [moffs32]
    CB_CHECK_EQ(len({0xD9, 0x45, 0x08}), std::size_t{3});              // fld dword [ebp+8]
    // not modelled, or cut short: 0, so the caller stops instead of mis-measuring
    CB_CHECK_EQ(len({0xD4, 0x0A}), std::size_t{0});
    CB_CHECK_EQ(len({0x0F, 0xFF}), std::size_t{0});
    CB_CHECK_EQ(len({0x66}), std::size_t{0});
    CB_CHECK_EQ(len({0x0F}), std::size_t{0});
    CB_CHECK_EQ(ft::instruction_length(Bytes{}, 0), std::size_t{0});
    CB_CHECK_EQ(ft::instruction_length(bytes_of({0x55}), 5), std::size_t{0});
});

CB_TEST("build.flat_thunk.a_stub_says_how_many_argument_bytes_and_pointers", [] {
    const std::set<std::uint32_t> marshal = {0x4000, 0x2000};
    const auto sig = [&](std::initializer_list<unsigned> v) {
        const Bytes b = bytes_of(v);
        return ft::stub_signature(b, 0x1000, marshal);
    };
    // push ebp; mov ebp, esp; mov eax, [ebp+8]; pop ebp; ret 4
    auto s = sig({0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x5D, 0xC2, 0x04, 0x00});
    CB_CHECK_EQ(s.argument_bytes.value_or(99), 4u);
    CB_CHECK_EQ(s.pointer_args, 0u);
    // call [0x4000] (an IAT slot of SMapLS): one pointer argument
    s = sig({0xFF, 0x15, 0x00, 0x40, 0x00, 0x00, 0xFF, 0x15, 0x00, 0x40, 0x00, 0x00, 0xC2, 0x08, 0x00});
    CB_CHECK_EQ(s.argument_bytes.value_or(99), 8u);
    CB_CHECK_EQ(s.pointer_args, 2u);
    // call rel32 to 0x2000 (a `jmp [slot]` thunk), from 0x1000: rel = 0x2000 - 0x1005
    s = sig({0xE8, 0xFB, 0x0F, 0x00, 0x00, 0xC3});
    CB_CHECK_EQ(s.argument_bytes.value_or(99), 0u);   // a plain `ret`: no cleanup
    CB_CHECK_EQ(s.pointer_args, 1u);
    // a call elsewhere is no marshalling
    CB_CHECK_EQ(sig({0xE8, 0xFC, 0x0F, 0x00, 0x00, 0xC3}).pointer_args, 0u);
    // a `ret N` that is not a stdcall count resolves nothing: odd, or past 256
    CB_CHECK(!sig({0xC2, 0x05, 0x00}).argument_bytes.has_value());
    CB_CHECK(!sig({0xC2, 0x04, 0x01}).argument_bytes.has_value());
    CB_CHECK_EQ(sig({0xC2, 0x00, 0x01}).argument_bytes.value_or(99), 256u);   // 0x100 = 256 is the largest allowed
    // int3 padding before any return: unknown
    CB_CHECK(!sig({0x55, 0xCC, 0xC2, 0x04, 0x00}).argument_bytes.has_value());
    // nothing there
    CB_CHECK(!ft::stub_signature(Bytes{}, 0x1000, marshal).argument_bytes.has_value());
    // an opcode that is not modelled ends the walk; the epilogue scan then needs the same `ret N` twice (or once right after
    // a pop/leave)
    s = sig({0xD4, 0x0A, 0x5D, 0xC2, 0x08, 0x00, 0x90, 0x5D, 0xC2, 0x08, 0x00});
    CB_CHECK_EQ(s.argument_bytes.value_or(99), 8u);
    CB_CHECK(!sig({0xD4, 0x90, 0xC2, 0x08, 0x00}).argument_bytes.has_value());
});

CB_TEST("build.flat_thunk.a_ret_cut_off_inside_its_operand_resolves_nothing", [] {
    // DIVERGES: the Python read the two operand bytes with struct.unpack and raised struct.error when the stub's extent ended
    // inside them (a decode desync that landed on 0xC2 at the end); the result is an unresolved signature
    const std::set<std::uint32_t> none;
    CB_CHECK(!ft::stub_signature(bytes_of({0x55, 0xC2}), 0x1000, none).argument_bytes.has_value());
    CB_CHECK(!ft::stub_signature(bytes_of({0x55, 0xC2, 0x04}), 0x1000, none).argument_bytes.has_value());
    CB_CHECK_EQ(ft::stub_signature(bytes_of({0x55, 0xC2, 0x04, 0x00}), 0x1000, none).argument_bytes.value_or(99), 4u);
});

CB_TEST("build.flat_thunk.argument_bytes_by_epilogue", [] {
    const auto by = [](std::initializer_list<unsigned> v) { return ft::argument_bytes_by_epilogue(bytes_of(v)); };
    CB_CHECK(!by({}).has_value());
    CB_CHECK(!by({0x90, 0x90}).has_value());
    CB_CHECK_EQ(by({0x5D, 0xC2, 0x04, 0x00}).value_or(99), 4u);                  // once, right after a pop
    CB_CHECK_EQ(by({0xC9, 0xC2, 0x0C, 0x00}).value_or(99), 12u);                 // once, right after leave
    CB_CHECK(!by({0x90, 0xC2, 0x04, 0x00}).has_value());                         // once, after nothing that ends a function
    CB_CHECK_EQ(by({0x90, 0xC2, 0x04, 0x00, 0x90, 0xC2, 0x04, 0x00}).value_or(99), 4u);   // twice
    CB_CHECK(!by({0x5D, 0xC2, 0x04, 0x00, 0x5D, 0xC2, 0x08, 0x00}).has_value());   // two single candidates: ambiguous
    CB_CHECK_EQ(by({0xC2, 0x04, 0x00, 0xC2, 0x08, 0x00, 0xC2, 0x08, 0x00, 0xC2, 0x04, 0x00}).value_or(99), 4u);   // a tie: the first met
    CB_CHECK_EQ(by({0xC2, 0x04, 0x00, 0xC2, 0x08, 0x00, 0xC2, 0x08, 0x00}).value_or(99), 8u);
    CB_CHECK(!by({0xC2, 0x00, 0x00, 0x5D}).has_value());                         // N = 0 is not a stdcall count here
    CB_CHECK(!by({0x5D, 0xC2, 0x05, 0x00}).has_value());                         // not a multiple of 4
    CB_CHECK(!by({0x5D, 0xC2, 0x04, 0x01}).has_value());                         // the high byte must be 0
});

// ---- classify and diff ----------------------------------------------------------------------------------------------

CB_TEST("build.flat_thunk.classify", [] {
    // the answers of the Python's classify on the same files
    auto c = ft::classify(formats_data("lz32.dll"));
    CB_CHECK(c.format == ft::Format::Pe);
    CB_CHECK(c.subtype == ft::Subtype::FlatThunk);
    CB_CHECK_EQ(c.recipe.value_or("?"), "thunk-ml-cl-link");
    CB_CHECK_EQ(c.exports.value_or(0), std::uint64_t{12});
    CB_CHECK_EQ(c.imports.value_or(0), std::uint64_t{17});
    CB_CHECK_EQ(c.why, "imports flat-thunk helpers (SMapLS,SUnMapLS)");

    c = ft::classify(formats_data("lzexpand.dll"));
    CB_CHECK(c.format == ft::Format::Ne);
    CB_CHECK(c.subtype == ft::Subtype::Win16C);
    CB_CHECK_EQ(c.recipe.value_or("?"), "msc6-dosbox");
    CB_CHECK_EQ(c.why, "16-bit NE; 12 segments");
    CB_CHECK(!c.exports.has_value());

    c = ft::classify(formats_data("xcopy32.exe"));
    CB_CHECK(c.subtype == ft::Subtype::Win32C);
    CB_CHECK_EQ(c.recipe.value_or("?"), "msvc-native");
    CB_CHECK_EQ(c.exports.value_or(99), std::uint64_t{0});
    CB_CHECK_EQ(c.why, "32-bit PE, no flat-thunk markers; .text=5130 B");
    CB_CHECK_EQ(ft::classify(formats_data("comcat.dll")).exports.value_or(0), std::uint64_t{4});

    // not a PE or an NE: unknown (an LE driver, a .COM file, a DOS program, a file that is not there)
    for (const char* name : {"bios.vxd", "choice.com", "setver.exe", "no_such_file.dll"}) {
        c = ft::classify(formats_data(name));
        CB_CHECK(c.format == ft::Format::Unknown);
        CB_CHECK(c.subtype == ft::Subtype::Unknown);
        CB_CHECK(!c.recipe.has_value());
        CB_CHECK_EQ(c.why, "not MZ/PE/NE");
    }
    CB_CHECK_EQ(ft::subtype_name(ft::Subtype::FlatThunk), "flat-thunk");
    CB_CHECK_EQ(ft::format_name(ft::Format::Unknown), "?");
});

CB_TEST("build.flat_thunk.classify_finds_kernel32_whatever_the_case_of_its_name", [] {
    // (the three flat-thunk modules of the test data import it as KERNEL32.dll; the lookup does not care)
    for (const char* name : {"version.dll", "dciman32.dll", "tapi32.dll", "mpr.dll"}) {
        CB_CHECK(ft::classify(golden_file(name)).subtype == ft::Subtype::FlatThunk);
    }
});

CB_TEST("build.flat_thunk.the_model_of_a_pe", [] {
    const ft::PeModule pe = ft::PeModule::open(formats_data("lz32.dll"));
    CB_CHECK_EQ(pe.entry_rva(), 0x1000u);
    const auto exports = pe.exports();
    CB_CHECK_EQ(exports.size(), std::size_t{12});
    CB_CHECK_EQ(exports.front().name, "LZCopy");
    CB_CHECK_EQ(exports.front().ordinal, std::uint64_t{1});
    CB_CHECK(std::is_sorted(exports.begin(), exports.end()));
    const auto imports = pe.imports();
    CB_CHECK_EQ(imports.size(), std::size_t{1});
    const auto* k32 = imports.find("KERNEL32.dll");
    CB_CHECK(k32 != nullptr);
    CB_CHECK_EQ(k32->size(), std::size_t{17});
    CB_CHECK_EQ(k32->front(), "@21");                // by ordinal
    CB_CHECK_EQ((*k32)[1], "SMapLS_IP_EBP_12");
    const auto addresses = pe.export_addresses();
    CB_CHECK_EQ(addresses.at("LZCopy"), 4522u);
    CB_CHECK_EQ(addresses.at("LZOpenFileA"), 4664u);
    CB_CHECK_EQ(pe.read_at(4522, 4).size(), std::size_t{4});
    CB_CHECK(pe.read_at(0x7FFFFFF0, 4).empty());     // not in the file
    CB_CHECK_THROWS_AS(FormatError, ft::PeModule::open(formats_data("lzexpand.dll")));
});

CB_TEST("build.flat_thunk.the_model_of_an_ne", [] {
    const ft::NeModule ne = ft::NeModule::open(formats_data("lzexpand.dll"));
    CB_CHECK_EQ(ne.segment_count(), std::size_t{12});
    const auto kinds = ne.segment_kinds();
    CB_CHECK_EQ(kinds.size(), std::size_t{12});
    CB_CHECK(kinds.front() == ft::SegmentKind::Code);
    CB_CHECK(kinds.back() == ft::SegmentKind::Data);
    CB_CHECK(!ne.resident_names().empty());
    CB_CHECK(ft::load(formats_data("lz32.dll")).has_value());
    CB_CHECK(!ft::load(formats_data("bios.vxd")).has_value());
});

CB_TEST("build.flat_thunk.bindiff", [] {
    // the same module twice: everything agrees, the imports too
    const auto same = ft::bindiff(formats_data("lz32.dll"), formats_data("lz32.dll"));
    CB_CHECK(same.ok());
    CB_CHECK(same.value().verdict == ft::Verdict::ByteSurfaceIdentical);
    CB_CHECK_EQ(same.value().size_built, std::uint64_t{5632});
    CB_CHECK_EQ(same.value().built, "lz32.dll");
    const auto& pe = std::get<ft::PeDiff>(same.value().detail);
    CB_CHECK(pe.entry_match && pe.sections_match && pe.exports_match);
    CB_CHECK_EQ(pe.entry.first, 0x1000u);
    CB_CHECK_EQ(pe.exports.first, std::size_t{12});
    CB_CHECK_EQ(pe.import_count.first, std::size_t{17});
    CB_CHECK_EQ(pe.import_dlls_built.size(), std::size_t{1});
    CB_CHECK(pe.imports_only_in_built.empty() && pe.imports_only_in_shipped.empty());
    const std::vector<std::string> sections = {".text", ".data", ".idata", ".edata", ".rsrc", ".reloc"};
    CB_CHECK(pe.sections_built == sections);

    // two different modules: partial, with the differences named
    const auto other = ft::bindiff(formats_data("lz32.dll"), formats_data("comcat.dll"));
    CB_CHECK(other.ok());
    CB_CHECK(other.value().verdict == ft::Verdict::Partial);
    const auto& od = std::get<ft::PeDiff>(other.value().detail);
    CB_CHECK(!od.exports_match);
    CB_CHECK(!od.imports_only_in_built.empty());
    CB_CHECK(std::is_sorted(od.imports_only_in_built.begin(), od.imports_only_in_built.end()));
    CB_CHECK(od.imports_only_in_built.front().find('!') != std::string::npos);

    // an NE pair
    const auto ne = ft::bindiff(formats_data("lzexpand.dll"), formats_data("lzexpand.dll"));
    CB_CHECK(ne.ok());
    CB_CHECK(ne.value().verdict == ft::Verdict::StructuralParity);
    const auto& nd = std::get<ft::NeDiff>(ne.value().detail);
    CB_CHECK(nd.kinds_match && nd.exports_match);
    CB_CHECK_EQ(nd.segments.first, std::size_t{12});

    // errors: unlike kinds, an input that is neither
    const auto mixed = ft::bindiff(formats_data("lzexpand.dll"), formats_data("lz32.dll"));
    CB_CHECK(!mixed.ok());
    CB_CHECK(mixed.error().kind() == "UsageError");
    CB_CHECK(!ft::bindiff(formats_data("lzexpand.dll"), formats_data("setver.exe")).ok());
    CB_CHECK(!ft::bindiff(formats_data("no_such_file.dll"), formats_data("lz32.dll")).ok());
});

CB_TEST("build.flat_thunk.format_diff", [] {
    const std::string same = ft::format_diff(ft::bindiff(formats_data("lz32.dll"), formats_data("lz32.dll")).value());
    CB_CHECK(same.starts_with("  kind=PE  size built=5632 shipped=5632  verdict=byte-surface-identical\n"));
    CB_CHECK(same.find("  entry 0x1000 / 0x1000 MATCH\n") != std::string::npos);
    CB_CHECK(same.find("  sections MATCH\n") != std::string::npos);
    CB_CHECK(same.find("  exports 12 / 12 MATCH\n") != std::string::npos);
    CB_CHECK(same.find("  imports 17 / 17  only-in-shipped=[]  only-in-built=[]\n") != std::string::npos);
    const std::string other = ft::format_diff(ft::bindiff(formats_data("lz32.dll"), formats_data("comcat.dll")).value());
    CB_CHECK(other.find("verdict=partial") != std::string::npos);
    CB_CHECK(other.find("  exports 12 / 4 differ\n") != std::string::npos);
    const std::string ne = ft::format_diff(ft::bindiff(formats_data("lzexpand.dll"), formats_data("lzexpand.dll")).value());
    CB_CHECK(ne.starts_with("  kind=NE  size built=9936 shipped=9936  verdict=structural-parity\n"));
    CB_CHECK(ne.find("  segments 12 / 12  kinds MATCH  exports MATCH\n") != std::string::npos);
});

// ---- the text helpers -----------------------------------------------------------------------------------------------

CB_TEST("build.flat_thunk.thunk_data_exports", [] {
    // data tables the thunk compiler emits: ...ThunkData32, ...ThunkData, a name holding _ThunkData, ...ThkSL, and
    // <alnum>Thk[SL][_]ThunkData... at the start
    for (const char* name : {"VerFThk_ThunkData32", "C16ThkSL01_ThunkData", "FooThunkData", "ThunkData", "ThkSL", "a_ThunkData_b",
                             "_Thk_ThunkData", "FooThkThunkDataX", "FooThkSL_ThunkDataX", "FooThkSLThunkDataX", "XThk_ThunkData32"}) {
        CB_CHECK(ft::is_thunk_data(name));
    }
    for (const char* name : {"LZCopy", "Thk", "Foo_Thk_ThunkX", "VerQueryValueA", "ThkThunk", "ThunkDat", "_ThkThunkDataX", "Foo-ThkThunkDataX"}) {
        CB_CHECK(!ft::is_thunk_data(name));
    }
});

CB_TEST("build.flat_thunk.parameter_bytes", [] {
    CB_CHECK_EQ(ft::parameter_bytes(""), 0u);
    CB_CHECK_EQ(ft::parameter_bytes("void"), 0u);
    CB_CHECK_EQ(ft::parameter_bytes("..."), 0u);
    CB_CHECK_EQ(ft::parameter_bytes("  void  "), 0u);
    CB_CHECK_EQ(ft::parameter_bytes("LPSTR lpName"), 4u);
    CB_CHECK_EQ(ft::parameter_bytes("const char *p"), 4u);
    CB_CHECK_EQ(ft::parameter_bytes("POINT pt"), 8u);
    CB_CHECK_EQ(ft::parameter_bytes("const RECT r"), 16u);
    CB_CHECK_EQ(ft::parameter_bytes("unsigned __int64 v"), 8u);
    CB_CHECK_EQ(ft::parameter_bytes("double d"), 8u);
    CB_CHECK_EQ(ft::parameter_bytes("far SIZE s"), 8u);
    CB_CHECK_EQ(ft::parameter_bytes("DWORD x"), 4u);
    CB_CHECK_EQ(ft::parameter_bytes("unsigned"), 4u);
});

CB_TEST("build.flat_thunk.split_parameters", [] {
    using V = std::vector<std::string>;
    CB_CHECK(ft::split_parameters("") == V{});
    CB_CHECK(ft::split_parameters("  ") == V{});
    CB_CHECK(ft::split_parameters("a") == V{"a"});
    CB_CHECK(ft::split_parameters("a, b ,c") == (V{"a", " b ", "c"}));
    CB_CHECK(ft::split_parameters("int (*cb)(int, int), HWND h") == (V{"int (*cb)(int, int)", " HWND h"}));
    CB_CHECK(ft::split_parameters("a,") == V{"a"});   // nothing after the last comma
});

CB_TEST("build.flat_thunk.name_parameters", [] {
    CB_CHECK_EQ(ft::name_parameters(""), "void");
    CB_CHECK_EQ(ft::name_parameters("void"), "void");
    CB_CHECK_EQ(ft::name_parameters("VOID"), "void");
    CB_CHECK_EQ(ft::name_parameters("HWND, BOOL"), "HWND a1, BOOL a2");
    CB_CHECK_EQ(ft::name_parameters("HWND hwnd, BOOL fAccept"), "HWND hwnd, BOOL fAccept");
    CB_CHECK_EQ(ft::name_parameters("LPCSTR, UINT FAR *puMxId"), "LPCSTR a1, UINT FAR *puMxId");
    // (a single token is a bare type, whatever it holds: `int*pn` is not told apart from a type)
    CB_CHECK_EQ(ft::name_parameters("int*pn, int"), "int*pn a1, int a2");
    CB_CHECK_EQ(ft::name_parameters("unsigned long"), "unsigned long a1");        // keywords only: no name
    CB_CHECK_EQ(ft::name_parameters("int x, int Y, int WH"), "int x, int Y, int WH a3");   // a single capital is a name; WH is a type
    CB_CHECK_EQ(ft::name_parameters("HANDLE, ..."), "HANDLE a1, ...");
    CB_CHECK_EQ(ft::name_parameters("const char *"), "const char * a1");            // ends in a pointer: unnamed
    CB_CHECK_EQ(ft::name_parameters("DWORD dwFlags, LPVOID *"), "DWORD dwFlags, LPVOID * a2");
});

CB_TEST("build.flat_thunk.needed_includes", [] {
    const ft::SdkPrototype a{"LONG", "void", "WINAPI", "MSACM.H"};
    const ft::SdkPrototype b{"BOOL", "void", "WINAPI", "WINUSER.H"};   // <windows.h> has it
    const ft::SdkPrototype c{"BOOL", "void", "WINAPI", "VFW.H"};
    const ft::SdkPrototype d{"BOOL", "void", "WINAPI", "COMMCTRL.H"};
    const ft::SdkPrototype e{"BOOL", "void", "WINAPI", "PRSHT.H"};
    const ft::SdkPrototype none{"BOOL", "void", "WINAPI", ""};
    CB_CHECK(ft::needed_includes({}).empty());
    CB_CHECK(ft::needed_includes({&b, &none, nullptr}).empty());
    using V = std::vector<std::string>;
    // prerequisites first, each header once
    CB_CHECK(ft::needed_includes({&a}) == (V{"mmsystem.h", "mmreg.h", "msacm.h"}));
    CB_CHECK(ft::needed_includes({&a, &c, &a}) == (V{"mmsystem.h", "mmreg.h", "msacm.h", "commdlg.h", "vfw.h"}));
    CB_CHECK(ft::needed_includes({&d}) == (V{"objbase.h", "commctrl.h"}));
    CB_CHECK(ft::needed_includes({&e, &d}) == (V{"objbase.h", "commctrl.h", "prsht.h"}));
});

CB_TEST("build.flat_thunk.thunk_target", [] {
    // from the strings of the image: the 32-bit name with "32" dropped when the image has it
    CB_CHECK_EQ(ft::thunk_target(ft::PeModule::open(golden_file("dciman32.dll")), "dciman32").dll16.value_or("?"), "DCIMAN.DLL");
    const auto t = ft::thunk_target(ft::PeModule::open(golden_file("dciman32.dll")), "dciman32");
    CB_CHECK(std::find(t.candidates.begin(), t.candidates.end(), "KERNEL32.DLL") != t.candidates.end());
    CB_CHECK(t.candidates.front() == "DCIMAN32.DLL");
    // a module whose partner is not in the image has the lone non-system .DLL string, or none
    CB_CHECK(ft::thunk_target(ft::PeModule::open(golden_file("dciman32.dll")), "nosuchmodule").dll16.has_value() == false ||
             ft::thunk_target(ft::PeModule::open(golden_file("dciman32.dll")), "nosuchmodule").dll16.value() != "NOSUCHMODULE.DLL");
});

CB_TEST("build.flat_thunk.kernel32_ordinals", [] {
    const auto* a = ft::kernel32_ordinal(21);
    CB_CHECK(a != nullptr);
    CB_CHECK_EQ(a->name, "Win32HandleToDosFileHandle");
    CB_CHECK_EQ(a->argument_bytes.value_or(99), 4u);
    CB_CHECK(!ft::kernel32_ordinal(41)->argument_bytes.has_value());
    CB_CHECK_EQ(ft::kernel32_ordinal(17)->argument_bytes.value_or(99), 0u);
    CB_CHECK(ft::kernel32_ordinal(1) == nullptr);
});

CB_TEST("build.flat_thunk.the_functions_a_veneer_defines", [] {
    const std::string c =
        "#include <windows.h>\n"
        "BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID res)\n{\n  return TRUE;\n}\n"
        "/* a comment: int WINAPI NotAFunction(void) { */\n"
        "int WINAPI LZInit(int hfSource) { return 0; }\n"
        "DWORD __cdecl vfunc(const char *fmt, ...) { return 0; }\n"
        "unsigned long WINAPI Wide(HWND,\n   BOOL) { return 0; }\n"
        "static int helper(int x) { return x; }\n"
        "extern BOOL __stdcall lz_ThunkConnect32(LPSTR,LPSTR,HINSTANCE,DWORD);\n";
    const auto f = ft::veneer_functions(c);
    CB_CHECK_EQ(f.size(), std::size_t{5});
    CB_CHECK_EQ(f[0].name, "DllMain");
    CB_CHECK_EQ(f[0].convention, "WINAPI");
    CB_CHECK_EQ(f[1].name, "LZInit");   // (a comment line does not start with a letter: not a definition)
    CB_CHECK_EQ(f[1].parameters, "int hfSource");
    CB_CHECK_EQ(f[2].name, "vfunc");
    CB_CHECK_EQ(f[2].convention, "__cdecl");
    CB_CHECK_EQ(f[2].parameters, "const char *fmt, ...");
    CB_CHECK_EQ(f[3].name, "Wide");
    CB_CHECK_EQ(f[3].parameters, "HWND,\n   BOOL");
    CB_CHECK_EQ(f[4].name, "helper");   // `static int helper(`: the pattern reads <type> <word> <name>; a storage class counts as the type
});

// ---- discover and toolchain -----------------------------------------------------------------------------------------

CB_TEST("build.flat_thunk.discover_the_pieces_of_a_source_folder", [] {
    fs::ScopedTempDir dir("cbft");
    const auto put = [&](const char* name, std::string_view text) { fs::write_text(dir.path() / name, text); };
    const auto missing = [&] {
        const auto r = ft::discover(dir.path());
        CB_CHECK(!r.ok());
        return std::string(r.ok() ? "" : r.error().what());
    };
    CB_CHECK(missing().starts_with("no .thk in "));
    put("lz.thk", "enablemapdirect3216 = true;");
    CB_CHECK(missing().starts_with("no module .def in "));
    put("lzk32.def", "LIBRARY KERNEL32");
    CB_CHECK(missing().starts_with("no module .def in "));   // the k32 .def is not the module's
    put("lz32.def", "; comment\n  library   LZ32\nEXPORTS\n    LZCopy @1\n");
    CB_CHECK(missing().starts_with("no veneer .c in "));
    put("lz32.c", "int x;");
    auto r = ft::discover(dir.path());
    CB_CHECK(r.ok());
    CB_CHECK_EQ(r.value().module, "lz32");   // from the LIBRARY line, lower-cased
    CB_CHECK_EQ(fs::to_text(r.value().thk.filename()), "lz.thk");
    CB_CHECK_EQ(fs::to_text(r.value().moddef.filename()), "lz32.def");
    CB_CHECK_EQ(fs::to_text(r.value().k32def.filename()), "lzk32.def");
    CB_CHECK_EQ(r.value().cfiles.size(), std::size_t{1});
    CB_CHECK(!r.value().rc.has_value());
    put("lz32.rc", "");
    put("A_first.c", "int y;");
    r = ft::discover(dir.path());
    CB_CHECK(r.value().rc.has_value());
    CB_CHECK_EQ(fs::to_text(r.value().cfiles.front().filename()), "A_first.c");   // by name, whatever the case
    // no LIBRARY line: the .def's own name
    put("lz32.def", "EXPORTS\n    LZCopy @1\n");
    CB_CHECK_EQ(ft::discover(dir.path()).value().module, "lz32");
    put("lz32.def", "LIBRARYX FOO\nEXPORTS\n");
    CB_CHECK_EQ(ft::discover(dir.path()).value().module, "lz32");
});

CB_TEST("build.flat_thunk.toolchain_is_found_where_the_reference_folder_and_the_setting_say", [] {
    fs::ScopedTempDir dir("cbft");
    const stdfs::path ref = dir.path() / "reference";
    const stdfs::path ref2 = dir.path() / "reference2";
    fs::write_text(ref / "WIN32SDK" / "MSTOOLS" / "BIN" / "I386" / "THUNK.EXE", "sdk thunk");
    fs::write_text(ref / "WIN32SDK" / "MSTOOLS" / "LIB" / "I386" / "THUNK32.LIB", "lib");
    fs::write_text(ref / "Microsoft Windows 95 DDK" / "MASM611C" / "ML.EXE", "ml");
    ft::Locations where;
    where.reference_roots = {dir.path() / "none", ref, ref2};
    where.period_compiler = dir.path() / "VC20" / "BIN";
    auto tc = ft::toolchain(where);
    CB_CHECK(fs::same_path(tc.thunk, ref / "WIN32SDK" / "MSTOOLS" / "BIN" / "I386" / "THUNK.EXE"));
    CB_CHECK(fs::same_path(tc.ml, ref / "Microsoft Windows 95 DDK" / "MASM611C" / "ML.EXE"));
    CB_CHECK(fs::same_path(tc.cl, dir.path() / "VC20" / "BIN" / "CL.EXE"));
    CB_CHECK(fs::same_path(tc.include, dir.path() / "VC20" / "INCLUDE"));
    CB_CHECK_EQ(tc.lib_dirs.size(), std::size_t{2});
    CB_CHECK(fs::same_path(tc.lib_dirs[0], dir.path() / "VC20" / "LIB"));
    CB_CHECK(fs::same_path(tc.lib_dirs[1], ref / "WIN32SDK" / "MSTOOLS" / "LIB" / "I386"));
    // the v1.01 thunk compiler of the Chicago beta SDK wins when it is there; the setting may name the root and may end in a slash
    fs::write_text(ref2 / "thunk-1.01" / "THUNK.EXE", "1.01");
    where.period_compiler = dir.path() / "VC20" / "";
    tc = ft::toolchain(where);
    CB_CHECK(fs::same_path(tc.thunk, ref2 / "thunk-1.01" / "THUNK.EXE"));
    CB_CHECK(fs::same_path(tc.link, dir.path() / "VC20" / "BIN" / "LINK.EXE"));
    where.period_compiler = dir.path() / "VC20" / "bin" / "";
    CB_CHECK(fs::same_path(ft::toolchain(where).rc, dir.path() / "VC20" / "BIN" / "RC.EXE"));
    // nothing there: no thunk, no ml
    ft::Locations nothing;
    tc = ft::toolchain(nothing);
    CB_CHECK(tc.thunk.empty() && tc.ml.empty());
    CB_CHECK_EQ(tc.lib_dirs.size(), std::size_t{1});
});

CB_TEST("build.flat_thunk.a_build_without_its_tools_or_sources_fails_with_a_reason", [] {
    fs::ScopedTempDir dir("cbft");
    ft::Locations where;
    where.period_compiler = dir.path() / "VC20" / "BIN";
    auto r = ft::build_period(where, dir.path());
    CB_CHECK(!r.ok);
    CB_CHECK(r.error.starts_with("missing tool: thunk ("));
    // with every tool present but no source
    for (const char* tool : {"CL.EXE", "LINK.EXE", "LIB.EXE"}) fs::write_text(dir.path() / "VC20" / "BIN" / tool, "x");
    const stdfs::path ref = dir.path() / "ref";
    fs::write_text(ref / "WIN32SDK" / "MSTOOLS" / "BIN" / "I386" / "THUNK.EXE", "x");
    fs::write_text(ref / "Microsoft Windows 95 DDK" / "MASM611C" / "ML.EXE", "x");
    where.reference_roots = {ref};
    fs::make_dirs(dir.path() / "src");
    r = ft::build_period(where, dir.path() / "src");
    CB_CHECK(!r.ok);
    CB_CHECK(r.error.starts_with("no .thk in "));
    // OpenWatcom that is not built
    const auto ow = watcom::OpenWatcom(dir.path() / "no-watcom");
    r = ft::build_openwatcom(ow, dir.path() / "src");
    CB_CHECK(!r.ok);
    CB_CHECK_EQ(r.error, "OpenWatcom not built");
    const auto c = ft::compile_openwatcom(ow, dir.path() / "src");
    CB_CHECK(!c.ok);
    CB_CHECK_EQ(c.error, "OpenWatcom not built");
});

// ---- recover --------------------------------------------------------------------------------------------------------

CB_TEST("build.flat_thunk.recovered_source_is_what_the_python_wrote", [] {
    // the files the Python's recover_flat_thunk made for the same modules and the same small SDK table (golden data: made by
    // the Python, read through for the cases below)
    const auto sdk = small_sdk();
    CB_CHECK_EQ(sdk.size(), std::size_t{8});
    for (const char* name : {"version", "dciman32", "tapi32", "mpr"}) {
        fs::ScopedTempDir out("cbft");
        const std::string module = name;
        const ft::Recovery r = ft::recover(golden_file(module + ".dll"), out.path() / module, sdk);
        for (const std::string ext : {".def", "k32.def", ".thk", ".c"}) {
            const std::string made = fs::read_text(out.path() / module / (module + ext));
            const std::string wanted = fs::read_text(golden_file(module + "/" + module + ext));
            CB_CHECK(made == wanted);
        }
        // and what it says about them
        std::map<std::string, std::string> expected;
        const std::string summary = fs::read_text(golden_file(module + "/summary.txt"));
        for (const std::string_view line : text::lines(summary)) {
            const std::size_t eq = line.find('=');
            if (eq != std::string_view::npos) expected[std::string(line.substr(0, eq))] = std::string(line.substr(eq + 1));
        }
        CB_CHECK_EQ(r.module, expected["module"]);
        CB_CHECK_EQ(std::to_string(r.exports), expected["n_exports"]);
        CB_CHECK_EQ(std::to_string(r.signatures), expected["n_signatures_recovered"]);
        CB_CHECK_EQ(std::to_string(r.pointer_args), expected["n_pointer_args"]);
        CB_CHECK_EQ(std::to_string(r.sdk_prototypes), expected["n_sdk_prototypes"]);
        CB_CHECK_EQ(r.target16.value_or("None"), expected["target16"]);
        CB_CHECK_EQ(r.scaffolded.size(), std::size_t{2});
    }
});

CB_TEST("build.flat_thunk.recover_says_what_it_found", [] {
    fs::ScopedTempDir out("cbft");
    const ft::Recovery r = ft::recover(formats_data("lz32.dll"), out.path(), {});
    CB_CHECK_EQ(r.module, "lz32");
    CB_CHECK_EQ(r.exports, std::size_t{12});
    CB_CHECK_EQ(r.target16.value_or("?"), "SYSTHUNK.DLL");
    CB_CHECK(r.kernel32_ordinals == (std::vector<std::uint32_t>{17, 20, 21, 22, 41, 89, 90}));
    CB_CHECK(r.unknown_ordinals.empty());
    CB_CHECK_EQ(r.sdk_prototypes, std::size_t{0});
    CB_CHECK(stdfs::exists(out.path() / "lz32.def") && stdfs::exists(out.path() / "lz32k32.def"));
    CB_CHECK(stdfs::exists(out.path() / "lz32.thk") && stdfs::exists(out.path() / "lz32.c"));
    const std::string def = lf(fs::read_text(out.path() / "lz32.def"));
    CB_CHECK(def.starts_with("LIBRARY   LZ32\nEXPORTS\n    LZCopy             @1\n"));
    const std::string c = lf(fs::read_text(out.path() / "lz32.c"));
    CB_CHECK(c.find("lz32_ThunkConnect32(\"SYSTHUNK.DLL\", \"LZ32.DLL\", h, r)") != std::string::npos);
    CB_CHECK(c.find("#undef LZCopy\n") != std::string::npos);
    // the files are Windows text
    CB_CHECK(fs::read_text(out.path() / "lz32.def").find("\r\n") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, ft::recover(formats_data("lzexpand.dll"), out.path() / "ne", {}));
    CB_CHECK_THROWS_AS(UsageError, ft::recover(formats_data("no_such_file.dll"), out.path() / "none", {}));
});

CB_TEST("build.flat_thunk.recover_uses_a_sdk_prototype_where_there_is_one", [] {
    fs::ScopedTempDir out("cbft");
    std::map<std::string, ft::SdkPrototype> sdk;
    sdk["LZCopy"] = {"LONG", "INT, INT", "WINAPI", "LZEXPAND.H"};
    sdk["LZClose"] = {"void", "INT hf", "WINAPI", "WINBASE.H"};
    const ft::Recovery r = ft::recover(formats_data("lz32.dll"), out.path(), sdk);
    CB_CHECK_EQ(r.sdk_prototypes, std::size_t{2});
    const std::string c = lf(fs::read_text(out.path() / "lz32.c"));
    CB_CHECK(c.find("#include <lzexpand.h>\n") != std::string::npos);
    CB_CHECK(c.find("#include <winbase.h>") == std::string::npos);   // <windows.h> has it
    CB_CHECK(c.find("LONG WINAPI LZCopy(INT a1, INT a2) { return 0; }\n") != std::string::npos);   // unnamed: named for a definition
    CB_CHECK(c.find("void WINAPI LZClose(INT hf) { }\n") != std::string::npos);                    // void: no return value
    const std::string thk = lf(fs::read_text(out.path() / "lz32.thk"));
    CB_CHECK(thk.find("LONG LZCopy16(INT, INT)\n{\n}\n") != std::string::npos);                   // (the .thk keeps the prototype as it is)
});

// ---- the command ----------------------------------------------------------------------------------------------------

CB_TEST("build.flat_thunk.the_command_classifies_recovers_and_diffs", [] {
    fs::ScopedTempDir out("cbft");
    ft::Services services;
    services.retail = [](std::string_view name) -> std::optional<stdfs::path> {
        if (name == "lz32.dll") return formats_data("lz32.dll");
        return std::nullopt;
    };
    services.sdk = [] { return small_sdk(); };
    const cli::Command command = ft::command(services);
    CB_CHECK_EQ(command.name, "flat-thunk");
    const auto run = [&](std::vector<std::string> args) {
        cli::Cli app("codebreaker", {command});
        args.insert(args.begin(), "flat-thunk");
        return app.run(args);
    };
    {
        testing::CapturedStd captured;
        CB_CHECK_EQ(run({"lz32.dll"}), kExitOk);
        const std::string text = captured.out();
        CB_CHECK(text.starts_with("=== classify lz32.dll ===\n"));
        CB_CHECK(text.find("  subtype      flat-thunk\n") != std::string::npos);
        CB_CHECK(text.find("  n_exports    12\n") != std::string::npos);
    }
    {
        testing::CapturedStd captured;
        const std::string dir = fs::to_text(out.path() / "rec");
        CB_CHECK_EQ(run({fs::to_text(formats_data("lz32.dll")), "--recover", dir, "--diff", fs::to_text(formats_data("lz32.dll"))}), kExitOk);
        const std::string text = captured.out();
        CB_CHECK(text.find("=== recover (flat-thunk) -> " + dir + " ===\n") != std::string::npos);
        CB_CHECK(text.find("  16-bit target: SYSTHUNK.DLL\n") != std::string::npos);
        CB_CHECK(text.find("  scaffolded (need curation): lz32.thk, lz32.c\n") != std::string::npos);
        CB_CHECK(text.find("=== diff lz32.dll vs lz32.dll ===\n") != std::string::npos);
        CB_CHECK(text.find("verdict=byte-surface-identical") != std::string::npos);
        CB_CHECK(stdfs::exists(out.path() / "rec" / "lz32.c"));
    }
    // a build that cannot run exits 1 and says why; a module that is nowhere is an error
    {
        testing::CapturedStd captured;
        CB_CHECK_EQ(run({fs::to_text(formats_data("lz32.dll")), "--ow-build", fs::to_text(out.path() / "rec")}), kExitFailed);
        CB_CHECK(captured.out().find("  BUILD FAILED: OpenWatcom not built\n") != std::string::npos);
    }
    CB_CHECK_THROWS_AS(NotFoundError, run({"nosuchmodule"}));
});

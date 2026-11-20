// Tests of cb/decompile/c_text.hpp. The module's own work -- the decisions it makes over a whole module -- is tested with stand-ins for what
// it asks of the rest (the decompiler, the symbol tables, the L2 library) that answer what the original was given: a 32-bit module with
// its globals, structure, imports and a function that takes its argument in ECX; a PowerPC image; and a real 16-bit module (LZEXPAND.DLL,
// described by the real describer) made compilable with every function "decompiled" by a stand-in.
//
// Notes (an old harness compared the whole text of 19 modules, which needs the native decompiler):
//  - the environment switches CB_SELECTOR_STRUCTS, CB_CODE_DATA and CB_PROTOTYPES are the fields of Policy;
//  - compilable16's result is not kept in the build cache, and `generation()` is the decompiler's own (nothing here is cached);
//  - a function whose own selector structure has overlapping fields is left without a declaration;
//  - a flag the back end reports that the module has no place for is an InternalError;
//  - sizes, addresses and offsets are fixed-width integers, and an address past what the 16-bit layout holds is not carried as a bignum.
#include "decompile/c_text.hpp"

#include <cstdlib>
#include <cstring>
#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "translate/l2.hpp"

namespace ct = cb::decompile::c_text;
namespace dsc = cb::decompile::describe;
namespace nl = cb::translate::native_l2;
using cb::Json;
using std::string;

namespace {

bool has(const string& text, const string& part) { return text.find(part) != string::npos; }

// ---- a stand-in for the L2 core library --------------------------------------------------------------------------------------

// What the library says of the function documents of these tests: the names each uses, what it calls ...
Json library_answer(const string& name, const Json& args) {
    if (name == "module_names32") {
        const string function = args.at(0).value("name", "");
        if (function == "ppc_main") return Json{{"names", Json::array()}, {"calls", Json::array({"import1_ord_5"})}, {"called_through", Json::array()},
                                                {"arity", Json::array({Json::array({"import1_ord_5", 2})})}};
        if (function == "sub_401000")
            return Json{{"names", Json::array({"uRam00403000", "uRam00403004", "_uRam00403004", "pcRam00403010", "cRam00403020"})},
                        {"calls", Json::array({"MessageBoxA", "sub_401000"})}, {"called_through", Json::array({"cRam00403020"})}, {"arity", Json::array()}};
        return Json{{"names", Json::array({"uRam00403000", "func_0x00010100"})}, {"calls", Json::array()}, {"called_through", Json::array()}, {"arity", Json::array()}};
    }
    if (name == "signature") return "int4 __stdcall f(void)";
    if (name == "takes32") return nullptr;
    if (name == "type_names") return Json::array({"unkint5", "uint6", "int4"});
    if (name == "parameter_names" || name == "struct_accesses") return Json::array();
    if (name == "flags") return args.at(1).contains("near") ? Json::array({"near"}) : Json::array();
    if (name == "caller_frame_plan" || name == "segment_plan") return Json::object();
    if (name == "prototypes") {
        Json out = Json::object();
        for (const Json& n : args.at(1)) out[n.get<string>()] = n.get<string>() + "(void);";
        return out;
    }
    if (name == "seg_layout") {
        Json fields = Json::array();
        for (const Json& kv : args.at(0).at("$d")) {
            if (kv.at(0).get<int>() < 0) return nullptr;   // (overlapping: no layout)
            fields.push_back(kv.at(1).get<string>() + " f_" + std::to_string(kv.at(0).get<int>()) + ";");
        }
        return fields;
    }
    return nullptr;
}

int fake_call(const char* name, const char* arguments, std::size_t length, char** out, std::size_t* out_length) {
    const Json args = Json::parse(string(arguments, length));
    const string answer = library_answer(name, args).dump();
    *out = static_cast<char*>(std::malloc(answer.size() + 1));
    std::memcpy(*out, answer.c_str(), answer.size() + 1);
    *out_length = answer.size();
    return 0;
}
void fake_free(char* text) { std::free(text); }

// ---- a stand-in for the decompiler and the symbol tables ----------------------------------------------------------------------

class Stand : public ct::Services {
public:
    Stand() : library_(&fake_call, &fake_free) {}

    ct::Decompiled decompiled;
    ct::Decompiled16 decompiled16;
    std::map<string, string> dlls;
    std::map<string, std::uint32_t> argument_sizes;
    dsc::PowerPcFacts ppc;
    std::map<string, ct::ModuleFacts> seen;   // what each function was written with
    std::map<string, string> headings;
    std::map<string, ct::NamesUsed> names16;
    bool all_far = true;

    ct::Decompiled raw(const std::filesystem::path&, std::span<const ct::Va>, const std::filesystem::path&) override { return decompiled; }
    ct::Decompiled16 decompile16(const std::filesystem::path&, const std::optional<std::filesystem::path>&) override { return decompiled16; }
    std::optional<string> c_from_l2(const std::optional<string>& text, ct::FunctionNotes& notes, const std::optional<ct::SegmentImages>&,
                                    ct::ModuleFacts* module) override {
        notes.from_l2 = true;
        const string name = cb::translate::l2::to_json(*notes.l2).value("name", "");
        seen[name] = *module;
        if (module->path == ct::ModuleFacts::Path::Win16) {
            ct::Heading h;
            h.declaration = headings.count(name) ? headings[name] : "int2 " + name + "();";
            h.params = ct::Heading::Params{"int2 " + name, {}};
            module->heading = h;
            if (names16.count(name)) module->names = names16[name];
            module->left = "{}";
            return "  int2 " + name + "() { return 0; }  ";
        }
        module->declared = headings.count(name) ? headings[name] : "int4 __stdcall " + name + "(int4 a1)";
        if (name == "sub_401000") module->registers = "ecx";
        if (name == "ppc_main") module->novalue = true;
        return "\n" + (text ? *text : string()) + "\n";
    }
    bool recovers_structures(const dsc::NeDescription&) override { return true; }
    bool tree_images() override { return false; }
    ct::SegmentImages segment_images(const dsc::NeDescription&) override { return {}; }
    const nl::Library& library() override { return library_; }
    std::map<string, string> import_dlls(const std::filesystem::path&) override { return dlls; }
    std::optional<std::uint32_t> argument_bytes(std::string_view, std::string_view name) override {
        const auto it = argument_sizes.find(string(name));
        return it == argument_sizes.end() ? std::nullopt : std::optional<std::uint32_t>(it->second);
    }
    std::optional<std::vector<cb::decompile::struct_text::Structure>> recover_structures(const std::filesystem::path&) override { return std::nullopt; }
    dsc::PowerPcFacts powerpc_facts(const std::filesystem::path&) override { return ppc; }

private:
    nl::Library library_;
};

// A function's L2 document: a MethodDecl of that name (what the stand-in library and decompiler read the name of).
cb::translate::ir::Node* document(cb::Arena& arena, const string& name) { return cb::translate::l2::function_node(arena, {.name = name, .text = "int f(){}"}); }

ct::FunctionNotes notes_of(cb::Arena& arena, const string& name) {
    ct::FunctionNotes n;
    n.l2 = document(arena, name);
    return n;
}

}  // namespace

// ---- the interrupt helpers ------------------------------------------------------------------------------------------------------

CB_TEST("decompile.c_text.interrupt_helper_name", [] {
    const std::vector<string> regs = {"ax", "dx"};
    CB_CHECK_EQ(ct::interrupt_helper_name(0x21, regs), "int21_ax_dx");
    CB_CHECK_EQ(ct::interrupt_helper_name(0x10, {}), "int10_none");
});

CB_TEST("decompile.c_text.interrupt_helpers", [] {
    CB_CHECK(ct::interrupt_helpers({}).empty());
    const ct::Interrupts used = {{0x21, {"ax", "dx"}}, {0x10, {}}};
    const std::vector<string> lines = ct::interrupt_helpers(used);
    const string all = cb::text::join(lines, "\n");
    CB_CHECK(has(all, "struct codebreaker_registers { uint2 ax, bx, cx, dx, si, di, ds, es; uint1 cf, zf; uint2 set; };"));
    CB_CHECK(has(all, "#define int_AX codebreaker_int.ax"));
    CB_CHECK(has(all, "#define int_ZF codebreaker_int.zf"));
    CB_CHECK(has(all, "#define int_BH (*((uint1 *)&codebreaker_int.bx + 1))"));
    CB_CHECK(has(all, "void __far codebreaker_int21(void);              /* INT 21h */"));
    // the registers it sets are the bits of `set`, in the record's order: ax is bit 0, dx bit 3
    CB_CHECK(has(all, "static uint4 int21_ax_dx(uint2 ax, uint2 dx) { codebreaker_int.ax = ax; codebreaker_int.dx = dx; codebreaker_int.set = 0x09; "
                      "codebreaker_int21(); return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }"));
    CB_CHECK(has(all, "static uint4 int10_none(void) {  codebreaker_int.set = 0x00; codebreaker_int10(); return"));
    // sorted: INT 10h before INT 21h
    CB_CHECK(all.find("codebreaker_int10(void)") < all.find("codebreaker_int21(void)"));
});

CB_TEST("decompile.c_text.interrupt_routines", [] {
    const ct::InterruptRoutines r = ct::interrupt_routines({0x21});
    CB_CHECK(has(r.assembly, "_codebreaker_int21 proc far"));
    CB_CHECK(has(r.assembly, "        int  021h"));
    CB_CHECK(has(r.assembly, "        mov  bx, word ptr _codebreaker_int+2"));
    CB_CHECK(has(r.assembly, "L_21_7:"));
    CB_CHECK(r.assembly.ends_with("_TEXT ends\nend\n"));
    CB_CHECK_EQ(r.c, "#include \"m.h\"\nstruct codebreaker_registers codebreaker_int;\n");
});

// ---- strings and selectors ----------------------------------------------------------------------------------------------------

CB_TEST("decompile.c_text.string_literal", [] {
    const cb::Bytes hello = {'H', 'i', '\n', '"', '?', 0};
    CB_CHECK_EQ(ct::string_literal(hello).value_or("none"), "\"Hi\\n\\\"\\?\"");
    CB_CHECK(!ct::string_literal(cb::Bytes{'a'}));                      // too short
    CB_CHECK(!ct::string_literal(cb::Bytes{'a', 'b'}));                 // not terminated
    CB_CHECK(!ct::string_literal(cb::Bytes{1, 2, 3, 'a', 0}));          // mostly not text
    CB_CHECK(!ct::string_literal(cb::Bytes{'a', 'b', 'c', 0x90, 0}));   // a byte that is not text at all
    CB_CHECK_EQ(ct::string_literal(cb::Bytes{'a', 0, 'b', 0}).value_or("none"), "\"a\\000b\"");
});

CB_TEST("decompile.c_text.shared_selector_structs", [] {
    Stand stand;
    const std::map<string, std::map<string, std::map<std::int64_t, string>>> shared = {
        {"seg_1", {{"f", {{0, "uint2"}}}, {"g", {{2, "uint2"}}}}},      // agree: one for the module
        {"seg_2", {{"f", {{0, "uint2"}}}, {"g", {{0, "uint1"}}}}},      // the offset has two types: each its own
    };
    const ct::SharedSelectorStructs s = ct::shared_selector_structs(stand.library(), shared);
    CB_CHECK_EQ(s.declarations.size(), std::size_t(3));
    CB_CHECK_EQ(s.declarations[0], "typedef _Packed struct seg_1 { uint2 f_0; uint2 f_2; } seg_1;");
    CB_CHECK_EQ(s.declarations[1], "typedef _Packed struct seg_f_2 { uint2 f_0; } seg_f_2;");
    CB_CHECK_EQ(s.declarations[2], "typedef _Packed struct seg_g_2 { uint1 f_0; } seg_g_2;");
    CB_CHECK_EQ(s.renames.at("f").at("seg_2"), "seg_f_2");
    CB_CHECK_EQ(s.renames.at("g").at("seg_2"), "seg_g_2");
    CB_CHECK(!s.renames.contains("f") || !s.renames.at("f").contains("seg_1"));
});

CB_TEST("decompile.c_text.module_facts16_reads_the_instructions", [] {
    // push bp / mov bp,sp / les bx,[bp+6] / mov ax,es:[bx] / mov [bp-2],ss / ret
    const cb::Bytes code = {0x55, 0x8b, 0xec, 0xc4, 0x5e, 0x06, 0x26, 0x8b, 0x07, 0x8c, 0x56, 0xfe, 0xc3};
    const ct::ModuleFacts f = ct::module_facts16(cb::ByteView(code), 0, 13, "near", std::nullopt, std::nullopt, false, 1);
    CB_CHECK(f.have_code);
    CB_CHECK_EQ(f.selector.value_or(0), 0x1000);
    CB_CHECK(f.loads.contains(6));
    CB_CHECK_EQ(f.widths_near.at("a2"), 2);   // (a near function takes its first argument at bp+4: the far pointer at bp+6 is its second, read as a word)
    CB_CHECK_EQ(f.widths_far.at("a1"), 2);    // (a far function takes its first at bp+6)
    CB_CHECK(f.ss_stores == std::vector<std::int64_t>{4});   // `mov [bp-2], ss`: offset 2 + 2
    CB_CHECK(!f.exported);
    const cb::Bytes exported = {0x8c, 0xd8, 0x90, 0x55};
    CB_CHECK(ct::module_facts16(cb::ByteView(exported), 0, 4, "far", std::nullopt, std::nullopt, false).exported);
    CB_CHECK(!ct::module_facts16(std::nullopt, 0, 4, "far", std::nullopt, std::nullopt, false).have_code);
});

CB_TEST("decompile.c_text.ss_store_places", [] {
    const cb::Bytes code = {0x8c, 0x56, 0xfc, 0x8c, 0x56, 0xfe};   // mov [bp-4], ss ; mov [bp-2], ss
    CB_CHECK(ct::ss_store_places(code, 0, 6) == (std::vector<std::int64_t>{6, 4}));
    CB_CHECK(ct::ss_store_places(code, 0, 0).empty());
});

CB_TEST("decompile.c_text.call_counts", [] {
    const std::map<string, std::set<std::int64_t>> calls = {{"a", {2}}, {"b", {1, 2}}, {"c", {3}}};
    const auto counts = ct::call_counts(calls, {"a", "b"});
    CB_CHECK_EQ(counts.size(), std::size_t(1));   // b is called with two different counts; c is not asked for
    CB_CHECK_EQ(counts.at("a"), 2);
});

CB_TEST("decompile.c_text.constants", [] {
    CB_CHECK_EQ(ct::cflags().size(), std::size_t(6));
    CB_CHECK(ct::header16().starts_with("typedef long long int8;"));
    CB_CHECK(ct::seg_ptr().starts_with("#define SEG_PTR(T, s)"));
    CB_CHECK_EQ(ct::interrupt_registers().size(), std::size_t(8));
    CB_CHECK_EQ(ct::intrinsics16().size(), std::size_t(6));
    CB_CHECK(has(ct::intrinsics16().at("in"), "0xEC"));
});

// ---- a 32-bit module ------------------------------------------------------------------------------------------------------------

CB_TEST("decompile.c_text.compilable32", [] {
    Stand stand;
    auto arena = std::make_shared<cb::Arena>();
    stand.decompiled.arena = arena;
    stand.decompiled.order = {"sub_401000", "sub_401100"};
    stand.decompiled.texts.insert_or_assign("sub_401000", "int f(){}");
    stand.decompiled.texts.insert_or_assign("sub_401100", "int g(){}");
    stand.decompiled.by_va = {{0x401000, "sub_401000"}, {0x401100, "sub_401100"}};
    ct::FunctionNotes first = notes_of(*arena, "sub_401000");
    first.globals = {{0x403000, 4, 0}, {0x403004, 4, 0}, {0x403010, 4, 0}, {0x403020, 4, 0}};
    first.structs = {{"s_f", 8, {{0, 4, "int4"}, {6, 2, "weird"}}}};
    ct::FunctionNotes second = notes_of(*arena, "sub_401100");
    second.globals = {{0x403000, 1, 0}};   // a function that uses the word at its own width
    stand.decompiled.notes["sub_401000"] = first;
    stand.decompiled.notes["sub_401100"] = second;
    stand.decompiled.numbered["Ordinal_7"] = {"MFC42", 7, std::nullopt};
    stand.dlls = {{"MessageBoxA", "USER32"}};
    stand.argument_sizes = {{"MessageBoxA", 16}};

    const ct::Compilable32 m = ct::compilable32(stand, "demo.dll", {}, "out");

    CB_CHECK_EQ(m.order.size(), std::size_t(2));
    CB_CHECK_EQ(m.texts.at("sub_401000"), "int f(){}");   // the back end's text, with the blank lines taken off
    CB_CHECK(m.preamble.starts_with("#pragma pack(1)"));
    // the globals as macros at their addresses, by the width most of the uses give; a pointer to char, a code pointer, the core's other name
    CB_CHECK(has(m.preamble, "#define uRam00403000 (*(uint4 *)0x00403000)"));
    CB_CHECK(has(m.preamble, "#define uRam00403004 (*(uint4 *)0x00403004)"));
    CB_CHECK(has(m.preamble, "#define _uRam00403004 (*(uint4 *)0x00403004)"));
    CB_CHECK(has(m.preamble, "#define pcRam00403010 (*(char * *)0x00403010)"));
    CB_CHECK(has(m.preamble, "#define cRam00403020 (*(code * *)0x00403020)"));   // (called through: it holds a function pointer)
    // sub_401100 reads the word as a byte: its own width, by an alias defined once
    CB_CHECK(has(m.preamble, "#define uRam00403000_1 (*(uint1 *)0x00403000)"));
    CB_CHECK(stand.seen.at("sub_401100").aliases.at("uRam00403000") == "uRam00403000_1");
    CB_CHECK(stand.seen.at("sub_401000").aliases.empty());
    CB_CHECK(stand.seen.at("sub_401000").shared->pointer_globals.contains("pcRam00403010"));
    CB_CHECK(stand.seen.at("sub_401000").shared->pointer_globals.contains("_uRam00403004") == false);
    CB_CHECK(stand.seen.at("sub_401000").shared->called_through.contains("cRam00403020"));
    CB_CHECK(stand.seen.at("sub_401000").without_notes);
    // the declarations: the import (its argument bytes from the table), each function, and the ECX pragma
    CB_CHECK(has(m.preamble, "int __stdcall MessageBoxA();"));
    CB_CHECK(has(m.preamble, "int4 __stdcall sub_401000(int4 a1);"));
    CB_CHECK(has(m.preamble, "#pragma aux sub_401000 \"_*\" parm routine [ecx] value [eax] modify [eax ecx edx];"));
    CB_CHECK(m.in_ecx.contains("sub_401000") && m.in_edx.empty());
    CB_CHECK(m.imports == std::vector<string>{"MessageBoxA"});
    // the structure, its filler, and a type the header does not know as the bytes it takes
    CB_CHECK(has(m.preamble, "typedef struct s_f s_f;"));
    CB_CHECK(has(m.preamble, "struct s_f { int4 f_0; uint1 _pad_4[2]; xunknown2 f_6; };"));
    // the core's names for values of sizes C has no type for are given a type each; one already declared is not
    CB_CHECK(has(m.preamble, "typedef long long unkint5;"));
    CB_CHECK(has(m.preamble, "typedef unsigned long long uint6;"));
    CB_CHECK(!has(m.preamble, "typedef int int4;\ntypedef int int4;"));
    CB_CHECK_EQ(m.declarations.at("sub_401000"), "int4 __stdcall sub_401000(int4 a1);");
    CB_CHECK_EQ(m.docs.size(), std::size_t(2));
    CB_CHECK_EQ(m.by_va.at(0x401100), "sub_401100");
});

CB_TEST("decompile.c_text.compilable32_a_function_the_back_end_declines", [] {
    Stand stand;
    auto arena = std::make_shared<cb::Arena>();
    stand.decompiled.arena = arena;
    stand.decompiled.order = {"lonely"};
    stand.decompiled.texts.insert_or_assign("lonely", "int f(){}");
    stand.decompiled.notes["lonely"] = ct::FunctionNotes();   // no document: the printed text stays, and there is no declaration
    const ct::Compilable32 m = ct::compilable32(stand, "demo.dll", {}, "out");
    CB_CHECK_EQ(m.texts.at("lonely"), "");
    CB_CHECK(m.declarations.empty());
    CB_CHECK(m.docs.empty());
});

CB_TEST("decompile.c_text.compilable32_crt_names_its_own_library_functions", [] {
    // a module that is itself a C runtime defines sqrt: the helpers' prototype of the name steps aside
    Stand stand;
    auto arena = std::make_shared<cb::Arena>();
    stand.decompiled.arena = arena;
    stand.decompiled.order = {"sqrt"};
    stand.decompiled.texts.insert_or_assign("sqrt", "double f(){}");
    stand.decompiled.notes["sqrt"] = notes_of(*arena, "sqrt");
    stand.headings["sqrt"] = "double __stdcall sqrt(double a1)";
    const ct::Compilable32 m = ct::compilable32(stand, "msvcrt.dll", {}, "out");
    CB_CHECK(has(m.preamble, "double __stdcall sqrt(double a1);"));
    CB_CHECK(!has(m.preamble, "double sqrt(double); "));
    CB_CHECK(has(m.preamble, "double sin(double);"));
});

// ---- a PowerPC image ---------------------------------------------------------------------------------------------------------------

CB_TEST("decompile.c_text.compilable_powerpc", [] {
    Stand stand;
    auto arena = std::make_shared<cb::Arena>();
    stand.decompiled.arena = arena;
    stand.decompiled.order = {"ppc_main", "ppc_other"};
    stand.decompiled.texts.insert_or_assign("ppc_main", "int f(){}");
    stand.decompiled.texts.insert_or_assign("ppc_other", "int g(){}");
    stand.decompiled.by_va = {{0x10000, "ppc_main"}, {0x10080, "ppc_other"}};
    ct::FunctionNotes main = notes_of(*arena, "ppc_main");
    main.globals = {{0x400000, 8, 0}};
    stand.decompiled.notes["ppc_main"] = main;
    stand.decompiled.notes["ppc_other"] = notes_of(*arena, "ppc_other");
    stand.ppc.spans = {{0x10000, 0, 0x1000}};
    stand.ppc.imports = {{0x10200, "import1_ord_5"}};
    const ct::ModulePPC m = ct::compilable_powerpc(stand, "demo/DEMO.XEX", "out");
    CB_CHECK_EQ(m.made.name(), "demo.xex");
    CB_CHECK(m.made.header().starts_with("#pragma pack(1)"));
    CB_CHECK(has(m.made.header(), "typedef uint8 code();"));   // (a code pointer is 8 bytes wide here)
    CB_CHECK(has(m.made.header(), "uint8 import1_ord_5();"));
    CB_CHECK(has(m.made.header(), "int4 __stdcall ppc_main();"));
    // `ppc_other`'s names say a place inside the image that is not a function made: it is called where it is, in the original
    CB_CHECK(has(m.made.header(), "#define func_0x00010100 ((code *)0x00010100)"));
    CB_CHECK(m.novalue.contains("ppc_main"));
    CB_CHECK_EQ(m.externs.at("import1_ord_5"), 0x10200);
    CB_CHECK_EQ(m.argc.at("import1_ord_5"), 2);
    CB_CHECK_EQ(m.made.texts().at("ppc_main"), "int f(){}");
    CB_CHECK_EQ(m.made.by_va().at(0x10080), "ppc_other");
});

// ---- a 16-bit module -----------------------------------------------------------------------------------------------------------------

CB_TEST("decompile.c_text.compilable16_on_lzexpand", [] {
    const std::filesystem::path module = cb::testing::data_path("formats/testdata/lzexpand.dll");
    cb::fs::ScopedTempDir dir;
    dsc::NoKnowledge knowledge;
    auto description = std::make_shared<dsc::NeDescription>(dsc::describe16(cb::formats::ne::Module::open_file(module), dir.path(), knowledge));
    Stand stand;
    auto arena = std::make_shared<cb::Arena>();
    stand.decompiled16.arena = arena;
    stand.decompiled16.description = description;
    std::vector<string> made_names;
    for (const auto& [va, name] : description->names) {
        if (!description->where_map.contains(va)) continue;
        // the decompiler "makes" every function but one it could not
        if (name == description->names.begin()->second) continue;
        stand.decompiled16.texts.insert_or_assign(name, "int2 " + name + "() { }");
        stand.decompiled16.order.push_back(name);
        stand.decompiled16.by_va[va] = name;
        description->notes[name] = notes_of(*arena, name);
        made_names.push_back(name);
    }
    CB_CHECK(made_names.size() > 5);
    // the first function made calls `in`, takes INT 21h and uses a global of the data segment and the BIOS data
    const string first = made_names.front();
    ct::NamesUsed used;
    used.called = {"in", "GetVersion"};
    used.globals = {{"u", "00000030"},
                    {"i", "00400010"}};
    used.mk_fp = true;
    stand.names16[first] = used;

    const ct::Module16 m = ct::compilable16(stand, module, dir.path(), {});

    CB_CHECK_EQ(m.made.order().size(), made_names.size() + 1);   // the function the decompiler could not make stays, with a body that says so
    CB_CHECK_EQ(m.failed.size(), std::size_t(1));
    CB_CHECK(m.failed.begin()->second == "the decompiler could not make it");
    const string& header = m.made.header();
    CB_CHECK(header.starts_with("typedef long long int8;"));
    for (const string& n : made_names) CB_CHECK(has(header, n + "(void);"));   // each declared with the parameters the library says every call agrees on
    CB_CHECK(has(header, "uint1 in(uint2 port);\n#pragma aux in = 0xEC parm [dx] value [al];"));   // the intrinsic it calls
    // the global in the data segment is an offset through DS, as in the original; the one through selector 0040h is the BIOS data
    CB_CHECK(has(header, "#define uRam00000030 (*(uint2 *)0x30)"));
    CB_CHECK(has(header, "#define iRam00400010 (*(int2 __far *)MK_FP(0x40, 0x10))"));
    CB_CHECK(m.bios_data);
    CB_CHECK(has(header, "#define MK_FP(s, o)"));
    CB_CHECK_EQ(m.declarations.size(), m.made.order().size());
    // a function's facts as it was written: its segment's selector, the model the description gave it
    const ct::ModuleFacts& facts = stand.seen.at(first);
    CB_CHECK(facts.path == ct::ModuleFacts::Path::Win16);
    CB_CHECK(facts.have_code);
    CB_CHECK(facts.without_notes);
    CB_CHECK(facts.selector.has_value());
    // every function of the description has its place
    for (const string& n : made_names) CB_CHECK(m.made.texts().contains(n));
    for (const auto& [va, place] : m.where) CB_CHECK_EQ(place.seg, description->where_map.at(va).seg);
    // no policy switch: the prototypes the library agreed on are not asked for
    ct::Policy off;
    off.prototypes = false;
    const ct::Module16 plain = ct::compilable16(stand, module, dir.path(), off);
    CB_CHECK(plain.prototyped.empty());
    for (const string& n : made_names) CB_CHECK(has(plain.made.header(), "int2 " + n + "();"));   // (else each by its heading)
    CB_CHECK(!m.prototyped.empty());
});

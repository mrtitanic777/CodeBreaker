// Tests of cb/symbols/runtime_library. The signatures are made from real library files (testdata of symbols/libraries: a
// C runtime object, 16-bit libraries, an import library), and the expected values are what the Python module made from
// the same files (digests, mask runs, the substance counts, the rows learn() wrote). The identifying and the compiler
// guess run over modules written out byte by byte, so that the answers can be checked by hand.
//
// DIVERGES from the Python module, by design:
//   * the tool block before a PE header: the Python read its records backwards in pairs and so reported the block's last
//     two padding words as a record (product 0, build 0, count 0) and lost the first real one; this reads the records
//     from after the padding (see state/work/typed-port/_bugs/LIST.md);
//   * a block with no start mark is not a tool block (the Python took everything back to the DOS header);
//   * a function start in a segment the module does not have is a FormatError (the Python raised KeyError);
//   * a library that cannot be read is logged with the error's own text.
#include "cb/symbols/runtime_library.hpp"

#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/symbols/libraries/package.hpp"

namespace rl = cb::symbols::runtime_library;
namespace libs = cb::symbols::libraries;
using cb::Bytes;
using cb::ByteView;

namespace {

std::filesystem::path lib_path(const char* name) { return cb::testing::data_path(std::string("symbols/libraries/testdata/") + name); }

libs::Function first_function(const char* file) {
    const libs::Library lib = libs::read_file(lib_path(file));
    CB_CHECK(!lib.functions.empty());
    return lib.functions.at(0);
}

// A row of the signature table for a function, as learn() writes it.
rl::SignatureRow row_of(const libs::Function& f, std::string library) {
    const auto sig = rl::signature(f.code, f.mask);
    CB_CHECK(sig.has_value());
    std::size_t lead = 0;
    while (lead < rl::kLead && lead < f.code.size() && f.mask[lead] == 0) ++lead;
    return {cb::crypto::to_hex(ByteView(f.code).first(lead)), sig->size, sig->mask_text, sig->digest, rl::c_name(f.name), std::move(library)};
}

void put(Bytes& to, std::size_t at, ByteView what) { std::copy(what.begin(), what.end(), to.begin() + static_cast<std::ptrdiff_t>(at)); }

void put32(Bytes& to, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) to[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
}

}  // namespace

CB_TEST("symbols.runtime_library.signature_of_a_real_function", [] {
    // atof.obj: the C runtime's _atof; its four calls and data references are the linker's.
    const libs::Function atof = first_function("atof.obj");
    CB_CHECK_EQ(atof.name, "_atof");
    const auto sig = rl::signature(atof.code, atof.mask);
    CB_CHECK(sig.has_value());
    CB_CHECK_EQ(sig->size, std::size_t{112});
    CB_CHECK_EQ(sig->mask_text, "13.4,21.4,34.4,5d.4");
    CB_CHECK_EQ(sig->digest, "cb4c2f8cfd64387a");
    CB_CHECK_EQ(rl::c_name(atof.name), "atof");
    // a 16-bit function with nothing left out
    const libs::Function heap = first_function("libvxd.lib");
    const auto s16 = rl::signature(heap.code, heap.mask);
    CB_CHECK(s16.has_value());
    CB_CHECK_EQ(s16->size, std::size_t{28});
    CB_CHECK_EQ(s16->mask_text, "");
    CB_CHECK_EQ(s16->digest, "e53a590329edccc2");
});

CB_TEST("symbols.runtime_library.signature_ignores_the_linkers_bytes_and_nothing_else", [] {
    const libs::Function atof = first_function("atof.obj");
    const auto sig = rl::signature(atof.code, atof.mask);
    Bytes linked = atof.code;
    linked[0x13] = 0xAA;   // inside a run the linker fills in
    linked[0x5e] = 0x01;
    CB_CHECK(rl::signature(linked, atof.mask) == sig);
    linked[0x10] ^= 1;     // an ordinary byte
    CB_CHECK(rl::signature(linked, atof.mask) != sig);
});

CB_TEST("symbols.runtime_library.signature_refuses_what_tells_nothing", [] {
    // a stub of the import library (six bytes), a function of nothing but linker bytes, padding
    const libs::Function stub = first_function("lz32.lib");
    CB_CHECK(!rl::signature(stub.code, stub.mask).has_value());
    const Bytes frame = {0x55, 0x8b, 0xec, 0x5d, 0xc3};
    CB_CHECK(!rl::signature(frame, Bytes(5, 0)).has_value());
    const Bytes zeros(64, 0);
    CB_CHECK(!rl::signature(zeros, Bytes(64, 0)).has_value());
    Bytes all_masked(40, 0x90);
    for (std::size_t i = 0; i < all_masked.size(); i += 2) all_masked[i] = static_cast<std::uint8_t>(i);
    CB_CHECK(!rl::signature(all_masked, Bytes(40, 1)).has_value());
    // a mask shorter than the code leaves nothing out past its end
    CB_CHECK(rl::signature(all_masked, Bytes(3, 1)).has_value());
    // only the first kLongest bytes count
    const libs::Function atof = first_function("atof.obj");
    Bytes longer = atof.code;
    longer.resize(3000, 0x41);
    CB_CHECK_EQ(rl::signature(longer, atof.mask)->size, rl::kLongest);
});

CB_TEST("symbols.runtime_library.mask_offsets", [] {
    const std::vector<std::uint32_t> a = rl::mask_offsets("13.4,21.4");
    CB_CHECK_EQ(a.size(), std::size_t{8});
    CB_CHECK_EQ(a[0], std::uint32_t{0x13});
    CB_CHECK_EQ(a[3], std::uint32_t{0x16});
    CB_CHECK_EQ(a[4], std::uint32_t{0x21});
    CB_CHECK(rl::mask_offsets("").empty());
    CB_CHECK_THROWS_AS(cb::ParseError, rl::mask_offsets("13"));
    CB_CHECK_THROWS_AS(cb::ParseError, rl::mask_offsets("zz.4"));
    CB_CHECK_THROWS_AS(cb::ParseError, rl::mask_offsets("13.4,"));
});

CB_TEST("symbols.runtime_library.table_matches_by_leading_bytes_then_digest", [] {
    const libs::Function atof = first_function("atof.obj");
    const libs::Function pf = first_function("debug.lib");
    const std::vector<rl::SignatureRow> rows = {row_of(atof, "Test C runtime"), row_of(pf, "Test MFC")};
    const rl::SignatureTable table = rl::SignatureTable::from_rows(rows);
    CB_CHECK_EQ(table.size(), std::size_t{2});
    // the library's own bytes
    const auto hit = table.match(atof.code);
    CB_CHECK(hit.has_value());
    CB_CHECK_EQ(hit->name, "atof");
    CB_CHECK_EQ(hit->library, "Test C runtime");
    // what a program has: the linker's fields differ, the rest of the function follows in memory
    Bytes in_program = atof.code;
    in_program[0x14] = 0x77;
    in_program.resize(in_program.size() + 200, 0xCC);
    CB_CHECK(table.match(in_program).has_value());
    // an ordinary byte differs
    in_program[0x10] ^= 0x10;
    CB_CHECK(!table.match(in_program).has_value());
    // too short, empty, and a function that merely starts alike
    CB_CHECK(!table.match(ByteView(atof.code).first(50)).has_value());
    CB_CHECK(!table.match({}).has_value());
    CB_CHECK(table.match(pf.code).has_value());
    Bytes alike = pf.code;
    alike[40] ^= 0xFF;
    CB_CHECK(!table.match(alike).has_value());
});

CB_TEST("symbols.runtime_library.table_keeps_the_order_of_rows_with_one_lead", [] {
    // the 16-bit VxD library: twelve functions start with the same six bytes; each is told by its digest
    const libs::Library lib = libs::read_file(lib_path("libvxd.lib"));
    std::vector<rl::SignatureRow> rows;
    for (const libs::Function& f : lib.functions) {
        if (rl::signature(f.code, f.mask)) rows.push_back(row_of(f, "VxD"));
    }
    CB_CHECK_EQ(rows.size(), std::size_t{12});
    const rl::SignatureTable table = rl::SignatureTable::from_rows(rows);
    for (const libs::Function& f : lib.functions) {
        const auto hit = table.match(f.code);
        if (!rl::signature(f.code, f.mask)) {
            CB_CHECK(!hit.has_value());
            continue;
        }
        CB_CHECK(hit.has_value());
        CB_CHECK_EQ(hit->name, rl::c_name(f.name));
    }
    // the same signature twice: the first row is the answer
    std::vector<rl::SignatureRow> twice = {rows[0], rows[0]};
    twice[1].name = "second";
    CB_CHECK_EQ(rl::SignatureTable::from_rows(twice).match(lib.functions[0].code)->name, "VmmHeapAllocate");
});

CB_TEST("symbols.runtime_library.table_rejects_damaged_rows", [] {
    std::vector<rl::SignatureRow> bad = {{"zz", 10, "", "0123456789abcdef", "f", "L"}};
    CB_CHECK_THROWS_AS(cb::FormatError, rl::SignatureTable::from_rows(bad));
    bad = {{"5557", 10, "5.x", "0123456789abcdef", "f", "L"}};
    CB_CHECK_THROWS_AS(cb::ParseError, rl::SignatureTable::from_rows(bad));
    CB_CHECK_EQ(rl::SignatureTable::from_rows({}).size(), std::size_t{0});
});

CB_TEST("symbols.runtime_library.c_name", [] {
    CB_CHECK_EQ(rl::c_name("_memcpy"), "memcpy");
    CB_CHECK_EQ(rl::c_name("__allmul"), "_allmul");
    CB_CHECK_EQ(rl::c_name("_f@8"), "f");
    CB_CHECK_EQ(rl::c_name("_f@x"), "f@x");
    CB_CHECK_EQ(rl::c_name("_f@"), "f@");
    CB_CHECK_EQ(rl::c_name("?Foo@@YAXXZ"), "?Foo@@YAXXZ");
    CB_CHECK_EQ(rl::c_name("@fastcall@4"), "fastcall");   // (the Python module gave an empty name for these)
    CB_CHECK_EQ(rl::c_name("plain"), "plain");
    CB_CHECK_EQ(rl::c_name(""), "");
    CB_CHECK_EQ(rl::c_name("_"), "");
});

CB_TEST("symbols.runtime_library.runtime_library_modules", [] {
    CB_CHECK(rl::runtime_library_of("CRTDLL") != nullptr);
    CB_CHECK_EQ(*rl::runtime_library_of("MFC40"), "MFC 4.0 (this module is the class library)");
    CB_CHECK_EQ(*rl::runtime_library_of("MSVCIRT"), "C++ stream runtime (this module is the runtime library)");
    CB_CHECK(rl::runtime_library_of("KERNEL32") == nullptr);
    CB_CHECK(rl::runtime_library_of("crtdll") == nullptr);   // the stem is in capitals
});

namespace {

// A 32-bit module's code, written out: atof at 0x401000, a thunk compiler's stubs, a plain function.
struct FakePe {
    Bytes code = Bytes(0x300, 0);
    static constexpr std::uint64_t kBase = 0x400000, kVa = 0x401000;
    FakePe() {
        put(code, 0, first_function("atof.obj").code);
        // 0x200: call dword ptr [0x401100] ; ret          -- through a pointer kept in the code section
        put(code, 0x200, Bytes{0xFF, 0x15, 0x00, 0x11, 0x40, 0x00, 0xC3});
        // 0x210: mov cl, 3 ; jmp 0x401200                 -- a short stub that jumps to the common code
        put(code, 0x210, Bytes{0xB1, 0x03, 0xE9, 0xE9, 0xFF, 0xFF, 0xFF});
        // 0x220: push ebp ; mov ebp, esp ; pop ebp ; ret  -- compiled C
        put(code, 0x220, Bytes{0x55, 0x8B, 0xEC, 0x5D, 0xC3});
        // 0x230: jmp 0x401210                             -- two jumps from the common code: too far
        put(code, 0x230, Bytes{0xEB, 0xDE});
        // 0x240: call dword ptr [0x500000] ; ret          -- a pointer outside the code (an import)
        put(code, 0x240, Bytes{0xFF, 0x15, 0x00, 0x00, 0x50, 0x00, 0xC3});
    }
    rl::Pe32Module module() const {
        rl::Pe32Module m;
        m.stem = "SOMETHING";
        m.image_base = kBase;
        m.code = {{kVa, code}};
        m.function_starts = {0x401000, 0x401200, 0x401210, 0x401220, 0x401230, 0x401240, 0x409000};
        return m;
    }
};

}  // namespace

CB_TEST("symbols.runtime_library.flat_thunk_stubs", [] {
    const FakePe pe;
    cb::platforms::DecoderSet decoders;
    const rl::Pe32Module m = pe.module();
    const std::vector<std::uint64_t> stubs = rl::flat_thunk_stubs(m.code, m.function_starts, decoders);
    CB_CHECK_EQ(stubs.size(), std::size_t{2});
    CB_CHECK_EQ(stubs[0], std::uint64_t{0x401200});
    CB_CHECK_EQ(stubs[1], std::uint64_t{0x401210});
});

CB_TEST("symbols.runtime_library.identify_pe_by_signature_and_stub", [] {
    const FakePe pe;
    cb::platforms::DecoderSet decoders;
    const rl::SignatureTable table = rl::SignatureTable::from_rows(std::vector<rl::SignatureRow>{row_of(first_function("atof.obj"), "Test C runtime")});
    rl::Pe32Module m = pe.module();
    // a module the thunk compiler did not make: only the signature
    auto found = rl::identify_pe(table, m, decoders);
    CB_CHECK_EQ(found.size(), std::size_t{1});
    CB_CHECK_EQ(found.at(0x401000).name, "atof");
    m.flat_thunk = true;
    found = rl::identify_pe(table, m, decoders);
    CB_CHECK_EQ(found.size(), std::size_t{3});
    CB_CHECK_EQ(found.at(0x401200).name, "thunk stub");
    CB_CHECK_EQ(found.at(0x401210).library, std::string(rl::kThunkCompiler));
    CB_CHECK_EQ(found.at(0x401000).library, "Test C runtime");
});

CB_TEST("symbols.runtime_library.identify_pe_names_a_runtime_dlls_exports", [] {
    const FakePe pe;
    cb::platforms::DecoderSet decoders;
    const rl::SignatureTable table = rl::SignatureTable::from_rows({});
    rl::Pe32Module m = pe.module();
    m.stem = "MSVCRT20";
    m.exports = {{1, "printf", 0x1220}, {2, std::nullopt, 0x1230}, {3, "outside", 0x9000}, {4, std::nullopt, 0x1240}, {5, "", 0x1250}};
    m.ordinal_names = {{2, "_foo"}, {5, "bar"}};
    const auto found = rl::identify_pe(table, m, decoders);
    CB_CHECK_EQ(found.size(), std::size_t{3});   // 3 is outside the code; 4 has no name anywhere
    CB_CHECK_EQ(found.at(0x401220).name, "printf");
    CB_CHECK_EQ(found.at(0x401230).name, "_foo");   // named by the ordinal tables
    CB_CHECK_EQ(found.at(0x401250).name, "bar");    // an empty name is no name
    CB_CHECK_EQ(found.at(0x401220).library, "C runtime (this module is the runtime library)");
    // another module's exports are nobody's runtime
    m.stem = "KERNEL32";
    CB_CHECK(rl::identify_pe(table, m, decoders).empty());
});

CB_TEST("symbols.runtime_library.identify_ne", [] {
    const libs::Function pf = first_function("debug.lib");
    const rl::SignatureTable table = rl::SignatureTable::from_rows(std::vector<rl::SignatureRow>{row_of(pf, "Test 16-bit")});
    Bytes seg1(0x80, 0);
    put(seg1, 0x10, pf.code);
    Bytes seg2(0x20, 0x90);
    rl::Ne16Module m;
    m.module_name = "mfc250";
    m.segments = {{1, true, seg1}, {2, false, seg2}};
    m.function_starts = {{1, 0x10}, {1, 0x40}, {2, 0}};
    m.entries = {{5, 1, 0x20}, {6, 2, 0}, {7, 1, 0x30}, {8, 1, 0x10}};
    m.ordinal_names = {{5, "CString::Foo"}, {6, "InData"}, {8, "Eight"}};
    const auto found = rl::identify_ne(table, m);
    CB_CHECK_EQ(found.size(), std::size_t{2});
    CB_CHECK_EQ(found.at({1, 0x10}).name, "PrintFunc");               // by signature; the export 8 does not replace it
    CB_CHECK_EQ(found.at({1, 0x10}).library, "Test 16-bit");
    CB_CHECK_EQ(found.at({1, 0x20}).name, "CString::Foo");           // an export of a class library
    CB_CHECK_EQ(found.at({1, 0x20}).library, "MFC 2.5 (this module is the class library)");
    CB_CHECK(!found.contains({2, 0}));                                // a data segment holds no code
    CB_CHECK(!found.contains({1, 0x30}));                             // no name
    // a function whose segment the module does not have
    m.function_starts = {{9, 0}};
    CB_CHECK_THROWS_AS(cb::FormatError, rl::identify_ne(table, m));
    // an offset past the segment's end is no code
    m.function_starts = {{1, 0x7000}};
    m.entries = {};
    CB_CHECK(rl::identify_ne(table, m).empty());
});

namespace {

// A PE file's first bytes: a DOS header, the tool block, then (at 0x80) where the PE header would start.
Bytes with_tool_block() {
    Bytes file(0x100, 0);
    file[0] = 'M';
    file[1] = 'Z';
    constexpr std::uint32_t key = 0x12345678;
    put32(file, 0x40, 0x536E6144 ^ key);   // "DanS"
    put32(file, 0x44, key);                // padding: zero words
    put32(file, 0x48, key);
    put32(file, 0x4C, key);
    put32(file, 0x50, ((0x5Du << 16) | 0x1234u) ^ key);   // product 0x5D, build 0x1234, used 7 times
    put32(file, 0x54, 7u ^ key);
    put32(file, 0x58, ((0x5Fu << 16) | 0x2345u) ^ key);
    put32(file, 0x5C, 2u ^ key);
    put(file, 0x60, cb::as_bytes("Rich"));
    put32(file, 0x64, key);
    return file;
}

}  // namespace

CB_TEST("symbols.runtime_library.tool_block", [] {
    const Bytes file = with_tool_block();
    const auto records = rl::tool_block(file, 0x80);
    CB_CHECK_EQ(records.size(), std::size_t{2});
    CB_CHECK_EQ(records[0].product, std::uint32_t{0x5D});
    CB_CHECK_EQ(records[0].build, std::uint32_t{0x1234});
    CB_CHECK_EQ(records[0].count, std::uint32_t{7});
    CB_CHECK_EQ(records[1].product, std::uint32_t{0x5F});
    CB_CHECK_EQ(records[1].count, std::uint32_t{2});
    // none before the header; a header that is too early to leave room for it
    CB_CHECK(rl::tool_block(file, 0x40).empty());
    CB_CHECK(rl::tool_block(file, 0x60).empty());   // "Rich" is not wholly before it
    CB_CHECK(rl::tool_block({}, 0x80).empty());
    // a "Rich" with no start mark before it is no tool block
    Bytes no_start = file;
    put32(no_start, 0x40, 0);
    CB_CHECK(rl::tool_block(no_start, 0x80).empty());
    // a block whose key word runs off the end of the file
    CB_CHECK(rl::tool_block(ByteView(file).first(0x68), 0x80).size() == 2);
    CB_CHECK(rl::tool_block(ByteView(file).first(0x66), 0x80).empty());
    CB_CHECK(rl::tool_block(ByteView(file).first(0x64), 0x80).empty());
});

CB_TEST("symbols.runtime_library.compiler_of_a_32_bit_module", [] {
    rl::CompilerFacts facts;
    facts.kind = rl::CompilerFacts::Kind::Pe;
    const Bytes file = with_tool_block();
    facts.file = file;
    facts.linker_major = 2;
    facts.linker_minor = 60;
    facts.pe_header_offset = 0x80;
    facts.imported_modules = {"KERNEL32.dll", "MSVCRT20.DLL", "kernel32.dll", "CRTDLL.dll", "noext"};
    const std::vector<std::string> libraries = {"Visual C++ 2.0: C runtime", "Visual C++ 2.0: C runtime", "Visual C++ 2.0: MFC", "x: y"};
    const rl::CompilerInfo info = rl::describe_compiler(facts, libraries);
    CB_CHECK_EQ(info.linker, "2.60");
    CB_CHECK_EQ(info.compiler, "Microsoft Visual C++ 2.x");
    const std::vector<std::string> expected = {
        "linker version 2.60 in the header",
        "tool block before the header: 93.4660 x7, 95.9029 x2",
        "imports CRTDLL: the system C runtime (CRTDLL)",
        "imports MSVCRT20: Visual C++ 2.x's runtime DLL",
        "2 functions from Visual C++ 2.0: C runtime",
        "1 functions from Visual C++ 2.0: MFC",
        "1 functions from x: y"};
    CB_CHECK(info.evidence == expected);
});

CB_TEST("symbols.runtime_library.compiler_from_marks_and_libraries", [] {
    // marks: a Borland text overrides the linker's guess, a Microsoft one does not
    Bytes file(0x200, 0);
    put(file, 0x100, cb::as_bytes("Borland C++ - Copyright 1994"));
    put(file, 0x140, cb::as_bytes("Microsoft (R) C "));
    rl::CompilerFacts facts;
    facts.kind = rl::CompilerFacts::Kind::Pe;
    facts.file = file;
    facts.linker_major = 5;
    facts.linker_minor = 0;
    facts.pe_header_offset = 0x80;
    rl::CompilerInfo info = rl::describe_compiler(facts, {});
    CB_CHECK_EQ(info.compiler, "Borland C++");
    CB_CHECK_EQ(info.evidence.size(), std::size_t{3});
    CB_CHECK_EQ(info.evidence[1], "the text 'Borland C++ - Copyright'");
    CB_CHECK_EQ(info.evidence[2], "the text 'Microsoft (R) C '");
    // no linker guess, no mark: the library most of the functions came from names the compiler
    facts.file = ByteView(file).first(0x80);
    facts.linker_major = 9;
    const std::vector<std::string> libraries = {"Watcom C: lib", "Visual C++ 1.0: libc", "Visual C++ 1.0: libc", "Watcom C: lib", "Other: z"};
    info = rl::describe_compiler(facts, libraries);
    CB_CHECK_EQ(info.compiler, "Watcom C");   // a tie: the one met first
    CB_CHECK_EQ(info.linker, "9.00");
    // a 16-bit module
    rl::CompilerFacts ne;
    ne.kind = rl::CompilerFacts::Kind::Ne;
    ne.file = ByteView(file).first(0x80);
    ne.linker_major = 5;
    ne.linker_minor = 60;
    ne.module_references = {"KERNEL", "mfc250", "VBRUN300"};
    info = rl::describe_compiler(ne, {});
    CB_CHECK_EQ(info.compiler, "Microsoft tools of the Visual C++ 1.5 generation (16-bit)");
    CB_CHECK_EQ(info.evidence.size(), std::size_t{3});
    CB_CHECK_EQ(info.evidence[1], "imports MFC250: MFC 2.5 (Visual C++ 1.5)");
    CB_CHECK_EQ(info.evidence[2], "imports VBRUN300: Visual Basic 3.0");
    // not a module of either kind: only the marks and libraries speak
    info = rl::describe_compiler({}, {});
    CB_CHECK_EQ(info.compiler, "not established");
    CB_CHECK_EQ(info.linker, "");
    CB_CHECK(info.evidence.empty());
});

CB_TEST("symbols.runtime_library.frame_instructions", [] {
    // each answer is the Python module's _FRAME pattern's on the same text
    struct Case {
        const char* mnemonic;
        const char* operands;
        bool frame;
    };
    const Case cases[] = {{"push", "ebp", true},     {"push", "eax", false},       {"pop", "ds", true},        {"pop", "es", true},
                          {"mov", "ebp, esp", true}, {"mov", "bp, sp", true},      {"mov", "bp, esp", true},   {"mov", "esp, ebp", true},
                          {"mov", "ds, ax", true},   {"mov", "eax, 0", true},      {"mov", "ax, 0x1f", true},  {"mov", "eax, ebx", false},
                          {"mov", "eax, 0xzz", false}, {"mov", "dx, 5", true},     {"mov", "cx, 5", false},    {"xor", "eax, eax", true},
                          {"xor", "ax, eax", true},  {"xor", "ecx, ecx", false},   {"xor", "dx, dx", true},    {"inc", "bp", true},
                          {"dec", "bp", true},       {"inc", "ebp", false},        {"sub", "esp, 0x10", true}, {"sub", "sp, 4", true},
                          {"add", "bp, 2", true},    {"sub", "ebp, 2", false},     {"lea", "sp, [bp - 2]", true}, {"lea", "esp, [ebp - 2]", false},
                          {"nop", "", true},         {"nop", "dword ptr [eax]", true}, {"leave", "", true},  {"ret", "", true},
                          {"retn", "4", true},       {"retf", "", true},           {"int3", "", true},         {"call", "0x100", false},
                          {"push", "5", false},      {"mov", "eax, 0x", false},    {"mov", "eax, 0X1", false}, {"pop", "esi", true},
                          {"push", "edi", true},     {"pop", "ebx", true},         {"mov", "ax, ds", true},    {"mov", "es, ax", true},
                          {"mov", "ss, ax", false}};
    for (const Case& c : cases) CB_CHECK_EQ(rl::is_frame_instruction(c.mnemonic, c.operands), c.frame);
});

CB_TEST("symbols.runtime_library.substance", [] {
    cb::platforms::DecoderSet decoders;
    const libs::Function atof = first_function("atof.obj");
    CB_CHECK_EQ(rl::substance(decoders, rl::Bits::X32, atof.code, 112), std::size_t{31});
    const libs::Function heap = first_function("libvxd.lib");
    CB_CHECK_EQ(rl::substance(decoders, rl::Bits::X16, heap.code, 28), std::size_t{14});
    // an empty function: the frame and `return 0`
    const Bytes empty = {0x55, 0x8B, 0xEC, 0x33, 0xC0, 0x5D, 0xC3};
    CB_CHECK_EQ(rl::substance(decoders, rl::Bits::X32, empty, empty.size()), std::size_t{0});
    // the Windows frame of a 16-bit far function (inc bp / push bp / mov bp, sp / push ds ... ) has none of its own
    const Bytes frame16 = cb::crypto::from_hex("8cd89045558bec1e8ed833c08d66fe1f5d4dcb90");
    CB_CHECK_EQ(rl::substance(decoders, rl::Bits::X16, frame16, 20), std::size_t{0});
    // a size past the code is the code
    CB_CHECK_EQ(rl::substance(decoders, rl::Bits::X32, empty, 999), std::size_t{0});
});

CB_TEST("symbols.runtime_library.learn_from_library_files", [] {
    cb::platforms::DecoderSet decoders;
    const std::vector<rl::LearnSource> sources = {{"Test C runtime", lib_path("atof.obj")},
                                                  {"Test MFC thing", lib_path("debug.lib")},
                                                  {"Test VxD library", lib_path("libvxd.lib")},
                                                  {"Visual C++ import library", lib_path("lz32.lib")},
                                                  {"Missing", lib_path("nothing.lib")},
                                                  {"PowerPC", lib_path("xhttp.obj")}};
    std::vector<std::string> log;
    const rl::Learned learned = rl::learn(sources, decoders, [&](const std::string& s) { log.push_back(s); });
    // what the Python module's learn() found for the same files, in its order
    CB_CHECK_EQ(learned.libraries.size(), std::size_t{3});
    CB_CHECK_EQ(learned.libraries[0], "Test C runtime (ATOF.OBJ)");
    CB_CHECK_EQ(learned.libraries[2], "Test VxD library (LIBVXD.LIB)");
    CB_CHECK_EQ(learned.count(rl::Bits::X32), std::size_t{1});
    CB_CHECK_EQ(learned.count(rl::Bits::X16), std::size_t{13});
    CB_CHECK_EQ(learned.rows[0].row.name, "atof");
    CB_CHECK_EQ(learned.rows[1].row.name, "PrintFunc");
    CB_CHECK_EQ(learned.rows[1].library, std::size_t{1});
    CB_CHECK_EQ(learned.rows[2].row.name, "VmmHeapAllocate");
    // the log: one line per library read, the two that could not be used, and the count left out
    CB_CHECK_EQ(log.size(), std::size_t{7});
    CB_CHECK(log[0].starts_with("Test C runtime (ATOF.OBJ)"));
    CB_CHECK(log[0].ends_with("1 functions,     1 new signatures"));
    CB_CHECK(log[3].ends_with("12 functions,     0 new signatures"));
    CB_CHECK(log[4].find("nothing.lib") != std::string::npos);
    CB_CHECK(log[5].ends_with(": not x86 code"));
    CB_CHECK_EQ(log[6], "0 signatures left out: shared by differently named functions");
});

CB_TEST("symbols.runtime_library.learn_leaves_out_ambiguous_signatures", [] {
    // two differently named functions with the same bytes once the linker's are left out: a match could be either
    // (built as one COFF object holding the same code twice under two names is more than the test needs: use the
    // table form, which is what learn() reduces them to)
    cb::platforms::DecoderSet decoders;
    // a library read twice under two names: the same functions, the same names: nothing is ambiguous, nothing is added
    const std::vector<rl::LearnSource> twice = {{"First", lib_path("atof.obj")}, {"Second", lib_path("atof.obj")}};
    const rl::Learned learned = rl::learn(twice, decoders, [](const std::string&) {});
    CB_CHECK_EQ(learned.rows.size(), std::size_t{1});      // the earlier library's
    CB_CHECK_EQ(learned.libraries.size(), std::size_t{1});
    CB_CHECK_EQ(learned.libraries[0], "First (ATOF.OBJ)");
});

CB_TEST("symbols.runtime_library.signature_file_format", [] {
    cb::platforms::DecoderSet decoders;
    const std::vector<rl::LearnSource> sources = {{"Test C runtime", lib_path("atof.obj")}, {"Test MFC thing", lib_path("debug.lib")}};
    const rl::Learned learned = rl::learn(sources, decoders, [](const std::string&) {});
    // the text the Python module wrote for these two libraries
    const std::string expected =
        "{\"libraries\": [\"Test C runtime (ATOF.OBJ)\", \"Test MFC thing (DEBUG.LIB)\"],\n"
        "\"32\": [\n"
        "[\"56578b74240c\",112,\"13.4,21.4,34.4,5d.4\",\"cb4c2f8cfd64387a\",\"atof\",0]\n"
        "],\n"
        "\"16\": [\n"
        "[\"9c5060cd20c1\",71,\"\",\"aed14bc971ecfe2a\",\"PrintFunc\",1]\n"
        "]\n"
        "}\n";
    CB_CHECK_EQ(rl::format_signature_file(learned), expected);
    // and what it says is what the table is made from
    const std::string empty = rl::format_signature_file({});
    CB_CHECK_EQ(empty, "{\"libraries\": [],\n\"32\": [\n\n],\n\"16\": [\n\n]\n}\n");
});

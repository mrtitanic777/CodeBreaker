// Tests of cb/symbols/libraries/omf. Two real libraries of the Windows 95 DDK (testdata: libvxd.lib, debug.lib: Microsoft C
// object modules in a paged .LIB) give the functions and their offsets; the fix-up masks are checked on a hand-made
// module whose records are written out below, so every byte of the expected mask can be read off its fix-ups (the
// Python module's answer on the same bytes was compared once: same functions, bytes, masks and offsets). The readers were
// also run over every DDK and Win32 SDK library of the reference folder against the Python module.
//
// DIVERGES from the Python module, by design: a record that runs past its end (a name longer than its record, a field
// cut short) leaves its module out whole, where the Python took the shorter name or the clamped slice; a data record
// that puts bytes beyond 64 MiB of one segment is refused (a 32-bit record can ask for 4 GiB) and likewise leaves its
// module out.
#include "cb/symbols/libraries/omf.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

namespace om = cb::symbols::libraries::omf;
using cb::Bytes;
using cb::ByteView;
using cb::symbols::libraries::Function;

namespace {

Bytes data(const char* name) { return cb::fs::read_file(cb::testing::data_path(std::string("symbols/libraries/testdata/") + name)); }

// One OMF record: type, length (the body and the checksum byte), body, checksum (not checked by the reader).
Bytes rec(std::uint8_t kind, const Bytes& body) {
    cb::io::ByteWriter w;
    w.u8(kind);
    w.u16(static_cast<std::uint16_t>(body.size() + 1));
    w.bytes(body);
    w.u8(0);
    return w.take();
}
Bytes cat(std::initializer_list<Bytes> parts) {
    Bytes out;
    for (const Bytes& p : parts) cb::append(out, p);
    return out;
}
Bytes name(std::string_view s) {
    Bytes out{static_cast<std::uint8_t>(s.size())};
    cb::append(out, cb::as_bytes(s));
    return out;
}
Bytes u16(std::uint16_t v) { return {static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8)}; }

// A module with two code functions (_func_a: a far call; _func_b: a 16-bit immediate), a data segment, and a COMDAT
// piece (_comdat_f) with a fix-up of its own.
//
//   segment 1 _TEXT (CODE), 14 bytes   0: 55 8B EC 9A 00 00 00 00 5D CB      _func_a   (the far call's pointer is fixed up)
//                                      10: B8 00 00 C3                        _func_b   (the immediate is fixed up)
//   segment 2 _DATA (DATA), 4 bytes    _var
//   COMDAT _comdat_f                   E8 00 00 C3                           (the displacement is fixed up)
Bytes module() {
    return cat({
        rec(0x80, name("T.ASM")),                                                                             // THEADR
        rec(0x96, cat({name("_TEXT"), name("CODE"), name("_DATA"), name("DATA"), name("_comdat_f")})),       // LNAMES 1..5
        rec(0x98, cat({{0x48}, u16(14), {1, 2}})),                                                            // SEGDEF: _TEXT, class CODE
        rec(0x98, cat({{0x48}, u16(4), {3, 4}})),                                                             // SEGDEF: _DATA, class DATA
        rec(0x90, cat({{0, 1}, name("_func_a"), u16(0), {0}, name("_func_b"), u16(10), {0}})),               // PUBDEF in segment 1
        rec(0x90, cat({{0, 2}, name("_var"), u16(0), {0}})),                                                  // PUBDEF in segment 2
        rec(0xA0, cat({{1}, u16(0), {0x55, 0x8B, 0xEC, 0x9A, 0, 0, 0, 0, 0x5D, 0xCB, 0xB8, 0, 0, 0xC3}})),  // LEDATA
        // FIXUPP: a thread first (it only says how later fix-ups are framed), then a pointer fix-up at offset 4 (the far
        // call's operand) and an offset fix-up at 11
        rec(0x9C, {0x00, 0x01, 0xCC, 4, 0x8C, 0xC4, 11, 0x8C}),
        rec(0xA0, cat({{2}, u16(0), {1, 2, 3, 4}})),                                                          // LEDATA of the data segment
        rec(0xC2, cat({{0x00, 0x01, 0x00}, u16(0), {0}, {5}, {0xE8, 0, 0, 0xC3}})),                          // COMDAT _comdat_f
        rec(0x9C, {0xC4, 1, 0x8C}),                                                                           // its fix-up at offset 1
        rec(0x8A, {0}),                                                                                       // MODEND
    });
}

// A module whose only fix-up names a place 40 bytes into a 4-byte segment.
Bytes damaged_module() {
    return cat({
        rec(0x80, name("B.ASM")),
        rec(0x96, cat({name("_TEXT"), name("CODE")})),
        rec(0x98, cat({{0x48}, u16(4), {1, 2}})),
        rec(0x90, cat({{0, 1}, name("_bad"), u16(0), {0}})),
        rec(0xA0, cat({{1}, u16(0), {1, 2, 3, 4}})),
        rec(0x9C, {0xCC, 40, 0x8C}),
        rec(0x8A, {0}),
    });
}

// A library: the header page (its record length is the page size less three), each module padded to the page, the end record.
Bytes library(const std::vector<Bytes>& modules, std::size_t page) {
    cb::io::ByteWriter w;
    w.u8(0xF0);
    w.u16(static_cast<std::uint16_t>(page - 3));
    w.zeros(page - 3);
    for (const Bytes& m : modules) {
        w.bytes(m);
        w.align(page);
    }
    w.u8(0xF1);
    w.u16(0);
    return w.take();
}

const Function* find(const std::vector<Function>& fs, std::string_view name) {
    for (const Function& f : fs) {
        if (f.name == name) return &f;
    }
    return nullptr;
}

}  // namespace

CB_TEST("symbols.libraries.omf.hand_made_module", [] {
    const std::vector<Function> fs = om::functions(module());
    // the pieces first (in the order they were met), then the publics of code segments by segment and offset; _var is data
    CB_CHECK_EQ(fs.size(), std::size_t{3});
    CB_CHECK_EQ(fs[0].name, "_comdat_f");
    CB_CHECK_EQ(fs[1].name, "_func_a");
    CB_CHECK_EQ(fs[2].name, "_func_b");
    CB_CHECK(fs[0].code == (Bytes{0xE8, 0, 0, 0xC3}));
    CB_CHECK(fs[0].mask == (Bytes{0, 1, 1, 0}));
    CB_CHECK_EQ(fs[0].at, 0u);
    // _func_a ends where _func_b starts; the far call's opcode is 2 (the linker may rewrite it), its pointer 1
    CB_CHECK(fs[1].code == (Bytes{0x55, 0x8B, 0xEC, 0x9A, 0, 0, 0, 0, 0x5D, 0xCB}));
    CB_CHECK(fs[1].mask == (Bytes{0, 0, 0, 2, 1, 1, 1, 1, 0, 0}));
    CB_CHECK_EQ(fs[1].at, 0u);
    CB_CHECK(fs[2].code == (Bytes{0xB8, 0, 0, 0xC3}));
    CB_CHECK(fs[2].mask == (Bytes{0, 1, 1, 0}));
    CB_CHECK_EQ(fs[2].at, 10u);
    CB_CHECK(fs[1].ignore.empty());
});

CB_TEST("symbols.libraries.omf.fields_only_drops_far_call_opcodes", [] {
    const std::vector<Function> fs = om::functions(module());
    CB_CHECK(om::fields_only(fs[1].mask) == (Bytes{0, 0, 0, 0, 1, 1, 1, 1, 0, 0}));
    CB_CHECK(om::fields_only({}).empty());
});

CB_TEST("symbols.libraries.omf.paged_library", [] {
    // three modules on 64-byte and on 16-byte pages; the middle one cannot be followed and gives nothing, the others still do
    for (const std::size_t page : {std::size_t{16}, std::size_t{64}}) {
        const Bytes lib = library({module(), damaged_module(), module()}, page);
        const std::vector<Function> fs = om::functions(lib);
        std::vector<std::string> names;
        for (const Function& f : fs) names.push_back(f.name);
        CB_CHECK(names == (std::vector<std::string>{"_comdat_f", "_func_a", "_func_b", "_comdat_f", "_func_a", "_func_b"}));
    }
});

CB_TEST("symbols.libraries.omf.damaged_module_alone", [] {
    CB_CHECK(om::functions(damaged_module()).empty());
    CB_CHECK_THROWS_AS(cb::FormatError, (void)om::module_functions(damaged_module()));
});

CB_TEST("symbols.libraries.omf.nothing_in_other_data", [] {
    CB_CHECK(om::functions({}).empty());
    const Bytes one{0xF0};
    CB_CHECK(om::functions(one).empty());                       // a library header cut off
    const Bytes junk(300, 0x41);
    CB_CHECK(om::functions(junk).empty());                      // no module end: nothing is read
    CB_CHECK(om::functions(cat({Bytes{0xF1, 0, 0}})).empty());  // the end record alone
});

CB_TEST("symbols.libraries.omf.real_ddk_libraries", [] {
    // LIBVXD.LIB: Microsoft's thunks to the VMM services, each `push 5 / int 20h / dd service` and a ret
    const std::vector<Function> vxd = om::functions(data("libvxd.lib"));
    CB_CHECK_EQ(vxd.size(), std::size_t{13});
    const Function* alloc = find(vxd, "_VmmHeapAllocate");
    CB_CHECK(alloc != nullptr);
    if (alloc == nullptr) return;
    CB_CHECK_EQ(alloc->code.size(), std::size_t{28});
    CB_CHECK_EQ(alloc->at, 0u);
    CB_CHECK(std::equal(alloc->code.begin(), alloc->code.begin() + 8, Bytes{0x6A, 0x05, 0xCD, 0x20, 0xF5, 0x00, 0x01, 0x00}.begin()));
    CB_CHECK(std::all_of(alloc->mask.begin(), alloc->mask.end(), [](std::uint8_t m) { return m == 0; }));   // no fix-ups: the VMM service number is in the bytes
    CB_CHECK_EQ(find(vxd, "_VmmHeapFree")->at, 28u);                // the next function of the segment
    CB_CHECK_EQ(find(vxd, "_VmmHeapGetSize")->at, 56u);
    CB_CHECK_EQ(find(vxd, "_VmmHeapReAllocate")->code.size(), std::size_t{34});
    // a piece that is no public: the linker's own label, one byte
    const Function* piece = find(vxd, "cs10");
    CB_CHECK(piece != nullptr);
    if (piece == nullptr) return;
    CB_CHECK(piece->code == Bytes{0xC3});

    // DEBUG.LIB: one function
    const std::vector<Function> dbg = om::functions(data("debug.lib"));
    CB_CHECK_EQ(dbg.size(), std::size_t{1});
    CB_CHECK_EQ(dbg[0].name, "PrintFunc");
    CB_CHECK_EQ(dbg[0].code.size(), std::size_t{71});
    CB_CHECK_EQ(dbg[0].code[0], std::uint8_t{0x9C});               // pushf
});

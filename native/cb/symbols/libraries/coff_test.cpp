// Tests of cb/symbols/libraries/coff on real objects and archives: atof.obj (a Visual C++ x86 object from the Win32 SDK's
// C runtime, one function with four fix-ups), xhttp.obj (an Xbox 360 XDK object of PowerPC: eight small functions each
// ending in a `bl` the linker fills), lz32.lib (the SDK's import library, twelve thunks `jmp [__imp_X]`), and cap32.lib
// (an import library whose functions are imported by number). The expected values were compared with the Python
// module's reading of the same files once; the readers were also run over the whole XDK library folder (XAPOBase,
// libcMT, xhttp, xboxkrnl ...) and the Win32 SDK and DDK libraries against the Python module, function for function.
// Archives of damaged or odd shapes are made in the test out of these objects.
//
// DIVERGES from the Python module, by design: a section whose raw data runs past the end of the file makes the object
// damaged (the Python cut the section short and read what was there); a long symbol name whose string-table offset
// lies beyond the data is empty and one that has no terminator runs to the end of the data (the Python took all but
// its last byte); a symbol past the end of its section names no bytes instead of an empty slice.
#include "cb/symbols/libraries/coff.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

namespace coff = cb::symbols::libraries::coff;
using cb::Bytes;
using cb::ByteView;
using cb::symbols::libraries::Function;

namespace {

Bytes data(const char* name) { return cb::fs::read_file(cb::testing::data_path(std::string("symbols/libraries/testdata/") + name)); }
Bytes ordinals_data(const char* name) { return cb::fs::read_file(cb::testing::data_path(std::string("symbols/testdata/ordinals/") + name)); }

// An archive member: its 60-byte header (name, date, owner, mode, size, the closing "`\n") and its body, padded to even.
void add_member(cb::io::ByteWriter& w, std::string_view label, ByteView body) {
    std::string head(label);
    head.resize(16, ' ');
    head += "0           0     0     100644  ";   // date, uid, gid, mode
    std::string size = std::to_string(body.size());
    size.resize(10, ' ');
    head += size;
    head += "`\n";
    w.bytes(cb::as_bytes(head));
    w.bytes(body);
    if (body.size() & 1) w.u8('\n');
}

Bytes archive(const std::vector<std::pair<std::string, Bytes>>& members) {
    cb::io::ByteWriter w;
    w.bytes(cb::as_bytes("!<arch>\n"));
    for (const auto& [label, body] : members) add_member(w, label, body);
    return w.take();
}

std::vector<std::string> labels(const std::vector<coff::Member>& ms) {
    std::vector<std::string> out;
    for (const coff::Member& m : ms) out.push_back(m.label);
    return out;
}

const Function* find(const std::vector<Function>& fs, std::string_view name) {
    for (const Function& f : fs) {
        if (f.name == name) return &f;
    }
    return nullptr;
}

}  // namespace

CB_TEST("symbols.libraries.coff.x86_object", [] {
    const Bytes obj = data("atof.obj");
    const std::vector<Function> fs = coff::functions(obj);
    CB_CHECK_EQ(fs.size(), std::size_t{1});
    const Function& f = fs[0];
    CB_CHECK_EQ(f.name, "_atof");
    CB_CHECK_EQ(f.code.size(), std::size_t{112});
    CB_CHECK_EQ(f.at, 0u);
    CB_CHECK(std::equal(f.code.begin(), f.code.begin() + 12, Bytes{0x56, 0x57, 0x8B, 0x74, 0x24, 0x0C, 0xBF, 1, 0, 0, 0, 0x05}.begin()));
    // four 32-bit fields of the code are filled by the linker (relocations at 19, 33, 52 and 93)
    std::vector<std::size_t> left_out;
    for (std::size_t i = 0; i < f.mask.size(); ++i) {
        if (f.mask[i]) left_out.push_back(i);
    }
    CB_CHECK(left_out == (std::vector<std::size_t>{19, 20, 21, 22, 33, 34, 35, 36, 52, 53, 54, 55, 93, 94, 95, 96}));
    CB_CHECK(f.ignore.empty());
});

CB_TEST("symbols.libraries.coff.x86_object_tables", [] {
    const Bytes obj = data("atof.obj");
    const coff::Object o = coff::Object::parse(obj);
    CB_CHECK_EQ(o.machine(), coff::kI386);
    CB_CHECK_EQ(o.sections().size(), std::size_t{2});
    const coff::Section& text = o.sections()[0];
    CB_CHECK_EQ(text.name, ".text");
    CB_CHECK_EQ(text.size, 112u);
    CB_CHECK_EQ(text.ptr, 100u);
    CB_CHECK_EQ(text.chars & coff::kSectionCode, coff::kSectionCode);
    CB_CHECK_EQ(o.raw_data(text).size(), std::size_t{112});
    const std::vector<coff::Relocation> rel = o.relocations(text);
    CB_CHECK_EQ(rel.size(), std::size_t{4});
    CB_CHECK_EQ(rel[0].offset, 19u);
    CB_CHECK_EQ(rel[0].symbol, 7u);
    CB_CHECK_EQ(rel[0].type, std::uint16_t{6});     // DIR32
    CB_CHECK_EQ(rel[1].type, std::uint16_t{20});    // REL32
    // the symbol table as the file lays it out: auxiliary records are empty slots, so a relocation's index addresses it
    const auto& syms = o.symbols();
    CB_CHECK_EQ(syms.size(), std::size_t{14});
    CB_CHECK(syms[0].has_value() && syms[0]->name == ".file" && syms[0]->sec == -2);   // a debug symbol
    CB_CHECK(!syms[1].has_value());
    CB_CHECK(syms[6].has_value() && syms[6]->name == "_atof" && syms[6]->cls == 2 && syms[6]->type == 0x20 && syms[6]->sec == 1);
    CB_CHECK(syms[rel[0].symbol].has_value() && syms[rel[0].symbol]->name == "___mb_cur_max");   // the field's target
    CB_CHECK(syms[rel[0].symbol]->sec == 0);                                                      // undefined: an import
});

CB_TEST("symbols.libraries.coff.powerpc_object", [] {
    const Bytes obj = data("xhttp.obj");
    const coff::Object o = coff::Object::parse(obj);
    CB_CHECK_EQ(o.machine(), coff::kPowerPc);
    CB_CHECK_EQ(o.sections().size(), std::size_t{25});
    CB_CHECK_EQ(o.symbols().size(), std::size_t{70});
    CB_CHECK(o.symbols()[0]->name == "@comp.id" && o.symbols()[0]->sec == -1);

    const std::vector<Function> fs = coff::functions(obj);
    std::vector<std::string> names;
    for (const Function& f : fs) names.push_back(f.name);
    CB_CHECK(names == (std::vector<std::string>{"XHttpSetCredentials", "XHttpQueryAuthSchemes", "XHttpCrackUrlW", "XHttpCrackUrl",
                                                 "XHttpCreateUrl", "XHttpCreateUrlW", "XHttpResetPerfCounters", "XHttpGetPerfCounters"}));
    const std::vector<std::size_t> sizes = {32, 24, 24, 24, 24, 24, 8, 12};
    for (std::size_t i = 0; i < fs.size(); ++i) {
        CB_CHECK_EQ(fs[i].code.size(), sizes[i]);
        CB_CHECK_EQ(fs[i].ignore.size(), sizes[i] / 4);
        CB_CHECK(fs[i].mask.empty());
        CB_CHECK_EQ(fs[i].at, 0u);   // each is the first of its own code section
    }
    // the last word of each is a `bl` whose 24-bit distance the linker fills: only that field is left out
    CB_CHECK(fs[0].ignore == (std::vector<std::uint32_t>{0, 0, 0, 0, 0, 0, 0, 0x03FFFFFC}));
    CB_CHECK(fs[1].ignore == (std::vector<std::uint32_t>{0, 0, 0, 0, 0, 0x03FFFFFC}));
    CB_CHECK(fs[6].ignore == (std::vector<std::uint32_t>{0, 0x03FFFFFC}));
    // big-endian words: mr r9,r8 (7D 09 43 78) first
    CB_CHECK(std::equal(fs[0].code.begin(), fs[0].code.begin() + 4, Bytes{0x7D, 0x09, 0x43, 0x78}.begin()));
});

CB_TEST("symbols.libraries.coff.relocation_fields_of_powerpc", [] {
    CB_CHECK_EQ(coff::field_bits(coff::kRelRel24).value(), 0x03FFFFFCu);
    CB_CHECK_EQ(coff::field_bits(coff::kRelAddr24).value(), 0x03FFFFFCu);
    CB_CHECK_EQ(coff::field_bits(coff::kRelRel14).value(), 0x0000FFFCu);
    CB_CHECK_EQ(coff::field_bits(coff::kRelAddr14).value(), 0x0000FFFCu);
    for (const std::uint16_t t : {coff::kRelRefHi, coff::kRelRefLo, coff::kRelAddr16, coff::kRelSecRelLo, coff::kRelSecRelHi, coff::kRelGpRel}) {
        CB_CHECK_EQ(coff::field_bits(t).value(), 0x0000FFFFu);
    }
    CB_CHECK(!coff::field_bits(coff::kRelAddr32).has_value());   // a whole word
    CB_CHECK(!coff::field_bits(coff::kRelPair).has_value());
    CB_CHECK_EQ(coff::field_bits(0x0F00 | coff::kRelRel24).value(), 0x03FFFFFCu);   // only the low byte of the type says which
});

CB_TEST("symbols.libraries.coff.real_import_library_archive", [] {
    const Bytes lib = data("lz32.lib");
    const std::vector<coff::Member> ms = coff::members(lib);
    // the two linker members ("/") are not objects; fifteen objects follow, each labelled with the DLL's name
    CB_CHECK_EQ(ms.size(), std::size_t{15});
    CB_CHECK_EQ(ms[0].label, "LZ32.dll");
    CB_CHECK_EQ(ms[0].body.size(), std::size_t{586});
    CB_CHECK_EQ(ms[1].body.size(), std::size_t{127});
    // twelve of them hold a thunk: jmp dword ptr [__imp__X] with its address filled in by the linker
    std::vector<Function> fs;
    for (const coff::Member& m : ms) {
        for (Function& f : coff::functions(m.body)) fs.push_back(std::move(f));
    }
    CB_CHECK_EQ(fs.size(), std::size_t{12});
    const Function* open = find(fs, "_LZOpenFileA@12");
    CB_CHECK(open != nullptr);
    if (open == nullptr) return;
    CB_CHECK(open->code == (Bytes{0xFF, 0x25, 0, 0, 0, 0}));
    CB_CHECK(open->mask == (Bytes{0, 0, 1, 1, 1, 1}));
    CB_CHECK(find(fs, "_LZDone@0") != nullptr);
});

CB_TEST("symbols.libraries.coff.archive_labels_and_shapes", [] {
    const Bytes obj = data("atof.obj");
    const std::string names = "a_rather_long_object_name.obj/\nand_another_one.obj/\n";
    // the symbol table ("/") and the long-name table ("//") are the linker's; "/0" and "/31" name members in the table
    Bytes odd = obj;
    if (odd.size() % 2 == 0) odd.push_back(0);   // an odd-sized member is padded to even with a newline
    const Bytes arch = archive({{"/", Bytes{0, 0, 0, 0}},
                                {"//", cb::to_bytes(names)},
                                {"/0", obj},
                                {"short.obj/", odd},
                                {"/31", obj},
                                {"/999", obj}});   // a long-name offset past the table: an empty label
    const std::vector<coff::Member> ms = coff::members(arch);
    CB_CHECK(labels(ms) == (std::vector<std::string>{"a_rather_long_object_name.obj", "short.obj", "and_another_one.obj", ""}));
    CB_CHECK_EQ(ms[0].body.size(), obj.size());
    CB_CHECK_EQ(ms[1].body.size(), odd.size());
    CB_CHECK(std::equal(ms[2].body.begin(), ms[2].body.end(), obj.begin(), obj.end()));   // the padding byte did not shift the next header

    // the archive's label for an object is raw bytes: Latin-1 is decoded once
    const Bytes latin = archive({{"caf\xE9.obj/", obj}});
    CB_CHECK_EQ(coff::members(latin)[0].label, "caf\xC3\xA9.obj");
});

CB_TEST("symbols.libraries.coff.archive_that_is_cut_or_damaged", [] {
    const Bytes obj = data("atof.obj");
    const Bytes arch = archive({{"a.obj/", obj}, {"b.obj/", obj}});
    // a last member the end of the file cuts short is taken as far as it goes
    const ByteView cut(arch.data(), arch.size() - 100);
    const std::vector<coff::Member> ms = coff::members(cut);
    CB_CHECK_EQ(ms.size(), std::size_t{2});
    CB_CHECK_EQ(ms[1].body.size(), obj.size() - 100);
    // a header that gives no size ends the archive; what came before it is kept
    Bytes broken = arch;
    std::fill(broken.begin() + 8 + 60 + static_cast<std::ptrdiff_t>(obj.size()) + 16, broken.begin() + 8 + 60 + static_cast<std::ptrdiff_t>(obj.size()) + 58, '?');
    CB_CHECK_EQ(coff::members(broken).size(), std::size_t{1});
    // not an archive, and an archive with nothing in it
    CB_CHECK(coff::members(obj).empty());
    CB_CHECK(coff::members({}).empty());
    CB_CHECK(coff::members(cb::as_bytes("!<arch>\n")).empty());
    CB_CHECK(coff::members(cb::as_bytes("!<arch>\njunkjunkjunkjunkjunkjunkjunkjunkjunkjunkjunkjunkjunkjunkjunkjunkjunkjunk")).empty());
    // a size too big for the data is not an overflow
    Bytes huge = archive({{"a.obj/", obj}});
    std::string size = "18446744073709551615";   // 2^64 - 1, 20 digits: more than the field holds, so the field is read as far as it goes
    std::copy(size.begin(), size.begin() + 10, huge.begin() + 8 + 48);
    CB_CHECK_EQ(coff::members(huge).size(), std::size_t{1});   // 1844674407 bytes: the member is taken to the end of the data
});

CB_TEST("symbols.libraries.coff.damaged_objects", [] {
    const Bytes obj = data("atof.obj");
    CB_CHECK(coff::functions({}).empty());
    CB_CHECK(coff::functions(ByteView(obj.data(), 19)).empty());   // too short to hold a header: nothing to say
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)coff::Object::parse(ByteView(obj.data(), 10)));
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)coff::Object::parse(ByteView(obj.data(), 40)));   // the section table runs past the data
    // the code of the object is cut at the end of the file: damaged (the relocations and the raw data are not all there)
    CB_CHECK_THROWS_AS(cb::FormatError, (void)coff::functions(ByteView(obj.data(), 150)));
    CB_CHECK(coff::functions(ByteView(obj.data(), 284)).empty());   // all but the symbol table is there: no symbol, no function
    const Bytes ppc = data("xhttp.obj");
    CB_CHECK(coff::functions(ByteView(ppc.data(), ppc.size() / 2)).empty());   // (its symbol table is at the end: gone)
    // another machine: nothing to read, not an error
    Bytes other = obj;
    other[0] = 0x64;
    other[1] = 0x86;
    CB_CHECK(coff::functions(other).empty());
    // a symbol table cut short ends where the data does
    const coff::Object o = coff::Object::parse(ByteView(obj.data(), 368));   // five of the fourteen records
    CB_CHECK_EQ(o.symbols().size(), std::size_t{6});             // (.file and its three auxiliary slots, .text and one)
    CB_CHECK(o.symbols()[4].has_value() && o.symbols()[4]->name == ".text");
});

CB_TEST("symbols.libraries.coff.long_symbol_names", [] {
    // a symbol whose name is in the string table, and one whose offset is past the data; an unterminated name runs to the end
    cb::io::ByteWriter w;
    w.u16(coff::kI386);
    w.u16(0);                 // no sections
    w.u32(0);
    w.u32(20);                // the symbol table at 20
    w.u32(3);                 // three symbols
    w.u16(0);
    w.u16(0);
    auto symbol = [&](std::uint32_t string_offset) {
        w.u32(0);             // a zero first word: the name is in the string table
        w.u32(string_offset);
        w.u32(0);
        w.u16(1);
        w.u16(0x20);
        w.u8(2);
        w.u8(0);
    };
    symbol(4);                // right after the table's own size word
    symbol(1000);             // beyond the data
    symbol(4 + 12);           // the unterminated name
    w.u32(0);                 // (the table's size word)
    w.bytes(cb::as_bytes("long_name_1"));
    w.u8(0);
    w.bytes(cb::as_bytes("unterminated"));
    const Bytes obj = w.take();
    const coff::Object o = coff::Object::parse(obj);
    CB_CHECK_EQ(o.symbols().size(), std::size_t{3});
    CB_CHECK_EQ(o.symbols()[0]->name, "long_name_1");
    CB_CHECK_EQ(o.symbols()[1]->name, "");
    CB_CHECK_EQ(o.symbols()[2]->name, "unterminated");
});

CB_TEST("symbols.libraries.coff.import_by_ordinal_real_library", [] {
    const Bytes lib = ordinals_data("cap32.lib");
    std::vector<std::pair<std::uint32_t, std::string>> found;
    for (const coff::Member& m : coff::members(lib)) {
        if (const auto imp = coff::import_by_ordinal(m.body)) {
            CB_CHECK_EQ(imp->dll, "CAP");   // (the long form names the DLL as its descriptor symbol does: without the extension)
            found.emplace_back(imp->ordinal, imp->symbol);
        }
    }
    std::sort(found.begin(), found.end());
    CB_CHECK(found == (std::vector<std::pair<std::uint32_t, std::string>>{
                          {2, "__penter"}, {3, "_StartCAP@0"}, {4, "_StopCAP@0"}, {5, "_DumpCAP@0"}}));
    // an import library that imports by name has none by number
    const Bytes by_name = data("lz32.lib");   // (the members are views into it: it must outlive the loop)
    for (const coff::Member& m : coff::members(by_name)) CB_CHECK(!coff::import_by_ordinal(m.body).has_value());
});

CB_TEST("symbols.libraries.coff.import_by_ordinal_short_form", [] {
    // the short import header: signature 0000 FFFF, version, machine, time, size, ordinal/hint, type; then the two names
    auto member = [](std::uint16_t ordinal, std::uint16_t type, std::string_view symbol, std::string_view dll) {
        cb::io::ByteWriter w;
        w.u16(0);
        w.u16(0xFFFF);
        w.u16(0);
        w.u16(coff::kI386);
        w.u32(0);
        w.u32(static_cast<std::uint32_t>(symbol.size() + dll.size() + 2));
        w.u16(ordinal);
        w.u16(type);
        w.cstr(symbol);
        w.cstr(dll);
        return w.take();
    };
    const auto by_number = coff::import_by_ordinal(member(42, 0x0000, "_Thing@4", "THING.dll"));
    CB_CHECK(by_number.has_value());
    if (by_number) {
        CB_CHECK_EQ(by_number->ordinal, 42u);
        CB_CHECK_EQ(by_number->symbol, "_Thing@4");
        CB_CHECK_EQ(by_number->dll, "THING.dll");
    }
    CB_CHECK(!coff::import_by_ordinal(member(42, 0x0004, "_Thing@4", "THING.dll")).has_value());   // name type 1: by name
    CB_CHECK(!coff::import_by_ordinal(Bytes(12, 0)).has_value());                                    // too short
    CB_CHECK(!coff::import_by_ordinal(data("atof.obj")).has_value());                               // an object that is no import
    CB_CHECK(!coff::import_by_ordinal(ByteView(data("atof.obj").data(), 40)).has_value());         // a damaged one
});

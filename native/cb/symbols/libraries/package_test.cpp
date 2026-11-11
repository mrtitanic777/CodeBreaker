// Tests of cb/symbols/libraries (read, fits, Index) on the real objects and libraries of testdata (see coff_test.cpp and
// omf_test.cpp for where they come from), and on words written out for the index. The Python module's answers on the
// same inputs were compared once (kind, function count, every function's name, bytes, mask and offset).
//
// DIVERGES from the Python module, by design: `read` of a single damaged COFF object is a FormatError (the Python let
// struct.error through), and of a library whose header page is cut off gives no function (the Python raised); the kind
// is an enum, not 16 / 32 / "powerpc"; `Index::find` with `ignore` not as long as the words is a UsageError (the Python let
// a shorter one compare only the words it covered and raised IndexError for a longer one).
#include "cb/symbols/libraries/package.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/symbols/libraries/coff.hpp"

namespace lib = cb::symbols::libraries;
using cb::Bytes;
using cb::ByteView;
using lib::Function;
using lib::Machine;

namespace {

std::filesystem::path path(const char* name) { return cb::testing::data_path(std::string("symbols/libraries/testdata/") + name); }
Bytes data(const char* name) { return cb::fs::read_file(path(name)); }

Bytes archive(const std::vector<std::pair<std::string, Bytes>>& members) {
    cb::io::ByteWriter w;
    w.bytes(cb::as_bytes("!<arch>\n"));
    for (const auto& [label, body] : members) {
        std::string head = label;
        head.resize(16, ' ');
        head += "0           0     0     100644  ";
        std::string size = std::to_string(body.size());
        size.resize(10, ' ');
        w.bytes(cb::as_bytes(head + size + "`\n"));
        w.bytes(body);
        if (body.size() & 1) w.u8('\n');
    }
    return w.take();
}

std::vector<std::string> names(const lib::Library& l) {
    std::vector<std::string> out;
    for (const Function& f : l.functions) out.push_back(f.name);
    return out;
}

}  // namespace

CB_TEST("symbols.libraries.read.kinds_of_file", [] {
    const lib::Library vxd = lib::read_file(path("libvxd.lib"));
    CB_CHECK(vxd.machine == Machine::X86_16);
    CB_CHECK_EQ(vxd.functions.size(), std::size_t{13});
    CB_CHECK_EQ(vxd.functions[0].name, "_VmmHeapAllocate");

    const lib::Library atof = lib::read_file(path("atof.obj"));
    CB_CHECK(atof.machine == Machine::X86_32);
    CB_CHECK_EQ(atof.functions.size(), std::size_t{1});

    const lib::Library ppc = lib::read_file(path("xhttp.obj"));
    CB_CHECK(ppc.machine == Machine::PowerPc);
    CB_CHECK_EQ(ppc.functions.size(), std::size_t{8});

    const lib::Library imports = lib::read_file(path("lz32.lib"));   // a COFF archive
    CB_CHECK(imports.machine == Machine::X86_32);
    CB_CHECK_EQ(imports.functions.size(), std::size_t{12});
});

CB_TEST("symbols.libraries.read.archives", [] {
    const Bytes x86 = data("atof.obj");
    const Bytes ppc = data("xhttp.obj");
    // the machine is that of the first object with a function in it
    CB_CHECK(lib::read(archive({{"a.obj/", ppc}})).machine == Machine::PowerPc);
    CB_CHECK(lib::read(archive({{"a.obj/", x86}})).machine == Machine::X86_32);
    const lib::Library mixed = lib::read(archive({{"a.obj/", x86}, {"b.obj/", ppc}}));
    CB_CHECK(mixed.machine == Machine::X86_32);
    CB_CHECK_EQ(mixed.functions.size(), std::size_t{9});
    CB_CHECK_EQ(mixed.functions[0].name, "_atof");
    CB_CHECK_EQ(mixed.functions[1].name, "XHttpSetCredentials");
    // an object that is damaged gives nothing; the others are still read
    const Bytes cut(x86.begin(), x86.begin() + 150);
    const lib::Library some = lib::read(archive({{"a.obj/", cut}, {"b.obj/", ppc}, {"c.obj/", Bytes(5, 7)}}));
    CB_CHECK(some.machine == Machine::PowerPc);
    CB_CHECK_EQ(some.functions.size(), std::size_t{8});
    // nothing in it: a 32-bit library with no functions
    const lib::Library empty = lib::read(cb::as_bytes("!<arch>\n"));
    CB_CHECK(empty.machine == Machine::X86_32);
    CB_CHECK(empty.functions.empty());
});

CB_TEST("symbols.libraries.read.other_data", [] {
    const lib::Library none = lib::read({});
    CB_CHECK(none.machine == Machine::X86_16);   // anything that is not COFF is read as OMF, and has no module
    CB_CHECK(none.functions.empty());
    CB_CHECK(lib::read(Bytes(100, 0x20)).functions.empty());
    // one COFF object that cannot be read is an error; there is nothing else in it
    const Bytes x86 = data("atof.obj");
    CB_CHECK_THROWS_AS(cb::FormatError, (void)lib::read(ByteView(x86.data(), 150)));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)lib::read_file(path("no_such.lib")));
});

CB_TEST("symbols.libraries.fits.library_function_in_a_program", [] {
    const lib::Library vxd = lib::read_file(path("libvxd.lib"));
    const Function& f = vxd.functions[0];   // _VmmHeapAllocate: 28 bytes, nothing left out
    Bytes program = f.code;
    program.insert(program.end(), {0x90, 0x90, 0x90});   // what follows it
    CB_CHECK(lib::fits(f, program));
    program[5] ^= 0xFF;
    CB_CHECK(!lib::fits(f, program));
    CB_CHECK(!lib::fits(f, ByteView(f.code.data(), f.code.size() - 1)));   // shorter than the function: not it

    // atof: four 32-bit fields are the linker's; a program may hold any address there
    const lib::Library atof_object = lib::read_file(path("atof.obj"));
    const Function& atof = atof_object.functions[0];
    Bytes linked = atof.code;
    for (const std::size_t at : {19u, 33u, 52u, 93u}) {
        linked[at] = 0x11;
        linked[at + 1] = 0x22;
        linked[at + 3] = 0x44;
    }
    CB_CHECK(lib::fits(atof, linked));
    linked[10] ^= 1;   // a byte of its own
    CB_CHECK(!lib::fits(atof, linked));
    // ...unless the program's own mask says the loader filled that byte in too
    Bytes theirs(atof.code.size(), 0);
    theirs[10] = 1;
    CB_CHECK(lib::fits(atof, linked, theirs));
    // a mask shorter than the code leaves nothing out past its end
    linked[10] ^= 1;   // (back as it was)
    linked[50] ^= 1;
    CB_CHECK(!lib::fits(atof, linked, theirs));   // theirs leaves out byte 10 only
    theirs[50] = 1;
    CB_CHECK(lib::fits(atof, linked, theirs));
    theirs.resize(30);
    CB_CHECK(!lib::fits(atof, linked, theirs));   // byte 50 lies past the mask
});

CB_TEST("symbols.libraries.index.finds_functions_by_their_rarest_pair", [] {
    constexpr std::uint32_t kBl = 0x48000001;
    constexpr std::uint32_t kBlr = 0x4E800020;
    // two copies of a five-word function (the third word a call whose distance differs), a decoy that shares its first
    // pair, and a prologue pair that is everywhere
    const std::vector<std::uint32_t> program = {0x7C0802A6, 0x9421FFF0, 0x7C0802A6, 0x9421FFF0, 0x38600001,
                                                kBl | 0x100,  0x38800002, kBlr,
                                                0x7C0802A6, 0x9421FFF0, 0x38600001, kBl | 0x2000, 0x38800002, kBlr,
                                                0x7C0802A6, 0x9421FFF0, 0x38600009};
    const lib::Index index(program);
    const std::vector<std::uint32_t> sig = {0x7C0802A6, 0x9421FFF0, 0x38600001, kBl, 0x38800002, kBlr};
    const std::vector<std::uint32_t> ignore = {0, 0, 0, 0x03FFFFFC, 0, 0};
    CB_CHECK((index.find(sig, ignore) == std::vector<std::uint32_t>{2, 8}));   // in order of position
    // the call's distance is compared when it is not left out
    CB_CHECK(index.find(sig, std::vector<std::uint32_t>(6, 0)).empty());
    // a pair the program does not have rules the function out at once
    CB_CHECK(index.find(std::vector<std::uint32_t>{1, 2, 3}, std::vector<std::uint32_t>{0, 0, 0}).empty());
    // nothing to look it up by: every neighbouring pair has a word left out
    CB_CHECK(index.find(sig, std::vector<std::uint32_t>(6, 0x0000FFFF)).empty());
    // a function that runs past the end of the program is not there
    const std::vector<std::uint32_t> tail = {0x7C0802A6, 0x9421FFF0, 0x38600009, 0x4E800020};
    CB_CHECK(index.find(tail, std::vector<std::uint32_t>(4, 0)).empty());
    // the words and what is left out are as many
    CB_CHECK_THROWS_AS(cb::UsageError, (void)index.find(sig, std::vector<std::uint32_t>(5, 0)));
    // an empty program has nothing in it
    const std::vector<std::uint32_t> none;
    CB_CHECK(lib::Index(none).find(sig, ignore).empty());
});

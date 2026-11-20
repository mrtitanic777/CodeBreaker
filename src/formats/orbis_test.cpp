// cb-unity: alone
#include "formats/orbis.hpp"

#include <string>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/orbis_test_image.hpp"

namespace cb::formats::orbis {
namespace {

namespace ot = orbis_test;

}  // namespace

CB_TEST("formats.orbis.an_elf_is_read_with_its_imports_and_relocations", [] {
    const Program p = parse(ot::elf());
    CB_CHECK(!p.self);
    CB_CHECK(!p.orbis);
    CB_CHECK_EQ(p.container, "ELF");
    CB_CHECK_EQ(p.entry, ot::kStart);
    CB_CHECK_EQ(p.segments.size(), 3u);
    CB_CHECK(p.segments[0].code());
    CB_CHECK(!p.segments[1].code());
    CB_CHECK_EQ(p.needed.size(), 1u);
    CB_CHECK_EQ(p.needed[0], "libkernel.sprx");
    CB_CHECK_EQ(p.relocations.size(), 2u);
    CB_CHECK_EQ(p.imports.size(), 1u);
    CB_CHECK_EQ(p.imports[0].name, "sceKernelUsleep");
    CB_CHECK_EQ(p.imports[0].slot, ot::kGot);
    CB_CHECK_EQ(p.import_at(ot::kGot), "sceKernelUsleep");
    CB_CHECK_EQ(p.import_at(ot::kGot + 8), "");
    CB_CHECK(is_playstation(p));
    CB_CHECK(p.in_code(0x1000));
    CB_CHECK(!p.in_code(0x2000));
    CB_CHECK_EQ(p.offset_of(0x2010).value_or(0), 0x2010u);
    CB_CHECK_EQ(p.image_end(), 0x2200u);
});

CB_TEST("formats.orbis.a_fake_self_gives_back_the_elf_it_holds", [] {
    const Bytes elf = ot::elf();
    const Bytes self = ot::fself(elf);
    CB_CHECK(kind_of(ByteView(self).first(64)) == Kind::Fself);
    const Program p = parse(self);
    CB_CHECK(p.self);
    CB_CHECK_EQ(p.container, "fake SELF");
    CB_CHECK_EQ(p.program_type, 1u);
    CB_CHECK(p.file == ot::elf_of_self(elf));
    CB_CHECK_EQ(p.imports.size(), 1u);
    CB_CHECK_EQ(p.imports[0].name, "sceKernelUsleep");
    CB_CHECK_EQ(p.entry, ot::kStart);
    CB_CHECK(is_playstation(p));
});

CB_TEST("formats.orbis.refuses_what_it_cannot_read", [] {
    const Bytes elf = ot::elf();
    CB_CHECK_THROWS_AS(FormatError, (void)parse(ot::fself(elf, 4)));                 // a retail program type: signed and encrypted
    CB_CHECK_THROWS_AS(FormatError, (void)parse(ot::fself(elf, 1, 8)));              // a compressed segment
    CB_CHECK_THROWS_AS(FormatError, (void)parse(ot::fself(elf, 1, 2)));              // an encrypted one
    Bytes ps5 = {0x54, 0x14, 0xF5, 0xEE, 0, 0, 0, 0};
    ps5.resize(0x100, 0);
    CB_CHECK(kind_of(ps5) == Kind::Ps5Self);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(ps5));
    Bytes pkg = {0x7F, 'C', 'N', 'T'};
    pkg.resize(0x100, 0);
    CB_CHECK(kind_of(pkg) == Kind::Pkg);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(pkg));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(100, 0x55)));
    Bytes i386 = elf;
    i386[18] = 3;   // an x86 ELF is not this
    CB_CHECK_THROWS_AS(FormatError, (void)parse(i386));
});

CB_TEST("formats.orbis.damaged_files_throw_and_never_overrun", [] {
    const Bytes elf = ot::elf();
    const Bytes self = ot::fself(elf);
    for (const Bytes* original : {&elf, &self}) {
        for (std::size_t n = 0; n < original->size(); n += 37) {
            try {
                (void)parse(ByteView(*original).first(n));
            } catch (const Error&) {   // (a refusal, as it should be)
            }
        }
        // a program-header table that claims too much, a dynamic table pointing past the file
        Bytes bad = *original;
        if (original == &elf) {
            ot::put16(bad, 56, 0xFFFF);
            try {
                (void)parse(bad);
            } catch (const Error&) {
            }
            bad = *original;
            ot::put64(bad, 0x2040 + 16 + 8, 0xFFFFFFF0);   // the string table far past the end
            try {
                (void)parse(bad);
            } catch (const Error&) {
            }
        }
    }
});

CB_TEST("formats.orbis.probe_names_the_console_and_leaves_linux_alone", [] {
    fs::ScopedTempDir dir("cb_orbis_probe");
    fs::write_file(dir.path() / "tool.elf", ot::elf());
    fs::write_file(dir.path() / "tool-ps5.elf", ot::elf());
    fs::write_file(dir.path() / "linux.elf", ot::elf("libc.so.6"));
    fs::write_file(dir.path() / "eboot.bin", ot::fself(ot::elf()));
    fs::write_file(dir.path() / "text.txt", Bytes(100, 'a'));
    CB_CHECK(probe(dir.path() / "tool.elf", Console::Ps4));
    CB_CHECK(!probe(dir.path() / "tool.elf", Console::Ps5));
    CB_CHECK(probe(dir.path() / "tool-ps5.elf", Console::Ps5));
    CB_CHECK(!probe(dir.path() / "tool-ps5.elf", Console::Ps4));
    CB_CHECK(!probe(dir.path() / "linux.elf", Console::Ps4));
    CB_CHECK(!probe(dir.path() / "linux.elf", Console::Ps5));
    CB_CHECK(probe(dir.path() / "eboot.bin", Console::Ps4));
    CB_CHECK(!probe(dir.path() / "text.txt", Console::Ps4));
    CB_CHECK(!probe(dir.path() / "missing.elf", Console::Ps4));
});

CB_TEST("formats.orbis.nids_are_the_hash_of_the_name", [] {
    CB_CHECK_EQ(nid_of("sceKernelUsleep"), "1jfXLRVzisc");
    CB_CHECK_EQ(nid_of("open"), "wuCroIGjt2g");
    CB_CHECK_EQ(nid_of("close"), "bY-PO6JhzhQ");
    CB_CHECK_EQ(function_name("1jfXLRVzisc"), "sceKernelUsleep");
    CB_CHECK_EQ(function_name("AAAAAAAAAAA"), "");
    CB_CHECK_EQ(syscall_name(4), "write");
    CB_CHECK_EQ(syscall_name(0xFFFF), "");
});

}  // namespace cb::formats::orbis

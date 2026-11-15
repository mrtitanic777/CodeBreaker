#include "cb/exact/emu/dos.hpp"

#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::exact::emu;
namespace stdfs = std::filesystem;

// The arena is the one the executive keeps in guest RAM, here over an `Engine`; the cases
// are laid out by hand and read through for what DOS does with the chain. The files are a MemFS. The executive
// (native/dosexec) runs the way the verify lane runs it: set CB_CBDOS_DLL to run the cases that need cbdos.dll.
//
// Design notes: the one process-wide cache of the tools' files is a `DiskCache` a session shares; a run that
// the executive stops (the block cap, the time limit) is an EmulationError;
// the `-` hooks (instrument, instrument_when, driver) and a DPMI host object handed in by the caller have no counterpart - a run asks for
// the DPMI host with `RunSpec::dpmi`.

namespace {

// 1 MB of guest memory, nothing else.
class DosTestMemory final : public dpmi::Engine {
public:
    DosTestMemory() : ram_(0x100000, 0) {}
    std::uint32_t reg(dpmi::Reg) override { return 0; }
    void set_reg(dpmi::Reg, std::uint32_t) override {}
    void set_table(dpmi::TableReg, std::uint16_t, std::uint32_t, std::uint32_t, std::uint32_t) override {}
    Bytes read(std::uint32_t address, std::size_t size) override { return Bytes(ram_.begin() + address, ram_.begin() + address + size); }
    void write(std::uint32_t address, ByteView data) override { std::copy(data.begin(), data.end(), ram_.begin() + address); }
    void map_ram(std::uint32_t, std::uint64_t) override {}
    void stop() override {}

private:
    Bytes ram_;
};

constexpr std::uint16_t kDosTestFirst = 0x100;

// H0 @0x100 DOS's (0x10), H1 @0x111 free (0x20), H2 @0x132 owned by 0x500 (0x10), H3 @0x143 free (0x100), the last.
void dos_test_lay_out(DosTestMemory& m) {
    dos::write_mcb(m, 0x100, 'M', dos::kOwnerDos, 0x10);
    dos::write_mcb(m, 0x111, 'M', dos::kOwnerFree, 0x20);
    dos::write_mcb(m, 0x132, 'M', 0x500, 0x10);
    dos::write_mcb(m, 0x143, 'Z', dos::kOwnerFree, 0x100);
}

}  // namespace

CB_TEST("exact.emu.dos.the_chain_is_read_from_the_headers", [] {
    DosTestMemory m;
    dos_test_lay_out(m);
    const std::vector<dos::Mcb> chain = dos::mcb_chain(m, kDosTestFirst);
    CB_CHECK_EQ(chain.size(), 4u);
    CB_CHECK((chain[1] == dos::Mcb{0x111, 'M', 0, 0x20}));
    CB_CHECK((chain[2] == dos::Mcb{0x132, 'M', 0x500, 0x10}));
    CB_CHECK_EQ(chain[3].signature, 'Z');
    CB_CHECK_EQ(dos::largest(m, kDosTestFirst), 0x100);
    // a header is a paragraph: the name and the rest are cleared when one is written
    CB_CHECK((m.read(0x111 * 16 + 5, 11) == Bytes(11, 0)));
    // a place that holds no header
    CB_CHECK_THROWS_AS(dos::ArenaError, (void)dos::read_mcb(m, 0x1000));
    try {
        (void)dos::read_mcb(m, 0x1000);
    } catch (const dos::ArenaError& e) {
        CB_CHECK_EQ(e.code(), 7);
    }
});

CB_TEST("exact.emu.dos.a_block_is_chosen_by_the_strategy", [] {
    {   // first fit: the first free block that is large enough, split
        DosTestMemory m;
        dos_test_lay_out(m);
        CB_CHECK_EQ(*dos::allocate(m, kDosTestFirst, 0x10, 0x600), 0x112);
        const auto chain = dos::mcb_chain(m, kDosTestFirst);
        CB_CHECK((chain[1] == dos::Mcb{0x111, 'M', 0x600, 0x10}));
        CB_CHECK((chain[2] == dos::Mcb{0x122, 'M', 0, 0x0F}));   // (what is left, a free block behind it)
    }
    {   // best fit: the smallest that fits (the 0x20 one, not the 0x100 one)
        DosTestMemory m;
        dos_test_lay_out(m);
        CB_CHECK_EQ(*dos::allocate(m, kDosTestFirst, 0x08, 0x600, dos::Strategy::Best), 0x112);
    }
    {   // last fit: the top of the last free block
        DosTestMemory m;
        dos_test_lay_out(m);
        CB_CHECK_EQ(*dos::allocate(m, kDosTestFirst, 0x10, 0x600, dos::Strategy::Last), 0x234);
        const auto chain = dos::mcb_chain(m, kDosTestFirst);
        CB_CHECK((chain[3] == dos::Mcb{0x143, 'M', 0, 0xEF}));
        CB_CHECK((chain[4] == dos::Mcb{0x233, 'Z', 0x600, 0x10}));
    }
    {   // nothing that large
        DosTestMemory m;
        dos_test_lay_out(m);
        CB_CHECK(!dos::allocate(m, kDosTestFirst, 0x200, 0x600).has_value());
    }
});

CB_TEST("exact.emu.dos.freed_neighbours_are_one_free_block", [] {
    DosTestMemory m;
    dos_test_lay_out(m);
    dos::free_block(m, kDosTestFirst, 0x133);   // H2's block: the free blocks around it join
    const auto chain = dos::mcb_chain(m, kDosTestFirst);
    CB_CHECK_EQ(chain.size(), 2u);
    CB_CHECK((chain[1] == dos::Mcb{0x111, 'Z', 0, 0x132}));
    CB_CHECK_EQ(dos::largest(m, kDosTestFirst), 0x132);
    // an address that is no block, and one before the first
    CB_CHECK_THROWS_AS(dos::ArenaError, dos::free_block(m, kDosTestFirst, 0x2000));
    CB_CHECK_THROWS_AS(dos::ArenaError, dos::free_block(m, kDosTestFirst, 0));
    try {
        dos::free_block(m, kDosTestFirst, 0x2000);
    } catch (const dos::ArenaError& e) {
        CB_CHECK_EQ(e.code(), 9);
    }
});

CB_TEST("exact.emu.dos.a_block_is_resized_in_place", [] {
    {   // smaller: the rest is free, and joins the free block behind
        DosTestMemory m;
        dos_test_lay_out(m);
        CB_CHECK(!dos::resize(m, kDosTestFirst, 0x133, 0x08).has_value());
        const auto chain = dos::mcb_chain(m, kDosTestFirst);
        CB_CHECK((chain[2] == dos::Mcb{0x132, 'M', 0x500, 0x08}));
        CB_CHECK((chain[3] == dos::Mcb{0x13B, 'Z', 0, 0x108}));
    }
    {   // larger: into the free block behind
        DosTestMemory m;
        dos_test_lay_out(m);
        CB_CHECK(!dos::resize(m, kDosTestFirst, 0x133, 0x20).has_value());
        const auto chain = dos::mcb_chain(m, kDosTestFirst);
        CB_CHECK((chain[2] == dos::Mcb{0x132, 'M', 0x500, 0x20}));
        CB_CHECK((chain[3] == dos::Mcb{0x153, 'Z', 0, 0xF0}));
    }
    {   // too large: the most it could have
        DosTestMemory m;
        dos_test_lay_out(m);
        CB_CHECK_EQ(*dos::resize(m, kDosTestFirst, 0x133, 0x200), 0x111);
        // and a block that is none
        CB_CHECK_THROWS_AS(dos::ArenaError, (void)dos::resize(m, kDosTestFirst, 0x2000, 1));
    }
});

CB_TEST("exact.emu.dos.a_damaged_arena_has_no_room", [] {
    DosTestMemory m;
    dos_test_lay_out(m);
    m.write(0x143 * 16, Bytes{0x00});   // the last header's signature gone: the chain does not end
    CB_CHECK_THROWS_AS(dos::ArenaError, (void)dos::mcb_chain(m, kDosTestFirst));
    CB_CHECK(!dos::allocate(m, kDosTestFirst, 1, 0x600).has_value());
    CB_CHECK_EQ(dos::largest(m, kDosTestFirst), 0);
});

CB_TEST("exact.emu.dos.a_file_of_the_memfs_is_a_buffer", [] {
    dos::MemFS fs;
    fs::ScopedTempDir dir("dostest");
    const stdfs::path p = dir.path() / "A.TXT";
    dos::MemFile f = fs.open(p, true);
    CB_CHECK_EQ(f.write(as_bytes("hello")), 5u);
    CB_CHECK_EQ(f.tell(), 5u);
    CB_CHECK_EQ(f.seek(0), 0u);
    CB_CHECK_EQ(as_text(f.read(2)), "he");
    CB_CHECK_EQ(as_text(f.read()), "llo");
    CB_CHECK_EQ(f.read().size(), 0u);                          // (at the end)
    CB_CHECK_EQ(f.seek(-2, 2), 3u);                            // from the end
    CB_CHECK_EQ(f.seek(-100, 1), 0u);                          // never below the start
    CB_CHECK_EQ(f.seek(8), 8u);                                // past the end: a write fills the gap with zeros
    f.write(as_bytes("!"));
    CB_CHECK((fs.read(p) == Bytes{'h', 'e', 'l', 'l', 'o', 0, 0, 0, '!'}));
    f.seek(2);
    f.truncate();                                              // at the position
    CB_CHECK_EQ(as_text(fs.read(p)), "he");
    f.truncate(1);
    CB_CHECK_EQ(as_text(fs.read(p)), "h");
    f.truncate(50);                                            // (a size beyond the end changes nothing)
    CB_CHECK_EQ(fs.size(p), 1u);
});

CB_TEST("exact.emu.dos.the_memfs_shows_the_disk_until_a_file_is_written", [] {
    CB_WINDOWS_ONLY("it relies on the case-insensitive file names of the disk");
    fs::ScopedTempDir dir("dostest");
    fs::write_text(dir.path() / "ON.DISK", "disk");
    dos::MemFS fs;
    const stdfs::path on_disk = dir.path() / "on.disk";   // (the system's names are not case sensitive; neither are the keys)
    CB_CHECK(fs.is_file(on_disk));
    CB_CHECK_EQ(as_text(fs.read(on_disk)), "disk");
    CB_CHECK_EQ(fs.size(on_disk), 4u);
    dos::MemFile f = fs.open(on_disk, false);                    // brought in from the disk
    CB_CHECK_EQ(as_text(f.read()), "disk");
    f.seek(0);
    f.write(as_bytes("MEM"));
    CB_CHECK_EQ(as_text(fs::read_file(dir.path() / "ON.DISK")), "disk");   // (no file is written to the disk)
    CB_CHECK_EQ(as_text(fs.read(on_disk)), "MEMk");
    // removed: gone from the view, still on the disk
    fs.remove(on_disk);
    CB_CHECK(!fs.is_file(on_disk));
    CB_CHECK_THROWS_AS(NotFoundError, fs.remove(on_disk));
    CB_CHECK_THROWS_AS(NotFoundError, (void)fs.open(on_disk, false));
    // a file never there
    CB_CHECK_THROWS_AS(NotFoundError, (void)fs.read(dir.path() / "NONE"));
    CB_CHECK_THROWS_AS(NotFoundError, (void)fs.size(dir.path() / "NONE"));
    // put makes it visible again; replace moves it
    fs.put(on_disk, as_bytes("again"));
    CB_CHECK(fs.is_file(on_disk));
    fs.replace(on_disk, dir.path() / "NEW.NAME");
    CB_CHECK(!fs.is_file(on_disk));
    CB_CHECK_EQ(as_text(fs.read(dir.path() / "NEW.NAME")), "again");
    CB_CHECK_EQ(fs.files().size(), 1u + 0u);
});

CB_TEST("exact.emu.dos.hidden_folders_show_nothing_of_the_disk", [] {
    fs::ScopedTempDir dir("dostest");
    stdfs::create_directories(dir.path() / "inc");
    fs::write_text(dir.path() / "inc" / "X.H", "x");
    fs::write_text(dir.path() / "Y.H", "y");
    dos::MemFS fs({dir.path() / "inc"});
    CB_CHECK(!fs.is_file(dir.path() / "inc" / "X.H"));   // (what the run was given whole is in the MemFS, not on the disk)
    CB_CHECK(fs.is_file(dir.path() / "Y.H"));
    fs.put(dir.path() / "inc" / "X.H", as_bytes("given"));
    CB_CHECK(fs.is_file(dir.path() / "inc" / "X.H"));
    CB_CHECK_EQ(fs.hidden().size(), 1u);
});

CB_TEST("exact.emu.dos.the_disk_cache_reads_a_changed_file_again", [] {
    fs::ScopedTempDir dir("dostest");
    const stdfs::path p = dir.path() / "TOOL.EXE";
    fs::write_text(p, "one");
    auto cache = std::make_shared<dos::DiskCache>();
    dos::MemFS first({}, cache), second({}, cache);
    CB_CHECK_EQ(as_text(first.read(p)), "one");
    fs::write_text(p, "three");   // (another size: read again)
    CB_CHECK_EQ(as_text(second.read(p)), "three");
    CB_CHECK_THROWS_AS(NotFoundError, (void)cache->read(dir.path() / "gone"));
});

CB_TEST("exact.emu.dos.the_summary_line", [] {
    dos::Result r;
    r.exit = 3;
    r.steps = 12;
    r.blocks = {1, 2, 3};
    CB_CHECK_EQ(dos::Executive::summary(r), "[exit 3, 12 basic blocks executed, 3 distinct]");
    r.exit.reset();
    CB_CHECK_EQ(dos::Executive::summary(r), "[exit none, 12 basic blocks executed, 3 distinct]");
});

CB_TEST("exact.emu.dos.an_executive_needs_its_files", [] {
    CB_CHECK_THROWS_AS(NotFoundError, (void)dos::Executive::load("C:/no/such/cbdos.dll", "C:/no/such/unicorn.dll"));
});

// The real executive: CB_CBDOS_DLL names cbdos.dll (CB_UNICORN_DLL the unicorn.dll; the one beside the sources by default).
CB_TEST("exact.emu.dos.the_executive_runs_a_program_and_hands_files_back", [] {
    const auto dll = fs::get_env("CB_CBDOS_DLL");
    if (!dll) return;
    const auto unicorn = fs::get_env("CB_UNICORN_DLL");
    const auto executive = dos::Executive::load(fs::from_text(*dll), unicorn ? fs::from_text(*unicorn) : testing::data_path("../third_party/bin/unicorn.dll"));
    fs::ScopedTempDir dir("dostest");
    dos::RunSpec spec;
    spec.exe = testing::data_path("port/testdata/verify/hello.exe");
    spec.workdir = dir.path();
    spec.fast = true;
    dos::MemFS files;
    spec.files = &files;
    const dos::Result r = executive->run(spec);
    CB_CHECK_EQ(as_text(r.console), "Hello from DOS\r\n");
    CB_CHECK(r.exit.has_value());
    CB_CHECK(r.blocks.empty());   // (a fast run keeps no blocks)
    spec.fast = false;
    const dos::Result slow = executive->run(spec);
    CB_CHECK(!slow.blocks.empty());
    CB_CHECK_EQ(slow.steps >= slow.blocks.size(), true);
    // a program that is not there
    spec.exe = dir.path() / "NONE.EXE";
    CB_CHECK_THROWS_AS(Error, (void)executive->run(spec));
});

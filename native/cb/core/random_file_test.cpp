#include "cb/core/random_file.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::fs {
namespace {

CB_TEST("core.random_file.create_write_read_at_offsets", [] {
    ScopedTempDir dir("rf-test");
    const auto path = dir.path() / "deep" / "disk.bin";   // the parent is made
    {
        RandomFile f(path, FileMode::Create);
        CB_CHECK_EQ(f.size(), 0u);
        f.write_at(0x1000, Bytes{1, 2, 3, 4});                // past the end: zero-filled before
        CB_CHECK_EQ(f.size(), 0x1004u);
        f.write_at(2, Bytes{9, 9});
        CB_CHECK((f.read_at(0, 6) == Bytes{0, 0, 9, 9, 0, 0}));
        CB_CHECK((f.read_at(0x1002, 100) == Bytes{3, 4}));    // fewer at the end
        CB_CHECK(f.read_at(0x1004, 10).empty());              // none at the end
        CB_CHECK(f.read_at(0x5000, 10).empty());              // none past it
        CB_CHECK_EQ(f.read_at(0, 0).size(), 0u);
        f.flush();
    }
    CB_CHECK_EQ(fs::file_size(path), 0x1004u);
});

CB_TEST("core.random_file.offsets_past_4gb", [] {
    ScopedTempDir dir("rf-test");
    RandomFile f(dir.path() / "big.bin", FileMode::Create);
    const std::uint64_t at = (std::uint64_t{1} << 32) + 7;
    f.write_at(at, Bytes{0xAB, 0xCD});
    CB_CHECK_EQ(f.size(), at + 2);
    CB_CHECK((f.read_at(at, 2) == Bytes{0xAB, 0xCD}));
    CB_CHECK((f.read_at(at - 1, 2) == Bytes{0, 0xAB}));
});

CB_TEST("core.random_file.modes", [] {
    ScopedTempDir dir("rf-test");
    const auto path = dir.path() / "f.bin";
    write_file(path, Bytes{1, 2, 3, 4, 5});
    {
        RandomFile r(path, FileMode::Read);
        CB_CHECK_EQ(r.size(), 5u);
        CB_CHECK((r.read_at(1, 3) == Bytes{2, 3, 4}));
        CB_CHECK_THROWS_AS(IoError, r.write_at(0, Bytes{7}));
        CB_CHECK(r.mode() == FileMode::Read);
    }
    {
        RandomFile rw(path, FileMode::ReadWrite);
        rw.write_at(1, Bytes{8, 8});
    }
    CB_CHECK((read_file(path) == Bytes{1, 8, 8, 4, 5}));
    {
        RandomFile c(path, FileMode::Create);   // an existing file is emptied
        CB_CHECK_EQ(c.size(), 0u);
    }
    CB_CHECK_THROWS_AS(NotFoundError, RandomFile(dir.path() / "missing.bin", FileMode::Read));
    CB_CHECK_THROWS_AS(NotFoundError, RandomFile(dir.path() / "missing.bin", FileMode::ReadWrite));
});

CB_TEST("core.random_file.move", [] {
    ScopedTempDir dir("rf-test");
    RandomFile a(dir.path() / "m.bin", FileMode::Create);
    a.write_at(0, Bytes{5});
    RandomFile b = std::move(a);
    CB_CHECK((b.read_at(0, 1) == Bytes{5}));
    RandomFile c(dir.path() / "n.bin", FileMode::Create);
    c = std::move(b);
    CB_CHECK((c.read_at(0, 1) == Bytes{5}));
});

}  // namespace
}  // namespace cb::fs

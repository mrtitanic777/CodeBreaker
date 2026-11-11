#include "cb/core/fs.hpp"

#include "cb/core/testing.hpp"

using namespace cb;
namespace stdfs = std::filesystem;

CB_TEST("core.fs.write_read_roundtrip", [] {
    fs::ScopedTempDir dir("cbfs");
    const stdfs::path file = dir.path() / "sub" / "deeper" / "data.bin";   // parents are created
    const Bytes data = {0, 1, 2, 255, 0, 7};
    fs::write_file(file, data);
    CB_CHECK(fs::read_file(file) == data);
    CB_CHECK_EQ(fs::file_size(file), 6u);
    fs::write_text(dir.path() / "t.txt", "hello\n");
    CB_CHECK_EQ(fs::read_text(dir.path() / "t.txt"), "hello\n");
    fs::write_file(file, Bytes{});                      // truncates
    CB_CHECK_EQ(fs::read_file(file).size(), 0u);
});

CB_TEST("core.fs.errors_have_the_right_type", [] {
    fs::ScopedTempDir dir("cbfs");
    CB_CHECK_THROWS_AS(NotFoundError, fs::read_file(dir.path() / "missing.bin"));
    CB_CHECK_THROWS_AS(NotFoundError, fs::read_text(dir.path() / "nodir" / "missing.bin"));
    CB_CHECK_THROWS_AS(IoError, fs::file_size(dir.path() / "missing.bin"));
    CB_CHECK_THROWS_AS(IoError, fs::read_file(dir.path()));   // a directory is not a file
    CB_CHECK_THROWS_AS(NotFoundError, fs::MappedFile(dir.path() / "missing.bin"));
    try {
        (void)fs::read_file(dir.path() / "missing.bin");
    } catch (const NotFoundError& e) {
        CB_CHECK_EQ(e.exit_code(), kExitNotFound);
        CB_CHECK_EQ(e.os_code(), 2);
    }
});

CB_TEST("core.fs.atomic_write_replaces", [] {
    fs::ScopedTempDir dir("cbfs");
    const stdfs::path file = dir.path() / "state.json";
    fs::write_file_atomic(file, as_bytes("first"));
    fs::write_file_atomic(file, as_bytes("second, longer"));
    CB_CHECK_EQ(fs::read_text(file), "second, longer");
    size_t entries = 0;
    for (const auto& e : stdfs::directory_iterator(dir.path())) { (void)e; ++entries; }
    CB_CHECK_EQ(entries, 1u);   // no temp file left behind
});

CB_TEST("core.fs.mapped_file", [] {
    fs::ScopedTempDir dir("cbfs");
    Bytes data(100000);
    for (size_t i = 0; i < data.size(); ++i) data[i] = static_cast<std::uint8_t>(i * 7);
    fs::write_file(dir.path() / "big.bin", data);
    fs::MappedFile m(dir.path() / "big.bin");
    CB_CHECK_EQ(m.view().size(), data.size());
    CB_CHECK(equal_bytes(m.view(), data));
    fs::MappedFile moved(std::move(m));
    CB_CHECK_EQ(moved.view().size(), data.size());
    CB_CHECK_EQ(m.view().size(), 0u);   // NOLINT: a moved-from mapping is empty
    fs::write_file(dir.path() / "empty.bin", Bytes{});
    fs::MappedFile e(dir.path() / "empty.bin");
    CB_CHECK_EQ(e.view().size(), 0u);
});

CB_TEST("core.fs.scoped_temp_dir_cleans_up", [] {
    stdfs::path kept;
    {
        fs::ScopedTempDir dir("cbfs");
        kept = dir.path();
        fs::write_text(kept / "a" / "b.txt", "x");
        CB_CHECK(stdfs::is_directory(kept));
    }
    CB_CHECK(!stdfs::exists(kept));
    // also on an exception
    stdfs::path thrown;
    try {
        fs::ScopedTempDir dir("cbfs");
        thrown = dir.path();
        fs::write_text(thrown / "x.txt", "x");
        throw UsageError("boom");
    } catch (const UsageError&) {
    }
    CB_CHECK(!stdfs::exists(thrown));
    // release keeps it
    stdfs::path released;
    {
        fs::ScopedTempDir dir("cbfs");
        released = dir.release();
    }
    CB_CHECK(stdfs::is_directory(released));
    fs::remove_tree(released);
    CB_CHECK(!stdfs::exists(released));
});

CB_TEST("core.fs.remove_tree_handles_read_only", [] {
    fs::ScopedTempDir dir("cbfs");
    const stdfs::path file = dir.path() / "tree" / "ro.txt";
    fs::write_text(file, "locked");
    stdfs::permissions(file, stdfs::perms::owner_read, stdfs::perm_options::replace);   // read-only attribute
    CB_CHECK(fs::remove_tree(dir.path() / "tree") >= 1u);
    CB_CHECK(!stdfs::exists(dir.path() / "tree"));
    CB_CHECK_EQ(fs::remove_tree(dir.path() / "never-existed"), 0u);
});

CB_TEST("core.fs.copy_tree", [] {
    fs::ScopedTempDir dir("cbfs");
    fs::write_text(dir.path() / "src" / "a.txt", "A");
    fs::write_text(dir.path() / "src" / "d" / "b.txt", "B");
    fs::copy_tree(dir.path() / "src", dir.path() / "dst");
    CB_CHECK_EQ(fs::read_text(dir.path() / "dst" / "a.txt"), "A");
    CB_CHECK_EQ(fs::read_text(dir.path() / "dst" / "d" / "b.txt"), "B");
    CB_CHECK_THROWS_AS(NotFoundError, fs::copy_tree(dir.path() / "nothing", dir.path() / "dst2"));
});

CB_TEST("core.fs.unicode_paths", [] {
    fs::ScopedTempDir dir("cbfs");
    const stdfs::path p = dir.path() / fs::from_text("caf\xC3\xA9 \xE4\xBD\xA0.txt");
    fs::write_text(p, "u");
    CB_CHECK_EQ(fs::read_text(p), "u");
    CB_CHECK(fs::to_text(p).find("caf\xC3\xA9 \xE4\xBD\xA0.txt") != std::string::npos);
    CB_CHECK_EQ(fs::to_text(fs::from_text("a/b\\c")), "a/b/c");
});

CB_TEST("core.fs.same_path_and_long_path", [] {
    CB_CHECK(fs::same_path("C:/Windows/System32", "c:\\windows\\system32\\"));
    CB_CHECK(fs::same_path("C:/a/../b", "C:/B"));
    CB_CHECK(!fs::same_path("C:/a", "C:/b"));
    CB_CHECK(fs::long_path("C:/x/y").native().starts_with(L"\\\\?\\C:\\"));
    CB_CHECK(fs::long_path(L"\\\\?\\C:\\already").native() == L"\\\\?\\C:\\already");
    // A path past MAX_PATH is readable and writable.
    fs::ScopedTempDir dir("cbfs");
    stdfs::path deep = dir.path();
    for (int i = 0; i < 12; ++i) deep /= std::string(30, static_cast<char>('a' + i));
    fs::write_text(deep / "f.txt", "long");
    CB_CHECK(deep.native().size() > 260);
    CB_CHECK_EQ(fs::read_text(deep / "f.txt"), "long");
});

CB_TEST("core.fs.scoped_env", [] {
    CB_CHECK(!fs::get_env("CB_TEST_ENV_VAR").has_value());
    {
        fs::ScopedEnv a("CB_TEST_ENV_VAR", "one");
        CB_CHECK_EQ(*fs::get_env("CB_TEST_ENV_VAR"), "one");
        {
            fs::ScopedEnv b("CB_TEST_ENV_VAR", "two\xC3\xA9");
            CB_CHECK_EQ(*fs::get_env("CB_TEST_ENV_VAR"), "two\xC3\xA9");
        }
        CB_CHECK_EQ(*fs::get_env("CB_TEST_ENV_VAR"), "one");
        fs::ScopedEnv c("CB_TEST_ENV_VAR", std::nullopt);
        CB_CHECK(!fs::get_env("CB_TEST_ENV_VAR").has_value());
    }
    CB_CHECK(!fs::get_env("CB_TEST_ENV_VAR").has_value());
});

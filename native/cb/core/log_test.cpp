#include "cb/core/log.hpp"

#include <windows.h>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;

namespace {

// Redirect stdout or stderr to a file for the life of the object.
class Redirect {
public:
    Redirect(DWORD which, const std::filesystem::path& file) : which_(which), saved_(GetStdHandle(which)) {
        SECURITY_ATTRIBUTES sa{sizeof(sa), nullptr, TRUE};
        h_ = CreateFileW(file.c_str(), GENERIC_WRITE, FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        SetStdHandle(which_, h_);
    }
    ~Redirect() {
        flush_out();
        SetStdHandle(which_, saved_);
        CloseHandle(h_);
    }

private:
    DWORD which_;
    HANDLE saved_;
    HANDLE h_;
};

}  // namespace

CB_TEST("core.log.levels_filter", [] {
    fs::ScopedTempDir dir("cblog");
    const auto saved = log::level();
    {
        Redirect r(STD_ERROR_HANDLE, dir.path() / "err.txt");
        log::set_level(log::Level::Warn);
        log::error("boom");
        log::warn("careful");
        log::info("chatter");
        log::debug("noise");
        log::set_level(log::Level::Debug);
        log::debug("now visible");
    }
    log::set_level(saved);
    const std::string got = fs::read_text(dir.path() / "err.txt");
    CB_CHECK_EQ(got, "error: boom\nwarn: careful\ndebug: now visible\n");
});

CB_TEST("core.log.level_names", [] {
    const auto saved = log::level();
    CB_CHECK(log::set_level_by_name("DEBUG"));
    CB_CHECK(log::level() == log::Level::Debug);
    CB_CHECK(log::set_level_by_name(" info "));
    CB_CHECK(log::level() == log::Level::Info);
    CB_CHECK(!log::set_level_by_name("loud"));
    CB_CHECK(log::level() == log::Level::Info);   // unchanged by a bad name
    CB_CHECK(log::enabled(log::Level::Error));
    CB_CHECK(!log::enabled(log::Level::Debug));
    log::set_level(saved);
});

CB_TEST("core.log.file_and_timestamps", [] {
    fs::ScopedTempDir dir("cblog");
    fs::ScopedEnv quiet("CB_LOG", "info");
    fs::ScopedEnv stamps("CB_LOG_TIME", "1");
    const auto saved = log::level();
    log::init_from_env();
    CB_CHECK(log::level() == log::Level::Info);
    {
        Redirect r(STD_ERROR_HANDLE, dir.path() / "err.txt");
        log::set_file(dir.path() / "log" / "run.log");
        log::info("to both");
        log::set_file({});
        log::info("stderr only");
    }
    log::set_timestamps(false);
    log::set_level(saved);
    const std::string file = fs::read_text(dir.path() / "log" / "run.log");
    CB_CHECK(file.find("info: to both\n") != std::string::npos);
    CB_CHECK(file.find("stderr only") == std::string::npos);
    CB_CHECK(file.size() > std::string("info: to both\n").size());   // a timestamp precedes the label
    CB_CHECK(fs::read_text(dir.path() / "err.txt").find("stderr only") != std::string::npos);
});

CB_TEST("core.log.out_writes_exact_utf8_to_stdout", [] {
    testing::CapturedStd captured;
    out("{:08x} {}\n", 0xBEEFu, "caf\xC3\xA9");
    out_text("PROGRESS 1 2\n");
    err("not on stdout\n");
    CB_CHECK_EQ(captured.out(), "0000beef caf\xC3\xA9\nPROGRESS 1 2\n");   // \n stays \n; stderr is separate
    CB_CHECK_EQ(captured.err(), "not on stdout\n");
});

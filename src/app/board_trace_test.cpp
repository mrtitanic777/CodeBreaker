// Tests of board_trace.hpp: what counts as a named call, what is only counted, and the kept file.
#include "app/board_trace.hpp"

#include <cmath>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

namespace stdfs = std::filesystem;

InstalledImage bt_image(const char* name, std::uint32_t base, std::uint32_t size, std::unordered_map<std::uint32_t, std::string> exports, std::uint32_t transfer_rva) {
    InstalledImage i;
    i.name = name;
    i.base = base;
    i.size = size;
    i.exports = std::move(exports);
    i.has_transfer = [transfer_rva](std::uint32_t rva) { return rva == transfer_rva; };
    return i;
}

std::vector<InstalledImage> bt_images() {
    std::vector<InstalledImage> v;
    v.push_back(bt_image("user32.dll", 0xBFF60000, 0x10000, {{0x100, "GetMessageA"}}, 0x200));
    v.push_back(bt_image("kernel32.dll", 0xBFF70000, 0x68000, {{0x300, "lstrlen"}}, 0x400));
    v.push_back(bt_image("app.exe", 0x00400000, 0x8000, {}, 0x500));
    v.push_back(bt_image("other.exe", 0x00400000, 0x8000, {}, 0x500));   // the same preferred base, the same instruction
    return v;
}

constexpr const char* kTrace =
    "#calltrace 1\ncycles\t2000\ncpu_hz\t1000\ntransfers\t999\noverflow\t0\n"
    "c\tbff60200\tbff70300\t00000000\t40\t0\t0\t0\t0\t0\t0\n"     // user32 > kernel32!lstrlen: named
    "c\tc0001000\tc0002000\t00000000\t500\t0\t0\t0\t0\t0\t0\n"    // ring 0
    "c\tbff60200\tbff70999\t00000000\t7\t0\t0\t0\t0\t0\t0\n"      // not an export
    "c\tbff60222\tbff70300\t00000000\t5\t0\t0\t0\t0\t0\t0\n"      // no such instruction there
    "c\t00400500\tbff60100\t00001000\t3\t0\t0\t0\t0\t0\t0\n"      // two different files could be the source
    "c\tbff70400\tbff70300\t00000000\t2\t0\t0\t0\t0\t0\t0\n";     // inside one module

CB_TEST("app.board_trace.only_what_is_proved_is_named_and_the_rest_is_counted", [] {
    const std::vector<InstalledImage> images = bt_images();
    const std::optional<Measured> m = read_trace(kTrace, images);
    CB_CHECK(m.has_value());
    CB_CHECK_EQ(m->seconds, 2.0);
    CB_CHECK_EQ(m->transfers, std::uint64_t{999});
    CB_CHECK_EQ(m->named, std::uint64_t{40});
    CB_CHECK_EQ(m->ring0, std::uint64_t{500});
    CB_CHECK_EQ(m->not_export, std::uint64_t{7});
    CB_CHECK_EQ(m->no_source, std::uint64_t{5});
    CB_CHECK_EQ(m->ambiguous, std::uint64_t{3});
    CB_CHECK_EQ(m->inside, std::uint64_t{2});
    CB_CHECK_EQ(m->channels.size(), std::size_t{1});
    CB_CHECK_EQ(m->channels[0].from, std::string("user32.dll"));
    CB_CHECK_EQ(m->channels[0].to, std::string("kernel32.dll"));
    CB_CHECK_EQ(m->functions[0].function, std::string("lstrlen"));
    CB_CHECK_EQ(m->per_hour(40), 72000.0);
});

CB_TEST("app.board_trace.what_is_not_a_trace_is_refused", [] {
    const std::vector<InstalledImage> images = bt_images();
    CB_CHECK(!read_trace("not a trace", images).has_value());
    CB_CHECK(!read_trace("#calltrace 1\ntransfers\t1\n", images).has_value());   // no clock: no rate can be said
});

CB_TEST("app.board_trace.a_measurement_is_kept_and_read_back", [] {
    const stdfs::path root = stdfs::temp_directory_path() / "cb_board_trace_test" / "kept";
    fs::remove_tree(root);
    const std::vector<InstalledImage> images = bt_images();
    const Measured m = *read_trace(kTrace, images);
    CB_CHECK(!load_measured(root).has_value());
    save_measured(root, m);
    const std::optional<Measured> again = load_measured(root);
    CB_CHECK(again.has_value());
    CB_CHECK(std::abs(again->seconds - 2.0) < 1e-6);
    CB_CHECK_EQ(again->named, m.named);
    CB_CHECK_EQ(again->channels.size(), m.channels.size());
    CB_CHECK_EQ(again->functions[0].user, std::string("user32.dll"));
});

}  // namespace
}  // namespace cb::app::board

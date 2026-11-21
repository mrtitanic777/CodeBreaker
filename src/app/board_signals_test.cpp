// Tests of board_signals.hpp: setup scripts and .reg files as Windows reads them, strings, GUIDs, ports, and the whole board kept and read back.
#include "app/board_signals.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

namespace stdfs = std::filesystem;

const char* const kScript =
    "[Version]\r\nClass=Net\r\n"
    "[Install]\r\nAddReg=Add.Reg\r\nDelReg=Del.Reg\r\nUpdateInis=Inis\r\nLogConfig=Log\r\nCopyFiles=Files\r\n"
    "[Files]\r\ndrv.386\r\n"
    "[Log]\r\nIOConfig=220-22F, 240-24F ;SB I/O\r\nIRQConfig=5, 7, S:11\r\nDMAConfig=1\r\n"
    "[Add.Reg]\r\nHKLM,%KEY_RUN%,Tool,,\"tool.exe /quiet\"\r\n"
    "HKCR,CLSID\\{FCB2D6A1-0DDD-101B-B18B-00AA0033819A}\\InProcServer32,,,server.dll\r\n"
    "[Del.Reg]\r\nHKLM,Software\\Deleted\\Key\r\n"
    "[Inis]\r\nsystem.ini,386Enh,\"device=removed.386\",\r\nsystem.ini,386Enh,,\"device=drv.386\"\r\n"
    "[Strings]\r\nKEY_RUN=\"Software\\Microsoft\\Windows\\CurrentVersion\\Run\"\r\n";

CB_TEST("app.board_signals.a_setup_script_reads_as_windows_reads_it", [] {
    const SetupFacts f = parse_inf(kScript);
    CB_CHECK_EQ(f.device_class, std::string("net"));
    CB_CHECK((f.io_ports == std::vector<std::uint32_t>{0x220, 0x240}));
    CB_CHECK((f.irqs == std::vector<std::uint32_t>{5, 7, 11}));
    CB_CHECK((f.dmas == std::vector<std::uint32_t>{1}));
    CB_CHECK_EQ(f.registry.size(), std::size_t{2});
    const auto run = std::find_if(f.registry.begin(), f.registry.end(), [](const RegWrite& w) { return w.key == "hklm\\software\\microsoft\\windows\\currentversion\\run"; });
    CB_CHECK(run != f.registry.end());
    CB_CHECK_EQ(run->data, std::string("tool.exe /quiet"));
    for (const RegWrite& w : f.registry) CB_CHECK(w.key.find("deleted") == std::string::npos);   // DelReg is no write
    CB_CHECK_EQ(f.inis.size(), std::size_t{1});                                                    // the old-entry field is a removal
    CB_CHECK_EQ(f.inis[0].entry, std::string("device=drv.386"));
});

CB_TEST("app.board_signals.a_movable_io_range_is_no_fixed_port", [] {
    const SetupFacts f = parse_inf("[L]\nIOConfig=8@100-3FF%fff0(3ff::), 3F8-3FF(3ff::)\n");
    CB_CHECK((f.io_ports == std::vector<std::uint32_t>{0x3F8}));
});

CB_TEST("app.board_signals.a_reg_file_keeps_writes_and_skips_deletions", [] {
    const std::vector<RegWrite> w = parse_reg(
        "REGEDIT4\r\n\r\n[HKEY_LOCAL_MACHINE\\Software\\A]\r\n\"Path\"=\"C:\\\\x\\\\tool.exe\"\r\n\"Gone\"=-\r\n[-HKEY_LOCAL_MACHINE\\Software\\B]\r\n\"Never\"=\"x\"\r\n");
    CB_CHECK_EQ(w.size(), std::size_t{2});
    CB_CHECK_EQ(w[1].key, std::string("hklm\\software\\a"));
    CB_CHECK_EQ(w[1].data, std::string("C:\\x\\tool.exe"));
});

CB_TEST("app.board_signals.inf_fields_keep_quoted_commas", [] {
    const std::vector<std::string> f = inf_fields("a, \"b,c\" ,,d");
    CB_CHECK((f == std::vector<std::string>{"a", "b,c", "", "d"}));
});

CB_TEST("app.board_signals.strings_are_found_in_ascii_and_utf16", [] {
    Bytes data = to_bytes(std::string("\x01plain text\x02", 12));
    for (const char c : std::string_view("wide text")) {
        data.push_back(static_cast<std::uint8_t>(c));
        data.push_back(0);
    }
    std::vector<std::string> seen;
    for_each_string(data, 8, [&](std::string_view s) { seen.emplace_back(s); });
    CB_CHECK(std::find(seen.begin(), seen.end(), "plain text") != seen.end());
    CB_CHECK(std::find(seen.begin(), seen.end(), "wide text") != seen.end());
});

CB_TEST("app.board_signals.a_guid_reads_as_text_and_in_its_stored_order", [] {
    const std::vector<std::string> g = guids_in_text("x {00021401-0000-0000-C000-000000000046} {not-a-guid} y");
    CB_CHECK_EQ(g.size(), std::size_t{1});
    CB_CHECK_EQ(g[0], std::string("{00021401-0000-0000-c000-000000000046}"));
    const auto b = guid_bytes(g[0]);
    CB_CHECK(b.has_value());
    const std::array<std::uint8_t, 16> expected{0x01, 0x14, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46};
    CB_CHECK(*b == expected);
    CB_CHECK(!guid_bytes("nope").has_value());
    CB_CHECK(guids_in_text("{00000000-0000-0000-0000-000000000000}").empty());   // the null GUID joins nothing to anything
});

CB_TEST("app.board_signals.a_registry_path_is_recognised_without_its_hive", [] {
    CB_CHECK_EQ(registry_path("Software\\Microsoft\\Windows"), std::string("software\\microsoft\\windows"));
    CB_CHECK_EQ(registry_path("HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services\\"), std::string("system\\currentcontrolset\\services"));
    CB_CHECK(registry_path("Software").empty());
    CB_CHECK(registry_path("Software\\%s\\x").empty());
    CB_CHECK(registry_path("a software\\thing\\x").empty());
});

CB_TEST("app.board_signals.a_port_needs_an_in_or_out_right_after_it", [] {
    const Bytes code = {0x66, 0xBA, 0xF8, 0x03, 0xEC,                     // mov dx, 3F8h; in al, dx
                        0xBA, 0x79, 0x03, 0x00, 0x00, 0xEE,               // mov edx, 379h; out dx, al
                        0x66, 0xBA, 0x00, 0x10, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,   // no in or out near it
                        0x66, 0xBA, 0x10, 0x00, 0xEC};                    // below any real device port
    const std::vector<PortUse> p = ports_in_code(code);
    CB_CHECK_EQ(p.size(), std::size_t{2});
    CB_CHECK_EQ(int{p[0].port}, 0x379);
    CB_CHECK(p[0].writes);
    CB_CHECK_EQ(int{p[1].port}, 0x3F8);
    CB_CHECK(!p[1].writes);
});

CB_TEST("app.board_signals.ports_follow_what_the_decoded_code_does_to_dx", [] {
    const Bytes code = {0x66, 0xBA, 0x78, 0x03,  0x42,  0xEC,                 // mov dx, 378h; inc edx; in al, dx          -> 379 read
                        0x66, 0xBA, 0xF8, 0x02,  0x80, 0xC2, 0x05,  0xEE,     // mov dx, 2F8h; add dl, 5; out dx, al       -> 2FD written
                        0x66, 0xBA, 0x00, 0x05,  0x99,  0xEC,                 // mov dx, 500h; cdq (dx is lost); in al, dx -> nothing
                        0x90, 0x90, 0x90,  0xE4, 0x60,  0xE6, 0x21};          // in al, 60h; out 21h, al
    const std::vector<PortUse> p = ports_in_code(code);
    CB_CHECK_EQ(p.size(), std::size_t{4});
    CB_CHECK_EQ(int{p[0].port}, 0x21);
    CB_CHECK(p[0].writes);
    CB_CHECK_EQ(int{p[1].port}, 0x60);
    CB_CHECK(!p[1].writes);
    CB_CHECK_EQ(int{p[2].port}, 0x2FD);
    CB_CHECK(p[2].writes);
    CB_CHECK_EQ(int{p[3].port}, 0x379);
    const Bytes wide = {0xBA, 0x20, 0x02, 0xEC};                              // in 16-bit code: mov dx, 220h; in al, dx
    CB_CHECK_EQ(ports_in_code(wide, 16).size(), std::size_t{1});
    CB_CHECK_EQ(int{ports_in_code(wide, 16)[0].port}, 0x220);
    const Bytes island = {0xEB, 0x02,  0xE4, 0x70,  0xE4, 0x60,  0xC3,  0xE4, 0x71};   // jmp over data; in al, 60h; ret; data
    const std::vector<PortUse> reached = ports_in_code(island);
    CB_CHECK_EQ(reached.size(), std::size_t{1});                              // the bytes jumped over and those after the ret are not code
    CB_CHECK_EQ(int{reached[0].port}, 0x60);
    const Bytes data = {0xE4, 0x70, 0x0F, 0x04};                              // reads like an in, then stops decoding: it was data
    CB_CHECK(ports_in_code(data).empty());
    const std::uint32_t second = 7;
    CB_CHECK_EQ(int{ports_in_code(island, 32, std::span<const std::uint32_t>(&second, 1))[0].port}, 0x71);   // a real entry point reaches it
});

Card bs_card(const char* rel, Pile pile) {
    Card c;
    c.rel = rel;
    c.name = stdfs::path(rel).filename().string();
    c.pile = pile;
    return c;
}

CB_TEST("app.board_signals.a_board_gets_its_signals_and_keeps_them", [] {
    const stdfs::path root = stdfs::temp_directory_path() / "cb_board_signals_test" / "whole";
    fs::remove_tree(root);
    fs::make_dirs(root);
    Board b;
    b.root = root;
    b.complete = true;
    b.cards = {bs_card("a/setup.inf", Pile::Setup), bs_card("a/tool.exe", Pile::Programs), bs_card("a/server.dll", Pile::Libraries), bs_card("a/drv.386", Pile::Drivers), bs_card("a/removed.386", Pile::Drivers)};
    fs::write_text(b.file_of(b.cards[0]), kScript);
    for (std::size_t i = 1; i < b.cards.size(); ++i) fs::write_text(b.file_of(b.cards[i]), "x" + std::to_string(i));
    const std::atomic<bool> go{false};
    const std::optional<Index> index = build_index(b, {}, go);
    CB_CHECK(index.has_value());
    const std::optional<Signals> s = build_signals(b, *index, {}, go);
    CB_CHECK(s.has_value());
    const auto has = [&](std::uint32_t node, SignalType type, std::string_view value, Direction direction, int confidence) {
        for (const NodeSignal& l : s->of_node(node))
            if (s->signals[l.signal].type == type && s->signals[l.signal].value == value && l.direction == direction && l.confidence == confidence) return true;
        return false;
    };
    CB_CHECK(has(1, SignalType::Autostart, "hklm run", Direction::Registers, 100));
    CB_CHECK(has(2, SignalType::Clsid, "{fcb2d6a1-0ddd-101b-b18b-00aa0033819a}", Direction::Registers, 100));
    CB_CHECK(has(3, SignalType::Autostart, "system.ini [386enh] device", Direction::Registers, 100));
    CB_CHECK(has(3, SignalType::IoPort, "0x220", Direction::Registers, 50));
    CB_CHECK(has(3, SignalType::HwClass, "net", Direction::Registers, 50));
    CB_CHECK(s->of_node(4).empty());   // named only in the old-entry field: removed, not started
    save_signals(b, *s);
    const std::optional<Signals> again = load_signals(b);
    CB_CHECK(again.has_value());
    CB_CHECK_EQ(again->links.size(), s->links.size());
    CB_CHECK_EQ(again->signals.size(), s->signals.size());
    b.cards[1].rel = "a/other.exe";
    CB_CHECK(!load_signals(b).has_value());   // for other cards
});

}  // namespace
}  // namespace cb::app::board

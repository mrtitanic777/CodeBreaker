#include "knowledge/describe.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "knowledge/package.hpp"
#include "knowledge/reads.hpp"
#include "knowledge/store.hpp"

// Behaviour pinned here:
//   * a description file has a header (cbk, platform, domain, about, source) and entries, each with a unique id and a kind
//     of the schema; its place (folder, file name) says its platform and domain, and the header must agree;
//   * validate() reports every fault of every file, and the checkout's own files have none;
//   * a description file is a kind of the base (platform / domain, rows by id) found by scanning config/knowledge, so a new
//     platform needs no C++; the reads of its entries are recorded like any other;
//   * the name tables that dispatch.cpp held (window messages, driver control messages, DOS / BIOS / DPMI services) are
//     files now, with the counts and spellings they had.

using namespace cb;
using namespace cb::knowledge;

namespace {

std::filesystem::path checkout() { return std::filesystem::path(CB_SOURCE_DIR).parent_path(); }

constexpr const char* kGood = R"({
 "cbk": 1, "platform": "nes", "domain": "registers", "about": "NES registers",
 "source": {"what": "a test"},
 "entries": [
  {"id": "PPUCTRL", "kind": "register", "address": "0x2000", "names": {"ca65": "PPUCTRL"}, "doc": "control"},
  {"id": "WM_PAINT", "kind": "constant", "set": "WM", "value": "0x000F"},
  {"id": "21.3D", "kind": "service", "vector": "21", "select": {"AH": "3D"}, "label": "open file"}
 ]})";

std::string with(std::string text, std::string_view from, std::string_view to) {
    const std::size_t at = text.find(from);
    CB_CHECK(at != std::string::npos);
    return text.replace(at, from.size(), to);
}

std::vector<Issue> issues_of(const std::string& text) {
    return check_description(Value::from_json_text(text, "nes/registers.json"), "nes/registers.json", "nes", "registers");
}

}  // namespace

CB_TEST("knowledge.describe.a_good_file_parses", [] {
    const Description d = parse_description(kGood, "nes/registers.json", "nes", "registers");
    CB_CHECK_EQ(d.platform, "nes");
    CB_CHECK_EQ(d.domain, "registers");
    CB_CHECK_EQ(d.entries.size(), 3u);
    const Value* paint = d.entries.find("WM_PAINT");
    CB_CHECK(paint != nullptr && paint->get("value") != nullptr);
    CB_CHECK_EQ(*paint->get("value")->text(), "0x000F");
    CB_CHECK_EQ(d.entries.entries().front().first, "PPUCTRL");   // the file's order
    CB_CHECK(issues_of(kGood).empty());
});

CB_TEST("knowledge.describe.faults_are_reported", [] {
    CB_CHECK(!issues_of(with(kGood, "\"cbk\": 1", "\"cbk\": 2")).empty());
    CB_CHECK(!issues_of(with(kGood, "\"platform\": \"nes\"", "\"platform\": \"gb\"")).empty());      // another folder's
    CB_CHECK(!issues_of(with(kGood, "\"domain\": \"registers\"", "\"domain\": \"ports\"")).empty());
    CB_CHECK(!issues_of(with(kGood, "\"about\": \"NES registers\",", "")).empty());
    CB_CHECK(!issues_of(with(kGood, "\"source\": {\"what\": \"a test\"},", "")).empty());
    CB_CHECK(!issues_of(with(kGood, "\"kind\": \"register\"", "\"kind\": \"gadget\"")).empty());     // not a kind of the schema
    CB_CHECK(!issues_of(with(kGood, "\"id\": \"WM_PAINT\"", "\"id\": \"PPUCTRL\"")).empty());        // an id repeats
    CB_CHECK(!issues_of(with(kGood, "\"0x2000\"", "\"zzz\"")).empty());                              // an address is hex
    CB_CHECK(!issues_of(with(kGood, "{\"AH\": \"3D\"}", "{\"AH\": 61}")).empty());                    // a select value is hex text
    CB_CHECK(!issues_of(with(kGood, "\"set\": \"WM\", ", "")).empty());                               // a constant has a set
    CB_CHECK(!issues_of(with(kGood, "\"label\": \"open file\"", "\"doc\": \"x\"")).empty());         // a service has a name
    const std::vector<Issue> repeats = issues_of(with(kGood, "\"id\": \"WM_PAINT\"", "\"id\": \"PPUCTRL\""));
    CB_CHECK(repeats.front().text().find("repeats entries[0]") != std::string::npos);
    CB_CHECK_THROWS_AS(FormatError, (void)parse_description(with(kGood, "\"cbk\": 1", "\"cbk\": 2"), "nes/registers.json", "nes", "registers"));
    CB_CHECK_THROWS_AS(ParseError, (void)parse_description("{", "nes/registers.json", "nes", "registers"));
});

CB_TEST("knowledge.describe.hex_values", [] {
    CB_CHECK_EQ(hex_value("0x0F").value(), 15u);
    CB_CHECK_EQ(hex_value("3D").value(), 0x3Du);
    CB_CHECK_EQ(hex_value("0000").value(), 0u);
    CB_CHECK(!hex_value("").has_value());
    CB_CHECK(!hex_value("0x").has_value());
    CB_CHECK(!hex_value("xyz").has_value());
});

CB_TEST("knowledge.describe.the_folders_are_scanned", [] {
    fs::ScopedTempDir dir{"cb-knowledge-describe"};
    fs::write_text(dir.path() / "nes" / "registers.json", kGood);
    fs::write_text(dir.path() / "models" / "win16.json", "{\"functions\": {}}");   // not a platform: never read as a description
    fs::write_text(dir.path() / "kept" / "given.json", "{}");
    fs::write_text(dir.path() / "gb" / "registers.json", with(with(kGood, "\"nes\"", "\"gb\""), "\"id\": \"PPUCTRL\"", "\"id\": \"WM_PAINT\""));
    const std::vector<std::filesystem::path> files = description_files(dir.path());
    CB_CHECK_EQ(files.size(), 2u);
    const ValidationReport report = validate_descriptions(dir.path());
    CB_CHECK_EQ(report.files, 2u);
    CB_CHECK_EQ(report.entries, 6u);
    CB_CHECK(!report.ok());   // gb's ids repeat
    CB_CHECK_EQ(report.issues.size(), 1u);
    CB_CHECK(report.issues.front().file == "gb/registers.json");
    // a new platform is a folder: its kinds are found by scanning, and its rows are in the base
    DataFolders folders;
    folders.described = dir.path();
    folders.symbols = dir.path() / "symbols";
    folders.models = dir.path() / "models";
    AuthoredSources sources(folders);
    const std::vector<AuthoredKind> all = sources.kinds();
    CB_CHECK_EQ(all.size(), AuthoredSources::catalogue().size() + 2);
    const std::optional<AuthoredKind> nes = sources.lookup({"nes", "registers"});
    CB_CHECK(nes.has_value());
    CB_CHECK_EQ(sources.source_label(*nes), "config/knowledge/nes/registers.json");
    CB_CHECK_EQ(sources.read(*nes).size(), 3u);
    CB_CHECK(!sources.lookup({"nes", "ports"}).has_value());
});

CB_TEST("knowledge.describe.the_checkouts_files_are_valid", [] {
    fs::ScopedTempDir dir{"cb-knowledge-describe"};
    Store store{dir.path() / "k.sqlite", DataFolders::under(checkout())};
    Knowledge kb(store);
    const ValidationReport report = kb.validate();
    for (const Issue& issue : report.issues) CB_CHECK_EQ(issue.text(), "");   // (shows what is wrong)
    CB_CHECK(report.ok());
    CB_CHECK(report.files >= 3u);
});

CB_TEST("knowledge.describe.the_dispatch_names_moved_here", [] {
    fs::ScopedTempDir dir{"cb-knowledge-describe"};
    Store store{dir.path() / "k.sqlite", DataFolders::under(checkout())};
    Knowledge kb(store);
    // the counts the tables had in analyze/dispatch.cpp
    CB_CHECK_EQ(kb.entries("windows", "messages").size(), 130u);
    CB_CHECK_EQ(kb.entries("vxd", "messages").size(), 51u);
    CB_CHECK_EQ(kb.entries("dos", "services").size(), 107u);   // 57 DOS functions, 19 interrupt vectors, 31 DPMI functions
    const auto field = [&kb](std::string_view platform, std::string_view domain, std::string_view id, std::string_view key) {
        const std::optional<Value> entry = kb.entry(platform, domain, id);
        const Value* v = entry ? entry->get(key) : nullptr;
        const std::string* text = v != nullptr ? v->text() : nullptr;
        return text != nullptr ? *text : std::string("(none)");
    };
    CB_CHECK_EQ(field("windows", "messages", "WM_PAINT", "value"), "0x000F");
    CB_CHECK_EQ(field("vxd", "messages", "Kernel32_Shutdown", "name"), "Kernel32_Shutdown");
    CB_CHECK_EQ(field("dos", "services", "21.3D", "label"), "open file");
    CB_CHECK_EQ(field("dos", "services", "31.0501", "label"), "allocate memory");
    CB_CHECK_EQ(field("dos", "services", "10", "label"), "video");
    CB_CHECK(!kb.entry("dos", "services", "21.FF").has_value());
    const std::vector<std::string> domains = kb.domains("dos");
    CB_CHECK(std::find(domains.begin(), domains.end(), "services") != domains.end());
    // the ask is a recorded read
    Recording rec;
    (void)kb.entry("dos", "services", "21.3D");
    (void)kb.entries("vxd", "messages");
    const auto reads = rec.reads()->sorted();
    CB_CHECK(std::find(reads.begin(), reads.end(), Read{"dos", "services", "21.3D"}) != reads.end());
    CB_CHECK(std::find(reads.begin(), reads.end(), Read{"vxd", "messages", "*"}) != reads.end());
});

CB_TEST("knowledge.describe.the_console_registers_moved_here", [] {
    fs::ScopedTempDir dir{"cb-knowledge-describe"};
    Store store{dir.path() / "k.sqlite", DataFolders::under(checkout())};
    Knowledge kb(store);
    // the counts the writers' tables had
    CB_CHECK_EQ(kb.hardware_names("sms", "ports").size(), 8u);
    CB_CHECK_EQ(kb.hardware_names("sms", "game_gear_ports").size(), 2u);
    CB_CHECK_EQ(kb.hardware_names("sms", "mapper").size(), 4u);
    CB_CHECK_EQ(kb.hardware_names("gb", "io").size(), 55u);
    CB_CHECK_EQ(kb.hardware_names("gb", "mbc").size(), 4u);
    CB_CHECK_EQ(kb.hardware_names("genesis", "hardware").size(), 17u);
    CB_CHECK_EQ(kb.hardware_names("nes", "hardware").size(), 30u);
    CB_CHECK_EQ(kb.hardware_names("atari2600", "tia_write").size(), 45u);
    CB_CHECK_EQ(kb.hardware_names("atari2600", "tia_read").size(), 14u);
    CB_CHECK_EQ(kb.hardware_names("atari2600", "riot").size(), 10u);
    // the names, and a dialect's spelling
    CB_CHECK_EQ(kb.hardware_names("sms", "ports").at(0xBF), "VDP_CONTROL");
    CB_CHECK_EQ(kb.hardware_names("gb", "io", "rgbds").at(0xFF40), "rLCDC");
    CB_CHECK_EQ(kb.hardware_names("gb", "io").at(0xFF40), "LCDC");
    CB_CHECK_EQ(kb.hardware_names("genesis", "hardware").at(0xC00004), "VDP_CTRL");
    CB_CHECK_EQ(kb.hardware_names("atari2600", "tia_write").at(0x02), "WSYNC");
    CB_CHECK_EQ(kb.hardware_names("atari2600", "tia_read").at(0x02), "CXP0FB");
    // the Z80's ports are in its io space, the others' registers in the processor's memory
    for (const HardwareRegister& r : kb.hardware_registers("sms", "ports")) CB_CHECK_EQ(r.space, "io");
    for (const HardwareRegister& r : kb.hardware_registers("sms", "mapper")) CB_CHECK_EQ(r.space, "cpu");
    CB_CHECK(kb.hardware_registers("no-such-machine").empty());
});

CB_TEST("knowledge.describe.every_other_consoles_tables_moved_here", [] {
    fs::ScopedTempDir dir{"cb-knowledge-describe"};
    Store store{dir.path() / "k.sqlite", DataFolders::under(checkout())};
    Knowledge kb(store);
    // the counts the writers' and formats' tables had (the Saturn's and the 32X's without the SH-2's, the Sega CD's without the Genesis's)
    CB_CHECK_EQ(kb.hardware_names("atari5200", "gtia_write").size(), 32u);
    CB_CHECK_EQ(kb.hardware_names("atari5200", "gtia_read").size(), 22u);
    CB_CHECK_EQ(kb.hardware_names("atari5200", "antic_write").size(), 11u);
    CB_CHECK_EQ(kb.hardware_names("atari5200", "antic_read").size(), 4u);
    CB_CHECK_EQ(kb.hardware_names("atari5200", "pokey_write").size(), 15u);
    CB_CHECK_EQ(kb.hardware_names("atari5200", "pokey_read").size(), 14u);
    CB_CHECK_EQ(kb.hardware_names("atari7800", "tia_write").size(), 7u);
    CB_CHECK_EQ(kb.hardware_names("atari7800", "tia_read").size(), 6u);
    CB_CHECK_EQ(kb.hardware_names("atari7800", "maria").size(), 32u);
    CB_CHECK_EQ(kb.hardware_names("atari7800", "riot").size(), 10u);
    CB_CHECK_EQ(kb.hardware_names("cdi", "hardware").size(), 6u);
    CB_CHECK_EQ(kb.table_names("cdi", "system_calls").size(), 31u);
    CB_CHECK_EQ(kb.table_names("chf", "bios").size(), 5u);
    CB_CHECK_EQ(kb.hardware_names("coleco", "ports").size(), 6u);
    CB_CHECK_EQ(kb.table_names("coleco", "bios").size(), 50u);
    CB_CHECK_EQ(kb.table_names("cpc", "firmware").size(), 216u);
    CB_CHECK_EQ(kb.hardware_names("dreamcast", "hardware").size(), 1593u);
    CB_CHECK_EQ(kb.hardware_names("gba", "hardware").size(), 97u);
    CB_CHECK_EQ(kb.hardware_names("intv", "hardware").size(), 58u);
    CB_CHECK_EQ(kb.hardware_names("jaguar", "hardware").size(), 107u);
    CB_CHECK_EQ(kb.hardware_names("lynx", "registers").size(), 203u);
    CB_CHECK_EQ(kb.hardware_names("msx", "ports").size(), 9u);
    CB_CHECK_EQ(kb.table_names("msx", "firmware").size(), 38u);
    CB_CHECK_EQ(kb.hardware_names("n3ds", "hardware").size(), 8u);
    CB_CHECK_EQ(kb.hardware_names("n64", "hardware").size(), 74u);
    CB_CHECK_EQ(kb.hardware_names("nds", "arm9").size(), 145u);
    CB_CHECK_EQ(kb.hardware_names("nds", "arm7").size(), 215u);
    CB_CHECK_EQ(kb.hardware_names("neogeo", "hardware").size(), 56u);
    CB_CHECK_EQ(kb.hardware_names("neogeo", "sound_ports").size(), 6u);
    CB_CHECK_EQ(kb.hardware_names("ngp", "hardware").size(), 71u);
    CB_CHECK_EQ(kb.hardware_names("o2", "vdc").size(), 4u);
    CB_CHECK_EQ(kb.hardware_names("pce", "read").size(), 12u);
    CB_CHECK_EQ(kb.hardware_names("pce", "write").size(), 23u);
    CB_CHECK_EQ(kb.hardware_names("ps1", "hardware").size(), 69u);
    CB_CHECK_EQ(kb.hardware_names("ps2", "registers").size(), 190u);
    CB_CHECK_EQ(kb.hardware_names("saturn", "hardware").size(), 180u);
    CB_CHECK_EQ(kb.hardware_names("sega32x", "hardware").size(), 37u);
    CB_CHECK_EQ(kb.hardware_names("segacd", "hardware").size(), 27u);
    CB_CHECK_EQ(kb.hardware_names("sh2", "chip_registers").size(), 52u);
    CB_CHECK_EQ(kb.hardware_names("snes", "registers").size(), 192u);
    CB_CHECK_EQ(kb.hardware_names("threedo", "hardware").size(), 19u);
    CB_CHECK_EQ(kb.hardware_names("vb", "hardware").size(), 87u);
    CB_CHECK_EQ(kb.table_names("vectrex", "bios").size(), 60u);
    CB_CHECK_EQ(kb.hardware_names("vectrex", "via").size(), 16u);
    CB_CHECK_EQ(kb.hardware_names("wonderswan", "ports").size(), 78u);
    CB_CHECK_EQ(kb.hardware_names("zx", "ports").size(), 2u);
    CB_CHECK_EQ(kb.table_names("zx", "firmware").size(), 21u);
    // spellings
    CB_CHECK_EQ(kb.hardware_names("msx", "ports").at(0x99), "VDP_CTRL");
    CB_CHECK_EQ(kb.table_names("msx", "firmware").at(0x009F), "CHGET");
    CB_CHECK_EQ(kb.table_names("cdi", "system_calls").at(0x06), "F_Exit");
    CB_CHECK_EQ(kb.hardware_names("snes", "registers").at(0x2100), "INIDISP");
    // the Z80 machines' ports are in its io space, and a firmware entry is not a register
    for (const HardwareRegister& r : kb.hardware_registers("msx")) CB_CHECK_EQ(r.space, "io");
    CB_CHECK_EQ(kb.hardware_registers("msx").size(), 9u);
});

CB_TEST("knowledge.describe.address_blocks_are_regions", [] {
    fs::ScopedTempDir dir{"cb-knowledge-describe"};
    Store store{dir.path() / "k.sqlite", DataFolders::under(checkout())};
    Knowledge kb(store);
    CB_CHECK_EQ(kb.hardware_regions("gamecube", "blocks").size(), 11u);
    CB_CHECK_EQ(kb.hardware_regions("wii", "blocks").size(), 8u);
    CB_CHECK_EQ(kb.hardware_regions("wiiu", "blocks").size(), 1u);
    CB_CHECK_EQ(kb.hardware_regions("ps2", "blocks").size(), 9u);
    CB_CHECK_EQ(kb.hardware_regions("psp", "blocks").size(), 7u);
    CB_CHECK_EQ(kb.hardware_registers("gamecube", "blocks").size(), 123u);
    CB_CHECK_EQ(kb.hardware_registers("wii", "blocks").size(), 17u);
    const std::vector<HardwareRegion> cube = kb.hardware_regions("gamecube");
    CB_CHECK_EQ(cube.front().name, "CP");
    CB_CHECK_EQ(cube.front().address, 0xCC000000u);
    CB_CHECK_EQ(cube.front().size, 0x1000u);
    // a block is not a register
    for (const HardwareRegister& r : kb.hardware_registers("ps2")) CB_CHECK(r.table != "blocks");
});

CB_TEST("knowledge.describe.the_xbox_360_ordinals_moved_here", [] {
    fs::ScopedTempDir dir{"cb-knowledge-describe"};
    Store store{dir.path() / "k.sqlite", DataFolders::under(checkout())};
    Knowledge kb(store);
    CB_CHECK_EQ(kb.export_name("xbox360", "xam.xex", 1).value_or(""), "NetDll_WSAStartup");
    CB_CHECK_EQ(kb.export_name("xbox360", "xboxkrnl.exe", 3).value_or(""), "DbgPrint");
    CB_CHECK(!kb.export_name("xbox360", "xam.xex", 4000000).has_value());
    CB_CHECK(!kb.export_name("xbox360", "no-such.xex", 1).has_value());
    // the counts the console_ordinals.json they came from had
    CB_CHECK_EQ(kb.export_names("xbox360", "xam.xex").size(), 1393u);
    CB_CHECK_EQ(kb.export_names("xbox360", "xboxkrnl.exe").size(), 859u);
    CB_CHECK_EQ(kb.export_names("xbox360", "xam.xex").at("1"), "NetDll_WSAStartup");
    Recording rec;
    (void)kb.export_name("xbox360", "xam.xex", 1);
    const auto reads = rec.reads()->sorted();
    CB_CHECK(std::find(reads.begin(), reads.end(), Read{"xbox360", "ordinals", "xam.xex#1"}) != reads.end());
});

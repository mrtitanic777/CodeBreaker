// Tests of cb/symbols/driver_services. The table is the real one (testdata/driver_services.json, the table the knowledge base
// holds for the vxd platform); the include-file reader runs over real files of the Windows 95 driver kit (testdata/driver_kit:
// VKD, VCACHE, PAGESWAP, MMDEVLDR and NETVXD whole, and the device-id and message parts of VMM.INC), its expected answers being
// what the Python module read from the same files; and over small include files written out for the forms the real ones do not
// have (the C header's #define, `=`, a macro's own Begin_Service_Table line, one device claimed by two files).
//
// DIVERGES from the Python module, by design: a table whose device key is not hex, or whose name is not text, is a FormatError
// at the load (the Python failed with ValueError / TypeError at the first use); lines of the kit's files end at \n, \r\n and
// \r (the Python's splitlines also split on form feeds and a few control characters, which the kit's files do not use); a
// device number that does not fit 32 bits is not a number.
#include "cb/symbols/driver_services.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/value.hpp"

namespace ds = cb::symbols::driver_services;
namespace kn = cb::knowledge;

namespace {

std::filesystem::path kit_path() { return cb::testing::data_path("symbols/testdata/driver_kit"); }

// The real table, as the knowledge base would hold it.
ds::Table real_table() {
    const kn::Value doc = kn::Value::from_json_text(cb::fs::read_text(cb::testing::data_path("symbols/testdata/driver_services.json")));
    auto rows = [&](const char* kind) {
        const kn::Value* part = doc.get(kind);
        CB_CHECK(part != nullptr && part->map() != nullptr);
        return kn::Rows(*part->map());
    };
    return ds::Table::from_rows(rows("devices"), rows("services"), rows("drivers"), rows("messages"));
}

void write(const std::filesystem::path& dir, const char* name, std::string_view text) { cb::fs::write_text(dir / name, text); }

}  // namespace

CB_TEST("symbols.driver_services.names_from_the_real_table", [] {
    const ds::Table t = real_table();
    CB_CHECK_EQ(*t.service_name(1, 0xD1), "Test_DBCS_Lead_Byte");
    CB_CHECK_EQ(*t.service_name(1, 0xF1), "Get_Crit_Status_No_Block");
    CB_CHECK_EQ(*t.service_name(1, 0x8001), "Get_Cur_VM_Handle");        // bit 15: a jump to the service, not part of its number
    CB_CHECK_EQ(*t.service_name(0xD, 1), "VKD_Define_Hot_Key");
    CB_CHECK(!t.service_name(0xD, 0x500).has_value());                  // past the table
    CB_CHECK(!t.service_name(0x7FF, 2).has_value());                    // no table
    CB_CHECK_EQ(*t.device_name(0xD), "VKD");
    CB_CHECK_EQ(*t.device_name(58), "VPMTD");                           // a corpus driver's own name
    CB_CHECK(!t.device_name(0x7FF).has_value());
    CB_CHECK_EQ(*t.message_name(1), "DEVICE_INIT");
    CB_CHECK_EQ(*t.message_name(0x23), "W32_DEVICEIOCONTROL");
    CB_CHECK(!t.message_name(0x999).has_value());
    CB_CHECK_EQ(t.messages().size(), std::size_t{51});
});

CB_TEST("symbols.driver_services.labels", [] {
    const ds::Table t = real_table();
    CB_CHECK_EQ(t.label(1, 0xD1), "Test_DBCS_Lead_Byte");           // the real name
    CB_CHECK_EQ(t.label(0xD, 0x500), "VKD_Service_0500");           // a known device, a service past its table
    CB_CHECK_EQ(t.label(0x44, 3), "PELOADER_Service_0003");
    CB_CHECK_EQ(t.label(1, 0x3FF), "VMM_Service_03FF");
    CB_CHECK_EQ(t.label(1, 0x83FF), "VMM_Service_03FF");            // bit 15 is not printed
    CB_CHECK_EQ(t.label(0x7FF, 2), "VxD_07FF_0002");               // an unknown device
    CB_CHECK_EQ(t.label(0, 0), "VxD_0000_0000");
    // with nothing known of the system's device, the placeholder is the number alone
    const ds::Table empty;
    CB_CHECK_EQ(empty.label(1, 5), "VMM_0005");
    CB_CHECK_EQ(empty.label(2, 5), "VxD_0002_0005");
    // a device name that is not an identifier
    kn::Rows devices;
    devices.insert_or_assign("0005", kn::Value("My-Dev 2"));
    const ds::Table odd = ds::Table::from_rows(devices, {}, {}, {});
    CB_CHECK_EQ(odd.label(5, 1), "My_Dev_2_Service_0001");
});

CB_TEST("symbols.driver_services.size_of_the_real_table", [] {
    const ds::Table::Size s = real_table().size();
    CB_CHECK_EQ(s.service_tables, std::size_t{38});
    CB_CHECK_EQ(s.services_named, std::size_t{1147});
    CB_CHECK_EQ(s.devices_named, std::size_t{134});     // the kit's 109 and the corpus drivers' 25, none of them twice
    CB_CHECK_EQ(ds::size_line(s), "38 service tables, 1147 services named, 134 device numbers named");
});

CB_TEST("symbols.driver_services.a_name_in_both_lists_is_counted_once_and_the_kit_wins", [] {
    kn::Rows devices, drivers, services;
    devices.insert_or_assign("0010", kn::Value("IOS"));
    drivers.insert_or_assign("0010", kn::Value("BLOCKDEV"));
    drivers.insert_or_assign("0011", kn::Value("OWN"));
    kn::Value::List names = {kn::Value("A"), kn::Value(), kn::Value("C"), kn::Value("")};   // gaps are empty names
    services.insert_or_assign("0010", kn::Value(names));
    const ds::Table t = ds::Table::from_rows(devices, services, drivers, {});
    CB_CHECK_EQ(*t.device_name(0x10), "IOS");
    CB_CHECK_EQ(*t.device_name(0x11), "OWN");
    CB_CHECK_EQ(t.size().devices_named, std::size_t{2});
    CB_CHECK_EQ(t.size().services_named, std::size_t{2});
    CB_CHECK_EQ(*t.service_name(0x10, 2), "C");
    CB_CHECK(!t.service_name(0x10, 1).has_value());
    CB_CHECK(!t.service_name(0x10, 3).has_value());
    CB_CHECK_EQ(t.label(0x10, 1), "IOS_Service_0001");
});

CB_TEST("symbols.driver_services.a_damaged_table_is_a_format_error", [] {
    kn::Rows bad_key;
    bad_key.insert_or_assign("zz", kn::Value("X"));
    CB_CHECK_THROWS_AS(cb::FormatError, ds::Table::from_rows(bad_key, {}, {}, {}));
    CB_CHECK_THROWS_AS(cb::FormatError, ds::Table::from_rows({}, {}, bad_key, {}));
    kn::Rows not_text;
    not_text.insert_or_assign("0001", kn::Value(std::int64_t{5}));
    CB_CHECK_THROWS_AS(cb::FormatError, ds::Table::from_rows(not_text, {}, {}, {}));
    CB_CHECK_THROWS_AS(cb::FormatError, ds::Table::from_rows({}, not_text, {}, {}));   // a service list that is a number
    kn::Rows list_of_numbers;
    list_of_numbers.insert_or_assign("0001", kn::Value(kn::Value::List{kn::Value(std::int64_t{1})}));
    CB_CHECK_THROWS_AS(cb::FormatError, ds::Table::from_rows({}, list_of_numbers, {}, {}));
});

CB_TEST("symbols.driver_services.coverage_of_a_driver", [] {
    const ds::Table t = real_table();
    const std::vector<ds::ServiceCall> calls = {{1, 0x8001}, {1, 1}, {0xD, 1}, {0xD, 0x8500}, {0xD, 0x500}, {0x7FF, 2}};
    const ds::Coverage c = ds::coverage(t, calls);
    CB_CHECK_EQ(c.calls, std::size_t{6});
    CB_CHECK_EQ(c.named, std::size_t{3});
    CB_CHECK_EQ(c.missing.size(), std::size_t{2});
    CB_CHECK_EQ((c.missing.at({0xD, 0x500})), std::size_t{2});   // the jump bit does not make a service another
    CB_CHECK_EQ((c.missing.at({0x7FF, 2})), std::size_t{1});
    // (the Python module's layout: "%-16s %4d service calls, %4d named" and "  NAME xN" for each unnamed one)
    CB_CHECK_EQ(ds::coverage_line(t, "vkd.vxd", c), std::format("{:<16} {:>4} service calls, {:>4} named  VKD_Service_0500 x2  VxD_07FF_0002 x1", "vkd.vxd", 6, 3));
    // at most six unnamed ones are listed
    std::vector<ds::ServiceCall> many;
    for (std::uint32_t i = 0; i < 9; ++i) many.push_back({0x7FF, i});
    const std::string line = ds::coverage_line(t, "x.vxd", ds::coverage(t, many));
    CB_CHECK(line.find("VxD_07FF_0005 x1") != std::string::npos);
    CB_CHECK(line.find("VxD_07FF_0006") == std::string::npos);
});

CB_TEST("symbols.driver_services.device_numbers", [] {
    const std::map<std::string, std::uint32_t> known = {{"NAME", 5}, {"MULTIMEDIA_OEM_ID", 0x440}};
    // the Python module's answers
    CB_CHECK_EQ(ds::parse_device_number("0000DH", known).value_or(999), std::uint32_t{13});
    CB_CHECK_EQ(ds::parse_device_number("38H", known).value_or(999), std::uint32_t{56});
    CB_CHECK_EQ(ds::parse_device_number("0x28", known).value_or(999), std::uint32_t{40});
    CB_CHECK_EQ(ds::parse_device_number("NAME + 3", known).value_or(999), std::uint32_t{8});
    CB_CHECK_EQ(ds::parse_device_number("MULTIMEDIA_OEM_ID+2", known).value_or(999), std::uint32_t{1090});
    CB_CHECK_EQ(ds::parse_device_number("(0440h) + 10", known).value_or(999), std::uint32_t{1098});
    CB_CHECK_EQ(ds::parse_device_number("12", known).value_or(999), std::uint32_t{12});
    CB_CHECK_EQ(ds::parse_device_number("0000dh+1", known).value_or(999), std::uint32_t{14});
    CB_CHECK_EQ(ds::parse_device_number("name", known).value_or(999), std::uint32_t{5});   // names are in capitals in `known`
    CB_CHECK(!ds::parse_device_number("abc", known).has_value());
    CB_CHECK(!ds::parse_device_number("", known).has_value());
    CB_CHECK(!ds::parse_device_number("1+", known).has_value());
    CB_CHECK(!ds::parse_device_number("0X10", known).has_value());   // the 0x of the C form is lower case
    CB_CHECK(!ds::parse_device_number("0xZ", known).has_value());
    CB_CHECK(!ds::parse_device_number("FFFFFFFFFh+1", known).has_value());   // past 32 bits
});

CB_TEST("symbols.driver_services.reads_the_real_include_files", [] {
    const ds::KitTables kit = ds::read_include_files(kit_path());
    CB_CHECK_EQ(kit.devices.size(), std::size_t{106});
    CB_CHECK_EQ(kit.devices.at(1), "VMM");
    CB_CHECK_EQ(kit.devices.at(7), "PAGESWAP");
    CB_CHECK_EQ(kit.devices.at(0xD), "VKD");
    CB_CHECK_EQ(kit.devices.at(0x44A), "MMDEVLDR");        // Multimedia_OEM_ID + 10, the equate and the expression in one file
    CB_CHECK_EQ(kit.devices.at(0x485), "MEMPROBE");        // the line before it in NETVXD.INC is a commented #define
    CB_CHECK(!kit.devices.contains(0x484));
    CB_CHECK_EQ(kit.devices.at(0x32), "SERVER");
    CB_CHECK_EQ(kit.devices.at(0x48B), "VCache");          // spelled as its own file spells it
    CB_CHECK_EQ(kit.services.size(), std::size_t{4});
    CB_CHECK_EQ(kit.services.at(7).size(), std::size_t{10});
    CB_CHECK_EQ(kit.services.at(7).front(), "PageSwap_Get_Version");
    CB_CHECK_EQ(kit.services.at(7)[1], "PageSwap_Invalid_Service1");
    CB_CHECK_EQ(kit.services.at(7).back(), "PageSwap_Init_File");
    CB_CHECK_EQ(kit.services.at(0xD).size(), std::size_t{21});
    CB_CHECK_EQ(kit.services.at(0xD)[1], "VKD_Define_Hot_Key");
    CB_CHECK_EQ(kit.services.at(0xD).back(), "VKD_Set_Shift_State");
    CB_CHECK_EQ(kit.services.at(0x44A).size(), std::size_t{6});
    CB_CHECK_EQ(kit.services.at(0x44A)[1], "_MMDEVLDR_SetDevicePresence");
    CB_CHECK_EQ(kit.services.at(0x48B).size(), std::size_t{25});
    CB_CHECK_EQ(kit.services.at(0x48B).back(), "_VCache_UpdateLookup");
});

CB_TEST("symbols.driver_services.reads_the_system_messages", [] {
    const std::map<std::uint32_t, std::string> m = ds::read_messages(kit_path());
    CB_CHECK_EQ(m.size(), std::size_t{51});
    CB_CHECK_EQ(m.at(0), "SYS_CRITICAL_INIT");
    CB_CHECK_EQ(m.at(1), "DEVICE_INIT");
    CB_CHECK_EQ(m.at(2), "INIT_COMPLETE");
    CB_CHECK_EQ(m.at(3), "SYS_VM_INIT");
    CB_CHECK_EQ(m.at(49), "KERNEL32_INITIALIZED");
    CB_CHECK_EQ(m.at(50), "KERNEL32_SHUTDOWN");          // the last before MAX_SYSTEM_CONTROL, which is not one
    CB_CHECK(!m.contains(0x32 + 1));
    CB_CHECK_THROWS_AS(cb::IoError, ds::read_messages(kit_path() / "nowhere"));
    CB_CHECK_THROWS_AS(cb::IoError, ds::read_include_files(kit_path() / "nowhere"));
});

CB_TEST("symbols.driver_services.forms_the_real_files_do_not_have", [] {
    cb::fs::ScopedTempDir dir("ds-kit");
    // the main file names the system; two files list the same device's services (the longer wins); the C header and `=`
    write(dir.path(), "VMM.INC",
          "VMM_DEVICE_ID\tEQU\t00001H\n"
          "VFOO_DEVICE_ID\tEQU\t00300H\t; comment\n"
          "SYS_CRITICAL_INIT\tEQU\t0000H\n"
          "DEVICE_INIT\tEQU\t0001H\n"
          "MAX_SYSTEM_CONTROL\tEQU\t0032H\n"
          "AFTER\tEQU\t0033H\n");
    write(dir.path(), "A.INC",
          "Begin_Service_Table VMACRO\n"            // another device, no ids: never numbered
          "VMACRO_Service Lost\n"
          "End_Service_Table VMACRO\n"
          "VBAR_Device_ID = VFOO_Device_ID + 1\n"
          "Begin_Service_Table MACRO name\n"        // a macro's own definition is not a table
          "Begin_Service_Table VFOO\n"
          "VFOO_Service Foo_Get_Version\n"
          "  vfoo_service Foo_Next ; a comment\n"   // any case, any indent; the comment is cut
          "VFOO_Service_Table_Entry no\n"           // not a service line
          "End_Service_Table VFOO\n"
          "VFOO_Service Foo_Outside\n");            // after the end: not in the table
    write(dir.path(), "B.INC",
          "Begin_Service_Table VFOO\n"
          "VFOO_Service Foo_Get_Version\n"
          "VFOO_Service Foo_Next\n"
          "VFOO_Service Foo_Third\n"
          "End_Service_Table VFOO\n");
    write(dir.path(), "C.H",
          "#define VBAZ_Device_ID 0x0400\n"
          "#define NOT_AN_ID 5\n"
          "  #define VQUX_Device_ID   0401h  \n"
          "VPLAIN_Device_ID equ\n"                   // no value: nothing
          "VEMPTY_Device_ID equ  ;\n"                // a blank, then a comment
          "Begin_Service_Table VBAZ\n"
          "VBAZ_Service Baz_One\n"
          "End_Service_Table VBAZ\n");
    write(dir.path(), "README.TXT", "VNOPE_Device_ID equ 0500h\n");   // not an include file
    const ds::KitTables kit = ds::read_include_files(dir.path());
    CB_CHECK_EQ(kit.devices.at(1), "VMM");
    CB_CHECK_EQ(kit.devices.at(0x300), "VFOO");
    CB_CHECK_EQ(kit.devices.at(0x301), "VBAR");              // = VFOO_Device_ID + 1: the name of another device, plus one
    CB_CHECK_EQ(kit.devices.at(0x400), "VBAZ");
    CB_CHECK_EQ(kit.devices.at(0x401), "VQUX");              // an indented #define, and a value in the assembler's hex form
    CB_CHECK(!kit.devices.contains(0x500));
    CB_CHECK_EQ(kit.services.size(), std::size_t{2});
    // B.INC lists three, A.INC two: the longer is kept
    CB_CHECK((kit.services.at(0x300) == std::vector<std::string>{"Foo_Get_Version", "Foo_Next", "Foo_Third"}));
    CB_CHECK((kit.services.at(0x400) == std::vector<std::string>{"Baz_One"}));
    const auto messages = ds::read_messages(dir.path());
    CB_CHECK_EQ(messages.size(), std::size_t{2});
    CB_CHECK_EQ(messages.at(0), "SYS_CRITICAL_INIT");
    CB_CHECK_EQ(messages.at(1), "DEVICE_INIT");
});

CB_TEST("symbols.driver_services.the_kits_main_list_names_a_shared_number", [] {
    cb::fs::ScopedTempDir dir("ds-kit");
    write(dir.path(), "VMM.INC", "VOLD_DEVICE_ID\tEQU\t00400H\n");
    write(dir.path(), "OTHER.INC", "VNEW_Device_ID equ 0400h\nVAAA_Device_ID equ 0400h\n");
    const ds::KitTables kit = ds::read_include_files(dir.path());
    CB_CHECK_EQ(kit.devices.size(), std::size_t{1});
    CB_CHECK_EQ(kit.devices.at(0x400), "VOLD");
    // numbers outside 1..0xFFFF are no device
    write(dir.path(), "BIG.INC", "VBIG_Device_ID equ 10000h\nVZERO_Device_ID equ 0\n");
    CB_CHECK_EQ(ds::read_include_files(dir.path()).devices.size(), std::size_t{1});
});

CB_TEST("symbols.driver_services.finds_the_include_folder", [] {
    cb::fs::ScopedTempDir dir("ds-ref");
    const std::filesystem::path kit = dir.path() / "win95ddk" / "INC32";
    cb::fs::write_text(kit / "VMM.INC", "x\n");
    CB_CHECK(cb::fs::same_path(ds::find_include_dir("", dir.path()), kit));
    // an override that holds VMM.INC wins; one that does not is passed over
    cb::fs::ScopedTempDir mine("ds-own");
    CB_CHECK(cb::fs::same_path(ds::find_include_dir(cb::fs::to_text(mine.path()), dir.path()), kit));
    cb::fs::write_text(mine.path() / "vmm.inc", "x\n");
    CB_CHECK(cb::fs::same_path(ds::find_include_dir(cb::fs::to_text(mine.path()), dir.path()), mine.path()));
    CB_CHECK(ds::find_include_dir("", dir.path() / "nothing").empty());
});

CB_TEST("symbols.driver_services.table_file_text", [] {
    ds::KitTables kit;
    kit.devices = {{1, "VMM"}, {0xD, "VKD"}};
    kit.services = {{0xD, {"VKD_Get_Version", "VKD_Define_Hot_Key"}}};
    const std::string text = ds::format_table(kit, {{0xD, "VKD"}, {0x100, "MYDRV"}}, {{1, "DEVICE_INIT"}});
    const std::string expected =
        "{\n"
        " \"source\": \"Windows 95 driver kit include files (INC32): Begin_Service_Table lists and *_Device_ID equates; \\\"drivers\\\" from the corpus drivers' own descriptors\",\n"
        " \"devices\": {\n  \"0001\": \"VMM\",\n  \"000D\": \"VKD\"\n },\n"
        " \"services\": {\n  \"000D\": [\n   \"VKD_Get_Version\",\n   \"VKD_Define_Hot_Key\"\n  ]\n },\n"
        " \"drivers\": {\n  \"0100\": \"MYDRV\"\n },\n"
        " \"messages\": {\n  \"0001\": \"DEVICE_INIT\"\n }\n"
        "}\n";
    CB_CHECK_EQ(text, expected);
    // empty
    CB_CHECK(ds::format_table({}, {}, {}).find("\"devices\": {},") != std::string::npos);
    // the file reads back into the same table
    const kn::Value doc = kn::Value::from_json_text(text);
    const ds::Table t = ds::Table::from_rows(*doc.get("devices")->map(), *doc.get("services")->map(), *doc.get("drivers")->map(),
                                             *doc.get("messages")->map());
    CB_CHECK_EQ(*t.service_name(0xD, 1), "VKD_Define_Hot_Key");
    CB_CHECK_EQ(*t.device_name(0x100), "MYDRV");
    CB_CHECK_EQ(*t.message_name(1), "DEVICE_INIT");
});

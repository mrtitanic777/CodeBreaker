// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/disassembly on the real Windows 95 drivers of formats/testdata (splitter.vxd, srammtd.vxd, bios.vxd). The
// goldens (testdata/splitter.asm.golden, srammtd.asm.golden) are the listings the Python module wrote for the first two, with the
// DDK's names the Python found: the tables below are the services those listings name (the DDK is not in the repository). The
// counts (instructions, code bytes, labels, tables, the service table, the check of every byte) are the Python's, from its harness
// (state/work/typed-port/source.vxd.disassembly/expected.txt).
//
// DIVERGES from the Python module, by design: an internal fix-up with no target (a 16-bit selector fix-up) is no entry point,
// branch target, label or table entry (the Python stopped with a TypeError when one reached an `&` or a comparison); a table of
// pointers ends at the first fix-up that is a selector; the recovery's byte states are arrays, not sets of places, and iterating
// the instructions, labels and tables is in order of place (the Python's were in the order the descent found things); the
// listing files are written with LF line ends (the Python's text-mode write made CRLF on Windows); Capstone is 5.0.7 where the
// Python's was 4.0.2 -- the listings are identical on these drivers.
#include "cb/source/vxd/disassembly.hpp"

#include <chrono>
#include <cmath>
#include <sstream>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace vx = cb::source::vxd;
namespace le = cb::formats::le;
using cb::Bytes;

namespace {

le::Module driver(const char* name) { return le::Module::open(cb::testing::data_path(std::string("formats/testdata/") + name)); }

std::string golden(const char* name) {
    std::string text = cb::fs::read_text(cb::testing::data_path(std::string("source/vxd/testdata/") + name));
    return cb::text::replace_all(text, "\r\n", "\n");
}

bool has(std::string_view text, std::string_view part) { return text.find(part) != std::string_view::npos; }

// The services the goldens name (DDK: INC32/VMM.INC, IFSMGR.INC, DOSMGR.INC, VNETBIOS.INC), each at its ordinal.
vx::DdkNames golden_names() {
    cb::OrderedMap<std::string, std::vector<std::string>> tables;
    const auto add = [&](const char* device, std::initializer_list<std::pair<std::size_t, const char*>> names) {
        std::vector<std::string>& list = tables[device];
        for (const auto& [ordinal, name] : names) {
            if (list.size() <= ordinal) list.resize(ordinal + 1);
            list[ordinal] = name;
        }
    };
    add("VMM", {{81, "_HeapFree"}, {144, "Hook_Device_Service"}, {155, "List_Create"}, {156, "List_Destroy"}, {157, "List_Allocate"}, {159, "List_Attach_Tail"},
                {161, "List_Remove"}, {162, "List_Deallocate"}, {163, "List_Get_First"}, {164, "List_Get_Next"}, {165, "List_Remove_First"}, {284, "Unhook_Device_Service"}});
    add("DOSMGR", {{4, "DOSMGR_Get_IndosPtr"}});
    add("IFSMGR", {{13, "IFSMgr_GetHeap"}, {14, "IFSMgr_RetHeap"}, {17, "IFSMgr_FillHeapSpare"}});
    add("VNETBIOS", {{0, "VNETBIOS_Get_Version"}, {2, "VNETBIOS_Submit"}});
    return vx::DdkNames(vx::ServiceTables(std::move(tables)), {}, {});
}

}  // namespace

CB_TEST("source.vxd.disassembly.device_names_and_branch_operands", [] {
    CB_CHECK_EQ(std::string(*vx::device_name(1)), "VMM");
    CB_CHECK_EQ(std::string(*vx::device_name(0x40)), "IFSMgr");
    CB_CHECK_EQ(std::string(*vx::device_name(0x0449)), "VWIN32");
    CB_CHECK_EQ(std::string(*vx::device_name(0x0480)), "NDIS2SUP");
    CB_CHECK(!vx::device_name(0) && !vx::device_name(0x1F) && !vx::device_name(0x097C) && !vx::device_name(0xFFFFFFFF));

    CB_CHECK_EQ(*vx::direct_operand("0x401000"), 0x401000);
    CB_CHECK_EQ(*vx::direct_operand("  0x10  "), 0x10);
    CB_CHECK(!vx::direct_operand("0x") && !vx::direct_operand("eax") && !vx::direct_operand("dword ptr [0x10]") && !vx::direct_operand("") &&
             !vx::direct_operand("0x10, 1"));
    const auto target = [](const char* mnemonic, const char* operands) {
        vx::Instruction i;
        i.mnemonic = mnemonic, i.operands = operands;
        return i.direct_target();
    };
    CB_CHECK_EQ(*target("call", "0x1234"), 0x1234);
    CB_CHECK_EQ(*target("jne", "0x20"), 0x20);
    CB_CHECK_EQ(*target("jmp", "0x20"), 0x20);
    CB_CHECK_EQ(*target("loop", "0x8"), 8);
    CB_CHECK_EQ(*target("jecxz", "0x8"), 8);
    CB_CHECK(!target("mov", "0x20") && !target("call", "dword ptr [eax]") && !target("ret", ""));
});

CB_TEST("source.vxd.disassembly.string_ranges_gates", [] {
    using namespace std::string_literals;
    const auto ranges = [](const std::string& text) { return vx::string_ranges(cb::as_bytes(text)); };
    using R = std::vector<std::pair<std::size_t, std::size_t>>;
    // (each answer is the Python module's on the same bytes)
    CB_CHECK(ranges("\x01\x02Hello World\x00\x03"s) == (R{{2, 13}}));          // NUL-terminated, 11 printable, vowels, letters
    CB_CHECK(ranges("\x01Hello W\x00"s).empty());                              // seven: too short
    CB_CHECK(ranges("\x01Hello Wo\x00"s) == (R{{1, 9}}));                      // eight is enough
    CB_CHECK(ranges("\x01" "BCDFGHJKLM\x00"s).empty());                        // no vowel
    CB_CHECK(ranges("\x01" "1234567890123456\x00"s).empty());                  // not mostly letters
    CB_CHECK(ranges("\x01" "ABCDEFGHIJKLMNOPQRSTU\x02") == (R{{1, 22}}));     // 21 long: no NUL needed
    CB_CHECK(ranges("\x01" "ABCDEFGHIJKLMNOPQRS\x02").empty());               // 19 and not terminated
    CB_CHECK(ranges("\x01" "abcdefghi12345\x00"s) == (R{{1, 15}}));            // 9 letters of 14: over 60%
    CB_CHECK(ranges("\x01" "abcdefgh123456\x00"s).empty());                    // 8 of 14: under
    CB_CHECK(ranges(std::string("Alpha Beta\x00\x07" "Gamma Delta\x00", 24)) == (R{{0, 10}, {12, 23}}));
    CB_CHECK(ranges("Hello World").empty());                                 // ends the buffer: neither terminated nor long
    CB_CHECK(ranges("x" "abcdefghijklmnopqrstuvwxyz") == (R{{0, 27}}));       // long, at the very end
    CB_CHECK(ranges("D$ ;D$$u\x00\x01"s).empty());                             // code that happens to be printable
    CB_CHECK(ranges("").empty());
});

CB_TEST("source.vxd.disassembly.splitter_listing_matches_the_python_module", [] {
    const le::Module splitter = driver("splitter.vxd");
    const vx::DdkNames names = golden_names();
    const vx::Disassembly code = vx::Disassembly::run(splitter, names);
    const std::string listing = vx::emit_asm(code, names);
    CB_CHECK_EQ(listing.size(), golden("splitter.asm.golden").size());
    CB_CHECK(listing == golden("splitter.asm.golden"));
    CB_CHECK_EQ(listing.size(), 18059u);

    // what the Python module counted
    CB_CHECK_EQ(code.instructions().size(), 501u);
    CB_CHECK_EQ(code.code_bytes(1), 1415u);
    CB_CHECK_EQ(code.labels().size(), 64u);
    CB_CHECK(code.jump_tables().empty());
    CB_CHECK(code.service_table() && code.service_table()->object == 1 && code.service_table()->base == 1416 && code.service_table()->count == 5);
    // the entry points the DDB gives, named for what they are; the service handlers by the driver's own numbering
    const auto label = [&](std::int64_t offset) { return code.labels().at(vx::Place{1, offset}); };
    CB_CHECK_EQ(label(153), "Splitter_Control");
    CB_CHECK_EQ(label(315), "Splitter_Service_00");
    CB_CHECK_EQ(label(322), "Splitter_Service_01");
    CB_CHECK_EQ(label(494), "Splitter_Service_02");
    CB_CHECK_EQ(label(600), "Splitter_Service_03");
    CB_CHECK_EQ(label(671), "Splitter_Service_04");
    CB_CHECK_EQ(label(0x15), "loc_1_0015");
    CB_CHECK_EQ(label(0x34), "sub_1_0034");   // a call target reads as a procedure
    CB_CHECK(code.ddb() && code.ddb()->name == "Splitter" && code.ddb()->device_id == 0x498 && code.ddb()->major == 3 && code.ddb()->minor == 10);

    // an INT 20h service call is one instruction of six bytes: the DWORD belongs to it
    const vx::Instruction& heap = code.instructions().at(vx::Place{1, 7});
    CB_CHECK(heap.service && heap.service->device == 0x40 && heap.service->service == 0x0d && !heap.service->from_import);
    CB_CHECK_EQ(heap.size, 6u);
    CB_CHECK((heap.raw == Bytes{0xcd, 0x20, 0x0d, 0x00, 0x40, 0x00}));
    CB_CHECK_EQ(heap.mnemonic, "int");
    CB_CHECK_EQ(heap.operands, "20h");
    // a relocated operand carries its fix-up
    const vx::Instruction& load = code.instructions().at(vx::Place{1, 0x15});
    CB_CHECK_EQ(load.mnemonic, "mov");
    CB_CHECK(load.fixup && std::holds_alternative<le::InternalFixup>(*load.fixup));
    CB_CHECK(code.is_code(vx::Place{1, 0}) && code.is_code(vx::Place{1, 0x15}) && !code.is_code(vx::Place{1, 0x5ec}) && !code.is_code(vx::Place{9, 0}) &&
             !code.is_code(vx::Place{1, -1}) && !code.is_code(vx::Place{1, 100000}));
    CB_CHECK(code.code(1).size() == 0x604 && code.code(0).empty() && code.code(2).empty());

    // every byte is claimed by one instruction or by none
    const auto checks = vx::verify(code);
    CB_CHECK_EQ(checks.size(), 1u);
    CB_CHECK(checks[0].object == 1 && checks[0].size == 1540 && checks[0].code == 1415 && checks[0].data == 125 && checks[0].overlap == 0 && checks[0].overrun == 0);

    // the names change what the listing says and nothing else: without the DDK a service is `VxDCall IFSMgr.Service_000Dh`
    const std::string bare = vx::emit_asm(splitter, vx::DdkNames());
    CB_CHECK(has(bare, "VxDCall  IFSMgr.Service_000Dh"));
    CB_CHECK(has(bare, "VMMCall  VMM.Service_0090h"));
    CB_CHECK(has(listing, "VMMCall  VMM.Hook_Device_Service"));
});

CB_TEST("source.vxd.disassembly.srammtd_listing_matches_the_python_module", [] {
    const le::Module module = driver("srammtd.vxd");
    const vx::DdkNames names = golden_names();
    const vx::Disassembly code = vx::Disassembly::run(module, names);
    const std::string listing = vx::emit_asm(code, names);
    CB_CHECK(listing == golden("srammtd.asm.golden"));
    CB_CHECK_EQ(listing.size(), 16200u);
    CB_CHECK_EQ(code.instructions().size(), 284u);
    CB_CHECK_EQ(code.code_bytes(1), 744u);
    CB_CHECK_EQ(code.labels().size(), 44u);
    CB_CHECK(code.jump_tables().empty() && !code.service_table());
    CB_CHECK_EQ(code.labels().at(vx::Place{1, 529}), "SRAMMTD_Control");
    const auto checks = vx::verify(code);
    CB_CHECK(checks.size() == 1 && checks[0].size == 1648 && checks[0].code == 744 && checks[0].data == 904 && checks[0].overlap == 0 && checks[0].overrun == 0);
    // a device the table does not know is named by its number
    CB_CHECK(has(listing, "VxDCall  Dev_097C.Service_0001h"));
    CB_CHECK(has(listing, "VMMCall  VMM._HeapFree"));
});

CB_TEST("source.vxd.disassembly.a_larger_driver", [] {
    const le::Module bios = driver("bios.vxd");
    const vx::DdkNames names{};
    const vx::Disassembly code = vx::Disassembly::run(bios, names);
    CB_CHECK(code.instructions().size() > 1000);
    CB_CHECK(code.ddb().has_value());
    for (const vx::ObjectCheck& c : vx::verify(code)) {
        CB_CHECK_EQ(c.overlap, 0u);
        CB_CHECK_EQ(c.overrun, 0u);
        CB_CHECK_EQ(c.code + c.data, c.size);
    }
    // an instruction never starts inside another, every label is a place that exists, every table entry is an address
    std::size_t covered = 0;
    for (const auto& [at, insn] : code.instructions()) {
        covered += insn.size;
        CB_CHECK(code.is_code(at));
    }
    std::size_t total = 0;
    for (const le::Object& o : bios.objects()) total += code.code_bytes(o.index);
    CB_CHECK_EQ(covered, total);
    const std::string listing = vx::emit_asm(code, names);
    CB_CHECK(has(listing, "Declare_Virtual_Device "));
    CB_CHECK(has(listing, "; ---- Device Descriptor Block ----"));
    // the listing is the same however many times it is made
    CB_CHECK(listing == vx::emit_asm(bios, names));
});

CB_TEST("source.vxd.disassembly.names_from_a_symbol_file", [] {
    // DEBUG/SPLITTER.SYM names the function at 0x34 (a call target: sub_1_0034 in the plain listing), one that is no function start,
    // a name that a label has already, and the same offset twice (the first name wins)
    cb::fs::ScopedTempDir tmp("cb-sym");
    const auto record = [](std::uint16_t offset, std::string_view name) {
        Bytes out = {static_cast<std::uint8_t>(offset), static_cast<std::uint8_t>(offset >> 8), static_cast<std::uint8_t>(name.size())};
        const Bytes text = cb::to_bytes(name);
        out.insert(out.end(), text.begin(), text.end());
        return out;
    };
    Bytes sym;
    for (const Bytes& r : {record(0x34, "Splitter_Return_Heap"), record(0x35, "Not_A_Function_Start"), record(0x47, "Splitter_Control"), record(0x34, "Second_Name_Loses")}) cb::append(sym, r);
    cb::fs::write_file(tmp.path() / "SPLITTER.SYM", sym);
    vx::DdkFolders folders;
    folders.debug = {tmp.path()};
    const vx::DdkNames names(golden_names().services(), {}, folders);
    const le::Module splitter = driver("splitter.vxd");
    const std::string listing = vx::emit_asm(splitter, names);
    const std::string plain = vx::emit_asm(splitter, golden_names());
    CB_CHECK(listing != plain);
    // the only change is the one name, in the label and in every reference to it
    CB_CHECK_EQ(cb::text::replace_all(plain, "sub_1_0034", "Splitter_Return_Heap"), listing);
    CB_CHECK(!has(listing, "Not_A_Function_Start") && !has(listing, "Second_Name_Loses"));
    CB_CHECK(has(listing, "\nSplitter_Return_Heap:\n"));
    // without a file for the device nothing is renamed
    vx::DdkFolders empty;
    empty.debug = {tmp.path() / "no-such-folder"};
    CB_CHECK_EQ(vx::emit_asm(splitter, vx::DdkNames(golden_names().services(), {}, empty)), plain);
});

CB_TEST("source.vxd.disassembly.control_messages_name_the_handlers", [] {
    // The control procedure of a driver that dispatches on the message number through a table (`sub eax, 1; cmp; ja; jmp [eax*4+table]`)
    // gives each handler the name of the message it services; a handler shared by two messages keeps its own name.
    const vx::DdkNames named(vx::ServiceTables(), {{0, "SYS_CRITICAL_INIT"}, {1, "DEVICE_INIT"}, {2, "INIT_COMPLETE"}, {3, "SYS_VM_INIT"}, {4, "SYS_VM_TERMINATE"},
                                                   {5, "SYS_VM_NOT_EXECUTEABLE"}, {6, "CREATE_VM"}, {7, "VM_CRITICAL_INIT"}, {8, "VM_INIT"}, {9, "VM_TERMINATE"}},
                             vx::DdkFolders());
    for (const char* file : {"splitter.vxd", "srammtd.vxd", "bios.vxd"}) {
        const le::Module module = driver(file);
        const vx::Disassembly with = vx::Disassembly::run(module, named);
        const vx::Disassembly without = vx::Disassembly::run(module, vx::DdkNames());
        // the names never touch the instructions, the tables or the entry points
        CB_CHECK_EQ(with.instructions().size(), without.instructions().size());
        CB_CHECK(with.jump_tables() == without.jump_tables());
        CB_CHECK_EQ(with.labels().size(), without.labels().size());
        std::size_t renamed = 0;
        for (const auto& [at, name] : with.labels()) {
            if (name != without.labels().at(at)) {
                ++renamed;
                CB_CHECK(without.labels().at(at).starts_with("sub_") || without.labels().at(at).starts_with("loc_"));
                CB_CHECK(name.starts_with(with.ddb()->name + "_"));
            }
        }
        cb::log::debug(std::format("{}: {} handlers named", file, renamed));
    }
});

CB_TEST("source.vxd.disassembly.files_and_commands", [] {
    cb::fs::ScopedTempDir tmp("cb-vxd-listing");
    const vx::DdkNames names = golden_names();
    // disassemble_file: the stats of a module and the listing written beside
    const std::filesystem::path splitter = cb::testing::data_path("formats/testdata/splitter.vxd");
    const auto stats = vx::disassemble_file(splitter, names, tmp.path() / "listing");
    CB_CHECK_EQ(stats.size(), 1u);
    CB_CHECK_EQ(stats[0].name, "splitter.vxd");
    CB_CHECK_EQ(stats[0].instructions, 501u);
    CB_CHECK_EQ(stats[0].labels, 64u);
    CB_CHECK_EQ(stats[0].jump_tables, 0u);
    CB_CHECK(stats[0].device == "Splitter" && stats[0].device_id == 0x498u);
    CB_CHECK(std::abs(stats[0].coverage_percent - 100.0 * 1415 / 1540) < 1e-9);
    CB_CHECK_EQ(cb::fs::read_text(tmp.path() / "listing" / "splitter.vxd.asm"), golden("splitter.asm.golden"));
    CB_CHECK_NOTHROW((void)vx::disassemble_file(splitter, names));   // no folder: nothing written
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)vx::disassemble_file(tmp.path() / "no-such.vxd", names));

    // the command over a folder of three drivers
    cb::fs::make_dirs(tmp.path() / "drivers");
    for (const char* name : {"splitter.vxd", "srammtd.vxd", "bios.vxd"}) std::filesystem::copy_file(cb::testing::data_path(std::string("formats/testdata/") + name), tmp.path() / "drivers" / name);
    cb::fs::write_text(tmp.path() / "drivers" / "notes.txt", "not a driver");
    cb::fs::write_file(tmp.path() / "drivers" / "broken.vxd", Bytes(100, 3));
    cb::base::Products products;
    products.checkout_root = tmp.path() / "checkout";
    products.state = tmp.path() / "state";
    const cb::base::Corpus corpus(products);
    const vx::Environment env{corpus, names, tmp.path() / "default-listing"};
    const auto run = [&](std::vector<std::string> args) { return vx::main(env, args); };
    cb::testing::CapturedStd captured;
    CB_CHECK_EQ(run({"--verify", (tmp.path() / "drivers").string()}), cb::kExitOk);
    CB_CHECK_EQ(run({"--all", (tmp.path() / "drivers").string(), (tmp.path() / "all").string()}), cb::kExitOk);
    CB_CHECK_EQ(run({"--all", (tmp.path() / "drivers").string()}), cb::kExitOk);   // the default listing folder
    CB_CHECK_EQ(run({splitter.string()}), cb::kExitOk);
    const std::string out = captured.out();
    CB_CHECK(has(out, "== verify: 3 modules, 3 CLEAN, 0 dirty | code="));
    CB_CHECK(has(out, "overlap=0 overrun=0 =="));
    CB_CHECK(has(out, "  ERROR broken.vxd       "));
    CB_CHECK(has(out, "  splitter.vxd Splitter  id=0x0498     501 insns   92% cov  0 jt"));
    CB_CHECK(has(out, "== 3 modules -> "));
    CB_CHECK(has(out, ", 1 errors =="));
    CB_CHECK(has(out, "; Splitter -- reconstructed 32-bit disassembly (VxD / LE format)"));
    CB_CHECK(std::filesystem::exists(tmp.path() / "all" / "splitter.vxd.asm") && std::filesystem::exists(tmp.path() / "all" / "srammtd.vxd.asm") &&
             std::filesystem::exists(tmp.path() / "all" / "bios.vxd.asm"));
    CB_CHECK(std::filesystem::exists(tmp.path() / "default-listing" / "bios.vxd.asm"));
});

// The Python module's counts on its whole harness (273 real drivers of the Windows 95 corpus), and the time this takes: with
// CB_VXD_DISASSEMBLY_HARNESS=<...>/state/work/typed-port/source.vxd.disassembly (its expected.txt names the drivers by their corpus path). The old
// C++ port's time for the same 273 recoveries, measured by that harness on the same machine, is 19.4 s (rc_run.log, `disasm`).
CB_TEST("source.vxd.disassembly.agrees_with_the_python_module_on_its_harness", [] {
    const auto folder = cb::fs::get_env("CB_VXD_DISASSEMBLY_HARNESS");
    if (!folder) return;
    std::istringstream in(cb::fs::read_text(std::filesystem::path(*folder) / "expected.txt"));
    std::size_t compared = 0, differ = 0;
    std::string first;
    double seconds = 0, emit_seconds = 0;
    std::size_t listing_bytes = 0;
    for (std::string line; std::getline(in, line);) {
        const std::vector<std::string_view> fields = cb::text::split(line, '	');
        if (fields.size() < 4 || fields[0] != "disasm" || !fields[1].starts_with("V:")) continue;
        cb::Json want;
        if (!cb::json::try_parse(fields.back(), want) || !want.is_array() || want.empty() || !want[0].is_number()) continue;   // an error case
        const std::string_view spec = fields[1].substr(2);
        const std::size_t bar = spec.rfind('|');
        const auto members = le::open_modules(std::filesystem::path(std::string(spec.substr(0, bar))));
        const le::Module& module = members.at(static_cast<std::size_t>(*cb::text::parse_int<int>(spec.substr(bar + 1)))).second;
        const auto begin = std::chrono::steady_clock::now();
        const vx::Disassembly code = vx::Disassembly::run(module, vx::DdkNames());
        seconds += std::chrono::duration<double>(std::chrono::steady_clock::now() - begin).count();
        const auto listing_begin = std::chrono::steady_clock::now();
        listing_bytes += vx::emit_asm(code, vx::DdkNames()).size();
        emit_seconds += std::chrono::duration<double>(std::chrono::steady_clock::now() - listing_begin).count();
        std::size_t covered = 0;
        for (const le::Object& o : module.objects()) covered += code.code_bytes(o.index);
        // [instructions, code bytes, data bytes, labels, jump tables, entries, service table, device name, ...]
        const bool same = want[0] == code.instructions().size() && want[1] == covered && want[3] == code.labels().size() && want[4] == code.jump_tables().size() &&
                          (want[6].is_null() ? !code.service_table().has_value()
                                             : (code.service_table() && want[6][0] == code.service_table()->object && want[6][1] == code.service_table()->base &&
                                                want[6][2] == code.service_table()->count)) &&
                          (want[7].is_null() ? !code.ddb().has_value() : (code.ddb() && want[7] == code.ddb()->name));
        ++compared;
        if (!same) {
            ++differ;
            if (differ <= 8) {
                first += std::format("\n  {}: python {} here {} {} {} {}", fields[1].substr(fields[1].rfind('\\') + 1), want.dump().substr(0, 70), code.instructions().size(), covered,
                                     code.labels().size(), code.jump_tables().size());
            }
        }
    }
    CB_CHECK(compared > 200);
    CB_CHECK_EQ(differ, 0u);
    cb::err("disassembly harness: {} drivers, {} differ, recovery {:.2f} s, listings {:.2f} s ({} bytes){}\n", compared, differ, seconds, emit_seconds, listing_bytes, first);
});

// cb-unity: alone
#include "formats/nid_data.hpp"

#include <algorithm>
#include <string>

#include "core/testing.hpp"
#include "formats/orbis.hpp"
#include "formats/ps3.hpp"
#include "formats/psp.hpp"
#include "formats/vita.hpp"

namespace cb::formats {

CB_TEST("formats.nids.the_tables_are_built_in_and_sorted", [] {
    CB_CHECK_EQ(nids::numeric("vita.nids", "functions").rows().size(), 8813u);
    CB_CHECK_EQ(nids::numeric("vita.nids", "libraries").rows().size(), 283u);
    CB_CHECK_EQ(nids::numeric("ps3.nids", "functions").rows().size(), 975u);
    CB_CHECK_EQ(nids::numeric("ps3.nids", "syscalls").rows().size(), 240u);
    CB_CHECK_EQ(nids::numeric("psp.nids", "functions").rows().size(), 556u);
    CB_CHECK_EQ(nids::numeric("orbis.nids", "syscalls").rows().size(), 81u);
    CB_CHECK_EQ(nids::text("orbis.nids", "functions").rows().size(), 686u);
    for (const auto* t : {&nids::numeric("vita.nids", "functions"), &nids::numeric("ps3.nids", "functions"), &nids::numeric("psp.nids", "functions")})
        CB_CHECK(std::is_sorted(t->rows().begin(), t->rows().end(), [](const nids::Row& a, const nids::Row& b) { return a.key < b.key; }));
    CB_CHECK(nids::numeric("nothing.nids", "functions").rows().empty());
    CB_CHECK(nids::numeric("vita.nids", "no_such_section").rows().empty());
});

CB_TEST("formats.nids.vita_ps3_psp_orbis_names_resolve_and_unknown_ids_are_empty", [] {
    CB_CHECK_EQ(vita::function_name(0x0009910A), "sceFiosFHToFileno");
    CB_CHECK_EQ(vita::function_name(0x00000001), "");
    CB_CHECK(!vita::library_name(0xFF4CD67F).empty());
    CB_CHECK_EQ(vita::library_name(0xFF4CD67F), "SceDrmBridge");
    CB_CHECK_EQ(ps3::function_name(0x006C4900), "sslCertGetNameEntryInfo");
    CB_CHECK_EQ(ps3::function_library(0x006C4900), "cellSsl");
    CB_CHECK_EQ(ps3::function_name(0x00000001), "");
    CB_CHECK_EQ(ps3::function_library(0x00000001), "");
    CB_CHECK_EQ(ps3::syscall_name(1), "process_getpid");
    CB_CHECK_EQ(ps3::syscall_name(999999), "");
    CB_CHECK_EQ(psp::nid_name(psp::nid_of("sceDisplaySetMode")), "sceDisplaySetMode");
    CB_CHECK_EQ(psp::nid_name(0xD632ACDBu), "module_start");
    CB_CHECK_EQ(psp::nid_name(0x00000001u), "");
    CB_CHECK_EQ(orbis::function_name(orbis::nid_of("sceKernelUsleep")), "sceKernelUsleep");
    CB_CHECK_EQ(orbis::function_name("wuCroIGjt2g"), "open");
    CB_CHECK_EQ(orbis::function_name("AAAAAAAAAAA"), "");
    CB_CHECK_EQ(orbis::function_name(""), "");
    CB_CHECK_EQ(orbis::syscall_name(4), "write");
    CB_CHECK_EQ(orbis::syscall_name(3000), "");
});

}  // namespace cb::formats

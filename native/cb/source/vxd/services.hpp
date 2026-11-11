// cb/source/vxd/services.hpp -- resolve VxD service ordinals to their DDK names.
//
// A VxD service call is `int 20h` + a DWORD whose high word is the callee's device id and whose low word is the
// service ORDINAL -- the service's position in that device's service table. The Win95 DDK headers declare those
// tables in order:
//
//     Begin_Service_Table VMM, VMM
//     VMM_Service  Get_VMM_Version, LOCAL     ; ordinal 0
//     VMM_Service  Get_Cur_VM_Handle          ; ordinal 1
//     ...
//
// so parsing every `Begin_Service_Table <DEV> ... <DEV>_Service <name>` run recovers a {device: [name by ordinal]} map.
// That turns `VMMCall VMM.Service_003Fh` into `VMMCall Get_System_Time` and names each driver's own exported service
// handlers.
//
//     const ServiceTables tables = ServiceTables::load(ddk_inc_folder);
//     tables.name("VMM", 0x3f)          // "Get_System_Time"
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/source/vxd/ddk.hpp"

namespace cb::source::vxd {

class ServiceTables {
public:
    ServiceTables() = default;
    // Tables made elsewhere: {DEVICE (in capitals): [service name by ordinal]}.
    explicit ServiceTables(OrderedMap<std::string, std::vector<std::string>> tables) : tables_(std::move(tables)) {}

    // Every `Begin_Service_Table` run of every *.INC of the folder, in the order of the files' names; the first table
    // of a device wins.
    static ServiceTables load(const std::filesystem::path& inc_folder);
    // The tables of one include file's text, added to `into` (a device that has a table already keeps it).
    static void parse_into(ServiceTables& into, std::string_view inc_text);
    // The folder of the DDK's INC32 includes: the first of `candidates` that is a folder, else (if `design_dir` is
    // given) an INC32 folder holding VMM.INC found by a shallow walk (four levels) of it that skips version-control,
    // build and corpus folders; nullopt for none.
    static std::optional<std::filesystem::path> find_inc_folder(const DdkFolders& ddk,
                                                                const std::optional<std::filesystem::path>& design_dir);
    // The tables of the DDK found as find_inc_folder() says; none when it is not there.
    static ServiceTables standard(const DdkFolders& ddk, const std::optional<std::filesystem::path>& design_dir);

    // {DEVICE: [service name by ordinal]}, in the order the devices were first met.
    const OrderedMap<std::string, std::vector<std::string>>& tables() const { return tables_; }

    // The device's service names (the device is matched in capitals), or nullptr.
    const std::vector<std::string>* names(std::string_view device) const;

    // The service name for (device, ordinal), or nullopt. The low 15 bits are the table index; the top bit is a
    // call-convention flag on some references, so the masked ordinal is tried too.
    std::optional<std::string> name(std::string_view device, std::uint32_t ordinal) const;

private:
    OrderedMap<std::string, std::vector<std::string>> tables_;   // iterated by tools that list the tables, in file order
};

}  // namespace cb::source::vxd

// cb/symbols/driver_services.hpp -- the real names of the services ring-0 drivers call.
//
// A virtual device driver calls the system and other drivers with `int 20h` followed by a service number and a device
// number; the loader turns those six bytes into a call. The numbers mean nothing by themselves. The driver kit's include
// files give each device its number (`VKD_Device_ID EQU 0000DH`) and list its services in order between
// `Begin_Service_Table VKD` and `End_Service_Table` -- the Nth `VKD_Service` line is service N. That list, read once from the
// include files, is kept beside the program as driver_services.json; the program only reads the table (through the knowledge
// base: platform vxd, kinds device, service_table, driver, message).
//
//     const Table table = Table::from_rows(devices, services, drivers, messages);
//     table.service_name(0x0001, 0x0001)     "Get_Cur_VM_Handle"
//     table.service_name(0x000D, 0x0001)     "VKD_Define_Hot_Key"
//     table.device_name(0x000D)              "VKD"
//     table.label(0x0001, 0x0123)            the real name, else a placeholder
//     table.message_name(1)                  "DEVICE_INIT"   (what a control procedure is sent)
//
//     const KitTables kit = read_include_files(folder);       // making the table
//     format_table(kit, corpus_drivers, read_messages(folder))
//
// Bit 15 of a service number says "jump, do not call"; it is not part of the number. Device numbers the kit does not name
// are filled in from the corpus: every driver states its own number and name in its device descriptor block (kept apart,
// under "drivers", because that is what a file says about itself, not what the kit publishes).
//
// This module reads no store and no corpus: the table comes from the rows the knowledge base holds, the corpus's driver names
// and service calls come from the caller (formats/le), and the file the table is written to is the caller's.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/knowledge/sources.hpp"

namespace cb::symbols::driver_services {

// Bit 15 of a service number: jump, do not call. The number is the rest.
inline constexpr std::uint32_t kServiceNumberMask = 0x7FFF;

class Table {
public:
    // (devices with a service table, services named, device numbers named)
    struct Size {
        std::size_t service_tables = 0;
        std::size_t services_named = 0;
        std::size_t devices_named = 0;
    };

    // The four kinds of the knowledge base, each {number in hex: value}: the kit's device names, the services of each
    // device in order, the names corpus drivers give themselves, the system control messages. FormatError for a number that
    // is not hex, a name that is not text, or a service list that is not a list of names.
    static Table from_rows(const knowledge::Rows& devices, const knowledge::Rows& services, const knowledge::Rows& drivers,
                           const knowledge::Rows& messages);

    // The name of a device number: the kit's, else what the corpus driver with that number calls itself.
    std::optional<std::string> device_name(std::uint32_t device) const;
    // The kit's name for service `service` of device `device`.
    std::optional<std::string> service_name(std::uint32_t device, std::uint32_t service) const;
    // A name to print for a service call: the real one when the table has it; for a known device with a service past its
    // table, DEVICE_Service_NNNN; else the number alone (VMM_NNNN for the system, VxD_DDDD_NNNN).
    std::string label(std::uint32_t device, std::uint32_t service) const;
    // The name of a message the system sends every driver's control procedure.
    std::optional<std::string> message_name(std::uint32_t number) const;
    const std::map<std::uint32_t, std::string>& messages() const { return messages_; }
    Size size() const;

private:
    std::map<std::uint32_t, std::string> devices_, drivers_, messages_;
    std::map<std::uint32_t, std::vector<std::string>> services_;   // a gap in the list is an empty name
};

// ---- how the table does on a driver ---------------------------------------------------------------------------------

struct ServiceCall {
    std::uint32_t device = 0;
    std::uint32_t service = 0;   // as the call states it, bit 15 included
};

struct Coverage {
    std::size_t calls = 0;
    std::size_t named = 0;
    std::map<std::pair<std::uint32_t, std::uint32_t>, std::size_t> missing;   // (device, service number) -> how many calls
};

Coverage coverage(const Table& table, std::span<const ServiceCall> calls);

// "N service tables, N services named, N device numbers named"
std::string size_line(const Table::Size& size);
// One driver's line: its name, the calls, the named ones, and up to six unnamed (device, service) by number.
std::string coverage_line(const Table& table, std::string_view file_name, const Coverage& coverage);

// ---- making the table -----------------------------------------------------------------------------------------------

// Where the driver kit's 32-bit include files are, or an empty path: `kit_folder` (the environment's CB_DRIVER_KIT; may be
// empty) and the kit under the reference folder beside the corpus; the first that holds VMM.INC.
std::filesystem::path find_include_dir(std::string_view kit_folder, const std::filesystem::path& reference_folder);

// A device number as the include files write it: 0000DH, 38H, 0x28, another device's name, or NAME + n. `known` holds the
// numbers of the names (capitals) an expression may use. nullopt when it cannot be worked out (or does not fit 32 bits).
std::optional<std::uint32_t> parse_device_number(std::string_view text, const std::map<std::string, std::uint32_t>& known);

struct KitTables {
    std::map<std::uint32_t, std::string> devices;                  // device number -> name
    std::map<std::uint32_t, std::vector<std::string>> services;    // device number -> service names in order
};

// The devices and service lists of every .INC and .H in `folder`. A header's assembly block lists services the same way an
// include file does; where two tables claim one device number (the old block device and its successor), the longer one is
// kept, as the newer driver answers the older one's services first. IoError when the folder cannot be read.
KitTables read_include_files(const std::filesystem::path& folder);

// {number: name} of the system control messages, from the kit's main include file: the run of four-digit equates from
// SYS_CRITICAL_INIT up to MAX_SYSTEM_CONTROL.
std::map<std::uint32_t, std::string> read_messages(const std::filesystem::path& folder);

// The text of driver_services.json for the kit's tables, the device names the corpus's drivers state for themselves (those
// the kit already names are left out), and the messages.
std::string format_table(const KitTables& kit, const std::map<std::uint32_t, std::string>& corpus_drivers,
                         const std::map<std::uint32_t, std::string>& messages);

}  // namespace cb::symbols::driver_services

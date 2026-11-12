// codebreaker/symbols/driver_services.hpp -- compat shim for cb/symbols/driver_services.hpp (docs/CPP_DESIGN.md 6.3); driver_services.cpp is its translation unit
//
// driver_services.py -- the real names of the services ring-0 drivers call.
//
// A virtual device driver calls the system and other drivers with `int 20h` followed by a
// service number and a device number; the loader turns those six bytes into a call. The numbers
// mean nothing by themselves. The driver kit's include files give each device its number
// (`VKD_Device_ID EQU 0000DH`) and list its services in order between `Begin_Service_Table VKD`
// and `End_Service_Table` -- the Nth `VKD_Service` line is service N. That list, read once from
// the include files, is kept beside this module in driver_services.json; the package only reads
// the table (through the knowledge base: platform vxd, kinds device, service_table, driver, message).
//
//     driver_services::service_name(0x0001, 0x0001)     -> "Get_Cur_VM_Handle"
//     driver_services::service_name(0x000D, 0x0001)     -> "VKD_Define_Hot_Key"
//     driver_services::device_name(0x000D)              -> "VKD"
//     driver_services::label(0x0001, 0x0123)            -> the real name, else a placeholder
//     driver_services::message_name(1)                  -> "DEVICE_INIT"   (what a control procedure is sent)
//     driver_services::generate()                       -> rewrite the table from the include files
//
//     python -m codebreaker driver-services [FILE ...]      the table's size; with driver files,
//                                                           how many of their service calls it names
//     python -m codebreaker driver-services --generate [INCLUDE_DIR]
//
// Bit 15 of a service number says "jump, do not call"; it is not part of the number. Device
// numbers the kit does not name are filled in from the corpus: every driver states its own
// number and name in its device descriptor block (kept apart, under "drivers", because that is
// what a file says about itself, not what the kit publishes).
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * `knowledge.table("vxd", kind)` (codebreaker.knowledge's __init__, not ported) is `knowledge_table()`: a
//     std::function giving the rows of an authored kind.  Its default is knowledge::sources::authored (ported) over
//     an in-memory stand-in for the base's store (a kind is read from its file once per file stamp); a program
//     that has the real base, or a test, sets its own.  `_loaded` (the cache the Python resets by assigning None)
//     is `_loaded()`.
//   * The tables are the struct Table: {device number: name}, {device number: [service names]}, ...  A row whose
//     key is no hex number is the ValueError int(k, 16) raises; a name that is not a str is a TypeError.
//   * TABLE (driver_services.json beside the module) is `TABLE()`: the module's folder is the knowledge package's
//     (CB_CHECKOUT_ROOT's codebreaker\symbols).
//   * main(argv) takes the arguments and gives the exit status; it prints with py::print.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/knowledge/sources.hpp"
#include "py.hpp"

namespace codebreaker::symbols::driver_services {

// driver_services.json, beside this module
py::str TABLE();

// The rows of an authored kind of the knowledge base: knowledge.table(platform, kind).
std::function<knowledge::sources::Rows(const py::str&, const py::str&)>& knowledge_table();

struct Table {
    py::dict<int64_t, py::str> devices;                      // device number -> name the kit gives
    py::dict<int64_t, py::list<py::str>> services;           // device number -> service names in order
    py::dict<int64_t, py::str> drivers;                      // device number -> name the corpus driver gives itself
    py::dict<int64_t, py::str> messages;                     // message number -> name
};
std::optional<Table>& _loaded();
const Table& _table();

// The name of a device number: the kit's, else what the corpus driver with that number calls itself, else None.
std::optional<py::str> device_name(int64_t device);
// The kit's name for service `service` of device `device`, or None.
std::optional<py::str> service_name(int64_t device, int64_t service);
// A name to print for a service call: the real one when the table has it; for a known device with a service
// past its table, DEVICE_Service_NNNN; else the number alone.
py::str label(int64_t device, int64_t service);
// {number: name} of the messages the system sends every driver's control procedure.
py::dict<int64_t, py::str> messages();
std::optional<py::str> message_name(int64_t number);
// (devices with a service table, services named, device numbers named)
std::tuple<int64_t, int64_t, int64_t> size();

// Making the table -----------------------------------------------------------------------------
// Where the driver kit's 32-bit include files are on this machine, or ''.
py::str include_dir();
// A device number as the include files write it: 0000DH, 38H, 0x28, another device's name, or NAME + n.
// None when it cannot be worked out.
std::optional<int64_t> _number(const py::str& text, const py::dict<py::str, int64_t>& known);
// -> ({device number: name}, {device number: [service names in order]}) from every .INC and .H in `folder`.
std::tuple<py::dict<int64_t, py::str>, py::dict<int64_t, py::list<py::str>>> read_include_files(const py::str& folder);
// {number: name} of the system control messages, from the kit's main include file.
py::dict<int64_t, py::str> read_messages(const py::str& folder);
// {device number: name} as every driver in the corpus states them in its own descriptor.
py::dict<int64_t, py::str> corpus_drivers();
// Rewrite the table from the include files in `folder` (None: include_dir()). -> size()
std::tuple<int64_t, int64_t, int64_t> generate(const std::optional<py::str>& folder = std::nullopt, const std::optional<py::str>& out = std::nullopt);
// How the table does on one driver file: -> (service calls, named, {unnamed (device, service): count}).
std::tuple<int64_t, int64_t, py::dict<std::tuple<int64_t, int64_t>, int64_t>> coverage(const py::str& path);
// python -m codebreaker driver-services [--generate [INCLUDE_DIR]] [FILE ...]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::symbols::driver_services

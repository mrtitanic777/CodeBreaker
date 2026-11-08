// codebreaker/source/vxd/services.hpp -- typed C++ of codebreaker.source.vxd.services
//
// services.py -- resolve VxD service ordinals to their DDK names.
//
// A VxD service call is `int 20h` + a DWORD whose high word is the callee's device id and whose low
// word is the service ORDINAL -- the service's position in that device's service table. The Win95
// DDK headers declare those tables in order:
//
//     Begin_Service_Table VMM, VMM
//     VMM_Service  Get_VMM_Version, LOCAL     ; ordinal 0
//     VMM_Service  Get_Cur_VM_Handle          ; ordinal 1
//     ...
//
// so parsing every `Begin_Service_Table <DEV> ... <DEV>_Service <name>` run recovers a
// {device: [name-by-ordinal]} map. That turns `VMMCall VMM.Service_003Fh` into
// `VMMCall Get_System_Time` and names each driver's own exported service handlers.
//
//     auto S = services::load();       // autodiscovers the DDK under binaries/
//     S.name("VMM", 0x3f)              // -> "Get_System_Time"
//
// Typing notes:
//   * HERE / ROOT are the Python's import-time constants, as functions (see symbols.hpp).
//   * _parse keeps its answers (functools.lru_cache(maxsize=8), keyed by the folder), so a folder read once is
//     not read again while it stays among the 8 last used, and the same tables are handed each time.
//   * `Services` is a struct: `tables` is the dict, names() and name() are its methods; None is std::nullopt.
//   * glob.glob has no counterpart in the runtime: a small glob lives in the .cpp (see symbols.hpp).
//   * _find_inc_dir takes the candidates from _candidates() (the Python's); _find_inc_dir_in(cands) is the same
//     search over a list given (the seam a test uses to reach the bounded walk, which the real candidates
//     rarely leave to run).
//   * The Python's `if __name__ == "__main__"` block (a printout of the tables) is not ported.
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::services {

py::str HERE();   // codebreaker/source/vxd
py::str ROOT();   // codebreaker

py::list<py::str> _candidates();

// directories never worth descending into when searching for the DDK headers
extern const py::set<py::str> _PRUNE;

std::optional<py::str> _find_inc_dir();
std::optional<py::str> _find_inc_dir_in(const py::list<py::str>& candidates);

// {DEVICE: [service name by ordinal]} of every *.INC of the folder (the first table of a device wins)
py::dict<py::str, py::list<py::str>> _parse(const py::str& inc_dir);

struct Services {
    py::dict<py::str, py::list<py::str>> tables;

    explicit Services(const py::dict<py::str, py::list<py::str>>& t) : tables(t) {}

    // the device's service names (the device is matched in capitals; None for no device), or None
    std::optional<py::list<py::str>> names(const std::optional<py::str>& device) const;

    // Service name for (device, ordinal), or None. The low 15 bits are the table index; the
    // top bit is a call-convention flag on some references, so try the masked ordinal too.
    std::optional<py::str> name(const std::optional<py::str>& device, const std::optional<int64_t>& ordinal) const;
};

// inc_dir None or "": the folder _find_inc_dir() finds (none: no tables)
Services load(const std::optional<py::str>& inc_dir = std::nullopt);

}  // namespace codebreaker::source::vxd::services

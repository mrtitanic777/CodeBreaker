// codebreaker/source/vxd/symbols.hpp -- typed C++ of codebreaker.source.vxd.symbols
//
// symbols.py -- recover VxD function/handler names from the Win95 DDK.
//
// Three DDK sources name things the raw disassembly can only call sub_XXXX:
//
//   * control_messages()  -- the system control messages (SYS_CRITICAL_INIT=0, DEVICE_INIT=1, ...,
//     from VMM.INC), so a control-dispatch handler can be named after the message it services.
//   * sym_names(name)     -- the DDK debug .SYM files (DEBUG/<DEV>.SYM, MAPSYM output) carry the REAL
//     internal function names (VKD_Filter_Keyboard_Input, VKD_Put_Byte, ...) for 12 core VxDs. Record
//     format: <u16 offset><u8 namelen><name>. NOTE these are the DEBUG build's offsets; the retail
//     oracle differs, so a caller applies a name only where the offset lands on a real function start.
//
// The service names live in engine.inverse_engine.vxd_services (the exported interface); this module
// adds the control handlers and the internal functions.
//
// Typing notes:
//   * HERE / ROOT / _INC / _DEBUG are the Python's import-time constants; they are functions here,
//     worked out on first use (HERE from the checkout root, as py::settings gives it: the Python takes it
//     from __file__) and then kept, as the import-time values are.
//   * control_messages() keeps its answer for the process (functools.lru_cache(maxsize=1)) and hands the
//     same dict each time.
//   * glob.glob has no counterpart in the runtime: a small glob lives in the .cpp (the rules of glob.glob
//     on Windows: wildcards in any part of the path, names matched without regard to case, no hidden names
//     for a pattern not starting with a dot).
//   * The Python's `if __name__ == "__main__"` block (a printout of the counts) is not ported.
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::vxd::symbols {

py::str HERE();   // codebreaker/source/vxd
py::str ROOT();   // codebreaker

const py::list<py::str>& _INC();     // the folders VMM.INC may be in
const py::list<py::str>& _DEBUG();   // the folders the .SYM files may be in

// The first of `cands` (joined to ROOT, which an absolute one replaces) that exists, with `leaf` under it.
std::optional<py::str> _find(const py::list<py::str>& cands, const py::str& leaf = py::str(""));

// {message_number: MESSAGE_NAME} for the system control messages, from VMM.INC.
py::dict<int64_t, py::str> control_messages();

// {offset: name} from DEBUG/<device>.SYM, or {} if there is no symbol file for it.
py::dict<int64_t, py::str> sym_names(const py::str& device);

}  // namespace codebreaker::source::vxd::symbols

// codebreaker/source/vxd/symbols.hpp -- compat shim for cb/source/vxd/symbols.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer.
//
//   * control_messages()  -- the system control messages (SYS_CRITICAL_INIT=0, DEVICE_INIT=1, ..., from VMM.INC), so a
//     control-dispatch handler can be named after the message it services.
//   * sym_names(name)     -- the DDK debug .SYM files (DEBUG/<DEV>.SYM) carry the REAL internal function names for 12
//     core VxDs. Record format: <u16 offset><u8 namelen><name>.
//
// control_messages() keeps its answer for the process (functools.lru_cache(maxsize=1)) and hands the same dict each
// time. The Python module's import-time constants and its glob (HERE, ROOT, _INC, _DEBUG, _find) belonged to its
// implementation; the folders are cb::source::vxd::DdkFolders now.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/symbols.hpp"
#include "codebreaker/base/corpus.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::vxd::symbols {

namespace detail {
// The DDK folders as the old layer's settings and corpus give them: beside the checkout, the reference folder.
inline const cb::source::vxd::DdkFolders& ddk() {
    static const cb::source::vxd::DdkFolders folders = cb::source::vxd::DdkFolders::standard(
        base::corpus::detail::instance(),
        cb::fs::from_text(py::os::path::dirname(py::settings::checkout_root()).utf8()));   // dirname(settings.ROOT)
    return folders;
}
}  // namespace detail

// {message_number: MESSAGE_NAME} for the system control messages, from VMM.INC.
inline py::dict<int64_t, py::str> control_messages() {
    static const py::dict<int64_t, py::str> kept = cb::compat::guard(
        [] { return cb::compat::to_py(cb::source::vxd::control_messages(detail::ddk())); });
    return kept;
}

// {offset: name} from DEBUG/<device>.SYM, or {} if there is no symbol file for it.
inline py::dict<int64_t, py::str> sym_names(const py::str& device) {
    return cb::compat::guard(
        [&] { return cb::compat::to_py(cb::source::vxd::sym_names(detail::ddk(), cb::compat::from_py(device))); });
}

}  // namespace codebreaker::source::vxd::symbols

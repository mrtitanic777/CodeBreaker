// codebreaker/source/vxd/services.hpp -- compat shim for cb/source/vxd/services.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer.
//
// Resolve VxD service ordinals to their DDK names:
//
//     auto S = services::load();       // autodiscovers the DDK
//     S.name("VMM", 0x3f)              // -> "Get_System_Time"
//
// `Services` keeps its dict (`tables`), with names() and name() as methods; None is std::nullopt. A folder read once is
// not read again while the process runs (the Python kept the 8 last used).
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "cb/compat/py_bridge.hpp"
#include "cb/source/vxd/services.hpp"
#include "codebreaker/base/corpus.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::vxd::services {

namespace detail {
inline const cb::source::vxd::DdkFolders& ddk() {
    static const cb::source::vxd::DdkFolders folders = cb::source::vxd::DdkFolders::standard(
        base::corpus::detail::instance(),
        cb::fs::from_text(py::os::path::dirname(py::settings::checkout_root()).utf8()));   // dirname(settings.ROOT)
    return folders;
}
}  // namespace detail

// The folder of the DDK's INC32 includes, if there is one.
inline std::optional<py::str> _find_inc_dir() {
    return cb::compat::guard([] {
        const auto folder = cb::source::vxd::ServiceTables::find_inc_folder(
            detail::ddk(), base::corpus::detail::instance().design_dir());
        return folder ? std::optional<py::str>(cb::compat::to_py(cb::fs::to_text(*folder))) : std::nullopt;
    });
}

struct Services {
    py::dict<py::str, py::list<py::str>> tables;

    explicit Services(const py::dict<py::str, py::list<py::str>>& t)
        : tables(t), inner_(cb::compat::from_py(t)) {}

    // the device's service names (the device is matched in capitals; None for no device), or None
    std::optional<py::list<py::str>> names(const std::optional<py::str>& device) const {
        const std::vector<std::string>* found = inner_.names(device ? cb::compat::from_py(*device) : std::string());
        if (found == nullptr) return std::nullopt;
        return cb::compat::to_py(*found);
    }

    // Service name for (device, ordinal), or None.
    std::optional<py::str> name(const std::optional<py::str>& device, const std::optional<int64_t>& ordinal) const {
        if (!ordinal) return std::nullopt;
        return cb::compat::to_py(
            inner_.name(device ? cb::compat::from_py(*device) : std::string(), static_cast<std::uint32_t>(*ordinal)));
    }

private:
    cb::source::vxd::ServiceTables inner_;
};

// inc_dir None or "": the folder _find_inc_dir() finds (none: no tables)
inline Services load(const std::optional<py::str>& inc_dir_in = std::nullopt) {
    std::optional<py::str> inc_dir = inc_dir_in;
    if (!inc_dir || inc_dir->empty()) inc_dir = _find_inc_dir();
    if (!inc_dir || inc_dir->empty()) return Services(py::dict<py::str, py::list<py::str>>());
    static std::map<std::string, std::shared_ptr<const cb::source::vxd::ServiceTables>> read;   // (functools.lru_cache)
    const std::string key = inc_dir->utf8();
    auto it = read.find(key);
    if (it == read.end()) {
        it = read.emplace(key, std::make_shared<const cb::source::vxd::ServiceTables>(cb::compat::guard([&] {
                                  return cb::source::vxd::ServiceTables::load(cb::fs::from_text(key));
                              }))).first;
    }
    return Services(cb::compat::to_py(it->second->tables()));
}

}  // namespace codebreaker::source::vxd::services

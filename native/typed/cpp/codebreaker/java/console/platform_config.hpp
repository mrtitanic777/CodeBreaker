// codebreaker/java/console/platform_config.hpp -- SHIM over cb/java/console/platform_config.hpp (docs/CPP_DESIGN.md
// 6.3); deleted with its last py:: consumer (run_tools).
//
// The old record of a config keeps its string-valued fields; it is converted from and to the typed config at the call.
// The decision tables and the Python-shaped helpers (_get_path, config_value, dump_json ...) are not carried over:
// nothing outside the module used them. The `platform` arguments are optional strings as before; none means the
// platform needs no SDK gate. The settings key xbox360_sdk is read here (through the sdk shim) for the SDK gate.
#pragma once

#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/console/platform_config.hpp"
#include "codebreaker/java/console/_shim.hpp"
#include "codebreaker/java/console/sdk.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::platform_config {

using Number = std::variant<int64_t, double>;

struct ControllerSection {
    py::str layout;
    bool invert_y = false;
    Number look_sensitivity = 1.0;
    Number stick_deadzone = 0.24;
    Number trigger_threshold = 0.30;
    py::dict<py::str, py::str> binds;
};

struct Config {
    ControllerSection controller;
    py::str keyboard;
    py::str storage;
    py::str title_id;
    py::dict<py::str, py::str> subsystems;
};

using Update = std::variant<py::str, bool, int64_t, double>;

namespace shim_ {
namespace cbc = cb::java::console::platform_config;

inline Config to_old(const cbc::Config& c) {
    Config o;
    o.controller.layout = cb::compat::to_py(cbc::layout_name(c.controller.layout));
    o.controller.invert_y = c.controller.invert_y;
    o.controller.look_sensitivity = c.controller.look_sensitivity;
    o.controller.stick_deadzone = c.controller.stick_deadzone;
    o.controller.trigger_threshold = c.controller.trigger_threshold;
    o.controller.binds = cb::compat::to_py(c.controller.binds);
    o.keyboard = cb::compat::to_py(cbc::keyboard_name(c.keyboard));
    o.storage = cb::compat::to_py(cbc::storage_name(c.storage));
    o.title_id = cb::compat::to_py(c.title_id);
    o.subsystems = cb::compat::to_py(c.subsystems);
    return o;
}

inline double number(const Number& n) { return std::holds_alternative<double>(n) ? std::get<double>(n) : static_cast<double>(std::get<int64_t>(n)); }

inline std::map<std::string, std::string> string_map(const py::dict<py::str, py::str>& d) {
    std::map<std::string, std::string> out;
    for (const auto& kv : d.items()) out[cb::compat::from_py(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
    return out;
}

inline cbc::Config from_old(const Config& o) {
    cbc::Config c = cbc::default_config();
    c.controller.layout = cbc::parse_layout(cb::compat::from_py(o.controller.layout)).value_or(c.controller.layout);
    c.controller.invert_y = o.controller.invert_y;
    c.controller.look_sensitivity = number(o.controller.look_sensitivity);
    c.controller.stick_deadzone = number(o.controller.stick_deadzone);
    c.controller.trigger_threshold = number(o.controller.trigger_threshold);
    c.controller.binds = string_map(o.controller.binds);
    c.keyboard = cbc::parse_keyboard(cb::compat::from_py(o.keyboard)).value_or(c.keyboard);
    c.storage = cbc::parse_storage(cb::compat::from_py(o.storage)).value_or(c.storage);
    c.title_id = cb::compat::from_py(o.title_id);
    c.subsystems = string_map(o.subsystems);
    return c;
}

inline std::string text_of(const Update& u) { return std::holds_alternative<py::str>(u) ? cb::compat::from_py(std::get<py::str>(u)) : std::string(); }
inline double number_of(const Update& u) {
    if (const auto* d = std::get_if<double>(&u)) return *d;
    if (const auto* i = std::get_if<int64_t>(&u)) return static_cast<double>(*i);
    return 0.0;
}

// The old dict of dotted keys as the typed update.
inline cbc::ConfigUpdate to_update(const std::optional<py::dict<py::str, Update>>& updates) {
    cbc::ConfigUpdate out;
    if (!updates) return out;
    for (const auto& kv : updates->items()) {
        const std::string k = cb::compat::from_py(std::get<0>(kv));
        const Update& value = std::get<1>(kv);
        if (k == "keyboard") {
            const auto v = cbc::parse_keyboard(text_of(value));
            if (!v) throw cb::UsageError("unknown keyboard medium '" + text_of(value) + "'");
            out.keyboard = *v;
        } else if (k == "storage") {
            const auto v = cbc::parse_storage(text_of(value));
            if (!v) throw cb::UsageError("unknown storage medium '" + text_of(value) + "'");
            out.storage = *v;
        } else if (k == "layout") {
            const auto v = cbc::parse_layout(text_of(value));
            if (!v) throw cb::UsageError("unknown controller layout '" + text_of(value) + "'");
            out.layout = *v;
        } else if (k == "invert_y") {
            out.invert_y = std::holds_alternative<bool>(value) && std::get<bool>(value);
        } else if (k == "look_sensitivity") {
            out.look_sensitivity = number_of(value);
        } else if (k == "stick_deadzone") {
            out.stick_deadzone = number_of(value);
        } else if (k == "trigger_threshold") {
            out.trigger_threshold = number_of(value);
        } else if (k.starts_with("bind.")) {
            out.binds[k.substr(5)] = text_of(value);
        } else if (k.starts_with("subsystem.")) {
            out.subsystems[k.substr(10)] = text_of(value);
        }
    }
    return out;
}

inline std::string platform_text(const std::optional<py::str>& platform) { return platform ? cb::compat::from_py(*platform) : std::string(); }
}  // namespace shim_

inline Config default_config() { return shim_::to_old(shim_::cbc::default_config()); }

inline Config load_config(const py::str& path) {
    return cb::compat::guard([&] { return shim_::to_old(shim_::cbc::load_config(console::shim::path_of(path))); });
}

inline void save_config(const py::str& path, const Config& cfg) {
    cb::compat::guard([&] { shim_::cbc::save_config(console::shim::path_of(path), shim_::from_old(cfg)); });
}

inline py::str _choices_md(const Config& cfg, const std::optional<sdk::SdkInfo>& sdk_info = std::nullopt,
                           const py::list<py::str>& unverified = py::list<py::str>()) {
    shim_::cbc::SdkNote note;
    if (sdk_info) note = {cb::compat::from_py(sdk_info->name), cb::compat::from_py(sdk_info->root), cb::compat::from_py(sdk_info->source)};
    return cb::compat::to_py(shim_::cbc::choices_md(shim_::from_old(cfg), sdk_info ? &note : nullptr, cb::compat::from_py(unverified)));
}

inline std::tuple<Config, py::list<py::str>, std::optional<sdk::SdkInfo>, py::list<py::str>> run(
    const py::str& out_dir, const std::optional<py::str>& config_path = std::nullopt,
    const std::optional<py::dict<py::str, Update>>& updates = std::nullopt, bool list_only = false,
    const std::optional<py::str>& platform = py::str("xbox360"), const std::optional<py::str>& sdk_override = std::nullopt) {
    shim_::cbc::RunOptions options;
    options.config_path = console::shim::given(config_path);
    options.list_only = list_only;
    options.platform = shim_::platform_text(platform);
    options.sdk_hints = sdk::detail_::hints_for(platform, sdk_override);
    shim_::cbc::RunResult r;
    try {
        options.updates = shim_::to_update(updates);
        r = shim_::cbc::run(console::shim::path_of(out_dir), options);
    } catch (const cb::java::console::sdk::SdkNotFoundError& e) {
        throw sdk::SdkNotFound(std::string(e.what()));
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
    py::list<py::str> written;
    for (const auto& p : r.written) written.append(console::shim::native(p));
    std::optional<sdk::SdkInfo> info;
    if (r.sdk) info = sdk::SdkInfo(std::move(*r.sdk));
    return {shim_::to_old(r.config), written, info, cb::compat::to_py(r.unverified)};
}

}  // namespace codebreaker::java::console::platform_config

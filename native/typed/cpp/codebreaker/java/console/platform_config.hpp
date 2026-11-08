// codebreaker/java/console/platform_config.hpp -- C++17 port of codebreaker.java.console.platform_config.
//
// platform_config.py -- the user-DECISION layer of the converter.
//
// Some platform questions have no single right answer -- they are the user's call. This module turns
// those into an explicit, prefilled MENU the user picks from, then EMITS the chosen implementation into
// the port instead of a blind stub:
//
//   1. Controller mapping   -- which 360 buttons/sticks drive the mouse-expected PC controls, plus a
//                              stick-layout preset (Default / Southpaw / Legacy / Legacy-Southpaw),
//                              Y-invert, sensitivity, deadzones. -> ControllerConfig.h (the Input layer
//                              reads it).
//   2. Keyboard / text input -- HOW the game gets typed text (world names, chat, server address): the
//                              system virtual keyboard `XShowKeyboardUI`, an in-game on-screen keyboard,
//                              or a fixed-string stub. -> Kernel_ShowKeyboard() in Kernel.cpp.
//   3. Save / load storage   -- WHERE worlds persist + device selection: STFS content container
//                              (`XShowDeviceSelectorUI` + `XContentCreate`), a raw dev HDD path, or
//                              memory-only. -> Kernel_SelectDevice()/Kernel_OpenSave() in Kernel.cpp.
//
// The choices live in `platform.config.json` (defaults written on first run); re-emit after editing. The
// emitted code carries `// J2X-STUB[...]` markers wherever a choice degrades to a no-op, so the master
// audit (`grep -rn J2X-STUB`) still finds it. A `PLATFORM_CHOICES.md` shows the full menu with the
// current pick marked, so every option is visible and switchable in one place.
//
// Typing notes: the Python config is a nested dict with a fixed set of keys, so it is the Config record
// (with ControllerSection inside); `platform.config.json` is read into it by load_config. Keys outside
// the fixed ones are kept in each record's `extra` (as JSON values) and written back by dump_config in
// json.dump(sort_keys=True) order; `binds` and `subsystems` are open dicts of str to str. The three
// controller thresholds are Numbers (int or float as the file had them). A value of the wrong JSON type
// raises TypeError where the Python would fail later on its use. The `updates` values of run() are
// str / bool / number, so Update is a std::variant of them; each key takes the kind its config field has.
#pragma once

#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "py_os.hpp"
#include "value.hpp"
#include "codebreaker/java/console/sdk.hpp"

namespace codebreaker::java::console::platform_config {

// Which chosen medium needs which 360 kernel symbols -- so we can (a) gate on the SDK being present
// and (b) verify each symbol actually exists in the user's installed SDK before emitting a call to it.
extern const py::dict<py::str, py::list<py::str>> _MEDIUM_SYMBOLS;

// 1. CONTROLLER MAPPING
// The mouse/keyboard controls the PC game expects, each needing a 360 binding: (key, label, default control).
extern const py::list<std::tuple<py::str, py::str, py::str>> CONTROLLER_ACTIONS;

// The 360 controls a binding may target.
extern const py::list<py::str> CONTROLLER_BUTTONS;

// Stick-layout presets: which physical stick does move vs look, and whether attack/place sit on the
// triggers (default) or the bumpers (legacy). Each preset overrides the relevant action binds.
extern const py::dict<py::str, py::dict<py::str, py::str>> CONTROLLER_LAYOUTS;

// One menu option: its label and description.
struct OptionSpec {
    py::str label;
    py::str desc;
};

// 2. KEYBOARD / TEXT-INPUT medium
extern const py::dict<py::str, OptionSpec> KEYBOARD_OPTIONS;

// 3. SAVE / LOAD storage medium
extern const py::dict<py::str, OptionSpec> STORAGE_OPTIONS;

// 4. OTHER SUBSYSTEM decisions (audio / video / networking / memory / user / debug)
// Each is an either/or the user picks. Mirrors the option menus in platform_coverage.SUBSYSTEMS,
// surfaced as pick-one choices the GUI renders as button groups and the emitter records.
struct SubsystemSpec {
    py::str default_;   // the Python's 'default' key
    py::dict<py::str, OptionSpec> options;
};
extern const py::dict<py::str, SubsystemSpec> SUBSYSTEM_OPTIONS;

// A JSON number as the file held it: an int stays an int (2), a float stays a float (2.0).
using Number = std::variant<int64_t, double>;

// Keys of a JSON object that this module does not know, in file order, with their parsed JSON values
// (py::V is the runtime's JSON value). They are carried through load -> save unchanged, as the Python's
// dict carries them.
using Extra = std::vector<std::pair<py::str, py::V>>;

// The controller section of the config.
struct ControllerSection {
    py::str layout;
    bool invert_y;
    Number look_sensitivity;
    Number stick_deadzone;
    Number trigger_threshold;
    py::dict<py::str, py::str> binds;
    Extra extra;   // unknown keys of the "controller" object
};

// The whole config (the dict default_config() returns / platform.config.json holds).
struct Config {
    ControllerSection controller;
    py::str keyboard;
    py::str storage;
    // Self-issued 32-bit Title ID (8 hex digits; high 16 = publisher, low 16 = title number). The
    // SPA/XLAST achievement config and the built XEX are BOTH keyed to it and MUST match or unlocks
    // never bind to a profile -- so it lives here, once, and both emitters read it from here.
    // '' means unset: the achievements step then suggests one (0xFFFE publisher prefix) and the XEX
    // is built with 00000000, which is fine for bring-up but cannot deliver achievements.
    py::str title_id;
    py::dict<py::str, py::str> subsystems;
    Extra extra;   // unknown top-level keys
};

Config default_config();

// The QUESTION each decision poses to the user + WHY it must be answered a certain way. Each entry is
// (question, why, (recommended key, one-line reason)).
// NOTE: keep all text UNIVERSAL -- describe the platform's general trade-offs, not any specific game,
// port, or this tool's internals. Other people port other Java programs with this; the questions must
// read the same for all of them.
extern const py::dict<py::str, std::tuple<py::str, py::str, std::tuple<py::str, py::str>>> _QUESTION;

extern const py::dict<py::str, py::str> _SUB_LABELS;

// One decision the UI renders as button groups: id, label, path (where the value lives in cfg),
// question, why (the stakes/guidance), recommend (the key we suggest + a one-line reason), and
// options [(key, label, consequence)].
struct Decision {
    py::str id;
    py::str label;
    py::str path;
    py::str question;
    py::str why;
    std::tuple<py::str, py::str> recommend;
    py::list<std::tuple<py::str, py::str, py::str>> options;
};

py::list<Decision> _decisions();

// The full, ordered list of decisions the UI renders as button groups.
extern const py::list<Decision> DECISIONS;

// The config as the JSON-shaped dict the Python holds (known keys and the extras alike).
py::V config_value(const Config& cfg);

// What a dotted path of the config leads to: a str, bool, int or float, or (for an object or list) the
// JSON value itself.
using PathValue = std::variant<py::str, bool, int64_t, double, py::V>;

// The value at a dotted path of the config ('controller.layout', 'keyboard', 'subsystems.<name>',
// 'controller.binds', an extra key ...): the Python's dict walk, which answers None (here nullopt) for
// a missing key, for a JSON null, and for a path that goes through something that is not an object.
std::optional<PathValue> _get_path(const Config& cfg, const py::str& path);

// json.dumps(v, indent=indent, sort_keys=True): `indent` nullopt is the one-line form (", " and ": ").
py::str dump_json(const py::V& v, std::optional<int64_t> indent = std::nullopt);

// Return a copy of the binds with the chosen layout preset applied over the base binds.
py::dict<py::str, py::str> _apply_layout(const Config& cfg);

// json.load of `path` filled with the defaults for any missing key (forward-compatible); the defaults
// when `path` is not a file. A file that is not JSON raises ValueError (JSONDecodeError's base).
Config load_config(const py::str& path);

// json.dump(cfg, indent=2, sort_keys=True) as text (no trailing newline).
py::str dump_config(const Config& cfg);

void save_config(const py::str& path, const Config& cfg);

// ---------------------------------------------------------------------------
// emission
// ---------------------------------------------------------------------------
py::str _controller_header(const Config& cfg);

// (KernelConfig.h text, Kernel.cpp text)
std::tuple<py::str, py::str> _kernel_files(const Config& cfg);

py::str _choices_md(const Config& cfg, const std::optional<sdk::SdkInfo>& sdk_info = std::nullopt,
                    const py::list<py::str>& unverified = py::list<py::str>());

// Write ControllerConfig.h, KernelConfig.h, Kernel.cpp, and PLATFORM_CHOICES.md into out_dir.
// Returns the list of paths written.
py::list<py::str> emit(const py::str& out_dir, const Config& cfg,
                       const std::optional<sdk::SdkInfo>& sdk_info = std::nullopt,
                       const py::list<py::str>& unverified = py::list<py::str>());

// The 360 kernel symbols the chosen mediums will call.
py::list<py::str> required_symbols(const Config& cfg);

// Enforce the SDK gate for the chosen mediums. If a chosen medium needs kernel symbols and the
// platform is SDK-gated, require the SDK (raises sdk::SdkNotFound if absent) and verify each symbol
// exists in the located SDK. Returns (sdk_info_or_None, unverified_symbols).
std::tuple<std::optional<sdk::SdkInfo>, py::list<py::str>> gate_sdk(
    const Config& cfg, const std::optional<py::str>& platform = py::str("xbox360"),
    const std::optional<py::str>& sdk_override = std::nullopt);

// A value in run()'s `updates` dict.
using Update = std::variant<py::str, bool, int64_t, double>;

// Load (or default) the config, apply `updates` (dict of dotted keys), save it, and emit -- unless
// list_only. The SDK gate runs before any 360 kernel code is written: a medium needing XDK symbols
// is refused unless the SDK is located (env/override/standard path). Returns
// (cfg, written_paths, sdk_info, unverified_symbols). `updates` keys: layout, invert_y,
// look_sensitivity, keyboard, storage, bind.<action>.
std::tuple<Config, py::list<py::str>, std::optional<sdk::SdkInfo>, py::list<py::str>> run(
    const py::str& out_dir, const std::optional<py::str>& config_path = std::nullopt,
    const std::optional<py::dict<py::str, Update>>& updates = std::nullopt, bool list_only = false,
    const std::optional<py::str>& platform = py::str("xbox360"),
    const std::optional<py::str>& sdk_override = std::nullopt);

}  // namespace codebreaker::java::console::platform_config

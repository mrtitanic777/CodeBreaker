// cb/java/console/platform_config.hpp -- the user-DECISION layer of the converter.
//
// Some platform questions have no single right answer -- they are the user's call. This module turns those into an
// explicit, prefilled MENU the user picks from, then EMITS the chosen implementation into the port instead of a blind
// stub:
//
//   1. Controller mapping    -- which 360 buttons/sticks drive the mouse-expected PC controls, plus a stick-layout
//                               preset (Default / Southpaw / Legacy / Legacy-Southpaw), Y-invert, sensitivity,
//                               deadzones. -> ControllerConfig.h (the Input layer reads it).
//   2. Keyboard / text input -- HOW the game gets typed text (world names, chat, server address): the system virtual
//                               keyboard `XShowKeyboardUI`, an in-game on-screen keyboard, or a fixed-string stub.
//                               -> Kernel_ShowKeyboard() in Kernel.cpp.
//   3. Save / load storage   -- WHERE worlds persist + device selection: STFS content container
//                               (`XShowDeviceSelectorUI` + `XContentCreate`), a raw dev HDD path, or memory-only.
//                               -> Kernel_SelectDevice()/Kernel_OpenSave() in Kernel.cpp.
//   4. Other subsystems      -- audio, video, networking, memory, user, debug and achievements: each an either/or the
//                               user picks and the emitter records.
//
// The choices live in `platform.config.json` (defaults written on first run); re-emit after editing. The emitted code
// carries `// J2X-STUB[...]` markers wherever a choice degrades to a no-op, so the master audit (`grep -rn J2X-STUB`)
// still finds it. A `PLATFORM_CHOICES.md` shows the full menu with the current pick marked, so every option is visible
// and switchable in one place.
//
// platform.config.json is also read by other tools (the Title ID by the XEX/achievement emitters); keys this module
// does not know are kept as they are when the file is written back.
#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/console/sdk.hpp"

namespace cb::java::console::platform_config {

// ---- the choices ----------------------------------------------------------------------------------------------------

// Which physical stick moves and which looks, and whether attack/place sit on the triggers (default) or the bumpers
// (legacy). Each preset overrides the relevant action binds.
enum class Layout { Default, Southpaw, Legacy, LegacySouthpaw };

// How the game gets typed text.
enum class Keyboard { XShowKeyboard, Osk, Stub };

// Where saves persist.
enum class Storage { Stfs, RawHdd, Memory };

std::string_view layout_name(Layout l);       // "default", "southpaw", "legacy", "legacy_southpaw"
std::string_view keyboard_name(Keyboard k);   // "xshowkeyboard", "osk", "stub"
std::string_view storage_name(Storage s);     // "stfs", "rawhdd", "memory"
std::optional<Layout> parse_layout(std::string_view name);
std::optional<Keyboard> parse_keyboard(std::string_view name);
std::optional<Storage> parse_storage(std::string_view name);

// A mouse/keyboard control the PC game expects, each needing a 360 binding.
struct ControllerAction {
    std::string_view key;               // "look"
    std::string_view label;             // "Mouse look (camera)"
    std::string_view default_binding;   // "RSTICK"
};
std::span<const ControllerAction> controller_actions();

// The 360 controls a binding may target.
std::span<const std::string_view> controller_buttons();

// The action binds a layout preset overrides: (action key, 360 control).
std::span<const std::pair<std::string_view, std::string_view>> layout_binds(Layout l);

// One answer to a decision: its key, the label shown, and what it means.
struct Option {
    std::string_view key;
    std::string_view label;
    std::string_view desc;
};

// A decision the user answers: the question it poses, why it matters, the answer we suggest, and the options. `id`
// is "controller_layout", "keyboard", "storage" or the name of an other subsystem ("audio", "video", "network",
// "memory", "user", "debug", "achievements").
//
// The text is universal on purpose: it describes the platform's general trade-offs, not any specific game, port or
// this tool's internals, so the questions read the same for every Java program ported with this.
struct Decision {
    std::string_view id;
    std::string_view label;
    std::string_view question;
    std::string_view why;
    std::string_view recommend_key;
    std::string_view recommend_reason;
    std::vector<Option> options;
};

// Every decision, in the order the UI renders them: layout, keyboard, storage, then the subsystems.
const std::vector<Decision>& decisions();

// The other-subsystem ids, in order, with the answer a new config starts with.
struct SubsystemDefault {
    std::string_view id;
    std::string_view default_choice;
};
std::span<const SubsystemDefault> subsystem_defaults();

// ---- the config -----------------------------------------------------------------------------------------------------

struct ControllerConfig {
    Layout layout = Layout::Default;
    bool invert_y = false;
    double look_sensitivity = 1.0;
    double stick_deadzone = 0.24;
    double trigger_threshold = 0.30;
    std::map<std::string, std::string> binds;   // action key -> 360 control
    std::map<std::string, std::string> extra;   // keys of the "controller" object this module does not know (JSON text)
};

struct Config {
    ControllerConfig controller;
    Keyboard keyboard = Keyboard::XShowKeyboard;
    Storage storage = Storage::Stfs;
    // Self-issued 32-bit Title ID (8 hex digits; high 16 = publisher, low 16 = title number). The SPA/XLAST
    // achievement config and the built XEX are BOTH keyed to it and MUST match or unlocks never bind to a profile --
    // so it lives here, once, and both emitters read it from here. Empty means unset: the achievements step then
    // suggests one (0xFFFE publisher prefix) and the XEX is built with 00000000, which is fine for bring-up but
    // cannot deliver achievements.
    std::string title_id;
    std::map<std::string, std::string> subsystems;   // subsystem id -> chosen key
    std::map<std::string, std::string> extra;         // top-level keys this module does not know (JSON text)
};

// A config with every default.
Config default_config();

// The key chosen for a decision ("default", "xshowkeyboard", "stfs", "xact3" ...); empty when the config holds no
// answer (a subsystem it does not mention).
std::string choice_of(const Config& cfg, std::string_view decision_id);

// The binds with the chosen layout preset applied over the base binds.
std::map<std::string, std::string> effective_binds(const Config& cfg);

// Read platform.config.json (a missing file is the default config; a key the file lacks takes its default).
// FormatError / ParseError for a file that is not a valid config.
Config load_config(const std::filesystem::path& path);

// Write it, keys sorted, as the file other tools read.
void save_config(const std::filesystem::path& path, const Config& cfg);

// ---- emission -------------------------------------------------------------------------------------------------------

// ControllerConfig.h: the layout, tuning and every action's 360 binding as #defines.
std::string controller_header(const Config& cfg);

// KernelConfig.h and Kernel.cpp: the 360 kernel wrappers for the chosen text-input and storage mediums.
struct KernelFiles {
    std::string header;
    std::string source;
};
KernelFiles kernel_files(const Config& cfg);

// The SDK the kernel code was emitted against, as PLATFORM_CHOICES.md states it (the converter ships none).
struct SdkNote {
    std::string name;     // "Xbox 360 SDK (XDK)"
    std::string root;     // where it was found
    std::string source;   // how: "env:XEDK", "override", "path" ...
};
SdkNote note_of(const console::sdk::SdkInfo& info);

// PLATFORM_CHOICES.md: the full menu with the current pick marked, the SDK the kernel code was emitted against
// (`sdk`; none: the chosen mediums need no SDK-gated symbol) and any `unverified` symbols the SDK lacks.
std::string choices_md(const Config& cfg, const SdkNote* sdk = nullptr, const std::vector<std::string>& unverified = {});

// Write ControllerConfig.h, KernelConfig.h, Kernel.cpp and PLATFORM_CHOICES.md into `out_dir`; the paths written.
std::vector<std::filesystem::path> emit(const std::filesystem::path& out_dir, const Config& cfg,
                                        const SdkNote* sdk = nullptr, const std::vector<std::string>& unverified = {});

// ---- the SDK gate ---------------------------------------------------------------------------------------------------

// The 360 kernel symbols the chosen mediums will call.
std::vector<std::string_view> required_symbols(const Config& cfg);

struct GateResult {
    std::optional<console::sdk::SdkInfo> sdk;   // the located SDK; none when the choices need no SDK-gated symbol
    std::vector<std::string> unverified;        // required symbols the located SDK does not declare
};

// Enforce the SDK gate for the chosen mediums. If a chosen medium needs kernel symbols and the platform is SDK-gated,
// the SDK is required (SdkNotFoundError if absent) and each symbol is verified against the located SDK's headers.
GateResult gate_sdk(const Config& cfg, std::string_view platform = "xbox360", const console::sdk::SdkHints& hints = {});

// ---- run ------------------------------------------------------------------------------------------------------------

// What to change in the config before it is saved. A bind or subsystem the config did not have is added.
struct ConfigUpdate {
    std::optional<Layout> layout;
    std::optional<bool> invert_y;
    std::optional<double> look_sensitivity;
    std::optional<double> stick_deadzone;
    std::optional<double> trigger_threshold;
    std::optional<Keyboard> keyboard;
    std::optional<Storage> storage;
    std::map<std::string, std::string> binds;        // action key -> 360 control
    std::map<std::string, std::string> subsystems;   // subsystem id -> chosen key
};

struct RunOptions {
    std::optional<std::filesystem::path> config_path;   // default: <out_dir>/platform.config.json
    ConfigUpdate updates;
    bool list_only = false;                              // show the config; write nothing
    std::string platform = "xbox360";
    console::sdk::SdkHints sdk_hints;
};

struct RunResult {
    Config config;
    std::vector<std::filesystem::path> written;
    std::optional<console::sdk::SdkInfo> sdk;
    std::vector<std::string> unverified;
};

// Load (or default) the config, apply the updates, save it and emit -- unless `list_only`. The SDK gate runs before any
// 360 kernel code is written: a medium needing XDK symbols is refused (SdkNotFoundError) unless the SDK is located.
RunResult run(const std::filesystem::path& out_dir, const RunOptions& options = {});

}  // namespace cb::java::console::platform_config

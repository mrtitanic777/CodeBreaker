#include "cb/java/console/platform_config.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::console::platform_config {

namespace stdfs = std::filesystem;

// ---- names ----------------------------------------------------------------------------------------------------------

std::string_view layout_name(Layout l) {
    switch (l) {
        case Layout::Default: return "default";
        case Layout::Southpaw: return "southpaw";
        case Layout::Legacy: return "legacy";
        case Layout::LegacySouthpaw: return "legacy_southpaw";
    }
    CB_ASSERT(false, "unknown Layout");
    return {};
}

std::string_view keyboard_name(Keyboard k) {
    switch (k) {
        case Keyboard::XShowKeyboard: return "xshowkeyboard";
        case Keyboard::Osk: return "osk";
        case Keyboard::Stub: return "stub";
    }
    CB_ASSERT(false, "unknown Keyboard");
    return {};
}

std::string_view storage_name(Storage s) {
    switch (s) {
        case Storage::Stfs: return "stfs";
        case Storage::RawHdd: return "rawhdd";
        case Storage::Memory: return "memory";
    }
    CB_ASSERT(false, "unknown Storage");
    return {};
}

std::optional<Layout> parse_layout(std::string_view name) {
    for (Layout l : {Layout::Default, Layout::Southpaw, Layout::Legacy, Layout::LegacySouthpaw}) {
        if (layout_name(l) == name) return l;
    }
    return std::nullopt;
}

std::optional<Keyboard> parse_keyboard(std::string_view name) {
    for (Keyboard k : {Keyboard::XShowKeyboard, Keyboard::Osk, Keyboard::Stub}) {
        if (keyboard_name(k) == name) return k;
    }
    return std::nullopt;
}

std::optional<Storage> parse_storage(std::string_view name) {
    for (Storage s : {Storage::Stfs, Storage::RawHdd, Storage::Memory}) {
        if (storage_name(s) == name) return s;
    }
    return std::nullopt;
}

// ---- the tables -----------------------------------------------------------------------------------------------------

std::span<const ControllerAction> controller_actions() {
    static const ControllerAction actions[] = {
        {"look", "Mouse look (camera)", "RSTICK"},
        {"move", "Move (WASD)", "LSTICK"},
        {"attack", "Attack / break (left mouse)", "RTRIGGER"},
        {"use", "Use / place (right mouse)", "LTRIGGER"},
        {"jump", "Jump (space)", "A"},
        {"sneak", "Sneak (shift)", "LSTICK_CLICK"},
        {"inventory", "Inventory (E)", "Y"},
        {"drop", "Drop item (Q)", "B"},
        {"hotbar_prev", "Hotbar left (wheel up)", "LBUMPER"},
        {"hotbar_next", "Hotbar right (wheel down)", "RBUMPER"},
        {"pause", "Pause / menu (esc)", "START"},
        {"chat", "Chat / command (T)", "DPAD_UP"},
        {"perspective", "Toggle view (F5)", "DPAD_DOWN"},
        {"sprint", "Sprint", "RSTICK_CLICK"},
        {"gui_cursor", "Virtual cursor in menus", "LSTICK"},
    };
    return actions;
}

std::span<const std::string_view> controller_buttons() {
    static const std::string_view buttons[] = {"A", "B", "X", "Y", "LBUMPER", "RBUMPER", "LTRIGGER", "RTRIGGER",
                                               "LSTICK", "RSTICK", "LSTICK_CLICK", "RSTICK_CLICK", "DPAD_UP",
                                               "DPAD_DOWN", "DPAD_LEFT", "DPAD_RIGHT", "START", "BACK"};
    return buttons;
}

std::span<const std::pair<std::string_view, std::string_view>> layout_binds(Layout l) {
    using Binds = std::pair<std::string_view, std::string_view>;
    static const Binds kDefault[] = {{"move", "LSTICK"}, {"look", "RSTICK"}, {"attack", "RTRIGGER"}, {"use", "LTRIGGER"}};
    static const Binds kSouthpaw[] = {{"move", "RSTICK"}, {"look", "LSTICK"}, {"attack", "RTRIGGER"}, {"use", "LTRIGGER"}};
    static const Binds kLegacy[] = {{"move", "LSTICK"}, {"look", "RSTICK"}, {"attack", "RBUMPER"}, {"use", "LBUMPER"}};
    static const Binds kLegacySouthpaw[] = {{"move", "RSTICK"}, {"look", "LSTICK"}, {"attack", "RBUMPER"}, {"use", "LBUMPER"}};
    switch (l) {
        case Layout::Default: return kDefault;
        case Layout::Southpaw: return kSouthpaw;
        case Layout::Legacy: return kLegacy;
        case Layout::LegacySouthpaw: return kLegacySouthpaw;
    }
    return {};
}

std::span<const SubsystemDefault> subsystem_defaults() {
    static const SubsystemDefault defaults[] = {
        {"audio", "xact3"},   {"video", "gl_emulator"}, {"network", "sockets"},  {"memory", "real_budget"},
        {"user", "profile"},  {"debug", "on"},          {"achievements", "online"},
    };
    return defaults;
}

namespace {

// The options of the keyboard and storage mediums and of every subsystem, in menu order.
std::vector<Option> conf_keyboard_options() {
    return {
        {"xshowkeyboard", "System virtual keyboard (XShowKeyboardUI) -- RECOMMENDED",
         "The platform's built-in on-screen keyboard. Blocks while open; returns the typed string. The standard way "
         "any console app takes text input."},
        {"osk", "In-game on-screen keyboard",
         "A custom controller-driven keyboard you render yourself. More work, fully styled, no system overlay."},
        {"stub", "Stub (fixed strings, no text entry)",
         "Text fields return a fixed placeholder. Boots, but the user cannot type. Bring-up only."},
    };
}

std::vector<Option> conf_storage_options() {
    return {
        {"stfs", "STFS content container (XShowDeviceSelectorUI + XContentCreate) -- RECOMMENDED",
         "Persistent per-title saves. The device selector picks HDD/USB once; XContentCreate mounts a writable "
         "\"Save:\" root the save files live under. Survives reboots, passes cert."},
        {"rawhdd", "Raw HDD dev path (game:\\Save\\)",
         "Writes straight to a dev-kit path. Simple, persists on devkit, but NOT a content container (no device "
         "select, will not pass retail cert)."},
        {"memory", "Memory-only (no persistence)",
         "Save data lives in RAM and is lost on exit. Boots and plays, but nothing persists."},
    };
}

std::vector<Option> conf_layout_options() {
    return {
        {"default", "Default",
         "Move on the left stick, camera on the right; primary/secondary actions on the triggers."},
        {"southpaw", "Southpaw", "Swaps the sticks: move on the right, camera on the left (left-handed aim)."},
        {"legacy", "Legacy", "Primary/secondary actions move from the triggers to the bumpers (older layout)."},
        {"legacy_southpaw", "Legacy Southpaw", "Southpaw sticks AND primary/secondary actions on the bumpers."},
    };
}

std::vector<Option> conf_subsystem_options(std::string_view id) {
    if (id == "audio") {
        return {{"xact3", "XACT3 (wave/sound banks)",
                 "Authored wave/sound banks, cue-driven. A bank-authoring pipeline instead of hand-written mixing."},
                {"xaudio2", "XAudio2 (source voices)",
                 "Low-level source voices; you own mixing, streaming, and format decode. More code, no authoring "
                 "pipeline."},
                {"silent", "Silent (no audio)", "No sound output. Boots quietly — bring-up only."}};
    }
    if (id == "video") {
        return {{"gl_emulator", "GL→D3D9 emulation layer",
                 "Map each OpenGL call onto Direct3D 9 through a translation layer — keeps the existing render code."},
                {"d3d9_fresh", "Native Direct3D 9 backend",
                 "Rewrite the renderer directly against D3D9. Most control, most work."},
                {"stub", "Stub (renders nothing)", "No-op graphics — the game runs but draws nothing. Bring-up only."}};
    }
    if (id == "network") {
        return {{"sockets", "Sockets (networked multiplayer)",
                 "BSD-style sockets over the console network stack. Off-subnet play may need a relay."},
                {"offline", "Offline (single-player only)", "No networking — single-player only."}};
    }
    if (id == "memory") {
        return {{"real_budget", "Real console budget",
                 "Report the platform's true free/total memory (via the console memory API)."},
                {"fixed", "Fixed constants",
                 "Free/total memory return a nominal number — fine unless game logic branches on it."}};
    }
    if (id == "user") {
        return {{"profile", "Signed-in profile name", "Read the console user/profile name for the on-screen identity."},
                {"stub", "Fixed name", "Hardcode a placeholder identity."}};
    }
    if (id == "debug") {
        return {{"on", "On (emit debug file)",
                 "Emit the single toggleable debug file (master switch, greppable markers, rate-limited output)."},
                {"off", "Off (no debug support)", "Do not emit any debug facility."}};
    }
    return {{"online", "Online profile achievements",
             "Real achievements written to the signed-in profile over the online service. Needs a Title ID + network "
             "(queues offline)."},
            {"local", "Local-only (no online write)",
             "Track unlocks locally; nothing is written to the online profile. Bring-up / no-LIVE builds."},
            {"off", "No achievements", "Skip achievement handling entirely."}};
}

std::string_view conf_subsystem_label(std::string_view id) {
    if (id == "audio") return "Audio";
    if (id == "video") return "Video";
    if (id == "network") return "Networking";
    if (id == "memory") return "Memory";
    if (id == "user") return "User / profile";
    if (id == "debug") return "Debug support";
    return "Achievements";
}

struct ConfQuestion {
    std::string_view id, question, why, recommend_key, recommend_reason;
};

std::span<const ConfQuestion> conf_questions() {
    static const ConfQuestion q[] = {
        {"controller_layout", "Which stick layout should the gamepad default to?",
         "A console gamepad has no mouse — the sticks are the camera and movement. This sets where each lives; an "
         "unfamiliar default feels backwards. Users can still remap in-game.",
         "default", "most players expect movement on the left stick and camera on the right"},
        {"keyboard", "How does the user enter text (names, chat, addresses)?",
         "A console has no attached keyboard by default. Any text field needs an input method, or the user is stuck "
         "at that prompt — required whenever the program asks for typed input.",
         "xshowkeyboard", "the built-in system keyboard is familiar and needs no custom UI"},
        {"storage", "Where does save data live, and must it survive a reboot?",
         "This decides whether saves persist across sessions and whether the build can pass the platform's "
         "certification. The wrong choice loses data on exit or fails cert.",
         "stfs", "a proper content container persists across reboots and passes certification"},
        {"audio", "Which audio backend plays music and sound effects?",
         "The platform offers a high-level banked audio system and a low-level voice API. The banked system provides "
         "an authoring pipeline; the low-level one makes you handle mixing and decode.",
         "xact3", "the banked system saves you writing mixing, streaming, and format decode by hand"},
        {"video", "How is the program's OpenGL rendering realized on this platform?",
         "The console has no OpenGL — every GL call must reach the native graphics API (Direct3D 9). A translation "
         "layer keeps the existing render code; a native rewrite is far more work.",
         "gl_emulator", "translating GL→D3D9 reuses the existing render code instead of a full rewrite"},
        {"network", "Does the program need networked multiplayer — and how?",
         "This gates whether the networking code is live. Sockets enable networked play; offline builds skip it "
         "entirely. Play across different subnets may need a relay you provide.",
         "sockets", "sockets keep multiplayer working; offline disables it completely"},
        {"memory", "How should the program report how much memory is free?",
         "Some program logic branches on available memory. Reporting the platform's true budget keeps that behavior "
         "honest; a fixed number is only safe if nothing reads it.",
         "real_budget", "report the true budget unless you know no code depends on the value"},
        {"user", "How is the user's identity / profile obtained?",
         "Used for the on-screen name and any per-user data. The real name comes from the console profile API; the "
         "stub hardcodes a placeholder.",
         "profile", "reading the signed-in profile makes the on-screen name correct"},
        {"debug", "Include the debug instrumentation in this build?",
         "Emits one toggleable debug file (a single master switch, greppable markers, rate-limited output). Keep it "
         "on while bringing the port up; turn it off for a clean release build.",
         "on", "keep it on during bring-up so crashes and leaks are traceable"},
        {"achievements", "How are the program's achievements delivered?",
         "A console can't invent achievements at runtime — each must be declared in the title config and unlocked by "
         "a networked write to the signed-in profile (needs a Title ID, a profile, and network; offline it queues). "
         "Local-only skips the online write.",
         "online", "online writes give real profile achievements; needs a Title ID + signed-in user"},
    };
    return q;
}

}  // namespace

const std::vector<Decision>& decisions() {
    static const std::vector<Decision> all = [] {
        std::vector<Decision> d;
        auto question = [](std::string_view id) -> const ConfQuestion& {
            for (const ConfQuestion& q : conf_questions()) {
                if (q.id == id) return q;
            }
            CB_ASSERT(false, "a decision without its question");
            return conf_questions()[0];
        };
        auto add = [&](std::string_view id, std::string_view label, std::vector<Option> options) {
            const ConfQuestion& q = question(id);
            d.push_back({id, label, q.question, q.why, q.recommend_key, q.recommend_reason, std::move(options)});
        };
        add("controller_layout", "Controller layout", conf_layout_options());
        add("keyboard", "Text input", conf_keyboard_options());
        add("storage", "Save / load storage", conf_storage_options());
        for (const SubsystemDefault& s : subsystem_defaults()) {
            add(s.id, conf_subsystem_label(s.id), conf_subsystem_options(s.id));
        }
        return d;
    }();
    return all;
}

// ---- the config -----------------------------------------------------------------------------------------------------

Config default_config() {
    Config cfg;
    for (const ControllerAction& a : controller_actions()) cfg.controller.binds[std::string(a.key)] = std::string(a.default_binding);
    for (const SubsystemDefault& s : subsystem_defaults()) cfg.subsystems[std::string(s.id)] = std::string(s.default_choice);
    return cfg;
}

std::string choice_of(const Config& cfg, std::string_view decision_id) {
    if (decision_id == "controller_layout") return std::string(layout_name(cfg.controller.layout));
    if (decision_id == "keyboard") return std::string(keyboard_name(cfg.keyboard));
    if (decision_id == "storage") return std::string(storage_name(cfg.storage));
    const auto it = cfg.subsystems.find(std::string(decision_id));
    return it == cfg.subsystems.end() ? std::string() : it->second;
}

std::map<std::string, std::string> effective_binds(const Config& cfg) {
    std::map<std::string, std::string> binds = cfg.controller.binds;
    for (const auto& [action, control] : layout_binds(cfg.controller.layout)) binds[std::string(action)] = std::string(control);
    return binds;
}

namespace {

using Json = cb::Json;

[[noreturn]] void conf_bad(const stdfs::path& path, std::string_view what) {
    throw ParseError(fs::to_text(path), 0, 0, std::string(what));
}

// The object member `key` as a string/bool/double, or `fallback` when the member is absent.
std::string conf_string(const Json& obj, const char* key, const std::string& fallback, const stdfs::path& path) {
    const auto it = obj.find(key);
    if (it == obj.end()) return fallback;
    if (!it->is_string()) conf_bad(path, std::format("'{}' must be a string", key));
    return it->get<std::string>();
}

bool conf_bool(const Json& obj, const char* key, bool fallback, const stdfs::path& path) {
    const auto it = obj.find(key);
    if (it == obj.end()) return fallback;
    if (!it->is_boolean()) conf_bad(path, std::format("'{}' must be true or false", key));
    return it->get<bool>();
}

double conf_number(const Json& obj, const char* key, double fallback, const stdfs::path& path) {
    const auto it = obj.find(key);
    if (it == obj.end()) return fallback;
    if (!it->is_number()) conf_bad(path, std::format("'{}' must be a number", key));
    return it->get<double>();
}

std::map<std::string, std::string> conf_string_map(const Json& obj, const char* key,
                                                   const std::map<std::string, std::string>& fallback,
                                                   const stdfs::path& path) {
    const auto it = obj.find(key);
    if (it == obj.end()) return fallback;
    if (!it->is_object()) conf_bad(path, std::format("'{}' must be an object", key));
    std::map<std::string, std::string> out;
    for (const auto& [k, v] : it->items()) {
        if (!v.is_string()) conf_bad(path, std::format("'{}.{}' must be a string", key, k));
        out[k] = v.get<std::string>();
    }
    return out;
}

// Copy the members of `obj` that are not in `known` into `extra` as compact JSON text.
void conf_collect_extra(const Json& obj, std::initializer_list<const char*> known, std::map<std::string, std::string>& extra) {
    for (const auto& [k, v] : obj.items()) {
        bool is_known = false;
        for (const char* name : known) is_known = is_known || k == name;
        if (!is_known) extra[k] = v.dump();
    }
}

// The document with every object's keys sorted, as the file is written.
Json conf_sorted(const Json& j) {
    if (j.is_object()) {
        std::map<std::string, Json> sorted;
        for (const auto& [k, v] : j.items()) sorted[k] = conf_sorted(v);
        Json out = Json::object();
        for (auto& [k, v] : sorted) out[k] = std::move(v);
        return out;
    }
    if (j.is_array()) {
        Json out = Json::array();
        for (const Json& v : j) out.push_back(conf_sorted(v));
        return out;
    }
    return j;
}

}  // namespace

Config load_config(const stdfs::path& path) {
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) return default_config();
    const Json doc = json::load(path);
    if (!doc.is_object()) conf_bad(path, "the config must be a JSON object");
    const Config def = default_config();
    Config cfg;

    // Every key the file lacks takes its default (forward-compatible).
    if (const auto it = doc.find("controller"); it != doc.end()) {
        if (!it->is_object()) conf_bad(path, "'controller' must be an object");
        const Json& c = *it;
        const std::string layout = conf_string(c, "layout", std::string(layout_name(def.controller.layout)), path);
        const auto parsed = parse_layout(layout);
        if (!parsed) conf_bad(path, std::format("unknown controller layout '{}'", layout));
        cfg.controller.layout = *parsed;
        cfg.controller.invert_y = conf_bool(c, "invert_y", def.controller.invert_y, path);
        cfg.controller.look_sensitivity = conf_number(c, "look_sensitivity", def.controller.look_sensitivity, path);
        cfg.controller.stick_deadzone = conf_number(c, "stick_deadzone", def.controller.stick_deadzone, path);
        cfg.controller.trigger_threshold = conf_number(c, "trigger_threshold", def.controller.trigger_threshold, path);
        cfg.controller.binds = conf_string_map(c, "binds", def.controller.binds, path);
        conf_collect_extra(c, {"layout", "invert_y", "look_sensitivity", "stick_deadzone", "trigger_threshold", "binds"},
                           cfg.controller.extra);
    } else {
        cfg.controller = def.controller;
    }
    const std::string keyboard = conf_string(doc, "keyboard", std::string(keyboard_name(def.keyboard)), path);
    const auto kb = parse_keyboard(keyboard);
    if (!kb) conf_bad(path, std::format("unknown keyboard medium '{}'", keyboard));
    cfg.keyboard = *kb;
    const std::string storage = conf_string(doc, "storage", std::string(storage_name(def.storage)), path);
    const auto st = parse_storage(storage);
    if (!st) conf_bad(path, std::format("unknown storage medium '{}'", storage));
    cfg.storage = *st;
    cfg.title_id = conf_string(doc, "title_id", def.title_id, path);
    // A subsystem the file does not name takes its default; one it names is kept (even one this version does not know).
    cfg.subsystems = def.subsystems;
    for (auto& [k, v] : conf_string_map(doc, "subsystems", {}, path)) cfg.subsystems[k] = std::move(v);
    conf_collect_extra(doc, {"controller", "keyboard", "storage", "title_id", "subsystems"}, cfg.extra);
    return cfg;
}

void save_config(const stdfs::path& path, const Config& cfg) {
    auto parsed = [](const std::map<std::string, std::string>& extra, std::map<std::string, Json>& into) {
        for (const auto& [k, text] : extra) into[k] = conf_sorted(json::parse(text));
    };
    std::map<std::string, Json> controller;
    controller["layout"] = std::string(layout_name(cfg.controller.layout));
    controller["invert_y"] = cfg.controller.invert_y;
    controller["look_sensitivity"] = cfg.controller.look_sensitivity;
    controller["stick_deadzone"] = cfg.controller.stick_deadzone;
    controller["trigger_threshold"] = cfg.controller.trigger_threshold;
    {
        Json binds = Json::object();
        for (const auto& [action, control] : cfg.controller.binds) binds[action] = control;   // std::map: sorted
        controller["binds"] = std::move(binds);
    }
    parsed(cfg.controller.extra, controller);
    Json controller_json = Json::object();
    for (auto& [k, v] : controller) controller_json[k] = std::move(v);

    std::map<std::string, Json> top;
    top["controller"] = std::move(controller_json);
    top["keyboard"] = std::string(keyboard_name(cfg.keyboard));
    top["storage"] = std::string(storage_name(cfg.storage));
    top["title_id"] = cfg.title_id;
    {
        Json subs = Json::object();
        for (const auto& [id, choice] : cfg.subsystems) subs[id] = choice;
        top["subsystems"] = std::move(subs);
    }
    parsed(cfg.extra, top);
    Json doc = Json::object();
    for (auto& [k, v] : top) doc[k] = std::move(v);
    json::save(path, doc, json::Style::PythonCompatiblePretty);
}

// ---- emission -------------------------------------------------------------------------------------------------------

std::string controller_header(const Config& cfg) {
    const ControllerConfig& c = cfg.controller;
    const std::map<std::string, std::string> binds = effective_binds(cfg);
    std::vector<std::string> L = {
        "// ControllerConfig.h -- CodeBreaker-emitted controller mapping (edit platform.config.json + re-emit).",
        "// The Input platform layer (Input.reference) reads these to map the 360 pad to the PC controls.",
        "#ifndef J2X_CONTROLLER_CONFIG_H",
        "#define J2X_CONTROLLER_CONFIG_H",
        "",
        std::format("// Stick layout: {}   (Default / Southpaw / Legacy / Legacy-Southpaw)", layout_name(c.layout)),
        std::format("#define CTRL_LAYOUT_{} 1", text::upper(layout_name(c.layout))),
        "",
        std::format("#define CTRL_INVERT_Y {}", c.invert_y ? 1 : 0),
        std::format("#define CTRL_LOOK_SENSITIVITY {:.3f}f", c.look_sensitivity),
        std::format("#define CTRL_STICK_DEADZONE {:.3f}f", c.stick_deadzone),
        std::format("#define CTRL_TRIGGER_THRESHOLD {:.3f}f", c.trigger_threshold),
        "",
        "// action -> 360 control binding:",
    };
    for (const ControllerAction& a : controller_actions()) {
        const auto it = binds.find(std::string(a.key));
        L.push_back(std::format("#define CTRL_BIND_{:<12} {:<14} // {}", text::upper(a.key),
                                it == binds.end() ? std::string("NONE") : it->second, a.label));
    }
    L.push_back("");
    L.push_back("#endif // J2X_CONTROLLER_CONFIG_H");
    L.push_back("");
    return text::join(L, "\n");
}

KernelFiles kernel_files(const Config& cfg) {
    const Keyboard kb = cfg.keyboard;
    const Storage st = cfg.storage;
    std::vector<std::string> h = {
        "// KernelConfig.h -- CodeBreaker-emitted 360 kernel-call selection (edit platform.config.json + re-emit).",
        "#ifndef J2X_KERNEL_CONFIG_H",
        "#define J2X_KERNEL_CONFIG_H",
        "",
        std::format("// text-input medium: {}", keyboard_name(kb)),
        std::format("#define KERNEL_KEYBOARD_{} 1", text::upper(keyboard_name(kb))),
        std::format("// storage medium: {}", storage_name(st)),
        std::format("#define KERNEL_STORAGE_{} 1", text::upper(storage_name(st))),
        "",
        "// Route ALL text entry and save/load through these -- the one seam the medium choice swaps.",
        "bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount);",
        "bool Kernel_SelectDevice();               // pick + remember the storage device (once)",
        "bool Kernel_OpenSave(const char* rootName); // mount the writable save root",
        "void Kernel_CloseSave();",
        "",
        "#endif // J2X_KERNEL_CONFIG_H",
        "",
    };

    std::vector<std::string> cpp = {
        "// Kernel.cpp -- CodeBreaker-emitted 360 kernel wrappers for the chosen mediums. See KernelConfig.h.",
        "#include \"stdafx.h\"",   // forced PCH: must be the first include
        "#include \"KernelConfig.h\"",
        "",
        "#if defined(_XBOX) || defined(_XENON)",
        "  #include <xtl.h>",
        "#endif",
        "#include <string.h>",
        "#include <wchar.h>",
        "",
    };
    auto add = [&cpp](std::initializer_list<const char*> lines) {
        for (const char* line : lines) cpp.emplace_back(line);
    };

    // --- keyboard ---
    add({"// ---- text input -------------------------------------------------------"});
    if (kb == Keyboard::XShowKeyboard) {
        add({"bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount)",
             "{",
             "#if defined(_XBOX) || defined(_XENON)",
             "    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));",
             "    DWORD r = XShowKeyboardUI(0, VKBD_DEFAULT, deflt ? deflt : L\"\", title ? title : L\"\",",
             "                              L\"\", out, (DWORD)outCount, &ov);",
             "    if (r != ERROR_IO_PENDING) return false;",
             "    while (!XHasOverlappedIoCompleted(&ov)) { /* pump: */ }",
             "    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;",
             "#else",
             "    if (out && outCount > 0) { if (deflt) wcsncpy(out, deflt, outCount); else out[0]=0; }",
             "    return true;",
             "#endif",
             "}",
             ""});
    } else if (kb == Keyboard::Osk) {
        add({"bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount)",
             "{",
             "    // In-game OSK: implemented in OSK.reference (controller-driven keyboard).",
             "    extern bool OSK_Show(const wchar_t*, const wchar_t*, wchar_t*, int);",
             "    return OSK_Show(title, deflt, out, outCount);",
             "}",
             ""});
    } else {
        add({"bool Kernel_ShowKeyboard(const wchar_t*, const wchar_t* deflt, wchar_t* out, int outCount)",
             "{   // J2X-STUB[kernel-keyboard]: no text entry -- returns the default string unchanged.",
             "    if (out && outCount > 0) { if (deflt) wcsncpy(out, deflt, outCount); else out[0]=0; }",
             "    return true;",
             "}",
             ""});
    }

    // --- storage ---
    add({"// ---- save / load storage ---------------------------------------------"});
    if (st == Storage::Stfs) {
        add({"#if defined(_XBOX) || defined(_XENON)",
             "static DWORD g_deviceId = 0;",
             "#endif",
             "bool Kernel_SelectDevice()",
             "{",
             "#if defined(_XBOX) || defined(_XENON)",
             "    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));",
             "    ULARGE_INTEGER needed; needed.QuadPart = 8ULL * 1024 * 1024;   // space to reserve for the save",
             "    DWORD r = XShowDeviceSelectorUI(0, XCONTENTTYPE_SAVEDGAME, XCONTENTFLAG_MANAGESTORAGE,",
             "                                    needed, &g_deviceId, &ov);",
             "    if (r != ERROR_IO_PENDING) return false;",
             "    while (!XHasOverlappedIoCompleted(&ov)) { }",
             "    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;",
             "#else",
             "    return true;",
             "#endif",
             "}",
             "",
             "bool Kernel_OpenSave(const char* rootName)",
             "{",
             "#if defined(_XBOX) || defined(_XENON)",
             "    XCONTENT_DATA data; memset(&data, 0, sizeof(data));",
             "    data.DeviceID = g_deviceId; data.dwContentType = XCONTENTTYPE_SAVEDGAME;",
             "    strncpy(data.szFileName, rootName, XCONTENT_MAX_FILENAME_LENGTH - 1);",
             "    DWORD disp = 0;",
             "    DWORD r = XContentCreate(0, \"Save\", &data, XCONTENTFLAG_OPENALWAYS,",
             "                             &disp, NULL, NULL);",
             "    return r == ERROR_SUCCESS;",
             "#else",
             "    (void)rootName; return true;",
             "#endif",
             "}",
             "",
             "void Kernel_CloseSave()",
             "{",
             "#if defined(_XBOX) || defined(_XENON)",
             "    XContentClose(\"Save\", NULL);",
             "#endif",
             "}",
             ""});
    } else if (st == Storage::RawHdd) {
        add({"bool Kernel_SelectDevice() { return true; }   // raw path: no device selector",
             "bool Kernel_OpenSave(const char* rootName)",
             "{   // dev HDD path (game:\\Save\\); not a content container -- devkit only.",
             "    (void)rootName; return true;",
             "}",
             "void Kernel_CloseSave() {}",
             ""});
    } else {
        add({"bool Kernel_SelectDevice() { return true; }",
             "bool Kernel_OpenSave(const char*)",
             "{   // J2X-STUB[kernel-storage]: memory-only -- worlds do NOT persist across reboots.",
             "    return true; }",
             "void Kernel_CloseSave() {}",
             ""});
    }
    return {text::join(h, "\n"), text::join(cpp, "\n")};
}

SdkNote note_of(const console::sdk::SdkInfo& info) { return {info.name(), fs::to_text(info.root()), info.source_text()}; }

std::string choices_md(const Config& cfg, const SdkNote* sdk, const std::vector<std::string>& unverified) {
    const ControllerConfig& c = cfg.controller;
    const std::map<std::string, std::string> binds = effective_binds(cfg);
    std::vector<std::string> L = {
        "# PLATFORM_CHOICES.md -- user decisions the converter emitted",
        "",
        "These are the questions only you can answer. The current pick is checked; change it in",
        "`platform.config.json` (or via `codebreaker java platform-config`) and re-emit. Everything below is wired",
        "into the port -- ControllerConfig.h, KernelConfig.h, Kernel.cpp.",
        "",
        "## Controller mapping",
        "",
    };
    std::vector<std::string_view> layouts;
    for (Layout l : {Layout::Default, Layout::Southpaw, Layout::Legacy, Layout::LegacySouthpaw}) layouts.push_back(layout_name(l));
    L.push_back(std::format("- Layout: **{}** (options: {})", layout_name(c.layout), text::join(layouts, ", ")));
    L.push_back(std::format("- Invert Y: **{}**   Sensitivity: **{:.2f}**   Stick deadzone: **{:.2f}**   Trigger: **{:.2f}**",
                            c.invert_y ? "yes" : "no", c.look_sensitivity, c.stick_deadzone, c.trigger_threshold));
    L.push_back("");
    L.push_back("| Action | Bound to |");
    L.push_back("|---|---|");
    for (const ControllerAction& a : controller_actions()) {
        const auto it = binds.find(std::string(a.key));
        L.push_back(std::format("| {} | `{}` |", a.label, it == binds.end() ? std::string("NONE") : it->second));
    }
    L.push_back("");
    L.push_back(std::format("Available controls to rebind to: `{}`.", text::join(controller_buttons(), "`, `")));
    L.push_back("");
    // Every decision, each posed as a QUESTION with its stakes + the recommended answer, then the options as a checked
    // multiple-choice list.
    L.push_back("## Subsystem decisions");
    L.push_back("");
    L.push_back(
        "Each of these is a real question you must answer for the port. The recommended answer and why are given; the "
        "current pick is checked.");
    L.push_back("");
    for (const Decision& d : decisions()) {
        if (d.id == "controller_layout") continue;   // already shown above under Controller mapping
        const std::string chosen = choice_of(cfg, d.id);
        L.push_back(std::format("### {} — {}", d.label, d.question));
        L.push_back("");
        L.push_back(std::format("*Why it matters:* {}", d.why));
        L.push_back("");
        L.push_back(std::format("*Recommended:* `{}` — {}", d.recommend_key, d.recommend_reason));
        L.push_back("");
        for (const Option& o : d.options) {
            L.push_back(std::format("- [{}] **{}** (`{}`){} — {}", o.key == chosen ? "x" : " ", o.label, o.key,
                                    o.key == d.recommend_key ? "  ⭐" : "", o.desc));
        }
        L.push_back("");
    }
    // SDK provenance: the kernel code was emitted against the user's own installed SDK, none shipped.
    L.push_back("## SDK");
    L.push_back("");
    if (sdk != nullptr && !sdk->root.empty()) {
        L.push_back(std::format(
            "Kernel code emitted against **{}** located at `{}` (via {}). The converter ships no SDK headers or "
            "signatures -- the real declarations come from your `<xtl.h>` at build time.",
            sdk->name, sdk->root, sdk->source));
        if (!unverified.empty()) {
            L.push_back("");
            L.push_back(std::format(
                "> WARNING: these symbols were NOT found in the located SDK headers -- verify the medium/SDK "
                "version: `{}`.",
                text::join(unverified, "`, `")));
        }
    } else {
        L.push_back("Chosen mediums need no SDK-gated kernel symbols (or a portable target).");
    }
    L.push_back("");
    return text::join(L, "\n") + "\n";
}

std::vector<stdfs::path> emit(const stdfs::path& out_dir, const Config& cfg, const SdkNote* sdk,
                              const std::vector<std::string>& unverified) {
    const KernelFiles kernel = kernel_files(cfg);
    const std::pair<const char*, std::string> files[] = {
        {"ControllerConfig.h", controller_header(cfg)},
        {"KernelConfig.h", kernel.header},
        {"Kernel.cpp", kernel.source},
        {"PLATFORM_CHOICES.md", choices_md(cfg, sdk, unverified)},
    };
    std::vector<stdfs::path> written;
    for (const auto& [name, content] : files) {
        const stdfs::path p = out_dir / name;
        textutil::write_text(p, content.ends_with('\n') ? content : content + "\n");
        written.push_back(p);
    }
    return written;
}

// ---- the SDK gate ---------------------------------------------------------------------------------------------------

std::vector<std::string_view> required_symbols(const Config& cfg) {
    std::vector<std::string_view> syms;
    if (cfg.keyboard == Keyboard::XShowKeyboard) syms.push_back("XShowKeyboardUI");
    if (cfg.storage == Storage::Stfs) {
        for (std::string_view s : {"XShowDeviceSelectorUI", "XContentCreate", "XContentClose"}) syms.push_back(s);
    }
    return syms;
}

GateResult gate_sdk(const Config& cfg, std::string_view platform, const console::sdk::SdkHints& hints) {
    GateResult out;
    const std::vector<std::string_view> syms = required_symbols(cfg);
    if (syms.empty() || !console::sdk::requires_sdk(platform)) return out;
    out.sdk = console::sdk::require(platform, hints);   // SdkNotFoundError: the caller reports
    for (std::string_view s : syms) {
        if (!out.sdk->has_symbol(s)) out.unverified.emplace_back(s);
    }
    return out;
}

// ---- run ------------------------------------------------------------------------------------------------------------

RunResult run(const stdfs::path& out_dir, const RunOptions& options) {
    const stdfs::path config_path = options.config_path.value_or(out_dir / "platform.config.json");
    RunResult result;
    result.config = load_config(config_path);
    Config& cfg = result.config;
    const ConfigUpdate& u = options.updates;
    if (u.keyboard) cfg.keyboard = *u.keyboard;
    if (u.storage) cfg.storage = *u.storage;
    if (u.layout) cfg.controller.layout = *u.layout;
    if (u.invert_y) cfg.controller.invert_y = *u.invert_y;
    if (u.look_sensitivity) cfg.controller.look_sensitivity = *u.look_sensitivity;
    if (u.stick_deadzone) cfg.controller.stick_deadzone = *u.stick_deadzone;
    if (u.trigger_threshold) cfg.controller.trigger_threshold = *u.trigger_threshold;
    for (const auto& [action, control] : u.binds) cfg.controller.binds[action] = control;
    for (const auto& [id, choice] : u.subsystems) cfg.subsystems[id] = choice;
    if (options.list_only) return result;

    GateResult gate = gate_sdk(cfg, options.platform, options.sdk_hints);
    save_config(config_path, cfg);
    const std::optional<SdkNote> note = gate.sdk ? std::optional<SdkNote>(note_of(*gate.sdk)) : std::nullopt;
    result.written = emit(out_dir, cfg, note ? &*note : nullptr, gate.unverified);
    result.sdk = std::move(gate.sdk);
    result.unverified = std::move(gate.unverified);
    return result;
}

}  // namespace cb::java::console::platform_config

// codebreaker/java/console/platform_config.cpp -- C++17 port of codebreaker.java.console.platform_config
// (see platform_config.hpp).

#include "codebreaker/java/console/platform_config.hpp"

#include <algorithm>
#include <cmath>
#include <initializer_list>

#include "json.hpp"
#include "py_json.hpp"

namespace codebreaker::java::console::platform_config {

namespace {

py::str S(const char* s) { return py::str(s); }

py::list<py::str> SL(std::initializer_list<const char*> xs) {
    py::list<py::str> out;
    for (const char* x : xs) out.append(py::str(x));
    return out;
}

// L += [ ... ] for a list of text lines
void add(py::list<py::str>& L, std::initializer_list<const char*> xs) {
    for (const char* x : xs) L.append(py::str(x));
}

// float(n)
double to_double(const Number& n) {
    if (const int64_t* i = std::get_if<int64_t>(&n)) return static_cast<double>(*i);
    return std::get<double>(n);
}

py::V sv(const py::str& s) { return py::V(s.utf8()); }

OptionSpec O(const char* label, const char* desc) { return OptionSpec{py::str(label), py::str(desc)}; }

}  // namespace

// Which chosen medium needs which 360 kernel symbols.
const py::dict<py::str, py::list<py::str>> _MEDIUM_SYMBOLS = {
    {S("xshowkeyboard"), SL({"XShowKeyboardUI"})},
    {S("osk"), SL({})},
    {S("stub"), SL({})},
    {S("stfs"), SL({"XShowDeviceSelectorUI", "XContentCreate", "XContentClose"})},
    {S("rawhdd"), SL({})},
    {S("memory"), SL({})},
};

// ---------------------------------------------------------------------------
// 1. CONTROLLER MAPPING
// ---------------------------------------------------------------------------
// The mouse/keyboard controls the PC game expects, each needing a 360 binding.
const py::list<std::tuple<py::str, py::str, py::str>> CONTROLLER_ACTIONS = {
    {S("look"), S("Mouse look (camera)"), S("RSTICK")},
    {S("move"), S("Move (WASD)"), S("LSTICK")},
    {S("attack"), S("Attack / break (left mouse)"), S("RTRIGGER")},
    {S("use"), S("Use / place (right mouse)"), S("LTRIGGER")},
    {S("jump"), S("Jump (space)"), S("A")},
    {S("sneak"), S("Sneak (shift)"), S("LSTICK_CLICK")},
    {S("inventory"), S("Inventory (E)"), S("Y")},
    {S("drop"), S("Drop item (Q)"), S("B")},
    {S("hotbar_prev"), S("Hotbar left (wheel up)"), S("LBUMPER")},
    {S("hotbar_next"), S("Hotbar right (wheel down)"), S("RBUMPER")},
    {S("pause"), S("Pause / menu (esc)"), S("START")},
    {S("chat"), S("Chat / command (T)"), S("DPAD_UP")},
    {S("perspective"), S("Toggle view (F5)"), S("DPAD_DOWN")},
    {S("sprint"), S("Sprint"), S("RSTICK_CLICK")},
    {S("gui_cursor"), S("Virtual cursor in menus"), S("LSTICK")},
};

// The 360 controls a binding may target.
const py::list<py::str> CONTROLLER_BUTTONS =
    SL({"A", "B", "X", "Y", "LBUMPER", "RBUMPER", "LTRIGGER", "RTRIGGER", "LSTICK", "RSTICK", "LSTICK_CLICK",
        "RSTICK_CLICK", "DPAD_UP", "DPAD_DOWN", "DPAD_LEFT", "DPAD_RIGHT", "START", "BACK"});

// Stick-layout presets: which physical stick does move vs look, and whether attack/place sit on the
// triggers (default) or the bumpers (legacy). Each preset overrides the relevant action binds.
const py::dict<py::str, py::dict<py::str, py::str>> CONTROLLER_LAYOUTS = {
    {S("default"),
     {{S("move"), S("LSTICK")}, {S("look"), S("RSTICK")}, {S("attack"), S("RTRIGGER")}, {S("use"), S("LTRIGGER")}}},
    {S("southpaw"),
     {{S("move"), S("RSTICK")}, {S("look"), S("LSTICK")}, {S("attack"), S("RTRIGGER")}, {S("use"), S("LTRIGGER")}}},
    {S("legacy"),
     {{S("move"), S("LSTICK")}, {S("look"), S("RSTICK")}, {S("attack"), S("RBUMPER")}, {S("use"), S("LBUMPER")}}},
    {S("legacy_southpaw"),
     {{S("move"), S("RSTICK")}, {S("look"), S("LSTICK")}, {S("attack"), S("RBUMPER")}, {S("use"), S("LBUMPER")}}},
};

// ---------------------------------------------------------------------------
// 2. KEYBOARD / TEXT-INPUT medium
// ---------------------------------------------------------------------------
const py::dict<py::str, OptionSpec> KEYBOARD_OPTIONS = {
    {S("xshowkeyboard"),
     O("System virtual keyboard (XShowKeyboardUI) -- RECOMMENDED",
       "The platform's built-in on-screen keyboard. Blocks while open; returns the typed "
       "string. The standard way any console app takes text input.")},
    {S("osk"),
     O("In-game on-screen keyboard",
       "A custom controller-driven keyboard you render yourself. More work, fully styled, "
       "no system overlay.")},
    {S("stub"),
     O("Stub (fixed strings, no text entry)",
       "Text fields return a fixed placeholder. Boots, but the user cannot type. Bring-up "
       "only.")},
};

// ---------------------------------------------------------------------------
// 3. SAVE / LOAD storage medium
// ---------------------------------------------------------------------------
const py::dict<py::str, OptionSpec> STORAGE_OPTIONS = {
    {S("stfs"),
     O("STFS content container (XShowDeviceSelectorUI + XContentCreate) -- RECOMMENDED",
       "Persistent per-title saves. The device selector picks HDD/USB once; XContentCreate "
       "mounts a writable \"Save:\" root the save files live under. Survives reboots, passes "
       "cert.")},
    {S("rawhdd"),
     O("Raw HDD dev path (game:\\Save\\)",
       "Writes straight to a dev-kit path. Simple, persists on devkit, but NOT a content "
       "container (no device select, will not pass retail cert).")},
    {S("memory"),
     O("Memory-only (no persistence)",
       "Save data lives in RAM and is lost on exit. Boots and plays, but nothing persists.")},
};

// ---------------------------------------------------------------------------
// 4. OTHER SUBSYSTEM decisions (audio / video / networking / memory / user / debug)
// ---------------------------------------------------------------------------
// Each is an either/or the user picks. Mirrors the option menus in platform_coverage.SUBSYSTEMS,
// surfaced as pick-one choices the GUI renders as button groups and the emitter records.
const py::dict<py::str, SubsystemSpec> SUBSYSTEM_OPTIONS = {
    {S("audio"),
     SubsystemSpec{S("xact3"),
                   {
                       {S("xact3"), O("XACT3 (wave/sound banks)", "Authored wave/sound banks, cue-driven. A bank-authoring pipeline instead of hand-written mixing.")},
                       {S("xaudio2"), O("XAudio2 (source voices)", "Low-level source voices; you own mixing, streaming, and format decode. More code, no authoring pipeline.")},
                       {S("silent"), O("Silent (no audio)", "No sound output. Boots quietly \xE2\x80\x94 bring-up only.")},
                   }}},
    {S("video"),
     SubsystemSpec{S("gl_emulator"),
                   {
                       {S("gl_emulator"), O("GL\xE2\x86\x92" "D3D9 emulation layer", "Map each OpenGL call onto Direct3D 9 through a translation layer \xE2\x80\x94 keeps the existing render code.")},
                       {S("d3d9_fresh"), O("Native Direct3D 9 backend", "Rewrite the renderer directly against D3D9. Most control, most work.")},
                       {S("stub"), O("Stub (renders nothing)", "No-op graphics \xE2\x80\x94 the game runs but draws nothing. Bring-up only.")},
                   }}},
    {S("network"),
     SubsystemSpec{S("sockets"),
                   {
                       {S("sockets"), O("Sockets (networked multiplayer)", "BSD-style sockets over the console network stack. Off-subnet play may need a relay.")},
                       {S("offline"), O("Offline (single-player only)", "No networking \xE2\x80\x94 single-player only.")},
                   }}},
    {S("memory"),
     SubsystemSpec{S("real_budget"),
                   {
                       {S("real_budget"), O("Real console budget", "Report the platform's true free/total memory (via the console memory API).")},
                       {S("fixed"), O("Fixed constants", "Free/total memory return a nominal number \xE2\x80\x94 fine unless game logic branches on it.")},
                   }}},
    {S("user"),
     SubsystemSpec{S("profile"),
                   {
                       {S("profile"), O("Signed-in profile name", "Read the console user/profile name for the on-screen identity.")},
                       {S("stub"), O("Fixed name", "Hardcode a placeholder identity.")},
                   }}},
    {S("debug"),
     SubsystemSpec{S("on"),
                   {
                       {S("on"), O("On (emit debug file)", "Emit the single toggleable debug file (master switch, greppable markers, rate-limited output).")},
                       {S("off"), O("Off (no debug support)", "Do not emit any debug facility.")},
                   }}},
    {S("achievements"),
     SubsystemSpec{S("online"),
                   {
                       {S("online"), O("Online profile achievements", "Real achievements written to the signed-in profile over the online service. Needs a Title ID + network (queues offline).")},
                       {S("local"), O("Local-only (no online write)", "Track unlocks locally; nothing is written to the online profile. Bring-up / no-LIVE builds.")},
                       {S("off"), O("No achievements", "Skip achievement handling entirely.")},
                   }}},
};

Config default_config() {
    py::dict<py::str, py::str> binds;
    for (const auto& a : CONTROLLER_ACTIONS) binds.set(std::get<0>(a), std::get<2>(a));
    py::dict<py::str, py::str> subsystems;
    for (const auto& kv : SUBSYSTEM_OPTIONS.items()) subsystems.set(std::get<0>(kv), std::get<1>(kv).default_);
    return Config{ControllerSection{S("default"), false, Number(1.0), Number(0.24), Number(0.30), binds, Extra()},
                  S("xshowkeyboard"),
                  S("stfs"),
                  S(""),
                  subsystems,
                  Extra()};
}

// The QUESTION each decision poses to the user + WHY it must be answered a certain way.
// NOTE: keep all text UNIVERSAL -- describe the platform's general trade-offs, not any specific game,
// port, or this tool's internals. Other people port other Java programs with this; the questions must
// read the same for all of them.
const py::dict<py::str, std::tuple<py::str, py::str, std::tuple<py::str, py::str>>> _QUESTION = {
    {S("controller_layout"),
     {S("Which stick layout should the gamepad default to?"),
      S("A console gamepad has no mouse \xE2\x80\x94 the sticks are the camera and movement. This sets where each "
        "lives; an unfamiliar default feels backwards. Users can still remap in-game."),
      {S("default"), S("most players expect movement on the left stick and camera on the right")}}},
    {S("keyboard"),
     {S("How does the user enter text (names, chat, addresses)?"),
      S("A console has no attached keyboard by default. Any text field needs an input method, or the "
        "user is stuck at that prompt \xE2\x80\x94 required whenever the program asks for typed input."),
      {S("xshowkeyboard"), S("the built-in system keyboard is familiar and needs no custom UI")}}},
    {S("storage"),
     {S("Where does save data live, and must it survive a reboot?"),
      S("This decides whether saves persist across sessions and whether the build can pass the "
        "platform's certification. The wrong choice loses data on exit or fails cert."),
      {S("stfs"), S("a proper content container persists across reboots and passes certification")}}},
    {S("audio"),
     {S("Which audio backend plays music and sound effects?"),
      S("The platform offers a high-level banked audio system and a low-level voice API. The banked "
        "system provides an authoring pipeline; the low-level one makes you handle mixing and decode."),
      {S("xact3"), S("the banked system saves you writing mixing, streaming, and format decode by hand")}}},
    {S("video"),
     {S("How is the program's OpenGL rendering realized on this platform?"),
      S("The console has no OpenGL \xE2\x80\x94 every GL call must reach the native graphics API (Direct3D 9). "
        "A translation layer keeps the existing render code; a native rewrite is far more work."),
      {S("gl_emulator"), S("translating GL\xE2\x86\x92" "D3D9 reuses the existing render code instead of a full rewrite")}}},
    {S("network"),
     {S("Does the program need networked multiplayer \xE2\x80\x94 and how?"),
      S("This gates whether the networking code is live. Sockets enable networked play; offline builds "
        "skip it entirely. Play across different subnets may need a relay you provide."),
      {S("sockets"), S("sockets keep multiplayer working; offline disables it completely")}}},
    {S("memory"),
     {S("How should the program report how much memory is free?"),
      S("Some program logic branches on available memory. Reporting the platform's true budget keeps "
        "that behavior honest; a fixed number is only safe if nothing reads it."),
      {S("real_budget"), S("report the true budget unless you know no code depends on the value")}}},
    {S("user"),
     {S("How is the user's identity / profile obtained?"),
      S("Used for the on-screen name and any per-user data. The real name comes from the console "
        "profile API; the stub hardcodes a placeholder."),
      {S("profile"), S("reading the signed-in profile makes the on-screen name correct")}}},
    {S("debug"),
     {S("Include the debug instrumentation in this build?"),
      S("Emits one toggleable debug file (a single master switch, greppable markers, rate-limited "
        "output). Keep it on while bringing the port up; turn it off for a clean release build."),
      {S("on"), S("keep it on during bring-up so crashes and leaks are traceable")}}},
    {S("achievements"),
     {S("How are the program's achievements delivered?"),
      S("A console can't invent achievements at runtime \xE2\x80\x94 each must be declared in the title config "
        "and unlocked by a networked write to the signed-in profile (needs a Title ID, a profile, and "
        "network; offline it queues). Local-only skips the online write."),
      {S("online"), S("online writes give real profile achievements; needs a Title ID + signed-in user")}}},
};

const py::dict<py::str, py::str> _SUB_LABELS = {
    {S("audio"), S("Audio")},   {S("video"), S("Video")},
    {S("network"), S("Networking")}, {S("memory"), S("Memory")},
    {S("user"), S("User / profile")}, {S("debug"), S("Debug support")},
    {S("achievements"), S("Achievements")},
};

py::list<Decision> _decisions() {
    auto entry = [](const py::str& did, const py::str& label, const py::str& path,
                    const py::list<std::tuple<py::str, py::str, py::str>>& opts) {
        const auto& q = _QUESTION.at(did);
        return Decision{did, label, path, std::get<0>(q), std::get<1>(q), std::get<2>(q), opts};
    };
    py::list<Decision> D;

    py::dict<py::str, py::str> layout_label = {{S("default"), S("Default")},
                                               {S("southpaw"), S("Southpaw")},
                                               {S("legacy"), S("Legacy")},
                                               {S("legacy_southpaw"), S("Legacy Southpaw")}};
    py::dict<py::str, py::str> layout_desc = {
        {S("default"), S("Move on the left stick, camera on the right; primary/secondary actions on the triggers.")},
        {S("southpaw"), S("Swaps the sticks: move on the right, camera on the left (left-handed aim).")},
        {S("legacy"), S("Primary/secondary actions move from the triggers to the bumpers (older layout).")},
        {S("legacy_southpaw"), S("Southpaw sticks AND primary/secondary actions on the bumpers.")}};
    py::list<std::tuple<py::str, py::str, py::str>> layout_opts;
    for (const py::str& k : CONTROLLER_LAYOUTS) layout_opts.append(std::make_tuple(k, layout_label.at(k), layout_desc.at(k)));
    D.append(entry(S("controller_layout"), S("Controller layout"), S("controller.layout"), layout_opts));

    py::list<std::tuple<py::str, py::str, py::str>> kb_opts;
    for (const auto& kv : KEYBOARD_OPTIONS.items())
        kb_opts.append(std::make_tuple(std::get<0>(kv), std::get<1>(kv).label, std::get<1>(kv).desc));
    D.append(entry(S("keyboard"), S("Text input"), S("keyboard"), kb_opts));

    py::list<std::tuple<py::str, py::str, py::str>> st_opts;
    for (const auto& kv : STORAGE_OPTIONS.items())
        st_opts.append(std::make_tuple(std::get<0>(kv), std::get<1>(kv).label, std::get<1>(kv).desc));
    D.append(entry(S("storage"), S("Save / load storage"), S("storage"), st_opts));

    for (const auto& sk : SUBSYSTEM_OPTIONS.items()) {
        const py::str& sid = std::get<0>(sk);
        py::list<std::tuple<py::str, py::str, py::str>> opts;
        for (const auto& kv : std::get<1>(sk).options.items())
            opts.append(std::make_tuple(std::get<0>(kv), std::get<1>(kv).label, std::get<1>(kv).desc));
        D.append(entry(sid, _SUB_LABELS.at(sid), py::percent(S("subsystems.%s"), sid), opts));
    }
    return D;
}

const py::list<Decision> DECISIONS = _decisions();

py::V config_value(const Config& cfg) {
    const ControllerSection& c = cfg.controller;
    auto number = [](const Number& n) {
        if (const int64_t* i = std::get_if<int64_t>(&n)) return py::V(static_cast<long long>(*i));
        return py::V(std::get<double>(n));
    };
    py::V binds = py::V::dict();
    for (const auto& kv : c.binds.items()) binds.set(sv(std::get<0>(kv)), sv(std::get<1>(kv)));
    py::V ctrl = py::V::dict();
    ctrl.set("layout", sv(c.layout));
    ctrl.set("invert_y", py::V(c.invert_y));
    ctrl.set("look_sensitivity", number(c.look_sensitivity));
    ctrl.set("stick_deadzone", number(c.stick_deadzone));
    ctrl.set("trigger_threshold", number(c.trigger_threshold));
    ctrl.set("binds", binds);
    for (const auto& kv : c.extra) ctrl.set(sv(kv.first), kv.second);
    py::V subs = py::V::dict();
    for (const auto& kv : cfg.subsystems.items()) subs.set(sv(std::get<0>(kv)), sv(std::get<1>(kv)));
    py::V top = py::V::dict();
    top.set("controller", ctrl);
    top.set("keyboard", sv(cfg.keyboard));
    top.set("storage", sv(cfg.storage));
    top.set("title_id", sv(cfg.title_id));
    top.set("subsystems", subs);
    for (const auto& kv : cfg.extra) top.set(sv(kv.first), kv.second);
    return top;
}

std::optional<PathValue> _get_path(const Config& cfg, const py::str& path) {
    py::V node = config_value(cfg);
    for (const py::str& part : py::split(path, S("."))) {
        py::V next = node.is_dict() ? node.get(sv(part)) : py::V();
        node = next;
    }
    if (node.is_none()) return std::nullopt;
    if (node.is_bool()) return PathValue(node.as_int() != 0);
    if (node.is_int() && !node.u64) return PathValue(static_cast<int64_t>(node.as_int()));   // (past int64 it stays the JSON value)
    if (node.is_float()) return PathValue(node.as_float());
    if (node.is_str()) return PathValue(py::str(node.str()));
    return PathValue(node);
}

// Return a copy of the binds with the chosen layout preset applied over the base binds.
py::dict<py::str, py::str> _apply_layout(const Config& cfg) {
    py::dict<py::str, py::str> binds = cfg.controller.binds.copy();
    binds.update(CONTROLLER_LAYOUTS.get(cfg.controller.layout, py::dict<py::str, py::str>()));
    return binds;
}

namespace {

// ---- reading platform.config.json into the Config record ----
[[noreturn]] void wrong_type(const char* what) {
    throw py::TypeError(std::string("platform config: wrong type for ") + what);
}

py::str str_value(const py::V& v, const char* what) {
    if (!v.is_str()) wrong_type(what);
    return py::str(v.str());
}

bool bool_value(const py::V& v, const char* what) {
    if (v.is_intlike()) return v.as_int() != 0;
    wrong_type(what);
}

Number number_value(const py::V& v, const char* what) {
    if (v.is_int()) return Number(static_cast<int64_t>(v.as_int()));
    if (v.is_float()) return Number(v.as_float());
    wrong_type(what);
}

// the keys of the object `v` that are not in `known`, in file order
Extra extras_of(const py::V& v, std::initializer_list<const char*> known) {
    Extra out;
    for (const auto& kv : v.items()) {
        if (!kv.first.is_str()) wrong_type("an object key");
        bool is_known = false;
        for (const char* k : known) is_known = is_known || kv.first.str() == k;
        if (!is_known) out.emplace_back(py::str(kv.first.str()), kv.second);
    }
    return out;
}

py::dict<py::str, py::str> str_dict(const py::V& v, const char* what) {
    if (!v.is_dict()) wrong_type(what);
    py::dict<py::str, py::str> out;
    for (const auto& kv : v.items()) {
        if (!kv.first.is_str()) wrong_type(what);
        out.set(py::str(kv.first.str()), str_value(kv.second, what));
    }
    return out;
}

// the loaded dict, with any missing key filled from the defaults (forward-compatible)
Config config_from_json(const py::V& loaded) {
    if (!loaded.is_dict()) wrong_type("the config document");
    Config d = default_config();
    Config cfg = d;
    if (loaded.has("controller")) {
        const py::V& c = loaded.get("controller");
        if (!c.is_dict()) wrong_type("controller");
        if (c.has("layout")) cfg.controller.layout = str_value(c.get("layout"), "controller.layout");
        if (c.has("invert_y")) cfg.controller.invert_y = bool_value(c.get("invert_y"), "controller.invert_y");
        if (c.has("look_sensitivity"))
            cfg.controller.look_sensitivity = number_value(c.get("look_sensitivity"), "controller.look_sensitivity");
        if (c.has("stick_deadzone"))
            cfg.controller.stick_deadzone = number_value(c.get("stick_deadzone"), "controller.stick_deadzone");
        if (c.has("trigger_threshold"))
            cfg.controller.trigger_threshold = number_value(c.get("trigger_threshold"), "controller.trigger_threshold");
        if (c.has("binds")) cfg.controller.binds = str_dict(c.get("binds"), "controller.binds");
        cfg.controller.extra = extras_of(c, {"layout", "invert_y", "look_sensitivity", "stick_deadzone",
                                             "trigger_threshold", "binds"});
    }
    if (loaded.has("keyboard")) cfg.keyboard = str_value(loaded.get("keyboard"), "keyboard");
    if (loaded.has("storage")) cfg.storage = str_value(loaded.get("storage"), "storage");
    if (loaded.has("title_id")) cfg.title_id = str_value(loaded.get("title_id"), "title_id");
    if (loaded.has("subsystems")) {
        cfg.subsystems = str_dict(loaded.get("subsystems"), "subsystems");
        for (const auto& kv : d.subsystems.items()) cfg.subsystems.setdefault(std::get<0>(kv), std::get<1>(kv));
    }
    cfg.extra = extras_of(loaded, {"controller", "keyboard", "storage", "title_id", "subsystems"});
    return cfg;
}

}  // namespace

Config load_config(const py::str& path) {
    if (py::os::path::isfile(path)) {
        py::V loaded;
        {
            py::file f = py::open(path, S("r"), S("utf-8"));
            py::str text = f.read_text();
            f.close();
            try {
                loaded = py::json_parse(text.utf8());
            } catch (const py::PyError& e) {
                if (e.type == "ValueError") throw py::ValueError(e.what());
                throw;
            }
        }
        return config_from_json(loaded);
    }
    return default_config();
}

namespace {

// the one-line or indented JSON text of v, objects with their keys sorted (json.dumps sort_keys=True)
void dump_v(std::string& o, const py::V& v, std::optional<int64_t> indent, int64_t level) {
    auto pad = [&](int64_t n) { return std::string(static_cast<size_t>(n * *indent), ' '); };
    if (v.is_none()) {
        o += "null";
    } else if (v.is_bool()) {
        o += v.as_int() ? "true" : "false";
    } else if (v.is_int()) {
        o += v.u64 ? std::to_string(static_cast<uint64_t>(v.i)) : std::to_string(v.as_int());
    } else if (v.is_float()) {
        double d = v.as_float();
        if (d != d) o += "NaN";
        else if (d == HUGE_VAL) o += "Infinity";
        else if (d == -HUGE_VAL) o += "-Infinity";
        else py::json::put(o, d);
    } else if (v.is_str()) {
        py::json::put(o, py::str(v.str()));
    } else if (v.is_seq() || v.is_dict()) {
        const bool obj = v.is_dict();
        std::vector<std::pair<std::string, const py::V*>> items;
        if (obj) {
            for (const auto& kv : v.items()) items.emplace_back(kv.first.str(), &kv.second);
            std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        } else {
            for (const py::V& x : v.seq()) items.emplace_back(std::string(), &x);
        }
        o += obj ? '{' : '[';
        if (items.empty()) {
            o += obj ? '}' : ']';
            return;
        }
        bool first = true;
        for (const auto& it : items) {
            if (!first) o += indent ? "," : ", ";
            if (indent) o += "\n" + pad(level + 1);
            if (obj) {
                py::json::put(o, py::str(it.first));
                o += ": ";
            }
            dump_v(o, *it.second, indent, level + 1);
            first = false;
        }
        if (indent) o += "\n" + pad(level);
        o += obj ? '}' : ']';
    } else {
        throw py::TypeError("Object of this type is not JSON serializable");
    }
}

}  // namespace

py::str dump_json(const py::V& v, std::optional<int64_t> indent) {
    std::string o;
    dump_v(o, v, indent, 0);
    return py::str(o);
}

py::str dump_config(const Config& cfg) { return dump_json(config_value(cfg), 2); }

void save_config(const py::str& path, const Config& cfg) {
    py::file f = py::open(path, S("w"), S("utf-8"));
    f.write(dump_config(cfg));
    f.close();
}

// ---------------------------------------------------------------------------
// emission
// ---------------------------------------------------------------------------
py::str _controller_header(const Config& cfg) {
    const ControllerSection& c = cfg.controller;
    py::dict<py::str, py::str> binds = _apply_layout(cfg);
    py::list<py::str> L;
    L.append(S("// ControllerConfig.h -- CodeBreaker-emitted controller mapping (edit platform.config.json + re-emit)."));
    L.append(S("// The Input platform layer (Input.reference) reads these to map the 360 pad to the PC controls."));
    L.append(S("#ifndef J2X_CONTROLLER_CONFIG_H"));
    L.append(S("#define J2X_CONTROLLER_CONFIG_H"));
    L.append(S(""));
    L.append(py::percent(S("// Stick layout: %s   (Default / Southpaw / Legacy / Legacy-Southpaw)"), c.layout));
    L.append(py::percent(S("#define CTRL_LAYOUT_%s 1"), c.layout.upper()));
    L.append(S(""));
    L.append(py::percent(S("#define CTRL_INVERT_Y %d"), static_cast<int64_t>(c.invert_y ? 1 : 0)));
    L.append(py::percent(S("#define CTRL_LOOK_SENSITIVITY %.3ff"), to_double(c.look_sensitivity)));
    L.append(py::percent(S("#define CTRL_STICK_DEADZONE %.3ff"), to_double(c.stick_deadzone)));
    L.append(py::percent(S("#define CTRL_TRIGGER_THRESHOLD %.3ff"), to_double(c.trigger_threshold)));
    L.append(S(""));
    L.append(S("// action -> 360 control binding:"));
    for (const auto& a : CONTROLLER_ACTIONS) {
        const py::str& key = std::get<0>(a);
        const py::str& label = std::get<1>(a);
        L.append(py::percent(S("#define CTRL_BIND_%-12s %-14s // %s"), key.upper(), binds.get(key, S("NONE")), label));
    }
    add(L, {"", "#endif // J2X_CONTROLLER_CONFIG_H", ""});
    return py::join(S("\n"), L);
}

std::tuple<py::str, py::str> _kernel_files(const Config& cfg) {
    const py::str& kb = cfg.keyboard;
    const py::str& st = cfg.storage;
    py::list<py::str> h;
    h.append(S("// KernelConfig.h -- CodeBreaker-emitted 360 kernel-call selection (edit platform.config.json + re-emit)."));
    add(h, {"#ifndef J2X_KERNEL_CONFIG_H", "#define J2X_KERNEL_CONFIG_H", ""});
    h.append(py::percent(S("// text-input medium: %s"), kb));
    h.append(py::percent(S("#define KERNEL_KEYBOARD_%s 1"), kb.upper()));
    h.append(py::percent(S("// storage medium: %s"), st));
    h.append(py::percent(S("#define KERNEL_STORAGE_%s 1"), st.upper()));
    add(h, {"", "// Route ALL text entry and save/load through these -- the one seam the medium choice swaps.",
            "bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount);",
            "bool Kernel_SelectDevice();               // pick + remember the storage device (once)",
            "bool Kernel_OpenSave(const char* rootName); // mount the writable save root",
            "void Kernel_CloseSave();", "", "#endif // J2X_KERNEL_CONFIG_H", ""});

    py::list<py::str> cpp;
    add(cpp, {"// Kernel.cpp -- CodeBreaker-emitted 360 kernel wrappers for the chosen mediums. See KernelConfig.h.",
              "#include \"stdafx.h\"",   // forced PCH: must be the first include
              "#include \"KernelConfig.h\"", "",
              "#if defined(_XBOX) || defined(_XENON)", "  #include <xtl.h>", "#endif",
              "#include <string.h>", "#include <wchar.h>", ""});

    // --- keyboard ---
    cpp.append(S("// ---- text input -------------------------------------------------------"));
    if (kb == S("xshowkeyboard")) {
        add(cpp, {
            "bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount)",
            "{", "#if defined(_XBOX) || defined(_XENON)",
            "    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));",
            R"X(    DWORD r = XShowKeyboardUI(0, VKBD_DEFAULT, deflt ? deflt : L"", title ? title : L"",)X",
            R"X(                              L"", out, (DWORD)outCount, &ov);)X",
            "    if (r != ERROR_IO_PENDING) return false;",
            "    while (!XHasOverlappedIoCompleted(&ov)) { /* pump: */ }",
            "    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;",
            "#else",
            "    if (out && outCount > 0) { if (deflt) wcsncpy(out, deflt, outCount); else out[0]=0; }",
            "    return true;", "#endif", "}", ""});
    } else if (kb == S("osk")) {
        add(cpp, {
            "bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount)",
            "{", "    // In-game OSK: implemented in OSK.reference (controller-driven keyboard).",
            "    extern bool OSK_Show(const wchar_t*, const wchar_t*, wchar_t*, int);",
            "    return OSK_Show(title, deflt, out, outCount);", "}", ""});
    } else {   // stub
        add(cpp, {
            "bool Kernel_ShowKeyboard(const wchar_t*, const wchar_t* deflt, wchar_t* out, int outCount)",
            "{   // J2X-STUB[kernel-keyboard]: no text entry -- returns the default string unchanged.",
            "    if (out && outCount > 0) { if (deflt) wcsncpy(out, deflt, outCount); else out[0]=0; }",
            "    return true;", "}", ""});
    }

    // --- storage ---
    cpp.append(S("// ---- save / load storage ---------------------------------------------"));
    if (st == S("stfs")) {
        add(cpp, {
            "#if defined(_XBOX) || defined(_XENON)",
            "static DWORD g_deviceId = 0;",
            "#endif",
            "bool Kernel_SelectDevice()",
            "{", "#if defined(_XBOX) || defined(_XENON)",
            "    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));",
            "    ULARGE_INTEGER needed; needed.QuadPart = 8ULL * 1024 * 1024;   // space to reserve for the save",
            "    DWORD r = XShowDeviceSelectorUI(0, XCONTENTTYPE_SAVEDGAME, XCONTENTFLAG_MANAGESTORAGE,",
            "                                    needed, &g_deviceId, &ov);",
            "    if (r != ERROR_IO_PENDING) return false;",
            "    while (!XHasOverlappedIoCompleted(&ov)) { }",
            "    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;",
            "#else", "    return true;", "#endif", "}", "",
            "bool Kernel_OpenSave(const char* rootName)",
            "{", "#if defined(_XBOX) || defined(_XENON)",
            "    XCONTENT_DATA data; memset(&data, 0, sizeof(data));",
            "    data.DeviceID = g_deviceId; data.dwContentType = XCONTENTTYPE_SAVEDGAME;",
            "    strncpy(data.szFileName, rootName, XCONTENT_MAX_FILENAME_LENGTH - 1);",
            "    DWORD disp = 0;",
            R"X(    DWORD r = XContentCreate(0, "Save", &data, XCONTENTFLAG_OPENALWAYS,)X",
            "                             &disp, NULL, NULL);",
            "    return r == ERROR_SUCCESS;",
            "#else", "    (void)rootName; return true;", "#endif", "}", "",
            "void Kernel_CloseSave()",
            "{", "#if defined(_XBOX) || defined(_XENON)", R"X(    XContentClose("Save", NULL);)X",
            "#endif", "}", ""});
    } else if (st == S("rawhdd")) {
        add(cpp, {
            "bool Kernel_SelectDevice() { return true; }   // raw path: no device selector",
            "bool Kernel_OpenSave(const char* rootName)",
            R"X({   // dev HDD path (game:\Save\); not a content container -- devkit only.)X",
            "    (void)rootName; return true;", "}",
            "void Kernel_CloseSave() {}", ""});
    } else {   // memory
        add(cpp, {
            "bool Kernel_SelectDevice() { return true; }",
            "bool Kernel_OpenSave(const char*)",
            "{   // J2X-STUB[kernel-storage]: memory-only -- worlds do NOT persist across reboots.",
            "    return true; }",
            "void Kernel_CloseSave() {}", ""});
    }
    return std::make_tuple(py::join(S("\n"), h), py::join(S("\n"), cpp));
}

py::str _choices_md(const Config& cfg, const std::optional<sdk::SdkInfo>& sdk_info, const py::list<py::str>& unverified) {
    // (the Python also defines a local menu() helper here that nothing calls; it is not carried over)
    const ControllerSection& c = cfg.controller;
    py::dict<py::str, py::str> binds = _apply_layout(cfg);
    py::list<py::str> L;
    add(L, {"# PLATFORM_CHOICES.md -- user decisions the converter emitted", "",
            "These are the questions only you can answer. The current pick is checked; change it in",
            "`platform.config.json` (or via `codebreaker java platform-config`) and re-emit. Everything below is wired",
            "into the port -- ControllerConfig.h, KernelConfig.h, Kernel.cpp.", ""});
    add(L, {"## Controller mapping", ""});
    L.append(py::percent(S("- Layout: **%s** (options: %s)"), c.layout, py::join(S(", "), CONTROLLER_LAYOUTS)));
    L.append(py::percent(S("- Invert Y: **%s**   Sensitivity: **%.2f**   Stick deadzone: **%.2f**   Trigger: **%.2f**"),
                         c.invert_y, to_double(c.look_sensitivity), to_double(c.stick_deadzone),
                         to_double(c.trigger_threshold)));
    add(L, {"", "| Action | Bound to |", "|---|---|"});
    for (const auto& a : CONTROLLER_ACTIONS) {
        L.append(py::percent(S("| %s | `%s` |"), std::get<1>(a), binds.get(std::get<0>(a), S("NONE"))));
    }
    add(L, {""});
    L.append(py::percent(S("Available controls to rebind to: `%s`."), py::join(S("`, `"), CONTROLLER_BUTTONS)));
    L.append(S(""));
    // Every decision, each posed as a QUESTION with its stakes + the recommended answer, then the
    // options as a checked multiple-choice list.
    add(L, {"## Subsystem decisions", "",
            "Each of these is a real question you must answer for the port. The recommended answer and "
            "why are given; the current pick is checked.",
            ""});
    for (const Decision& d : DECISIONS) {
        if (d.id == S("controller_layout")) continue;   // already shown above under Controller mapping
        std::optional<PathValue> chosen_value = _get_path(cfg, d.path);
        const py::str* chosen = chosen_value ? std::get_if<py::str>(&*chosen_value) : nullptr;
        L.append(py::percent(S("### %s \xE2\x80\x94 %s"), d.label, d.question));
        L.append(S(""));
        L.append(py::percent(S("*Why it matters:* %s"), d.why));
        L.append(S(""));
        L.append(py::percent(S("*Recommended:* `%s` \xE2\x80\x94 %s"), std::get<0>(d.recommend), std::get<1>(d.recommend)));
        L.append(S(""));
        for (const auto& o : d.options) {
            const py::str& key = std::get<0>(o);
            py::str mark = (chosen != nullptr && key == *chosen) ? S("x") : S(" ");
            py::str rec = key == std::get<0>(d.recommend) ? S("  \xE2\xAD\x90") : S("");
            L.append(py::percent(S("- [%s] **%s** (`%s`)%s \xE2\x80\x94 %s"), mark, std::get<1>(o), key, rec, std::get<2>(o)));
        }
        L.append(S(""));
    }
    // SDK provenance: the kernel code was emitted against the user's own installed SDK, none shipped.
    add(L, {"## SDK", ""});
    if (sdk_info && !sdk_info->root.empty()) {
        L.append(py::percent(S("Kernel code emitted against **%s** located at `%s` (via %s). The converter ships no "
                               "SDK headers or signatures -- the real declarations come from your `<xtl.h>` at build "
                               "time."),
                             sdk_info->name, sdk_info->root, sdk_info->source));
        if (!unverified.empty()) {
            add(L, {""});
            L.append(py::percent(S("> WARNING: these symbols were NOT found in the located SDK headers -- verify the "
                                   "medium/SDK version: `%s`."),
                                 py::join(S("`, `"), unverified)));
        }
    } else {
        L.append(S("Chosen mediums need no SDK-gated kernel symbols (or a portable target)."));
    }
    L.append(S(""));
    return py::join(S("\n"), L) + S("\n");
}

py::list<py::str> emit(const py::str& out_dir, const Config& cfg, const std::optional<sdk::SdkInfo>& sdk_info,
                       const py::list<py::str>& unverified) {
    py::str kh, kc;
    std::tie(kh, kc) = _kernel_files(cfg);
    py::list<std::tuple<py::str, py::str>> files = {
        {S("ControllerConfig.h"), _controller_header(cfg)},
        {S("KernelConfig.h"), kh},
        {S("Kernel.cpp"), kc},
        {S("PLATFORM_CHOICES.md"), _choices_md(cfg, sdk_info, unverified)},
    };
    py::list<py::str> written;
    for (const auto& nc : files) {
        const py::str& name = std::get<0>(nc);
        const py::str& content = std::get<1>(nc);
        py::str p = py::os::path::join(out_dir, name);
        py::file f = py::open(p, S("w"), S("utf-8"));
        f.write(content.endswith(S("\n")) ? content : content + S("\n"));
        f.close();
        written.append(p);
    }
    return written;
}

py::list<py::str> required_symbols(const Config& cfg) {
    py::list<py::str> syms;
    for (const py::str& medium : {cfg.keyboard, cfg.storage}) {
        syms.extend(_MEDIUM_SYMBOLS.get(medium, py::list<py::str>()));
    }
    return syms;
}

std::tuple<std::optional<sdk::SdkInfo>, py::list<py::str>> gate_sdk(const Config& cfg, const std::optional<py::str>& platform,
                                                                    const std::optional<py::str>& sdk_override) {
    py::list<py::str> syms = required_symbols(cfg);
    if (syms.empty() || !sdk::requires_sdk(platform)) return std::make_tuple(std::optional<sdk::SdkInfo>(), py::list<py::str>());
    sdk::SdkInfo info = sdk::require(platform, sdk_override);   // raises SdkNotFound -> caller reports
    py::list<py::str> unverified;
    for (const py::str& s : syms)
        if (!info.has_symbol(s)) unverified.append(s);
    return std::make_tuple(std::optional<sdk::SdkInfo>(info), unverified);
}

namespace {

py::str update_str(const Update& u, const char* what) {
    if (const py::str* s = std::get_if<py::str>(&u)) return *s;
    wrong_type(what);
}

bool update_bool(const Update& u, const char* what) {
    if (const bool* b = std::get_if<bool>(&u)) return *b;
    if (const int64_t* i = std::get_if<int64_t>(&u)) return *i != 0;
    if (const double* d = std::get_if<double>(&u)) return *d != 0.0;
    wrong_type(what);
}

Number update_number(const Update& u, const char* what) {
    if (const double* d = std::get_if<double>(&u)) return Number(*d);
    if (const int64_t* i = std::get_if<int64_t>(&u)) return Number(*i);
    wrong_type(what);
}

}  // namespace

std::tuple<Config, py::list<py::str>, std::optional<sdk::SdkInfo>, py::list<py::str>> run(
    const py::str& out_dir, const std::optional<py::str>& config_path_, const std::optional<py::dict<py::str, Update>>& updates,
    bool list_only, const std::optional<py::str>& platform, const std::optional<py::str>& sdk_override) {
    py::str config_path = (config_path_ && !config_path_->empty()) ? *config_path_
                                                                    : py::os::path::join(out_dir, S("platform.config.json"));
    Config cfg = load_config(config_path);
    if (updates) {
        for (const auto& kv : updates->items()) {
            const py::str& k = std::get<0>(kv);
            const Update& v = std::get<1>(kv);
            if (k == S("keyboard")) {
                cfg.keyboard = update_str(v, "keyboard");
            } else if (k == S("storage")) {
                cfg.storage = update_str(v, "storage");
            } else if (k == S("layout")) {
                cfg.controller.layout = update_str(v, "layout");
            } else if (k == S("invert_y")) {
                cfg.controller.invert_y = update_bool(v, "invert_y");
            } else if (k == S("look_sensitivity")) {
                cfg.controller.look_sensitivity = update_number(v, "look_sensitivity");
            } else if (k == S("stick_deadzone")) {
                cfg.controller.stick_deadzone = update_number(v, "stick_deadzone");
            } else if (k == S("trigger_threshold")) {
                cfg.controller.trigger_threshold = update_number(v, "trigger_threshold");
            } else if (k.startswith(S("bind."))) {
                cfg.controller.binds.set(k.slice_from(5), update_str(v, "bind"));
            } else if (k.startswith(S("subsystem."))) {
                cfg.subsystems.set(k.slice_from(10), update_str(v, "subsystem"));
            }
        }
    }
    if (list_only) return std::make_tuple(cfg, py::list<py::str>(), std::optional<sdk::SdkInfo>(), py::list<py::str>());
    std::optional<sdk::SdkInfo> info;
    py::list<py::str> unverified;
    std::tie(info, unverified) = gate_sdk(cfg, platform, sdk_override);
    save_config(config_path, cfg);
    py::list<py::str> written = emit(out_dir, cfg, info, unverified);
    return std::make_tuple(cfg, written, info, unverified);
}

}  // namespace codebreaker::java::console::platform_config

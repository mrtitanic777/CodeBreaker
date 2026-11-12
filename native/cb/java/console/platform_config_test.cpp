// platform_config: the emitted files are checked against goldens written by the Python module and reviewed by hand
// (testdata/gen_config_golden.py), the JSON file against the one Python writes, plus load/save/run behaviour.
#include <map>
#include <string>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/console/platform_config.hpp"

namespace {

using namespace cb;
namespace pcfg = cb::java::console::platform_config;
namespace stdfs = std::filesystem;

std::string conf_golden(const std::string& case_name, const std::string& file) {
    return fs::read_text(testing::data_path("java/console/testdata/config/" + case_name + "/" + file));
}

bool conf_has(const std::string& text, std::string_view needle) { return text.find(needle) != std::string::npos; }

// The configs the goldens were made from (see gen_config_golden.py).
pcfg::Config conf_case(const std::string& name) {
    pcfg::Config c = pcfg::default_config();
    if (name == "legacy_southpaw_osk_rawhdd") {
        c.controller.layout = pcfg::Layout::LegacySouthpaw;
        c.controller.invert_y = true;
        c.controller.look_sensitivity = 1.25;
        c.controller.stick_deadzone = 0.3;
        c.controller.trigger_threshold = 0.5;
        c.controller.binds["jump"] = "X";
        c.keyboard = pcfg::Keyboard::Osk;
        c.storage = pcfg::Storage::RawHdd;
        c.title_id = "FFFE07D1";
        c.subsystems["audio"] = "silent";
        c.subsystems["achievements"] = "off";
    } else if (name == "stub_memory") {
        c.keyboard = pcfg::Keyboard::Stub;
        c.storage = pcfg::Storage::Memory;
        c.controller.layout = pcfg::Layout::Southpaw;
        c.controller.binds.erase("chat");
    }
    return c;
}

const char* const kConfCases[] = {"default", "legacy_southpaw_osk_rawhdd", "stub_memory", "with_sdk"};

CB_TEST("java.console.platform_config.emitted_files_match_the_python_goldens", [] {
    for (const std::string name : kConfCases) {
        const pcfg::Config cfg = conf_case(name == "with_sdk" ? "default" : name);
        CB_CHECK_EQ(pcfg::controller_header(cfg), conf_golden(name, "ControllerConfig.h"));
        const pcfg::KernelFiles k = pcfg::kernel_files(cfg);
        CB_CHECK_EQ(k.header, conf_golden(name, "KernelConfig.h"));
        CB_CHECK_EQ(k.source, conf_golden(name, "Kernel.cpp"));
        if (name == "with_sdk") {
            const pcfg::SdkNote sdk{"Xbox 360 SDK (XDK)", "C:\\XDK", "env:XEDK"};
            CB_CHECK_EQ(pcfg::choices_md(cfg, &sdk, {"XContentClose", "XShowKeyboardUI"}), conf_golden(name, "PLATFORM_CHOICES.md"));
        } else {
            CB_CHECK_EQ(pcfg::choices_md(cfg), conf_golden(name, "PLATFORM_CHOICES.md"));
        }
    }
});

CB_TEST("java.console.platform_config.emit_writes_the_four_files", [] {
    fs::ScopedTempDir dir("cb-pcfg");
    const std::vector<stdfs::path> written = pcfg::emit(dir.path(), conf_case("stub_memory"));
    CB_CHECK_EQ(written.size(), std::size_t{4});
    CB_CHECK_EQ(written[0].filename().string(), "ControllerConfig.h");
    CB_CHECK_EQ(written[1].filename().string(), "KernelConfig.h");
    CB_CHECK_EQ(written[2].filename().string(), "Kernel.cpp");
    CB_CHECK_EQ(written[3].filename().string(), "PLATFORM_CHOICES.md");
    CB_CHECK_EQ(fs::read_text(written[0]), conf_golden("stub_memory", "ControllerConfig.h"));
    CB_CHECK_EQ(fs::read_text(written[2]), conf_golden("stub_memory", "Kernel.cpp"));
    CB_CHECK_EQ(fs::read_text(written[3]), conf_golden("stub_memory", "PLATFORM_CHOICES.md"));
    // a function that degrades to a no-op says so, so the master audit finds it
    CB_CHECK(conf_has(fs::read_text(written[2]), "J2X-STUB[kernel-keyboard]"));
    CB_CHECK(conf_has(fs::read_text(written[2]), "J2X-STUB[kernel-storage]"));
});

CB_TEST("java.console.platform_config.json_is_the_file_python_writes", [] {
    for (const std::string name : {"default", "legacy_southpaw_osk_rawhdd", "stub_memory"}) {
        fs::ScopedTempDir dir("cb-pcfg");
        pcfg::save_config(dir.path() / "platform.config.json", conf_case(name));
        // DIVERGES: json.dump leaves no newline at the end of the file; cb::json::save adds one (the golden has it too)
        CB_CHECK_EQ(fs::read_text(dir.path() / "platform.config.json"), conf_golden(name, "platform.config.json"));
    }
});

CB_TEST("java.console.platform_config.load_round_trips", [] {
    fs::ScopedTempDir dir("cb-pcfg");
    for (const std::string name : {"default", "legacy_southpaw_osk_rawhdd", "stub_memory"}) {
        const stdfs::path p = dir.path() / (name + ".json");
        pcfg::save_config(p, conf_case(name));
        const pcfg::Config back = pcfg::load_config(p);
        const pcfg::Config want = conf_case(name);
        CB_CHECK(back.controller.layout == want.controller.layout);
        CB_CHECK(back.controller.invert_y == want.controller.invert_y);
        CB_CHECK(back.controller.look_sensitivity == want.controller.look_sensitivity);
        CB_CHECK(back.controller.binds == want.controller.binds);
        CB_CHECK(back.keyboard == want.keyboard && back.storage == want.storage);
        CB_CHECK_EQ(back.title_id, want.title_id);
        CB_CHECK(back.subsystems == want.subsystems);
    }
});

CB_TEST("java.console.platform_config.load_fills_what_the_file_lacks", [] {
    fs::ScopedTempDir dir("cb-pcfg");
    // missing file: the default config
    const pcfg::Config none = pcfg::load_config(dir.path() / "absent.json");
    CB_CHECK(none.keyboard == pcfg::Keyboard::XShowKeyboard && none.storage == pcfg::Storage::Stfs);
    CB_CHECK_EQ(none.controller.binds.size(), std::size_t{15});
    CB_CHECK_EQ(none.subsystems.size(), std::size_t{7});
    // an old file: only a keyboard and one subsystem; everything else is the default, the named subsystem is kept
    fs::write_text(dir.path() / "old.json", R"({"keyboard": "stub", "subsystems": {"audio": "xaudio2", "future": "x"}})");
    const pcfg::Config old = pcfg::load_config(dir.path() / "old.json");
    CB_CHECK(old.keyboard == pcfg::Keyboard::Stub);
    CB_CHECK(old.storage == pcfg::Storage::Stfs);
    CB_CHECK_EQ(old.subsystems.at("audio"), "xaudio2");
    CB_CHECK_EQ(old.subsystems.at("video"), "gl_emulator");
    CB_CHECK_EQ(old.subsystems.at("future"), "x");   // a subsystem this version does not know is not lost
    CB_CHECK_EQ(old.controller.look_sensitivity, 1.0);
    CB_CHECK_EQ(old.controller.binds.at("look"), "RSTICK");
    // a controller object missing some keys; the binds it names replace the default binds
    fs::write_text(dir.path() / "partial.json", R"({"controller": {"layout": "southpaw", "look_sensitivity": 2, "binds": {"look": "A"}}})");
    const pcfg::Config partial = pcfg::load_config(dir.path() / "partial.json");
    CB_CHECK(partial.controller.layout == pcfg::Layout::Southpaw);
    CB_CHECK_EQ(partial.controller.look_sensitivity, 2.0);   // an integer in the file is a number
    CB_CHECK_EQ(partial.controller.stick_deadzone, 0.24);
    CB_CHECK_EQ(partial.controller.binds.size(), std::size_t{1});
});

CB_TEST("java.console.platform_config.unknown_keys_survive_a_save", [] {
    fs::ScopedTempDir dir("cb-pcfg");
    fs::write_text(dir.path() / "x.json",
                   R"({"zeta": {"b": 1, "a": [3, 2]}, "title_id": "FFFE0001", "controller": {"custom": true}})");
    const pcfg::Config cfg = pcfg::load_config(dir.path() / "x.json");
    CB_CHECK_EQ(cfg.title_id, "FFFE0001");
    pcfg::save_config(dir.path() / "y.json", cfg);
    const std::string text = fs::read_text(dir.path() / "y.json");
    // both unknown keys are back, with their object keys sorted, and the top-level order is sorted too
    CB_CHECK(conf_has(text, "\"custom\": true"));
    CB_CHECK(conf_has(text, "\"zeta\": {\n    \"a\": [\n      3,\n      2\n    ],\n    \"b\": 1\n  }"));
    CB_CHECK(text.find("\"title_id\"") < text.find("\"zeta\""));
    CB_CHECK(text.find("\"controller\"") < text.find("\"keyboard\""));
});

CB_TEST("java.console.platform_config.load_rejects_a_config_that_is_not_one", [] {
    fs::ScopedTempDir dir("cb-pcfg");
    auto load = [&](const std::string& json) {
        fs::write_text(dir.path() / "bad.json", json);
        return pcfg::load_config(dir.path() / "bad.json");
    };
    CB_CHECK_THROWS_AS(cb::ParseError, load("{not json"));
    CB_CHECK_THROWS_AS(cb::ParseError, load("[1, 2]"));
    CB_CHECK_THROWS_AS(cb::ParseError, load(R"({"keyboard": "typewriter"})"));
    CB_CHECK_THROWS_AS(cb::ParseError, load(R"({"storage": 7})"));
    CB_CHECK_THROWS_AS(cb::ParseError, load(R"({"controller": {"layout": "sideways"}})"));
    CB_CHECK_THROWS_AS(cb::ParseError, load(R"({"controller": {"invert_y": "yes"}})"));
    CB_CHECK_THROWS_AS(cb::ParseError, load(R"({"controller": {"look_sensitivity": "high"}})"));
    CB_CHECK_THROWS_AS(cb::ParseError, load(R"({"controller": {"binds": {"look": 3}}})"));
    CB_CHECK_THROWS_AS(cb::ParseError, load(R"({"controller": 5})"));
    CB_CHECK_THROWS_AS(cb::ParseError, load(R"({"subsystems": ["audio"]})"));
});

CB_TEST("java.console.platform_config.names_and_tables", [] {
    CB_CHECK(pcfg::parse_layout("southpaw") == pcfg::Layout::Southpaw);
    CB_CHECK(!pcfg::parse_layout("Southpaw"));
    CB_CHECK(pcfg::parse_keyboard("osk") == pcfg::Keyboard::Osk);
    CB_CHECK(!pcfg::parse_keyboard(""));
    CB_CHECK(pcfg::parse_storage("memory") == pcfg::Storage::Memory);
    for (auto l : {pcfg::Layout::Default, pcfg::Layout::Southpaw, pcfg::Layout::Legacy, pcfg::Layout::LegacySouthpaw}) {
        CB_CHECK(pcfg::parse_layout(pcfg::layout_name(l)) == l);
        CB_CHECK_EQ(pcfg::layout_binds(l).size(), std::size_t{4});
    }
    CB_CHECK_EQ(pcfg::controller_actions().size(), std::size_t{15});
    CB_CHECK_EQ(pcfg::controller_buttons().size(), std::size_t{18});
    CB_CHECK_EQ(pcfg::subsystem_defaults().size(), std::size_t{7});
    // every action's default control is one of the controls a binding may target
    for (const auto& a : pcfg::controller_actions()) {
        bool known = false;
        for (std::string_view b : pcfg::controller_buttons()) known = known || b == a.default_binding;
        CB_CHECK(known);
    }
    // ten decisions: layout, keyboard, storage and the seven subsystems; each recommends one of its own options
    const auto& d = pcfg::decisions();
    CB_CHECK_EQ(d.size(), std::size_t{10});
    CB_CHECK_EQ(d[0].id, "controller_layout");
    CB_CHECK_EQ(d[9].id, "achievements");
    for (const pcfg::Decision& dec : d) {
        bool recommended = false;
        for (const pcfg::Option& o : dec.options) recommended = recommended || o.key == dec.recommend_key;
        CB_CHECK(recommended);
        CB_CHECK(!dec.question.empty() && !dec.why.empty());
    }
});

CB_TEST("java.console.platform_config.choice_of_and_effective_binds", [] {
    pcfg::Config c = conf_case("legacy_southpaw_osk_rawhdd");
    CB_CHECK_EQ(pcfg::choice_of(c, "controller_layout"), "legacy_southpaw");
    CB_CHECK_EQ(pcfg::choice_of(c, "keyboard"), "osk");
    CB_CHECK_EQ(pcfg::choice_of(c, "storage"), "rawhdd");
    CB_CHECK_EQ(pcfg::choice_of(c, "audio"), "silent");
    CB_CHECK_EQ(pcfg::choice_of(c, "nonexistent"), "");
    const auto binds = pcfg::effective_binds(c);
    CB_CHECK_EQ(binds.at("look"), "LSTICK");   // the preset wins over the base bind
    CB_CHECK_EQ(binds.at("attack"), "RBUMPER");
    CB_CHECK_EQ(binds.at("jump"), "X");        // the rest is the base bind
    CB_CHECK_EQ(c.controller.binds.at("look"), "RSTICK");   // the config itself is not changed
});

CB_TEST("java.console.platform_config.required_symbols_follow_the_mediums", [] {
    pcfg::Config c = pcfg::default_config();
    CB_CHECK_EQ(pcfg::required_symbols(c).size(), std::size_t{4});   // keyboard UI + three storage calls
    c.keyboard = pcfg::Keyboard::Osk;
    CB_CHECK_EQ(pcfg::required_symbols(c).size(), std::size_t{3});
    c.storage = pcfg::Storage::Memory;
    CB_CHECK(pcfg::required_symbols(c).empty());
});

CB_TEST("java.console.platform_config.gate_needs_no_sdk_when_no_symbol_is_needed", [] {
    pcfg::Config c = pcfg::default_config();
    c.keyboard = pcfg::Keyboard::Stub;
    c.storage = pcfg::Storage::Memory;
    const pcfg::GateResult g = pcfg::gate_sdk(c, "xbox360");
    CB_CHECK(!g.sdk && g.unverified.empty());
    // a portable platform is not gated even when symbols are required
    const pcfg::GateResult pc = pcfg::gate_sdk(pcfg::default_config(), "pc_windows");
    CB_CHECK(!pc.sdk && pc.unverified.empty());
});

CB_TEST("java.console.platform_config.gate_requires_an_sdk_that_declares_the_symbols", [] {
    fs::ScopedTempDir none("cb-pcfg-nosdk");
    fs::ScopedTempDir sdk("cb-pcfg-sdk");
    fs::make_dirs(sdk.path() / "include" / "xbox");
    fs::write_text(sdk.path() / "include" / "xbox" / "xtl.h",
                   "DWORD XShowKeyboardUI(DWORD, DWORD, const wchar_t*);\nDWORD XContentCreate(DWORD);\n");
    cb::java::console::sdk::SdkHints hints;
    // no SDK at the given root: refused
    hints.override_root = none.path() / "missing";
    CB_CHECK_THROWS_AS(cb::java::console::sdk::SdkNotFoundError, pcfg::gate_sdk(pcfg::default_config(), "xbox360", hints));
    // an SDK that has some of the symbols: located, and the rest are reported
    hints.override_root = sdk.path();
    const pcfg::GateResult g = pcfg::gate_sdk(pcfg::default_config(), "xbox360", hints);
    CB_CHECK(g.sdk.has_value());
    CB_CHECK((g.unverified == std::vector<std::string>{"XShowDeviceSelectorUI", "XContentClose"}));
});

CB_TEST("java.console.platform_config.run_applies_updates_saves_and_emits", [] {
    fs::ScopedTempDir dir("cb-pcfg");
    pcfg::RunOptions options;
    options.updates.layout = pcfg::Layout::Southpaw;
    options.updates.invert_y = true;
    options.updates.look_sensitivity = 1.5;
    options.updates.keyboard = pcfg::Keyboard::Stub;
    options.updates.storage = pcfg::Storage::Memory;
    options.updates.binds["jump"] = "X";
    options.updates.binds["brand_new_action"] = "BACK";
    options.updates.subsystems["audio"] = "silent";
    const pcfg::RunResult r = pcfg::run(dir.path(), options);
    CB_CHECK_EQ(r.written.size(), std::size_t{4});
    CB_CHECK(!r.sdk && r.unverified.empty());
    CB_CHECK(r.config.controller.layout == pcfg::Layout::Southpaw && r.config.controller.invert_y);
    CB_CHECK_EQ(r.config.controller.binds.at("jump"), "X");
    CB_CHECK_EQ(r.config.controller.binds.at("brand_new_action"), "BACK");
    // the config was saved and the files carry the choices
    const pcfg::Config saved = pcfg::load_config(dir.path() / "platform.config.json");
    CB_CHECK(saved.controller.layout == pcfg::Layout::Southpaw && saved.keyboard == pcfg::Keyboard::Stub);
    CB_CHECK_EQ(saved.controller.look_sensitivity, 1.5);
    CB_CHECK_EQ(saved.subsystems.at("audio"), "silent");
    const std::string header = fs::read_text(dir.path() / "ControllerConfig.h");
    CB_CHECK(conf_has(header, "#define CTRL_LAYOUT_SOUTHPAW 1"));
    CB_CHECK(conf_has(header, "#define CTRL_INVERT_Y 1"));
    CB_CHECK(conf_has(header, "#define CTRL_LOOK_SENSITIVITY 1.500f"));
    // a second run keeps what the first saved and applies new updates over it
    pcfg::RunOptions again;
    again.updates.invert_y = false;
    const pcfg::RunResult r2 = pcfg::run(dir.path(), again);
    CB_CHECK(r2.config.controller.layout == pcfg::Layout::Southpaw);
    CB_CHECK(!r2.config.controller.invert_y);
    CB_CHECK_EQ(r2.config.controller.binds.at("jump"), "X");
});

CB_TEST("java.console.platform_config.run_list_only_writes_nothing", [] {
    fs::ScopedTempDir dir("cb-pcfg");
    pcfg::RunOptions options;
    options.list_only = true;
    options.updates.layout = pcfg::Layout::Legacy;
    const pcfg::RunResult r = pcfg::run(dir.path(), options);
    CB_CHECK(r.written.empty());
    CB_CHECK(r.config.controller.layout == pcfg::Layout::Legacy);   // the update is applied to what is shown
    CB_CHECK(!stdfs::exists(dir.path() / "platform.config.json"));
    CB_CHECK(!stdfs::exists(dir.path() / "Kernel.cpp"));
});

CB_TEST("java.console.platform_config.run_uses_a_given_config_path_and_gates_before_writing", [] {
    fs::ScopedTempDir dir("cb-pcfg");
    pcfg::RunOptions options;
    options.config_path = dir.path() / "elsewhere.json";
    options.platform = "pc_windows";   // not gated: the default mediums need symbols but nothing is checked
    const pcfg::RunResult r = pcfg::run(dir.path(), options);
    CB_CHECK(stdfs::exists(dir.path() / "elsewhere.json"));
    CB_CHECK(!stdfs::exists(dir.path() / "platform.config.json"));
    CB_CHECK_EQ(r.written.size(), std::size_t{4});
    // an SDK-gated platform with no SDK: refused before anything is written
    fs::ScopedTempDir out("cb-pcfg-gated");
    pcfg::RunOptions gated;
    gated.sdk_hints.override_root = out.path() / "no_sdk_here";
    CB_CHECK_THROWS_AS(cb::java::console::sdk::SdkNotFoundError, pcfg::run(out.path(), gated));
    CB_CHECK(!stdfs::exists(out.path() / "platform.config.json"));
    CB_CHECK(!stdfs::exists(out.path() / "Kernel.cpp"));
});

}  // namespace

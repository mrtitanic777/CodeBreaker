#include <nlohmann/json.hpp>

#include <string>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/platforms.hpp"

namespace {

using namespace cb::java::platforms;
using cb::java::stubs::Subsystem;

const cb::Json& load(const char* name) {
    static std::map<std::string, cb::Json> cache;
    auto it = cache.find(name);
    if (it == cache.end()) it = cache.emplace(name, cb::json::load(cb::testing::data_path(std::string("java/testdata/") + name))).first;
    return it->second;
}

CB_TEST("java.platforms.table", [] {
    auto all = platforms();
    CB_CHECK_EQ(all.size(), std::size_t{7});
    const char* order[] = {"xbox360", "pc_windows", "pc_opengl", "ps3", "wii", "psp", "nds"};
    for (std::size_t i = 0; i < 7; ++i) CB_CHECK_EQ(std::string(all[i].key), order[i]);

    const Platform* x = find_platform("xbox360");
    CB_CHECK(x != nullptr && x->id == PlatformId::Xbox360);
    CB_CHECK(x->big_endian && !x->null_page_protected);
    CB_CHECK_EQ(std::string(*x->sdk_dir), "PlatformSDKs/WindowsSDK7.0");
    CB_CHECK_EQ(std::string(x->api(Subsystem::Audio).sdk), "XACT3");
    CB_CHECK_EQ(std::string(x->api(Subsystem::System).apis), "QueryPerformanceCounter, QueryPerformanceFrequency, GetTickCount");

    const Platform* gl = find_platform("pc_opengl");
    CB_CHECK(!gl->sdk_dir.has_value());          // only the two Windows-SDK platforms name an SDK folder
    CB_CHECK(!gl->big_endian && gl->null_page_protected);
    CB_CHECK_EQ(std::string(gl->api(Subsystem::Networking).sdk), "BSD sockets");
    CB_CHECK(find_platform("amiga") == nullptr);
    CB_CHECK(find_platform("") == nullptr);

    // every platform has all twelve subsystems filled, and the big-endian ones are the console PPC ones
    for (const Platform& p : all) {
        for (const auto& api : p.subsystems) CB_CHECK(!api.sdk.empty() && !api.libs.empty() && !api.apis.empty());
        CB_CHECK_EQ(p.big_endian, p.id == PlatformId::Xbox360 || p.id == PlatformId::Ps3 || p.id == PlatformId::Wii);
    }
});

// testdata/stub_comments.golden.json: get_stub_comment of the Python module for every platform x subsystem key,
// including an unknown platform, an unknown subsystem and an empty one.
CB_TEST("java.platforms.stub_comments_match_python", [] {
    const cb::Json& golden = load("stub_comments.golden.json");
    CB_CHECK_EQ(golden.size(), std::size_t{112});
    for (const auto& [key, expected] : golden.items()) {
        const auto slash = key.find('/');
        CB_CHECK_EQ(stub_comment(key.substr(0, slash), key.substr(slash + 1)), expected.get<std::string>());
    }
});

// testdata/stdafx.golden.json: generate_stdafx of the Python module for each platform, two unknown ids ("generic"
// is what the runtime passes when there is no platform) and one non-default standard label.
CB_TEST("java.platforms.stdafx_matches_python", [] {
    const cb::Json& golden = load("stdafx.golden.json");
    for (const auto& [key, expected] : golden.items()) {
        const auto bar = key.find('|');
        const std::string text = bar == std::string::npos ? generate_stdafx(key) : generate_stdafx(key.substr(0, bar), key.substr(bar + 1));
        CB_CHECK_EQ(text, expected.get<std::string>());
    }
});

CB_TEST("java.platforms.stdafx_spot_checks", [] {
    const std::string x360 = generate_stdafx("xbox360", "C++03");
    CB_CHECK(x360.starts_with("// stdafx.h -- precompiled header for Xbox 360 (C++03)\n"));
    CB_CHECK(x360.find("#include <xtl.h>") != std::string::npos);
    CB_CHECK(x360.find("extern IDirect3DDevice9* g_pd3dDevice;") != std::string::npos);
    CB_CHECK(x360.find("j2x_wider") == std::string::npos);       // the host min/max overloads are not for the console
    const std::string gl = generate_stdafx("pc_opengl");
    CB_CHECK(gl.find("j2x_wider") != std::string::npos);
    CB_CHECK(gl.find("#define NOGDI") != std::string::npos);    // GDI's Rectangle() would hide java.awt.Rectangle
    const std::string generic = generate_stdafx("generic");
    CB_CHECK(generic.starts_with("// stdafx.h -- precompiled header for generic\n#pragma once\n"));
});

}  // namespace

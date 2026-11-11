// cb/java/platforms.hpp -- target platform definitions for stub annotation.
//
// Each platform maps the converter's subsystem ids (see stubs.hpp) to the platform-specific SDK, API and library
// names that should fill that stub. The converter emits these as comments inside the stub body so the developer
// knows exactly what to reach for. The platform also knows its stdafx.h.
#pragma once

#include <array>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "cb/java/stubs.hpp"

namespace cb::java::platforms {

enum class PlatformId { Xbox360, PcWindows, PcOpengl, Ps3, Wii, Psp, Nds };

// What a platform offers for one stub subsystem.
struct SubsystemApi {
    std::string_view sdk;
    std::string_view libs;
    std::string_view apis;
};

struct Platform {
    PlatformId id;
    std::string_view key;           // "xbox360", "pc_windows", "pc_opengl", "ps3", "wii", "psp", "nds"
    std::string_view label;
    std::string_view description;
    std::optional<std::string_view> sdk_dir;   // the SDK folder under the XDK tree, for the platforms that have one
    bool big_endian = false;
    bool null_page_protected = false;
    std::string_view null_deref_msg;
    // Indexed by stubs::Subsystem.
    std::array<SubsystemApi, 12> subsystems;

    const SubsystemApi& api(stubs::Subsystem s) const { return subsystems[static_cast<std::size_t>(s)]; }
};

// Every platform, in the order reports list them.
std::span<const Platform> platforms();

// The platform with that key, or nullptr.
const Platform* find_platform(std::string_view key);

// A comment for a stub body, given a platform and a subsystem key ("rendering", ... or "unknown"):
// `// TODO [Xbox 360]: Implement with D3D9 (d3d9.lib, d3dx9.lib) -- IDirect3DDevice9, ...`. An unknown platform or
// subsystem gets the generic wording.
std::string stub_comment(std::string_view platform_key, std::string_view subsystem_key);

// The platform-specific stdafx.h (a precompiled header). An unknown platform key gets the generic header, which
// brings in the Windows headers on a Windows host and Java's min/max overloads where no macro exists.
std::string generate_stdafx(std::string_view platform_key, std::string_view cpp_std_label = "C++03");

}  // namespace cb::java::platforms

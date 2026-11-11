// cb/java/stubs.hpp -- classify the converter's stubs by target subsystem.
//
// Every stub header the converter emits needs a REAL implementation backed by platform-specific APIs. This module
// classifies a stub class (rendering, audio, input, io, networking, ...) and says what SDK/platform feature the
// stub needs, so a port knows which SDK APIs map to which stub methods.
#pragma once

#include <optional>
#include <span>
#include <string_view>
#include <vector>

namespace cb::java::stubs {

enum class Subsystem {
    Rendering,
    Audio,
    Input,
    Io,
    Compression,
    Networking,
    Nio,
    Threading,
    Text,
    Reflection,
    Exceptions,
    System,
};

struct SubsystemInfo {
    Subsystem id;
    std::string_view key;           // the short name reports use: "rendering", "audio", ...
    std::string_view label;
    std::string_view description;
    std::string_view sdk_hint;
    std::vector<std::string_view> members;   // the stub class names, sorted
};

// Every subsystem, in the order reports list them.
std::span<const SubsystemInfo> subsystems();

// The subsystem a stub class belongs to, or nullptr for a class no subsystem claims. A class named by two
// subsystems (ConnectException) belongs to the first in `subsystems()` order.
const SubsystemInfo* classify_stub(std::string_view class_name);

// The subsystem named by its short key ("rendering", "audio", ...), or nullopt for any other text.
std::optional<Subsystem> subsystem_from_key(std::string_view key);

}  // namespace cb::java::stubs


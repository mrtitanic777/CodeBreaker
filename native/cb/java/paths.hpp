// cb/java/paths.hpp -- where the data the Java transpiler ships with is found: the platform layer
// (platform/canonical, reference, companions, assets) and the API maps (api_maps/), all inside the lane's
// java folder. Machine locations (the XDK, Java, the decompiler jar, overlays) are settings, not here.
//
// A JavaData is made by whoever owns the conversion and handed down; nothing holds it globally.
#pragma once

#include <filesystem>
#include <initializer_list>
#include <string_view>
#include <utility>

namespace cb::java {

class JavaData {
public:
    explicit JavaData(std::filesystem::path root) : root_(std::move(root)) {}

    // The folder of a checkout's Java lane: $CB_CHECKOUT_ROOT/codebreaker/java when the variable is set, else the
    // nearest codebreaker/java above the current directory. NotFoundError if neither exists.
    static JavaData locate();

    const std::filesystem::path& root() const { return root_; }

    // A data file or folder shipped with the lane: data.path({"platform", "canonical"}).
    std::filesystem::path path(std::initializer_list<std::string_view> parts) const;

private:
    std::filesystem::path root_;
};

}  // namespace cb::java

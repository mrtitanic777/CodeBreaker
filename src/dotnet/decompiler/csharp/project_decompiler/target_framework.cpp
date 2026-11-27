#include "dotnet/decompiler/csharp/project_decompiler/target_framework.hpp"

#include <stdexcept>

namespace cb::dotnet::decompiler::csharp::project_decompiler {

namespace {
const char* const kDotNetPortableIdentifier = ".NETPortable";
}

TargetFramework::TargetFramework(std::optional<std::string> identifier, int version, std::optional<std::string> profile) {
    if (version < 100) throw std::invalid_argument("The version number must be greater than or equal to 100");

    identifier_ = std::move(identifier);
    version_number_ = version;
    version_string_ = "v" + get_version_string(version, /*with_dots=*/true);
    moniker_ = get_target_framework_moniker(identifier_, version);
    profile_ = std::move(profile);
    is_portable_class_library_ = identifier_.has_value() && *identifier_ == kDotNetPortableIdentifier;
}

std::optional<std::string> TargetFramework::get_target_framework_moniker(const std::optional<std::string>& framework_identifier, int version) {
    // Reference: https://docs.microsoft.com/en-us/dotnet/standard/frameworks
    if (!framework_identifier || *framework_identifier == ".NETFramework") return "net" + get_version_string(version, /*with_dots=*/false);

    const std::string& id = *framework_identifier;
    if (id == ".NETCoreApp") {
        if (version >= 500) return "net" + get_version_string(version, /*with_dots=*/true);
        return "netcoreapp" + get_version_string(version, /*with_dots=*/true);
    }
    if (id == ".NETStandard") return "netstandard" + get_version_string(version, /*with_dots=*/true);
    if (id == "Silverlight") return "sl" + std::to_string(version / 100);
    if (id == ".NETCore") return "netcore" + get_version_string(version, /*with_dots=*/false);
    if (id == "WindowsPhone") return "wp" + get_version_string(version, /*with_dots=*/false, /*omit_minor_when_zero=*/true);
    if (id == ".NETMicroFramework") return "netmf";
    return std::nullopt;
}

std::string TargetFramework::get_version_string(int version, bool with_dots, bool omit_minor_when_zero) {
    const int major = version / 100;
    const int minor = version % 100 / 10;
    const int patch = version % 10;

    if (omit_minor_when_zero && minor == 0 && patch == 0) return std::to_string(major);

    std::string result = std::to_string(major);
    if (with_dots) result.push_back('.');
    result += std::to_string(minor);
    if (patch != 0) {
        if (with_dots) result.push_back('.');
        result += std::to_string(patch);
    }
    return result;
}

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler

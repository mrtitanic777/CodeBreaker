// dotnet/decompiler/csharp/project_decompiler/target_framework.hpp -- the .NET framework a module was built for: its identifier (".NETFramework",
// ".NETCoreApp" ...), version (as a number: 450 for 4.5), profile, and the moniker a project file names it by ("net45", "net8.0").
//
//     TargetFramework tf(".NETCoreApp", 800, std::nullopt);
//     tf.moniker() == "net8.0";   tf.version_string() == "v8.0";   tf.version_number() == 800;
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.ProjectDecompiler: TargetFramework, from TargetFramework.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). Upstream's null strings (an unknown identifier, a missing profile or moniker) are
// std::nullopt.
#pragma once

#include <optional>
#include <string>

namespace cb::dotnet::decompiler::csharp::project_decompiler {

// A class describing the target framework of a module.
class TargetFramework {
public:
    // `identifier`: the framework identifier string (nullopt when not defined). `version`: the framework version, which must be at least 100 (where
    // 100 corresponds to v1.0); std::invalid_argument otherwise. `profile`: the framework profile (nullopt when none).
    TargetFramework(std::optional<std::string> identifier, int version, std::optional<std::string> profile);

    // The target framework identifier; nullopt if not defined.
    const std::optional<std::string>& identifier() const { return identifier_; }
    // The target framework moniker; nullopt if not supported.
    const std::optional<std::string>& moniker() const { return moniker_; }
    // The target framework version, e.g. "v4.5".
    const std::string& version_string() const { return version_string_; }
    // The target framework version as integer (multiplied by 100), e.g. 450.
    int version_number() const { return version_number_; }
    // The target framework profile; nullopt if not set or not available.
    const std::optional<std::string>& profile() const { return profile_; }
    // Whether the target is a portable class library (PCL).
    bool is_portable_class_library() const { return is_portable_class_library_; }

private:
    static std::optional<std::string> get_target_framework_moniker(const std::optional<std::string>& framework_identifier, int version);
    static std::string get_version_string(int version, bool with_dots, bool omit_minor_when_zero = false);

    std::optional<std::string> identifier_, moniker_, profile_;
    std::string version_string_;
    int version_number_;
    bool is_portable_class_library_;
};

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler

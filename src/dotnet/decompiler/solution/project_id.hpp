// dotnet/decompiler/solution/project_id.hpp -- what identifies a Visual Studio project: its platform, its GUID, and the GUID of its kind.
//
//     ProjectId id("AnyCPU", util::guid::new_guid(), ProjectTypeGuids::csharp_windows());
//     id.platform_name();   id.guid();   id.type_guid();
//
// Ported from ILSpy (ICSharpCode.Decompiler.Solution: ProjectId and ProjectTypeGuids, from ProjectId.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). The project type GUIDs are functions (upstream's static readonly fields).
#pragma once

#include <string>

#include "dotnet/decompiler/util/guid.hpp"

namespace cb::dotnet::decompiler::solution {

// A container class that holds platform and GUID information about a Visual Studio project.
class ProjectId {
public:
    // UsageError (an ArgumentException upstream) when the platform is empty or white space only.
    ProjectId(std::string project_platform, util::Guid project_guid, util::Guid type_guid);

    // The GUID of this project. This is usually a newly generated GUID for each decompiled project.
    const util::Guid& guid() const { return guid_; }
    // The primary type GUID of this project: one of the GUIDs of ProjectTypeGuids.
    const util::Guid& type_guid() const { return type_guid_; }
    // The platform name of this project. Only single platform per project is supported.
    const std::string& platform_name() const { return platform_name_; }

private:
    util::Guid guid_, type_guid_;
    std::string platform_name_;
};

struct ProjectTypeGuids {
    static util::Guid solution_folder();
    static util::Guid csharp_windows();
    static util::Guid csharp_core();
    static util::Guid silverlight();
    static util::Guid portable_library();
};

}  // namespace cb::dotnet::decompiler::solution

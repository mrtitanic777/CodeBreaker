#include "dotnet/decompiler/solution/project_id.hpp"

#include "core/error.hpp"

namespace cb::dotnet::decompiler::solution {

ProjectId::ProjectId(std::string project_platform, util::Guid project_guid, util::Guid type_guid)
    : guid_(project_guid), type_guid_(type_guid), platform_name_(std::move(project_platform)) {
    if (platform_name_.find_first_not_of(" \t\r\n\v\f") == std::string::npos) throw UsageError("The platform cannot be null or empty.");
}

util::Guid ProjectTypeGuids::solution_folder() { return util::guid::parse("{2150E333-8FDC-42A3-9474-1A3956D46DE8}"); }
util::Guid ProjectTypeGuids::csharp_windows() { return util::guid::parse("{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}"); }
util::Guid ProjectTypeGuids::csharp_core() { return util::guid::parse("{9A19103F-16F7-4668-BE54-9A1E7A4F7556}"); }
util::Guid ProjectTypeGuids::silverlight() { return util::guid::parse("{A1591282-1198-4647-A2B1-27E5FF5F6F3B}"); }
util::Guid ProjectTypeGuids::portable_library() { return util::guid::parse("{786C830F-07A1-408B-BD7F-6EE04809D6DB}"); }

}  // namespace cb::dotnet::decompiler::solution

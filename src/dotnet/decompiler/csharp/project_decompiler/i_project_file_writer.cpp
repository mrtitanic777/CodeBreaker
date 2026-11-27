#include "dotnet/decompiler/csharp/project_decompiler/i_project_file_writer.hpp"

#include "dotnet/decompiler/csharp/project_decompiler/project_file_writer_default.hpp"
#include "dotnet/decompiler/csharp/project_decompiler/project_file_writer_sdk_style.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

const IProjectFileWriter& project_file_writer_from_settings(const DecompilerSettings& settings) {
    return settings.use_sdk_style_project_format ? static_cast<const IProjectFileWriter&>(ProjectFileWriterSdkStyle::default_instance())
                                                 : static_cast<const IProjectFileWriter&>(ProjectFileWriterDefault::instance());
}

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler

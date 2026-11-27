// dotnet/decompiler/decompile_run.cpp -- see decompile_run.hpp.
#include "dotnet/decompiler/decompile_run.hpp"

#include <stdexcept>

#include "dotnet/decompiler/csharp/record_decompiler.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"

namespace cb::dotnet::decompiler {

DecompileRun::DecompileRun(const DecompilerSettings& settings_, const csharp::type_system::UsingScope* using_scope_)
    : settings(settings_), using_scope(using_scope_) {
    if (using_scope_ == nullptr) throw std::invalid_argument("using_scope");
}

DecompileRun::~DecompileRun() = default;

}  // namespace cb::dotnet::decompiler

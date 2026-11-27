#include "dotnet/decompiler/decompiler_exception.hpp"

#include <format>
#include <typeinfo>

#include "dotnet/decompiler/util/dotnet_path.hpp"

namespace cb::dotnet::decompiler {

DecompilerError::DecompilerError(const typesystem::IModule& module, const typesystem::IEntity* decompiled_entity, const std::exception& inner_exception,
                                 const std::string& message)
    : Error(message.empty() ? default_message(decompiled_entity) : message),
      file_(module.metadata_file()),
      module_(&module),
      decompiled_entity_(decompiled_entity) {
    set_inner(inner_exception);
}

DecompilerError::DecompilerError(const metadata::MetadataFile& file, const std::string& message, const std::exception& inner_exception)
    : Error(message), file_(&file) {
    set_inner(inner_exception);
}

void DecompilerError::set_inner(const std::exception& inner) {
    if (const auto* e = dynamic_cast<const Error*>(&inner)) inner_type_ = std::string(e->kind());
    else inner_type_ = typeid(inner).name();
    inner_message_ = inner.what();
}

std::string DecompilerError::default_message(const typesystem::IEntity* entity) {
    if (entity == nullptr) return "Error decompiling";
    return std::format("Error decompiling @{:08X} {}", cb::dotnet::metadata::get_token(entity->metadata_token()), entity->full_name());
}

const std::string& DecompilerError::assembly_name() const { return file_->name(); }

std::string DecompilerError::file_name() const { return util::path::native_text(file_->file_name()); }

std::string DecompilerError::to_string() const {
    return std::string(what()) + "\r\nin assembly \"" + file_name() + "\"\r\n ---> " + inner_type_ + ": " + inner_message_ + "\r\n";
}

}  // namespace cb::dotnet::decompiler

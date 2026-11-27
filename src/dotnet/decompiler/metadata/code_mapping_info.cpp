// dotnet/decompiler/metadata/code_mapping_info.cpp -- see code_mapping_info.hpp.
#include "dotnet/decompiler/metadata/code_mapping_info.hpp"

namespace cb::dotnet::decompiler::metadata {

std::vector<srm::MethodDefinitionHandle> CodeMappingInfo::get_method_parts(srm::MethodDefinitionHandle method) const {
    auto it = parts_.find(method);
    if (it != parts_.end()) return it->second;
    return {method};
}

srm::MethodDefinitionHandle CodeMappingInfo::get_parent_method(srm::MethodDefinitionHandle method) const {
    auto it = parents_.find(method);
    if (it != parents_.end()) return it->second;
    return method;
}

void CodeMappingInfo::add_mapping(srm::MethodDefinitionHandle parent, srm::MethodDefinitionHandle part) {
    if (parents_.contains(part)) return;
    parents_.emplace(part, parent);
    parts_[parent].push_back(part);
}

}  // namespace cb::dotnet::decompiler::metadata

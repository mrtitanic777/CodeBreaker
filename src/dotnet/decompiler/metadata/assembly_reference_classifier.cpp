#include "dotnet/decompiler/metadata/assembly_reference_classifier.hpp"

#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"

namespace cb::dotnet::decompiler::metadata {

bool AssemblyReferenceClassifier::is_gac_assembly(const IAssemblyReference& reference) {
    return UniversalAssemblyResolver::get_assembly_in_gac(reference).has_value();
}

bool AssemblyReferenceClassifier::is_shared_assembly(const IAssemblyReference&, std::string* runtime_pack) {
    if (runtime_pack != nullptr) runtime_pack->clear();
    return false;
}

}  // namespace cb::dotnet::decompiler::metadata

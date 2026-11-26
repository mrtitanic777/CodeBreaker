// dotnet/decompiler/il/il_ast_dump.cpp -- see il_ast_dump.hpp.
#include "dotnet/decompiler/il/il_ast_dump.hpp"

#include <format>

#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::il {

namespace srm = cb::dotnet::metadata;

std::string dump_il_ast(const typesystem::DecompilerTypeSystem& type_system) {
    const typesystem::MetadataModule& module = type_system.main_metadata_module();
    const metadata::MetadataFile& file = *module.metadata_file();
    const srm::MetadataReader& md = module.metadata();
    std::string text;
    for (srm::MethodDefinitionHandle h : md.method_definitions()) {
        const srm::MethodDefinition def = md.get_method_definition(h);
        if (def.relative_virtual_address() == 0) continue;
        text += std::format("method {:08x} {}::{}\n", srm::get_token(h), md.get_string(md.get_type_definition(def.get_declaring_type()).name()),
                            md.get_string(def.name()));
        ILArena arena;
        ILArenaScope scope(arena);
        PlainTextOutput output("\n");
        try {
            const srm::MethodBodyBlock body = file.get_method_body(def.relative_virtual_address());
            ILReader reader(module);
            reader.use_debug_symbols = false;
            reader.use_ref_locals_for_accurate_order_of_evaluation = true;
            ILFunction* function = reader.read_il(h, body);
            function->write_to(output, ILAstWritingOptions{});
            text += output.text();
            text += '\n';
        } catch (const srm::BadImageFormatError& ex) {
            text += output.text();
            text += "error BadImageFormatException: " + ex.message() + "\n";
        }
    }
    return text;
}

}  // namespace cb::dotnet::decompiler::il

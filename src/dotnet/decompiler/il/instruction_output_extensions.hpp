// dotnet/decompiler/il/instruction_output_extensions.hpp -- a metadata entity written as IL names it: a type ([System.Runtime]System.Type,
// Outer/Inner, a generic instance), a field (int32 Type::name), a method or method reference with its signature, a method instance, a
// standalone signature; and a signature header's "instance", "explicit" and calling convention.
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: InstructionOutputExtensions, the metadata half), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). The half for ILAst opcodes, stack types, IType and IMember comes with the IL AST.
#pragma once

#include "dotnet/decompiler/disassembler/disassembler_signature_type_provider.hpp"

namespace cb::dotnet::decompiler::il {

namespace srm = cb::dotnet::metadata;
using disassembler::ILNameSyntax;

void write_to(srm::EntityHandle entity, const metadata::MetadataFile& module, ITextOutput& output, const metadata::MetadataGenericContext& generic_context,
              ILNameSyntax syntax = ILNameSyntax::Signature);
void write_to(const srm::SignatureHeader& header, ITextOutput& output);
void write_parameter_list(ITextOutput& output, const srm::MethodSignature<disassembler::TypeWriter>& signature);

}  // namespace cb::dotnet::decompiler::il

// dotnet/decompiler/decompiler_exception.hpp -- what goes wrong while decompiling, with where: the assembly, and the entity (a method, a
// type) being decompiled when it did.
//
//     try { ... } catch (const std::exception& ex) { throw DecompilerError(module, entity, ex); }
//     error.assembly_name();   error.file_name();   error.decompiled_entity();   error.to_string();
//
// Ported from ILSpy (ICSharpCode.Decompiler: DecompilerException, from DecompilerException.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler). Named DecompilerError as the errors of this port are. Not ported: a stack trace (what to_string() adds
// upstream after the message: C++ has none to show), serialization. to_string() is the message, the assembly, and the cause (its type and its
// message), as upstream's ToString() writes them without the frames. The file and the module are those the caller keeps alive (an error is shown
// while the decompiler's modules are loaded).
#pragma once

#include <exception>
#include <string>
#include <string_view>

#include "core/error.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler {

class DecompilerError : public Error {
public:
    // While decompiling an entity (nullptr: none: the message is "Error decompiling"); the message is "Error decompiling @06000001 Ns.Type.Method"
    // when none is given.
    DecompilerError(const typesystem::IModule& module, const typesystem::IEntity* decompiled_entity, const std::exception& inner_exception,
                    const std::string& message = std::string());
    // A failure of a whole file or of something not tied to an entity.
    DecompilerError(const metadata::MetadataFile& file, const std::string& message, const std::exception& inner_exception);

    const std::string& assembly_name() const;
    // The file name of the file (a path as text); "" when the module is not backed by a file.
    std::string file_name() const;
    const typesystem::IEntity* decompiled_entity() const { return decompiled_entity_; }
    const typesystem::IModule* module() const { return module_; }
    const metadata::MetadataFile& file() const { return *file_; }

    // The error's own type name (the kind of the cause when it is one of CodeBreaker's errors) and message, of the cause.
    const std::string& inner_type() const { return inner_type_; }
    const std::string& inner_message() const { return inner_message_; }
    // The message, the assembly and the cause, one per line as upstream's ToString() writes them.
    std::string to_string() const;

    CB_ERROR_BOILERPLATE(DecompilerError)

private:
    static std::string default_message(const typesystem::IEntity* entity);
    void set_inner(const std::exception& inner);

    const metadata::MetadataFile* file_ = nullptr;
    const typesystem::IModule* module_ = nullptr;
    const typesystem::IEntity* decompiled_entity_ = nullptr;
    std::string inner_type_, inner_message_;
};

}  // namespace cb::dotnet::decompiler

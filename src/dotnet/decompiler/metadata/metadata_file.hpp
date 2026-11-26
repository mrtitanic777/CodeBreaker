// dotnet/decompiler/metadata/metadata_file.hpp -- a file the decompiler reads: its metadata, and for a PE file its method bodies and
// sections. What every part of the decompiler takes a module as.
//
//     auto module = PEFile::open("Game.exe");            // shared: resolvers and type systems hold modules
//     module->name();                                     // the assembly's name ("Game")
//     module->get_method_body(rva);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: MetadataFile, PEFile, SectionHeader; MetadataGenericContext), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported: the lookups the type system brings
// (GetTypeDefinition by name, type forwarders, method semantics, backing fields, WithOptions) and the resources; they come with it.
// SectionData is the section's bytes from the RVA on (a ByteView).
#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "dotnet/metadata/pe_reader.hpp"

namespace cb::dotnet::decompiler::metadata {

namespace srm = cb::dotnet::metadata;

// ICSharpCode.Decompiler.TargetRuntime
enum class TargetRuntime : std::uint8_t { Unknown, Net_1_0, Net_1_1, Net_2_0, Net_4_0 };

// The file is not a .NET module (MetadataFileNotSupportedException).
class MetadataFileNotSupportedError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(MetadataFileNotSupportedError)
};

class MetadataFile {
public:
    enum class Kind : std::uint8_t { PortableExecutable, ProgramDebugDatabase, WebCIL, Metadata };
    virtual ~MetadataFile() = default;
    MetadataFile(const MetadataFile&) = delete;
    MetadataFile& operator=(const MetadataFile&) = delete;

    Kind kind() const { return kind_; }
    const std::filesystem::path& file_name() const { return file_name_; }
    const srm::MetadataReader& metadata() const { return *metadata_; }
    virtual int metadata_offset() const { return 0; }
    virtual bool is_embedded() const { return false; }
    virtual bool is_metadata_only() const { return true; }
    bool is_assembly() const { return metadata_->is_assembly(); }
    // The assembly's name, else the module's.
    const std::string& name() const;
    // "Name, Version=1.0.0.0, Culture=neutral, PublicKeyToken=..." for an assembly, else the name.
    const std::string& full_name() const;
    TargetRuntime get_runtime() const;

    // A method body by RVA, the bytes of a section from an RVA, the section holding an RVA: BadImageFormatError for a file without them.
    virtual srm::MethodBodyBlock get_method_body(int rva) const;
    virtual ByteView get_section_data(int rva) const;
    virtual int get_containing_section_index(int rva) const;
    virtual const std::vector<srm::SectionHeader>& section_headers() const;
    virtual const srm::CorHeader* cor_header() const { return nullptr; }

protected:
    MetadataFile(Kind kind, std::filesystem::path file_name, const srm::MetadataReader* metadata)
        : kind_(kind), file_name_(std::move(file_name)), metadata_(metadata) {}

private:
    Kind kind_;
    std::filesystem::path file_name_;
    const srm::MetadataReader* metadata_;
    mutable std::optional<std::string> name_, full_name_;
};

class PEFile final : public MetadataFile {
public:
    // A PE file with a CLR header; MetadataFileNotSupportedError for one without (FormatError for a file that is not PE).
    static std::shared_ptr<PEFile> open(const std::filesystem::path& file);
    static std::shared_ptr<PEFile> from_bytes(std::filesystem::path file_name, Bytes image);

    const srm::PEReader& reader() const { return *reader_; }
    bool is_embedded() const override { return false; }
    int metadata_offset() const override;
    bool is_metadata_only() const override { return false; }
    srm::MethodBodyBlock get_method_body(int rva) const override;
    ByteView get_section_data(int rva) const override;
    int get_containing_section_index(int rva) const override;
    const std::vector<srm::SectionHeader>& section_headers() const override { return reader_->section_headers(); }
    const srm::CorHeader* cor_header() const override { return &reader_->cor_header(); }

    PEFile(std::filesystem::path file_name, std::unique_ptr<srm::PEReader> reader);

private:
    std::unique_ptr<srm::PEReader> reader_;
};

// The generic parameters a signature's !n and !!n refer to: those of a type, and of a method in it.
class MetadataGenericContext {
public:
    MetadataGenericContext() = default;
    MetadataGenericContext(srm::MethodDefinitionHandle method, const srm::MetadataReader& metadata);
    MetadataGenericContext(srm::MethodDefinitionHandle method, const MetadataFile& module) : MetadataGenericContext(method, module.metadata()) {}
    MetadataGenericContext(srm::TypeDefinitionHandle declaring_type, const srm::MetadataReader& metadata)
        : metadata_(&metadata), declaring_type_(declaring_type) {}
    MetadataGenericContext(srm::TypeDefinitionHandle declaring_type, const MetadataFile& module)
        : MetadataGenericContext(declaring_type, module.metadata()) {}

    // A parameter's name, or its index when it has none.
    std::string get_generic_type_parameter_name(int index) const;
    std::string get_generic_method_type_parameter_name(int index) const;
    // The parameter's handle; nil when there is no such parameter.
    srm::GenericParameterHandle get_generic_type_parameter_handle_or_null(int index) const;
    srm::GenericParameterHandle get_generic_method_type_parameter_handle_or_null(int index) const;

private:
    const srm::MetadataReader* metadata_ = nullptr;
    srm::TypeDefinitionHandle declaring_type_;
    srm::MethodDefinitionHandle method_;
};

}  // namespace cb::dotnet::decompiler::metadata

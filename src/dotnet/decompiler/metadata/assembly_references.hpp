// dotnet/decompiler/metadata/assembly_references.hpp -- references to assemblies, and what finds the file one names.
//
//     AssemblyNameReference r = AssemblyNameReference::parse("System.Runtime, Version=9.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a");
//     r.name() == "System.Runtime";   r.full_name();
//     std::shared_ptr<MetadataFile> file = resolver.resolve(r);     // an IAssemblyResolver; nullptr when it is not found
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: ResolutionException, IAssemblyResolver, IAssemblyReference, AssemblyNameReference,
// AssemblyReference in AssemblyReferences.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The
// async halves of the resolver are not ported; EnumUnderlyingTypeResolveException (its own file upstream) is here (resolving is synchronous here); AssemblyReference's TypeReferences and ExportedTypes (a
// viewer's lists) and the reference classifier come with the assembly resolver.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler::metadata {

// A version as System.Version holds one: two to four parts (a missing build or revision is -1).
struct AssemblyVersion {
    int major = 0, minor = 0, build = -1, revision = -1;
    // System.Version.Parse: "1.2", "1.2.3" or "1.2.3.4"; UsageError for anything else.
    static AssemblyVersion parse(std::string_view text);
    // Version.ToString(4); UsageError when the version has fewer parts (upstream's ArgumentException).
    std::string to_string4() const;
    std::string to_string() const;   // the parts it has
};

// An enum's underlying type could not be found (Metadata.EnumUnderlyingTypeResolveException).
class EnumUnderlyingTypeResolveError : public Error {
public:
    EnumUnderlyingTypeResolveError() : Error("the underlying type of an enum could not be resolved") {}
    CB_ERROR_BOILERPLATE(EnumUnderlyingTypeResolveError)
};

class ResolutionError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(ResolutionError)
};

class IAssemblyReference {
public:
    virtual ~IAssemblyReference() = default;
    virtual std::string name() const = 0;
    virtual std::string full_name() const = 0;
    virtual std::optional<AssemblyVersion> version() const = 0;
    virtual std::optional<std::string> culture() const = 0;
    virtual std::optional<Bytes> public_key_token() const = 0;
    virtual bool is_windows_runtime() const = 0;
    virtual bool is_retargetable() const = 0;
};

class IAssemblyResolver {
public:
    virtual ~IAssemblyResolver() = default;
    virtual std::shared_ptr<MetadataFile> resolve(const IAssemblyReference& reference) = 0;
    virtual std::shared_ptr<MetadataFile> resolve_module(const MetadataFile& main_module, std::string_view module_name) = 0;
};

// A reference parsed from an assembly's display name.
class AssemblyNameReference final : public IAssemblyReference {
public:
    // UsageError for an empty name or a malformed part.
    static AssemblyNameReference parse(std::string_view full_name);

    std::string name() const override { return name_; }
    std::string full_name() const override;
    std::optional<AssemblyVersion> version() const override { return version_; }
    std::optional<std::string> culture() const override { return culture_; }
    std::optional<Bytes> public_key_token() const override { return public_key_token_; }
    bool is_windows_runtime() const override { return false; }
    bool is_retargetable() const override { return false; }
    std::string to_string() const { return full_name(); }

private:
    std::string name_;
    std::optional<AssemblyVersion> version_;
    std::optional<std::string> culture_;
    std::optional<Bytes> public_key_token_;
};

// A reference from a module's AssemblyRef table.
class AssemblyReference final : public IAssemblyReference {
public:
    AssemblyReference(const srm::MetadataReader& metadata, srm::AssemblyReferenceHandle handle);

    const srm::MetadataReader& metadata() const { return *metadata_; }
    srm::AssemblyReferenceHandle handle() const { return handle_; }
    std::string name() const override;
    std::string full_name() const override;
    std::optional<AssemblyVersion> version() const override;
    std::optional<std::string> culture() const override;
    std::optional<Bytes> public_key_token() const override;
    bool is_windows_runtime() const override;
    bool is_retargetable() const override;
    std::string to_string() const { return full_name(); }

private:
    const srm::MetadataReader* metadata_;
    srm::AssemblyReferenceHandle handle_;
};

}  // namespace cb::dotnet::decompiler::metadata

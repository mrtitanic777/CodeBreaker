// dotnet/decompiler/metadata/resource.hpp -- the resources of a .NET module: what its ManifestResource table lists (embedded in the file, linked
// to another file, or in another assembly), and resources made in memory; with the bytes of an embedded one.
//
//     for (const auto& r : get_resources(*module))
//         if (r->resource_type() == ResourceType::Embedded)
//             if (std::optional<ByteView> bytes = r->try_open_stream()) ...      // the resource's data, valid while the module is
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: ResourceType, Resource, ByteArrayResource, MetadataResource, from Resource.cs; MetadataFile.Resources
// as get_resources), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). The "stream" upstream opens is a ByteView
// of the data (it reads from position 0; a stream of ByteArrayResource is a view of the resource's own bytes). Where upstream's stream would
// run past the end of the resources section (a length field that does not fit), the resource has no stream here.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/metadata/handles.hpp"

namespace cb::dotnet::decompiler::metadata {

enum class ResourceType { Linked, Embedded, AssemblyLinked };

// System.Reflection.ManifestResourceAttributes
inline constexpr std::uint32_t kManifestResourcePublic = 0x0001;
inline constexpr std::uint32_t kManifestResourcePrivate = 0x0002;
inline constexpr std::uint32_t kManifestResourceVisibilityMask = 0x0007;

class Resource {
public:
    virtual ~Resource() = default;
    virtual ResourceType resource_type() const { return ResourceType::Embedded; }
    virtual std::uint32_t attributes() const { return kManifestResourcePublic; }
    virtual std::string name() const = 0;
    // The data of the resource; nullopt when it cannot be read (a linked resource, a length that does not fit the file).
    virtual std::optional<ByteView> try_open_stream() const = 0;
    virtual std::optional<std::int64_t> try_get_length() const = 0;
};

class ByteArrayResource final : public Resource {
public:
    ByteArrayResource(std::string name, Bytes data) : name_(std::move(name)), data_(std::move(data)) {}
    std::string name() const override { return name_; }
    std::optional<ByteView> try_open_stream() const override { return ByteView(data_); }
    std::optional<std::int64_t> try_get_length() const override { return static_cast<std::int64_t>(data_.size()); }

private:
    std::string name_;
    Bytes data_;
};

// A row of the ManifestResource table of a module (which must stay alive as long as this object).
class MetadataResource final : public Resource {
public:
    MetadataResource(const MetadataFile& module, cb::dotnet::metadata::ManifestResourceHandle handle) : module_(&module), handle_(handle) {}

    const MetadataFile& module() const { return *module_; }
    cb::dotnet::metadata::ManifestResourceHandle handle() const { return handle_; }
    bool is_nil() const { return handle_.is_nil(); }
    std::string name() const override;
    std::uint32_t attributes() const override;
    bool has_flag(std::uint32_t flag) const { return (attributes() & flag) == flag; }
    ResourceType resource_type() const override;
    std::optional<ByteView> try_open_stream() const override;
    std::optional<std::int64_t> try_get_length() const override;
    friend bool operator==(const MetadataResource& a, const MetadataResource& b) { return a.module_ == b.module_ && a.handle_ == b.handle_; }

private:
    // The bytes of an embedded resource in the module's resources section; false for any other, or one that does not fit.
    bool try_read_resource(ByteView& data) const;

    const MetadataFile* module_;
    cb::dotnet::metadata::ManifestResourceHandle handle_;
};

// MetadataFile.Resources: a MetadataResource for every row of the ManifestResource table, in order.
std::vector<std::unique_ptr<Resource>> get_resources(const MetadataFile& module);

}  // namespace cb::dotnet::decompiler::metadata

// dotnet/metadata/pe_reader.hpp -- a .NET PE image (any PE, 32- or 64-bit, with a CLR header): the CLR header (II.25.3.3), the
// metadata it points at, section data by RVA and method bodies (II.25.4), as System.Reflection.Metadata's PEReader and
// MethodBodyBlock give them. On top of formats/pe, which reads the PE headers.
//
//     PEReader pe = PEReader::open("Game.exe");
//     const MetadataReader& md = pe.metadata();
//     MethodBodyBlock body = pe.get_method_body(md.get_method_definition(h).relative_virtual_address());
//     body.il_bytes();                      // the method's IL
#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "dotnet/metadata/metadata_reader.hpp"
#include "formats/pe.hpp"

namespace cb::dotnet::metadata {

struct DirectoryEntry {
    std::uint32_t relative_virtual_address = 0, size = 0;
};

// The CLR runtime header (IMAGE_COR20_HEADER).
struct CorHeader {
    std::uint16_t major_runtime_version = 0, minor_runtime_version = 0;
    DirectoryEntry metadata_directory;
    std::uint32_t flags = 0;   // CorFlags: 1 IL only, 2 32-bit required, 8 strong-name signed, 0x10 native entry point, 0x20000 32-bit preferred
    std::uint32_t entry_point_token_or_relative_virtual_address = 0;
    DirectoryEntry resources_directory, strong_name_signature_directory, code_manager_table_directory, vtable_fixups_directory,
        export_address_table_jumps_directory, managed_native_header_directory;
};

enum class ExceptionRegionKind : std::uint16_t { Catch = 0, Filter = 1, Finally = 2, Fault = 4 };

struct ExceptionRegion {
    ExceptionRegionKind kind = ExceptionRegionKind::Catch;
    int try_offset = 0, try_length = 0, handler_offset = 0, handler_length = 0;
    EntityHandle catch_type;   // a Catch's exception type, else nil
    int filter_offset = -1;    // a Filter's filter, else -1
};

// A method body: its header, IL and exception regions.
class MethodBodyBlock {
public:
    // Read the body at the start of `reader` (tiny or fat header). BadImageFormatError for a malformed one.
    static MethodBodyBlock create(BlobReader reader);

    int max_stack() const { return max_stack_; }
    bool local_variables_initialized() const { return init_locals_; }
    StandaloneSignatureHandle local_signature() const { return locals_; }
    ByteView il_bytes() const { return il_; }
    BlobReader il_reader() const { return BlobReader(il_); }
    const std::vector<ExceptionRegion>& exception_regions() const { return regions_; }
    int size() const { return size_; }   // the header, the IL and the exception sections

private:
    int max_stack_ = 8;
    bool init_locals_ = false;
    StandaloneSignatureHandle locals_;
    ByteView il_;
    std::vector<ExceptionRegion> regions_;
    int size_ = 0;
};

class PEReader {
public:
    // The image of a file or of bytes. FormatError when it is not a PE image; a PE without a CLR header opens, but has no
    // metadata (has_metadata() is false).
    static PEReader open(const std::filesystem::path& file);
    static PEReader from_bytes(Bytes image, std::string_view name = "image");
    PEReader(PEReader&&) noexcept;
    PEReader& operator=(PEReader&&) noexcept;
    ~PEReader();

    const formats::pe::Image& pe_image() const { return *image_; }
    bool has_metadata() const { return cor_.has_value(); }
    // The CLR header; UsageError when the image has none.
    const CorHeader& cor_header() const;
    // The metadata bytes and their reader (BadImageFormatError for metadata that is not readable; UsageError without any).
    ByteView metadata_block() const;
    const MetadataReader& metadata() const;
    // The bytes from `rva` to the end of its section's data in the file (System.Reflection.Metadata's GetSectionData): empty
    // when no section holds it.
    ByteView get_section_data(std::uint32_t rva) const;
    // The method body at `rva` (MethodDefinition::relative_virtual_address()).
    MethodBodyBlock get_method_body(std::uint32_t rva) const;

private:
    explicit PEReader(std::unique_ptr<formats::pe::Image> image);

    std::unique_ptr<formats::pe::Image> image_;
    std::optional<CorHeader> cor_;
    ByteView metadata_;
    std::unique_ptr<MetadataReader> reader_;
};

}  // namespace cb::dotnet::metadata

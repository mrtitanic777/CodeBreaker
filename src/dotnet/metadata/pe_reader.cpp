// dotnet/metadata/pe_reader.cpp -- the CLR header, the metadata block, section data and method bodies (as
// System.Reflection.Metadata's PEReader and MethodBodyBlock read them).
#include "dotnet/metadata/pe_reader.hpp"

#include <format>

namespace cb::dotnet::metadata {
namespace {

constexpr std::size_t kClrDirectory = 14;
constexpr std::uint8_t kFormatMask = 0x03, kTinyFormat = 0x02, kFatFormat = 0x03, kMoreSects = 0x08, kInitLocals = 0x10;
constexpr std::uint8_t kSectEHTable = 0x01, kSectFatFormat = 0x40;

}  // namespace

MethodBodyBlock MethodBodyBlock::create(BlobReader reader) {
    const int start = reader.offset();
    MethodBodyBlock b;
    const std::uint8_t head = reader.read_byte();
    if ((head & kFormatMask) == kTinyFormat) {
        const int size = head >> 2;
        b.il_ = reader.read_bytes(size);
        b.size_ = 1 + size;
        return b;
    }
    if ((head & kFormatMask) != kFatFormat) throw BadImageFormatError(0, std::format("an invalid method header {:#04x}", head));
    const std::uint8_t head2 = reader.read_byte();
    if ((head2 >> 4) != 3) throw BadImageFormatError(0, std::format("an invalid method header {:#04x} {:#04x}", head, head2));
    b.init_locals_ = (head & kInitLocals) != 0;
    const bool more = (head & kMoreSects) != 0;
    b.max_stack_ = reader.read_uint16();
    const int il_size = reader.read_int32();
    const std::uint32_t locals = reader.read_uint32();
    if (locals != 0) {
        if ((locals & 0xFF000000) != 0x11000000) throw BadImageFormatError(0, std::format("an invalid local signature token {:#010x}", locals));
        b.locals_ = StandaloneSignatureHandle(locals & 0xFFFFFF);
    }
    b.il_ = reader.read_bytes(il_size);
    if (more) {
        reader.set_offset((reader.offset() + 3) & ~3);
        const std::uint8_t seh = reader.read_byte();
        if ((seh & kSectEHTable) != kSectEHTable) throw BadImageFormatError(0, std::format("an invalid exception section header {:#04x}", seh));
        int data_size = reader.read_byte();
        auto region = [](std::uint32_t kind, int try_offset, int try_length, int handler_offset, int handler_length, std::uint32_t token) {
            ExceptionRegion r;
            r.kind = static_cast<ExceptionRegionKind>(kind);
            r.try_offset = try_offset;
            r.try_length = try_length;
            r.handler_offset = handler_offset;
            r.handler_length = handler_length;
            if (r.kind == ExceptionRegionKind::Catch) r.catch_type = EntityHandle(token);
            if (r.kind == ExceptionRegionKind::Filter) r.filter_offset = static_cast<int>(token);
            return r;
        };
        if ((seh & kSectFatFormat) != 0) {
            data_size += reader.read_uint16() << 8;
            for (int i = 0; i < data_size / 24; ++i) {
                const std::uint32_t kind = reader.read_uint32();
                const int to = reader.read_int32(), tl = reader.read_int32(), ho = reader.read_int32(), hl = reader.read_int32();
                b.regions_.push_back(region(kind, to, tl, ho, hl, reader.read_uint32()));
            }
        } else {
            reader.read_uint16();   // reserved
            for (int i = 0; i < data_size / 12; ++i) {
                const std::uint16_t kind = reader.read_uint16();
                const int to = reader.read_uint16(), tl = reader.read_byte(), ho = reader.read_uint16(), hl = reader.read_byte();
                b.regions_.push_back(region(kind, to, tl, ho, hl, reader.read_uint32()));
            }
        }
    }
    b.size_ = reader.offset() - start;
    return b;
}

PEReader::PEReader(std::unique_ptr<formats::pe::Image> image) : image_(std::move(image)) {
    const formats::pe::DataDirectory dir = image_->directory(kClrDirectory);
    if (dir.rva == 0) return;
    const std::optional<ByteView> raw = image_->read_rva(dir.rva, 72);
    if (!raw) throw BadImageFormatError(dir.rva, "the CLR header lies outside the file");
    BlobReader r(*raw, dir.rva);
    CorHeader c;
    r.read_uint32();   // cb
    c.major_runtime_version = r.read_uint16();
    c.minor_runtime_version = r.read_uint16();
    auto directory = [&] { DirectoryEntry d; d.relative_virtual_address = r.read_uint32(); d.size = r.read_uint32(); return d; };
    c.metadata_directory = directory();
    c.flags = r.read_uint32();
    c.entry_point_token_or_relative_virtual_address = r.read_uint32();
    c.resources_directory = directory();
    c.strong_name_signature_directory = directory();
    c.code_manager_table_directory = directory();
    c.vtable_fixups_directory = directory();
    c.export_address_table_jumps_directory = directory();
    c.managed_native_header_directory = directory();
    cor_ = c;
    const std::optional<ByteView> md = image_->read_rva(c.metadata_directory.relative_virtual_address, c.metadata_directory.size);
    if (!md || c.metadata_directory.size == 0) throw BadImageFormatError(c.metadata_directory.relative_virtual_address, "the metadata lies outside the file");
    metadata_ = *md;
}

PEReader::PEReader(PEReader&&) noexcept = default;
PEReader& PEReader::operator=(PEReader&&) noexcept = default;
PEReader::~PEReader() = default;

PEReader PEReader::open(const std::filesystem::path& file) {
    return PEReader(std::make_unique<formats::pe::Image>(formats::pe::Image::open(file)));
}

PEReader PEReader::from_bytes(Bytes image, std::string_view name) {
    return PEReader(std::make_unique<formats::pe::Image>(formats::pe::Image::parse(std::move(image), name)));
}

const CorHeader& PEReader::cor_header() const {
    if (!cor_) throw UsageError("dotnet: the image has no CLR header (it is not a .NET program)");
    return *cor_;
}

ByteView PEReader::metadata_block() const {
    (void)cor_header();
    return metadata_;
}

const MetadataReader& PEReader::metadata() const {
    if (!reader_) const_cast<PEReader*>(this)->reader_ = std::make_unique<MetadataReader>(metadata_block());
    return *reader_;
}

ByteView PEReader::get_section_data(std::uint32_t rva) const {
    for (const formats::pe::Section& s : image_->sections()) {
        if (rva < s.virtual_address || rva >= static_cast<std::uint64_t>(s.virtual_address) + s.virtual_size) continue;
        const std::uint64_t size = std::min(s.virtual_size, s.raw_size);
        const ByteView file = image_->data();
        const std::uint64_t start = std::min<std::uint64_t>(s.raw_ptr, file.size());
        const std::uint64_t end = std::min<std::uint64_t>(start + size, file.size());
        const std::uint64_t at = start + (rva - s.virtual_address);
        if (at > end) return {};
        return file.subspan(static_cast<std::size_t>(at), static_cast<std::size_t>(end - at));
    }
    return {};
}

MethodBodyBlock PEReader::get_method_body(std::uint32_t rva) const {
    const ByteView data = get_section_data(rva);
    if (data.empty()) throw BadImageFormatError(rva, "a method body outside every section");
    return MethodBodyBlock::create(BlobReader(data, rva));
}

}  // namespace cb::dotnet::metadata

// dotnet/metadata/metadata_reader.cpp -- the metadata root, the streams, the table layout and every row view (ECMA-335 II.24;
// the same reading, the same answers for damaged and unusual metadata, as System.Reflection.Metadata's MetadataReader).
#include "dotnet/metadata/metadata_reader.hpp"

#include <algorithm>
#include <format>

namespace cb::dotnet::metadata {
namespace {

constexpr std::uint32_t kMetadataSignature = 0x424A5342;   // "BSJB"
constexpr std::uint64_t kValidTables = 0x00FF1FFFFFFFFFFFull;   // 0x00 .. 0x2C and 0x30 .. 0x37
constexpr std::uint8_t kHeapStrings4 = 0x01, kHeapGuid4 = 0x02, kHeapBlob4 = 0x04, kHeapExtraData = 0x40;

using TI = TableIndex;
constexpr TI kListPtr[5] = {TI::FieldPtr, TI::MethodPtr, TI::ParamPtr, TI::EventPtr, TI::PropertyPtr};

int ptr_slot(TableIndex ptr) {
    for (int i = 0; i < 5; ++i)
        if (kListPtr[i] == ptr) return i;
    return -1;
}

std::uint32_t le(ByteView b, std::size_t at, int size) {
    std::uint32_t v = 0;
    for (int i = size - 1; i >= 0; --i) v = v << 8 | b[at + static_cast<std::size_t>(i)];
    return v;
}

std::vector<CustomAttributeHandle> to_attributes(const std::vector<std::uint32_t>& rows) {
    std::vector<CustomAttributeHandle> out;
    out.reserve(rows.size());
    for (const std::uint32_t r : rows) out.emplace_back(r);
    return out;
}

template <class H>
std::vector<H> to_handles(const std::vector<std::uint32_t>& rows) {
    std::vector<H> out;
    out.reserve(rows.size());
    for (const std::uint32_t r : rows) out.emplace_back(r);
    return out;
}

}  // namespace

std::string Guid::to_string() const {
    const auto& b = bytes;
    return std::format("{:02x}{:02x}{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}", b[3], b[2], b[1],
                       b[0], b[5], b[4], b[7], b[6], b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15]);
}

std::string Version::to_string() const { return std::format("{}.{}.{}.{}", major, minor, build, revision); }

// ---- the root and the streams ---------------------------------------------------------------------------------------------

MetadataReader::MetadataReader(ByteView metadata) : md_(metadata) {
    auto need = [&](std::size_t at, std::size_t n, const char* what) {
        if (at > md_.size() || n > md_.size() - at) throw BadImageFormatError(at, std::string("the metadata ends inside its ") + what);
    };
    need(0, 16, "header");
    if (le(md_, 0, 4) != kMetadataSignature) throw BadImageFormatError(0, "not metadata (no BSJB signature)");
    const std::uint32_t version_length = le(md_, 12, 4);
    need(16, version_length, "version string");
    {
        const auto* p = reinterpret_cast<const char*>(md_.data() + 16);
        std::size_t n = 0;
        while (n < version_length && p[n] != '\0') ++n;
        version_.assign(p, n);
    }
    std::size_t at = 16 + version_length;
    need(at, 4, "header");
    const std::uint16_t streams = static_cast<std::uint16_t>(le(md_, at + 2, 2));
    at += 4;
    std::optional<std::pair<std::uint32_t, std::uint32_t>> tables_stream;
    for (int i = 0; i < streams; ++i) {
        need(at, 8, "stream headers");
        const std::uint32_t offset = le(md_, at, 4), size = le(md_, at + 4, 4);
        at += 8;
        std::string name;
        while (true) {
            need(at, 1, "stream headers");
            const char c = static_cast<char>(md_[at++]);
            if (c == '\0') break;
            name.push_back(c);
            if (name.size() > 32) throw BadImageFormatError(at, "a stream name longer than 32 characters");
        }
        at = (at + 3) & ~std::size_t{3};
        if (offset > md_.size() || size > md_.size() - offset) throw BadImageFormatError(offset, "a stream outside the metadata");
        const Heap h{offset, size};
        if (name == "#Strings") {
            // as upstream: the heap's alignment padding (all its trailing zeros but one) is not part of it
            Heap trimmed = h;
            while (trimmed.size >= 2 && md_[trimmed.offset + trimmed.size - 1] == 0 && md_[trimmed.offset + trimmed.size - 2] == 0) --trimmed.size;
            heaps_[static_cast<std::size_t>(HeapIndex::String)] = trimmed;
        }
        else if (name == "#US") heaps_[static_cast<std::size_t>(HeapIndex::UserString)] = h;
        else if (name == "#Blob") heaps_[static_cast<std::size_t>(HeapIndex::Blob)] = h;
        else if (name == "#GUID") heaps_[static_cast<std::size_t>(HeapIndex::Guid)] = h;
        else if (name == "#~") tables_stream = {offset, size};
        else if (name == "#-") { tables_stream = {offset, size}; uncompressed_ = true; }
        else if (name == "#JTD") minimal_delta_ = true;
    }
    if (version_.starts_with("WindowsRuntime")) kind_ = version_.find("CLR") != std::string::npos ? MetadataKind::ManagedWindowsMetadata : MetadataKind::WindowsMetadata;
    if (!tables_stream) throw BadImageFormatError(0, "the metadata has no tables stream (#~ or #-)");
    read_tables_stream(tables_stream->first, tables_stream->second);
}

void MetadataReader::read_tables_stream(std::uint32_t offset, std::uint32_t size) {
    const ByteView s = md_.subspan(offset, size);
    if (s.size() < 24) throw BadImageFormatError(offset, "the tables stream ends inside its header");
    heap_sizes_ = s[6];
    valid_ = static_cast<std::uint64_t>(le(s, 8, 4)) | static_cast<std::uint64_t>(le(s, 12, 4)) << 32;
    sorted_ = static_cast<std::uint64_t>(le(s, 16, 4)) | static_cast<std::uint64_t>(le(s, 20, 4)) << 32;
    if ((valid_ & ~kValidTables) != 0) throw BadImageFormatError(offset + 8, "the tables stream has tables this reader does not know");
    std::size_t at = 24;
    for (int t = 0; t < kTableCount; ++t) {
        if ((valid_ >> t & 1) == 0) continue;
        if (s.size() - at < 4) throw BadImageFormatError(offset + at, "the tables stream ends inside its row counts");
        tables_[static_cast<std::size_t>(t)].rows = le(s, at, 4);
        if (tables_[static_cast<std::size_t>(t)].rows > 0xFFFFFF) throw BadImageFormatError(offset + at, "a table with more than 2^24 rows");
        at += 4;
    }
    if ((heap_sizes_ & kHeapExtraData) != 0) at += 4;
    std::uint32_t table_offset = offset + static_cast<std::uint32_t>(at);
    for (int t = 0; t < kTableCount; ++t) {
        Table& tab = tables_[static_cast<std::size_t>(t)];
        const TableSchema& sch = schema(static_cast<TableIndex>(t));
        int col_at = 0;
        for (std::size_t c = 0; c < sch.columns.size(); ++c) {
            const Column& col = sch.columns[c];
            int width = 0;
            switch (col.kind) {
            case ColumnKind::U8x2:
            case ColumnKind::U16: width = 2; break;
            case ColumnKind::U32: width = 4; break;
            case ColumnKind::String: width = minimal_delta_ || (heap_sizes_ & kHeapStrings4) ? 4 : 2; break;
            case ColumnKind::Guid: width = minimal_delta_ || (heap_sizes_ & kHeapGuid4) ? 4 : 2; break;
            case ColumnKind::Blob: width = minimal_delta_ || (heap_sizes_ & kHeapBlob4) ? 4 : 2; break;
            case ColumnKind::Table: width = index_size(static_cast<TableIndex>(col.target)); break;
            case ColumnKind::Coded: width = coded_size(static_cast<CodedIndex>(col.target)); break;
            }
            tab.column_offset[c] = static_cast<std::uint8_t>(col_at);
            tab.column_size[c] = static_cast<std::uint8_t>(width);
            col_at += width;
        }
        tab.row_size = col_at;
        if (tab.rows == 0) continue;
        tab.offset = table_offset;
        const std::uint64_t bytes = static_cast<std::uint64_t>(tab.rows) * static_cast<std::uint64_t>(tab.row_size);
        if (table_offset + bytes > static_cast<std::uint64_t>(offset) + size)
            throw BadImageFormatError(table_offset, std::format("table {} runs past the end of the tables stream", sch.name));
        table_offset += static_cast<std::uint32_t>(bytes);
    }
}

int MetadataReader::index_size(TableIndex t) const {
    if (minimal_delta_) return 4;
    std::uint32_t rows = row_count(t);
    // a list column may hold an index into the Ptr table instead: as wide as the wider of the two
    switch (t) {
    case TI::Field: rows = std::max(rows, row_count(TI::FieldPtr)); break;
    case TI::MethodDef: rows = std::max(rows, row_count(TI::MethodPtr)); break;
    case TI::Param: rows = std::max(rows, row_count(TI::ParamPtr)); break;
    case TI::Event: rows = std::max(rows, row_count(TI::EventPtr)); break;
    case TI::Property: rows = std::max(rows, row_count(TI::PropertyPtr)); break;
    default: break;
    }
    return rows > 0xFFFF ? 4 : 2;
}

int MetadataReader::coded_size(CodedIndex kind) const {
    if (minimal_delta_) return 4;
    const std::uint32_t limit = 1u << (16 - coded_index_tag_bits(kind));
    for (const std::uint8_t t : coded_index_tables(kind))
        if (t != kNoTable && row_count(static_cast<TableIndex>(t)) >= limit) return 4;
    return 2;
}

std::uint32_t MetadataReader::read_at(std::uint64_t offset, int size) const {
    if (offset + static_cast<std::uint64_t>(size) > md_.size()) throw BadImageFormatError(offset, "a read past the end of the metadata");
    return le(md_, static_cast<std::size_t>(offset), size);
}

std::uint32_t MetadataReader::cell(TableIndex t, std::uint32_t row, int column) const {
    const Table& tab = table(t);
    if (row == 0 || row > tab.rows)
        throw BadImageFormatError(tab.offset, std::format("row {} of table {} (it has {})", row, schema(t).name, tab.rows));
    const std::uint64_t at = tab.offset + static_cast<std::uint64_t>(row - 1) * static_cast<std::uint64_t>(tab.row_size) +
                             tab.column_offset[static_cast<std::size_t>(column)];
    const int size = tab.column_size[static_cast<std::size_t>(column)];
    if (schema(t).columns[static_cast<std::size_t>(column)].kind == ColumnKind::U8x2) return read_at(at, 1);
    return read_at(at, size);
}

EntityHandle MetadataReader::coded_cell(TableIndex t, std::uint32_t row, int column) const {
    const std::uint32_t v = cell(t, row, column);
    const auto kind = static_cast<CodedIndex>(schema(t).columns[static_cast<std::size_t>(column)].target);
    const int bits = coded_index_tag_bits(kind);
    const std::uint32_t tag = v & ((1u << bits) - 1);
    const auto tables = coded_index_tables(kind);
    if (tag >= tables.size() || tables[tag] == kNoTable) return EntityHandle();
    return EntityHandle(static_cast<HandleKind>(tables[tag]), v >> bits);
}

std::optional<std::uint32_t> MetadataReader::encode_coded(CodedIndex kind, EntityHandle h) const {
    const auto tables = coded_index_tables(kind);
    for (std::size_t tag = 0; tag < tables.size(); ++tag)
        if (tables[tag] == static_cast<std::uint8_t>(h.kind())) return h.row() << coded_index_tag_bits(kind) | static_cast<std::uint32_t>(tag);
    return std::nullopt;
}

std::vector<std::uint32_t> MetadataReader::rows_where(TableIndex t, int column, std::uint32_t value) const {
    std::vector<std::uint32_t> out;
    const std::uint32_t n = row_count(t);
    if (n == 0) return out;
    // binary search when the column is in order (the tables the standard requires sorted are, in practice; checked once, as a
    // damaged or hand-made assembly may not be)
    if (!column_in_order(t, column)) {
        for (std::uint32_t r = 1; r <= n; ++r)
            if (cell(t, r, column) == value) out.push_back(r);
        return out;
    }
    std::uint32_t lo = 1, hi = n + 1;   // the first row whose cell is >= value
    while (lo < hi) {
        const std::uint32_t mid = lo + (hi - lo) / 2;
        if (cell(t, mid, column) < value) lo = mid + 1;
        else hi = mid;
    }
    for (std::uint32_t r = lo; r <= n && cell(t, r, column) == value; ++r) out.push_back(r);
    return out;
}

bool MetadataReader::column_in_order(TableIndex t, int column) const {
    const std::lock_guard lock(order_mutex_);
    std::int8_t& known = order_[static_cast<std::size_t>(t)][static_cast<std::size_t>(column)];
    if (known == 0) {
        bool ordered = true;
        for (std::uint32_t r = 2; r <= row_count(t) && ordered; ++r) ordered = cell(t, r - 1, column) <= cell(t, r, column);
        known = ordered ? 1 : 2;
    }
    return known == 1;
}

std::uint32_t MetadataReader::first_row_where(TableIndex t, int column, std::uint32_t value) const {
    const std::vector<std::uint32_t> rows = rows_where(t, column, value);
    return rows.empty() ? 0 : rows.front();
}

std::uint32_t MetadataReader::member_at(TableIndex ptr, bool indirect, std::uint32_t first, std::uint32_t i) const {
    const std::uint32_t row = first + i;
    return indirect ? cell(ptr, row, 0) : row;
}

std::uint32_t MetadataReader::list_owner(TableIndex parent, int column, TableIndex member, TableIndex ptr, std::uint32_t member_row) const {
    std::uint32_t position = member_row;   // the member's place in the list order
    if (row_count(ptr) > 0) {
        const int slot = ptr_slot(ptr);
        std::call_once(ptr_once_[slot], [&] {
            auto& inv = ptr_inverse_[slot];
            inv.assign(static_cast<std::size_t>(row_count(member)) + 1, 0);
            for (std::uint32_t p = 1; p <= row_count(ptr); ++p) {
                const std::uint32_t m = cell(ptr, p, 0);
                if (m < inv.size() && inv[m] == 0) inv[m] = p;
            }
        });
        if (member_row >= ptr_inverse_[slot].size() || ptr_inverse_[slot][member_row] == 0) return 0;
        position = ptr_inverse_[slot][member_row];
    }
    const std::uint32_t n = row_count(parent);
    std::uint32_t lo = 1, hi = n + 1;   // the first parent whose list starts after `position`
    while (lo < hi) {
        const std::uint32_t mid = lo + (hi - lo) / 2;
        if (cell(parent, mid, column) <= position) lo = mid + 1;
        else hi = mid;
    }
    const std::uint32_t owner = lo - 1;
    if (owner == 0) return 0;
    const std::uint32_t total = row_count(ptr) > 0 ? row_count(ptr) : row_count(member);
    const std::uint32_t end = owner < n ? cell(parent, owner + 1, column) : total + 1;
    return position < end ? owner : 0;
}

const std::vector<TypeDefinitionHandle>& MetadataReader::nested_types_of(TypeDefinitionHandle enclosing) const {
    std::call_once(nested_once_, [&] {
        for (std::uint32_t r = 1; r <= row_count(TI::NestedClass); ++r)
            nested_[cell(TI::NestedClass, r, 1)].emplace_back(cell(TI::NestedClass, r, 0));
    });
    static const std::vector<TypeDefinitionHandle> kNone;
    const auto it = nested_.find(enclosing.row());
    return it == nested_.end() ? kNone : it->second;
}

std::vector<CustomAttributeHandle> MetadataReader::get_custom_attributes(EntityHandle parent) const {
    const auto v = encode_coded(CodedIndex::HasCustomAttribute, parent);
    if (!v || parent.is_nil()) return {};
    return to_attributes(rows_where(TI::CustomAttribute, 0, *v));
}

// ---- the heaps ------------------------------------------------------------------------------------------------------------

ByteView MetadataReader::heap(HeapIndex h) const {
    const Heap& hp = heaps_[static_cast<std::size_t>(h)];
    return md_.subspan(hp.offset, hp.size);
}

std::string_view MetadataReader::get_string(StringHandle h) const {
    const ByteView s = heap(HeapIndex::String);
    if (h.offset() >= s.size()) {
        if (h.is_nil()) return {};
        throw BadImageFormatError(heaps_[static_cast<std::size_t>(HeapIndex::String)].offset + h.offset(), "a string outside #Strings");
    }
    const auto* p = reinterpret_cast<const char*>(s.data()) + h.offset();
    const std::size_t max = s.size() - h.offset();
    std::size_t n = 0;
    while (n < max && p[n] != '\0') ++n;
    return {p, n};
}

bool MetadataReader::heap_value(HeapIndex h, std::uint32_t offset, std::uint32_t& value_offset, std::uint32_t& value_size) const {
    const ByteView b = heap(h);
    if (offset >= b.size()) return false;
    BlobReader r(b.subspan(offset));
    const auto size = r.try_read_compressed_integer();
    if (!size) return false;
    value_offset = offset + static_cast<std::uint32_t>(r.offset());
    value_size = static_cast<std::uint32_t>(*size);
    return true;
}

std::u16string MetadataReader::get_user_string(UserStringHandle h) const {
    std::uint32_t at = 0, size = 0;
    if (!heap_value(HeapIndex::UserString, h.offset(), at, size)) return {};
    const ByteView b = heap(HeapIndex::UserString);
    if (static_cast<std::uint64_t>(at) + size > b.size()) throw BadImageFormatError(at, "a user string past the end of #US");
    return BlobReader(b.subspan(at, size)).read_utf16(static_cast<int>(size & ~1u));
}

ByteView MetadataReader::get_blob_bytes(BlobHandle h) const {
    std::uint32_t at = 0, size = 0;
    if (h.is_nil() || !heap_value(HeapIndex::Blob, h.offset(), at, size)) {
        if (h.is_nil()) return {};
        throw BadImageFormatError(heaps_[static_cast<std::size_t>(HeapIndex::Blob)].offset + h.offset(), "a blob outside #Blob");
    }
    const ByteView b = heap(HeapIndex::Blob);
    if (static_cast<std::uint64_t>(at) + size > b.size()) throw BadImageFormatError(at, "a blob past the end of #Blob");
    return b.subspan(at, size);
}

BlobReader MetadataReader::get_blob_reader(BlobHandle h) const {
    const ByteView b = get_blob_bytes(h);
    const std::uint64_t base = b.empty() ? 0 : static_cast<std::uint64_t>(b.data() - md_.data());
    return BlobReader(b, base);
}

Guid MetadataReader::get_guid(GuidHandle h) const {
    Guid g;
    if (h.is_nil()) return g;
    const ByteView b = heap(HeapIndex::Guid);
    const std::uint64_t at = static_cast<std::uint64_t>(h.offset() - 1) * 16;
    if (at + 16 > b.size()) throw BadImageFormatError(at, "a GUID outside #GUID");
    std::copy_n(b.begin() + static_cast<std::ptrdiff_t>(at), 16, g.bytes.begin());
    return g;
}

StringHandle MetadataReader::get_next_handle(StringHandle h) const {
    const ByteView s = heap(HeapIndex::String);
    for (std::size_t i = h.offset(); i < s.size(); ++i)
        if (s[i] == 0) return i + 1 >= s.size() ? StringHandle() : StringHandle(static_cast<std::uint32_t>(i + 1));
    return StringHandle();
}

UserStringHandle MetadataReader::get_next_handle(UserStringHandle h) const {
    std::uint32_t at = 0, size = 0;
    if (!heap_value(HeapIndex::UserString, h.offset(), at, size)) return UserStringHandle();
    const std::uint64_t next = static_cast<std::uint64_t>(at) + size;
    return next >= heap(HeapIndex::UserString).size() ? UserStringHandle() : UserStringHandle(static_cast<std::uint32_t>(next));
}

BlobHandle MetadataReader::get_next_handle(BlobHandle h) const {
    std::uint32_t at = 0, size = 0;
    if (!heap_value(HeapIndex::Blob, h.offset(), at, size)) return BlobHandle();
    const std::uint64_t next = static_cast<std::uint64_t>(at) + size;
    return next >= heap(HeapIndex::Blob).size() ? BlobHandle() : BlobHandle(static_cast<std::uint32_t>(next));
}

AssemblyDefinition MetadataReader::get_assembly_definition() const {
    if (!is_assembly()) throw UsageError("dotnet metadata: the module is not an assembly (it has no Assembly row)");
    return AssemblyDefinition(this);
}

// ---- the views ------------------------------------------------------------------------------------------------------------

namespace {
std::vector<CustomAttributeHandle> attributes_of(const MetadataReader* md, EntityHandle h) { return md->get_custom_attributes(h); }
std::vector<DeclarativeSecurityAttributeHandle> security_of(const MetadataReader* md, EntityHandle h) {
    const auto v = md->encode_coded(CodedIndex::HasDeclSecurity, h);
    if (!v) return {};
    return to_handles<DeclarativeSecurityAttributeHandle>(md->rows_where(TI::DeclSecurity, 1, *v));
}
std::vector<GenericParameterHandle> generic_parameters_of(const MetadataReader* md, EntityHandle h) {
    const auto v = md->encode_coded(CodedIndex::TypeOrMethodDef, h);
    if (!v) return {};
    return to_handles<GenericParameterHandle>(md->rows_where(TI::GenericParam, 2, *v));
}
ConstantHandle constant_of(const MetadataReader* md, EntityHandle h) {
    const auto v = md->encode_coded(CodedIndex::HasConstant, h);
    return v ? ConstantHandle(md->first_row_where(TI::Constant, 1, *v)) : ConstantHandle();
}
BlobHandle marshal_of(const MetadataReader* md, EntityHandle h) {
    const auto v = md->encode_coded(CodedIndex::HasFieldMarshal, h);
    if (!v) return {};
    const std::uint32_t row = md->first_row_where(TI::FieldMarshal, 0, *v);
    return row ? BlobHandle(md->cell(TI::FieldMarshal, row, 1)) : BlobHandle();
}
struct Semantics {
    std::vector<std::pair<std::uint16_t, MethodDefinitionHandle>> rows;
};
Semantics semantics_of(const MetadataReader* md, EntityHandle h) {
    Semantics s;
    const auto v = md->encode_coded(CodedIndex::HasSemantics, h);
    if (!v) return s;
    for (const std::uint32_t r : md->rows_where(TI::MethodSemantics, 2, *v))
        s.rows.emplace_back(static_cast<std::uint16_t>(md->cell(TI::MethodSemantics, r, 0)), MethodDefinitionHandle(md->cell(TI::MethodSemantics, r, 1)));
    return s;
}
}  // namespace

int ModuleDefinition::generation() const { return static_cast<int>(md_->cell(TI::Module, 1, 0)); }
StringHandle ModuleDefinition::name() const { return StringHandle(md_->cell(TI::Module, 1, 1)); }
GuidHandle ModuleDefinition::mvid() const { return GuidHandle(md_->cell(TI::Module, 1, 2)); }
GuidHandle ModuleDefinition::generation_id() const { return GuidHandle(md_->cell(TI::Module, 1, 3)); }
GuidHandle ModuleDefinition::base_generation_id() const { return GuidHandle(md_->cell(TI::Module, 1, 4)); }
std::vector<CustomAttributeHandle> ModuleDefinition::get_custom_attributes() const { return attributes_of(md_, ModuleDefinitionHandle(1)); }

EntityHandle TypeReference::resolution_scope() const { return md_->coded_cell(TI::TypeRef, h_.row(), 0); }
StringHandle TypeReference::name() const { return StringHandle(md_->cell(TI::TypeRef, h_.row(), 1)); }
StringHandle TypeReference::namespace_() const { return StringHandle(md_->cell(TI::TypeRef, h_.row(), 2)); }

std::uint32_t TypeDefinition::attributes() const { return md_->cell(TI::TypeDef, h_.row(), 0); }
StringHandle TypeDefinition::name() const { return StringHandle(md_->cell(TI::TypeDef, h_.row(), 1)); }
StringHandle TypeDefinition::namespace_() const { return StringHandle(md_->cell(TI::TypeDef, h_.row(), 2)); }
EntityHandle TypeDefinition::base_type() const { return md_->coded_cell(TI::TypeDef, h_.row(), 3); }
TypeDefinitionHandle TypeDefinition::get_declaring_type() const {
    const std::uint32_t r = md_->first_row_where(TI::NestedClass, 0, h_.row());
    return r ? TypeDefinitionHandle(md_->cell(TI::NestedClass, r, 1)) : TypeDefinitionHandle();
}
TypeLayout TypeDefinition::get_layout() const {
    const std::uint32_t r = md_->first_row_where(TI::ClassLayout, 2, h_.row());
    if (!r) return {};
    return {static_cast<std::int32_t>(md_->cell(TI::ClassLayout, r, 1)), static_cast<std::int32_t>(md_->cell(TI::ClassLayout, r, 0))};
}
HandleList<FieldDefinitionHandle> TypeDefinition::get_fields() const {
    return md_->member_list<FieldDefinitionHandle>(TI::TypeDef, h_.row(), 4, TI::Field, TI::FieldPtr);
}
HandleList<MethodDefinitionHandle> TypeDefinition::get_methods() const {
    return md_->member_list<MethodDefinitionHandle>(TI::TypeDef, h_.row(), 5, TI::MethodDef, TI::MethodPtr);
}
HandleList<PropertyDefinitionHandle> TypeDefinition::get_properties() const {
    const std::uint32_t r = md_->first_row_where(TI::PropertyMap, 0, h_.row());
    if (!r) return {};
    return md_->member_list<PropertyDefinitionHandle>(TI::PropertyMap, r, 1, TI::Property, TI::PropertyPtr);
}
HandleList<EventDefinitionHandle> TypeDefinition::get_events() const {
    const std::uint32_t r = md_->first_row_where(TI::EventMap, 0, h_.row());
    if (!r) return {};
    return md_->member_list<EventDefinitionHandle>(TI::EventMap, r, 1, TI::Event, TI::EventPtr);
}
std::vector<TypeDefinitionHandle> TypeDefinition::get_nested_types() const { return md_->nested_types_of(h_); }
std::vector<InterfaceImplementationHandle> TypeDefinition::get_interface_implementations() const {
    return to_handles<InterfaceImplementationHandle>(md_->rows_where(TI::InterfaceImpl, 0, h_.row()));
}
std::vector<GenericParameterHandle> TypeDefinition::get_generic_parameters() const { return generic_parameters_of(md_, h_); }
std::vector<MethodImplementationHandle> TypeDefinition::get_method_implementations() const {
    return to_handles<MethodImplementationHandle>(md_->rows_where(TI::MethodImpl, 0, h_.row()));
}
std::vector<DeclarativeSecurityAttributeHandle> TypeDefinition::get_declarative_security_attributes() const { return security_of(md_, h_); }
std::vector<CustomAttributeHandle> TypeDefinition::get_custom_attributes() const { return attributes_of(md_, h_); }

std::uint16_t FieldDefinition::attributes() const { return static_cast<std::uint16_t>(md_->cell(TI::Field, h_.row(), 0)); }
StringHandle FieldDefinition::name() const { return StringHandle(md_->cell(TI::Field, h_.row(), 1)); }
BlobHandle FieldDefinition::signature() const { return BlobHandle(md_->cell(TI::Field, h_.row(), 2)); }
TypeDefinitionHandle FieldDefinition::get_declaring_type() const {
    return TypeDefinitionHandle(md_->list_owner(TI::TypeDef, 4, TI::Field, TI::FieldPtr, h_.row()));
}
ConstantHandle FieldDefinition::get_default_value() const { return constant_of(md_, h_); }
int FieldDefinition::get_offset() const {
    const std::uint32_t r = md_->first_row_where(TI::FieldLayout, 1, h_.row());
    return r ? static_cast<int>(md_->cell(TI::FieldLayout, r, 0)) : -1;
}
int FieldDefinition::get_relative_virtual_address() const {
    const std::uint32_t r = md_->first_row_where(TI::FieldRva, 1, h_.row());
    return r ? static_cast<int>(md_->cell(TI::FieldRva, r, 0)) : 0;
}
BlobHandle FieldDefinition::get_marshalling_descriptor() const { return marshal_of(md_, h_); }
std::vector<CustomAttributeHandle> FieldDefinition::get_custom_attributes() const { return attributes_of(md_, h_); }

int MethodDefinition::relative_virtual_address() const { return static_cast<int>(md_->cell(TI::MethodDef, h_.row(), 0)); }
std::uint16_t MethodDefinition::impl_attributes() const { return static_cast<std::uint16_t>(md_->cell(TI::MethodDef, h_.row(), 1)); }
std::uint16_t MethodDefinition::attributes() const { return static_cast<std::uint16_t>(md_->cell(TI::MethodDef, h_.row(), 2)); }
StringHandle MethodDefinition::name() const { return StringHandle(md_->cell(TI::MethodDef, h_.row(), 3)); }
BlobHandle MethodDefinition::signature() const { return BlobHandle(md_->cell(TI::MethodDef, h_.row(), 4)); }
HandleList<ParameterHandle> MethodDefinition::get_parameters() const {
    return md_->member_list<ParameterHandle>(TI::MethodDef, h_.row(), 5, TI::Param, TI::ParamPtr);
}
std::vector<GenericParameterHandle> MethodDefinition::get_generic_parameters() const { return generic_parameters_of(md_, h_); }
TypeDefinitionHandle MethodDefinition::get_declaring_type() const {
    return TypeDefinitionHandle(md_->list_owner(TI::TypeDef, 5, TI::MethodDef, TI::MethodPtr, h_.row()));
}
MethodImport MethodDefinition::get_import() const {
    const auto v = md_->encode_coded(CodedIndex::MemberForwarded, h_);
    const std::uint32_t r = v ? md_->first_row_where(TI::ImplMap, 1, *v) : 0;
    if (!r) return {};
    return {static_cast<std::uint16_t>(md_->cell(TI::ImplMap, r, 0)), StringHandle(md_->cell(TI::ImplMap, r, 2)),
            ModuleReferenceHandle(md_->cell(TI::ImplMap, r, 3))};
}
std::vector<DeclarativeSecurityAttributeHandle> MethodDefinition::get_declarative_security_attributes() const { return security_of(md_, h_); }
std::vector<CustomAttributeHandle> MethodDefinition::get_custom_attributes() const { return attributes_of(md_, h_); }

std::uint16_t Parameter::attributes() const { return static_cast<std::uint16_t>(md_->cell(TI::Param, h_.row(), 0)); }
int Parameter::sequence_number() const { return static_cast<int>(md_->cell(TI::Param, h_.row(), 1)); }
StringHandle Parameter::name() const { return StringHandle(md_->cell(TI::Param, h_.row(), 2)); }
ConstantHandle Parameter::get_default_value() const { return constant_of(md_, h_); }
BlobHandle Parameter::get_marshalling_descriptor() const { return marshal_of(md_, h_); }
std::vector<CustomAttributeHandle> Parameter::get_custom_attributes() const { return attributes_of(md_, h_); }

EntityHandle InterfaceImplementation::interface_() const { return md_->coded_cell(TI::InterfaceImpl, h_.row(), 1); }
std::vector<CustomAttributeHandle> InterfaceImplementation::get_custom_attributes() const { return attributes_of(md_, h_); }

EntityHandle MemberReference::parent() const { return md_->coded_cell(TI::MemberRef, h_.row(), 0); }
StringHandle MemberReference::name() const { return StringHandle(md_->cell(TI::MemberRef, h_.row(), 1)); }
BlobHandle MemberReference::signature() const { return BlobHandle(md_->cell(TI::MemberRef, h_.row(), 2)); }
MemberReferenceKind MemberReference::get_kind() const {
    BlobReader r = md_->get_blob_reader(signature());
    return r.read_signature_header().kind() == SignatureKind::Field ? MemberReferenceKind::Field : MemberReferenceKind::Method;
}
std::vector<CustomAttributeHandle> MemberReference::get_custom_attributes() const { return attributes_of(md_, h_); }

ConstantTypeCode Constant::type_code() const { return static_cast<ConstantTypeCode>(md_->cell(TI::Constant, h_.row(), 0)); }
EntityHandle Constant::parent() const { return md_->coded_cell(TI::Constant, h_.row(), 1); }
BlobHandle Constant::value() const { return BlobHandle(md_->cell(TI::Constant, h_.row(), 2)); }

EntityHandle CustomAttribute::parent() const { return md_->coded_cell(TI::CustomAttribute, h_.row(), 0); }
EntityHandle CustomAttribute::constructor() const { return md_->coded_cell(TI::CustomAttribute, h_.row(), 1); }
BlobHandle CustomAttribute::value() const { return BlobHandle(md_->cell(TI::CustomAttribute, h_.row(), 2)); }

std::uint16_t DeclarativeSecurityAttribute::action() const { return static_cast<std::uint16_t>(md_->cell(TI::DeclSecurity, h_.row(), 0)); }
EntityHandle DeclarativeSecurityAttribute::parent() const { return md_->coded_cell(TI::DeclSecurity, h_.row(), 1); }
BlobHandle DeclarativeSecurityAttribute::permission_set() const { return BlobHandle(md_->cell(TI::DeclSecurity, h_.row(), 2)); }

BlobHandle StandaloneSignature::signature() const { return BlobHandle(md_->cell(TI::StandAloneSig, h_.row(), 0)); }
StandaloneSignatureKind StandaloneSignature::get_kind() const {
    BlobReader r = md_->get_blob_reader(signature());
    const SignatureHeader h = r.read_signature_header();
    switch (h.kind()) {
    case SignatureKind::Method: return StandaloneSignatureKind::Method;
    case SignatureKind::LocalVariables: return StandaloneSignatureKind::LocalVariables;
    default: throw BadImageFormatError(0, std::format("a standalone signature of kind {:#x}", static_cast<int>(h.kind())));
    }
}
std::vector<CustomAttributeHandle> StandaloneSignature::get_custom_attributes() const { return attributes_of(md_, h_); }

std::uint16_t EventDefinition::attributes() const { return static_cast<std::uint16_t>(md_->cell(TI::Event, h_.row(), 0)); }
StringHandle EventDefinition::name() const { return StringHandle(md_->cell(TI::Event, h_.row(), 1)); }
EntityHandle EventDefinition::type() const { return md_->coded_cell(TI::Event, h_.row(), 2); }
EventAccessors EventDefinition::get_accessors() const {
    EventAccessors a;
    for (const auto& [sem, m] : semantics_of(md_, h_).rows) {
        if (sem == 0x08) a.adder = m;
        else if (sem == 0x10) a.remover = m;
        else if (sem == 0x20) a.raiser = m;
        else if (sem == 0x04) a.others.push_back(m);
    }
    return a;
}
std::vector<CustomAttributeHandle> EventDefinition::get_custom_attributes() const { return attributes_of(md_, h_); }

std::uint16_t PropertyDefinition::attributes() const { return static_cast<std::uint16_t>(md_->cell(TI::Property, h_.row(), 0)); }
StringHandle PropertyDefinition::name() const { return StringHandle(md_->cell(TI::Property, h_.row(), 1)); }
BlobHandle PropertyDefinition::signature() const { return BlobHandle(md_->cell(TI::Property, h_.row(), 2)); }
ConstantHandle PropertyDefinition::get_default_value() const { return constant_of(md_, h_); }
PropertyAccessors PropertyDefinition::get_accessors() const {
    PropertyAccessors a;
    for (const auto& [sem, m] : semantics_of(md_, h_).rows) {
        if (sem == 0x02) a.getter = m;
        else if (sem == 0x01) a.setter = m;
        else if (sem == 0x04) a.others.push_back(m);
    }
    return a;
}
std::vector<CustomAttributeHandle> PropertyDefinition::get_custom_attributes() const { return attributes_of(md_, h_); }

TypeDefinitionHandle MethodImplementation::type() const { return TypeDefinitionHandle(md_->cell(TI::MethodImpl, h_.row(), 0)); }
EntityHandle MethodImplementation::method_body() const { return md_->coded_cell(TI::MethodImpl, h_.row(), 1); }
EntityHandle MethodImplementation::method_declaration() const { return md_->coded_cell(TI::MethodImpl, h_.row(), 2); }

StringHandle ModuleReference::name() const { return StringHandle(md_->cell(TI::ModuleRef, h_.row(), 0)); }

BlobHandle TypeSpecification::signature() const { return BlobHandle(md_->cell(TI::TypeSpec, h_.row(), 0)); }
std::vector<CustomAttributeHandle> TypeSpecification::get_custom_attributes() const { return attributes_of(md_, h_); }

std::uint32_t AssemblyDefinition::hash_algorithm() const { return md_->cell(TI::Assembly, 1, 0); }
Version AssemblyDefinition::version() const {
    return {static_cast<std::uint16_t>(md_->cell(TI::Assembly, 1, 1)), static_cast<std::uint16_t>(md_->cell(TI::Assembly, 1, 2)),
            static_cast<std::uint16_t>(md_->cell(TI::Assembly, 1, 3)), static_cast<std::uint16_t>(md_->cell(TI::Assembly, 1, 4))};
}
std::uint32_t AssemblyDefinition::flags() const { return md_->cell(TI::Assembly, 1, 5); }
BlobHandle AssemblyDefinition::public_key() const { return BlobHandle(md_->cell(TI::Assembly, 1, 6)); }
StringHandle AssemblyDefinition::name() const { return StringHandle(md_->cell(TI::Assembly, 1, 7)); }
StringHandle AssemblyDefinition::culture() const { return StringHandle(md_->cell(TI::Assembly, 1, 8)); }
std::vector<CustomAttributeHandle> AssemblyDefinition::get_custom_attributes() const { return attributes_of(md_, AssemblyDefinitionHandle(1)); }
std::vector<DeclarativeSecurityAttributeHandle> AssemblyDefinition::get_declarative_security_attributes() const {
    return security_of(md_, AssemblyDefinitionHandle(1));
}

Version AssemblyReference::version() const {
    return {static_cast<std::uint16_t>(md_->cell(TI::AssemblyRef, h_.row(), 0)), static_cast<std::uint16_t>(md_->cell(TI::AssemblyRef, h_.row(), 1)),
            static_cast<std::uint16_t>(md_->cell(TI::AssemblyRef, h_.row(), 2)), static_cast<std::uint16_t>(md_->cell(TI::AssemblyRef, h_.row(), 3))};
}
std::uint32_t AssemblyReference::flags() const { return md_->cell(TI::AssemblyRef, h_.row(), 4); }
BlobHandle AssemblyReference::public_key_or_token() const { return BlobHandle(md_->cell(TI::AssemblyRef, h_.row(), 5)); }
StringHandle AssemblyReference::name() const { return StringHandle(md_->cell(TI::AssemblyRef, h_.row(), 6)); }
StringHandle AssemblyReference::culture() const { return StringHandle(md_->cell(TI::AssemblyRef, h_.row(), 7)); }
BlobHandle AssemblyReference::hash_value() const { return BlobHandle(md_->cell(TI::AssemblyRef, h_.row(), 8)); }
std::vector<CustomAttributeHandle> AssemblyReference::get_custom_attributes() const { return attributes_of(md_, h_); }

bool AssemblyFile::contains_metadata() const { return (md_->cell(TI::File, h_.row(), 0) & 1) == 0; }
StringHandle AssemblyFile::name() const { return StringHandle(md_->cell(TI::File, h_.row(), 1)); }
BlobHandle AssemblyFile::hash_value() const { return BlobHandle(md_->cell(TI::File, h_.row(), 2)); }

std::uint32_t ExportedType::attributes() const { return md_->cell(TI::ExportedType, h_.row(), 0); }
bool ExportedType::is_forwarder() const {
    return (attributes() & 0x00200000) != 0 && implementation().kind() == HandleKind::AssemblyReference;
}
int ExportedType::get_type_definition_id() const { return static_cast<int>(md_->cell(TI::ExportedType, h_.row(), 1)); }
StringHandle ExportedType::name() const { return StringHandle(md_->cell(TI::ExportedType, h_.row(), 2)); }
StringHandle ExportedType::namespace_() const { return StringHandle(md_->cell(TI::ExportedType, h_.row(), 3)); }
EntityHandle ExportedType::implementation() const { return md_->coded_cell(TI::ExportedType, h_.row(), 4); }

std::int64_t ManifestResource::offset() const { return md_->cell(TI::ManifestResource, h_.row(), 0); }
std::uint32_t ManifestResource::attributes() const { return md_->cell(TI::ManifestResource, h_.row(), 1); }
StringHandle ManifestResource::name() const { return StringHandle(md_->cell(TI::ManifestResource, h_.row(), 2)); }
EntityHandle ManifestResource::implementation() const { return md_->coded_cell(TI::ManifestResource, h_.row(), 3); }

int GenericParameter::index() const { return static_cast<int>(md_->cell(TI::GenericParam, h_.row(), 0)); }
std::uint16_t GenericParameter::attributes() const { return static_cast<std::uint16_t>(md_->cell(TI::GenericParam, h_.row(), 1)); }
EntityHandle GenericParameter::parent() const { return md_->coded_cell(TI::GenericParam, h_.row(), 2); }
StringHandle GenericParameter::name() const { return StringHandle(md_->cell(TI::GenericParam, h_.row(), 3)); }
std::vector<GenericParameterConstraintHandle> GenericParameter::get_constraints() const {
    return to_handles<GenericParameterConstraintHandle>(md_->rows_where(TI::GenericParamConstraint, 0, h_.row()));
}
std::vector<CustomAttributeHandle> GenericParameter::get_custom_attributes() const { return attributes_of(md_, h_); }

GenericParameterHandle GenericParameterConstraint::parameter() const {
    return GenericParameterHandle(md_->cell(TI::GenericParamConstraint, h_.row(), 0));
}
EntityHandle GenericParameterConstraint::type() const { return md_->coded_cell(TI::GenericParamConstraint, h_.row(), 1); }
std::vector<CustomAttributeHandle> GenericParameterConstraint::get_custom_attributes() const { return attributes_of(md_, h_); }

EntityHandle MethodSpecification::method() const { return md_->coded_cell(TI::MethodSpec, h_.row(), 0); }
BlobHandle MethodSpecification::signature() const { return BlobHandle(md_->cell(TI::MethodSpec, h_.row(), 1)); }

}  // namespace cb::dotnet::metadata

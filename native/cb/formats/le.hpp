// cb/formats/le.hpp -- Linear Executables (Windows 95's VxDs, .vxd / .386) and the containers that hold several.
//
// The reader gives the header, the object table and the page map, each object's bytes as loaded, the name and
// entry tables, every kind of fix-up record, the device descriptor block (DDB) with its service table and entry
// points, the service calls, the W3 directory (VMM32.VXD, WIN386.EXE) and the W4 page compression -- and a way to
// take an image apart (`Parts`) and put it back together (`write_parts`), which the driver build uses.
//
// A `Module` shares the file's bytes (the members of a container all point into the one buffer). Readers are
// strict: a damaged table throws FormatError/TruncatedError, except where a function says it is tolerant.
#pragma once

#include <array>
#include <compare>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <span>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::formats::le {

inline constexpr std::size_t kHeaderSize = 0xC4;

// Page kinds as the page map stores them.
inline constexpr std::uint8_t kPagePresent = 0, kPageIterated = 1, kPageInvalid = 2, kPageZeroFilled = 3;

// Module flags of a driver.
inline constexpr std::uint32_t kModuleStatic = 0x00000000, kModuleDynamic = 0x00008000, kModuleOld = 0x00010000;

// A place in the image: object (1-based) and the offset within it. The offset is signed because a fix-up source
// offset can lie before the start of its page.
struct Place {
    std::uint32_t object = 0;
    std::int64_t offset = 0;
    friend auto operator<=>(const Place&, const Place&) = default;
};

struct Object {
    std::uint32_t index = 0;  // 1-based
    std::uint32_t vsize = 0, base = 0, flags = 0, first_page = 0, page_count = 0;
    bool executable() const { return (flags & 0x04) != 0; }
    bool readable() const { return (flags & 0x01) != 0; }
    bool writable() const { return (flags & 0x02) != 0; }
};

struct PageEntry {
    std::uint32_t number = 0;  // the page's number in the file (0: none)
    std::uint8_t kind = kPagePresent;
};

// The header's fields, every one (offsets are as stored: relative to the LE header unless noted).
struct Header {
    std::uint8_t byte_order = 0, word_order = 0;
    std::uint32_t level = 0;
    std::uint16_t cpu = 0, os = 0;
    std::uint32_t version = 0, module_flags = 0, pages = 0, eip_object = 0, eip = 0, esp_object = 0, esp = 0,
                  page_size = 0, last_page_bytes = 0, fixup_size = 0, loader_size = 0, object_table = 0, objects = 0,
                  page_map = 0, resources = 0, resident_names = 0, entry_table = 0, fixup_pages = 0, fixup_records = 0,
                  import_modules = 0, import_module_count = 0, import_names = 0, data_pages = 0 /* from the top of the file */,
                  preload_pages = 0, nonresident_names = 0 /* from the top of the file */, nonresident_size = 0,
                  debug_info = 0, debug_size = 0, resource_data = 0, resource_size = 0;
    std::uint16_t device_id = 0, kit_version = 0;
};

// One patched place's target: another place in this image, or an import by ordinal.
struct InternalFixup {
    std::uint32_t object = 0;
    std::optional<std::uint32_t> target;  // absent: a 16-bit selector fix-up has no offset
    std::uint8_t source_type = 0;
};
struct ImportFixup {
    std::uint32_t module = 0;  // 1-based index into the import-module table
    std::uint32_t ordinal = 0;
};
using Fixup = std::variant<InternalFixup, ImportFixup>;

// One fix-up record as stored.
struct FixupRecord {
    enum class Kind { Internal, Import, ImportName, Entry };
    std::uint32_t page = 0;  // 1-based
    std::uint8_t source = 0, source_flags = 0, flags = 0;
    Kind kind = Kind::Internal;
    std::uint32_t first = 0;                  // the object / module / entry the record names
    std::optional<std::uint32_t> target;      // Internal (absent: a selector)
    std::uint32_t ordinal = 0, name = 0;      // Import, ImportName
    std::optional<std::uint32_t> addend;
    std::vector<std::int16_t> sources;        // offsets within the page, one or several
    Bytes raw;                                // the record's bytes as stored
};

struct Reference {  // an internal fix-up resolved to places
    Place at;
    std::uint8_t source = 0;
    std::uint32_t target_object = 0;
    std::int64_t target_offset = 0;
};

// The device descriptor block located by the version stamp (older drivers).
struct Ddb {
    std::uint32_t object = 0;
    std::int64_t rva = 0;
    std::uint16_t version = 0, device_id = 0;
    std::uint8_t major = 0, minor = 0;
    std::uint16_t flags = 0;
    std::string name;  // trailing blanks and zeros cut
    std::uint32_t init_order = 0, control_proc = 0;
};

struct Export {
    std::optional<std::string> name;
    std::uint32_t ordinal = 0, object = 0;
    std::uint32_t offset = 0;
};
struct EntryPoint {
    std::uint32_t ordinal = 0, object = 0;  // object 0: an absolute (kind 4) entry
    std::uint32_t offset = 0;
    std::uint8_t flags = 0;
};
struct NameOrdinal {
    std::string name;
    std::uint16_t ordinal = 0;
};

// The descriptor block, every field; `at_*` are the places the block's pointer fields point at, found through the
// image's own fix-ups (absent where the field has no fix-up).
struct Descriptor {
    Place where;
    std::string name;
    std::uint32_t next = 0;
    std::uint16_t kit_version = 0, device_id = 0;
    std::uint8_t major = 0, minor = 0;
    std::uint16_t flags = 0;
    std::uint32_t init_order = 0, control = 0, v86_api = 0, pm_api = 0, v86_api_address = 0, pm_api_address = 0,
                  reference_data = 0, service_table = 0, service_count = 0;  // as stored
    std::optional<Place> at_control, at_v86_api, at_pm_api, at_service_table, at_reference_data;
    std::size_t size = 0;  // 0x50 with the "Prev" extension, else 0x34
    Bytes bytes;           // the block's bytes
};

struct ServiceCall {  // an `int 20h` VxD service call or jump in code
    std::uint32_t object = 0;
    std::uint64_t offset = 0;
    std::uint32_t device = 0, service = 0;
    bool jump = false;
};

// ---- the parts of an image, to take it apart and write it back ------------------------------------------------
struct ObjectEntry {
    std::uint32_t vsize = 0, base = 0, flags = 0, first_page = 0, page_count = 0;
    std::optional<std::array<std::uint8_t, 4>> reserved;  // absent: four zero bytes
};
struct StoredPage {
    std::uint8_t kind = kPagePresent;
    Bytes body;
};
// A name table to build, or the table's bytes as stored.
using NameTable = std::variant<std::vector<NameOrdinal>, Bytes>;
struct Moved {
    std::uint32_t debug_info = 0, resource_data = 0, from = 0;
};
struct Parts {
    Bytes stub, header;
    std::vector<ObjectEntry> objects;
    std::vector<StoredPage> pages;
    NameTable resident, nonresident;
    Bytes entry_table;
    OrderedMap<std::uint32_t, std::vector<Bytes>> fixups;  // page number -> the page's records, first seen first
    std::vector<std::string> import_modules;
    Bytes import_names;
    std::optional<std::uint32_t> align;
    std::optional<std::uint32_t> last_page_bytes;
    std::optional<std::pair<Bytes, Bytes>> gaps;
    Bytes after;
    Moved moved;
    std::optional<Bytes> page_map;
    std::optional<Bytes> tables_tail;
    std::optional<std::uint32_t> data_pages;
    // header field name -> value, for fields that point into the data pages and move with them
    std::optional<OrderedMap<std::string, std::int64_t>> in_pages;
    std::optional<Bytes> fill_from;
};

class Module {
public:
    // The module whose header is at `lfa` of the shared data (a container's members share one buffer).
    static Module parse(std::shared_ptr<const Bytes> data, std::uint64_t lfa);
    static Module parse(Bytes data, std::uint64_t lfa) { return parse(std::make_shared<const Bytes>(std::move(data)), lfa); }
    // An ordinary file: e_lfanew finds an "LE"/"LX" header, else FormatError.
    static Module from_buffer(Bytes data);
    static Module open(const std::filesystem::path& file);

    ByteView data() const { return *data_; }
    std::uint64_t lfa() const { return lfa_; }
    const Header& header() const { return header_; }
    std::string_view signature() const { return signature_; }  // "LE" or "LX"
    std::span<const Object> objects() const { return objects_; }
    // A member of a container (its e_lfanew does not point at its own header).
    bool is_member() const;

    // ---- address space ------------------------------------------------------------------------------------
    std::vector<PageEntry> page_map() const;
    // The object as loaded: its pages in order, each padded to the page size, cut or zero-extended to its virtual size.
    Bytes object_data(const Object& obj) const;
    // The file offset of byte `offset` of object `object` (1-based); nullopt where the page is not in the file.
    std::optional<std::uint64_t> file_offset(std::uint32_t object, std::int64_t offset) const;
    // Which object a page lies in, and the offset of the page within it.
    std::optional<std::pair<const Object*, std::uint64_t>> page_to_object(std::uint32_t page) const;

    // ---- names, entries, exports ----------------------------------------------------------------------------
    std::string resident_name() const;  // the module name: the first resident-name entry
    std::string description() const;    // the non-resident description string, "" if none
    std::vector<std::string> import_modules() const;
    // The resident and non-resident name tables (each entry as stored: the first is the module name or description).
    std::pair<std::vector<NameOrdinal>, std::vector<NameOrdinal>> names() const;
    std::vector<EntryPoint> entries() const;
    std::size_t entry_table_size() const;
    std::vector<Export> exports() const;

    // ---- fix-ups ----------------------------------------------------------------------------------------------
    // Every patched place and its target (import by ordinal and internal references; by-name and entry
    // fix-ups end the page's records and are not read here).
    std::vector<std::pair<Place, Fixup>> fixups() const;
    std::vector<FixupRecord> fixup_records() const;
    std::vector<Reference> references() const;
    // Absolute 32-bit pointers that fix-ups give a target: place -> target place.
    std::map<Place, Place> pointers() const;

    // ---- the device ------------------------------------------------------------------------------------------------
    std::optional<Ddb> ddb() const;
    std::optional<Descriptor> descriptor() const;
    // The service table's entries as places (nullopt where the table has no fix-up for a slot).
    std::vector<std::optional<Place>> service_table(const Descriptor& d) const;
    std::vector<std::optional<Place>> service_table() const;  // of this module's own descriptor
    std::vector<ServiceCall> service_calls() const;

    // ---- taking apart and putting back -------------------------------------------------------------------------
    Parts parts() const;

private:
    Module() = default;
    void index_pages();
    Bytes page_body(const PageEntry& e, std::uint32_t last_number) const;

    std::shared_ptr<const Bytes> data_;
    std::uint64_t lfa_ = 0;
    std::string signature_;
    Header header_;
    std::uint64_t objtab_ = 0, pagemap_ = 0, resnames_ = 0, entrytab_ = 0, fixup_page_ = 0, fixup_rec_ = 0;
    std::vector<Object> objects_;
    std::map<std::uint32_t, std::uint32_t> page_order_;  // page number -> first place in the map carrying it
};

// The bytes of an image from its parts (stub, header, tables, pages ...). FormatError when a field does not fit.
Bytes write_parts(const Parts& image);
// One fix-up record as stored.
Bytes encode_fixup(const FixupRecord& rec);
Bytes object_table(const std::vector<ObjectEntry>& objects);
Bytes name_table(const NameTable& names);
// (fix-up page table, fix-up records) for `pages` pages from {page number -> record bytes}.
std::pair<Bytes, Bytes> fixup_tables(const std::map<std::uint32_t, std::vector<Bytes>>& fixups, std::uint32_t pages);
// Whether a device descriptor block (DDB) can start at `at` of an object's bytes: a DDK version (0x03xx or 0x04xx)
// followed by an alphanumeric, printable name.
bool plausible_descriptor(ByteView object_bytes, std::uint64_t at);
// The names the table at `offset` of `d` lists (tolerant: it stops at the first bad entry); `end` bounds it.
std::vector<NameOrdinal> names_at(ByteView d, std::uint64_t offset, std::optional<std::uint64_t> end = std::nullopt);

// ---- containers: W3 (VMM32.VXD, WIN386.EXE) and W4 (the same, page-compressed) -----------------------------------
struct ContainerMember {
    std::string name;
    std::uint32_t offset = 0, size = 0;  // of the member's LE header in the (expanded) file
};
struct Container {
    enum class Kind { W3, W4 };
    Kind kind = Kind::W3;
    std::uint64_t at = 0;  // the container header's offset
    std::uint16_t version = 0;
    Bytes reserved;  // W3
    std::vector<ContainerMember> members;  // W3
};
std::optional<Container> container(ByteView data);  // nullopt when it is neither
Bytes pack_directory(const Container& c);

// The modules a file holds: a plain LE/LX is one entry named `name`; a W3 container's members are its entries
// (a W4 container is expanded first). The modules share one buffer.
std::vector<std::pair<std::string, Module>> modules(Bytes data, std::string_view name);
std::vector<std::pair<std::string, Module>> open_modules(const std::filesystem::path& file);

// W4 page compression. `expand` returns the file with every compressed page expanded (a non-W4 input unchanged);
// `compress` compresses the pages of `data` the way the W4 container `like` is laid out.
Bytes expand_page(ByteView compressed, std::size_t size = 0x2000);
Bytes compress_page(ByteView page);
Bytes expand(ByteView data);
Bytes compress(ByteView data, ByteView like);

}  // namespace cb::formats::le

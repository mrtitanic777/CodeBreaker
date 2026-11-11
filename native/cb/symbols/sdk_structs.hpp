// cb/symbols/sdk_structs.hpp -- the layouts of the SDK's structures, read from its headers.
//
//     const StructTable table(read_headers(folders, constants));
//     const Layout* rect = table.layout("RECT");       size 16, fields left/top/right/bottom
//     table.field_at(*rect, 8, 4)                      "right": the field that starts at 8 and is 4 bytes
//     table.pointed_to("LPCRECT")                      "RECT": the structure a pointer type points at
//     table.definition("RECT")                         C text defining it, field for field, at the SDK's offsets
//
// A structure is read from its `typedef struct tagX { ... } X, *PX, FAR *LPX;` in the 32-bit headers (symbols/prototypes
// finds them). Offsets follow the headers' packing (#include <pshpackN.h> ... <poppack.h>, natural alignment
// otherwise). A field of a type this does not know the size of makes the whole structure unknown -- a wrong offset
// would name the wrong field. The definition written out lays the fields at those offsets explicitly (packed to 1,
// with the gaps as padding), so it means the same to any compiler.
//
// Reading the headers (`read_headers`) is the slow part; the caller keeps what it makes in the knowledge base (platform
// win32, kinds "struct" and "struct.pointer"), made again when the header folders or the reader change.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/ordered_map.hpp"

namespace cb::symbols::sdk_structs {

// A change of the reader, to be part of the stamp of what the caller keeps.
inline constexpr int kReaderRevision = 8;

// One field: where it starts, how many bytes it takes (all of an array), its name and its C type.
struct Field {
    std::uint32_t offset = 0;
    std::uint32_t size = 0;
    std::string name;
    std::string ctype;       // "uint4", "int2", "struct RECT", "uint1[16]" ...
    bool pointer = false;    // a pointer (ctype "uint4"), not an array of them
    friend bool operator==(const Field&, const Field&) = default;
};

// A structure that could be laid out.
struct Layout {
    std::uint32_t size = 0;
    std::vector<Field> fields;
    std::uint32_t alignment = 1;   // the biggest alignment of a field: what a structure inside another is aligned to
    friend bool operator==(const Layout&, const Layout&) = default;
};

// {name: layout}, in the order the structures were met; a structure that could not be laid out is nullopt.
using Structs = OrderedMap<std::string, std::optional<Layout>>;
// {pointer type: the structure it points at}
using Pointers = OrderedMap<std::string, std::string>;

struct HeaderStructs {
    Structs structs;
    Pointers pointers;
};

// The values of the headers' #defines (array lengths are often one).
using Constants = std::unordered_map<std::string, std::int64_t>;

// Every .h of the folders, in the order of the names (the first declaration of a name is kept).
HeaderStructs read_headers(std::span<const std::filesystem::path> folders, const Constants& constants);

class StructTable {
public:
    explicit StructTable(HeaderStructs data);

    const HeaderStructs& data() const { return data_; }

    // The layout of a structure, or nullptr when it is not known or could not be laid out.
    const Layout* layout(std::string_view name) const;

    // The structure a parameter of this type points at ("LPRECT", "CONST RECT *"), if it is one that is known.
    std::optional<std::string> pointed_to(std::string_view type_text) const;

    // The field (a dotted path into a nested structure) of `layout` that starts at `offset` and is `size` bytes.
    std::optional<std::string> field_at(const Layout& layout, std::uint32_t offset, std::uint32_t size) const;

    // (field path, its C type) for the field of `layout` starting at `offset` that is `size` bytes: a plain integer
    // field, not an array or a structure.
    struct Typed {
        std::string path;
        std::string ctype;
    };
    std::optional<Typed> typed_field(const Layout& layout, std::uint32_t offset, std::uint32_t size) const;

    // C text defining the structure and the structures in it (before it), laid out field for field at the SDK's
    // offsets. `known`: structures already defined, which are left out. The structure must be one layout() knows.
    std::string definition(std::string_view name, const std::vector<std::string>& known = {}) const;

private:
    HeaderStructs data_;
};

}  // namespace cb::symbols::sdk_structs

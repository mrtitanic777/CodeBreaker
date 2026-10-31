// le.h -- a Linear Executable (Windows 95's VxDs, .vxd / .386) and the containers that hold
// several (W3: VMM32.VXD, WIN386.EXE; W4: the same compressed).
//
// The C++ form of codebreaker/formats/le.py: the header, the object table and the page map,
// each object's bytes as loaded, the name and entry tables, every kind of fix-up record, the
// device descriptor block with its service table and entry points, the service calls, the
// W3 directory and the W4 page compression -- and taking an image apart and writing it back.
// Reads keep Python's rules (core/bytes.h), so a damaged image raises what the Python raised.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "../core/bytes.h"
#include "../core/pyvalue.h"

namespace cbf::le {

constexpr int64_t kHeaderSize = 0xC4;

struct Object {
    int64_t idx;
    uint32_t vsize, base, flags, pmidx, pcnt;
    bool exec() const { return flags & 0x04; }
    bool read() const { return flags & 0x01; }
    bool write() const { return flags & 0x02; }
};

struct PageEntry {                    // page_map(): the page's number in the file, its kind
    int64_t number;
    uint8_t kind;                     // 0 present, 1 iterated, 2 invalid, 3 zero-filled
};

// LEFile.fixups(): one patched place's target
struct Fixup {
    bool import;                      // else internal
    int64_t obj_or_module;
    std::optional<int64_t> target;    // internal (None: a 16-bit selector)
    int64_t srctype = 0;              // internal
    int64_t ordinal = 0;              // import
};

// fixup_records(): one record as stored
struct FixupRecord {
    int64_t page;                     // 1-based
    uint8_t source, source_flags, flags;
    enum Kind { Internal, Import, ImportName, Entry } kind;
    int64_t first;                    // obj / module / entry
    std::optional<uint64_t> target;   // Internal (None: a selector)
    uint64_t ordinal = 0, name = 0;   // Import, ImportName
    std::optional<uint64_t> addend;
    std::vector<int64_t> sources;
    Bytes raw;
};

struct Ddb {                          // LEFile.ddb()
    int64_t obj, rva;
    uint16_t version, device_id;
    uint8_t major, minor;
    uint16_t flags;
    Bytes name;                       // trailing blanks and zeros cut; latin1
    uint32_t init_order, control_proc;
};

struct Export {                       // exports(): name, ordinal, object, offset
    std::string name;
    int64_t ordinal, obj, off;
};

struct EntryPoint {                   // entries(): ordinal, object, offset, flags
    int64_t ordinal, obj, off;
    uint8_t flags;
};

struct Reference {                    // references()
    int64_t obj, off, source, target_obj, target_off;
};

using Place = std::pair<int64_t, int64_t>;   // (object, offset)

// The descriptor block, every field (descriptor())
struct Descriptor {
    int64_t obj, offset;
    std::string name;
    std::vector<std::pair<const char *, uint64_t>> fields;   // DESCRIPTOR's fields, as stored
    std::map<std::string, std::optional<Place>> addresses;   // control, v86_api ... -> where, or None
    int64_t size;
    Bytes bytes;
    uint64_t field(const char *name) const;
};

struct ServiceCall {
    int64_t obj, off, device, service;
    bool jump;
};

class Image {                         // LEFile
public:
    Image(Bytes data, int64_t lfa);   // raises as LEFile.__init__
    static Image from_buffer(Bytes data);

    Bytes d;
    int64_t lfa;
    Bytes sig;
    uint16_t cpu, ostype;
    uint32_t mpages, eip_obj, eip, pagesize, last_page_bytes, nobj, datapages;
    int64_t objtab, pagemap, resnames, entrytab, fixup_page, fixup_rec;
    std::vector<Object> objects;

    std::pair<const Object *, int64_t> page_to_obj(int64_t pg) const;   // the page order read once, then kept (_page_order)
    std::vector<std::pair<Place, Fixup>> fixups(std::vector<size_t> *shared = nullptr) const;
    std::vector<std::string> import_modules() const;
    std::string resident_name() const;
    std::string description() const;
    std::optional<Ddb> ddb() const;
    uint64_t header_u(int64_t off, int width) const;   // header(): 0 past the end

private:
    mutable std::optional<std::map<int64_t, int64_t>> page_order_;
};

struct W3Entry {
    std::string name;
    uint32_t off, size;
};

class W3 {                            // W3File
public:
    W3(Bytes data, int64_t lfa);
    Bytes d;
    int64_t lfa;
    uint16_t version;
    uint32_t count;
    std::vector<W3Entry> entries;
    std::vector<std::pair<std::string, int64_t>> modules() const;   // (name, its LE header) to open
};

struct Container {                    // container()
    std::string kind;                 // "W3" | "W4"
    int64_t at;
    uint16_t version;
    Bytes reserved;                   // W3
    std::vector<std::tuple<std::string, uint32_t, uint32_t>> members;
};

// the module-level readers
std::vector<std::pair<const char *, uint64_t>> header_fields(const Image &lf);
bool is_member(const Image &lf);
std::vector<PageEntry> page_map(const Image &lf);
std::string object_data(const Image &lf, int64_t pmidx, int64_t pcnt, int64_t vsize);
std::optional<int64_t> file_offset(const Image &lf, int64_t obj_index, int64_t off);
std::vector<std::pair<std::string, uint16_t>> names_at(Bytes d, int64_t p, std::optional<int64_t> end);
std::pair<std::vector<std::pair<std::string, uint16_t>>, std::vector<std::pair<std::string, uint16_t>>> names(const Image &lf);
std::vector<EntryPoint> entries(const Image &lf);
int64_t entry_table_size(const Image &lf);
std::vector<Export> exports(const Image &lf);
std::vector<FixupRecord> fixup_records(const Image &lf);
std::vector<Reference> references(const Image &lf);
std::vector<std::pair<Place, Place>> pointers(const Image &lf);   // a dict: first place kept, last value
std::optional<Descriptor> descriptor(const Image &lf);
std::vector<std::optional<Place>> service_table(const Image &lf, std::optional<Place> table, int64_t count);
std::vector<ServiceCall> service_calls(const Image &lf);
std::optional<Container> container(Bytes data);
std::string expand_page(Bytes buf, int64_t size);
std::string compress_page(Bytes page);
std::string expand(Bytes data);

// image.cc: taking an image apart and writing it back
void read_image(const Image &lf, Out &o);            // the dict, written out
std::string write_image(const Value &image);
std::string encode_fixup(const Value &rec);
std::string object_table(const Value &objects);
std::string name_table(const Value &pairs);
std::pair<std::string, std::string> fixup_tables(const std::map<int64_t, std::vector<std::string>> &fixups, int64_t pages);
std::string pack_directory(const Value &c);
std::string compress(Bytes data, Bytes like);

}  // namespace cbf::le

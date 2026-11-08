// codebreaker/formats/le.hpp -- typed C++ of codebreaker.formats.le
//
// le.py -- Linear Executable (LE) reader, answered by the C++ (native/formats/le: formats.dll) for Win95
// Virtual Device Drivers (.vxd / .386).
//
// VxDs are Ring-0 32-bit drivers in the LE format (not the 16-bit NE of the system DLLs), so
// they need their own loader. This parses the LE header, the object table, and extracts each
// object's bytes from the data pages, and it locates + decodes the DDB (Device Descriptor Block)
// -- the VxD's identity: its 8-char device name, DDK version, device id, and control procedure.
//
//     LEFile v = LEFile::from_file("vmouse.vxd");
//     v.ddb();                              // {name: 'VMD', version: 0x40a, device_id: ...}
//     v.object_bytes(v.objects[0]);         // 32-bit code of the first object
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * The readers are the C++ of native/formats/le (le.cc, with core/bytes.cc and core/text.cc): linked, and
//     called directly with typed results, no ctypes and no pickle.  The Python got its answers back as pickled
//     dicts/tuples; they are the structs below (same fields, same order).  An exception the reader raised comes
//     back as the py:: exception of the same Python type (struct.error is py::StructError).
//     The question handlers of native/formats/le/questions.cc and the image code of native/formats/le/image.cc
//     (which shaped their answers as pickle / a dynamic Value) are the typed functions in le.cpp.
//   * LEFile's `objects` (a list of dicts) are `Object`; `_page_order` (always None after open) is not kept.
//   * A fix-up of LEFile::fixups is a dict of one of two shapes: the std::variant InternalFixup / ImportFixup.
//   * A fix-up record (fixup_records) is a dict whose keys depend on its kind: the struct FixupRecord with
//     std::optional fields (only the fields of its kind are set).  encode_fixup takes one back; a field that
//     its kind needs and that is not set raises the KeyError the missing key raised.
//   * read_image gives a dict that write_image takes back (a driver build may add keys, or leave some out):
//     the struct Image.  Keys that read_image always writes are plain fields (a missing one, KeyError in
//     Python, cannot be expressed); the optional keys write_image also reads are std::optional.
//     "resident"/"nonresident" may be a name list or a table's bytes: the std::variant NameTable.
//   * A data argument is py::bytes (a bytearray is not a separate type here).  LEFile keeps its bytes shared
//     (members of one container share the container's bytes, as in Python).
//   * descriptor()'s dict is the struct Descriptor; service_table() takes it back as an optional.
//   * Ints are int64_t (the native readers' uint32 values fit); no real input comes near its overflow.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::le {

using Place = std::tuple<int64_t, int64_t>;                           // (object, offset)

// ---- an object of the object table (what LEFile.objects holds, as dicts)
struct Object {
    int64_t idx = 0;                                                  // 1-based
    int64_t vsize = 0, base = 0, flags = 0, pmidx = 0, pcnt = 0;
    bool exec = false, read = false, write = false;
};

// ---- LEFile.fixups(): {(obj_idx, off_in_obj): fixup}
struct InternalFixup {                                                // {'kind': 'internal', 'obj', 'target', 'srctype'}
    int64_t obj = 0;
    std::optional<int64_t> target;                                    // None: a 16-bit selector
    int64_t srctype = 0;
};
struct ImportFixup {                                                  // {'kind': 'import', 'module', 'ordinal'}
    int64_t module = 0;
    int64_t ordinal = 0;
};
using Fixup = std::variant<InternalFixup, ImportFixup>;

// ---- LEFile.header()
struct Header {
    py::str signature;
    int64_t cpu = 0, os = 0, module_version = 0, module_flags = 0, pages = 0, entry_object = 0, entry = 0,
            stack_object = 0, stack = 0, page_size = 0, last_page_bytes = 0, fixup_section_size = 0,
            loader_section_size = 0, objects = 0, imported_modules = 0, data_pages_offset = 0, device_id = 0,
            ddk_version = 0;
};

// ---- LEFile.ddb()
struct Ddb {
    int64_t obj = 0, rva = 0, version = 0, device_id = 0, major = 0, minor = 0, flags = 0;
    py::str name;
    int64_t init_order = 0, control_proc = 0;
};

// ---- fixup_records(): one record as stored
struct FixupRecord {
    int64_t source = 0, source_flags = 0, flags = 0;
    py::str kind;                                                     // 'internal', 'import', 'import-name', 'entry'
    std::optional<int64_t> obj;                                       // internal
    std::optional<int64_t> target;                                    // internal (None: a selector)
    std::optional<int64_t> module;                                    // import, import-name
    std::optional<int64_t> ordinal;                                   // import
    std::optional<int64_t> name;                                      // import-name
    std::optional<int64_t> entry;                                     // entry
    std::optional<int64_t> addend;
    py::list<int64_t> sources;
    py::bytes raw;
};

// ---- descriptor(): the device descriptor block, every field
struct Descriptor {
    int64_t obj = 0, offset = 0;
    py::str name;
    int64_t next = 0, kit_version = 0, device_id = 0, major = 0, minor = 0, flags = 0, init_order = 0;
    std::optional<Place> control, v86_api, pm_api;
    int64_t v86_api_address = 0, pm_api_address = 0;
    std::optional<Place> reference_data, service_table;
    int64_t service_count = 0;
    int64_t control_raw = 0, v86_api_raw = 0, pm_api_raw = 0, service_table_raw = 0, reference_data_raw = 0;
    int64_t size = 0;                                                 // 0x50 or 0x34
    py::bytes bytes;
};

// ---- read_image() / write_image()
struct ImageObject {
    int64_t vsize = 0, base = 0, flags = 0, first_page = 0, page_count = 0;
    std::optional<py::bytes> reserved;                                // absent: four zero bytes
};
using NameList = py::list<std::tuple<py::str, int64_t>>;
using NameTable = std::variant<NameList, py::bytes>;                  // names to build, or the table's bytes as stored
struct Moved {
    int64_t debug_info = 0, resource_data = 0, from = 0;
};
struct Image {
    py::bytes stub, header;
    py::list<ImageObject> objects;
    py::list<std::tuple<int64_t, py::bytes>> pages;                   // (kind, bytes)
    NameTable resident, nonresident;
    py::bytes entry_table;
    py::dict<int64_t, py::list<py::bytes>> fixups;                    // {page: [record bytes]}
    py::list<py::str> import_modules;
    py::bytes import_names;
    std::optional<int64_t> align;                                     // read_image always sets it
    std::optional<int64_t> last_page_bytes;
    std::optional<std::tuple<py::bytes, py::bytes>> gaps;
    py::bytes after;
    Moved moved;
    // what a driver rebuilt from recovered code keeps (read_image leaves them out)
    std::optional<py::bytes> page_map;
    std::optional<py::bytes> tables_tail;
    std::optional<int64_t> data_pages;
    std::optional<py::dict<py::str, int64_t>> in_pages;
    std::optional<py::bytes> fill_from;
};

// ---- container()
struct Container {
    py::str kind;                                                     // 'W3' or 'W4'
    int64_t at = 0, version = 0;
    std::optional<py::bytes> reserved;                                // W3 only
    py::list<std::tuple<py::str, int64_t, int64_t>> members;          // (name, offset of the member's LE header, size)
};

class W3File;

// An LE/LX image at `lfa` in `data`: sig, cpu, objects, pagesize ...
class LEFile {
  public:
    LEFile(const py::bytes& data, int64_t lfa);
    LEFile(std::shared_ptr<const py::bytes> data, int64_t lfa);       // (the bytes shared, as members of a container are)
    static LEFile from_buffer(const py::bytes& data);
    static LEFile from_file(const py::str& path);

    const py::bytes& d() const { return *data_; }
    std::shared_ptr<const py::bytes> shared_data() const { return data_; }

    int64_t lfa = 0;
    py::bytes sig;
    int64_t cpu = 0, ostype = 0, mpages = 0, eip_obj = 0, eip = 0, pagesize = 0, last_page_bytes = 0, objtab = 0,
            nobj = 0, pagemap = 0, resnames = 0, entrytab = 0, fixup_page = 0, fixup_rec = 0, datapages = 0;
    py::list<Object> objects;

    // Raw bytes of one object: its pages found through the page map, up to its virtual size.
    py::bytes object_bytes(const Object& obj) const;
    // A page as the fix-up tables number it (1-based) -> (object, byte offset of the page within the object).
    std::tuple<std::optional<Object>, int64_t> page_to_obj(int64_t pg) const;
    // Parse the LE fixup page + record tables into {(obj_idx, off_in_obj): fixup}.
    py::dict<Place, Fixup> fixups() const;
    // The imported module names (VMM, VPICD, ...) from the Imported Modules Name Table.
    py::list<py::str> import_modules() const;
    // The module's own name (first entry of the resident-name table, length-prefixed).
    py::str resident_name() const;
    // The header's fields by name -- what an inspector shows.
    Header header() const;
    // The module's description line: the first entry of the non-resident name table ('' when none).
    py::str description() const;
    // Locate and decode the Device Descriptor Block (None when there is none).
    std::optional<Ddb> ddb() const;

  private:
    std::shared_ptr<const py::bytes> data_;
    void open_();
};

struct W3Entry {
    py::str name;
    int64_t off = 0, size = 0;
};

// W3 container (e.g. vmm32.vxd): a bundle of concatenated LE images.
class W3File {
  public:
    W3File(const py::bytes& data, int64_t lfa);
    // [(name, LEFile), ...] for each embedded LE image (offsets are the module's LE header).
    py::list<std::tuple<py::str, LEFile>> modules() const;
    static W3File from_file(const py::str& path);

    const py::bytes& d() const { return *data_; }
    int64_t lfa = 0;
    int64_t version = 0, count = 0;
    py::list<W3Entry> entries;

  private:
    std::shared_ptr<const py::bytes> data_;
};

// [(name, LEFile), ...] for any VxD file: one entry for a plain LE/LX, all embedded modules for a W3 container.
py::list<std::tuple<py::str, LEFile>> open_vxds(const py::str& path);

// ================================================================================================
// The rest of what an LE image holds
// ================================================================================================
extern const int64_t HEADER_SIZE;
extern const int64_t MODULE_STATIC, MODULE_DYNAMIC, MODULE_OLD;                         // module flags of a driver
extern const py::list<std::tuple<py::str, int64_t, py::str>> _FIELDS;                  // (field, offset, struct format)

// The LE header's fields as a dict.
py::dict<py::str, int64_t> header_fields(const LEFile& lf);
// Is this image one inside a container?
bool is_member(const LEFile& lf);
// [(page in the file, kind)] for each of the image's pages in order.
py::list<std::tuple<int64_t, int64_t>> page_map(const LEFile& lf);
// One object's bytes as loaded.
py::bytes object_data(const LEFile& lf, const Object& obj);
// Where byte `off` of object `obj_index` (1-based) is in the file, or None.
std::optional<int64_t> file_offset(const LEFile& lf, int64_t obj_index, int64_t off);
py::list<std::tuple<py::str, int64_t>> _names(const py::bytes& d, int64_t p, std::optional<int64_t> end = std::nullopt);
// ([(name, ordinal)] resident, [(name, ordinal)] non-resident).
std::tuple<NameList, NameList> names(const LEFile& lf);
// [(ordinal, object, offset, flags)] from the entry table.
py::list<std::tuple<int64_t, int64_t, int64_t, int64_t>> entries(const LEFile& lf);
int64_t _entry_table_size(const LEFile& lf);
// [(name, ordinal, object, offset)] -- the entry table joined with the name tables.
py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> exports(const LEFile& lf);
// Every fix-up record of the image, page by page: [(page, record)] with page 1-based.
py::list<std::tuple<int64_t, FixupRecord>> fixup_records(const LEFile& lf);
// The bytes of a fix-up record as fixup_records gives it (or one built the same way).
py::bytes encode_fixup(const FixupRecord& rec);
// [(object, offset, source kind, target object, target offset)]
py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t>> references(const LEFile& lf);
// {(object, offset): (target object, target offset)} for every place that holds the 32-bit address of another place.
py::dict<Place, Place> pointers(const LEFile& lf);
// Does a device descriptor block start at `at`?
bool _plausible(const py::bytes& b, int64_t at);
// The device descriptor block, every field, or None.
std::optional<Descriptor> descriptor(const LEFile& lf);
// [(object, offset) or None] -- where each service the driver offers is, by service number.
py::list<std::optional<Place>> service_table(const LEFile& lf, const std::optional<Descriptor>& d = std::nullopt);
// {label: (object, offset)} -- everywhere the system or another driver can enter this one.
py::dict<py::str, Place> entry_points(const LEFile& lf);
// [(object, offset, device, service, is_jump)] -- every `int 20h` service call in the executable objects.
py::list<std::tuple<int64_t, int64_t, int64_t, int64_t, bool>> service_calls(const LEFile& lf);

// Writing an image back ---------------------------------------------------------------------------
// An LE image taken apart into what write_image puts together again.
Image read_image(const LEFile& lf);
// A name table: its bytes when given as stored, else built from [(name, ordinal)].
py::bytes _name_table(const NameTable& pairs);
// The object table.
py::bytes object_table(const py::list<ImageObject>& objects);
// The fix-up page table and record table for {page: [record bytes]} over pages 1..`pages`.
std::tuple<py::bytes, py::bytes> fixup_tables(const py::dict<int64_t, py::list<py::bytes>>& fixups, int64_t pages);
// The file for an image as read_image describes it.
py::bytes write_image(const Image& image);

// Containers -------------------------------------------------------------------------------------
// The directory of a container of drivers (VMM32.VXD, WIN386.EXE), or None when `data` is not one.
std::optional<Container> container(const py::bytes& data);
// The header and directory bytes of a W3 container, from container()'s dict.
py::bytes pack_directory(const Container& c);
// One compressed page of a W4 container -> its bytes.
py::bytes expand_page(const py::bytes& buf, int64_t size = 0x2000);
// One page -> compressed bytes expand_page turns back into it.
py::bytes compress_page(const py::bytes& page);
// A compressed (W4) container as the plain (W3) one it holds; anything else unchanged.
py::bytes expand(const py::bytes& data);
// A plain (W3) container `data` as a compressed one, reusing the program and header in front of `like`.
py::bytes compress(const py::bytes& data, const py::bytes& like);
// [(name, LEFile)] for any file of drivers (a W4 container after expanding it).
py::list<std::tuple<py::str, LEFile>> members(const py::str& path);
// Every file in the corpus that holds drivers (LE images and containers), one per file name, smallest first.
py::list<py::str> corpus_files();

}  // namespace codebreaker::formats::le

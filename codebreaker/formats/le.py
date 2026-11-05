r"""le.py -- Linear Executable (LE) reader, answered by the C++ (native/formats/le: formats.dll) for Win95 Virtual Device Drivers (.vxd / .386).

VxDs are Ring-0 32-bit drivers in the LE format (not the 16-bit NE of the system DLLs), so
they need their own loader. This parses the LE header, the object table, and extracts each
object's bytes from the data pages, and it locates + decodes the DDB (Device Descriptor Block)
-- the VxD's identity: its 8-char device name, DDK version, device id, and control procedure.
That identity is the anchor for reconstructing the driver as clean MASM (its VxD_Desc_Block,
Begin_Control_Dispatch, and service table).

    from codebreaker.formats import le
    v = le.LEFile.from_file("vmouse.vxd")
    print(v.ddb())                       # {'name': 'VMD', 'version': 0x40a, 'device_id': ...}
    code = v.object_bytes(v.objects[0])  # 32-bit code of the first object
"""
import os

from codebreaker.formats import native


# ---- the C++ reader (native/formats/le): its Python is gone (docs/NATIVE.md, step 4) -----------
def _image_args(lf, *rest):
    return lf.d, (lf.lfa,) + rest


def _file_args(path):
    with open(path, "rb") as f:
        return f.read(), (os.path.basename(path),)


def _opened(data, pairs):
    """[(name, LEFile)] for (name, lfa) pairs found in `data`, opened in order (one that fails
    raises there)."""
    return [(name, LEFile(data, lfa)) for name, lfa in pairs]


def _vxds_result(answer, data, path):
    return _opened(data, answer)


def _members_result(answer, data, path):
    expanded, pairs = answer
    return _opened(data if expanded is None else expanded, pairs)


def _object_of(lf, answer):
    idx, off = answer
    return (lf.objects[idx - 1] if idx is not None else None), off


def _where(lf):
    return (lf.lfa,)


class LEFile:
    """An LE/LX image at `lfa` in `data`: sig, cpu, objects, pagesize ... (le.open's attributes),
    every method answered by formats.dll."""

    def __init__(s, data, lfa):
        native.required()
        s.d = data
        s.lfa = lfa
        state = native.call("le.open", data, lfa)
        if isinstance(data, bytearray):
            state["sig"] = bytearray(state["sig"])
        s.__dict__.update(state)

    @classmethod
    def from_buffer(cls, data):
        native.required()
        return cls(data, native.call("le.from_buffer", data))

    @classmethod
    def from_file(cls, path):
        return cls.from_buffer(open(path, "rb").read())

    @native.ask("le.object_bytes", _where, data="d")
    def object_bytes(s, obj):
        """Raw bytes of one object: its pages found through the page map, up to its virtual size."""

    @native.ask("le.page_to_obj", _where, data="d", result=_object_of)
    def page_to_obj(s, pg):
        """A page as the fix-up tables number it (1-based) -> (object, byte offset of the page
        within the object).

        The page map gives, for each page of the image in order, a number; the page's bytes in
        the file AND its row of the fix-up page table are found by that number. In most drivers
        the numbers simply count up and the distinction does not show; in the ones linked with
        their pages out of order (DOSMGR, SHELL, ...) reading the fix-up tables by image order
        gives every patched place to the wrong object."""

    @native.ask("le.fixups", _where, data="d")
    def fixups(s):
        """Parse the LE fixup page + record tables into {(obj_idx, off_in_obj): fixup}. Each
        fixup is {'kind':'internal', 'obj', 'target'} (an intra-image code/data reference -> a
        label) or {'kind':'import', 'module', 'ordinal'} (a VMM/VxD service imported by
        module:ordinal -> a named call). This is what turns raw operands into symbols."""

    @native.ask("le.import_modules", _where, data="d")
    def import_modules(s):
        """The imported module names (VMM, VPICD, ...) from the Imported Modules Name Table."""

    @native.ask("le.resident_name", _where, data="d")
    def resident_name(s):
        """The module's own name (first entry of the resident-name table, length-prefixed)."""

    @native.ask("le.header", _where, data="d")
    def header(s):
        """The header's fields by name -- what an inspector shows (used by formats/inspect.py)."""

    @native.ask("le.description", _where, data="d")
    def description(s):
        """The module's description line: the first entry of the non-resident name table ('' when
        there is none)."""

    @native.ask("le.ddb", _where, data="d")
    def ddb(s):
        """Locate and decode the Device Descriptor Block. Scan each object's bytes for the DDB
        signature: a dword-aligned DDK version (0x03xx/0x04xx) followed by device id, a
        major/minor byte pair, flags, and an 8-byte device name of printable ASCII. Returns the
        decoded DDB or None. The name is what `Declare_Virtual_Device <name>` emits."""


class W3File:
    r"""W3 container (e.g. vmm32.vxd): a bundle of concatenated LE images. The header is 'W3',
    a version word, a module count dword, then `count` 16-byte entries (8-char name + dword file
    offset + dword size) starting at +0x10. Win95 builds vmm32.vxd by packing VMM (and, on some
    machines, the statically-loaded VxDs) into one W3 so the loader maps them together."""

    def __init__(s, data, lfa):
        native.required()
        s.d = data
        s.lfa = lfa
        s.__dict__.update(native.call("le.w3", data, lfa))

    def modules(s):
        """[(name, LEFile), ...] for each embedded LE image (offsets are the module's LE header)."""
        return _opened(s.d, native.call("le.w3_modules", s.d, s.lfa))

    @classmethod
    def from_file(cls, path):
        d = open(path, "rb").read()
        return cls(d, int.from_bytes(d[0x3C:0x40], "little"))


@native.answered("le.open_vxds", args_of=_file_args, result=_vxds_result)
def open_vxds(path):
    """[(name, LEFile), ...] for any VxD file: one entry for a plain LE/LX, all embedded modules
    for a W3 container. The single entry point for feeding the disassembler."""


# ================================================================================================
# The rest of what an LE image holds: every header field, the loader's tables (page map, entry
# table, names, every kind of fix-up record), the device descriptor block in full with its
# service table and entry points, service calls, compressed containers -- and writing an image
# back.
#
#     from codebreaker.formats import le
#     for name, lf in le.members("vmm32.vxd"):         # any driver file; a container's members
#         d = le.descriptor(lf)                         # every field of the descriptor block
#         le.entry_points(lf)                           # {"control": (obj, off), "service_0": ...}
#         le.service_calls(lf)                          # [(obj, off, device, service, is_jump)]
#     image = le.read_image(lf); data = le.write_image(image)       # round trip
# ================================================================================================
HEADER_SIZE = 0xC4
MODULE_STATIC, MODULE_DYNAMIC, MODULE_OLD = 0x28000, 0x38000, 0x8020     # module flags of a driver
_FIELDS = (("byte_order", 0x02, "B"), ("word_order", 0x03, "B"), ("level", 0x04, "I"), ("cpu", 0x08, "H"),
           ("os", 0x0A, "H"), ("version", 0x0C, "I"), ("module_flags", 0x10, "I"), ("pages", 0x14, "I"),
           ("eip_object", 0x18, "I"), ("eip", 0x1C, "I"), ("esp_object", 0x20, "I"), ("esp", 0x24, "I"),
           ("page_size", 0x28, "I"), ("last_page_bytes", 0x2C, "I"), ("fixup_size", 0x30, "I"),
           ("loader_size", 0x38, "I"), ("object_table", 0x40, "I"), ("objects", 0x44, "I"),
           ("page_map", 0x48, "I"), ("resources", 0x50, "I"), ("resident_names", 0x58, "I"),
           ("entry_table", 0x5C, "I"), ("fixup_pages", 0x68, "I"), ("fixup_records", 0x6C, "I"),
           ("import_modules", 0x70, "I"), ("import_module_count", 0x74, "I"), ("import_names", 0x78, "I"),
           ("data_pages", 0x80, "I"), ("preload_pages", 0x84, "I"), ("nonresident_names", 0x88, "I"),
           ("nonresident_size", 0x8C, "I"), ("debug_info", 0x98, "I"), ("debug_size", 0x9C, "I"),
           ("resource_data", 0xB8, "I"), ("resource_size", 0xBC, "I"), ("device_id", 0xC0, "H"),
           ("kit_version", 0xC2, "H"))


@native.answered("le.header_fields", args_of=_image_args)
def header_fields(lf):
    """The LE header's fields as a dict (table offsets are as stored: relative to the header,
    except data_pages, nonresident_names, debug_info and resource_data, which are file offsets)."""


@native.answered("le.is_member", args_of=_image_args)
def is_member(lf):
    """Is this image one inside a container (its header is not the one the file points at)?"""


@native.answered("le.page_map", args_of=_image_args)
def page_map(lf):
    """[(page in the file, kind)] for each of the image's pages in order. kind: 0 present,
    1 iterated, 2 invalid, 3 zero-filled."""


@native.answered("le.object_data", args_of=_image_args)
def object_data(lf, obj):
    """One object's bytes as loaded: its pages found through the page map (they need not be in
    order or all present in the file), cut or zero-filled to its virtual size."""


@native.answered("le.file_offset", args_of=_image_args)
def file_offset(lf, obj_index, off):
    """Where byte `off` of object `obj_index` (1-based) is in the file, or None."""


@native.answered("le._names")
def _names(d, p, end=None):
    ...


@native.answered("le.names", args_of=_image_args)
def names(lf):
    """([(name, ordinal)] resident, [(name, ordinal)] non-resident). The first resident name
    (ordinal 0) is the module's own, the first non-resident one its description; the others
    name its exports. A container member's non-resident table is not looked for (its stored
    offset no longer means anything)."""


@native.answered("le.entries", args_of=_image_args)
def entries(lf):
    """[(ordinal, object, offset, flags)] from the entry table: the places the image exports.
    A driver has one, its device descriptor block."""


@native.answered("le._entry_table_size", args_of=_image_args)
def _entry_table_size(lf):
    ...


@native.answered("le.exports", args_of=_image_args)
def exports(lf):
    """[(name, ordinal, object, offset)] -- the entry table joined with the name tables."""


@native.answered("le.fixup_records", args_of=_image_args)
def fixup_records(lf):
    """Every fix-up record of the image, page by page: [(page, record)] with page 1-based and
    record a dict -- raw (its bytes), source (kind of place patched: 7 = 32-bit offset, 8 =
    32-bit self-relative, 2 = selector...), sources (offsets within the page; one may be
    negative when the place starts on the previous page), kind ('internal', 'import',
    'import-name', 'entry'), and for that kind: obj + target, module + ordinal, module + name,
    or entry; plus addend when the record carries one. Unlike LEFile.fixups this stops at
    nothing: all four kinds are read."""


@native.answered("le.encode_fixup", data_arg=None)
def encode_fixup(rec):
    """The bytes of a fix-up record as fixup_records gives it (or one built the same way)."""


@native.answered("le.references", args_of=_image_args)
def references(lf):
    """[(object, offset, source kind, target object, target offset)] -- every place in the
    image the loader patches with the address of another place in it (source kind 7: the
    32-bit address itself; 8: its distance from the end of the patched field, as in a call)."""


@native.answered("le.pointers", args_of=_image_args)
def pointers(lf):
    """{(object, offset): (target object, target offset)} for every place that holds the
    32-bit address of another place in the image."""


@native.answered("le._plausible")
def _plausible(b, at):
    """Does a device descriptor block start at `at`?"""


@native.answered("le.descriptor", args_of=_image_args)
def descriptor(lf):
    """The device descriptor block, every field, or None: where it is (obj, offset -- from the
    entry table, which exports exactly it; else found by its shape), the numbers it holds
    (kit_version, device_id, major, minor, flags, name, init_order, service_count), and each
    address it holds as (object, offset) or None: control, v86_api, pm_api, service_table,
    reference_data (the stored number is kept as <field>_raw). `size` is 0x50 for the block
    of this system, 0x34 for the older one; `bytes` is the block itself."""


@native.answered("le.service_table", args_of=_image_args)
def service_table(lf, d=None):
    """[(object, offset) or None] -- where each service the driver offers is, by service number
    (None: a number kept free). Empty for a driver that offers none."""


@native.answered("le.entry_points", args_of=_image_args)
def entry_points(lf):
    """{label: (object, offset)} -- everywhere the system or another driver can enter this one:
    'control' (the control procedure), 'service_N', 'v86_api', 'pm_api'."""


@native.answered("le.service_calls", args_of=_image_args)
def service_calls(lf):
    """[(object, offset, device, service, is_jump)] -- every `int 20h` service call in the
    executable objects (CD 20, service number, device number; bit 15 of the service number
    says jump instead of call and is returned apart)."""


# Writing an image back ----------------------------------------------------------------------------
@native.answered("le.read_image", args_of=_image_args)
def read_image(lf):
    """An LE image taken apart into what write_image puts together again: {"stub" (what is in
    front of the header), "header" (the header's own bytes), "objects" [{vsize, base, flags,
    reserved, first_page, page_count}], "pages" [(kind, bytes)] (all of them, in page-map
    order; an object owns a run of them), "resident" / "nonresident" [(name, ordinal)],
    "entry_table" (bytes), "fixups" {page: [record bytes]} (by the page's place in the page
    map, whatever its number), "import_modules" [names],
    "import_names" (bytes), "align" (what the data pages are aligned to in the file),
    "last_page_bytes" (the header's count of bytes on the last page, or None to count the
    last page written; a linker's 0 for a full last page is kept), "gaps" (the filler in front
    of the data pages, written when it still fits exactly, and the filler in front of the
    non-resident names), "after" (everything behind the data pages and the non-resident
    names: debug information, resources), "moved" (where the parts of "after" the header
    points at were)}. Change a part, write it back.

    write_image also takes what a driver rebuilt from recovered code keeps of its original
    linker's layout, which read_image leaves out: "page_map" (the page map's bytes as that
    linker numbered the pages), "tables_tail" (bytes kept right behind the import names,
    outside the fix-up section's size), "data_pages" (the file offset the data pages stay at
    while the tables fit in front of it), "in_pages" ({header field: file offset} for parts
    that sit among the data pages, as a container's member keeps its non-resident names and
    resources: the field points there, moved with the data pages, and a non-resident name table
    there is not written again) and "fill_from" (the original file, whose trailing bytes fill
    out a written file that comes out shorter). "resident" and
    "nonresident" may also be a name table's bytes, kept as stored."""


@native.answered("le._name_table", data_arg=None)
def _name_table(pairs):
    """A name table: its bytes when given as stored, else built from [(name, ordinal)]."""


@native.answered("le.object_table", data_arg=None)
def object_table(objects):
    """The object table: each object's size, base, flags, first page and page count, and the four
    bytes behind them ("reserved"; a driver's linker leaves the object's name there)."""


@native.answered("le.fixup_tables", data_arg=None)
def fixup_tables(fixups, pages):
    """The fix-up page table and record table for {page: [record bytes]} over pages 1..`pages`:
    where each page's records start, and where the last one's end. -> (page table, records)"""


@native.answered("le.write_image", data_arg=None)
def write_image(image):
    """The file for an image as read_image describes it: the stub (the header sits right behind
    it), the header with every offset and size worked out again, the object table, the page
    map, the name and entry tables, the fix-up tables, the import names and the tables'
    tail, the filler and the data pages, the filler and the non-resident names, and whatever came after. Pages are
    numbered in page-map order, unless the image keeps its own page map; either way each page is
    stored at the place its number gives and its fix-ups go in the row of that number (a loader
    finds both by the number), the highest-numbered page last and not filled out."""


# Containers -------------------------------------------------------------------------------------
@native.answered("le.container")
def container(data):
    """The directory of a container of drivers (VMM32.VXD, WIN386.EXE), or None when `data` is
    not one: {"kind": 'W3' or 'W4', "at": where its header is, "version", "members":
    [(name, offset of the member's LE header, size of its header and tables)]}. A W4 holds the
    same thing compressed: expand(data) gives the W3."""


@native.answered("le.pack_directory", data_arg=None)
def pack_directory(c):
    """The header and directory bytes of a W3 container, from container()'s dict."""


@native.answered("le.expand_page")
def expand_page(buf, size=0x2000):
    """One compressed page of a W4 container -> its bytes. Bits are read low first. Two equal
    bits start a copy from earlier in the page (00: distance in 6 bits; 110: 8 bits + 64;
    111: 12 bits + 320, all ones marking the end of a 512-byte stretch), two unequal bits a
    literal byte; a copy's length is a run of zero bits, a one, and that many more bits."""


@native.answered("le.compress_page")
def compress_page(page):
    """One page -> compressed bytes expand_page turns back into it (longest earlier match,
    never across a 512-byte boundary, each stretch closed with the end marker)."""


@native.answered("le.expand")
def expand(data):
    """A compressed (W4) container as the plain (W3) one it holds; anything else unchanged.
    The result keeps the program in front of the header, so member offsets are as stored."""


@native.answered("le.compress")
def compress(data, like):
    """A plain (W3) container `data` as a compressed one, reusing the program and header in
    front of `like` (a W4 container: its loader is what expands the pages at start-up)."""


@native.answered("le.members", args_of=_file_args, result=_members_result)
def members(path):
    """[(name, LEFile)] for any file of drivers: one for a plain LE image (also one behind a
    DOS program, like DRVSPACE.BIN), every member of a W3 container, and the same for a
    compressed W4 container after expanding it."""


def corpus_files():
    """Every file in the corpus that holds drivers (LE images and containers), one per file
    name, smallest first."""
    import os
    from codebreaker.base import corpus
    from codebreaker.formats import detect
    out = [cands[0] for _name, cands in corpus.index().items() if detect.holds_drivers(cands[0])]
    return sorted(out, key=lambda p: (os.path.getsize(p), p.lower()))

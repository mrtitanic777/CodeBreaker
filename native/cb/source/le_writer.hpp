// cb/source/le_writer.hpp -- a virtual device driver (LE, and the W3 bundle VMM32.VXD) as assembly source that builds back byte for byte.
//
//     le_writer::Result r = le_writer::reconstruct("vmouse.vxd", "out", {});
//     r.source, r.image, r.same, r.instructions, r.raw, r.rounds
//
// source/driver_writer writes a driver as the driver kit's macro source, built by wasm and wlink: that is the driver as it was written, and
// it comes back the same in structure. This writes the FILE, the way pe_writer and ne_writer do: one NASM source assembled flat (-f bin).
// The headers and tables are a section of their own; each object is a section at its first page's place in the file (start=) whose addresses
// are the object's offsets (vstart=0), so a label is what a fix-up names. Every fix-up record's place is written as its label less the
// start of its page, every internal target as the label it names, every 32-bit offset the file stores at a fix-up's place (the target, as
// LINK386 and the driver linker leave it) as that label, and the fix-up page table as the distances of each page's records from the first.
// An object whose pages are not in the file in order, or not all there, is written as its pages' bytes in the order the file has them.
//
// Code is what is reached from the entry point, the device descriptor's control procedure, the entry table and every internal fix-up that
// names a place in an executable object; an instruction is a mnemonic where NASM gives back its bytes (checked each round from the
// assembler's map), its bytes otherwise, and always its bytes when a fix-up patches a place inside it (that place labelled). A W3 bundle
// (VMM32.VXD) is its header and each of its drivers, each written as above, its labels named by the driver.
#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/le.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/source/flat_rounds.hpp"
#include "cb/source/x86_flat.hpp"
#include "cb/translate/l2.hpp"

namespace cb::source::le_writer {

// A place in a driver: object (1-based) and offset.
struct Place {
    std::uint32_t object = 0;
    std::uint32_t offset = 0;
    friend auto operator<=>(const Place&, const Place&) = default;
};

// A place in the file: the driver it is in (0 for a plain driver; a bundle's drivers in file order) and its place there.
struct Key {
    std::uint32_t driver = 0;
    Place at;
    friend auto operator<=>(const Key&, const Key&) = default;
};

struct Options {
    std::function<void(std::string_view)> say;          // progress, one line a round
};

// One object of the LE with what the writer knows of its place in the file.
struct Object {
    formats::le::Object object;
    bool whole = false;                 // all its pages are in the file, in order
    std::size_t start = 0;              // where in the file (when whole)
    std::size_t size = 0;               // its bytes in the file (when whole)
    unsigned bits = 16;                 // 16 or 32
};

// What a fix-up patches at one place: its record, and how many bytes.
struct Site {
    std::size_t record = 0;
    std::uint32_t width = 0;
};

// One LE image inside the file (the whole file, or one driver of a W3 bundle), read and decoded.
class Driver {
public:
    Driver(std::shared_ptr<const Bytes> data, std::uint64_t lfa, std::string tag, platforms::DecoderSet& decoders);

    const formats::le::Module& module() const { return module_; }
    // Prefix of the driver's labels: empty for a plain driver, `m<n>_` for the n-th member of a bundle.
    const std::string& tag() const { return tag_; }
    std::span<const Object> objects() const { return objects_; }
    const std::map<Place, x86_flat::Insn>& instructions() const { return insns_; }
    // Where each function begins: the entry points, the descriptor's procedures and every place a call lands.
    const std::set<Place>& function_starts() const { return starts_; }
    const std::map<Place, Site>& sites() const { return sites_; }

    bool has_object(std::uint32_t index) const { return object_at_.contains(index); }
    const Object& object(std::uint32_t index) const { return objects_[object_at_.at(index)]; }

private:
    friend class Image;
    std::size_t page_length(std::uint32_t page) const;
    std::string name(Place at) const;                           // the name of a place of the driver
    void decode_code(platforms::DecoderSet& decoders);
    std::set<Place> find_function_starts(const std::vector<Place>& entries) const;

    std::shared_ptr<const Bytes> data_;
    formats::le::Module module_;
    std::string tag_;
    std::uint32_t last_page_ = 0;
    std::vector<Object> objects_;
    std::map<std::uint32_t, std::size_t> object_at_;            // object number -> its place in objects_
    std::vector<formats::le::FixupRecord> records_;
    std::map<Place, Site> sites_;                                // the place each fix-up patches
    std::vector<Place> site_order_;                              // ... in the order they were found (it decides what is decoded first)
    std::map<Place, x86_flat::Insn> insns_;
    std::map<Place, std::uint32_t> covered_;                     // every byte of an instruction -> where its instruction starts
    std::set<Place> starts_;
    std::map<Place, std::string> placed_;                        // the labelled places of the last writing
};

// The file as one NASM source, found out in rounds (source/flat_rounds).
class Image final : public flat_rounds::FlatImage<Key> {
public:
    // FormatError for a file that is not an LE driver or a W3 bundle of them.
    Image(Bytes file, std::string file_name);
    static Image open(const std::filesystem::path& file);

    ByteView data() const { return *data_; }
    bool is_bundle() const { return bundle_; }
    std::span<const Driver> drivers() const { return drivers_; }
    std::size_t instruction_count() const;
    // The last writing, as one L2 tree (each object, function and table a fragment); build the document with `assembly().document(arena)`.
    const translate::l2::Assembly& assembly() const { return assembly_; }

    flat_rounds::Source<Key> write(const std::set<Key>& raw, bool final) override;
    std::set<Key> differing(ByteView built, const flat_rounds::LabelMap& labels, const std::set<Key>& raw) override;

    // The file's text with `raw` written as bytes. `mark_all`: a label on every instruction (the rounds need them to find where each went);
    // `exact`: the sections stand at their own places in the file, else each follows the one before.
    flat_rounds::Source<Key> source(const std::set<Key>& raw, bool mark_all, bool exact);

private:
    ByteView bytes(std::size_t lo, std::size_t hi) const;       // [lo, hi) of the file, cut to its end
    void place_labels(Driver& driver, bool mark_all);
    std::string ref(const Driver& driver, std::uint32_t object, std::uint32_t offset) const;   // how the source spells a place
    std::string site_value(const Driver& driver, const formats::le::FixupRecord& rec, std::uint32_t width, ByteView at) const;
    std::vector<std::string> text(const Driver& driver, std::uint32_t object, const x86_flat::Insn& insn, bool raw, unsigned bits) const;
    std::optional<std::string> nasm_text(const x86_flat::Insn& insn, unsigned bits, const std::function<std::string(std::uint32_t)>& ref) const;
    void write_object(translate::l2::Assembly& out, std::size_t driver_index, const Object& o, const std::set<Key>& raw,
                      std::map<std::size_t, Key>& lines_of) const;
    void write_data(translate::l2::Assembly& out, const Driver& driver, std::uint32_t object, std::size_t base, std::size_t from, std::size_t to) const;
    void write_fixup_records(translate::l2::Assembly& out, const Driver& driver) const;

    std::shared_ptr<const Bytes> data_;
    std::string file_name_;
    bool bundle_ = false;
    platforms::DecoderSet decoders_;
    std::vector<Driver> drivers_;
    translate::l2::Assembly assembly_;
};

struct Result {
    std::filesystem::path source;
    std::optional<std::filesystem::path> image;   // the file built
    bool same = false;                            // it is the original
    std::size_t rounds = 0;
    std::size_t raw = 0;                          // instructions written as bytes
    std::string log;
    std::size_t instructions = 0;

    flat_rounds::FlatReport report() const;
};

// Write `driver` as <outdir>/<name>.asm (name: its file name in lower case), assembled with NASM, rounds until nothing differs.
// NotFoundError when there is no assembler; FormatError for a file that is not a driver; ToolError when NASM rejects what is not the
// writer's to blame.
Result reconstruct(const std::filesystem::path& driver, const std::filesystem::path& outdir, const Options& options);

// What the command `le-source <driver> [--out DIR]` needs of its surroundings.
struct Environment {
    std::filesystem::path state;     // the default out folder is <state>/le_source/<driver name>
};
// The command line without the command's name; `say` takes every line printed. -> the exit code (2: no driver named).
int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say);

// The command's usage text.
std::string_view usage();

}  // namespace cb::source::le_writer

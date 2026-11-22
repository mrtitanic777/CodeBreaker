// source/driver_writer.hpp -- a virtual device driver as assembly source that builds back into it.
//
//     driver_writer::Result r = driver_writer::reconstruct("vkd.vxd", "out", driver_writer::options_for(open_watcom, probes));
//     r.source, r.image, r.linked, r.built->checks, r.instructions, r.raw, r.rounds
//
// A driver was written in assembly, so assembly is what its source is. What makes assembly of a driver SOURCE rather than a listing is that
// nothing in it is an address: every place the loader patches (a pointer to a variable, a call into another segment, an entry of a jump
// table) is written as the label it refers to, so the file can be assembled, linked at any address and changed. The image says exactly where
// those places are (its fix-up records), so nothing about them is guessed.
//
// The file has one segment per object of the original, with the attributes that object had. Code is found by following it from the
// driver's entry points (control procedure, services, API procedures), from every place a call or jump across objects lands, and from
// pointers into code; everything not reached is written as data. Calls to the system and other drivers (`int 20h` followed by the service
// number) carry the service's name.
//
// An instruction is written as its mnemonic only when the assembler turns that text back into the bytes the original has -- which is checked,
// not assumed (assemblers choose between equivalent encodings): every distinct instruction is first assembled on its own in a probe file and
// its bytes read back from the object file; one that comes back different is written as its bytes, with the mnemonic beside it. Then the whole
// file is assembled and linked and each object compared with the original outside the patched places; an instruction at a place that still
// differs is written as bytes too, until nothing differs.
//
// The file is also built as one L2 tree (translate/l2.hpp's Assembly: each object's head and each function a fragment) and written as
// module.l2.json beside the source.
//
// The VxD of a driver+vxd file (DRVSPACE.BIN) is this writer's: `vxd_writer` fills dos_writer::Options::write_vxd.
//
//     dos_writer::Options o = dos_writer::options_for(open_watcom, probes);
//     o.write_vxd = driver_writer::vxd_writer(driver_writer::options_for(open_watcom, probes, label_of_service));
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "build/driver.hpp"
#include "build/watcom.hpp"
#include "core/bytes.hpp"
#include "formats/le.hpp"
#include "platforms/decoder.hpp"
#include "source/assembly.hpp"
#include "source/dos_writer.hpp"
#include "translate/l2.hpp"

namespace cb::source::driver_writer {

using Log = std::function<void(const std::string&)>;
// The name of a service call (device number, service number): the driver kit's, else a placeholder. The writer asks it for a service the
// driver offers (to name the function) and for every `int 20h` it writes (the comment beside it).
using ServiceLabel = std::function<std::string(std::uint32_t device, std::uint32_t service)>;

// The placeholder the kit's table gives where it has no name: VMM_NNNN for the system, VxD_DDDD_NNNN for another device.
std::string placeholder_label(std::uint32_t device, std::uint32_t service);

struct Options {
    build::driver::Tools tools;          // wasm and wlink (the driver kit's include folder is not needed: nothing here uses its macros)
    std::filesystem::path probes;        // the file what has been probed is kept in (<state>/assembly_probes.json); empty: in the out folder
    Log log;                             // a round's account, when given
    ServiceLabel service_label;          // the kit's names (symbols/driver_services); empty: placeholders
    std::size_t max_rounds = 40;
};

// The tools of an OpenWatcom tree, as drivers are assembled and linked.
Options options_for(const build::watcom::OpenWatcom& open_watcom, const std::filesystem::path& probes, ServiceLabel service_label = {});

// A place in a driver: object (1-based) and offset.
using Place = formats::le::Place;

// What the writer reads of one driver, and writes of it. `reconstruct` drives it; the pieces are public so that what is written can be
// looked at without an assembler.
class Writer {
public:
    // The offsets are signed because a fix-up's place can lie before its object.
    using Off = std::int64_t;

    // One decoded instruction (or the six bytes of a service call).
    struct Instruction {
        Bytes bytes;
        std::string mnemonic;            // "service" for an `int 20h` and its two words
        std::string operands;
        std::optional<Off> target;       // where a jump or call inside the object goes
    };
    // A patched field: its kind as the loader knows it (7: the 32-bit offset itself; 8: its distance from the end of the field) and the place
    // it holds.
    struct Fix {
        std::uint8_t kind = 0;
        std::uint32_t object = 0;
        Off offset = 0;
    };
    struct ServiceCall {
        std::uint32_t device = 0, service = 0;
        bool jump = false;
    };
    struct Object {
        std::uint32_t index = 0, flags = 0, size = 0;
        Bytes data;                                  // as loaded, to its virtual size
        bool code = false;
        bool wide = false;                           // 32-bit code; otherwise real-mode start-up code
        std::set<Off> labels;
        std::map<Off, Fix> fix;                      // offset of a patched field -> where it points
        std::vector<Off> fix_order;                  // ... in the order the image lists them (it decides what is decoded first)
        std::map<Off, Instruction> ins;              // code only
        std::map<Off, ServiceCall> services;         // offset of an `int 20h` -> what it calls
    };
    struct Counts {
        std::size_t instructions = 0, raw = 0, data = 0;
    };
    struct Text {
        std::string text;
        Counts counts;
        std::map<std::size_t, Place> line_of;        // source line (1-based) -> the instruction a mnemonic line came from
    };

    // Reads and decodes `image`. `decoder`: an x86 32-bit decoder opened with detail; it outlives the writer.
    Writer(const formats::le::Module& image, const platforms::Decoder& decoder, ServiceLabel service_label = {});

    const std::map<std::uint32_t, Object>& objects() const { return objects_; }
    std::size_t instruction_count() const;
    // The name of a place. A place nothing can be written AT -- inside a patched field or an instruction, or outside its object altogether --
    // is the nearest place before it that can, plus the distance. Asking adds that place to the object's labels.
    std::string label(std::uint32_t object, Off at);
    // {place: its label} of each place a function begins (the driver's entry points and every place a call lands, where code was found).
    std::map<Place, std::string> function_starts();
    // An instruction as the text the assembler takes, or nothing where it has no such text (a service call, or more than one patched field).
    std::optional<std::string> mnemonic(const Object& object, Off at);
    // The assembly text. `good`: the instructions to write as mnemonics; nullptr writes every one that has a text. The line map says which
    // instruction each mnemonic line came from, so an assembler error on a line names the instruction to write as bytes instead.
    Text source(const std::set<Place>* good = nullptr);
    // The segment classes, one per object, as the linker is told them: (O<n>CODE or O<n>DATA, attributes).
    std::vector<std::pair<std::string, std::string>> classes() const;
    // The instructions whose text assembles to the original's bytes. Jumps and calls inside an object are not probed (their bytes depend on
    // where things are); the build as a whole checks those.
    std::set<Place> probe(assembly::Prober& prober, const std::filesystem::path& outdir);
    // Which instructions written as mnemonics did not come back as the original's bytes in `built`, and which objects differ.
    struct Differences {
        std::set<Place> bad;
        std::vector<std::uint32_t> differing;
    };
    Differences differences(const formats::le::Module& built, const std::set<Place>& good) const;
    // The last writing as one L2 tree's builder (nothing before the first `source`).
    const std::optional<translate::l2::Assembly>& assembly() const { return assembly_; }

private:
    struct Step {
        Instruction instruction;
        std::optional<Off> next;                      // where control goes on, when it does
    };
    struct Followed {
        std::map<Off, Instruction> found;
        std::vector<Off> more;                        // targets in this object
        bool clean = true;                            // reached a clean end
    };
    void decode();
    const std::optional<Step>& step(Object& object, Off at);
    Followed follow(Object& object, Off start);
    std::vector<std::string> raw_lines(const Object& object, Off at);
    const Fix* fix_at(const Object& object, Off at) const;

    const formats::le::Module& image_;
    const platforms::Decoder& decoder_;
    ServiceLabel service_label_;
    std::optional<formats::le::Descriptor> descriptor_;
    std::map<std::uint32_t, Object> objects_;
    std::map<std::uint32_t, std::unordered_map<Off, std::optional<Step>>> memo_;   // lookup only: what decoding at an offset gave
    std::map<Place, std::string> names_;                                            // lookup only
    std::vector<std::pair<std::string, Place>> entries_;                            // the driver's entry points, in the order the image lists them
    bool places_known_ = false;                                                     // `within_` is made
    std::map<Place, Off> within_;                                                   // a place inside an instruction -> where that starts
    std::optional<translate::l2::Assembly> assembly_;
};

struct Result {
    std::filesystem::path source;
    std::optional<std::filesystem::path> image;                 // the driver built
    bool linked = false;
    std::size_t rounds = 0, instructions = 0, raw = 0, data = 0;
    std::vector<std::string> notes;                             // why it did not build, or what the comparison said
    std::optional<std::filesystem::path> definitions;           // the .def the linker was given, when the original has one
    std::optional<build::driver::Comparison> built;             // the built file against the original, once it linked
    std::vector<std::uint32_t> differing_objects;               // objects whose bytes still differ
    std::optional<std::filesystem::path> l2;                    // module.l2.json

    // The module came back: it linked and objects, descriptor, services, exports and kind of module all equal the original's.
    bool same() const { return built.has_value() && built->checks.all(); }
};

// Write a driver's source in `outdir` (made), build it, and make it come back as the original (see the top). `member` picks one of a
// container's drivers (VMM32.VXD, WIN386.EXE). FormatError when the file holds no driver; UsageError for a member it has not got.
Result reconstruct(const std::filesystem::path& module, const std::filesystem::path& outdir, const Options& options, std::size_t member = 0);

// The hook of dos_writer::Options::write_vxd: the driver of a driver+vxd file (DRVSPACE.BIN) written into <outdir> as `reconstruct` does.
// The integrator wires it where a dos_writer::Options is made:
//
//     dos_options.write_vxd = driver_writer::vxd_writer(driver_writer::options_for(open_watcom, probes, service_label));
dos_writer::WriteVxd vxd_writer(Options options);

// What the command `driver-source <driver> [--out DIR]` needs of its surroundings.
struct Environment {
    Options options;
    std::filesystem::path state;                  // the default out folder is <state>/driver_source/<driver name>
    std::function<std::optional<std::filesystem::path>(std::string_view name)> retail;   // a corpus name to its file
};
// The command line without the command's name; `say` takes every line printed. -> the exit code (2: no driver named, 1: one did not come back).
// NotFoundError for a name that is no file and no corpus driver.
int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say);

// The command's usage text.
std::string_view usage();

}  // namespace cb::source::driver_writer

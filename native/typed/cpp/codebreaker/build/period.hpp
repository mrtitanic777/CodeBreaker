// codebreaker/build/period.hpp -- typed C++ port of codebreaker.build.period.
//
// period.py -- compilers of the original's time, as the repair search uses them.
//
//     period::names()                              the period compilers set up on this machine
//     auto p = period::get("xenix-c")              (or "msvc2", "msc6", "msvc152", "msc203"; flags=...)
//     auto built = p->compile(source_text, work)   (bytes, mask) of the code it gives, or None
//                                                  (header=..., function=name: that function's code)
//     for (Unit& unit : p->units())                what there is to compare with: Unit(name, source
//         unit.original                            text or None, (bytes, mask) of the original)
//
// A reconstruction is judged by behaviour with a modern compiler (build/functions). Whether it
// is the source that was WRITTEN is a different question, and only a compiler of the original's
// time can speak to it: the authored source gives the original's bytes back, another source that
// behaves the same does not. verify/authored/repair searches with that distance; this module is the one
// place that knows how each such compiler is run and how its output is read.
//
// A compiler exact/ also runs is not described twice: its profile there (exact/toolchains/NAME.json,
// its files in the settings.json `kits` entry the profile names) is the one description, and this
// module reads it -- exact/toolchain runs the passes.
//
//     msvc2      Microsoft's 32-bit C compiler of 1994 (settings: period_compiler -- the folder of
//                CL.EXE). One function at a time; see verify/authored/match.
//     xenix-c    the C compiler of XENIX for the 8086 (cpp, c0, c1, c2, as) in the revision with which
//                Microsoft's linker and librarian of 1983-84 were compiled: exact's profile "xenixc"
//                (kit: xenix86). One source file at a time; a file's code is one stretch of the
//                executable. The reconstruction of those programs it is compared with is found
//                through settings: xenix_c (the folder holding xcc.py, which knows where its sources
//                and the original executables are).
//     msc6       Microsoft C 6.0a, the 16-bit Windows modules' compiler, in DOSBox-X (settings: msc6,
//     msvc152    dosbox_x); and Visual C++ 1.52c's C/C++ 8.00c (settings: msvc152), which built a few
//                of them. Many sources in one emulator session: build/msc6.
//     msc203     Microsoft C 2.03 of 1984, MC1 then MC2: exact's profile "msc203" (kit: msc203).
//
// `mask` marks the bytes the linker fills in (an address, the distance of a call): they are not
// the compiler's to get right, and are left out of every comparison.
//
// Notes on the port:
//   * The compilers are classes with one interface (`Period`: name, bits, compile, units), as in the Python where they
//     share no base class but the same four members.
//   * `xcc.py` (settings: xenix_c) is a Python program outside CodeBreaker that XenixC imports for two things: its `WORK`
//     folder and its `image()`, the original executable's bytes. C++ cannot import it: `xcc_loader()` is the hook that
//     gives those two (the program sets it; unset, XenixC's compile answers None and units() raises ModuleNotFoundError,
//     as the Python does when the import fails).
//   * `compile`'s `work` is a str for XenixC and Msvc2 and may be None for the 16-bit compilers (an optional here).
//   * `(bytes, mask)` is `Code` (build/msc6.hpp's: std::tuple<py::bytes, py::bytes>).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/build/msc6.hpp"
#include "py.hpp"

namespace codebreaker::exact::toolchain {
class Toolchain;
}

namespace codebreaker::build::period {

using Code = msc6::Code;      // (bytes, mask)

// Something that can be compiled and compared: a function, or a source file.
struct Unit {
    py::str name;
    std::optional<py::str> source;
    std::optional<Code> original;
    int64_t address = 0;
    Unit(const py::str& name, const std::optional<py::str>& source, const std::optional<Code>& original, int64_t address = 0)
        : name(name), source(source), original(original), address(address) {}
};

// The names of the period compilers, in the order `names()` lists them.
extern const py::list<py::str> NAMES;

// What a compiler needs of the Python program xcc.py: its WORK folder and the original executable's image.
struct Xcc {
    py::str WORK;
    std::function<py::bytes()> image;
};
// import xcc (with TARGET = target, and `tools` on the path): set by the program that has xcc.py's equivalent.
std::function<std::shared_ptr<Xcc>(const py::str& tools, const py::str& target)>& xcc_loader();

// The interface of every period compiler.
class Period {
  public:
    virtual ~Period() = default;
    py::str name;
    int64_t bits = 0;
    // -> (bytes, mask) of the code it gives, or None
    virtual std::optional<Code> compile(const py::str& source, const std::optional<py::str>& work = std::nullopt,
                                        const py::str& header = py::str(""), const py::str& function = py::str("")) = 0;
    virtual py::list<Unit> units() { return py::list<Unit>(); }
};

// The XENIX C compiler (exact's profile "xenixc", Microsoft's revision), with the
// reconstruction of LINK and LIB it is compared with (xcc.py's; `target` picks the program).
class XenixC : public Period {
  public:
    XenixC(const py::str& tools, const py::str& target = py::str("link240"));
    py::str tools, target;
    std::optional<Code> compile(const py::str& source, const std::optional<py::str>& work = std::nullopt,
                                const py::str& header = py::str(""), const py::str& function = py::str("")) override;
    // the words the linker fills in (each relocated word's offset in the text)
    static py::bytes _mask(int64_t size, const py::list<int64_t>& fixed);
    // The source files of the reconstruction, each with the stretch of the executable it was compiled into.
    py::list<Unit> units() override;

  private:
    std::shared_ptr<Xcc> _load();     // xcc.py: where the reconstruction's sources and the original executable are
    std::shared_ptr<Xcc> xcc_;
    std::shared_ptr<exact::toolchain::Toolchain> tc_;
};

// Microsoft's 32-bit C compiler of 1994, one function at a time (verify/authored/match).
class Msvc2 : public Period {
  public:
    explicit Msvc2(const py::str& cl, const py::str& flags = py::str("/O2 /Gz"));
    py::str cl, flags;
    py::dict<py::str, py::str> env;
    // -> (bytes, mask) of the function (the one named, or the only one), or None
    std::optional<Code> compile(const py::str& source, const std::optional<py::str>& work = std::nullopt,
                                const py::str& header = py::str(""), const py::str& function = py::str("")) override;
};

// What compile_many takes besides the sources (the Python's **kw).
struct ManyOptions {
    std::optional<py::str> work;
    int64_t timeout = 900;
    std::optional<py::dict<msc6::Key, py::str>> names;
    int64_t batch = msc6::BATCH;
    std::optional<py::list<py::list<msc6::Key>>> together;
};

// Microsoft C 6.0a ("msc6") or the C/C++ 8.00c of Visual C++ 1.52c ("msvc152"), the 16-bit
// Windows modules' compilers (build/msc6: one emulator session for many sources).
class MicrosoftC16 : public Period {
  public:
    explicit MicrosoftC16(const py::str& name, const py::str& flags = py::str("/AM /Gcsw /Os"));
    py::str flags;
    // -> (bytes, mask) of the public function named, else of the first code segment; or None
    std::optional<Code> compile(const py::str& source, const std::optional<py::str>& work = std::nullopt,
                                const py::str& header = py::str(""), const py::str& function = py::str("")) override;
    py::dict<msc6::Key, std::optional<Code>> compile_many(const py::dict<msc6::Key, py::str>& sources, const py::str& header = py::str(""),
                                                          const ManyOptions& kw = ManyOptions());
};

// Microsoft C 2.03 (1984): MC1 then MC2, as exact's profile "msc203" runs them.
class Msc203 : public Period {
  public:
    Msc203();
    // -> the .OBJ's bytes, or None (stem and timeout are not needed: the profile names the
    // file, and the passes run in this process)
    std::optional<py::bytes> object(const py::str& source, const py::str& stem = py::str("cand"), int64_t timeout = 120);
    // -> (bytes, mask) of the public function named, else of the first code segment; or None
    std::optional<Code> compile(const py::str& source, const std::optional<py::str>& work = std::nullopt,
                                const py::str& header = py::str(""), const py::str& function = py::str("")) override;

  private:
    std::shared_ptr<exact::toolchain::Toolchain> tc_;
};

// A work folder of this module (settings: state/period)
py::str _scratch();

// Whether exact's profile `profile` has its kit set up (with these files in it).
bool _kit(const py::str& profile, const py::list<py::str>& files = py::list<py::str>());

// {name: the setting it needs, and whether that is there}
py::dict<py::str, bool> _set_up();

// The period compilers that are set up here.
py::list<py::str> names();

// The period compiler `name` (NAMES): .compile(source, work, header="", function="") ->
// (bytes, mask) or None, .units(), .bits. target: the XENIX reconstruction's (default link240);
// flags: the compiler's options, else its usual ones.
std::shared_ptr<Period> get(const py::str& name, const std::optional<py::str>& target = std::nullopt,
                            const std::optional<py::str>& flags = std::nullopt);

}  // namespace codebreaker::build::period

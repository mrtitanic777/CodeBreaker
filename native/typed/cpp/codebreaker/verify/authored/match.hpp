// codebreaker/verify/authored/match.hpp -- typed C++ of codebreaker.verify.authored.match.
//
// match.py -- does a period compiler turn the reconstruction back into the original's bytes?
//
//     python -m codebreaker match <module> [--flags "/O2 /Gz"] [--try] [--json FILE]
//
// This is NOT the measure of a reconstruction: that is whether it behaves as the original (`measure`). It is a
// second, optional signal. Where a function's reconstructed C, compiled with a compiler of the original's time, comes
// out as the very bytes the original has, the C is as close to what was written as a binary can show.
//
// Each reconstructed function is compiled on its own with a compiler of the original's time, its bytes are read from the
// object file with the places the linker fills in masked out, and compared with the original function's bytes, masked at
// the same places (compare/mask.py): identical / same size / different. `--try` compiles under several sets of switches.
//
// What differs from one processor to the next is in ARCH, one entry each: "x86" (a 32-bit Windows module: the compiler
// `period_compiler` names), "x86-16" (a 16-bit module: Microsoft C 6.0a or 8.00c; match16.cpp drives it), "powerpc" (an
// Xbox 360 image or .xex). On PowerPC what may differ is masked at the level of bits, not bytes.
//
// Typing notes:
//   * A (bytes, mask) pair is `Masked` = (py::bytes, compare::mask::Mask): the mask is a bytes of 0/1 flags for x86 (a
//     bytearray in a few places of the Python, which compare as the same bytes) and a list of the bits left out of each word
//     for PowerPC.  The x86 functions that only ever give a byte mask (object_functions, _trim, original_functions ...) are
//     typed with py::bytes, the PowerPC ones with py::list<int64_t>; ARCH's entries carry the variant.
//   * What the Python takes from modules without a typed port is a hook, set once (the interface x86.hpp / exact use):
//       - platforms.decoder(bits) (capstone): `Decoder` = (bytes, address) -> [Insn]; set_decoder(bits, d). The default
//         is capstone through emulate/capstone_x86 (the capstone.dll the process has loaded, or load_capstone's);
//       - pe_functions.function_spans(path): set_function_spans(fn) -> {start: stop} of the 32-bit module's functions
//         (the Python reads `spans[start][0]`; nothing else of the span is used here);
//     and what `run` / `run_powerpc` / main take from verify/lanes (win32.prepare, powerpc.prepare) is passed in: `Prepared`.
//   * A module's `Prepared` is what the Python reads of the lane's module object m: header, texts, order, by_va (and the
//     PowerPC image).
//   * subprocess.run(env=, timeout=): the runtime has neither; the environment the Python builds with dict(os.environ, ...)
//     is set on the process for the length of the call (xna::run::detail::EnvGuard's twin, below); no timeout is applied
//     (the Python's are 300 and 600 seconds, a hung compiler's last resort).  `compile_function`'s `env` argument is that dict.
//   * ThreadPoolExecutor.map is std::thread over the indices, results in order; an exception goes out of the first index
//     that raised, as map's iteration raises it.
//   * `_cached` pickles (bytes, mask) / None under the cache kind "period" as the Python does (protocol 3, read back from
//     any protocol); a PowerPC result's mask is a list of ints.
//   * The usage `__doc__` is DOC; main() prints it.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/compare/mask.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/symbols/libraries/coff.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::authored::match {

using compare::mask::Mask;
using Masked = std::tuple<py::bytes, Mask>;
using ByteMasked = std::tuple<py::bytes, py::bytes>;
using WordMasked = std::tuple<py::bytes, py::list<int64_t>>;

extern const py::list<py::str> FLAG_SETS;
extern const py::list<py::str> MSC8_FLAG_SETS;
extern const py::list<py::str> POWERPC_FLAG_SETS;
inline constexpr int64_t POWERPC_REV = 3;                 // how a PowerPC object is read (_powerpc_objects): part of the cache key
extern const py::list<py::str> _POWERPC_FLAGS;
inline constexpr int64_t _LI = 0x03FFFFFC, _BD = 0x0000FFFC, _HALF = 0x0000FFFF, _WORD = 0xFFFFFFFF;
extern const py::dict<int64_t, int64_t> _FIELD;           // IMAGE_REL_PPC_* -> which bits of the 4-byte word the linker fills in
extern const py::dict<int64_t, int64_t> _NE_SIZE;         // bytes a 16-bit relocation of each source type covers

// ---- what the Python takes from other modules ---------------------------------------------------------------------
struct Insn {
    int64_t address = 0;
    int64_t size = 0;
    py::str mnemonic;
    py::str op_str;
};
// platforms.decoder(bits).disasm(code, address): the instructions up to the first byte that decodes to none
using Decoder = std::function<py::list<Insn>(const py::bytes& code, int64_t address)>;
Decoder capstone_decoder(int64_t bits);                   // the default: capstone, 16 or 32 bits
void set_decoder(int64_t bits, const Decoder& d);
const Decoder& decoder(int64_t bits);

// pe_functions.function_spans(path): {start: end} of the functions of a 32-bit module
using FunctionSpans = std::function<py::dict<int64_t, int64_t>(const py::str& path)>;
void set_function_spans(const FunctionSpans& f);

// The part of a lane's prepared module `run` / `run_powerpc` read.
struct Prepared {
    py::str header;
    py::dict<py::str, py::str> texts;                     // {function name: its C}
    py::list<py::str> order;                              // the names in the order they are judged
    py::dict<int64_t, py::str> by_va;                     // {address: name}
    py::ref<formats::powerpc_image::Image> image;         // PowerPC only
};
using Prepare = std::function<Prepared(const py::str& module_path, const py::str& work)>;

// dict(os.environ, **overrides) for the length of a scope: the variables are set on the process, then put back.
class EnvGuard {
  public:
    explicit EnvGuard(const py::dict<py::str, py::str>& env);   // sets each variable that is not as the dict has it
    ~EnvGuard();
    EnvGuard(const EnvGuard&) = delete;
    EnvGuard& operator=(const EnvGuard&) = delete;

  private:
    std::vector<std::pair<std::string, std::optional<std::string>>> saved_;
};

// ---- what a compiler made: the functions of its object -------------------------------------------------------------
// CL.EXE of the period compiler, or '' when settings.json names none.
py::str compiler();

// {section: [(offset, name)]} of the symbols an object's code sections say are functions, in order; `also` keeps a symbol.
py::dict<int64_t, py::list<std::tuple<int64_t, py::str>>> _code_symbols(
    const symbols::libraries::coff::Coff& obj,
    const std::function<bool(const symbols::libraries::coff::Symbol&)>& also = nullptr);

// {symbol: (bytes, mask)} for the functions of an x86 COFF object file; mask has 1 for every byte the linker fills in.
py::dict<py::str, ByteMasked> object_functions(const py::bytes& obj);

// {symbol: (bytes, [ignored bits per word])} for the functions of a PowerPC COFF object (a function's symbol says so;
// labels inside one -- $M..., class 6 -- do not).
py::dict<py::str, WordMasked> _powerpc_objects(const py::bytes& obj);

// Without the padding a compiler puts after a function (int 3, nop, zero bytes).
ByteMasked _trim(const py::bytes& code, const py::bytes& mask);
// Without the zero words a compiler pads a function with.
WordMasked _trim_words(const py::bytes& code, const py::list<int64_t>& mask);

// ---- the original's functions --------------------------------------------------------------------------------------
// {address: (bytes, mask)} for functions of a 32-bit module; the mask marks relocated words and the distances of direct
// calls and jumps, which depend on where things are.
py::dict<int64_t, ByteMasked> original_functions(const py::str& path, const py::list<int64_t>& addresses);

// {address: (bytes, mask)} for a 16-bit module's functions: every word a relocation record of its segment names, and the
// distance of every near call and jump to another function. `where` {address: (segment, offset)}, `spans`
// {address: ((start, end), instructions)} as verify/lanes/win16.py makes them.
using Where16 = py::dict<int64_t, std::tuple<int64_t, int64_t>>;
using Spans16 = py::dict<int64_t, std::tuple<std::tuple<int64_t, int64_t>, int64_t>>;
py::dict<int64_t, ByteMasked> _originals_ne(const py::str& module_path, const Where16& where, const Spans16& spans);

// {address: (bytes, [ignored bits per word])} for functions of a PowerPC image: each runs to the length its .pdata record
// gives, else to the next function, and ends before the first zero word; a `b`/`bl` that leaves the function has its
// distance masked.
py::dict<int64_t, WordMasked> _originals_powerpc(const formats::powerpc_image::Image& img, const py::list<int64_t>& addresses);

// ---- the processors ------------------------------------------------------------------------------------------------
// One processor's part of `match`.
//   unit, byteorder     how its code is compared: a byte at a time (a mask flags each byte left out), or as words of `unit`
//                       bytes (a mask names the bits of each word left out)
//   flag_sets           the compiler switches `--try` goes through
//   trim(code, mask)    a function without the padding its compiler puts after it
//   object_functions    (object file bytes) -> {symbol: (bytes, mask)}; empty (None) where the compiler's output is read a
//                       module at a time (build/msc6.py)
//   originals           the original's functions: the arguments are the processor's (a path and addresses; a path, segments
//                       and spans; an image)
using Originals32 = std::function<py::dict<int64_t, ByteMasked>(const py::str& path, const py::list<int64_t>& addresses)>;
using Originals16 = std::function<py::dict<int64_t, ByteMasked>(const py::str& path, const Where16& where, const Spans16& spans)>;
using OriginalsPPC = std::function<py::dict<int64_t, WordMasked>(const formats::powerpc_image::Image& img, const py::list<int64_t>& addresses)>;
using ObjectFunctions = std::function<py::dict<py::str, Masked>(const py::bytes& obj)>;

struct Arch {
    int64_t unit = 1;
    py::str byteorder;
    py::list<py::str> flag_sets;
    std::function<Masked(const py::bytes&, const Mask&)> trim;
    ObjectFunctions object_functions;                      // empty: None
    std::variant<Originals32, Originals16, OriginalsPPC> originals;
};
const py::dict<py::str, Arch>& ARCH();

// ---- comparing -----------------------------------------------------------------------------------------------------
// difflib.SequenceMatcher(None, a, b, autojunk=False).ratio() of two byte strings
double sequence_ratio(const py::bytes& a, const py::bytes& b);

// -> (identical | same size | different -- verify/verdicts.py's byte words, share of bytes equal outside masked places)
std::tuple<py::str, double> compare(const ByteMasked& built, const ByteMasked& original);

// How far a compiled function is from the original, outside the places the linker fills in.
// -> (verdict, number of units -- bytes, or a PowerPC's words -- that differ, total compared, first differing unit or -1).
// When the lengths differ the whole tail counts as differing.
std::tuple<py::str, int64_t, int64_t, int64_t> distance(const Masked& built, const Masked& original, const py::str& arch = py::str("x86"));

// The addresses the module's own relocations point at: every place in it that a word of code or data names by address.
py::set<int64_t> relocation_targets(const py::str& path);

extern const py::re::Pattern _BOUND;
extern const py::re::Pattern _CONSTANT;
extern const py::re::Pattern _RESULT_OF_NOTHING;

// (header, {name: text}) as they are given to a period compiler (see the Python's docstring: dllimport, variables for the
// globals the module's relocations show, void for a function whose every `return` hands back whatever was in EAX).
std::tuple<py::str, py::dict<py::str, py::str>> period_source(const py::str& header, const py::dict<py::str, py::str>& texts,
                                                              const py::set<py::str>& imports, const py::set<int64_t>& targets);

extern const py::re::Pattern _DECLARES;

// The header with only the declarations `text` uses (types and macros are all kept).
py::str needed_header(const py::str& header, const py::str& text);

// ---- compiling -----------------------------------------------------------------------------------------------------
// Compile one function's C with the period compiler. -> (bytes, mask) of the function, or None when it does not compile.
// `header` is the shared types/declarations; `env` the environment of the compiler's process (compiler_env(cl)).
std::optional<ByteMasked> compile_function(const py::str& cl, const py::dict<py::str, py::str>& env, const py::str& header, const py::str& source,
                                           const py::str& name, const py::str& work, const py::str& flags);
std::optional<ByteMasked> _compile_function(const py::str& cl, const py::dict<py::str, py::str>& env, const py::str& header, const py::str& source,
                                            const py::str& name, const py::str& work, const py::str& flags);

// dict(os.environ, PATH=<compiler folder>;PATH, INCLUDE=<compiler folder>\..\INCLUDE)
py::dict<py::str, py::str> compiler_env(const py::str& cl);

// -> {function name: (verdict, share, compiled size, original size)}; a function the compiler rejects has verdict "does not compile"
using Result32 = std::tuple<py::str, double, int64_t, int64_t>;
py::dict<py::str, Result32> run(const py::str& module_path, const py::str& flags, const py::str& work, const Prepare& win32_prepare, int64_t jobs = 4);

// Compile one function on its own with the Xbox 360 compiler. -> (bytes, mask), or None when it does not compile.
std::optional<WordMasked> compile_powerpc(const py::str& cl, const py::str& header, const py::str& text, const py::str& name, const py::str& work,
                                          const py::str& flags);
std::optional<WordMasked> _compile_powerpc(const py::str& cl, const py::str& header, const py::str& text, const py::str& name, const py::str& work,
                                           const py::str& flags);

// -> {name: (verdict, words differing, words, original words)}
using ResultPPC = std::tuple<py::str, int64_t, int64_t, int64_t>;
py::dict<py::str, ResultPPC> run_powerpc(const py::str& module_path, const py::str& flags, const py::str& work, const Prepare& powerpc_prepare,
                                         int64_t jobs = 4);

const py::list<py::str>& _ORDER();                        // V.MATCHES (best first)

// `python -m codebreaker match ...`; `match16_main` is match16.main (the Python imports it for a 16-bit module: the two
// modules import each other).
extern const py::str DOC;
int64_t _main_powerpc(const py::list<py::str>& argv, const Prepare& powerpc_prepare);
int64_t main(const py::list<py::str>& argv, const Prepare& win32_prepare, const Prepare& powerpc_prepare,
             const std::function<int64_t(const py::list<py::str>&)>& match16_main);

}  // namespace codebreaker::verify::authored::match

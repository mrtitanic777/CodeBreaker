// exact/draft/candidates.hpp -- the model's text made the project's source: the engine's names, structures, longs,
// the frame's chars and buffers, the library's helpers, and every rewrite applied to make the candidates a compiler is
// given.
//
// The inverse model prints a function as C in the layout cb/exact/draft/text.hpp reads. The passes of shapes and forms
// rewrite it as the statements the compiler gave; what is left here is what needs the whole function and the
// project: the structs the pointers' fields make, the longs two words make, the frame's filler, the declarations of
// the data and routines the text names, and the list of variants the compiler is to decide between. `candidates` runs
// all of it and gives the sources in the order the prover tries them.
//
// What the passes learn of the function (which bytes are unsigned, which data are arrays, the longs ...) lives in a
// `State` the caller holds for the function (there are no module-level tables) - and what the passes ask of
// the project and its toolchain's profile is a `Settings`.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/ordered_map.hpp"
#include "exact/draft/forms.hpp"
#include "exact/draft/package.hpp"
#include "exact/draft/text.hpp"

namespace cb::exact::draft::candidates {

// ---- what the passes learn -----------------------------------------------------------------------------------------

// How the code reads a frame slot (bp + offset): a byte, a word, or a byte widened with zeros after it is loaded.
enum class SlotWidth { Byte, Word, ZeroExtended };

struct FrameSlot {
    std::int64_t offset = 0;   // from bp: parameters above (+), locals below (-)
    SlotWidth width = SlotWidth::Word;
    friend auto operator<=>(const FrameSlot&, const FrameSlot&) = default;
};

// The places the machine code reads a byte of, the globals read a byte at a time, the frame slots by width, and the
// ranges a loaded register is tested against: what the model's checker mode and the code itself show of widths.
struct ByteFields {
    std::set<text::Site> sites;           // (pointer or register + "*" / "w" / "data", offset)
    std::set<std::string> globals;        // g_xxxx
    std::set<FrameSlot> frame_slots;
    std::set<text::Range> ranges;
};

// What the passes of one function learn that text::FunctionFacts does not hold. The caller sets `fp` (what the model
// read of the 8087's values) before and `facts.longs` (the model's longs); `candidates` clears the tables that belong
// to one variant of the text.
struct State {
    text::FunctionFacts facts;
    std::map<std::string, std::string> struct_pointers;   // data the draft declares a struct pointer: name -> struct tag
    std::set<FrameSlot> frame_slots;                      // the frame slots the code reads or writes, by width
    FpInfo fp;                                            // the function's floating point: types, its value, any
};

// ---- what the passes ask of the project ----------------------------------------------------------------------------

// The project and its toolchain's profile, as candidates reads them.
struct Settings {
    forms::KitContext kit;                                   // the project, the toolchain, its kit and the library's data
    std::optional<forms::FrameRule> frame_order;             // profile "frame_order" (absent: the compiler lays out frames in declaration order)
    std::vector<forms::TableMacroSpec> table_macros;         // profile "table_macros"
    std::vector<forms::DataMacroSpec> data_macros;           // profile "data_macros"
    std::vector<forms::LibraryMacroSpec> macros;             // profile "macros"
    std::vector<std::string> fp_conversions;                 // profile "fp_conversions": the routines that turn the 8087's top into an integer
    std::map<std::string, std::string> long_helpers;         // profile "long_helpers": routine -> C operator
    ByteView image;                                          // the program's image
    std::uint32_t data_segment = 0;                          // where its data segment begins (image offset)
    std::map<std::uint32_t, std::string> data_names;         // the data SYMBOLS names where the name is not the place's own g_XXXX (see data_names_of)
};

// {data offset: name} of the data a SYMBOLS table names (a name of the form g_XXXX at another place may be another
// place's: the program's own names are used by place).
std::map<std::uint32_t, std::string> data_names_of(const SymbolTable& symbols);

// ---- the stage -------------------------------------------------------------------------------------------------

// A source for the compiler, and the machine registers the model left in it (a draft for a person, not the compiler).
struct Candidate {
    std::string source;
    std::vector<std::string> registers_left;
};

// How the registers the model named are to be read: register -> the index of the parameter it carries (an
// argument loaded into it at the start).
using RegisterParams = OrderedMap<std::string, int>;

// The model's text made the project's source, then the variants the compiler is to decide between: as read, without
// its last return (a function of no value), with the locals whose address it takes as the buffers they are - and both;
// each base's readings taken in turn (each base's first, then each one's second ..) so that no base's many variants
// keep another's first readings from the compiler. `names` are the places of the routines the program has names for
// (image offset -> name), `at` where the function is, `regs` the registers a compiler keeps register variables in
// (si, di), `widths` the byte-wide places the code reads (from `byte_fields`), `frame_size` the model's frame.
// State: `state.facts.byte_sites` is what the prover's widths were; the tables that belong to one text are cleared here.
std::vector<Candidate> candidates(const Settings& settings, std::string_view code, std::uint32_t at, const std::map<std::uint32_t, std::string>& names,
                                  const std::vector<std::string>& regs, const RegisterParams& regparam, const ByteFields& widths,
                                  std::uint32_t frame_size, State& state, text::ProjectCaches& caches);

// The model's checker run in its byte-marking mode over the code: the text it prints, as BYTE(x) around the byte-wide
// places. Nothing when the model declines.
class MarkingModel {
public:
    virtual ~MarkingModel() = default;
    virtual std::optional<std::string> marked_c(ByteView code) const = 0;
};

// The fields the code reads or writes a byte of: the model's text with BYTE() marks (nullopt without a checker, or
// when it declines), and the code itself - a byte through a register at an offset, a global read a byte at a time, a
// frame slot by its width, a register loaded and tested against a range (a switch's). 16-bit code.
std::optional<ByteFields> byte_fields(const MarkingModel* model, ByteView code);

// ---- the passes ----------------------------------------------------------------------------------------------------

// The pointers the text goes through to fields (`p->fN`, N its offset) declared pointers to a struct of those fields:
// a char where the code reads a byte (`widths`: the byte-wide places), else an int, and a pad between. Records the
// struct pointers in `state` and the longs' fields in `state.facts.longs`.
std::string structs(std::string_view code, const std::set<text::Site>& widths, State& state);

// The locals and parameters the code only ever reads or writes a byte of (`state.frame_slots`): chars.
std::string frame_chars(std::string_view code, const State& state);

// The locals whose address the text takes (`&vN`) as char arrays, each as long as the frame has room before the next
// local (vN is at bp-N); a store into one goes through an int. Nullopt when no local's address is taken.
std::optional<std::string> buffers(std::string_view code);

// A function whose every return gives dx too (the high half: dx:ax) returns a long.
std::string long_returns(std::string_view code);

// Two words the code takes as one long (x the low word): x declared long, y gone, the pair written x. Records the longs.
std::string long_pairs(std::string_view code, State& state);

// Two longs compared the compiler's way - the high words, then the low ones - left as ifs of the halves.
std::string long_ladders(std::string_view code);

// A register given the place of a local (`r = bp - N`): a char pointer into it.
std::string local_pointers(std::string_view code);

// The draft without its last `return X;` (a function of no value whose last call's result the model kept).
std::optional<std::string> void_variant(std::string_view code);

// The registers a compiler keeps its register variables in made those variables (see the .cpp), `regparam` the
// parameters loaded into them at the start.
std::string register_variables(std::string_view code, const std::vector<std::string>& regs, const RegisterParams& regparam);

// The model's trailing frame filler less two bytes for each register local (a variant: MS C 3.00 gives some register
// locals a slot in the frame). `register_locals_have_slots`: the profile has a frame order.
std::optional<std::string> register_slots(std::string_view code, bool register_locals_have_slots);

// The text with one int local unsigned - a variant each, for the locals compared (the compiler's unsigned jumps).
std::vector<std::string> unsigned_locals(std::string_view code, std::size_t most = 3);

// The register locals declared after the others (a variant).
std::optional<std::string> registers_last(std::string_view code, bool register_locals_have_slots);

// The drafts where a number passed or given is the place of a string in the data: that string.
std::vector<std::string> string_variants(const Settings& settings, std::string_view code);

// The text with the engine's names and the declarations it needs: `at` is where the function is, `names` the routines
// the program has names for.
Candidate localize(const Settings& settings, std::string_view code, std::uint32_t at, const std::map<std::uint32_t, std::string>& names,
                   State& state, text::ProjectCaches& caches);

// The library's long arithmetic as the operator: lmul(X, Y) is (X * Y); an assigning one, almul(&X, Y), is X *= Y.
std::string long_helpers(std::string_view code, const std::map<std::string, std::string>& helpers);

// The library's conversion of the 8087's top to an integer is C's own conversion in an assignment. Records the
// long it makes in `state.fp`.
std::string fp_conversions(std::string_view code, const std::vector<std::string>& routines, State& state);

// The types the 8087's operands showed (`state.fp`): declared where the text names them.
std::string fp_types(std::string_view code, const FpInfo& fp);

// What a filler for the frame's unused part is called: `_pf`, or `_p<its place>` (the frame's order lays it out).
enum class FillerName { Fixed, ByPlace };

// Locals declared for the part of the frame no declared local takes: a filler `char _pf[n]` at the end.
// `registers_count`: a register local has a slot of two bytes.
std::string fill_frame(std::string_view code, std::uint32_t frame_size, bool registers_count = true, FillerName name = FillerName::Fixed);

}  // namespace cb::exact::draft::candidates

// cb/exact/batch.hpp -- many candidates of a function compiled in one run of the compiler.
//
// A run's cost is mostly the compiler starting its passes, not the function: N candidates in one source cost little
// more than one. Each candidate's own names - the function, what it declares extern, the structs it defines - are
// given a suffix of its own (__Q<k>) so that they do not meet; the object's names are given back after; each
// function of the object is held to the program's code as a lone compile's would be.
//
// A candidate the compiler refuses (the error's line says whose) is left out and the rest compiled again; the ones
// left out the caller proves alone (draft/prove's mending). Candidates with preprocessor lines other than #include,
// or a different file comment (MFLAGS) from the rest, are not batched.
//
// The toolchain and the OMF symbol renamer are not refactored yet; `compile_many` takes them as the small interfaces
// below, which the project and the OMF module will fill.
#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::exact::batch {

// A candidate's source cut into the part every candidate of a batch must share and its own text.
struct SplitSource {
    std::string preamble;   // the file comment and #include lines (blank lines dropped)
    std::string body;
};

// The cut, or nullopt when the candidate is not one to batch: a preprocessor line other than #include after the
// preamble, or a text that does not hold together (its errors would run on into the next one's).
std::optional<SplitSource> split(std::string_view source);

// The names a candidate gives the file: its function, what it declares extern, the structs it defines.
std::set<std::string> own_names(std::string_view body);

// The body with each of its own names given the suffix __Q<k> (not inside a string).
std::string renamed(std::string_view body, std::size_t k);

// A name as the compiler's object has it with the batch suffix taken off again.
std::string strip_suffix(std::string_view name);

struct LineSpan {
    std::size_t first = 0, last = 0;   // 1-based lines of the batch source
};

struct BatchSource {
    std::string text;
    std::vector<LineSpan> spans;           // each candidate's lines
    std::vector<std::size_t> candidates;   // the index in the caller's list of each candidate in the batch
};

// The candidates in one source, or nullopt when fewer than two can be batched.
std::optional<BatchSource> build(const std::vector<std::string>& sources);

// The lines the compiler's output blames with an error (a warning is not one).
std::vector<std::size_t> error_lines(std::string_view console);

// ---- what the batch asks of its surroundings --------------------------------------------------------------------

struct CompileOptions {
    std::map<std::string, Bytes> headers;   // the project's headers by name
    std::vector<std::string> include;
    std::optional<std::string> switches;
};

struct CompileOutcome {
    std::optional<Bytes> object;   // the OMF object, if the compiler made one
    std::string console;           // what the compiler printed
};

class Toolchain {
public:
    virtual ~Toolchain() = default;
    virtual CompileOutcome compile(ByteView source, const CompileOptions& options) = 0;
};

// The object with every external and public name nm made fn(nm) (what formats/omf's rename_symbols does).
using SymbolRenamer = std::function<Bytes(ByteView object, const std::function<std::string(std::string_view)>& fn)>;

struct Placement {
    std::size_t source;     // the index in the caller's list
    std::size_t function;   // the function's index among the object's
};

struct Compiled {
    Bytes object;                      // the batch's one object, its names given back
    std::vector<Placement> placed;     // the candidates compiled together
};

// Compile as many of `sources` together as the compiler accepts, leaving out (up to `rounds` times) the ones whose
// lines it blames. Nothing comes back when fewer than two are left; the caller then proves each alone.
std::optional<Compiled> compile_many(Toolchain& toolchain, const CompileOptions& options, const SymbolRenamer& rename,
                                     const std::vector<std::string>& sources, int rounds = 3);

}  // namespace cb::exact::batch

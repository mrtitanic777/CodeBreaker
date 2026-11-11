// cb/exact/linkemu.hpp -- a linker's work done here, as it did it, for a linker that cannot be run (or is the program
// itself).
//
// link24: Microsoft's LINK 2.4x as XENIX C built it (runfile.c), for a small-model program:
//
//   the segments   in the order the modules first name them (crt0's: _TEXT, then the group DGROUP - CRT, _DATA, _BSS,
//                  c_common, STACK); each module's piece of a segment begins where its alignment says, the group on a
//                  paragraph. Names are the linker's without regard to case.
//   the communal   variables (an external with a size; the greatest asked for): c_common. The linker placed them in
//   variables      the order of its table of names, a hash of the names - and most of the names are not known. So
//                  their order is taken from SYMBOLS, the record of where each was found (each must begin where the
//                  one before ends).
//   the run file   a header of 512 bytes with the relocations (the paragraph of DGROUP, each a place in its segment:
//                  the code's or DGROUP's), the segments as far as there is data, the checksum that makes the file's
//                  words add up to FFFF.
//
// The inputs are objects already read (cb/exact/object.hpp: an OMF module or a Version 7 a.out). A failure of the
// link itself (a name defined twice, a name nothing defines, a communal variable SYMBOLS has no place for) is a
// ToolError of tool "link24": the emulated linker is the tool that failed.
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/object.hpp"
#include "cb/exact/symbols.hpp"

namespace cb::exact::linkemu {

struct LinkInput {
    std::string name;      // the module's file name, for messages and the map
    ObjectFile object;
    bool odd = false;      // the code ends on an odd byte (the compiler's filler is not there: the module after it is
                           // byte aligned)
};

// Where one module's piece of a segment went.
struct PlacedPiece {
    std::string segment;           // spelled as the first module that named it spelled it
    std::uint32_t at = 0;          // linker address (image offset; DGROUP-relative data starts at `dgroup`)
    std::uint32_t length = 0;
};

struct ModuleMap {
    std::string name;
    std::vector<PlacedPiece> pieces;   // in the module's own segment order; pieces never placed are omitted
};

struct LinkResult {
    Bytes exe;                         // the run file
    std::uint32_t dgroup = 0;          // where DGROUP begins in the image
    std::vector<ModuleMap> modules;    // where each module's pieces went: a map
};

using Log = std::function<void(std::string_view)>;

// Link the objects as LINK 2.4x did. `symbols` is the project's SYMBOLS (the order of the communal variables is
// taken from it); `log` (optional) hears what disagrees with it and the sizes of the result.
LinkResult link24(const std::vector<LinkInput>& objects, const SymbolTable& symbols, const Log& log = nullptr);

}  // namespace cb::exact::linkemu

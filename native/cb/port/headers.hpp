// cb/port/headers.hpp -- the files the port adds to the program: port.h, proto.h, bss.c and data.c.
//
// port.h is what every file of the port includes first (the C library, the platform's calls, the old compiler's
// structure packing, the names the old linker gave to places in arrays); proto.h declares every function of the
// program; bss.c defines the variables the old build defined without a value; data.c defines the variables the
// headers share, once (a file each for headers that define one tag two ways).
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/port/bss.hpp"
#include "cb/port/rewrite.hpp"

namespace cb::port::pre_ansi {

// Rewrites a piece of text's `long`/`int`/`unsigned` to the fixed-width types as the file `file` needs them (the
// port's width pass, which owns the rules; handed in).
using WidthsFn = std::function<std::string(std::string_view text, std::string_view file, bool int16)>;

struct HeaderInputs {
    const OrderedMap<std::string, Fn>& fns;
    const Files& files;                                                      // the program's files as the port has them now
    const std::vector<std::pair<std::string, std::string>>& data;            // (header, a definition of a variable it declares)
    const std::vector<std::pair<std::string, std::string>>& asm_data;        // (module, definition) of a module of data alone
    const std::map<std::string, std::string>& asm_protos;                    // the program's assembly routines: their C declarations
    const Profile& profile;
    const std::set<std::string>& view_headers;                               // headers of structures some names are members of
    bool int16 = false;
};

struct Headers {
    // port.h, proto.h, bss.c, data.c, data2.c ...: the text, or empty for a file the program has no content for
    OrderedMap<std::string, std::optional<std::string>> files;
    // each file's lines before the rules' fixes (what Finished::origin gives)
    OrderedMap<std::string, std::vector<LineOrigin>> origins;
};

// Makes the headers. `aliases` are the names the old linker gave places in arrays; the BSS plan may add to them (the
// BSS definitions are made here, after port.h's text is). `rules` are applied to each file last.
Headers make_headers(const HeaderInputs& in, Aliases& aliases, BssPlan& bss, const FinishRules& rules, const WidthsFn& widths);

// Whether two headers define a tag alike (one header including the other: the same definition).
bool same_definition(const Files& files, std::string_view tag, std::string_view h1, std::string_view h2);

}  // namespace cb::port::pre_ansi

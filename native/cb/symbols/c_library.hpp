// cb/symbols/c_library.hpp -- the C library functions of a period compiler, as its headers declare them.
//
//     const CLibrary library = CLibrary::load(checkout / "data" / "symbols" / "c_library.json");
//     const Prototype* p = library.find("strcpy");     p->header "string.h", p->result 'p', p->arguments {'p', 'p'}
//     p->result_bytes(4)                               2: the width of the pointer, in bytes, is the model's (2 small and medium, 4 compact and large)
//     p->argument_sizes(4)                             {"4p", "4p"}: the sizes a decompiler description gives a cdecl function's arguments
//
// A program calls `strcpy`; nobody on the program's team wrote it. A function identified as the compiler's library (runtime_library)
// is declared by its header -- the `#include` at the top of the program -- and called by name; what the header says of its arguments
// tells the decompiler how many words each call passes. The kinds: v void, c char, i int, u unsigned, l long, f float, d double,
// p pointer (a function taking any number more says "..." last). A fourth field, where present, is the C type of each pointer
// argument (`,,,FILE *`): a call passing another pointer there is written with that cast.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace cb::symbols::c_library {

struct Prototype {
    std::string header;                  // "string.h"
    char result = 'i';
    std::vector<char> arguments;
    bool variadic = false;               // more arguments follow `arguments`
    std::vector<std::string> pointers;   // each argument's C type where it is a pointer ("FILE *"), "" for the others; empty: not known

    // Bytes the result takes in AX (DX:AX); 0 for void and for a result that is not in those registers (a double).
    std::uint32_t result_bytes(std::uint32_t pointer_bytes) const;
    // One entry for each argument as a description's `F` line gives them: its size in bytes, "4p" for a far pointer. Empty for a
    // variadic function, whose callers pass as many as they please.
    std::vector<std::string> argument_sizes(std::uint32_t pointer_bytes) const;
};

class CLibrary {
public:
    // ParseError for text that is not the table's JSON.
    static CLibrary parse(std::string_view json_text, std::string_view source = "c_library.json");
    // The file; IoError / ParseError.
    static CLibrary load(const std::filesystem::path& file);

    const Prototype* find(std::string_view name) const;
    std::size_t size() const { return functions_.size(); }

private:
    std::map<std::string, Prototype, std::less<>> functions_;
};

}  // namespace cb::symbols::c_library

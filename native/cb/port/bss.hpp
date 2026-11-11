// cb/port/bss.hpp -- the old linker's BSS list: variables given one place after another.
//
// A program built by the old toolchain kept the variables that began without a value in one run of bytes (its BSS
// list), each at the place the sizes in order give it, and some of the program's code counted on it: it passed
// `&g_4e52` to a function that takes a structure's pointer, the variables after it standing for the structure's
// members; it walked off the end of an array into the variable after. A compiler of today lays variables out as it
// likes. BssPlan finds what the original counted on and defines the variables so that it still holds: one structure
// variable for the run that is a structure, one array for the arrays that are one, and the other names places in it.
#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "cb/port/original_program.hpp"

namespace cb::port::pre_ansi {

// A name that stands for a place in another's storage: `where` is the expression that replaces it (`(&buf[3])`,
// `(rec_s.len)`) and `declaration` is the declaration of the variable that owns the place.
struct Alias {
    std::string where;
    std::string declaration;
    friend bool operator==(const Alias&, const Alias&) = default;
};
using Aliases = OrderedMap<std::string, Alias>;

// {variable: its bytes} in the order the old linker placed them.
using BssSizes = OrderedMap<std::string, std::uint64_t>;

// A structure variable made of a run of BSS variables: `rec_s` of `struct rec`, whose members are the variables.
struct BssStruct {
    std::string type;
    std::vector<std::string> names;
};

class BssPlan {
public:
    // `runtime_internals`: names of the old runtime's own variables, which the program does not define.
    BssPlan(OriginalProgram& original, std::vector<std::string> runtime_internals);

    // The BSS list (set from outside: the sizes the old build's map gave).
    void set_sizes(BssSizes sizes);
    const BssSizes& sizes() const { return sizes_; }
    const OrderedMap<std::string, BssStruct>& structs() const { return structs_; }
    const OrderedMap<std::string, std::vector<std::string>>& joined() const { return joined_; }

    // `f(&g_4e52)` where f takes a struct type * and g_4e52 is a variable of the BSS list with those after it at the
    // structure's members' places: the old linker's order made them one structure -- one variable of it, their names
    // its members'. Done once. The headers that define the structures are added to `view_headers`.
    void bss_views(const Files& files, const OrderedMap<std::string, Fn>& fns, Aliases& aliases,
                   std::set<std::string>& view_headers, Notes& notes);

    // A BSS array the program went past the end of into the variable after it (in the old layout: the BSS list's
    // order): the two one array, the second's name a place in it, when they are arrays of one element type. True if
    // they were joined (and then the array's externs get its new size: a rule is added to `decl_rules`).
    bool join_bss(const std::string& var, Aliases& aliases, std::vector<DeclRule>& decl_rules);

    // The definitions of the BSS list's variables no file of the program defines: the type its extern says, an array
    // of as many elements as its old bytes hold. Names of joined arrays are given their places in `aliases`.
    std::vector<std::string> definitions(Aliases& aliases);

private:
    OriginalProgram& original_;
    std::vector<std::string> runtime_internals_;
    BssSizes sizes_;
    OrderedMap<std::string, BssStruct> structs_;
    OrderedMap<std::string, std::vector<std::string>> joined_;
    bool views_done_ = false;
    std::optional<ExternDecls> decls_;

    const ExternDecls& extern_decls();
};

}  // namespace cb::port::pre_ansi

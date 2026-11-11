// omf.h -- Intel/Microsoft OMF-86 object modules and libraries (the 16-bit records and the 32-bit
// ones a compiler of the period writes): one module read whole, a library cut into its modules,
// the records walked, and the few kinds of record the exact lanes write.
//
// The C++ form of codebreaker/formats/omf.py.  An a.out object (XENIX's compiler: exact/aout.py)
// is not OMF and stays with the Python reader (a Fallback); so do patched (a spec read with a
// regular expression) and rename_symbols (a Python function applied to each name).
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "../core/bytes.h"

namespace cbf::omf {

struct Record {
    uint8_t type;
    Bytes payload;                         // without the checksum
};

struct Target {                            // a fix-up's target: ("seg"|"grp"|"ext"|"?", name / index / None)
    std::string kind;
    enum { None, Name, Number } what = None;
    std::string name;
    int64_t number = 0;
};

struct Fixup {
    std::string segment;
    int64_t at;
    std::string kind;                      // off base ptr lo rel, or ?N
    Target target;
    int64_t displacement;
    int64_t location;                      // the 4-bit location field (fixloc)
};

struct Public {
    std::string name;
    std::optional<std::string> segment;
    int64_t offset;
};

struct SegDef {
    std::string name, cls;
    int64_t length, align;
};

// read(data): what omf.read's Obj holds (raw: the caller's own bytes)
struct Object {
    std::vector<Public> pubs;
    std::vector<std::string> ext;
    std::vector<Fixup> fix;
    std::vector<std::pair<std::string, std::vector<std::string>>> groups;     // a dict, in order
    std::vector<std::pair<std::string, std::pair<int64_t, bool>>> comm;       // name -> (size, far)
    std::vector<std::pair<std::string, int64_t>> combine;
    std::optional<std::pair<std::string, int64_t>> start;
    std::vector<std::pair<std::string, std::string>> seg;                     // name -> its bytes
    std::vector<SegDef> segs;
    std::vector<std::pair<std::string, int64_t>> filled;
};

bool is_aout(Bytes data);
std::vector<Record> records(Bytes data);
std::string record(int64_t type, Bytes payload);
std::string name(const std::string &latin1_bytes);
Object read(Bytes data);
std::vector<std::pair<std::string, Bytes>> library(Bytes data);
std::vector<std::string> publics(Bytes data);           // a set's members, in the order first added
std::vector<std::string> externs(Bytes data);
std::string rename_externs(Bytes data, const std::map<std::string, std::string> &mapping);

}  // namespace cbf::omf

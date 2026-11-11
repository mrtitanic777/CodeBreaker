// codebreaker/formats/xbox/xdbf.hpp -- typed C++ from codebreaker.formats.xbox.xdbf.
//
// XDBF / GPD -- Xbox 360 database format reader (gamer profiles, achievements).
//
// XDBF (Xbox DataBase Format) backs .gpd gamer-profile files, PEC, and dashboard
// databases: a header, an entry table, a free-space table, then a data heap.
// Entries are keyed by (namespace, id); the namespace says what the payload is
// (1 achievements, 2 images, 3 settings, 4 title/games-played, 5 strings,
// 6 avatar awards). This is a clean-room reader written to the public/
// documented format -- the profile-data half of what the GPL X360 library does,
// as original code.
//
// Commands:
//     xdbf info    <file.gpd>          header + per-namespace entry counts
//     xdbf list    <file.gpd>          every entry (namespace, id, size)
//     xdbf ach     <file.gpd>          decode the achievement records
//     xdbf extract <file.gpd> [outdir] dump every entry's raw payload
//
// Typing notes:
//   * The reader (xdbf.parse / xdbf.achievements / xdbf.build) is formats.dll's (native/formats/xbox/assets.cc), linked in
//     and asked through cbf_call.  An entry id is a 64-bit number in Python; here int64_t (the reader's answer for an id
//     of 2**63 or more is an OverflowError-class failure: no real profile has one).
//   * The Python's optional `_ov` attribute (edits made so far) is always there, empty at first.
//   * Replaces the hook `xdbf.XDBF` of the formats.xbox package.
//   * main() takes sys.argv[1:]; sys.exit(n) is py::SystemExit(n).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::formats::xbox::xdbf {

extern const py::str DESCRIPTION;
extern const py::bytes MAGIC;
extern const py::dict<int64_t, py::str> NS;

// one decoded achievement record
struct Achievement {
    int64_t id = 0;
    int64_t image = 0;
    int64_t score = 0;
    int64_t flags = 0;
    bool unlocked = false;
    py::str name;
    py::str unlocked_desc;
    py::str locked_desc;
};
template <class F> void for_each_field_mut(Achievement& a, F&& f) {
    f("id", a.id, true);
    f("image", a.image, true);
    f("score", a.score, true);
    f("flags", a.flags, true);
    f("unlocked", a.unlocked, true);
    f("name", a.name, true);
    f("unlocked_desc", a.unlocked_desc, true);
    f("locked_desc", a.locked_desc, true);
}

// (namespace, id, offset, length)
using Entry = std::tuple<int64_t, int64_t, int64_t, int64_t>;

class XDBF {
  public:
    py::bytes d;
    int64_t version = 0;
    int64_t entry_table_len = 0;
    int64_t entry_count = 0;
    int64_t free_table_len = 0;
    int64_t free_count = 0;
    py::list<Entry> entries;
    int64_t data_start = 0;
    py::dict<std::tuple<int64_t, int64_t>, py::bytes> _ov;   // edits made so far, by (namespace, id)

    // FILETIME for a fixed, plausible unlock date (2015-06-15 12:00 UTC).
    static constexpr int64_t UNLOCK_FILETIME = 130788648000000000LL;

    explicit XDBF(const py::bytes& data);

    std::optional<py::bytes> payload(int64_t ns, int64_t eid) const;
    py::bytes _entry_bytes(int64_t off, int64_t length) const;

    // Decode namespace-1 achievement records. Each record: fixed fields
    // then a UTF-16BE name + unlocked-description + locked-description.
    py::list<Achievement> achievements() const;
    py::dict<int64_t, int64_t> counts() const;

    // ---- write side (edit + rebuild) ----------------------------------
    // Current payload for every (ns, id), honouring edits made so far.
    py::dict<std::tuple<int64_t, int64_t>, py::bytes> _payload_map() const;
    // Replace (or add) the payload for an entry; build() serialises it.
    void set_payload(int64_t ns, int64_t eid, const py::bytes& data);
    // Set/clear the 'achieved' flag (0x20000) and stamp the unlock time on
    // the namespace-1 record whose achievement id == ach_id.
    bool unlock_achievement(int64_t ach_id, bool unlock = true);
    int64_t unlock_all();
    // Serialise back to a valid XDBF byte stream (compacted heap). Entry
    // order is preserved; edited/new payloads are honoured.
    py::bytes build() const;
    int64_t extract_all(const py::str& outdir) const;
};

// the command line without the program name (sys.argv[1:]); prints to stdout
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::xdbf

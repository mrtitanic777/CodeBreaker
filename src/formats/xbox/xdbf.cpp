#include "formats/xbox/xdbf.hpp"

#include <algorithm>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "formats/gen/xdbf.hpp"
#include "core/text.hpp"
#include "formats/xbox/utf16.hpp"

namespace cb::formats::xbox::xdbf {

namespace {

constexpr std::string_view kXdbfReader = "xdbf";
constexpr std::size_t kXdbfHeaderSize = 0x18;
constexpr std::size_t kXdbfEntrySize = 0x12;
constexpr std::size_t kXdbfFreeEntrySize = 8;
constexpr std::size_t kXdbfAchievementMin = 0x1C;   // the fixed fields before the strings
constexpr std::size_t kXdbfUnlockTimeAt = 0x14;
constexpr std::size_t kXdbfFlagsAt = 0x10;
constexpr std::uint16_t kXdbfAchievementNamespace = 1;

std::uint32_t xdbf_be32(ByteView v, std::size_t at) {
    return static_cast<std::uint32_t>(v[at]) << 24 | static_cast<std::uint32_t>(v[at + 1]) << 16 | static_cast<std::uint32_t>(v[at + 2]) << 8 | v[at + 3];
}

void xdbf_put_be32(Bytes& v, std::size_t at, std::uint32_t x) {
    for (int i = 0; i < 4; ++i) v[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(x >> (24 - 8 * i));
}

void xdbf_put_be64(Bytes& v, std::size_t at, std::uint64_t x) {
    for (int i = 0; i < 8; ++i) v[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(x >> (56 - 8 * i));
}

}  // namespace

std::string namespace_name(std::uint16_t ns) {
    switch (ns) {
        case 1: return "achievement";
        case 2: return "image";
        case 3: return "setting";
        case 4: return "title";
        case 5: return "string";
        case 6: return "avatar-award";
        case 7: return "avatar-award-1";
        default: return std::format("ns{}", ns);
    }
}

std::optional<Achievement> decode_achievement(ByteView b) {
    if (b.size() < kXdbfAchievementMin) return std::nullopt;
    Achievement a;
    a.id = xdbf_be32(b, 4);
    a.image = xdbf_be32(b, 8);
    a.score = xdbf_be32(b, 0x0C);
    a.flags = xdbf_be32(b, kXdbfFlagsAt);
    a.unlocked = (a.flags & kAchievedFlag) != 0;
    // The strings follow, each ended by a zero unit: split on 16-bit boundaries (a character whose low byte is zero followed by one
    // whose high byte is zero has two zero bytes that are not a terminator).
    const ByteView rest = b.subspan(kXdbfAchievementMin);
    std::string* const fields[] = {&a.name, &a.unlocked_desc, &a.locked_desc};
    std::size_t from = 0;
    for (std::string* field : fields) {
        if (from > rest.size()) break;
        std::size_t end = from;
        while (end + 1 < rest.size() && (rest[end] != 0 || rest[end + 1] != 0)) end += 2;
        const bool terminated = end + 1 < rest.size();
        if (!terminated) end = rest.size();
        *field = from_utf16be(rest.subspan(from, end - from));
        while (!field->empty() && field->back() == '\0') field->pop_back();
        if (!terminated) break;
        from = end + 2;
    }
    return a;
}

// The layout is xdbf.fmt's (its generated reader, formats/gen/xdbf); what a cut-short table or a payload outside the file
// means is this reader's.
Database Database::parse(ByteView file) {
    if (file.size() >= 4 && as_text(file.first(4)) != kMagic) throw FormatError(kXdbfReader, 0, "not an XDBF file (bad magic)");   // (shorter: the read says it is cut short)
    const gen::xdbf::File h = gen::xdbf::read(file);
    Database db;
    db.version = h.version;
    db.entry_table_len = h.entry_table_len;
    db.free_table_len = h.free_table_len;
    db.free_count = h.free_count;
    if (!h.entries) throw TruncatedError(kXdbfReader, kXdbfHeaderSize, std::uint64_t{h.entry_count} * kXdbfEntrySize, file.size() - kXdbfHeaderSize);
    db.entries.reserve(h.entries->size());
    for (const gen::xdbf::Entry& n : *h.entries) {
        Entry e;
        e.ns = n.ns;
        e.id = n.id;
        // A zero-length payload is empty wherever its offset points; any other must lie in the file (not read: outside it).
        if (n.length) {
            const ByteView at = n.payload ? *n.payload : io::checked_range(file, db.data_start() + n.offset, n.length);
            e.payload.assign(at.begin(), at.end());
        }
        db.entries.push_back(std::move(e));
    }
    return db;
}

const Entry* Database::find(std::uint16_t ns, std::uint64_t id) const {
    const auto it = std::find_if(entries.begin(), entries.end(), [&](const Entry& e) { return e.ns == ns && e.id == id; });
    return it == entries.end() ? nullptr : &*it;
}

std::optional<ByteView> Database::payload(std::uint16_t ns, std::uint64_t id) const {
    const Entry* e = find(ns, id);
    if (!e) return std::nullopt;
    return ByteView(e->payload);
}

std::map<std::uint16_t, std::size_t> Database::counts() const {
    std::map<std::uint16_t, std::size_t> out;
    for (const Entry& e : entries) ++out[e.ns];
    return out;
}

std::vector<Achievement> Database::achievements() const {
    std::vector<Achievement> out;
    for (const Entry& e : entries) {
        if (e.ns != kXdbfAchievementNamespace) continue;
        if (auto a = decode_achievement(e.payload)) out.push_back(std::move(*a));
    }
    return out;
}

void Database::set_payload(std::uint16_t ns, std::uint64_t id, Bytes payload) {
    for (Entry& e : entries) {
        if (e.ns == ns && e.id == id) {
            e.payload = std::move(payload);
            return;
        }
    }
    entries.push_back(Entry{ns, id, std::move(payload)});
}

bool Database::unlock_achievement(std::uint32_t id, bool unlock) {
    for (Entry& e : entries) {
        if (e.ns != kXdbfAchievementNamespace || e.payload.size() < kXdbfAchievementMin || xdbf_be32(e.payload, 4) != id) continue;
        std::uint32_t flags = xdbf_be32(e.payload, kXdbfFlagsAt);
        flags = unlock ? (flags | kAchievedFlag) : (flags & ~kAchievedFlag);
        xdbf_put_be32(e.payload, kXdbfFlagsAt, flags);
        xdbf_put_be64(e.payload, kXdbfUnlockTimeAt, unlock ? kUnlockFiletime : 0);
        return true;
    }
    return false;
}

std::size_t Database::unlock_all() {
    std::size_t n = 0;
    for (const Achievement& a : achievements()) {
        if (unlock_achievement(a.id, true)) ++n;
    }
    return n;
}

Bytes Database::build() const {
    const std::uint64_t used = entries.size();
    const std::uint64_t capacity = std::max<std::uint64_t>(entry_table_len, used);
    const std::uint64_t free_capacity = std::max<std::uint32_t>(free_table_len, 1);
    std::uint64_t heap_size = 0;
    for (const Entry& e : entries) heap_size += e.payload.size();
    if (capacity > 0xFFFFFFFFull || heap_size > 0xFFFFFFFFull) throw FormatError(kXdbfReader, 0, "the database is too large to write");

    io::ByteWriter w;
    w.bytes(as_bytes(kMagic));
    w.u32_be(version);
    w.u32_be(static_cast<std::uint32_t>(capacity));
    w.u32_be(static_cast<std::uint32_t>(used));
    w.u32_be(static_cast<std::uint32_t>(free_capacity));
    w.u32_be(1);   // the free table holds one entry: the free space after the heap
    std::uint32_t at = 0;
    for (const Entry& e : entries) {
        w.u16_be(e.ns);
        w.u64_be(e.id);
        w.u32_be(at);
        w.u32_be(static_cast<std::uint32_t>(e.payload.size()));
        at += static_cast<std::uint32_t>(e.payload.size());
    }
    w.zeros(static_cast<std::size_t>(capacity - used) * kXdbfEntrySize);
    w.u32_be(static_cast<std::uint32_t>(heap_size));
    w.u32_be(0);
    w.zeros(static_cast<std::size_t>(free_capacity - 1) * kXdbfFreeEntrySize);
    for (const Entry& e : entries) w.bytes(e.payload);
    return w.take();
}

std::size_t Database::extract_all(const std::filesystem::path& folder) const {
    fs::make_dirs(folder);
    for (const Entry& e : entries) fs::write_file(folder / std::format("{}_{:016X}.bin", namespace_name(e.ns), e.id), e.payload);
    return entries.size();
}

// ---- the command line ----------------------------------------------------------------------------------------------------------

namespace {

std::string xdbf_pad(std::string_view s, std::size_t width) {
    std::string padded(text::utf8_prefix(s, width));
    const std::size_t shown = text::display_width(padded);
    if (shown < width) padded.append(width - shown, ' ');
    return padded;
}

Database xdbf_open(const cli::Args& a) { return Database::parse(fs::read_file(fs::from_text(a.required("file")))); }

void xdbf_write(const Database& db, const std::string& path) {
    fs::write_file(fs::from_text(path), db.build());
    out("wrote {}\n", path);
}

cli::Positional xdbf_file_arg() { return cli::Positional{"file", "the .gpd / .spa database"}; }

}  // namespace

cli::Command command() {
    cli::Command c;
    c.name = "xdbf";
    c.summary = "read and edit Xbox 360 XDBF / GPD databases (profiles, achievements)";
    c.area = "Xbox 360";

    cli::Command info{"info", "the header and the entry count of each namespace", {xdbf_file_arg()}, [](const cli::Args& a) {
        const std::string path = a.required("file");
        const Database db = Database::parse(fs::read_file(fs::from_text(path)));
        out("file        : {}\n", fs::from_text(path).filename().string());
        out("version     : 0x{:X}\n", db.version);
        out("entries     : {} (table {})  free: {}\n", db.entries.size(), db.entry_table_len, db.free_count);
        out("data start  : 0x{:X}\n", db.data_start());
        out("by namespace:\n");
        for (const auto& [ns, n] : db.counts()) out("   {} {}\n", xdbf_pad(namespace_name(ns), 14), n);
        return kExitOk;
    }};
    cli::Command list{"list", "every entry: namespace, id, size", {xdbf_file_arg()}, [](const cli::Args& a) {
        const Database db = xdbf_open(a);
        for (const Entry& e : db.entries) out("  {} id={:016X}  {:>8} bytes\n", xdbf_pad(namespace_name(e.ns), 14), e.id, e.payload.size());
        out("total: {} entries\n", db.entries.size());
        return kExitOk;
    }};
    cli::Command ach{"ach", "decode the achievement records", {xdbf_file_arg()}, [](const cli::Args& a) {
        const std::vector<Achievement> all = xdbf_open(a).achievements();
        std::size_t unlocked = 0;
        std::uint64_t score = 0;
        for (const Achievement& x : all) {
            out("  [{}] {:>5}G  {} | {}\n", x.unlocked ? "*" : " ", x.score, xdbf_pad(x.name, 40), text::utf8_prefix(x.unlocked_desc, 50));
            unlocked += x.unlocked ? 1 : 0;
            score += x.score;
        }
        out("total: {} achievements, {} unlocked, {}G\n", all.size(), unlocked, score);
        return kExitOk;
    }};
    cli::Command extract{"extract", "write every entry's payload to a folder", {xdbf_file_arg(), cli::Positional{"folder", "where (default: <file>_xdbf)", false}}, [](const cli::Args& a) {
        const Database db = xdbf_open(a);
        const std::filesystem::path file = fs::from_text(a.required("file"));
        const std::string folder = a.value("folder").value_or(fs::to_text(std::filesystem::path(file).replace_extension()) + "_xdbf");
        out("extracted {} entries -> {}\n", db.extract_all(fs::from_text(folder)), folder);
        return kExitOk;
    }};
    cli::Command unlock{"unlock", "unlock one achievement and write the database",
                        {xdbf_file_arg(), cli::Positional{"id", "the achievement id"}, cli::Positional{"out", "where to write (default: the file itself)", false}},
                        [](const cli::Args& a) {
        Database db = xdbf_open(a);
        const auto id = a.integer<std::uint32_t>("id");
        const bool found = db.unlock_achievement(*id);
        out("unlocked achievement {}: {}\n", a.required("id"), found ? "ok" : "NOT FOUND");
        xdbf_write(db, a.value("out").value_or(a.required("file")));
        return kExitOk;
    }};
    cli::Command unlock_all{"unlock-all", "unlock every achievement and write the database",
                            {xdbf_file_arg(), cli::Positional{"out", "where to write (default: the file itself)", false}}, [](const cli::Args& a) {
        Database db = xdbf_open(a);
        out("unlocked {} achievements\n", db.unlock_all());
        xdbf_write(db, a.value("out").value_or(a.required("file")));
        return kExitOk;
    }};
    cli::Command set{"set", "replace (or add) one entry's payload from a file and write the database",
                     {xdbf_file_arg(), cli::Positional{"namespace", "the namespace number"}, cli::Positional{"id", "the entry id"},
                      cli::Positional{"payload", "the file holding the new payload"}, cli::Positional{"out", "where to write (default: the file itself)", false}},
                     [](const cli::Args& a) {
        Database db = xdbf_open(a);
        const auto ns = a.integer<std::uint16_t>("namespace");
        const auto id = a.integer<std::uint64_t>("id");
        Bytes payload = fs::read_file(fs::from_text(a.required("payload")));
        const std::size_t size = payload.size();
        db.set_payload(*ns, *id, std::move(payload));
        out("set ns={} id={} ({} bytes)\n", a.required("namespace"), a.required("id"), size);
        xdbf_write(db, a.value("out").value_or(a.required("file")));
        return kExitOk;
    }};
    c.subcommands = {std::move(info), std::move(list), std::move(ach), std::move(extract), std::move(unlock), std::move(unlock_all), std::move(set)};
    return c;
}

}  // namespace cb::formats::xbox::xdbf

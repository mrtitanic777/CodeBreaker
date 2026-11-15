#include "cb/formats/cpc.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::cpc {

namespace {

constexpr std::string_view kReader = "cpc";
constexpr std::size_t kHeader = 128;
constexpr std::size_t kSnaMemory = 0x100;
constexpr std::size_t kDirectoryEntries = 64;

bool starts_with(ByteView d, const char* text) { return d.size() >= std::strlen(text) && std::memcmp(d.data(), text, std::strlen(text)) == 0; }

std::string trimmed(ByteView d) {
    std::string s;
    for (const std::uint8_t b : d) s.push_back(static_cast<char>(b & 0x7F));
    while (!s.empty() && (s.back() == ' ' || s.back() == 0)) s.pop_back();
    return s;
}

// ---- disk images ----------------------------------------------------------------------------------------------------------------

struct Sector {
    std::uint8_t id = 0;
    ByteView data;
};

// The sectors of each track (side 0), in the order they are on the track.
std::vector<std::vector<Sector>> read_tracks(ByteView d) {
    const bool extended = starts_with(d, "EXTENDED");
    if (!extended && !starts_with(d, "MV - CPC")) throw FormatError(kReader, 0, "not a CPC disk image");
    if (d.size() < 0x100) throw TruncatedError(kReader, 0, 0x100, d.size());
    const std::size_t tracks = d[0x30], sides = d[0x31];
    if (tracks == 0 || sides == 0) throw FormatError(kReader, 0x30, "a disk with no tracks");
    std::vector<std::vector<Sector>> out(tracks);
    std::size_t offset = 0x100;
    for (std::size_t t = 0; t < tracks; ++t) {
        for (std::size_t side = 0; side < sides; ++side) {
            const std::size_t size = extended ? std::size_t{d[0x34 + t * sides + side]} * 256 : (std::size_t{d[0x32]} | std::size_t{d[0x33]} << 8);
            const std::size_t at = offset;
            offset += size;
            if (side != 0 || size == 0) continue;
            if (at + 0x100 > d.size() || !starts_with(d.subspan(at), "Track-Info")) throw FormatError(kReader, at, "no track information");
            const std::size_t count = d[at + 0x15];
            std::size_t data_at = at + 0x100;
            for (std::size_t s = 0; s < count; ++s) {
                const std::size_t info = at + 0x18 + s * 8;
                const std::size_t length = extended ? (std::size_t{d[info + 6]} | std::size_t{d[info + 7]} << 8) : (std::size_t{128} << (d[info + 3] & 7));
                if (data_at + length > d.size()) throw TruncatedError(kReader, data_at, length, d.size() - std::min(d.size(), data_at));
                out[t].push_back({d[info + 2], d.subspan(data_at, length)});
                data_at += length;
            }
        }
    }
    return out;
}

std::optional<AmsdosHeader> header_of(ByteView d) {
    if (d.size() < kHeader) return std::nullopt;
    unsigned sum = 0;
    for (std::size_t i = 0; i < 67; ++i) sum += d[i];
    if ((sum & 0xFFFF) != static_cast<unsigned>(d[67] | d[68] << 8)) return std::nullopt;
    AmsdosHeader h;
    h.name = trimmed(d.subspan(1, 8));
    const std::string ext = trimmed(d.subspan(9, 3));
    if (!ext.empty()) h.name += "." + ext;
    h.type = d[18];
    h.load = static_cast<std::uint16_t>(d[21] | d[22] << 8);
    h.length = static_cast<std::uint16_t>(d[24] | d[25] << 8);
    h.exec = static_cast<std::uint16_t>(d[26] | d[27] << 8);
    h.logical_length = static_cast<std::uint32_t>(d[64] | d[65] << 8 | d[66] << 16);
    return h;
}

bool is_binary(const AmsdosHeader& h) { return (h.type == 2 || h.type == 3) && h.length != 0; }

Program from_binary(const AmsdosHeader& h, ByteView file) {
    Program p;
    p.kind = Kind::Binary;
    p.name = h.name;
    p.memory.assign(kMemorySize, 0);
    p.origin = h.load;
    const ByteView body = file.subspan(kHeader);
    const std::size_t room = std::min<std::size_t>(kMemorySize - h.load, std::min<std::size_t>(h.length, body.size()));
    p.image.assign(body.begin(), body.begin() + static_cast<std::ptrdiff_t>(room));
    const std::uint32_t end = p.origin + static_cast<std::uint32_t>(p.image.size());
    const std::uint16_t start = h.exec >= p.origin && h.exec < end ? h.exec : static_cast<std::uint16_t>(p.origin);
    p.entries.push_back({start, "start"});
    return p;
}

bool runs_inside(const AmsdosHeader& x) { return x.exec >= x.load && static_cast<std::uint32_t>(x.exec) < static_cast<std::uint32_t>(x.load) + x.length; }

}  // namespace

std::optional<AmsdosHeader> amsdos_header(ByteView data) { return header_of(data); }

std::vector<DiskFile> disk_files(ByteView data) {
    const std::vector<std::vector<Sector>> tracks = read_tracks(data);
    if (tracks[0].empty()) throw FormatError(kReader, 0, "an unformatted first track");
    std::uint8_t first = 0xFF;
    for (const Sector& s : tracks[0]) first = std::min(first, s.id);
    std::size_t reserved = 1;                                  // IBM format: sectors $01-$08
    if ((first & 0xC0) == 0xC0) reserved = 0;                  // data format: $C1-$C9
    else if ((first & 0xC0) == 0x40) reserved = 2;             // system format: $41-$49
    // Logical sector n: track reserved + n / (sectors per track), the sector whose id is the track's lowest + n % (sectors per track).
    const auto logical = [&](std::size_t n) -> ByteView {
        for (std::size_t t = reserved; t < tracks.size(); ++t) {
            const std::size_t spt = tracks[t].size();
            if (spt == 0) throw FormatError(kReader, 0, "an unformatted track");
            if (n >= spt) {
                n -= spt;
                continue;
            }
            std::uint8_t lowest = 0xFF;
            for (const Sector& s : tracks[t]) lowest = std::min(lowest, s.id);
            for (const Sector& s : tracks[t])
                if (s.id == lowest + n) return s.data;
            throw FormatError(kReader, 0, "a sector missing from its track");
        }
        throw FormatError(kReader, 0, "a block past the last track");
    };
    const auto block = [&](std::size_t b) {
        Bytes out;
        for (std::size_t s = 0; s < 2; ++s) {
            const ByteView v = logical(b * 2 + s);
            out.insert(out.end(), v.begin(), v.end());
        }
        return out;
    };
    Bytes directory;
    for (std::size_t b = 0; b < 2; ++b) {
        const Bytes part = block(b);
        directory.insert(directory.end(), part.begin(), part.end());
    }
    struct Extent {
        unsigned number;
        unsigned records;
        std::vector<std::uint8_t> blocks;
    };
    std::map<std::string, std::vector<Extent>> by_name;
    std::vector<std::string> order;
    for (std::size_t e = 0; e < kDirectoryEntries && (e + 1) * 32 <= directory.size(); ++e) {
        const ByteView entry = ByteView(directory).subspan(e * 32, 32);
        if (entry[0] != 0) continue;   // user 0 only ($E5 is deleted)
        std::string name = trimmed(entry.subspan(1, 8));
        const std::string ext = trimmed(entry.subspan(9, 3));
        if (!ext.empty()) name += "." + ext;
        if (!by_name.contains(name)) order.push_back(name);
        Extent x{static_cast<unsigned>(entry[12] & 0x1F) | static_cast<unsigned>(entry[14]) << 5, entry[15], {}};
        for (std::size_t i = 16; i < 32; ++i)
            if (entry[i] != 0) x.blocks.push_back(entry[i]);
        by_name[name].push_back(std::move(x));
    }
    std::vector<DiskFile> files;
    for (const std::string& name : order) {
        std::vector<Extent>& extents = by_name[name];
        std::sort(extents.begin(), extents.end(), [](const Extent& a, const Extent& b) { return a.number < b.number; });
        DiskFile f;
        f.name = name;
        for (const Extent& x : extents) {
            Bytes raw;
            for (const std::uint8_t b : x.blocks) {
                const Bytes part = block(b);
                raw.insert(raw.end(), part.begin(), part.end());
            }
            raw.resize(std::min<std::size_t>(raw.size(), std::size_t{x.records} * 128));
            f.data.insert(f.data.end(), raw.begin(), raw.end());
        }
        f.header = header_of(f.data);
        files.push_back(std::move(f));
    }
    return files;
}

Program parse(ByteView data, std::string_view extension) {
    if (text::iequals(extension, ".sna")) {
        if (!starts_with(data, "MV - SNA")) throw FormatError(kReader, 0, "not a CPC snapshot");
        if (data.size() >= 0x6D && (data[0x6B] | data[0x6C] << 8) == 0) throw UnsupportedError(kReader, 0x6B, "a chunked (version 3) snapshot, not read yet");
        if (data.size() < kSnaMemory + kMemorySize) throw TruncatedError(kReader, 0, kSnaMemory + kMemorySize, data.size());
        Program p;
        p.kind = Kind::Snapshot;
        Registers r;
        r.f = data[0x11], r.a = data[0x12], r.c = data[0x13], r.b = data[0x14], r.e = data[0x15], r.d = data[0x16], r.l = data[0x17], r.h = data[0x18];
        r.r = data[0x19], r.i = data[0x1A];
        r.ix = static_cast<std::uint16_t>(data[0x1D] | data[0x1E] << 8);
        r.iy = static_cast<std::uint16_t>(data[0x1F] | data[0x20] << 8);
        r.sp = static_cast<std::uint16_t>(data[0x21] | data[0x22] << 8);
        r.pc = static_cast<std::uint16_t>(data[0x23] | data[0x24] << 8);
        r.im = data[0x25];
        p.registers = r;
        p.memory.assign(data.begin() + kSnaMemory, data.begin() + kSnaMemory + kMemorySize);
        p.origin = kSnapshotStart;
        p.image.assign(p.memory.begin() + kSnapshotStart, p.memory.begin() + kSnapshotEnd);
        if (r.pc >= kSnapshotStart && r.pc < kSnapshotEnd) p.entries.push_back({r.pc, "start"});
        return p;
    }
    if (text::iequals(extension, ".dsk")) {
        const std::vector<DiskFile> files = disk_files(data);
        const DiskFile* best = nullptr;
        std::vector<std::string> listing;
        for (const DiskFile& f : files) {
            if (f.header) listing.push_back(std::format("{} type {} load ${:04X} length ${:04X} exec ${:04X}", f.name, f.header->type, f.header->load, f.header->length, f.header->exec));
            else listing.push_back(f.name + " (no header)");
            if (!f.header || !is_binary(*f.header)) continue;
            const AmsdosHeader& h = *f.header;
            if (!best || (runs_inside(h) && !runs_inside(*best->header)) || (runs_inside(h) == runs_inside(*best->header) && h.length > best->header->length)) best = &f;
        }
        if (!best) throw FormatError(kReader, 0, "no AMSDOS binary on the disk");
        Program p = from_binary(*best->header, best->data);
        p.kind = Kind::Disk;
        p.files = std::move(listing);
        return p;
    }
    const std::optional<AmsdosHeader> h = header_of(data);
    if (!h || !is_binary(*h)) throw FormatError(kReader, 0, "no AMSDOS binary header");
    return from_binary(*h, data);
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view(), extension_of(path));
}

bool probe(const std::filesystem::path& path) {
    const std::string ext = extension_of(path);
    if (ext != ".dsk" && ext != ".sna" && ext != ".bin") return false;
    if (ext == ".sna") {
        const std::optional<Bytes> head = read_prefix(path, 0, 8);
        return head && starts_with(*head, "MV - SNA");
    }
    if (ext == ".bin") {
        const std::optional<Bytes> head = read_prefix(path, 0, kHeader);
        if (!head) return false;
        const std::optional<AmsdosHeader> h = header_of(*head);
        return h && is_binary(*h);
    }
    const std::optional<Bytes> head = read_prefix(path, 0, 8);
    if (!head || !(starts_with(*head, "MV - CPC") || starts_with(*head, "EXTENDED"))) return false;
    try {
        const fs::MappedFile file(path);
        (void)parse(file.view(), ".dsk");
        return true;
    } catch (const Error&) {   // isolation boundary: a disk with no binary, or a damaged one, is not a program
        return false;
    }
}

const std::map<std::uint32_t, std::string>& firmware_names() {
    static const std::map<std::uint32_t, std::string> kNames = {
        {0xB900, "KL_U_ROM_ENABLE"}, {0xB903, "KL_U_ROM_DISABLE"}, {0xB906, "KL_L_ROM_ENABLE"}, {0xB909, "KL_L_ROM_DISABLE"},
        {0xB90C, "KL_ROM_RESTORE"}, {0xB90F, "KL_ROM_SELECT"}, {0xB912, "KL_CURR_SELECTION"}, {0xB915, "KL_PROBE_ROM"},
        {0xB918, "KL_ROM_DESELECT"}, {0xB91B, "KL_LDIR"}, {0xB91E, "KL_LDDR"}, {0xB921, "KL_POLL_SYNCHRONOUS"},
        {0xB92A, "KL_SCAN_NEEDED"}, {0xBB00, "KM_INITIALISE"}, {0xBB03, "KM_RESET"}, {0xBB06, "KM_WAIT_CHAR"},
        {0xBB09, "KM_READ_CHAR"}, {0xBB0C, "KM_CHAR_RETURN"}, {0xBB0F, "KM_SET_EXPAND"}, {0xBB12, "KM_GET_EXPAND"},
        {0xBB15, "KM_EXP_BUFFER"}, {0xBB18, "KM_WAIT_KEY"}, {0xBB1B, "KM_READ_KEY"}, {0xBB1E, "KM_TEST_KEY"},
        {0xBB21, "KM_GET_STATE"}, {0xBB24, "KM_GET_JOYSTICK"}, {0xBB27, "KM_SET_TRANSLATE"}, {0xBB2A, "KM_GET_TRANSLATE"},
        {0xBB2D, "KM_SET_SHIFT"}, {0xBB30, "KM_GET_SHIFT"}, {0xBB33, "KM_SET_CONTROL"}, {0xBB36, "KM_GET_CONTROL"},
        {0xBB39, "KM_SET_REPEAT"}, {0xBB3C, "KM_GET_REPEAT"}, {0xBB3F, "KM_SET_DELAY"}, {0xBB42, "KM_GET_DELAY"},
        {0xBB45, "KM_ARM_BREAK"}, {0xBB48, "KM_DISARM_BREAK"}, {0xBB4B, "KM_BREAK_EVENT"}, {0xBB4E, "TXT_INITIALISE"},
        {0xBB51, "TXT_RESET"}, {0xBB54, "TXT_VDU_ENABLE"}, {0xBB57, "TXT_VDU_DISABLE"}, {0xBB5A, "TXT_OUTPUT"},
        {0xBB5D, "TXT_WR_CHAR"}, {0xBB60, "TXT_RD_CHAR"}, {0xBB63, "TXT_SET_GRAPHIC"}, {0xBB66, "TXT_WIN_ENABLE"},
        {0xBB69, "TXT_GET_WINDOW"}, {0xBB6C, "TXT_CLEAR_WINDOW"}, {0xBB6F, "TXT_SET_COLUMN"}, {0xBB72, "TXT_SET_ROW"},
        {0xBB75, "TXT_SET_CURSOR"}, {0xBB78, "TXT_GET_CURSOR"}, {0xBB7B, "TXT_CUR_ENABLE"}, {0xBB7E, "TXT_CUR_DISABLE"},
        {0xBB81, "TXT_CUR_ON"}, {0xBB84, "TXT_CUR_OFF"}, {0xBB87, "TXT_VALIDATE"}, {0xBB8A, "TXT_PLACE_CURSOR"},
        {0xBB8D, "TXT_REMOVE_CURSOR"}, {0xBB90, "TXT_SET_PEN"}, {0xBB93, "TXT_GET_PEN"}, {0xBB96, "TXT_SET_PAPER"},
        {0xBB99, "TXT_GET_PAPER"}, {0xBB9C, "TXT_INVERSE"}, {0xBB9F, "TXT_SET_BACK"}, {0xBBA2, "TXT_GET_BACK"},
        {0xBBA5, "TXT_GET_MATRIX"}, {0xBBA8, "TXT_SET_MATRIX"}, {0xBBAB, "TXT_SET_M_TABLE"}, {0xBBAE, "TXT_GET_M_TABLE"},
        {0xBBB1, "TXT_GET_CONTROLS"}, {0xBBB4, "TXT_STR_SELECT"}, {0xBBB7, "TXT_SWAP_STREAMS"}, {0xBBBA, "GRA_INITIALISE"},
        {0xBBBD, "GRA_RESET"}, {0xBBC0, "GRA_MOVE_ABSOLUTE"}, {0xBBC3, "GRA_MOVE_RELATIVE"}, {0xBBC6, "GRA_ASK_CURSOR"},
        {0xBBC9, "GRA_SET_ORIGIN"}, {0xBBCC, "GRA_GET_ORIGIN"}, {0xBBCF, "GRA_WIN_WIDTH"}, {0xBBD2, "GRA_WIN_HEIGHT"},
        {0xBBD5, "GRA_GET_W_WIDTH"}, {0xBBD8, "GRA_GET_W_HEIGHT"}, {0xBBDB, "GRA_CLEAR_WINDOW"}, {0xBBDE, "GRA_SET_PEN"},
        {0xBBE1, "GRA_GET_PEN"}, {0xBBE4, "GRA_SET_PAPER"}, {0xBBE7, "GRA_GET_PAPER"}, {0xBBEA, "GRA_PLOT_ABSOLUTE"},
        {0xBBED, "GRA_PLOT_RELATIVE"}, {0xBBF0, "GRA_TEST_ABSOLUTE"}, {0xBBF3, "GRA_TEST_RELATIVE"}, {0xBBF6, "GRA_LINE_ABSOLUTE"},
        {0xBBF9, "GRA_LINE_RELATIVE"}, {0xBBFC, "GRA_WR_CHAR"}, {0xBBFF, "SCR_INITIALISE"}, {0xBC02, "SCR_RESET"},
        {0xBC05, "SCR_SET_OFFSET"}, {0xBC08, "SCR_SET_BASE"}, {0xBC0B, "SCR_GET_LOCATION"}, {0xBC0E, "SCR_SET_MODE"},
        {0xBC11, "SCR_GET_MODE"}, {0xBC14, "SCR_CLEAR"}, {0xBC17, "SCR_CHAR_LIMITS"}, {0xBC1A, "SCR_CHAR_POSITION"},
        {0xBC1D, "SCR_DOT_POSITION"}, {0xBC20, "SCR_NEXT_BYTE"}, {0xBC23, "SCR_PREV_BYTE"}, {0xBC26, "SCR_NEXT_LINE"},
        {0xBC29, "SCR_PREV_LINE"}, {0xBC2C, "SCR_INK_ENCODE"}, {0xBC2F, "SCR_INK_DECODE"}, {0xBC32, "SCR_SET_INK"},
        {0xBC35, "SCR_GET_INK"}, {0xBC38, "SCR_SET_BORDER"}, {0xBC3B, "SCR_GET_BORDER"}, {0xBC3E, "SCR_SET_FLASHING"},
        {0xBC41, "SCR_GET_FLASHING"}, {0xBC44, "SCR_FILL_BOX"}, {0xBC4A, "SCR_CHAR_INVERT"}, {0xBC4D, "SCR_HW_ROLL"},
        {0xBC50, "SCR_SW_ROLL"}, {0xBC53, "SCR_UNPACK"}, {0xBC56, "SCR_REPACK"}, {0xBC59, "SCR_ACCESS"},
        {0xBC5C, "SCR_PIXELS"}, {0xBC5F, "SCR_HORIZONTAL"}, {0xBC62, "SCR_VERTICAL"}, {0xBC65, "CAS_INITIALISE"},
        {0xBC68, "CAS_SET_SPEED"}, {0xBC6B, "CAS_NOISY"}, {0xBC6E, "CAS_START_MOTOR"}, {0xBC71, "CAS_STOP_MOTOR"},
        {0xBC74, "CAS_RESTORE_MOTOR"}, {0xBC77, "CAS_IN_OPEN"}, {0xBC7A, "CAS_IN_CLOSE"}, {0xBC7D, "CAS_IN_ABANDON"},
        {0xBC80, "CAS_IN_CHAR"}, {0xBC83, "CAS_IN_DIRECT"}, {0xBC86, "CAS_RETURN"}, {0xBC89, "CAS_TEST_EOF"},
        {0xBC8C, "CAS_OUT_OPEN"}, {0xBC8F, "CAS_OUT_CLOSE"}, {0xBC92, "CAS_OUT_ABANDON"}, {0xBC95, "CAS_OUT_CHAR"},
        {0xBC98, "CAS_OUT_DIRECT"}, {0xBC9B, "CAS_CATALOG"}, {0xBC9E, "CAS_WRITE"}, {0xBCA1, "CAS_READ"},
        {0xBCA4, "CAS_CHECK"}, {0xBCA7, "SOUND_RESET"}, {0xBCAA, "SOUND_QUEUE"}, {0xBCAD, "SOUND_CHECK"},
        {0xBCB0, "SOUND_ARM_EVENT"}, {0xBCB3, "SOUND_RELEASE"}, {0xBCB6, "SOUND_HOLD"}, {0xBCB9, "SOUND_CONTINUE"},
        {0xBCBC, "SOUND_AMPL_ENVELOPE"}, {0xBCBF, "SOUND_TONE_ENVELOPE"}, {0xBCC2, "SOUND_A_ADDRESS"}, {0xBCC5, "SOUND_T_ADDRESS"},
        {0xBCC8, "KL_CHOKE_OFF"}, {0xBCCB, "KL_ROM_WALK"}, {0xBCCE, "KL_INIT_BACK"}, {0xBCD1, "KL_LOG_EXT"},
        {0xBCD4, "KL_FIND_COMMAND"}, {0xBCD7, "KL_NEW_FRAMEFLY"}, {0xBCDA, "KL_ADD_FRAMEFLY"}, {0xBCDD, "KL_DEL_FRAMEFLY"},
        {0xBCE0, "KL_NEW_FAST_TICKER"}, {0xBCE3, "KL_ADD_FAST_TICKER"}, {0xBCE6, "KL_DEL_FAST_TICKER"}, {0xBCE9, "KL_ADD_TICKER"},
        {0xBCEC, "KL_DEL_TICKER"}, {0xBCEF, "KL_INIT_EVENT"}, {0xBCF2, "KL_EVENT"}, {0xBCF5, "KL_SYNC_RESET"},
        {0xBCF8, "KL_DEL_SYNCHRONOUS"}, {0xBCFB, "KL_NEXT_SYNC"}, {0xBCFE, "KL_DO_SYNC"}, {0xBD01, "KL_DONE_SYNC"},
        {0xBD04, "KL_EVENT_DISABLE"}, {0xBD07, "KL_EVENT_ENABLE"}, {0xBD0A, "KL_DISARM_EVENT"}, {0xBD0D, "KL_TIME_PLEASE"},
        {0xBD10, "KL_TIME_SET"}, {0xBD13, "MC_BOOT_PROGRAM"}, {0xBD16, "MC_START_PROGRAM"}, {0xBD19, "MC_WAIT_FLYBACK"},
        {0xBD1C, "MC_SET_MODE"}, {0xBD1F, "MC_SCREEN_OFFSET"}, {0xBD22, "MC_CLEAR_INKS"}, {0xBD25, "MC_SET_INKS"},
        {0xBD28, "MC_RESET_PRINTER"}, {0xBD2B, "MC_PRINT_CHAR"}, {0xBD2E, "MC_BUSY_PRINTER"}, {0xBD31, "MC_SEND_PRINTER"},
        {0xBD34, "MC_SOUND_REGISTER"}, {0xBD37, "MC_JUMP_RESTORE"}, {0xBD3A, "KM_SET_LOCKS"}, {0xBD3D, "KM_FLUSH"},
        {0xBD40, "TXT_ASK_STATE"}, {0xBD43, "GRA_DEFAULT"}, {0xBD46, "GRA_SET_BACK"}, {0xBD49, "GRA_SET_FIRST"},
        {0xBD4C, "GRA_SET_LINE_MASK"}, {0xBD4F, "GRA_FROM_USER"}, {0xBD52, "GRA_FILL"}, {0xBD55, "SCR_SET_POSITION"},
        {0xBD58, "MC_PRINT_TRANSLATION"}, {0xBD5B, "KL_BANK_SWITCH"}, {0xBDF1, "MC_WAIT_PRINTER"}, {0xBDF4, "KM_SCAN_KEYS"},
    };
    return kNames;
}

}  // namespace cb::formats::cpc

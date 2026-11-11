// detect.cc -- what kind of file a file is (codebreaker/formats/detect.py): the new-header
// signature of the MZ family, the lane it goes to, the DOS header, and identify() for any file.
#include <cstdio>
#include <string>

#include "../core/files.h"
#include "../core/questions.h"
#include "../core/text.h"

namespace cbf::detect {
namespace {

const char *const kKnown[] = {"PE", "NE", "LE", "LX", "W3", "W4"};

bool known(Bytes sig) {
    for (const char *k : kKnown)
        if (sig.is(k, 2)) return true;
    return false;
}

// signature_of_bytes(head, read_at): None -> false; the signature (may be "") in sig
struct Reader {
    const std::string *path = nullptr;     // read_at reads this file
    std::string got;
    Bytes at(int64_t off, int64_t n) {
        if (!path || !read_part(*path, static_cast<uint64_t>(off), static_cast<uint64_t>(n), got)) got.clear();
        return Bytes(got);
    }
};

bool signature_of(Bytes head, Reader *rd, std::string &sig) {
    if (head.size() < 0x40 || !head.slice_is(0, 2, "MZ", 2)) return false;
    int64_t off = head.u32(0x3C);
    Bytes s;
    if (off + 2 <= head.size()) s = head.slice(off, off + 2);
    else if (rd && 0 < off && off < 0x10000000) s = rd->at(off, 2);
    sig = known(s) ? s.str() : std::string();
    return true;
}

// signature(path): None when unreadable or not MZ
bool signature_path(const std::string &path, std::string &sig) {
    std::string head;
    if (!read_part(path, 0, 0x40, head)) return false;
    Reader rd;
    rd.path = &path;
    return signature_of(Bytes(head), &rd, sig);
}

void signature_q(Call &c) {
    std::string sig;
    if (signature_path(c.str_arg(0), sig)) c.out.bytes(Bytes(sig));
    else c.out.none();
}

void format_of_q(Call &c) {
    std::string sig;
    if (!signature_path(c.str_arg(0), sig)) c.out.none();
    else c.out.text(sig.empty() ? std::string("MZ") : sig);
}

const char *lane_of(const std::string &sig) {
    if (sig == "PE") return "pe";
    if (sig == "NE") return "ne";
    if (sig == "LE" || sig == "LX") return "le";
    return "dos";
}

void lane_q(Call &c) {
    std::string sig;
    if (!signature_path(c.str_arg(0), sig)) c.out.none();
    else c.out.text(lane_of(sig));
}

void holds_drivers_q(Call &c) {   // (path, bundles)
    std::string sig;
    bool ok = signature_path(c.str_arg(0), sig);
    bool hit = false;
    if (ok) {
        if (sig == "LE" || sig == "LX") hit = true;
        if (c.argc() < 2) {                       // the default: bundles=(b"W3", b"W4")
            if (sig == "W3" || sig == "W4") hit = true;
        } else {
            for (const Value &v : c.arg(1).items)
                if (v.kind == Value::Bytes && v.s == sig) hit = true;
        }
    }
    c.out.boolean(hit);
}

void dos_header_q(Call &c) {
    Bytes d = c.data;
    if (d.size() < 0x1C || !d.slice_is(0, 2, "MZ", 2)) {
        c.out.none();
        return;
    }
    int64_t f[13];
    for (int k = 0; k < 13; ++k) f[k] = d.u16(2 + 2 * k);
    int64_t last = f[0], pages = f[1], nrel = f[2], hdr = f[3], reltab = f[11];
    int64_t image = pages * 512 - (last ? 512 - last : 0) - hdr * 16;
    if (image < 0) image = 0;
    int64_t rest = d.size() - hdr * 16;
    if (rest < 0) rest = 0;
    Out &o = c.out;
    o.begin_dict();
    o.item("bytes_in_last_page", last);
    o.item("pages", pages);
    o.item("relocations", nrel);
    o.item("header_paragraphs", hdr);
    o.item("header_size", hdr * 16);
    o.item("min_alloc", f[4]);
    o.item("max_alloc", f[5]);
    o.item("ss", f[6]);
    o.item("sp", f[7]);
    o.item("checksum", f[8]);
    o.item("ip", f[9]);
    o.item("cs", f[10]);
    o.item("relocation_table", reltab);
    o.item("overlay", f[12]);
    o.item("new_header", d.size() >= 0x40 ? static_cast<int64_t>(d.u32(0x3C)) : 0);
    o.item("load_size", image < rest ? image : rest);
    o.key("relocation_entries");
    o.begin_list();
    for (int64_t i = 0; i < nrel; ++i) {
        if (reltab + i * 4 + 4 > d.size()) break;
        o.begin_tuple();
        o.integer(d.u16(reltab + i * 4 + 2));
        o.integer(d.u16(reltab + i * 4));
        o.end_tuple();
    }
    o.end_list();
    o.end_dict();
}

// ---- identify ------------------------------------------------------------------------------
struct Magic {
    const char *magic;
    size_t n;
    const char *format, *family, *command;
};
#define M(s, f, fam, cmd) {s, sizeof(s) - 1, f, fam, cmd}
const Magic kMagic[] = {
    M("XEX2", "Xbox 360 executable (XEX2)", "xbox360", "xex"),
    M("XEX1", "Xbox 360 executable (XEX1, early kit)", "xbox360", "xex"),
    M("XEX%", "Xbox 360 executable (XEX%, prototype)", "xbox360", "xex"),
    M("XBEH", "original Xbox executable (XBE)", "xbox", "info"),
    M("FATX", "FATX filesystem (Xbox drive / memory unit)", "xbox", "media"),
    M("CISO", "compressed disc image (CSO)", "xbox", "media"),
    M("CCIM", "compressed disc image (CCI)", "xbox", "media"),
    M("XUIZ", "XUI package (XZP)", "xbox360", "media"),
    M("XPR0", "Xbox resource bundle (XPR0)", "xbox", "info"),
    M("XPR2", "Xbox 360 resource bundle (XPR2)", "xbox360", "info"),
    M("XDBF", "Xbox 360 database (XDBF: GPD / SPA)", "xbox360", "info"),
    M("WBND", "XACT wave bank (XWB, big-endian)", "xbox360", "info"),
    M("DNBW", "XACT wave bank (XWB, little-endian)", "xbox", "info"),
    M("XUIB", "compiled XUI scene (XUR)", "xbox360", "info"),
    M("MComprHD", "CHD compressed disc image", "disc", "info"),
    M("BIK", "Bink video", "media", "info"),
    M("DDS ", "DirectDraw Surface texture (DDS)", "media", ""),
    M("MSCF", "Microsoft cabinet (CAB)", "windows", "cab"),
    M("\x7f" "ELF", "ELF executable / object", "unix", ""),
    M("dex\n", "Android DEX bytecode", "android", ""),
    M("\xfe\xed\xfa\xce", "Mach-O (32-bit)", "apple", ""),
    M("\xfe\xed\xfa\xcf", "Mach-O (64-bit)", "apple", ""),
    M("\xce\xfa\xed\xfe", "Mach-O (32-bit)", "apple", ""),
    M("\xcf\xfa\xed\xfe", "Mach-O (64-bit)", "apple", ""),
    M("NES\x1a", "NES ROM (iNES)", "nes", ""),
    M("\xff\x4f", "Xbox 360 NAND flash image", "xbox360", ""),
    M("PK\x03\x04", "ZIP archive (or .jar / .apk / .xlsx)", "archive", ""),
    M("RIFF", "RIFF container (WAV / AVI / XMA)", "media", "info"),
    M("XNBw", "XNA asset (XNB, Windows)", "xna", ""),
    M("XNBx", "XNA asset (XNB, Xbox 360)", "xna", ""),
    M("XNBm", "XNA asset (XNB, Windows Phone)", "xna", ""),
    M("Microsoft C/C++ MSF 7.00", "program database (PDB)", "windows", ""),
    M("Microsoft C/C++ program database 2.00", "program database (PDB 2.0)", "windows", ""),
    M("\x89PNG", "PNG image", "media", ""),
    M("\xff\xd8\xff", "JPEG image", "media", ""),
    M("GIF8", "GIF image", "media", ""),
    M("OggS", "Ogg audio / video", "media", ""),
    M("fLaC", "FLAC audio", "media", ""),
    M("ID3", "MP3 audio", "media", ""),
};
#undef M

struct Content {
    uint32_t type;
    const char *name;
};
const Content kContent[] = {
    {0x00000001, "saved game"}, {0x00000002, "marketplace / DLC content"}, {0x00000003, "publisher"},
    {0x00001000, "Xbox 360 title (installer)"}, {0x00002000, "IPTV DVR"}, {0x00004000, "Xbox 360 title"},
    {0x00005000, "original Xbox title"}, {0x00007000, "Games on Demand"}, {0x00009000, "avatar item"},
    {0x00010000, "profile"}, {0x00020000, "gamer picture"}, {0x00030000, "theme"}, {0x00040000, "cache file"},
    {0x00050000, "storage download"}, {0x00070000, "installer"}, {0x000B0000, "community game"}};

const char *coff_name(uint32_t m) {
    switch (m) {
    case 0x014C: return "x86";
    case 0x8664: return "x86-64";
    case 0x01F2: return "PowerPC big-endian (Xbox 360)";
    case 0x01F0: return "PowerPC";
    case 0x01C0: return "ARM";
    case 0x01C4: return "ARM Thumb-2";
    case 0xAA64: return "ARM64";
    default: return nullptr;
    }
}

void match(Out &o, const std::string &format, const std::string &detail, const std::string &family,
           const std::string &command) {
    o.begin_list();
    o.begin_dict();
    o.item_text("format", format);
    o.item_text("detail", detail);
    o.item_text("family", family);
    o.item_text("command", command);
    o.end_dict();
    o.end_list();
}

std::string lower(std::string s) {
    for (char &ch : s)
        if (ch >= 'A' && ch <= 'Z') ch = static_cast<char>(ch - 'A' + 'a');
    return s;
}
bool ends_with(const std::string &s, const char *suffix) {
    size_t n = std::char_traits<char>::length(suffix);
    return s.size() >= n && s.compare(s.size() - n, n, suffix) == 0;
}

void identify_q(Call &c) {
    const std::string path = c.str_arg(0);
    Out &o = c.out;
    uint64_t size64;
    std::string headbuf;
    if (!file_size(path, size64) || !read_part(path, 0, 0x400, headbuf) || is_dir(path)) {
        o.begin_list();
        o.end_list();
        return;
    }
    const int64_t size = static_cast<int64_t>(size64);
    Bytes head(headbuf);
    Reader rd;
    rd.path = &path;
    char buf[128];
    if (head.slice_is(0, 2, "MZ", 2)) {
        std::string sig;
        if (signature_of(head.slice(0, 0x40), &rd, sig)) {
            if (sig == "PE") {
                int64_t e = head.u32(0x3C);
                Bytes m = rd.at(e + 4, 2);
                if (m.empty()) m = lit("\0\0", 2);
                uint32_t machine = static_cast<uint32_t>(m.exact(2).u16(0));
                const char *cpu = coff_name(machine);
                std::snprintf(buf, sizeof buf, "machine 0x%04X", machine);
                match(o, "PE executable", cpu ? cpu : buf, machine == 0x01F2 ? "xbox360" : "windows", "info");
                return;
            }
            if (sig == "W3" || sig == "W4") {
                match(o, "driver bundle (" + sig + ")", "VxDs stored together", "windows", "info");
                return;
            }
            const char *name = sig == "NE" ? "16-bit Windows (NE)" : sig == "LE" ? "virtual device driver (LE)"
                             : sig == "LX" ? "OS/2 / VxD (LX)" : "DOS program (MZ)";
            match(o, name, "", sig.empty() ? "dos" : "windows", "info");
            return;
        }
    }
    Bytes h4 = head.slice(0, 4);
    if (h4.is("CON ", 4) || h4.is("LIVE", 4) || h4.is("PIRS", 4)) {
        Bytes t = rd.at(0x344, 4);
        std::string ct(4 - (t.size() < 4 ? t.size() : 4), '\0');
        ct += t.str();
        uint32_t type = Bytes(ct).be32(0);
        std::string detail;
        for (const Content &k : kContent)
            if (k.type == type) detail = k.name;
        if (detail.empty()) {
            std::snprintf(buf, sizeof buf, "content type 0x%X", type);
            detail = buf;
        }
        std::string tag = h4.str();
        while (!tag.empty() && tag.back() == ' ') tag.pop_back();
        match(o, "STFS package (" + tag + ")", detail, "xbox360", "media");
        return;
    }
    if (h4.is("\xca\xfe\xba\xbe", 4)) {
        uint32_t n = head.size() >= 8 ? head.be32(4) : 0;
        if (0 < n && n < 30) {
            std::snprintf(buf, sizeof buf, "%u architectures", n);
            match(o, "Mach-O universal binary", buf, "apple", "");
        } else {
            match(o, "Java class file", "", "java", "java");
        }
        return;
    }
    for (const Magic &m : kMagic) {
        if (head.size() >= static_cast<int64_t>(m.n) && head.slice_is(0, static_cast<int64_t>(m.n), m.magic, m.n)) {
            std::string detail;
            if (m.n == 3 && std::memcmp(m.magic, "BIK", 3) == 0 && head.size() > 3) {
                // "codec %r" % chr(head[3])
                uint8_t ch = head.at(3);
                std::string r;
                if (ch == '\'') r = "\"'\"";
                else if (ch == '\\') r = "'\\\\'";
                else if (ch == '\t') r = "'\\t'";
                else if (ch == '\n') r = "'\\n'";
                else if (ch == '\r') r = "'\\r'";
                else if (ch < 0x20 || ch == 0x7F || (ch >= 0x80 && ch < 0xA0)) {
                    std::snprintf(buf, sizeof buf, "'\\x%02x'", ch);
                    r = buf;
                } else if (ch == 0xAD) r = "'\\xad'";
                else {
                    r = "'";
                    put_codepoint(r, ch);
                    r += "'";
                }
                detail = "codec " + r;
            }
            match(o, m.format, detail, m.family, m.command);
            return;
        }
    }
    const int64_t bases[] = {0, 0x10000, 0xFD90000, 0x2080000, 0xFB20000, 0x18300000};
    for (int64_t base : bases)
        if (base + 0x10000 + 20 <= size && rd.at(base + 0x10000, 20).is("MICROSOFT*XBOX*MEDIA", 20)) {
            std::snprintf(buf, sizeof buf, "volume at 0x%llX", static_cast<long long>(base));
            match(o, "Xbox disc image (XDVDFS)", buf, "xbox", "media");
            return;
        }
    const int64_t sectors[3][2] = {{2048, 0}, {2352, 16}, {2352, 24}};
    for (auto &s : sectors)
        if (rd.at(16 * s[0] + s[1] + 1, 5).is("CD001", 5)) {
            std::snprintf(buf, sizeof buf, "%lld-byte sectors", static_cast<long long>(s[0]));
            match(o, "CD image (ISO-9660)", buf, "disc", "media");
            return;
        }
    if (head.size() >= 512 && head.slice_is(510, 512, "\x55\xaa", 2)) {
        bool fat = false;
        uint8_t h0 = head.at(0);
        if (h0 == 0xEB || h0 == 0xE9) {
            uint16_t bps = head.u16(11);
            fat = bps == 512 || bps == 1024 || bps == 2048 || bps == 4096;
        }
        if (!fat) {
            bool flags = true, any = false;
            for (int i = 0; i < 4; ++i) {
                uint8_t f = head.at(0x1BE + 16 * i);
                if (f != 0 && f != 0x80) flags = false;
            }
            if (flags)
                for (int i = 0; i < 4; ++i)
                    if (head.at(0x1BE + 16 * i + 4)) any = true;
            fat = flags && any;
        }
        if (fat) {
            match(o, "disk image (FAT)", size <= 2949120 ? "floppy image" : "hard-disk image", "disc", "media");
            return;
        }
    }
    if ((head.slice_is(0, 1, "\x80", 1) || head.slice_is(0, 1, "\x82", 1)) && head.size() > 3) {
        match(o, "OMF object", "", "dos", "");
        return;
    }
    if (head.slice_is(0, 1, "\xf0", 1)) {
        match(o, "OMF library", "", "dos", "");
        return;
    }
    if (head.size() >= 20) {
        uint32_t machine = head.u16(0), nsec = head.u16(2);
        const char *cpu = coff_name(machine);
        if (cpu && 0 < nsec && nsec < 200) {
            std::snprintf(buf, sizeof buf, "%s, %u sections", cpu, nsec);
            match(o, "COFF object", buf, machine == 0x01F2 ? "xbox360" : "windows", "");
            return;
        }
    }
    std::string lp = lower(path);
    if (ends_with(lp, ".zar")) {
        match(o, "ZArchive (ZAR)", "", "archive", "media");
        return;
    }
    if (ends_with(lp, ".pck")) {
        match(o, "Minecraft LCE package (PCK)", "", "xbox360", "media");
        return;
    }
    o.begin_list();
    o.end_list();
}

}  // namespace

CBF_QUESTIONS(detect) {
    add("detect.signature", signature_q, "bytes?");
    add("detect.format_of", format_of_q, "str?");
    add("detect.lane", lane_q, "str?");
    add("detect.holds_drivers", holds_drivers_q, "bool");
    add("detect.dos_header", dos_header_q, "DosHeader{bytes_in_last_page: u16, pages: u16, relocations: u16, header_paragraphs: u16, header_size: u32, min_alloc: u16, max_alloc: u16, ss: u16, sp: u16, checksum: u16, ip: u16, cs: u16, relocation_table: u16, overlay: u16, new_header: u32, load_size: int, relocation_entries: list[(u16, u16)]}?");
    add("detect.identify", identify_q, "list[Detected{format: str, detail: str, family: str, command: str}]");
}

}  // namespace cbf::detect

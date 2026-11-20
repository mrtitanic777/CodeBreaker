// The recorded cases of the format readers: each reader was given a small retail binary once and what it
// answered was kept; the same binary goes to the native reader here and the same facts are wanted. The fixtures are copied to testdata/frozen. The facts are
// compared as the reader names them (not as a dictionary would be shaped), every one the fixture holds.
//
//   ne_keyboard        KEYBOARD.DRV: segments, entry table, names, references, relocations, resource table
//   le_vmouse          VMOUSE.VXD: header, device block, description, every fix-up
//   pe_notepad         NOTEPAD.EXE: headers, imports, code section, base relocations
//   resources_notepad  NOTEPAD.EXE: its resource directory
//   dos_mode           MODE.COM: kind, packer, entry, image
//   detect_vmouse, detect_mode   the signature, format and lane
//
// keyboard.drv and vmouse.vxd are copies kept beside other tests; notepad.exe and mode.com are read from the Windows 95 corpus (settings.json, "corpus") and
// the test passes without them. A file that is not the recorded one (another SHA-256) is not compared.
#include <algorithm>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "formats/detect.hpp"
#include "formats/dos.hpp"
#include "formats/le.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"

namespace stdfs = std::filesystem;
using cb::Json;
namespace fz = cb::formats;

namespace {

Json frozen_case(const char* name) { return cb::json::load(cb::testing::data_path(std::string("formats/testdata/frozen/") + name + ".json")); }

// The file's path when it is there and is the one that was recorded, else nothing (and a line saying so).
std::optional<stdfs::path> frozen_file(const Json& recorded, const stdfs::path& path) {
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) {
        std::cerr << "frozen formats: " << recorded["file"].get<std::string>() << " is not at " << path.string() << "; skipped\n";
        return std::nullopt;
    }
    if (cb::crypto::to_hex(cb::crypto::sha256(cb::fs::read_file(path))) != recorded["sha256"].get<std::string>()) {
        std::cerr << "frozen formats: " << path.string() << " is not the file that was recorded; skipped\n";
        return std::nullopt;
    }
    return path;
}

std::optional<stdfs::path> frozen_corpus_file(const Json& recorded, const char* relative) {
    // settings.json is found from the source tree upwards (a worktree's is in the main checkout); "corpus" is relative to its folder
    stdfs::path dir = stdfs::path(CB_SOURCE_DIR);
    std::error_code ec;
    for (int i = 0; i < 8 && !dir.empty(); ++i, dir = dir.parent_path()) {
        if (!stdfs::is_regular_file(dir / "settings.json", ec)) continue;
        const Json settings = cb::json::load(dir / "settings.json");
        if (!settings.contains("corpus")) return std::nullopt;
        return frozen_file(recorded, stdfs::weakly_canonical(dir / settings["corpus"].get<std::string>(), ec) / relative);
    }
    std::cerr << "frozen formats: no settings.json found; skipped\n";
    return std::nullopt;
}

std::string frozen_hex_sha256(cb::ByteView bytes) { return cb::crypto::to_hex(cb::crypto::sha256(bytes)); }

}  // namespace

CB_TEST("formats.frozen.ne_keyboard", [] {
    const Json rec = frozen_case("ne_keyboard");
    const auto path = frozen_file(rec, cb::testing::data_path("decompile/testdata/describe/keyboard.drv"));
    if (!path) return;
    const Json& want = rec["output"];
    const fz::ne::Module m = fz::ne::Module::open_file(*path);
    CB_CHECK_EQ(m.module_name(), want["module"].get<std::string>());
    // segments
    const auto segments = m.segments();
    CB_CHECK_EQ(segments.size(), want["segments"].size());
    for (std::size_t i = 0; i < segments.size() && i < want["segments"].size(); ++i) {
        const Json& s = want["segments"][i];
        CB_CHECK_EQ(segments[i].index, s["idx"].get<std::uint16_t>());
        CB_CHECK_EQ(segments[i].file_offset, s["file_off"].get<std::uint64_t>());
        CB_CHECK_EQ(segments[i].length, s["length"].get<std::uint32_t>());
        CB_CHECK_EQ(segments[i].flags, s["flags"].get<std::uint16_t>());
        CB_CHECK_EQ(segments[i].min_alloc, s["minalloc"].get<std::uint32_t>());
        CB_CHECK_EQ(std::string(segments[i].is_data() ? "DATA" : "CODE"), s["kind"].get<std::string>());
    }
    // the entry table, the unused places as well
    const auto entries = m.entries();
    CB_CHECK_EQ(entries.size(), want["entries"].size());
    for (std::size_t i = 0; i < entries.size() && i < want["entries"].size(); ++i) {
        const Json& e = want["entries"][i];
        CB_CHECK_EQ(entries[i].ordinal, e["ordinal"].get<std::uint32_t>());
        const std::string kind = e["kind"].get<std::string>();
        CB_CHECK_EQ(std::string(entries[i].kind == fz::ne::Entry::Kind::Unused ? "UNUSED" : entries[i].kind == fz::ne::Entry::Kind::Moveable ? "MOVEABLE" : "FIXED"), kind);
        if (kind == "UNUSED") continue;
        CB_CHECK_EQ(entries[i].segment, e["seg"].get<int>());
        CB_CHECK_EQ(entries[i].offset, e["offset"].get<int>());
        CB_CHECK_EQ(entries[i].flags, e["flags"].get<int>());
    }
    CB_CHECK(m.export_ordinals() == want["exports"].get<std::vector<std::uint32_t>>());
    const auto names = [&](const std::vector<fz::ne::Name>& got, const Json& wanted) {
        CB_CHECK_EQ(got.size(), wanted.size());
        for (std::size_t i = 0; i < got.size() && i < wanted.size(); ++i) {
            CB_CHECK_EQ(got[i].name, wanted[i][0].get<std::string>());
            CB_CHECK_EQ(got[i].ordinal, wanted[i][1].get<int>());
        }
    };
    names(m.resident_names(), want["resident"]);
    names(m.nonresident_names(), want["nonresident"]);
    CB_CHECK(m.module_refs() == want["refs"].get<std::vector<std::string>>());
    // relocations, per segment that has data
    std::size_t counted = 0, imports = 0, internals = 0;
    for (const auto& s : segments) {
        if (s.file_offset == 0) continue;
        const Json& list = want["relocations"][std::to_string(s.index)];
        const auto relocs = m.relocations(s);
        CB_CHECK_EQ(relocs.size(), list.size());
        for (std::size_t i = 0; i < relocs.size() && i < list.size(); ++i) {
            const Json& r = list[i];
            CB_CHECK_EQ(relocs[i].offset, r["offset"].get<int>());
            CB_CHECK_EQ(relocs[i].source, r["src"].get<int>());
            CB_CHECK_EQ(relocs[i].flags, r["rel_type"].get<int>());   // (the flags byte as stored: the target kind, and 4 for an additive one)
            CB_CHECK_EQ(relocs[i].additive(), r["additive"].get<bool>());
            const std::string kind = r["kind"].get<std::string>();
            if (kind == "IMPORTORD") {
                const auto* t = std::get_if<fz::ne::ImportOrdinalRef>(&relocs[i].target);
                CB_CHECK(t != nullptr);
                if (t) {
                    CB_CHECK_EQ(t->module, r["module"].get<int>());
                    CB_CHECK_EQ(t->ordinal, r["ordinal"].get<int>());
                }
                ++imports;
            } else {
                const auto* t = std::get_if<fz::ne::InternalRef>(&relocs[i].target);
                CB_CHECK(kind == "INTERNAL" && t != nullptr);
                if (t) {
                    CB_CHECK_EQ(t->segment, r["seg"].get<int>());
                    CB_CHECK_EQ(t->entry, r["entry"].get<int>());
                }
                ++internals;
            }
            ++counted;
        }
    }
    CB_CHECK_EQ(counted, std::size_t{64});
    CB_CHECK_EQ(imports, std::size_t{53});
    CB_CHECK_EQ(internals, std::size_t{11});
    // the resource table
    const auto resources = m.resources();
    CB_CHECK_EQ(resources.size(), want["resources"].size());
    for (std::size_t i = 0; i < resources.size() && i < want["resources"].size(); ++i) {
        const Json& r = want["resources"][i];
        CB_CHECK_EQ(resources[i].type, r["type"].get<std::string>());
        CB_CHECK_EQ(resources[i].id, r["id"].get<std::string>());
        CB_CHECK_EQ(resources[i].file_offset, r["off"].get<std::uint64_t>());
        CB_CHECK_EQ(resources[i].length, r["length"].get<std::uint64_t>());
        CB_CHECK_EQ(resources[i].flags, r["flags"].get<int>());
    }
});

CB_TEST("formats.frozen.le_vmouse", [] {
    const Json rec = frozen_case("le_vmouse");
    const auto path = frozen_file(rec, cb::testing::data_path("analyze/testdata/le/vmouse.vxd"));
    if (!path) return;
    const Json& want = rec["output"];
    const fz::le::Module m = fz::le::Module::open(*path);
    const fz::le::Header& h = m.header();
    const Json& wh = want["header"];
    CB_CHECK_EQ(std::string(m.signature()), wh["signature"].get<std::string>());
    CB_CHECK_EQ(h.cpu, wh["cpu"].get<int>());
    CB_CHECK_EQ(h.os, wh["os"].get<int>());
    CB_CHECK_EQ(h.version, wh["module_version"].get<std::uint32_t>());
    CB_CHECK_EQ(h.module_flags, wh["module_flags"].get<std::uint32_t>());
    CB_CHECK_EQ(h.pages, wh["pages"].get<std::uint32_t>());
    CB_CHECK_EQ(h.objects, wh["objects"].get<std::uint32_t>());
    CB_CHECK_EQ(h.page_size, wh["page_size"].get<std::uint32_t>());
    CB_CHECK_EQ(h.last_page_bytes, wh["last_page_bytes"].get<std::uint32_t>());
    CB_CHECK_EQ(h.fixup_size, wh["fixup_section_size"].get<std::uint32_t>());
    CB_CHECK_EQ(h.loader_size, wh["loader_section_size"].get<std::uint32_t>());
    CB_CHECK_EQ(h.data_pages, wh["data_pages_offset"].get<std::uint32_t>());
    CB_CHECK_EQ(h.import_module_count, wh["imported_modules"].get<std::uint32_t>());
    CB_CHECK_EQ(h.eip, wh["entry"].get<std::uint32_t>());
    CB_CHECK_EQ(h.eip_object, wh["entry_object"].get<std::uint32_t>());
    CB_CHECK_EQ(h.esp, wh["stack"].get<std::uint32_t>());
    CB_CHECK_EQ(h.esp_object, wh["stack_object"].get<std::uint32_t>());
    CB_CHECK_EQ(h.device_id, wh["device_id"].get<int>());
    CB_CHECK_EQ(h.kit_version, wh["ddk_version"].get<int>());
    CB_CHECK_EQ(m.description(), want["description"].get<std::string>());
    CB_CHECK_EQ(m.resident_name(), want["name"].get<std::string>());
    CB_CHECK(m.import_modules() == want["imports"].get<std::vector<std::string>>());
    // the device descriptor block
    const auto ddb = m.ddb();
    CB_CHECK(ddb.has_value());
    if (ddb) {
        const Json& d = want["ddb"];
        CB_CHECK_EQ(ddb->name, d["name"].get<std::string>());
        CB_CHECK_EQ(ddb->device_id, d["device_id"].get<int>());
        CB_CHECK_EQ(ddb->major, d["major"].get<int>());
        CB_CHECK_EQ(ddb->minor, d["minor"].get<int>());
        CB_CHECK_EQ(ddb->version, d["version"].get<int>());
        CB_CHECK_EQ(ddb->flags, d["flags"].get<int>());
        CB_CHECK_EQ(ddb->init_order, d["init_order"].get<std::uint32_t>());
        CB_CHECK_EQ(ddb->control_proc, d["control_proc"].get<std::uint32_t>());
        CB_CHECK_EQ(ddb->object, d["obj"].get<std::uint32_t>());
        CB_CHECK_EQ(ddb->rva, d["rva"].get<std::int64_t>());
    }
    // every fix-up: where it is, and the place it points at
    const auto fixups = m.fixups();
    CB_CHECK_EQ(fixups.size(), want["fixups"].size());
    std::size_t matched = 0;
    for (const auto& [place, fixup] : fixups) {
        const std::string key = "(" + std::to_string(place.object) + ", " + std::to_string(place.offset) + ")";
        if (!want["fixups"].contains(key)) {
            CB_CHECK(false);
            continue;
        }
        const Json& f = want["fixups"][key];
        const auto* internal = std::get_if<fz::le::InternalFixup>(&fixup);
        CB_CHECK(internal != nullptr && f["kind"].get<std::string>() == "internal");
        if (!internal) continue;
        CB_CHECK_EQ(internal->object, f["obj"].get<std::uint32_t>());
        CB_CHECK_EQ(internal->source_type, f["srctype"].get<int>());
        CB_CHECK_EQ(internal->target.value_or(0xFFFFFFFFu), f["target"].get<std::uint32_t>());
        ++matched;
    }
    CB_CHECK_EQ(matched, std::size_t{593});
});

CB_TEST("formats.frozen.pe_notepad", [] {
    const Json rec = frozen_case("pe_notepad");
    const auto path = frozen_corpus_file(rec, "WINDOWS/notepad.exe");
    if (!path) return;
    const Json& want = rec["output"];
    const fz::pe::Image image = fz::pe::Image::open(*path);
    CB_CHECK_EQ(image.machine_name(), want["machine"].get<std::string>());
    CB_CHECK_EQ(image.header().is_dll(), want["dll"].get<bool>());
    CB_CHECK_EQ(image.module_name(), want["module"].get<std::string>());
    CB_CHECK_EQ(image.exports().size(), want["exports"].size());
    CB_CHECK(!image.debug_codeview().has_value());
    // imports: slot -> (dll, symbol)
    const auto imports = image.imports();
    CB_CHECK_EQ(imports.size(), want["imports"].size());
    for (const fz::pe::Import& i : imports) {
        const std::string key = std::to_string(i.slot_va);
        CB_CHECK(want["imports"].contains(key));
        if (!want["imports"].contains(key)) continue;
        CB_CHECK_EQ(i.dll, want["imports"][key][0].get<std::string>());
        CB_CHECK_EQ(i.name, want["imports"][key][1].get<std::string>());
    }
    // the code sections: where, how long, what bytes
    const auto code = image.code_sections();
    CB_CHECK_EQ(code.size(), want["code_sections"].size());
    for (std::size_t i = 0; i < code.size() && i < want["code_sections"].size(); ++i) {
        const Json& c = want["code_sections"][i];
        CB_CHECK_EQ(code[i].va, c[0].get<std::uint64_t>());
        CB_CHECK_EQ(code[i].section->name, c[2].get<std::string>());
        const cb::ByteView bytes = image.section_data(*code[i].section);
        CB_CHECK_EQ(bytes.size(), c[1]["len"].get<std::size_t>());
        CB_CHECK_EQ(frozen_hex_sha256(bytes), c[1]["sha256"].get<std::string>());
    }
    // the base relocations: how many, which first
    const auto relocs = image.base_relocs();
    const Json& wr = want["relocs"];
    CB_CHECK_EQ(relocs.size(), wr["count"].get<std::size_t>());
    // (the recorded "head" is a window of twenty in the middle of the list, from entry 396: a summary of a long list)
    const std::size_t window = 396;
    CB_CHECK_EQ(wr["head"].size(), std::size_t{20});
    for (std::size_t i = 0; i < wr["head"].size() && window + i < relocs.size(); ++i) CB_CHECK_EQ(relocs[window + i], wr["head"][i].get<std::uint64_t>());
    CB_CHECK(std::is_sorted(relocs.begin(), relocs.end()));
    CB_CHECK_EQ(relocs.front(), std::uint64_t{4105});   // (the first relocation: checked against the directory read by hand)
    // the optional header
    const fz::pe::OptionalHeader o = image.optional_header();
    const Json& wo = want["optional"];
    CB_CHECK_EQ(o.magic, wo["magic"].get<int>());
    CB_CHECK_EQ(o.checksum, wo["checksum"].get<std::uint32_t>());
    CB_CHECK_EQ(o.code_base, wo["code_base"].get<std::uint32_t>());
    CB_CHECK_EQ(o.code_size, wo["code_size"].get<std::uint32_t>());
    CB_CHECK_EQ(o.dll_characteristics, wo["dll_characteristics"].get<int>());
    CB_CHECK_EQ(o.entry_point, wo["entry_point"].get<std::uint32_t>());
    CB_CHECK_EQ(o.file_alignment, wo["file_alignment"].get<std::uint32_t>());
    CB_CHECK_EQ(o.headers_size, wo["headers_size"].get<std::uint32_t>());
    CB_CHECK_EQ(o.image_base, wo["image_base"].get<std::uint64_t>());
    CB_CHECK_EQ(o.image_size, wo["image_size"].get<std::uint32_t>());
    CB_CHECK_EQ(o.section_alignment, wo["section_alignment"].get<std::uint32_t>());
    CB_CHECK_EQ(o.subsystem, wo["subsystem"].get<int>());
    CB_CHECK_EQ(o.initialized_data_size, wo["initialized_data_size"].get<std::uint32_t>());
    CB_CHECK_EQ(o.uninitialized_data_size, wo["uninitialized_data_size"].get<std::uint32_t>());
    // the build facts: the entry, the timestamp
    const Json& wb = want["build"];
    CB_CHECK_EQ(image.header().entry_rva, wb["entry_rva"].get<std::uint32_t>());
    CB_CHECK_EQ(image.entry_va().value_or(0), wb["entry_va"].get<std::uint64_t>());
    CB_CHECK_EQ(image.header().image_base, wb["image_base"].get<std::uint64_t>());
    CB_CHECK_EQ(image.header().timestamp, wb["timestamp"].get<std::uint32_t>());
});

// The resource directory of NOTEPAD.EXE: every entry's type, name, language and size, in the order of the directory ("<ICON 1 lang=1033 296 bytes>").
CB_TEST("formats.frozen.resources_notepad", [] {
    const Json rec = frozen_case("resources_notepad");
    const auto path = frozen_corpus_file(rec, "WINDOWS/notepad.exe");
    if (!path) return;
    const fz::pe::Image image = fz::pe::Image::open(*path);
    std::vector<std::string> got;
    static const char* const kTypes[] = {"", "CURSOR", "BITMAP", "ICON", "MENU", "DIALOG", "STRING", "FONTDIR", "FONT", "ACCELERATOR", "RCDATA", "MESSAGETABLE",
                                         "GROUP_CURSOR", "", "GROUP_ICON", "", "VERSION"};
    for (const fz::pe::ResourceType& type : image.resource_tree()) {
        const std::string type_name = type.name ? *type.name : (type.id && *type.id < std::size(kTypes) && kTypes[*type.id][0] != 0 ? kTypes[*type.id] : "#" + std::to_string(type.id.value_or(0)));
        for (const fz::pe::ResourceItem& item : type.items) {
            if (!item.file_offset) continue;   // (an entry whose data is not in the file is left out)
            const std::string name = item.name ? "'" + *item.name + "'" : std::to_string(item.id.value_or(0));
            got.push_back("<" + type_name + " " + name + " lang=" + std::to_string(item.language) + " " + std::to_string(item.size) + " bytes>");
        }
    }
    const std::vector<std::string> want = rec["output"].get<std::vector<std::string>>();
    //
    CB_CHECK_EQ(got.size(), want.size());
    std::vector<std::string> a = got, b = want;
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    CB_CHECK(a == b);
    CB_CHECK(got == want);
});

CB_TEST("formats.frozen.dos_mode", [] {
    const Json rec = frozen_case("dos_mode");
    const auto path = frozen_corpus_file(rec, "WINDOWS/COMMAND/mode.com");
    if (!path) return;
    const Json& want = rec["output"];
    const cb::Bytes file = cb::fs::read_file(*path);
    CB_CHECK(fz::dos::kind_of(file, "mode.com").has_value());
    CB_CHECK_EQ(std::string(fz::dos::kind_name(*fz::dos::kind_of(file, "mode.com"))), want["kind"].get<std::string>());
    const fz::dos::Program p = fz::dos::read_program(file, "mode.com");
    const Json& w = want["program"];
    CB_CHECK(!p.packer.has_value() && w["packer"].is_null());
    CB_CHECK_EQ(std::string(fz::dos::kind_name(p.kind)), w["kind"].get<std::string>());
    CB_CHECK_EQ(p.base, w["base"].get<std::uint32_t>());
    CB_CHECK_EQ(p.cs, w["cs"].get<std::uint32_t>());
    CB_CHECK_EQ(p.ip, w["ip"].get<std::uint32_t>());
    CB_CHECK_EQ(p.ss, w["ss"].get<std::uint32_t>());
    CB_CHECK_EQ(p.sp, w["sp"].get<std::uint32_t>());
    CB_CHECK_EQ(p.ds.value_or(0), w["ds"].get<std::uint32_t>());
    CB_CHECK_EQ(p.device_segment, w["device_segment"].get<std::uint32_t>());
    CB_CHECK(p.devices.empty() && p.relocations.empty() && p.notes.empty() && p.overlay.empty());
    CB_CHECK_EQ(p.image.size(), w["image"]["len"].get<std::size_t>());
    CB_CHECK_EQ(frozen_hex_sha256(p.image), w["image"]["sha256"].get<std::string>());
    CB_CHECK_EQ(p.entries.size(), std::size_t{1});
    if (!p.entries.empty()) {
        CB_CHECK_EQ(p.entries[0].address, w["entries"][0][0].get<std::uint32_t>());
        CB_CHECK_EQ(p.entries[0].name, w["entries"][0][1].get<std::string>());
        CB_CHECK_EQ(std::string(p.entries[0].call == fz::dos::Entry::Call::Near ? "near" : "far"), w["entries"][0][2].get<std::string>());
    }
    CB_CHECK_EQ(p.segments.size(), std::size_t{1});
    CB_CHECK_EQ(p.spans.size(), std::size_t{1});
    if (!p.spans.empty()) {
        CB_CHECK_EQ(p.spans[0].address, w["spans"][0][0].get<std::uint32_t>());
        CB_CHECK_EQ(p.spans[0].length, w["spans"][0][1].get<std::uint32_t>());
        CB_CHECK_EQ(p.spans[0].label, w["spans"][0][2].get<std::string>());
    }
});

CB_TEST("formats.frozen.detect_vmouse", [] {
    const Json rec = frozen_case("detect_vmouse");
    const auto path = frozen_file(rec, cb::testing::data_path("analyze/testdata/le/vmouse.vxd"));
    if (!path) return;
    const Json& want = rec["output"];
    CB_CHECK_EQ(cb::formats::detect::format_of(*path).value_or(""), want["format"].get<std::string>());
    const auto lane = cb::formats::detect::lane(*path);
    CB_CHECK(lane.has_value());
    if (lane) CB_CHECK_EQ(std::string(cb::formats::detect::lane_name(*lane)), want["lane"].get<std::string>());
    // the signature, as lower-case hex of its two bytes ("4c45" for "LE")
    const auto header = cb::formats::detect::new_header(*path);
    CB_CHECK(header.has_value());
    if (header) {
        const std::string sig = cb::formats::detect::signature_text(*header);
        std::string hex;
        for (const char c : sig) hex += std::format("{:02x}", static_cast<unsigned>(static_cast<unsigned char>(c)));
        CB_CHECK_EQ(hex, want["signature"].get<std::string>());
    }
});

// MODE.COM is a plain DOS program: no new-style header, so no format, no lane and no signature.
CB_TEST("formats.frozen.detect_mode", [] {
    const Json rec = frozen_case("detect_mode");
    const auto path = frozen_corpus_file(rec, "WINDOWS/COMMAND/mode.com");
    if (!path) return;
    CB_CHECK(rec["output"]["format"].is_null() && rec["output"]["lane"].is_null() && rec["output"]["signature"].is_null());
    CB_CHECK(!cb::formats::detect::format_of(*path).has_value());
    CB_CHECK(!cb::formats::detect::lane(*path).has_value());
    const auto header = cb::formats::detect::new_header(*path);
    CB_CHECK(!header.has_value() || *header == cb::formats::detect::NewHeader::None);
});

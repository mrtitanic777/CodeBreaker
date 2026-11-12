// cb-unity: alone (file-local helpers named for this module)
#include <algorithm>
#include <format>
#include <set>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/source/vxd/disassembly.hpp"

namespace cb::source::vxd {

namespace le = formats::le;

namespace {

const le::InternalFixup* internal_fixup(const std::optional<le::Fixup>& f) { return f ? std::get_if<le::InternalFixup>(&*f) : nullptr; }

std::optional<Place> internal_target(const std::optional<le::Fixup>& f) {
    const le::InternalFixup* i = internal_fixup(f);
    if (i == nullptr || !i->target) return std::nullopt;
    return Place{i->object, static_cast<std::int64_t>(*i->target)};
}

std::string hex_of(ByteView bytes) {
    std::string out;
    for (const std::uint8_t b : bytes) out += std::format("{:02x}", b);
    return out;
}

const std::string* label_of(const Disassembly& code, const Place& at) {
    const auto found = code.labels().find(at);
    return found == code.labels().end() || found->second.empty() ? nullptr : &found->second;
}

// A fix-up written as the symbol it names: its label, else obj<N>:0x<offset>; an import as DEVICE.Service_<n>h.
std::string fixup_symbol(const Disassembly& code, const le::Fixup& f) {
    if (const le::InternalFixup* i = std::get_if<le::InternalFixup>(&f)) {
        if (i->target) {
            if (const std::string* label = label_of(code, Place{i->object, static_cast<std::int64_t>(*i->target)})) return *label;
        }
        return std::format("obj{}:0x{:04X}", i->object, i->target.value_or(0));
    }
    const le::ImportFixup& imp = std::get<le::ImportFixup>(f);
    const auto device = device_name(imp.module);
    return std::format("{}.Service_{:04X}h", device ? std::string(*device) : std::format("Mod_{:04X}", imp.module), imp.ordinal);
}

// A data span [start, end): the DDB structured, jump tables as `dd loc_X`, dword fix-ups as symbol pointers, the rest as raw bytes.
std::vector<std::string> data_lines(const Disassembly& code, std::uint32_t object, std::int64_t start, std::int64_t end) {
    std::vector<std::string> out;
    const ByteView bytes = code.code(object);
    const auto& db = code.ddb();
    const auto is_ddb_start = [&](std::int64_t at) { return db && object == db->object && at == db->rva; };
    const auto fixup_pointer = [&](std::int64_t at) { return internal_fixup(code.fixup_at(Place{object, at})) != nullptr; };
    std::int64_t a = start;
    while (a < end) {
        if (const std::string* label = label_of(code, Place{object, a})) out.push_back(*label + ":");
        if (is_ddb_start(a)) {   // the DDB itself
            out.push_back("        ; ---- Device Descriptor Block ----");
            out.push_back("        dd  0                          ; DDB_Next");
            out.push_back(std::format("        dw  0{:04X}h                      ; DDB_SDK_Version", db->version));
            out.push_back(std::format("        dw  0{:04X}h                      ; DDB_Req_Device_Number", db->device_id));
            out.push_back(std::format("        db  {}, {}                       ; DDB_Dev_Major/Minor", db->major, db->minor));
            out.push_back(std::format("        dw  0{:04X}h                      ; DDB_Flags", db->flags));
            out.push_back(std::format("        db  '{:<8}'                  ; DDB_Name", db->name));
            out.push_back(std::format("        dd  0{:08X}h                  ; DDB_Init_Order", db->init_order));
            for (const auto& [field, name] : {std::pair{kDdbControlProc, "DDB_Control_Proc"}, std::pair{kDdbV86ApiProc, "DDB_V86_API_Proc"},
                                              std::pair{kDdbPmApiProc, "DDB_PM_API_Proc"}, std::pair{kDdbServiceTable, "DDB_Service_Table_Ptr"}}) {
                const auto f = code.fixup_at(Place{object, db->rva + field});
                out.push_back(std::format("        dd  {:<24} ; {}", f ? fixup_symbol(code, *f) : std::string("0"), name));
            }
            a = db->rva + 0x38;   // (the rest of the DDB's 0x54 bytes follow as raw bytes)
            continue;
        }
        if (const auto table = code.jump_tables().find(Place{object, a}); table != code.jump_tables().end() && !table->second.empty()) {   // a jump or pointer table
            out.push_back(std::format("        ; ---- dispatch table ({} entries) ----", table->second.size()));
            for (const Place& target : table->second) {
                const std::string* label = label_of(code, target);
                out.push_back("        dd  " + (label ? *label : std::format("obj{}:0x{:04X}", target.object, target.offset)));
            }
            a += static_cast<std::int64_t>(table->second.size() * 4);
            continue;
        }
        if (const auto f = code.fixup_at(Place{object, a}); internal_fixup(f) != nullptr) {   // a lone pointer slot
            out.push_back(std::format("        dd  {:<24} ; fixup", fixup_symbol(code, *f)));
            a += 4;
            continue;
        }
        // raw bytes up to the next interesting boundary
        std::int64_t b = a;
        while (b < end && label_of(code, Place{object, b}) == nullptr && !code.jump_tables().contains(Place{object, b}) && !fixup_pointer(b) && !is_ddb_start(b)) ++b;
        const std::int64_t stop = b > a ? b : a + 1;
        const ByteView chunk = bytes.subspan(static_cast<std::size_t>(a), static_cast<std::size_t>(std::min<std::int64_t>(stop, static_cast<std::int64_t>(bytes.size())) - a));
        for (std::size_t i = 0; i < chunk.size(); i += 16) {
            std::vector<std::string> cells;
            for (std::size_t k = i; k < std::min(chunk.size(), i + 16); ++k) cells.push_back(std::format("0{:02X}h", chunk[k]));
            out.push_back("        db  " + text::join(cells, ", "));
        }
        a = stop;
    }
    return out;
}

std::string module_listing_name(const std::string& name) { return text::lower(name) + ".asm"; }

}  // namespace

std::string service_text(const ServiceCall& call, const DdkNames& names) {
    const auto known = device_name(call.device);
    const std::string device = known ? std::string(*known) : std::format("Dev_{:04X}", call.device);
    const std::string kind = call.device == 1 ? "VMMCall" : "VxDCall";
    const std::optional<std::string> service = names.services().name(device, call.service);
    const std::string reference = service && !service->empty() ? device + "." + *service : std::format("{}.Service_{:04X}h", device, call.service);
    return std::format("{}  {}{}", kind, reference, call.from_import ? "  ; (import-fixup)" : "");
}

std::pair<std::string, bool> operand_text(const Instruction& insn, std::uint32_t object, const LabelLookup& label_of) {
    static const std::set<std::string_view> branches = {"call", "jmp", "je", "jne", "jz", "jnz", "ja", "jae", "jb", "jbe", "jg", "jge", "jl", "jle", "jo", "jno", "js",
                                                        "jns", "jp", "jnp", "jc", "jnc", "jcxz", "jecxz", "loop", "loope", "loopne"};
    if (branches.contains(insn.mnemonic)) {
        if (const auto target = internal_target(insn.fixup)) {
            if (auto label = label_of(*target)) return {std::move(*label), true};
        }
        if (const auto target = insn.direct_target()) {
            if (auto label = label_of(Place{object, *target})) return {std::move(*label), true};
        }
    }
    return {insn.operands, false};
}

std::string emit_asm(const Disassembly& code, const DdkNames& names) {
    const le::Module& driver = code.driver();
    const auto& db = code.ddb();
    const std::string name = db && !db->name.empty() ? db->name : driver.resident_name();
    std::vector<std::string> lines;
    lines.push_back(std::format("; {} -- reconstructed 32-bit disassembly (VxD / LE format)", name));
    lines.push_back(std::format("; {} objects, {} instructions recovered by recursive descent", driver.objects().size(), code.instructions().size()));
    if (db) {
        lines.push_back("");
        lines.push_back(std::format("Declare_Virtual_Device {}, {}, {}, {}_Control, 0{:04X}h, Undefined_Init_Order", db->name, db->major, db->minor, db->name, db->device_id));
        lines.push_back(std::format(";   DDK version 0{:03X}h   init_order 0{:08X}h   flags 0{:04X}h", db->version, db->init_order, db->flags));
        if (const auto& table = code.service_table()) lines.push_back(std::format(";   service table: obj{}:0x{:x}, {} services", table->object, table->base, table->count));
    }
    lines.push_back("");

    for (const le::Object& o : driver.objects()) {
        const std::uint32_t oi = o.index;
        const char* segment = o.executable() ? "_LTEXT" : o.writable() ? "_LDATA" : "_LRDATA";
        lines.push_back(std::format("VxD_{} SEGMENT   ; object {}  vsize=0x{:x}  {}{}{}", segment, oi, o.vsize, o.readable() ? "R" : "-", o.writable() ? "W" : "-",
                                    o.executable() ? "X" : "-"));
        const std::int64_t n = static_cast<std::int64_t>(code.code(oi).size());
        std::int64_t a = 0;
        while (a < n) {
            const Place here{oi, a};
            const auto row = code.instructions().find(here);
            if (row == code.instructions().end()) {   // a data span, up to the next code or label
                const std::int64_t start = a;
                while (a < n && !code.is_code(Place{oi, a})) {
                    ++a;
                    if (code.labels().contains(Place{oi, a}) && code.instructions().contains(Place{oi, a})) break;
                }
                if (a == start) ++a;   // (a byte that is code but starts no instruction cannot be reached by a walk that follows the instructions)
                for (std::string& line : data_lines(code, oi, start, a)) lines.push_back(std::move(line));
                continue;
            }
            const Instruction& insn = row->second;
            if (const std::string* label = label_of(code, here)) lines.push_back(*label + ":");
            if (insn.service) {
                lines.push_back(std::format("        {:<40} ; {}", service_text(*insn.service, names), hex_of(insn.raw)));
            } else {
                const auto [operands, resolved] = operand_text(insn, oi, [&](const Place& p) {
                    const std::string* label = label_of(code, p);
                    return label ? std::optional<std::string>(*label) : std::nullopt;
                });
                std::string line = std::format("        {:<6} {}", insn.mnemonic, operands);
                if (insn.fixup && !resolved) line = std::format("{:<44} ; -> {}", line, fixup_symbol(code, *insn.fixup));   // a non-branch fix-up: name the symbol
                lines.push_back(std::move(line));
            }
            a += insn.size;
        }
        lines.push_back(std::format("VxD_{} ENDS", segment));
        lines.push_back("");
    }
    return text::join(lines, "\n");
}

std::string emit_asm(const le::Module& driver, const DdkNames& names) { return emit_asm(Disassembly::run(driver, names), names); }

std::vector<ObjectCheck> verify(const Disassembly& code) {
    std::vector<ObjectCheck> report;
    for (const le::Object& o : code.driver().objects()) {
        if (!o.executable()) continue;
        ObjectCheck check;
        check.object = o.index;
        check.size = o.vsize;
        std::vector<bool> claimed(o.vsize, false);
        for (const auto& [at, insn] : code.instructions()) {
            if (at.object != o.index) continue;
            for (std::uint32_t k = 0; k < insn.size; ++k) {
                const std::int64_t byte = at.offset + k;
                if (byte >= static_cast<std::int64_t>(o.vsize)) ++check.overrun;
                else if (claimed[static_cast<std::size_t>(byte)]) ++check.overlap;
                else claimed[static_cast<std::size_t>(byte)] = true;
            }
        }
        check.code = static_cast<std::size_t>(std::count(claimed.begin(), claimed.end(), true));
        check.data = o.vsize - check.code;
        report.push_back(check);
    }
    return report;
}

std::vector<ModuleStats> disassemble_file(const std::filesystem::path& file, const DdkNames& names, const std::optional<std::filesystem::path>& out_folder) {
    std::vector<ModuleStats> stats;
    for (const auto& [name, driver] : le::open_modules(file)) {
        const Disassembly code = Disassembly::run(driver, names);
        std::size_t executable = 0, covered = 0;
        for (const le::Object& o : driver.objects()) {
            if (!o.executable()) continue;
            executable += o.vsize;
            covered += code.code_bytes(o.index);
        }
        ModuleStats st;
        st.name = name;
        st.instructions = code.instructions().size();
        st.coverage_percent = 100.0 * static_cast<double>(covered) / static_cast<double>(executable == 0 ? 1 : executable);
        st.labels = code.labels().size();
        st.jump_tables = code.jump_tables().size();
        if (code.ddb()) {
            st.device = code.ddb()->name;
            st.device_id = code.ddb()->device_id;
        }
        if (out_folder) {
            std::filesystem::create_directories(*out_folder);
            fs::write_text(*out_folder / module_listing_name(name), emit_asm(code, names));
        }
        stats.push_back(std::move(st));
    }
    return stats;
}

int main(const Environment& env, std::span<const std::string> args) {
    // the drivers of a folder: *.vxd and *.386, by name
    const auto drivers_of = [](const std::filesystem::path& folder) {
        std::vector<std::filesystem::path> files;
        for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            const std::string extension = text::lower(entry.path().extension().string());
            if (entry.is_regular_file() && (extension == ".vxd" || extension == ".386")) files.push_back(entry.path());
        }
        std::sort(files.begin(), files.end());
        return files;
    };
    const auto cut = [](std::string_view what) { return std::string(what.substr(0, 50)); };

    const cli::Command command{
        "disassembly",
        "the driver's code as the reader sees it",
        {cli::Positional{"paths", "driver files (--verify and --all: a folder, and with --all the folder to write to)", false, true},
         cli::Option{"--verify", "", "does every instruction claim its own bytes? over a folder", cli::Arg::Flag},
         cli::Option{"--all", "", "every driver of a folder to <module>.asm", cli::Arg::Flag}},
        [&](const cli::Args& a) -> int {
            const std::vector<std::string> paths = a.values("paths");
            if (a.flag("--verify")) {
                const std::filesystem::path folder = paths.empty() ? env.corpus.root() : fs::from_text(paths[0]);
                std::size_t clean = 0, dirty = 0, modules = 0, code_total = 0, data_total = 0, overlap_total = 0, overrun_total = 0;
                for (const std::filesystem::path& file : drivers_of(folder)) {
                    try {
                        for (const auto& [name, driver] : le::open_modules(file)) {
                            std::size_t overlap = 0, overrun = 0, code = 0, data = 0;
                            for (const ObjectCheck& c : verify(Disassembly::run(driver, env.names))) {
                                overlap += c.overlap, overrun += c.overrun, code += c.code, data += c.data;
                            }
                            code_total += code, data_total += data, overlap_total += overlap, overrun_total += overrun, ++modules;
                            if (overlap != 0 || overrun != 0) {
                                ++dirty;
                                cb::out("  DIRTY {:<12} overlap={} overrun={}\n", name, overlap, overrun);
                            } else {
                                ++clean;
                            }
                        }
                    } catch (const Error& e) {   // one file of a batch
                        cb::out("  ERROR {:<16} {}\n", file.filename().string(), cut(e.what()));
                    }
                }
                cb::out("== verify: {} modules, {} CLEAN, {} dirty | code={} data={} overlap={} overrun={} ==\n", modules, clean, dirty, code_total, data_total, overlap_total,
                        overrun_total);
            } else if (a.flag("--all")) {
                const std::filesystem::path folder = paths.empty() ? env.corpus.root() : fs::from_text(paths[0]);
                const std::filesystem::path out_folder = paths.size() > 1 ? fs::from_text(paths[1]) : env.listing_folder;
                std::size_t done = 0, errors = 0;
                for (const std::filesystem::path& file : drivers_of(folder)) {
                    try {
                        for (const ModuleStats& st : disassemble_file(file, env.names, out_folder)) {
                            ++done;
                            cb::out("  {:<12} {:<9} id=0x{:04x}  {:6} insns  {:3.0f}% cov  {} jt\n", st.name, st.device.value_or("?"), st.device_id.value_or(0), st.instructions,
                                    st.coverage_percent, st.jump_tables);
                        }
                    } catch (const Error& e) {   // one file of a batch
                        ++errors;
                        cb::out("  ERROR {:<16} {}\n", file.filename().string(), cut(e.what()));
                    }
                }
                cb::out("== {} modules -> {}, {} errors ==\n", done, fs::to_text(out_folder), errors);
            } else {
                for (const std::string& path : paths) cb::out("{}\n", emit_asm(le::Module::open(fs::from_text(path)), env.names));
            }
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("disassembly", {command});
    std::vector<std::string> line = {"disassembly"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd

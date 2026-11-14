// cb/decompile/describe_flat.cpp -- the flat modules: PE (x86 and PowerPC), LE drivers, real-mode programs, NES cartridges
// (see describe.hpp).
// cb-unity: alone

#include "cb/core/fs.hpp"
#include "cb/core/intmath.hpp"
#include "cb/core/log.hpp"
#include "cb/decompile/describe_internal.hpp"

namespace cb::decompile::describe {

using namespace detail;

namespace {

constexpr std::string_view kPe = "describe32", kDos = "describe_dos";

std::filesystem::path native_path(const std::filesystem::path& p) {
    return std::filesystem::absolute(p).lexically_normal().make_preferred();
}

// name -> rva as the export directory gives it (a name met twice takes the later rva), in name order
std::map<std::string, std::uint32_t> exports_by_name(const formats::pe::Image& image) {
    std::map<std::string, std::uint32_t> out;
    for (const formats::pe::Export& e : image.exports())
        if (e.name && !e.name->empty()) out[*e.name] = e.rva;
    return out;
}

// the module's name with its extension cut ("KERNEL32.DLL" -> "KERNEL32"), then upper case
std::string stem_upper(std::string_view dll) {
    const std::size_t sep = dll.find_last_of("/\\"), dot = dll.rfind('.');
    if (dot != std::string_view::npos && (sep == std::string_view::npos || dot > sep)) {
        const std::size_t first = sep == std::string_view::npos ? 0 : sep + 1;
        // (a name of dots alone has no extension)
        if (dll.find_first_not_of('.', first) < dot) dll = dll.substr(0, dot);
    }
    return text::upper(dll);
}

// A file of at least `size` zero bytes in `outdir`, for describing memory the file does not hold.
std::string zeros_file(const std::filesystem::path& outdir, std::int64_t size) {
    const std::filesystem::path path = outdir / "zeros.bin";
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec) || static_cast<std::int64_t>(std::filesystem::file_size(path, ec)) < size)
        fs::write_file(path, Bytes(static_cast<std::size_t>(std::max<std::int64_t>(size, 1 << 20)), 0));
    return native_path(path).string();
}

std::int64_t get32(const Bytes& img, std::int64_t off) {
    std::int64_t v = 0;
    for (int k = 3; k >= 0; --k) v = (v << 8) | img[static_cast<std::size_t>(off) + k];
    return v;
}
void put32(Bytes& img, std::int64_t off, std::int64_t v) {
    for (int k = 0; k < 4; ++k) img[static_cast<std::size_t>(off) + k] = static_cast<std::uint8_t>((v >> (8 * k)) & 0xFF);
}

void write_lines(const std::filesystem::path& path, const std::vector<std::string>& lines) {
    std::string out;
    for (const std::string& l : lines) {
        out += l;
        out += '\n';
    }
    fs::write_text(path, out);
}

}  // namespace

// {va: name} -- an export's own name, else sub_<rva>. A name also used by an import of this module stays with the import
// (the call has to bind to it).
std::map<Va, std::string> function_names(const formats::pe::Image& image, std::span<const Va> functions, const std::set<std::string>& imported) {
    std::map<std::uint32_t, std::string> by_rva;
    for (const auto& [name, rva] : exports_by_name(image))
        if (text::is_identifier(name) && !imported.contains(name)) by_rva.try_emplace(rva, name);
    const Va base = static_cast<Va>(image.header().image_base);
    std::map<Va, std::string> out;
    for (Va va : functions) {
        const Va rva = va - base;
        const auto it = rva >= 0 && rva <= 0xFFFFFFFFLL ? by_rva.find(static_cast<std::uint32_t>(rva)) : by_rva.end();
        out[va] = it != by_rva.end() ? it->second : "sub_" + hex_upper(rva, 6);
    }
    return out;
}

// {address: linker name} for the functions a module exports under a name that is not a C name -- a C++ function's decorated
// name. In the C text such a function is sub_<address>; this is the symbol it must be linked as, and the demangler makes
// it a declaration.
std::map<Va, std::string> decorated_names(const formats::pe::Image& image) {
    std::map<Va, std::string> out;
    for (const auto& [name, rva] : exports_by_name(image))
        if (!text::is_identifier(name)) out.try_emplace(static_cast<Va>(image.header().image_base) + rva, name);
    return out;
}

// Write the description of a PE module for the native program: its processor, where its bytes sit in memory, and the facts
// about `functions` (addresses) and its imports.
PeDescription describe32(const std::filesystem::path& module, const formats::pe::Image& image, std::span<const Va> functions,
                         const std::filesystem::path& outdir, Knowledge& knowledge, const PowerPcFacts* ppc) {
    const formats::pe::Header& header = image.header();
    const std::optional<std::string_view> arch = language_of_machine(header.machine);
    if (!arch) throw UnsupportedError(kPe, 0, std::format("{}: no language for machine type {:#x}", module.filename().string(), header.machine));
    const bool x86 = arch->starts_with("x86:");
    PeDescription out;
    out.path = outdir / "description.txt";
    const std::set<std::string> imported = x86 ? knowledge.imported_names(module) : std::set<std::string>{};
    if (x86) {
        out.names = function_names(image, functions, imported);
    } else {   // PowerPC: what run-time type information names
        for (Va va : functions) {
            const auto own = ppc ? ppc->names.find(va) : std::map<Va, std::string>::const_iterator{};
            out.names[va] = ppc && own != ppc->names.end() ? own->second : "sub_" + hex_upper(va, 8);
        }
    }
    std::vector<std::string> lines;
    lines.push_back("arch\t" + std::string(*arch));
    const std::string mpath = native_path(module).string();
    if (!x86) {
        // PowerPC: the image says where its bytes sit, which stubs are imports, and which register save/restore runs are part
        // of their callers (formats/powerpc_image)
        if (ppc) {
            for (const PowerPcFacts::Span& span : ppc->spans)
                lines.push_back(std::format("bytes\t{}\t{}\t{}\t{}", hx(span.va), mpath, hx(static_cast<Va>(span.file_offset)), hx(static_cast<Va>(span.length))));
            for (const auto& [va, name] : ppc->imports) lines.push_back(std::format("X\t{}\t{}\t-1", hx(va), name));
            for (const auto& [va, name] : ppc->inlined) lines.push_back(std::format("F\t{}\t{}\t-1\tinline", hx(va), name));
        }
    } else {
        const Va image_base = static_cast<Va>(header.image_base);
        for (const formats::pe::Section& sec : image.sections()) {
            const Va vsize = sec.virtual_size, rawsize = sec.raw_size;
            const Va size = std::min(rawsize, vsize ? vsize : rawsize);
            if (size && sec.raw_ptr)
                lines.push_back(std::format("bytes\t{}\t{}\t{}\t{}", hx(image_base + sec.virtual_address), mpath, hx(sec.raw_ptr), hx(size)));
            // The part of a section the file does not hold (.bss; a section's tail past its raw size) is zeros when the module
            // is loaded: described as such, its globals are the image's, and their sizes reach the C (undescribed, every one
            // was declared 4 bytes: setup32's `dec word ptr [40e36c]` stored over the word beside it).
            if (vsize > size)
                lines.push_back(std::format("bytes\t{}\t{}\t0\t{}", hx(image_base + sec.virtual_address + size), zeros_file(outdir, vsize - size), hx(vsize - size)));
        }
    }
    // Functions that take an argument in ECX and nothing else in a register: a C++ member function's object. Read from the code
    // (ECX is used before anything is put in it), and said to the decompiler, which otherwise shows the object as a variable
    // nobody set -- in the function and, worse, not passed at all by its callers.
    Knowledge::Conventions conventions;
    if (x86) {
        try {
            conventions = knowledge.conventions(module);
        } catch (const std::exception& e) {   // isolation boundary: the analysis of one module
            // (said, not swallowed: without it no function takes ECX or EDX, and every C++ method of the module loses it --
            // msjava.dll's analysis stopped on a function of padding alone, and 116 of its functions passed EnterCriticalSection
            // garbage)
            log::warn(std::format("describe32: {}: calling conventions not found ({}); no function takes ECX or EDX",
                                  module.filename().string(), e.what()));
            conventions = {};
        }
    }
    for (Va va : functions)
        if (conventions.thiscall.contains(va)) out.in_ecx.insert(out.names.at(va));
    for (Va va : functions) {
        std::optional<std::int64_t> pop;
        if (x86) pop = knowledge.pe_function_pop(module, static_cast<std::uint32_t>(va - static_cast<Va>(header.image_base)));
        const char* convention = conventions.thiscall.contains(va) && pop ? "\t__thiscall" : conventions.fastcall.contains(va) && pop ? "\t__fastcall" : "";
        lines.push_back(std::format("F\t{}\t{}\t{}{}", hx(va), out.names.at(va), pop ? *pop : -1, convention));
    }
    // An import by number is called by its real name when the module that exports it (or that module's definition file) gives
    // it one that is a C name nothing else here already has; how many bytes of arguments it takes is read from the export with
    // that number.
    std::map<std::string, std::optional<std::pair<std::string, std::int64_t>>> taken;   // name -> who has it (lookup only)
    if (x86) {
        for (const std::string& k : imported) taken[k] = std::nullopt;
        for (Va va : functions) taken[out.names.at(va)] = std::nullopt;
    }
    std::vector<formats::pe::Import> imports;
    if (x86) imports = image.imports();
    std::stable_sort(imports.begin(), imports.end(), [](const auto& a, const auto& b) { return a.slot_va < b.slot_va; });
    for (const formats::pe::Import& imp : imports) {
        const std::string stem = stem_upper(imp.dll);
        const std::string& name = imp.name;
        const std::string slot = hx(static_cast<Va>(imp.slot_va));
        if (name.empty() || name.starts_with("#")) {
            const std::int64_t number = (!name.empty() && is_dec_lit(std::string_view(name).substr(1))) ? text::parse_int<std::int64_t>(std::string_view(name).substr(1)).value_or(0) : 0;
            const std::optional<std::string> real = knowledge.import_name(stem, static_cast<std::uint32_t>(number));
            bool take = false;
            if (real && !real->empty()) {
                const std::pair<std::string, std::int64_t> want{stem, number};
                if (!taken.contains(*real)) taken[*real] = want;
                take = taken[*real] && *taken[*real] == want;
            }
            if (take) {
                std::optional<std::int64_t> pop = knowledge.declared_argument_bytes(*real);
                if (!pop) pop = knowledge.win32_argument_bytes(stem, name);
                lines.push_back(std::format("I\t{}\t{}\t{}", slot, *real, pop ? *pop : -1));
                out.by_number[*real] = {stem, number, pop};
            } else {
                // no name for it -- but the module that exports it still says how many bytes of arguments it removes, and without
                // that the call is recovered with none
                const std::optional<std::int64_t> pop = !name.empty() ? knowledge.win32_argument_bytes(stem, name) : std::nullopt;
                const std::string shown = name.empty() ? "0" : name.substr(1);
                const std::string id = text::sanitize_identifier(stem + "_ord_" + shown);
                lines.push_back(std::format("I\t{}\t{}\t{}", slot, id, pop ? *pop : -1));
                out.by_number[id] = {stem, number, pop};
            }
            continue;
        }
        const std::optional<std::int64_t> pop = x86 ? knowledge.win32_argument_bytes(stem, name) : std::nullopt;
        lines.push_back(std::format("I\t{}\t{}\t{}", slot, text::sanitize_identifier(name), pop ? *pop : -1));
    }
    // (the old CB_POINTER_CALLS=1 experiment -- `O` lines for calls through a pointer -- is not carried over: it was off, and
    // on the 32-bit sample it fixed no program and made msjava.dll worse; to be made right before it is on)
    write_lines(out.path, lines);
    return out;
}

// ---- Virtual device drivers (LE) ---------------------------------------------------------------------------------------

// Describe one LE image to the decompiler.
//
// A driver's objects are relocatable, so each is given an address of its own and the fix-ups the loader would apply are
// applied: a 32-bit reference to another place in the image gets that place's address. Ring-0 code calls the system and other
// drivers with `int 20h` followed by a service number -- six bytes the loader rewrites into a call; they are rewritten here into
// a call through a pointer that stands for that service, so the decompiler sees a named call instead of an interrupt and four
// bytes of nonsense.
LeDescription describe_le(const formats::le::Module& module, std::string_view name, const std::filesystem::path& outdir, Knowledge& knowledge) {
    std::map<std::int64_t, Bytes> images;
    for (const formats::le::Object& o : module.objects()) {
        Bytes b = module.object_data(o);
        if (b.size() < o.vsize) b.resize(o.vsize, 0);
        images[o.index] = std::move(b);
    }
    std::set<std::int64_t> execs;
    for (const formats::le::Object& o : module.objects())
        if (o.executable()) execs.insert(o.index);
    std::set<Va> starts;
    std::vector<std::pair<formats::le::Place, formats::le::Fixup>> fixups;
    try {
        fixups = module.fixups();
    } catch (const FormatError&) {   // a damaged fix-up table: the objects are described as the file holds them
        fixups.clear();
    }
    for (const auto& [place, fixup] : fixups) {
        const auto it = images.find(place.object);
        if (it == images.end()) continue;
        Bytes& img = it->second;
        if (place.offset < 0 || place.offset + 4 > static_cast<std::int64_t>(img.size())) continue;
        const auto* fx = std::get_if<formats::le::InternalFixup>(&fixup);
        if (!fx || !fx->target || !images.count(fx->object)) continue;
        const Va target = le_address(fx->object, *fx->target);
        if (fx->source_type == 7) {   // 32-bit offset
            put32(img, place.offset, target & 0xFFFFFFFF);
        } else if (fx->source_type == 8) {   // 32-bit self-relative (a call or jump)
            put32(img, place.offset, (target - (le_address(place.object, place.offset) + 4)) & 0xFFFFFFFF);
            if (execs.count(fx->object)) starts.insert(target);
        }
    }
    // service calls: CD 20 <service:16> <device:16>
    std::map<std::pair<std::int64_t, std::int64_t>, Va> services;
    std::vector<std::pair<std::int64_t, std::int64_t>> order;
    for (const std::int64_t obj : execs) {
        Bytes& img = images[obj];
        std::size_t i = 0;
        for (;;) {
            while (i + 6 <= img.size() && !(img[i] == 0xCD && img[i + 1] == 0x20)) ++i;
            if (i + 6 > img.size()) break;
            const std::int64_t svc = word_at(img, static_cast<std::int64_t>(i) + 2), dev = word_at(img, static_cast<std::int64_t>(i) + 4);
            if (0 < dev && dev < 0x600 && (svc & 0x7FFF) < 0x400) {
                const auto key = std::make_pair(dev, svc & 0x7FFF);
                if (!services.count(key)) {
                    services[key] = kLeSlots + 4 * static_cast<Va>(order.size());
                    order.push_back(key);
                }
                img[i] = 0xFF;
                img[i + 1] = (svc & 0x8000) ? 0x25 : 0x15;   // jmp / call [slot]
                put32(img, static_cast<std::int64_t>(i) + 2, services[key]);
                i += 6;
            } else {
                i += 2;
            }
        }
    }
    // where functions start: the control procedure, self-relative targets, agreed near calls
    std::optional<formats::le::Ddb> ddb;
    try {
        ddb = module.ddb();
    } catch (const FormatError&) {   // a damaged descriptor: no control procedure to start from
        ddb = std::nullopt;
    }
    if (ddb) {
        const auto it = images.find(ddb->object);
        const std::int64_t at = ddb->rva + 4 + 20;
        if (it != images.end() && at + 4 <= static_cast<std::int64_t>(it->second.size())) {
            const std::int64_t cp = get32(it->second, at);
            bool in_exec = false;
            for (const std::int64_t o : execs)
                if (le_address(o, 0) <= cp && cp < le_address(o, static_cast<std::int64_t>(images[o].size()))) in_exec = true;
            if (in_exec) starts.insert(cp);
        }
    }
    for (const std::int64_t obj : execs) {
        const Bytes& img = images[obj];
        std::map<std::int64_t, std::set<std::int64_t>> sites;
        for (std::int64_t i = 0; i < static_cast<std::int64_t>(img.size()) - 4; ++i) {
            if (img[static_cast<std::size_t>(i)] == 0xE8) {
                const std::int64_t rel = static_cast<std::int32_t>(static_cast<std::uint32_t>(get32(img, i + 1)));
                const std::int64_t t = i + 5 + rel;
                if (0 <= t && t < static_cast<std::int64_t>(img.size())) sites[t].insert(i);
            }
        }
        for (const auto& [target, from] : sites)
            if (from.size() >= 2) starts.insert(le_address(obj, target));
    }
    const std::filesystem::path blob = native_path(outdir / "image_le.bin");
    std::vector<std::string> lines;
    lines.push_back("arch\t" + std::string(kArchLe));
    Bytes blob_bytes;
    for (const auto& [obj, img] : images) {
        if (img.empty()) continue;
        lines.push_back(std::format("bytes\t{}\t{}\t{}\t{}", hx(le_address(obj, 0)), blob.string(), hx(static_cast<Va>(blob_bytes.size())), hx(static_cast<Va>(img.size()))));
        append(blob_bytes, img);
    }
    fs::write_file(blob, blob_bytes);
    LeDescription d;
    d.path = outdir / "description.txt";
    for (const Va va : starts)
        d.names[va] = std::format("sub_{}_{}", floor_div<Va>(va - kLeBase, kLeStride) + 1, hex_upper(floor_mod<Va>(va - kLeBase, kLeStride), 5));
    for (const Va va : starts) lines.push_back(std::format("F\t{}\t{}\t-1", hx(va), d.names.at(va)));
    for (const auto& dev_svc : order)
        lines.push_back(std::format("I\t{}\t{}\t-1", hx(services[dev_svc]), knowledge.driver_service_label(static_cast<std::uint32_t>(dev_svc.first), static_cast<std::uint32_t>(dev_svc.second))));
    for (const std::int64_t obj : execs) d.spans[obj] = {le_address(obj, 0), static_cast<std::int64_t>(images[obj].size())};
    d.device = ddb && !ddb->name.empty() ? ddb->name : std::string(name);
    d.lines = std::move(lines);
    d.write();
    return d;
}

// ---- DOS programs (MZ) -----------------------------------------------------------------------------------------------

// Describe a real-mode program to the decompiler.
//
// The reader (formats/dos) gives the bytes as they sit in memory -- unpacked when the file stores them packed, a .COM behind
// its program prefix, a device driver, the parts of IO.SYS each where it runs -- and the analysis (analyze/dos_functions)
// every function start that can be established, with whether it returns far or near and how many bytes its return removes. The
// Description also carries `program`, `analysis` and `interrupts` ({function name: [interrupt calls]}) so the interrupt calls in
// the output can be named.
DosDescription describe_dos(const formats::dos::Program& program, const std::filesystem::path& outdir) {
    if (!program.ds) throw UnsupportedError(kDos, 0, "the program sets no data segment");
    auto found = std::make_shared<const analyze::dos_functions::Analysis>(analyze::dos_functions::discover(program));
    const std::filesystem::path blob = native_path(outdir / "image_dos.bin");
    fs::write_file(blob, program.image);
    std::vector<std::string> lines;
    lines.push_back("arch\t" + std::string(kArchDos));
    for (const formats::dos::Span& span : program.spans)
        lines.push_back(std::format("bytes\t{}\t{}\t{}\t{}", hx(span.address), blob.string(), hx(static_cast<Va>(span.address) - static_cast<Va>(program.base)), hx(span.length)));
    lines.push_back("reg\tDS\t" + hx(*program.ds));
    DosDescription d;
    d.path = outdir / "description.txt";
    for (const auto& [va, fn] : found->functions) {   // (ascending addresses)
        d.names[va] = fn.name;
        lines.push_back(std::format("F\t{}\t{}\t{}\t{}{}", hx(fn.address), fn.name, fn.removes, fn.removes > 0 ? "__stdcall16" : "__cdecl16",
                                    analyze::dos_functions::name_of(fn.distance)));
    }
    std::int64_t k = 0;
    for (const formats::dos::Span& span : program.spans) d.spans[++k] = {span.address, span.length};
    d.image = program.image;
    d.base = program.base;
    d.program = program;
    d.analysis = found;
    for (const analyze::dos_functions::Interrupt& call : found->interrupts)
        if (call.function) d.interrupts[found->functions.at(*call.function).name].push_back(call);
    d.lines = std::move(lines);
    d.write();
    return d;
}

// ---- NES --------------------------------------------------------------------------------------------------------------

// Write the description of a NES cartridge for the native program: the 6502, the program ROM where the CPU sees it at power-on
// (formats/nes), and its functions -- the three vectors' and every JSR target.
NesDescription describe_nes(const formats::nes::Rom& rom, std::span<const std::uint32_t> functions, const std::filesystem::path& outdir) {
    const std::filesystem::path image = outdir / "cpu.bin";
    const Bytes memory = rom.memory();
    fs::write_file(image, ByteView(memory).subspan(std::min<std::size_t>(0x8000, memory.size())));
    std::map<Va, std::string> vectors;   // lookup only
    vectors.try_emplace(rom.vectors.reset, "reset");
    vectors.try_emplace(rom.vectors.nmi, "nmi");
    vectors.try_emplace(rom.vectors.irq, "irq");
    std::vector<std::string> lines;
    lines.push_back("arch\t" + std::string(kArchNes));
    lines.push_back("bytes\t8000\t" + native_path(image).string() + "\t0\t8000");
    NesDescription out;
    out.path = outdir / "description.txt";
    for (const std::uint32_t va : functions) {
        const auto v = vectors.find(va);
        out.names[va] = v != vectors.end() ? v->second : "sub_" + hex_upper(va, 4);
        lines.push_back(std::format("F\t{}\t{}\t-1", hx(va), out.names.at(va)));
    }
    write_lines(out.path, lines);
    return out;
}

}  // namespace cb::decompile::describe

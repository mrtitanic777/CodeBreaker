// compare/versions.cpp -- reading a build's functions and fingerprinting them; matching two builds (see versions.hpp).
// cb-unity: alone

#include "compare/versions.hpp"

#include <format>
#include <unordered_set>

#include "analyze/dos_functions.hpp"
#include "symbols/mask.hpp"
#include "compare/versions_internal.hpp"
#include "core/capstone_api.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/describe.hpp"
#include "formats/detect.hpp"
#include "formats/dos.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"
#include "formats/pe_functions.hpp"
#include "platforms/decoder.hpp"

namespace cb::compare::versions {

using namespace detail;

std::string_view level_name(Level level) { return level == Level::Bytes ? "bytes" : level == Level::Text ? "text" : "shape"; }
std::optional<Level> level_from_name(std::string_view name) {
    for (Level l : kLevels)
        if (level_name(l) == name) return l;
    return std::nullopt;
}
std::string_view kind_name(Kind kind) { return kind == Kind::Pe ? "pe" : kind == Kind::Ne ? "ne" : "dos"; }
std::string_view how_name(How how) {
    switch (how) {
        case How::Name: return "name";
        case How::Bytes: return "bytes";
        case How::Text: return "text";
        case How::Shape: return "shape";
        case How::Calls: return "calls";
        case How::Likeness: return "likeness";
        case How::Position: return "position";
    }
    return "";
}

// ---- Side -------------------------------------------------------------------------------------------------------------------
void Side::fill_name() { name_ = text::lower(path_.filename().string()); }

std::int64_t Side::size() const {
    std::int64_t n = 0;
    for (const Function& f : functions_) n += f.size;
    return n;
}

std::optional<std::size_t> Side::index_of(std::string_view key) const {
    const auto it = index_.find(key);
    return it == index_.end() ? std::nullopt : std::optional<std::size_t>(it->second);
}
const Function* Side::find(std::string_view key) const {
    const auto i = index_of(key);
    return i ? &functions_[*i] : nullptr;
}

void Side::add(Function f) {
    index_[f.key] = functions_.size();
    functions_.push_back(std::move(f));
}

void Side::finish() {
    callers_.assign(functions_.size(), {});
    for (std::size_t i = 0; i < functions_.size(); ++i) {
        for (const Call& c : functions_[i].calls) {
            if (c.to != Call::To::Function) continue;
            const auto target = index_of(c.key);
            if (target && (callers_[*target].empty() || callers_[*target].back() != i)) callers_[*target].push_back(i);
        }
    }
}

// ---- reading a build --------------------------------------------------------------------------------------------------------
namespace {

constexpr int kOpReg = X86_OP_REG, kOpImm = X86_OP_IMM, kOpMem = X86_OP_MEM;

struct Operand {
    int type = 0;
    std::uint32_t reg = 0, segment = 0, base = 0, index = 0;
    std::int64_t imm = 0, disp = 0;
    int scale = 0;
};
// One decoded instruction with what the fingerprint reads of it: Capstone's record, copied.
struct Ins {
    std::int64_t address = 0, size = 0;
    std::string mnemonic, op_str;
    Bytes bytes;
    std::vector<Operand> operands;
    int imm_offset = 0, imm_size = 0, disp_offset = 0, disp_size = 0;
};

// The Capstone engines this thread has made (one per word size, detail on).
const platforms::Decoder& versions_decoder(unsigned bits) {
    thread_local platforms::DecoderSet set;
    return set.get(platforms::Arch::X86, bits, true);
}

std::vector<Ins> versions_decode(unsigned bits, ByteView code, std::int64_t address) {
    const platforms::Decoder& md = versions_decoder(bits);
    std::vector<Ins> out;
    md.each(code, static_cast<std::uint64_t>(address), 0, [&](const cs_insn& c) {
        Ins x;
        x.address = static_cast<std::int64_t>(c.address);
        x.size = c.size;
        x.mnemonic = c.mnemonic;
        x.op_str = c.op_str;
        x.bytes.assign(c.bytes, c.bytes + c.size);
        if (c.detail) {
            const cs_x86& d = c.detail->x86;
            for (int k = 0; k < d.op_count; ++k) {
                const cs_x86_op& o = d.operands[k];
                Operand p;
                p.type = o.type;
                if (o.type == X86_OP_REG) p.reg = static_cast<std::uint32_t>(o.reg);
                else if (o.type == X86_OP_IMM) p.imm = o.imm;
                else if (o.type == X86_OP_MEM) {
                    p.segment = static_cast<std::uint32_t>(o.mem.segment);
                    p.base = static_cast<std::uint32_t>(o.mem.base);
                    p.index = static_cast<std::uint32_t>(o.mem.index);
                    p.scale = o.mem.scale;
                    p.disp = o.mem.disp;
                }
                x.operands.push_back(p);
            }
            x.imm_offset = d.encoding.imm_offset;
            x.imm_size = d.encoding.imm_size;
            x.disp_offset = d.encoding.disp_offset;
            x.disp_size = d.encoding.disp_size;
        }
        out.push_back(std::move(x));
        return true;
    });
    return out;
}

std::string register_name(unsigned bits, std::uint32_t reg) {
    const char* n = versions_decoder(bits).register_name(reg);
    return n ? n : "";
}

// a reference a relocated byte has: an import ("KERNEL.5"), a function of the module (its key), or anything else
struct Ref {
    enum class Is { Import, Function, Data } is = Is::Data;
    std::string key;
};

// What a function's fingerprint asks of the module it is in.
struct Surroundings {
    std::function<bool(std::int64_t)> relocated;                        // a loader or linker fills the byte at this address in
    std::function<std::optional<std::string>(std::int64_t)> direct;     // the function a direct call or jump to this address reaches
    std::function<std::optional<Ref>(std::int64_t)> site;               // what the relocated byte there refers to
    std::function<std::optional<std::string>(std::int64_t)> slot;       // the import an absolute address is the slot of
    std::function<bool(std::int64_t)> inside;                           // the address is in the module's own image
};

// The instructions of the function at `lo`: up to the return or jump away after which no jump inside it still points ahead (or
// to `hi`, where the next function starts).
std::vector<Ins> body_of(const std::vector<Ins>& stream, std::int64_t lo, std::int64_t hi) {
    std::vector<Ins> out;
    std::int64_t reach = lo;
    for (const Ins& ins : stream) {
        out.push_back(ins);
        const std::string& mn = ins.mnemonic;
        const std::int64_t next = ins.address + ins.size;
        if (mn[0] == 'j' || mn.starts_with("loop")) {
            if (ins.operands.size() == 1 && ins.operands[0].type == kOpImm && lo <= ins.operands[0].imm && ins.operands[0].imm < hi)
                reach = std::max(reach, ins.operands[0].imm);
        }
        const bool leaves = mn == "jmp" || mn == "ljmp" || mn == "ret" || mn == "retf" || mn == "lret" || mn == "iret" || mn == "iretd";
        if (next >= hi || (leaves && next > reach)) break;
    }
    return out;
}

// The kind of instruction, for the shape: tests and compares are one, the moves are one, every conditional jump is one.
std::string alike(const std::string& mn, bool jumps) {
    if (mn == "test" || mn == "cmp") return "cmp";
    if (mn == "mov" || mn == "lea" || mn == "movzx" || mn == "movsx") return "mov";
    if (jumps && mn != "jmp" && mn != "ljmp") return "jcc";
    return mn;
}

std::string digest(const std::vector<std::string>& parts) { return hex_of(blake2b8(text::join(parts, "\n"))); }

// Fingerprint one function: its instructions as machine code with what the loader fills in blanked, as text with every
// address replaced by what it stands for, and as kinds only; what it calls; what is distinctive in it.
Function make_function(unsigned bits, std::string key, std::string name, bool named, const std::vector<Ins>& body, std::int64_t lo,
                       std::int64_t hi, const Surroundings& around) {
    Function f;
    f.key = std::move(key);
    f.name = std::move(name);
    f.named = named;
    f.at = lo;
    f.size = body.empty() ? 0 : body.back().address + body.back().size - lo;
    std::unordered_map<std::int64_t, std::int64_t> index;   // lookup only
    for (std::size_t i = 0; i < body.size(); ++i) index[body[i].address] = static_cast<std::int64_t>(i);
    std::vector<std::string> raw, texts, shapes;
    for (std::size_t ii = 0; ii < body.size(); ++ii) {
        const Ins& ins = body[ii];
        const std::string& mn = ins.mnemonic;
        std::vector<Operand> ops = ins.operands;
        const std::int64_t addr = ins.address;
        std::vector<std::int64_t> hits;
        for (std::int64_t k = 0; k < ins.size; ++k)
            if (around.relocated(addr + k)) hits.push_back(k);
        std::set<std::int64_t> out(hits.begin(), hits.end());   // the bytes left out of the `bytes` digest
        std::optional<Ref> ref;
        for (std::int64_t k : hits) {
            if (auto r = around.site(addr + k)) {
                ref = r;
                break;
            }
        }
        const bool calls = mn == "call" || mn == "lcall";
        const bool jumps = mn[0] == 'j' || mn.starts_with("loop") || mn == "ljmp";
        std::vector<std::string> parts, kinds;
        std::optional<Call> called;
        auto stands_for_import = [&] { return ref && ref->is == Ref::Is::Import; };
        if ((mn == "lcall" || mn == "ljmp") && ops.size() == 2) {   // far, to segment:offset
            if (ref && ref->is != Ref::Is::Data) called = Call{ref->is == Ref::Is::Function ? Call::To::Function : Call::To::Import, ref->key};
            parts.push_back(stands_for_import() ? "!" + ref->key : ">");
            kinds.push_back("t");
            ops.clear();
        }
        for (const Operand& op : ops) {
            if (op.type == kOpReg) {
                const std::string reg = register_name(bits, op.reg);
                parts.push_back(reg);
                kinds.push_back(reg == "bp" || reg == "ebp" ? "f" : reg == "sp" || reg == "esp" ? "s" : (reg.size() == 2 && reg.back() == 's') ? "g" : "r");
            } else if (op.type == kOpImm) {
                const std::int64_t lo_f = ins.imm_offset, n_f = ins.imm_size;
                const bool any_hit = std::any_of(hits.begin(), hits.end(), [&](std::int64_t k) { return lo_f <= k && k < lo_f + n_f; });
                if ((calls || jumps) && ops.size() == 1) {
                    if (!calls && lo <= op.imm && op.imm < hi && index.count(op.imm)) {
                        parts.push_back(std::format("{:+}", index[op.imm] - static_cast<std::int64_t>(ii)));
                        f.jumps += 1;
                    } else {   // leaves the function
                        for (std::int64_t k = lo_f; k < lo_f + n_f; ++k) out.insert(k);
                        const auto tgt = around.direct(op.imm);
                        called = tgt ? std::optional<Call>(Call{Call::To::Function, *tgt}) : std::nullopt;
                        parts.push_back(">");
                    }
                    kinds.push_back("t");
                } else if (any_hit || (n_f == 4 && around.inside(op.imm & 0xFFFFFFFFLL))) {
                    for (std::int64_t k = lo_f; k < lo_f + n_f; ++k) out.insert(k);
                    parts.push_back(stands_for_import() ? "!" + ref->key : around.direct(op.imm) ? ">" : "@");
                    kinds.push_back("a");
                } else {
                    const std::int64_t v = op.imm & 0xFFFFFFFFLL;
                    parts.push_back(std::format("{:x}", v));
                    kinds.push_back("i");
                    if (0x100 <= v && v < 0xFFFF0000LL && v != 0xFFFF && v != 0xFF00 && v != 0x8000 && v != 0x7FFF) f.marks.insert(std::format("c{:x}", v));
                }
            } else if (op.type == kOpMem) {
                const std::int64_t lo_f = ins.disp_offset, n_f = ins.disp_size;
                const bool bare = !op.base && !op.index;
                const bool any_hit = std::any_of(hits.begin(), hits.end(), [&](std::int64_t k) { return lo_f <= k && k < lo_f + n_f; });
                std::string disp;
                if (bare || any_hit || (n_f == 4 && around.inside(op.disp & 0xFFFFFFFFLL))) {
                    if (n_f == 4)
                        for (std::int64_t k = lo_f; k < lo_f + n_f; ++k) out.insert(k);
                    std::optional<std::string> imp;
                    if (bare) imp = around.slot(op.disp & 0xFFFFFFFFLL);
                    if (imp && !imp->empty() && (calls || jumps)) called = Call{Call::To::Import, *imp};
                    disp = imp && !imp->empty() ? "!" + *imp : stands_for_import() ? "!" + ref->key : "@";
                } else {
                    disp = std::format("{:x}", op.disp & 0xFFFFFFFFLL);
                }
                parts.push_back("[" + (op.segment ? register_name(bits, op.segment) : std::string()) + ":" + (op.base ? register_name(bits, op.base) : std::string()) +
                                "+" + (op.index ? register_name(bits, op.index) : std::string()) + "*" + std::to_string(op.scale) + "+" + disp + "]");
                kinds.push_back("m");
            }
        }
        if (called) {
            f.calls.push_back(*called);
            if (called->to == Call::To::Import) f.marks.insert("i" + called->key);
        }
        raw.emplace_back(as_text(symbols::mask::blank(ins.bytes, out)));
        f.rows.push_back({static_cast<std::uint64_t>(addr), std::string(text::trim(mn + " " + ins.op_str))});
        std::string joined = text::join(parts, ",");
        texts.push_back(mn + " " + joined);
        f.text.push_back(texts.back());
        f.shape.push_back(alike(mn, jumps) + " " + text::join(kinds, ""));
        shapes.push_back(f.shape.back());
    }
    f.ids = {digest(raw), digest(texts), digest(shapes)};
    return f;
}

std::string key16(std::int64_t seg, std::int64_t off) { return std::format("{}:{:04x}", seg, off); }

// The bytes of a segment as the file holds them (cut to the file).
ByteView stored_segment(const formats::ne::Module& m, const formats::ne::Segment& s) {
    const ByteView whole = m.data();
    if (s.file_offset == 0 || s.file_offset >= whole.size()) return {};
    return whole.subspan(s.file_offset, std::min<std::uint64_t>(s.length, whole.size() - s.file_offset));
}

// {offset in the segment: what the relocated byte there refers to}
std::unordered_map<std::int64_t, Ref> sites_of(const formats::ne::Module& m, const formats::ne::Segment& seg, ByteView code,
                                               const std::map<std::uint16_t, formats::ne::Segment>& segs,
                                               const std::map<std::uint32_t, formats::ne::Entry>& entries, const std::vector<std::string>& refs) {
    std::unordered_map<std::int64_t, Ref> out;   // lookup only
    for (const formats::ne::Relocation& r : m.relocations(seg)) {
        Ref what;
        if (const auto* by_number = std::get_if<formats::ne::ImportOrdinalRef>(&r.target)) {
            const std::string mod = (0 < by_number->module && by_number->module <= refs.size()) ? refs[by_number->module - 1] : "?";
            what = {Ref::Is::Import, mod + "." + std::to_string(by_number->ordinal)};
        } else if (const auto* by_name = std::get_if<formats::ne::ImportNameRef>(&r.target)) {
            const std::string mod = (0 < by_name->module && by_name->module <= refs.size()) ? refs[by_name->module - 1] : "?";
            what = {Ref::Is::Import, mod + "." + m.imported_name(by_name->name_offset)};
        } else if (const auto* internal = std::get_if<formats::ne::InternalRef>(&r.target)) {
            std::optional<std::pair<std::uint32_t, std::uint32_t>> e;   // (segment, offset)
            if (internal->segment == 0xFF) {
                const auto it = entries.find(internal->entry);
                if (it != entries.end()) e = std::pair<std::uint32_t, std::uint32_t>(it->second.segment, it->second.offset);
            } else {
                e = std::pair<std::uint32_t, std::uint32_t>(internal->segment, internal->entry);
            }
            if (e) {
                const auto s = segs.find(static_cast<std::uint16_t>(e->first));
                if (s != segs.end() && !s->second.is_data() && r.source == 3) what = {Ref::Is::Function, key16(e->first, e->second)};
            }
        }
        const int width = r.source == 3 ? 4 : r.source == 2 ? 2 : r.source == 5 ? 2 : r.source == 0 ? 1 : 2;
        std::int64_t off = r.offset, hops = 0;
        while (off != 0xFFFF && off + 1 < static_cast<std::int64_t>(seg.length) && hops < 0x10000) {
            for (int k = 0; k < width; ++k) out[off + k] = what;
            if (r.additive()) break;
            if (off + 1 >= static_cast<std::int64_t>(code.size())) break;   // (the chain runs out of the stored bytes)
            off = static_cast<std::int64_t>(code[static_cast<std::size_t>(off)]) | (static_cast<std::int64_t>(code[static_cast<std::size_t>(off) + 1]) << 8);
            ++hops;
        }
    }
    return out;
}

Side load_pe(const std::filesystem::path& path) {
    const formats::pe::Image image = formats::pe::Image::open(path);
    const std::vector<formats::pe_functions::Function> spans = formats::pe_functions::function_spans(image);
    const auto sections = image.code_sections();
    const std::int64_t low = static_cast<std::int64_t>(image.header().image_base);
    std::int64_t span_max = 0;
    for (const formats::pe::Section& s : image.sections())
        span_max = std::max<std::int64_t>(span_max, static_cast<std::int64_t>(s.virtual_address) + std::max(s.virtual_size, s.raw_size));
    const std::int64_t high = low + span_max;
    std::vector<bool> relocated(static_cast<std::size_t>(span_max) + 4, false);   // by address - image base
    for (std::uint64_t rva : image.base_relocs())
        for (int k = 0; k < 4; ++k)
            if (rva + k < relocated.size()) relocated[rva + k] = true;
    std::unordered_map<std::int64_t, std::string> imports;   // lookup only
    for (const formats::pe::Import& imp : image.imports()) {
        std::string dll = imp.dll;
        const std::size_t dot = dll.rfind('.');
        if (dot != std::string::npos && dot != 0) dll.resize(dot);
        imports[static_cast<std::int64_t>(imp.slot_va)] = text::upper(dll) + "." + imp.name;
    }
    std::int64_t code = 0;
    for (const auto& s : sections) code += static_cast<std::int64_t>(image.section_data(*s.section).size());
    Side side(path, Kind::Pe, code);
    std::unordered_set<std::int64_t> starts;   // lookup only
    for (const auto& s : spans) starts.insert(s.start);
    Surroundings around;
    around.relocated = [&](std::int64_t a) { const std::int64_t r = a - low; return r >= 0 && static_cast<std::size_t>(r) < relocated.size() && relocated[static_cast<std::size_t>(r)]; };
    around.direct = [&](std::int64_t t) -> std::optional<std::string> { return starts.count(t) ? std::optional<std::string>(std::format("{:x}", t)) : std::nullopt; };
    around.site = [](std::int64_t) { return std::nullopt; };
    around.slot = [&](std::int64_t a) -> std::optional<std::string> {
        const auto it = imports.find(a);
        return it == imports.end() ? std::nullopt : std::optional<std::string>(it->second);
    };
    around.inside = [&](std::int64_t v) { return low <= v && v < high; };
    std::vector<formats::pe_functions::Function> sorted = spans;
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.start < b.start; });
    for (const auto& sp : sorted) {
        const std::int64_t va = sp.start, hi = sp.end;
        ByteView data;
        std::int64_t base = 0;
        bool found = false;
        for (const auto& s : sections) {
            const ByteView d = image.section_data(*s.section);
            const std::int64_t b = static_cast<std::int64_t>(s.va);
            if (b <= va && va < b + static_cast<std::int64_t>(d.size())) {
                base = b;
                data = d;
                found = true;
                break;
            }
        }
        if (!found) throw FormatError("versions", static_cast<std::uint64_t>(va), "a function that is in no code section");
        const std::int64_t from = va - base, to = std::min<std::int64_t>(hi - base, static_cast<std::int64_t>(data.size()));
        const ByteView body = to > from ? data.subspan(static_cast<std::size_t>(from), static_cast<std::size_t>(to - from)) : ByteView{};
        const bool named = sp.name && !sp.name->empty() && !sp.name->starts_with("Ord");
        side.add(make_function(32, std::format("{:x}", va), named ? *sp.name : std::format("sub_{:06X}", va - low), named, body_of(versions_decode(32, body, va), va, hi), va, hi,
                               around));
    }
    side.finish();
    return side;
}

Side load_ne(const std::filesystem::path& path) {
    const formats::ne::Module m = formats::ne::Module::open_file(path);
    std::map<std::uint16_t, formats::ne::Segment> segs;
    for (const formats::ne::Segment& s : m.segments()) segs[s.index] = s;
    std::map<std::uint32_t, formats::ne::Entry> entries;
    for (const formats::ne::Entry& e : m.entries())
        if (e.kind != formats::ne::Entry::Kind::Unused) entries[e.ordinal] = e;
    const std::vector<std::string> refs = m.module_refs();
    std::map<std::pair<std::uint32_t, std::uint32_t>, std::string> exported;   // (segment, offset) -> name; the first name stands
    {
        const auto resident = m.resident_names(), nonresident = m.nonresident_names();
        std::vector<formats::ne::Name> all;
        if (resident.size() > 1) all.insert(all.end(), resident.begin() + 1, resident.end());
        if (nonresident.size() > 1) all.insert(all.end(), nonresident.begin() + 1, nonresident.end());
        for (const formats::ne::Name& no : all) {
            const auto it = entries.find(no.ordinal);
            if (it != entries.end()) exported.try_emplace({it->second.segment, it->second.offset}, no.name);
        }
    }
    std::map<std::uint32_t, std::set<std::int64_t>> starts;
    for (const auto& [place, how] : decompile::describe::find_functions(m)) starts[place.seg].insert(place.off);
    std::int64_t code_total = 0;
    for (const auto& [idx, s] : segs)
        if (!s.is_data() && s.file_offset) code_total += s.length;
    Side side(path, Kind::Ne, code_total);
    for (auto& [idx, offsets] : starts) {
        const auto sit = segs.find(static_cast<std::uint16_t>(idx));
        if (sit == segs.end() || sit->second.is_data() || !sit->second.file_offset) continue;
        const formats::ne::Segment& s = sit->second;
        const ByteView code = stored_segment(m, s);
        const std::int64_t length = static_cast<std::int64_t>(code.size());
        const auto sites = sites_of(m, s, code, segs, entries, refs);
        std::set<std::int64_t> offs;
        for (std::int64_t o : offsets)
            if (o < length) offs.insert(o);
        std::map<std::int64_t, std::pair<std::vector<Ins>, std::int64_t>> bodies;   // start -> (instructions, end)
        for (int round = 0; round < 8; ++round) {   // near calls name more starts
            const std::vector<std::int64_t> order(offs.begin(), offs.end());
            std::set<std::int64_t> fresh;
            for (std::size_t n = 0; n < order.size(); ++n) {
                const std::int64_t lo = order[n], hi = n + 1 < order.size() ? order[n + 1] : length;
                const auto b = bodies.find(lo);
                if (b != bodies.end() && b->second.second == hi) continue;
                auto body = body_of(versions_decode(16, code.subspan(static_cast<std::size_t>(lo), static_cast<std::size_t>(hi - lo)), lo), lo, hi);
                for (const Ins& ins : body) {
                    if (ins.mnemonic == "call" && !ins.operands.empty() && ins.operands[0].type == kOpImm) {
                        const std::int64_t t = ins.operands[0].imm;
                        if (t < length && !offs.count(t)) fresh.insert(t);
                    }
                }
                bodies[lo] = {std::move(body), hi};
            }
            if (fresh.empty() || round == 7) break;
            offs.insert(fresh.begin(), fresh.end());
        }
        Surroundings around;
        around.relocated = [&](std::int64_t a) { return sites.count(a) != 0; };
        around.direct = [&](std::int64_t t) -> std::optional<std::string> { return offs.count(t) ? std::optional<std::string>(key16(idx, t)) : std::nullopt; };
        around.site = [&](std::int64_t a) -> std::optional<Ref> {
            const auto it = sites.find(a);
            return it == sites.end() ? std::nullopt : std::optional<Ref>(it->second);
        };
        around.slot = [](std::int64_t) { return std::nullopt; };
        around.inside = [](std::int64_t) { return false; };
        for (std::int64_t lo : offs) {
            const auto& bh = bodies.at(lo);
            const auto ex = exported.find({idx, static_cast<std::uint32_t>(lo)});
            const bool named = ex != exported.end() && !ex->second.empty();
            side.add(make_function(16, key16(idx, lo), named ? ex->second : std::format("sub_{}_{:04X}", idx, lo), named, bh.first, lo, bh.second, around));
        }
    }
    side.finish();
    return side;
}

Side load_dos(const std::filesystem::path& path) {
    const formats::dos::Program p = formats::dos::read_program(fs::read_file(path), path.filename().string());
    const analyze::dos_functions::Analysis analysis = analyze::dos_functions::discover(p);
    std::vector<std::uint32_t> starts;
    for (const auto& [lin, fn] : analysis.functions) starts.push_back(lin);   // (ascending)
    const std::int64_t end = static_cast<std::int64_t>(p.base) + static_cast<std::int64_t>(p.image.size());
    std::int64_t code = 0;
    for (const formats::dos::Span& sp : p.spans) code += sp.length;
    Side side(path, Kind::Dos, code);
    std::vector<std::uint32_t> relocs = p.relocations;
    std::sort(relocs.begin(), relocs.end());
    for (std::size_t n = 0; n < starts.size(); ++n) {
        const std::int64_t lin = starts[n];
        const analyze::dos_functions::Function& fn = analysis.functions.at(starts[n]);
        const std::int64_t seg = fn.cs;
        const std::int64_t hi = n + 1 < starts.size() ? static_cast<std::int64_t>(starts[n + 1]) : end;
        if (!(static_cast<std::int64_t>(p.base) <= lin && lin < end) || !(0 <= lin - seg && lin - seg < 0x10000)) continue;
        const std::int64_t lo_off = lin - seg, hi_off = std::min<std::int64_t>(hi - seg, 0x10000);
        std::unordered_set<std::int64_t> relocated;   // lookup only
        for (auto it = std::lower_bound(relocs.begin(), relocs.end(), static_cast<std::uint32_t>(lin)); it != relocs.end() && *it < hi; ++it)
            relocated.insert(static_cast<std::int64_t>(*it) - seg);
        const std::int64_t from = lin - static_cast<std::int64_t>(p.base), to = std::min<std::int64_t>(hi - static_cast<std::int64_t>(p.base), static_cast<std::int64_t>(p.image.size()));
        const ByteView body_bytes = to > from ? ByteView(p.image).subspan(static_cast<std::size_t>(from), static_cast<std::size_t>(to - from)) : ByteView{};
        Surroundings around;
        around.relocated = [&](std::int64_t a) { return relocated.count(a) != 0; };
        around.direct = [&](std::int64_t t) -> std::optional<std::string> {
            return analysis.functions.contains(static_cast<std::uint32_t>(seg + t)) ? std::optional<std::string>(std::format("{:05x}", seg + t)) : std::nullopt;
        };
        around.site = [](std::int64_t) { return std::nullopt; };
        around.slot = [](std::int64_t) { return std::nullopt; };
        around.inside = [](std::int64_t) { return false; };
        const bool named = !fn.name.empty() && !fn.name.starts_with("sub_");
        side.add(make_function(16, std::format("{:05x}", lin), named ? fn.name : std::format("sub_{:05X}", lin), named,
                               body_of(versions_decode(16, body_bytes, lo_off), lo_off, hi_off), lo_off, hi_off, around));
    }
    side.finish();
    return side;
}

}  // namespace

Side load(const std::filesystem::path& path) {
    const std::optional<formats::detect::Lane> kind = formats::detect::kind(path);
    if (kind == formats::detect::Lane::Pe) return load_pe(path);
    if (kind == formats::detect::Lane::Ne) return load_ne(path);
    if (kind == formats::detect::Lane::Dos) return load_dos(path);
    throw UnsupportedError("versions", 0, std::format("{}: functions of a {} file are not compared yet", path.filename().string(),
                                                      kind ? formats::detect::lane_name(*kind) : "non-executable"));
}

std::vector<Shared> shared(std::span<const std::filesystem::path> paths, Level level, std::size_t least) {
    struct Entry {
        std::string hash;
        Shared found;
    };
    std::vector<Entry> table;                                 // by the hash, in the order met
    std::unordered_map<std::string, std::size_t> by_hash;     // lookup only
    for (const std::filesystem::path& path : paths) {
        const Side side = load(path);
        for (const Function& f : side.functions()) {
            if (f.text.size() < least) continue;
            const std::string& h = f.ids.at(level);
            auto [it, fresh] = by_hash.try_emplace(h, table.size());
            if (fresh) table.push_back({h, {}});
            Shared& s = table[it->second].found;
            auto w = std::find_if(s.where.begin(), s.where.end(), [&](const auto& p) { return p.first == side.name(); });
            if (w == s.where.end()) w = s.where.insert(s.where.end(), {side.name(), {}});
            w->second.push_back(f.key);
            s.instructions = f.text.size();
        }
    }
    std::vector<Shared> out;
    for (Entry& e : table)
        if (e.found.where.size() >= 2) out.push_back(std::move(e.found));
    std::stable_sort(out.begin(), out.end(), [](const Shared& a, const Shared& b) {
        if (a.instructions != b.instructions) return a.instructions > b.instructions;
        std::vector<std::string_view> na, nb;
        for (const auto& w : a.where) na.push_back(w.first);
        for (const auto& w : b.where) nb.push_back(w.first);
        std::sort(na.begin(), na.end());
        std::sort(nb.begin(), nb.end());
        return na < nb;
    });
    return out;
}

// ---- matching -------------------------------------------------------------------------------------------------------------
namespace {

// 0..1: how alike two shapes are (1 = the same sequence); 0 for a pair too unlike to be worth the exact ratio.
double ratio_of(const std::vector<std::string>& sa, const std::vector<std::string>& sb) {
    Interner in;
    const std::vector<int> a = in.all(sa), b = in.all(sb);
    SeqMatcher m(a, b);
    return m.real_quick_ratio() >= 0.3 && m.quick_ratio() >= 0.3 ? m.ratio() : 0.0;
}

// the runs of five shapes of a function, each as a number that is the same in every process
std::set<std::uint64_t> grams_of(const Function& f, std::size_t n = 5) {
    std::set<std::uint64_t> out;   // sorted: the order candidates are looked up in
    for (std::size_t i = 0; i + n <= f.count(); ++i) {
        std::string joined;
        for (std::size_t k = 0; k < n; ++k) joined += (k ? "\n" : "") + f.shape[i + k];
        std::uint64_t v = 0;
        for (std::uint8_t c : blake2b8(joined)) v = (v << 8) | c;
        out.insert(v);
    }
    return out;
}

constexpr std::size_t kNone = static_cast<std::size_t>(-1);

class Matcher {
public:
    Matcher(const Side& a, const Side& b, double least)
        : a_(a), b_(b), least_(least), pair_(a.count(), kNone), back_(b.count(), kNone) {}

    Pairing run() {
        unique([](const Function& f) { return f.named ? std::optional<std::string_view>(f.name) : std::nullopt; }, How::Name, 0);
        unique([](const Function& f) { return std::optional<std::string_view>(f.ids.bytes); }, How::Bytes, 4);
        unique([](const Function& f) { return std::optional<std::string_view>(f.ids.text); }, How::Text, 4);
        unique([](const Function& f) { return std::optional<std::string_view>(f.ids.shape); }, How::Shape, 8);
        for (int round = 0; round < 64; ++round) {
            const std::size_t n = links_.size();
            by_calls();
            if (links_.size() == n) by_likeness();
            if (links_.size() == n) by_order();
            if (links_.size() == n) break;
        }
        by_gaps();
        by_calls();
        return {links_};
    }

    double likeness(std::size_t x, std::size_t y) {
        const Function& fa = a_.function(x);
        const Function& fb = b_.function(y);
        if (fa.ids.shape == fb.ids.shape) return 1.0;
        const std::uint64_t key = (static_cast<std::uint64_t>(x) << 32) | y;
        const auto it = memo_.find(key);
        if (it != memo_.end()) return it->second;
        return memo_[key] = ratio_of(fa.shape, fb.shape);
    }

private:
    void link(std::size_t x, std::size_t y, How how) {
        const Function& fa = a_.function(x);
        const Function& fb = b_.function(y);
        if (pair_[x] != kNone || back_[y] != kNone || (fa.named && fb.named && fa.name != fb.name)) return;
        pair_[x] = y;
        back_[y] = x;
        links_.push_back({x, y, how});
    }

    // A function of a with the key `of` gives and none other has, is paired with the one of b that alone has it.
    template <class Of>
    void unique(const Of& of, How how, std::size_t least_count) {
        std::unordered_map<std::string_view, std::vector<std::size_t>> in_b;   // lookup only
        for (std::size_t y = 0; y < b_.count(); ++y) {
            const Function& f = b_.function(y);
            if (back_[y] == kNone && f.count() >= least_count)
                if (auto k = of(f)) in_b[*k].push_back(y);
        }
        std::unordered_map<std::string_view, std::vector<std::size_t>> in_a;
        std::vector<std::string_view> order;   // the keys of a in the order first met
        for (std::size_t x = 0; x < a_.count(); ++x) {
            const Function& f = a_.function(x);
            if (pair_[x] == kNone && f.count() >= least_count)
                if (auto k = of(f)) {
                    auto [it, fresh] = in_a.try_emplace(*k);
                    if (fresh) order.push_back(*k);
                    it->second.push_back(x);
                }
        }
        for (std::string_view k : order) {
            const auto& xs = in_a[k];
            const auto b = in_b.find(k);
            if (xs.size() == 1 && b != in_b.end() && b->second.size() == 1) link(xs[0], b->second[0], how);
        }
    }

    // How many of the calls of two functions agree with the pairs so far, and how many do not.
    std::pair<std::int64_t, std::int64_t> calls_agree(const Function& fa, const Function& fb) const {
        std::int64_t plus = 0, minus = 0;
        if (fa.calls.size() != fb.calls.size()) return {0, 0};
        for (std::size_t i = 0; i < fa.calls.size(); ++i) {
            const Call& ca = fa.calls[i];
            const Call& cb = fb.calls[i];
            if (ca.to != cb.to) {
                ++minus;
            } else if (ca.to == Call::To::Import) {
                if (ca == cb) ++plus;
                else ++minus;
            } else if (const auto x = a_.index_of(ca.key); x && pair_[*x] != kNone) {
                if (b_.function(pair_[*x]).key == cb.key) ++plus;
                else ++minus;
            } else if (const auto y = b_.index_of(cb.key); y && back_[*y] != kNone) {
                ++minus;
            }
        }
        return {plus, minus};
    }

    // Pair what the calls of already paired functions say: the callee in the same place of the call list, or the only unpaired
    // caller of a pair.
    void by_calls() {
        struct Vote {
            std::size_t x, y;
            std::int64_t n;
        };
        std::vector<Vote> votes;   // in the order first voted for
        std::unordered_map<std::uint64_t, std::size_t> vindex;   // lookup only
        auto vote = [&](std::size_t x, std::size_t y, std::int64_t n) {
            auto [it, fresh] = vindex.try_emplace((static_cast<std::uint64_t>(x) << 32) | y, votes.size());
            if (fresh) votes.push_back({x, y, n});
            else votes[it->second].n += n;
        };
        Interner imports;
        for (const Link& l : links_) {
            const std::size_t x = l.from, y = l.to;
            const Function& fx = a_.function(x);
            const Function& fy = b_.function(y);
            if (!fx.calls.empty() && !fy.calls.empty()) {
                // the callee of each call as a number: an import by its name, a paired function by its partner, the rest are one
                // unknown (so two unknowns at the same place are taken for each other)
                std::vector<int> la, lb;
                std::vector<bool> ua, ub;
                for (const Call& c : fx.calls) {
                    const auto t = c.to == Call::To::Function ? a_.index_of(c.key) : std::nullopt;
                    if (c.to == Call::To::Import) { la.push_back(imports.of("i\n" + c.key)); ua.push_back(false); }
                    else if (t && pair_[*t] != kNone) { la.push_back(static_cast<int>(kOffset + pair_[*t])); ua.push_back(false); }
                    else { la.push_back(kUnknown); ua.push_back(true); }
                }
                for (const Call& c : fy.calls) {
                    const auto t = c.to == Call::To::Function ? b_.index_of(c.key) : std::nullopt;
                    if (c.to == Call::To::Import) { lb.push_back(imports.of("i\n" + c.key)); ub.push_back(false); }
                    else if (t && back_[*t] != kNone) { lb.push_back(static_cast<int>(kOffset + *t)); ub.push_back(false); }
                    else { lb.push_back(kUnknown); ub.push_back(true); }
                }
                dense(la, lb);
                std::vector<SeqMatcher::Opcode> ops;
                if (la == lb) ops.push_back({SeqMatcher::Tag::Equal, 0, static_cast<int>(la.size()), 0, static_cast<int>(lb.size())});
                else ops = SeqMatcher(la, lb).opcodes();
                for (const auto& op : ops) {
                    if (op.tag == SeqMatcher::Tag::Equal || (op.tag == SeqMatcher::Tag::Replace && op.i2 - op.i1 == op.j2 - op.j1)) {
                        for (int i = op.i1, j = op.j1; i < op.i2; ++i, ++j) {
                            if (!ua[static_cast<std::size_t>(i)] || !ub[static_cast<std::size_t>(j)]) continue;
                            const auto cx = a_.index_of(fx.calls[static_cast<std::size_t>(i)].key);
                            const auto cy = b_.index_of(fy.calls[static_cast<std::size_t>(j)].key);
                            if (cx && cy) vote(*cx, *cy, op.tag == SeqMatcher::Tag::Equal ? 2 : 1);
                        }
                    }
                }
            }
            std::vector<std::size_t> unpaired_a, unpaired_b;
            for (std::size_t c : a_.callers_of(x)) if (pair_[c] == kNone) unpaired_a.push_back(c);
            for (std::size_t c : b_.callers_of(y)) if (back_[c] == kNone) unpaired_b.push_back(c);
            if (unpaired_a.size() == 1 && unpaired_b.size() == 1) vote(unpaired_a[0], unpaired_b[0], 1);
        }
        // each function's best vote, as (votes, likeness); a vote counts when both ends of it are each other's best
        struct Best {
            std::pair<std::int64_t, double> score{0, 0};
            std::size_t other = kNone;
        };
        std::vector<std::size_t> best_a_order;   // the keys in the order first offered
        std::unordered_map<std::size_t, Best> best_a, best_b;   // lookup only
        auto offer = [](std::unordered_map<std::size_t, Best>& m, std::vector<std::size_t>* order, std::size_t k, std::pair<std::int64_t, double> score, std::size_t other) {
            const auto it = m.find(k);
            const std::pair<std::int64_t, double> cur = it == m.end() ? std::pair<std::int64_t, double>{0, 0} : it->second.score;
            if (score > cur) {
                if (it == m.end() && order) order->push_back(k);
                m[k] = {score, other};
            }
        };
        for (const Vote& vt : votes) {
            const Function& fx = a_.function(vt.x);
            const Function& fy = b_.function(vt.y);
            const bool same = fx.ids.bytes == fy.ids.bytes || fx.ids.text == fy.ids.text || fx.ids.shape == fy.ids.shape;
            const std::pair<std::int64_t, double> score{vt.n + (same ? 2 : 0), !same ? likeness(vt.x, vt.y) : 1.0};
            if ((!same && score.second < 0.4) || (fx.named && fy.named && fx.name != fy.name)) continue;
            offer(best_a, &best_a_order, vt.x, score, vt.y);
            offer(best_b, nullptr, vt.y, score, vt.x);
        }
        for (std::size_t x : best_a_order) {
            const std::size_t y = best_a[x].other;
            if (best_b[y].other == x) link(x, y, How::Calls);
        }
    }

    // Pair what is alike in shape, from what is distinctive in each (an import it calls, a constant it uses, a run of five
    // instructions' shapes): of the candidates sharing most with a function, the likeness decides, and only a pair that is
    // each other's clear best is made.
    void by_likeness() {
        std::unordered_map<std::string_view, std::vector<std::size_t>> posting_marks;   // lookup only
        std::map<std::uint64_t, std::vector<std::size_t>> posting_grams;                // by run id
        for (std::size_t y = 0; y < b_.count(); ++y) {
            const Function& f = b_.function(y);
            if (back_[y] != kNone || f.count() < 5) continue;
            for (const std::string& m : f.marks) posting_marks[m].push_back(y);
            for (std::uint64_t g : grams_of(f)) posting_grams[g].push_back(y);
        }
        struct Scored {
            double score;
            std::size_t other;
        };
        std::vector<std::size_t> order_a;
        std::unordered_map<std::size_t, std::vector<Scored>> of_a, of_b;   // lookup only
        for (std::size_t x = 0; x < a_.count(); ++x) {
            const Function& f = a_.function(x);
            if (pair_[x] != kNone || f.count() < 5) continue;
            std::vector<std::pair<std::size_t, std::int64_t>> cand;   // in the order first met
            std::unordered_map<std::size_t, std::size_t> cidx;        // lookup only
            auto hit = [&](const std::vector<std::size_t>& hits) {
                if (hits.size() > 40) return;
                for (std::size_t h : hits) {
                    auto [it, fresh] = cidx.try_emplace(h, cand.size());
                    if (fresh) cand.emplace_back(h, 1);
                    else cand[it->second].second++;
                }
            };
            for (const std::string& m : f.marks) {   // sorted marks, then sorted run ids
                const auto it = posting_marks.find(m);
                if (it != posting_marks.end()) hit(it->second);
            }
            for (std::uint64_t g : grams_of(f)) {
                const auto it = posting_grams.find(g);
                if (it != posting_grams.end()) hit(it->second);
            }
            std::stable_sort(cand.begin(), cand.end(), [](const auto& p, const auto& q) { return p.second > q.second; });   // most_common
            for (std::size_t ci = 0; ci < cand.size() && ci < 10; ++ci) {
                const std::size_t y = cand[ci].first;
                const Function& g = b_.function(y);
                if (2 * std::min(f.count(), g.count()) < std::max(f.count(), g.count()) || (f.named && g.named && f.name != g.name)) continue;
                const double v = likeness(x, y);
                const auto [plus, minus] = calls_agree(f, g);
                const bool share = std::any_of(f.marks.begin(), f.marks.end(), [&](const std::string& m) { return g.marks.count(m) != 0; });
                if (v < least_ || minus > plus || (f.count() < 12 && !plus && !share)) continue;   // a small one needs more than its shape
                const double t = f.ids.text == g.ids.text ? 1.0 : ratio_of(f.text, g.text);
                const double score = (v + t) / 2 + (plus ? 0.1 : 0.0);
                if (!of_a.contains(x)) order_a.push_back(x);
                of_a[x].push_back({score, y});
                of_b[y].push_back({score, x});
            }
        }
        // one candidate, or one well ahead
        auto clear = [&](std::vector<Scored>& found, const Side& other_side) {
            std::stable_sort(found.begin(), found.end(), [&](const Scored& p, const Scored& q) {
                if (p.score != q.score) return p.score > q.score;
                return other_side.function(q.other).key < other_side.function(p.other).key;   // (the later key first, as before)
            });
            return found.size() == 1 || found[0].score - found[1].score >= 0.03;
        };
        for (std::size_t x : order_a) {
            auto& found = of_a[x];
            if (!clear(found, b_)) continue;
            const std::size_t y = found[0].other;
            auto& back = of_b[y];
            if (clear(back, a_) && back[0].other == x) link(x, y, How::Likeness);
        }
    }

    // Unmatched functions of the same shape standing in the same place in both builds.
    void by_order() {
        Interner in;
        std::vector<int> la, lb;
        for (std::size_t x = 0; x < a_.count(); ++x)
            la.push_back(pair_[x] != kNone ? in.of("p\n" + b_.function(pair_[x]).key) : in.of("s\n" + a_.function(x).ids.shape));
        for (std::size_t y = 0; y < b_.count(); ++y)
            lb.push_back(back_[y] != kNone ? in.of("p\n" + b_.function(y).key) : in.of("s\n" + b_.function(y).ids.shape));
        for (const auto& blk : SeqMatcher(la, lb).matching_blocks()) {
            std::vector<std::pair<std::size_t, std::size_t>> free_ones;
            for (int k = 0; k < blk.k; ++k) {
                const std::size_t x = static_cast<std::size_t>(blk.i + k);
                if (pair_[x] == kNone) free_ones.emplace_back(x, static_cast<std::size_t>(blk.j + k));
            }
            if (!free_ones.empty() && (static_cast<int>(free_ones.size()) < blk.k || blk.k >= 3))   // beside a matched one, or three in a row
                for (const auto& [x, y] : free_ones) link(x, y, How::Position);
        }
    }

    // The same few unmatched functions between two matched neighbours, if at all alike.
    void by_gaps() {
        std::vector<std::size_t> run;
        std::ptrdiff_t last = -1;
        for (std::size_t x = 0; x <= a_.count(); ++x) {
            const bool end = x == a_.count();
            if (!end && pair_[x] == kNone) {
                run.push_back(x);
                continue;
            }
            const std::ptrdiff_t here = end ? static_cast<std::ptrdiff_t>(b_.count()) : static_cast<std::ptrdiff_t>(pair_[x]);
            std::vector<std::size_t> gap;
            if (here > last)
                for (std::ptrdiff_t k = last + 1; k < here; ++k)
                    if (back_[static_cast<std::size_t>(k)] == kNone) gap.push_back(static_cast<std::size_t>(k));
            if (!run.empty() && run.size() == gap.size() && run.size() <= 4)
                for (std::size_t i = 0; i < run.size(); ++i)
                    if (likeness(run[i], gap[i]) >= 0.4) link(run[i], gap[i], How::Position);
            run.clear();
            last = std::max(last, here);
        }
    }

    using Link = Pairing::Link;
    static constexpr int kUnknown = -1;
    static constexpr std::size_t kOffset = 1u << 20;   // the numbers of paired functions are above the numbers of imports

    // Number the elements of two sequences densely from 0 (the matcher indexes by element).
    static void dense(std::vector<int>& a, std::vector<int>& b) {
        std::unordered_map<int, int> ids;   // lookup only
        auto number = [&](int& x) { x = ids.try_emplace(x, static_cast<int>(ids.size())).first->second; };
        for (int& x : a) number(x);
        for (int& x : b) number(x);
    }

    const Side& a_;
    const Side& b_;
    double least_;
    std::vector<std::size_t> pair_, back_;   // the partner of each function, or kNone
    std::vector<Link> links_;                // in the order they were made
    std::unordered_map<std::uint64_t, double> memo_;   // likeness by (x, y); lookup only
};

}  // namespace

double likeness(const Function& a, const Function& b) {
    if (a.ids.shape == b.ids.shape) return 1.0;
    return ratio_of(a.shape, b.shape);
}

Pairing match(const Side& a, const Side& b, double least) { return Matcher(a, b, least).run(); }

}  // namespace cb::compare::versions

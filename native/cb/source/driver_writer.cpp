// cb-unity: alone  (it names file-local helpers after what a driver is made of; the other writers have helpers of the same kind)
#include "cb/source/driver_writer.hpp"

#include <algorithm>
#include <format>
#include <memory>

#include "cb/build/vxdfile.hpp"
#include "cb/core/arena.hpp"
#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/source/image_writer.hpp"
#include <nlohmann/json.hpp>

namespace cb::source::driver_writer {
namespace {

namespace le = formats::le;
using Off = Writer::Off;

// Stands in for a patched address while decoding, so the operand a field belongs to can be recognised in the decoder's text.
constexpr std::uint32_t kMark = 0x7A000000;
constexpr std::size_t kMaxRoundsDefault = 40;
// Opcodes common enough that one bad encoding does not mean every instance is bad; a rarer one the assembler rejects (a privileged or system
// instruction) is rejected wherever it appears, so all instances of it are written as bytes together.
bool is_common_opcode(std::string_view m) {
    static constexpr std::string_view common[] = {"mov", "add", "sub", "cmp", "lea", "push", "pop", "call", "jmp", "test", "and", "or", "xor", "inc",
                                                  "dec", "ret", "nop", "mov zx", "movzx", "movsx", "shl", "shr", "adc", "sbb"};
    return std::ranges::find(common, m) != std::end(common);
}

std::string attributes_of(std::uint32_t flags) {
    std::string words = (flags & 0x40) != 0 ? "preload" : "loadoncall";
    static constexpr std::pair<std::uint32_t, std::string_view> extra[] = {{0x10, "discardable"}, {0x20, "shared"}, {0x8000, "iopl"}, {0x4000, "conforming"}};
    for (const auto& [bit, word] : extra) {
        if ((flags & bit) != 0) words += " " + std::string(word);
    }
    return words;
}

// Is what was decoded really a piece of text? (a message table sits in an executable object, and letters decode as instructions)
bool reads_as_text(const std::map<Off, Writer::Instruction>& found) {
    Bytes data;
    for (const auto& [at, ins] : found) append(data, ins.bytes);
    if (data.size() < 6) return false;
    std::size_t text_bytes = 0;
    for (const std::uint8_t b : data) {
        if ((b >= 0x20 && b < 0x7F) || b == 0 || b == 10 || b == 13) ++text_bytes;
    }
    return static_cast<double>(text_bytes) >= 0.85 * static_cast<double>(data.size());
}

std::string hex_data(std::uint8_t b) { return std::format("0{:02X}h", b); }

std::string db_line(const std::vector<std::uint8_t>& run) {
    std::string out = "        db   ";
    for (std::size_t i = 0; i < run.size(); ++i) out += (i ? ", " : "") + hex_data(run[i]);
    return out;
}

std::string last_message_line(std::string_view messages, std::size_t at_most) {
    const std::vector<std::string_view> lines = text::lines(text::trim(messages));
    const std::string_view last = lines.empty() ? std::string_view("failed") : lines.back();
    return std::string(last.substr(0, at_most));
}

std::string with_crlf(std::string_view text_lf) { return text::replace_all(text_lf, "\n", "\r\n"); }

// A file of text as the assembler reads it: Latin-1, CRLF line ends.
void write_assembler_text(const std::filesystem::path& path, std::string_view text_lf) { fs::write_file(path, text::to_latin1(with_crlf(text_lf))); }

std::string joined_ids(const std::vector<std::uint32_t>& v) {
    std::string out = "[";
    for (std::size_t i = 0; i < v.size(); ++i) out += (i ? ", " : "") + std::to_string(v[i]);
    return out + "]";
}

}  // namespace

std::string placeholder_label(std::uint32_t device, std::uint32_t service) {
    const std::uint32_t n = service & 0x7FFF;
    return device == 1 ? std::format("VMM_{:04X}", n) : std::format("VxD_{:04X}_{:04X}", device, n);
}

Options options_for(const build::watcom::OpenWatcom& open_watcom, const std::filesystem::path& probes, ServiceLabel service_label) {
    Options o;
    o.tools.wasm = build::driver::tool(open_watcom, "wasm");
    o.tools.wlink = build::driver::tool(open_watcom, "wlink");
    o.tools.env = open_watcom.environment(build::watcom::Target::Win32);
    o.probes = probes;
    o.service_label = std::move(service_label);
    return o;
}

// ---- reading the driver ---------------------------------------------------------------------------------------

Writer::Writer(const le::Module& image, const platforms::Decoder& decoder, ServiceLabel service_label)
    : image_(image), decoder_(decoder), service_label_(service_label ? std::move(service_label) : ServiceLabel(placeholder_label)) {
    descriptor_ = image.descriptor();
    for (const le::Object& o : image.objects()) {
        Object ob;
        ob.index = o.index;
        ob.flags = o.flags;
        ob.size = o.vsize;
        ob.data = image.object_data(o);
        ob.data.resize(ob.size, 0);
        ob.code = (o.flags & 4) != 0;
        ob.wide = (o.flags & 0x2000) != 0;
        objects_.emplace(o.index, std::move(ob));
    }
    for (const le::Reference& r : image.references()) {
        const auto from = objects_.find(r.at.object);
        const auto to = objects_.find(r.target_object);
        if (from == objects_.end() || to == objects_.end()) continue;
        const bool added = from->second.fix.insert_or_assign(r.at.offset, Fix{r.source, r.target_object, r.target_offset}).second;
        if (added) from->second.fix_order.push_back(r.at.offset);
        to->second.labels.insert(r.target_offset);
    }
    if (descriptor_) {
        if (const auto ob = objects_.find(descriptor_->where.object); ob != objects_.end()) ob->second.labels.insert(descriptor_->where.offset);
    }
    // everywhere the system or another driver can enter this one
    if (descriptor_) {
        const le::Descriptor& d = *descriptor_;
        if (d.at_control) entries_.emplace_back("control", *d.at_control);
        if (d.at_v86_api) entries_.emplace_back("v86_api", *d.at_v86_api);
        if (d.at_pm_api) entries_.emplace_back("pm_api", *d.at_pm_api);
        const std::vector<std::optional<le::Place>> table = image.service_table(d);
        for (std::size_t i = 0; i < table.size(); ++i) {
            if (table[i]) entries_.emplace_back(std::format("service_{}", i), *table[i]);
        }
    }
    const std::string device = descriptor_ ? std::string(text::trim(descriptor_->name)) : "DRV";
    std::vector<std::pair<std::string, Place>> by_label = entries_;
    std::ranges::sort(by_label, {}, &std::pair<std::string, Place>::first);   // (the names are given in the order of the labels)
    for (const auto& [label, at] : by_label) {
        std::string name = device + "_" + label;
        if (label.starts_with("service_") && descriptor_ && descriptor_->device_id != 0) {
            // a service this driver offers has a name of its own in the kit
            const std::optional<std::uint32_t> n = text::parse_int<std::uint32_t>(label.substr(8), 10);
            const std::string known = n ? service_label_(descriptor_->device_id, *n) : std::string();
            const bool placeholder = (known.starts_with("VMM_") || known.starts_with("VxD_")) && known.size() >= 8 &&
                                     std::all_of(known.begin() + 4, known.begin() + 8, [](char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'); });
            if (text::is_identifier(known) && !placeholder) name = known;
        }
        names_.emplace(at, name);   // (the first label of a place names it)
        if (const auto ob = objects_.find(at.object); ob != objects_.end()) ob->second.labels.insert(at.offset);
    }
    if (descriptor_) names_[descriptor_->where] = std::string(text::trim(descriptor_->name)) + "_DDB";

    decode();
    // places inside an instruction, and every name that will be needed, before writing starts
    for (const auto& [index, ob] : objects_) {
        for (const auto& [off, ins] : ob.ins) {
            for (Off x = off + 1; x < off + static_cast<Off>(ins.bytes.size()); ++x) within_[Place{index, x}] = off;
        }
    }
    places_known_ = true;
    for (auto& [index, ob] : objects_) {
        std::erase_if(ob.labels, [&](Off a) { return a < 0 || a > static_cast<Off>(ob.size); });
        for (const Off a : ob.fix_order) {
            const Fix& f = ob.fix.at(a);
            (void)label(f.object, f.offset);
        }
    }
}

std::size_t Writer::instruction_count() const {
    std::size_t n = 0;
    for (const auto& [index, ob] : objects_) n += ob.ins.size();
    return n;
}

const Writer::Fix* Writer::fix_at(const Object& object, Off at) const {
    const auto it = object.fix.find(at);
    return it == object.fix.end() ? nullptr : &it->second;
}

std::string Writer::label(std::uint32_t object, Off at) {
    Object& ob = objects_.at(object);
    Off base = at;
    const Off size = static_cast<Off>(ob.size);
    if (at < 0 || at > size) {
        base = at < 0 || at > 0x7FFFFFFF ? 0 : size;
        if (at > 0x7FFFFFFF) at -= Off{1} << 32;
    } else if (places_known_ && within_.contains(Place{object, at})) {
        base = within_.at(Place{object, at});
    } else {
        for (Off back = 1; back <= 3; ++back) {
            if (ob.fix.contains(at - back)) {
                base = at - back;
                break;
            }
        }
    }
    if (base != at) {
        ob.labels.insert(base);
        return std::format("{} {} {}", label(object, base), at > base ? "+" : "-", at > base ? at - base : base - at);
    }
    if (const auto named = names_.find(Place{object, at}); named != names_.end() && !named->second.empty()) return named->second;
    return std::format("L{}_{:05X}", object, at);
}

// ---- finding the code -----------------------------------------------------------------------------------------

void Writer::decode() {
    struct Item {
        std::uint32_t object;
        Off at;
        bool maybe;
    };
    std::vector<Item> work;
    for (const auto& [name, at] : entries_) work.push_back({at.object, at.offset, false});
    for (const auto& [index, ob] : objects_) {
        for (const Off a : ob.fix_order) {
            const Fix& f = ob.fix.at(a);
            if (objects_.at(f.object).code) work.push_back({f.object, f.offset, f.kind != 8});
        }
    }
    std::set<std::pair<std::uint32_t, Off>> tried;
    while (!work.empty()) {
        const Item item = work.back();
        work.pop_back();
        const auto found_object = objects_.find(item.object);
        if (found_object == objects_.end()) continue;
        Object& ob = found_object->second;
        if (!ob.code || !ob.wide || tried.contains({item.object, item.at}) || ob.ins.contains(item.at)) continue;
        tried.emplace(item.object, item.at);
        Followed f = follow(ob, item.at);
        if (item.maybe && (!f.clean || reads_as_text(f.found))) continue;   // a pointer into code that is not code
        for (auto& [off, ins] : f.found) ob.ins[off] = std::move(ins);
        for (const Off t : f.more) {
            ob.labels.insert(t);
            work.push_back({item.object, t, false});
        }
    }
    // drop instructions another instruction's bytes run into (data read as code by mistake)
    for (auto& [index, ob] : objects_) {
        std::set<Off> covered;
        for (const Off off : std::vector<Off>([&] {
                 std::vector<Off> keys;
                 for (const auto& [o, ins] : ob.ins) keys.push_back(o);
                 return keys;
             }())) {
            const Off n = static_cast<Off>(ob.ins.at(off).bytes.size());
            bool overlaps = false;
            for (Off x = off; x < off + n && !overlaps; ++x) overlaps = covered.contains(x);   // ...or that something points into the middle of
            for (Off x = off + 1; x < off + n && !overlaps; ++x) overlaps = ob.labels.contains(x);
            if (overlaps) {
                ob.ins.erase(off);
                continue;
            }
            for (Off x = off; x < off + n; ++x) covered.insert(x);
        }
        // ...and jumps to places that are not the start of anything (so not code either)
        for (bool again = true; again;) {
            again = false;
            std::set<Off> inside;
            for (const auto& [off, ins] : ob.ins) {
                for (Off x = off + 1; x < off + static_cast<Off>(ins.bytes.size()); ++x) inside.insert(x);
            }
            for (const auto& [a, f] : ob.fix) {
                for (Off k = 1; k <= 3; ++k) inside.insert(a + k);
            }
            std::vector<Off> doomed;
            for (const auto& [off, ins] : ob.ins) {
                if (ins.target && inside.contains(*ins.target)) doomed.push_back(off);
            }
            for (const Off off : doomed) {
                ob.ins.erase(off);
                again = true;
            }
        }
        for (const auto& [a, f] : ob.fix) {
            for (Off k = 1; k <= 3; ++k) ob.labels.erase(a + k);
        }
    }
}

// The instruction at `at`, decoded once: where control goes on, and the instruction -- or nothing where nothing can be decoded.
const std::optional<Writer::Step>& Writer::step(Object& ob, Off at) {
    std::unordered_map<Off, std::optional<Step>>& memo = memo_[ob.index];
    if (const auto hit = memo.find(at); hit != memo.end()) return hit->second;
    std::optional<Step> out;
    const Off size = static_cast<Off>(ob.size);
    const auto u16_at = [&](Off o) { return static_cast<std::uint32_t>(ob.data[static_cast<std::size_t>(o)]) | (static_cast<std::uint32_t>(ob.data[static_cast<std::size_t>(o) + 1]) << 8); };
    if (at + 6 <= size && ob.data[static_cast<std::size_t>(at)] == 0xCD && ob.data[static_cast<std::size_t>(at) + 1] == 0x20) {
        bool patched = false;
        for (Off x = at; x < at + 6 && !patched; ++x) patched = ob.fix.contains(x);
        if (!patched) {
            const std::uint32_t svc = u16_at(at + 2), dev = u16_at(at + 4);
            if (dev > 0 && dev < 0x8000 && (svc & 0x7FFF) < 0x800) {
                ob.services[at] = ServiceCall{dev, svc & 0x7FFF, (svc & 0x8000) != 0};
                Instruction ins;
                ins.bytes.assign(ob.data.begin() + at, ob.data.begin() + at + 6);
                ins.mnemonic = "service";
                out = Step{std::move(ins), (svc & 0x8000) != 0 ? std::nullopt : std::optional<Off>(at + 6)};
            }
        }
    }
    if (!out) {
        // patched fields read as a marker, so the operand they belong to can be recognised
        const std::size_t first = static_cast<std::size_t>(at);
        Bytes window(ob.data.begin() + at, ob.data.begin() + std::min<Off>(at + 16, size));
        for (Off x = at; x < std::min<Off>(at + 13, size); ++x) {
            if (!ob.fix.contains(x)) continue;
            const std::size_t where = static_cast<std::size_t>(x) - first;
            if (where + 4 > window.size()) continue;   // (a field that runs past the end of the object: no instruction can hold it)
            const std::uint32_t marker = kMark + (static_cast<std::uint32_t>(x) & 0xFFFFF);
            for (std::size_t k = 0; k < 4; ++k) window[where + k] = static_cast<std::uint8_t>(marker >> (8 * k));
        }
        struct Decoded {
            std::size_t size = 0;
            std::string mnemonic, op_str;
            bool flow = false;
            std::optional<std::int64_t> first_immediate;
        };
        std::optional<Decoded> d;
        decoder_.each(window, static_cast<std::uint64_t>(at), 1, [&](const cs_insn& c) {
            Decoded x;
            x.size = c.size;
            x.mnemonic = c.mnemonic;
            x.op_str = c.op_str;
            if (c.detail != nullptr) {
                for (int g = 0; g < c.detail->groups_count; ++g) {
                    if (c.detail->groups[g] == CS_GRP_JUMP || c.detail->groups[g] == CS_GRP_CALL) x.flow = true;
                }
                if (c.detail->x86.op_count > 0 && c.detail->x86.operands[0].type == X86_OP_IMM) x.first_immediate = c.detail->x86.operands[0].imm;
            }
            d = std::move(x);
            return false;
        });
        if (d && at + static_cast<Off>(d->size) <= size) {
            std::vector<Off> inside;
            for (Off x = at; x < at + static_cast<Off>(d->size); ++x) {
                if (ob.fix.contains(x)) inside.push_back(x);
            }
            bool refused = false;   // a field that runs out of the instruction, or one that began just before it
            for (const Off x : inside) refused = refused || x + 4 > at + static_cast<Off>(d->size);
            for (Off k = 1; k <= 3; ++k) refused = refused || ob.fix.contains(at - k);
            if (!refused) {
                std::optional<Off> target;
                if (d->flow && d->first_immediate && inside.empty()) {
                    target = static_cast<Off>(static_cast<std::uint64_t>(*d->first_immediate) & 0xFFFFFFFF);
                    if (*target >= size) target.reset();
                }
                static constexpr std::string_view ends[] = {"ret", "retn", "retf", "iretd", "iret", "jmp", "hlt"};
                const bool stops = std::ranges::find(ends, d->mnemonic) != std::end(ends);
                Instruction ins;
                ins.bytes.assign(ob.data.begin() + at, ob.data.begin() + at + static_cast<Off>(d->size));
                ins.mnemonic = d->mnemonic;
                ins.operands = d->op_str;
                ins.target = target;
                out = Step{std::move(ins), stops ? std::nullopt : std::optional<Off>(at + static_cast<Off>(d->size))};
            }
        }
    }
    return memo.emplace(at, std::move(out)).first->second;
}

// Decode from `start` until control cannot continue.
Writer::Followed Writer::follow(Object& ob, Off start) {
    Followed out;
    Off a = start;
    while (a >= 0 && a < static_cast<Off>(ob.size) && !ob.ins.contains(a) && !out.found.contains(a)) {
        const std::optional<Step>& one = step(ob, a);
        if (!one) {
            out.clean = false;
            return out;
        }
        out.found.emplace(a, one->instruction);
        if (one->instruction.target) out.more.push_back(*one->instruction.target);
        if (!one->next) return out;
        a = *one->next;
    }
    return out;
}

// ---- writing --------------------------------------------------------------------------------------------------

std::optional<std::string> Writer::mnemonic(const Object& ob, Off at) {
    const Instruction& ins = ob.ins.at(at);
    const Off n = static_cast<Off>(ins.bytes.size());
    if (ins.mnemonic == "service") return std::nullopt;
    std::vector<Off> fixes;
    for (Off x = at; x < at + n; ++x) {
        if (ob.fix.contains(x)) fixes.push_back(x);
    }
    if (fixes.size() > 1) return std::nullopt;
    if (fixes.empty()) {
        if (const auto s = assembly::string_op(ins.mnemonic + " ")) return std::string(s->prefix) + std::string(s->op) + s->width;
    }
    if (ins.target) {
        // the distance is said, so the assembler keeps the original's size
        const std::string where = label(ob.index, *ins.target);
        static constexpr std::string_view loops[] = {"loop", "loope", "loopne", "jecxz", "jcxz"};
        if (std::ranges::find(loops, ins.mnemonic) != std::end(loops)) return std::format("{:<6} {}", ins.mnemonic, where);
        return std::format("{:<6} {} {}", ins.mnemonic, n == 2 ? "short" : "near ptr", where);
    }
    std::string ops = ins.operands;
    if (!fixes.empty()) {
        const Fix& f = ob.fix.at(fixes.front());
        const std::string marker = std::format("0x{:x}", kMark + (static_cast<std::uint32_t>(fixes.front()) & 0xFFFFF));
        if (ops.find(marker) == std::string::npos) return std::nullopt;
        const std::string name = label(f.object, f.offset);
        if (f.kind == 8) {
            ops = text::replace_all(ops, marker, name);
        } else {
            // inside square brackets the label is the address; anywhere else it is the offset
            bool in_memory = false;
            for (std::size_t p = ops.find(marker); p != std::string::npos && !in_memory; p = ops.find(marker, p + 1)) {
                const std::size_t open = ops.rfind('[', p);
                in_memory = open != std::string::npos && ops.find(']', open) > p && ops.find(']', p + marker.size()) != std::string::npos;
            }
            ops = text::replace_all(ops, marker, in_memory ? name : "offset " + name);
        }
    }
    ops = assembly::operands(ops);
    return std::string(text::rtrim(std::format("{:<6} {}", assembly::mnemonic(ins.mnemonic), ops)));
}

// An instruction as its bytes, each patched field as the label it holds.
std::vector<std::string> Writer::raw_lines(const Object& ob, Off at) {
    const Instruction& ins = ob.ins.at(at);
    const Off n = static_cast<Off>(ins.bytes.size());
    if (ins.mnemonic == "service") {
        const ServiceCall& call = ob.services.at(at);
        const std::uint32_t both = static_cast<std::uint32_t>(ins.bytes[2]) | (static_cast<std::uint32_t>(ins.bytes[3]) << 8) |
                                   (static_cast<std::uint32_t>(ins.bytes[4]) << 16) | (static_cast<std::uint32_t>(ins.bytes[5]) << 24);
        return {"        int  20h", std::format("        dd   0{:08X}h    ; {}{}", both, call.jump ? "jump to " : "", service_label_(call.device, call.service))};
    }
    std::vector<std::string> lines;
    std::vector<std::uint8_t> run;
    const auto flush = [&] {
        if (run.empty()) return;
        lines.push_back(db_line(run));
        run.clear();
    };
    for (Off x = at; x < at + n;) {
        if (const Fix* f = fix_at(ob, x)) {
            const std::string name = label(f->object, f->offset);
            if (f->kind == 8 && !run.empty() && (run.back() == 0xE8 || run.back() == 0xE9)) {
                const std::uint8_t op = run.back();
                run.pop_back();
                flush();
                lines.push_back(std::format("        {} {}", op == 0xE8 ? "call" : "jmp ", name));
            } else if (f->kind == 8 && run.size() > 1 && run[run.size() - 2] == 0x0F && run.back() >= 0x80 && run.back() <= 0x8F) {
                const std::string_view cc = assembly::kConditions[run.back() - 0x80];
                run.pop_back();
                run.pop_back();
                flush();
                lines.push_back(std::format("        j{:<3} near ptr {}", cc, name));
            } else {
                flush();
                lines.push_back(std::format("        dd   offset {}{}", name, f->kind == 8 ? " - ($ + 4)" : ""));
            }
            x += 4;
        } else {
            run.push_back(ins.bytes[static_cast<std::size_t>(x - at)]);
            ++x;
        }
    }
    flush();
    const std::string spelled = mnemonic(ob, at).value_or(std::string(text::trim(ins.mnemonic + " " + ins.operands)));
    if (!lines.empty()) lines.front() = std::format("{:<44} ; {}", lines.front(), spelled);
    return lines;
}

// {place: its label} of each place a function begins: the driver's entry points and every place a call lands (a near call inside an object, or
// one across objects through a fix-up), where code was found -- for the fragment each function is in the file's L2 tree.
std::map<Place, std::string> Writer::function_starts() {
    std::set<Place> found;
    for (const auto& [name, at] : entries_) found.insert(at);
    for (const auto& [index, ob] : objects_) {
        for (const auto& [off, ins] : ob.ins) {
            if (ins.mnemonic == "call" && ins.target) found.insert(Place{index, *ins.target});
        }
        for (const auto& [a, f] : ob.fix) {
            if (f.kind == 8 && a >= 1 && a - 1 < static_cast<Off>(ob.size) && ob.data[static_cast<std::size_t>(a) - 1] == 0xE8 && ob.ins.contains(a - 1))
                found.insert(Place{f.object, f.offset});
        }
    }
    std::map<Place, std::string> out;
    for (const Place& p : found) {
        const auto ob = objects_.find(p.object);
        if (ob == objects_.end() || !ob->second.ins.contains(p.offset) || !ob->second.labels.contains(p.offset)) continue;
        out.emplace(p, label(p.object, p.offset));
    }
    return out;
}

Writer::Text Writer::source(const std::set<Place>* good) {
    Text result;
    const std::string name = descriptor_ ? std::string(text::trim(descriptor_->name)) : "DRIVER";
    translate::l2::Assembly out(
        {std::format("; {} -- a virtual device driver, reconstructed from the image by CodeBreaker.", name),
         ";",
         "; One segment per object of the original. Nothing here is an address: every place the",
         "; loader patches is written as the label it refers to. Labels are L<object>_<offset in the",
         "; original>; names the image itself carries (entry points, the descriptor block) are used",
         "; where there are any. Instructions written as bytes are ones the assembler would encode",
         "; differently from the original; their mnemonic is in the comment.",
         "", ".386p", ".model flat", ""},
        "masm", name);   // (the file as one L2 tree)
    Counts& counts = result.counts;
    const std::map<Place, std::string> starts = function_starts();
    for (auto& [i, ob] : objects_) {
        out.part(std::format("S{}", i), "object", translate::ir::OriginSegment{name, i, 0}, ob.size);
        std::vector<Off> mine;
        for (const auto& [p, l] : starts) {
            if (p.object == i) mine.push_back(p.offset);
        }
        std::map<Off, Off> ends;   // (a function runs to the next one)
        for (std::size_t k = 0; k < mine.size(); ++k) ends[mine[k]] = k + 1 < mine.size() ? mine[k + 1] : static_cast<Off>(ob.size);
        out.append(std::format("; object {}: {} bytes, {}", i, ob.size, attributes_of(ob.flags)));
        out.append(std::format("S{} segment dword public {} 'O{}{}'", i, ob.wide ? "use32" : "use16", i, ob.code ? "CODE" : "DATA"));
        std::vector<std::uint8_t> run;
        const auto flush = [&] {
            out.extend(assembly::data_lines(run));
            run.clear();
        };
        const auto jump_at_label = [&](Off a) {
            // The kind of fix-up at `a` is a distance, and the bytes before it in the data are the opcode of a call or jump
            return a >= 0 && ob.labels.contains(a) && ob.fix.contains(a) && !ob.ins.contains(a) && ob.fix.at(a).kind == 8;
        };
        Off a = 0;
        while (a < static_cast<Off>(ob.size)) {
            if (jump_at_label(a) && run.size() > 1 && run[run.size() - 2] == 0x0F && run.back() >= 0x80 && run.back() <= 0x8F) {
                const std::string_view cc = assembly::kConditions[run.back() - 0x80];   // the same, for a conditional jump
                run.pop_back();
                run.pop_back();
                flush();
                const Fix& f = ob.fix.at(a);
                out.append(std::format("        j{:<3} near ptr {}", cc, label(f.object, f.offset)));
                out.append(std::format("{} equ $ - 4", label(i, a)));
                counts.data -= 2;
                a += 4;
                continue;
            }
            if (jump_at_label(a) && !run.empty() && (run.back() == 0xE8 || run.back() == 0xE9)) {
                // A call or jump among the data whose distance field is itself referred to (code that rewrites the jump): written as the
                // instruction, so the loader's patch is kept -- a `dd` of the distance to another object loses it -- and the label is the
                // four bytes before the instruction's end.
                const std::uint8_t op = run.back();
                run.pop_back();
                flush();
                const Fix& f = ob.fix.at(a);
                out.append(std::format("        {} {}", op == 0xE8 ? "call" : "jmp ", label(f.object, f.offset)));
                out.append(std::format("{} equ $ - 4", label(i, a)));
                counts.data -= 1;
                a += 4;
                continue;
            }
            if (ob.labels.contains(a)) {
                flush();
                if (ends.contains(a)) {   // (each function a fragment of its own)
                    out.part(starts.at(Place{i, a}), "function", translate::ir::OriginSegment{name, i, a}, static_cast<std::uint64_t>(ends.at(a) - a));
                }
                if (descriptor_ && Place{i, a} == descriptor_->where) out.append("public " + label(i, a));
                out.append(label(i, a) + ":");
            }
            if (ob.ins.contains(a) && !ob.fix.contains(a)) {   // (bytes the loader patches from their first are an address, not an instruction)
                flush();
                std::optional<std::string> text;
                if (good == nullptr || good->contains(Place{i, a})) text = mnemonic(ob, a);
                ++counts.instructions;
                if (!text) {
                    counts.raw += ob.ins.at(a).mnemonic != "service" ? 1 : 0;
                    out.extend(raw_lines(ob, a));
                } else {
                    out.append("        " + *text);
                    result.line_of[out.lines().size()] = Place{i, a};
                }
                a += static_cast<Off>(ob.ins.at(a).bytes.size());
            } else if (ob.fix.contains(a)) {
                flush();
                const Fix& f = ob.fix.at(a);
                out.append(std::format("        dd   offset {}{}", label(f.object, f.offset), f.kind == 8 ? " - ($ + 4)" : ""));
                a += 4;
            } else {
                run.push_back(ob.data[static_cast<std::size_t>(a)]);
                ++counts.data;
                ++a;
            }
        }
        flush();
        if (ob.labels.contains(static_cast<Off>(ob.size))) out.append(label(i, static_cast<Off>(ob.size)) + ":");
        out.append(std::format("S{} ends", i));
        out.append("");
    }
    out.part("end", "end");
    out.append("end");
    result.text = image_writer::text(out) + "\n";
    assembly_ = std::move(out);
    return result;
}

std::vector<std::pair<std::string, std::string>> Writer::classes() const {
    std::vector<std::pair<std::string, std::string>> out;
    for (const auto& [i, ob] : objects_) out.emplace_back(std::format("O{}{}", i, ob.code ? "CODE" : "DATA"), attributes_of(ob.flags));
    return out;
}

// ---- which instructions the assembler gives back as they were ----------------------------------------------------------

std::set<Place> Writer::probe(assembly::Prober& prober, const std::filesystem::path& outdir) {
    std::map<Place, assembly::Candidate> candidates;
    std::set<Place> good;
    for (const auto& [i, ob] : objects_) {
        for (const auto& [off, ins] : ob.ins) {
            const std::optional<std::string> text = mnemonic(ob, off);
            if (!text) continue;
            if (ins.target) {
                good.insert(Place{i, off});
                continue;
            }
            assembly::Candidate c;
            c.text = *text;
            c.mask.assign(ins.bytes.size(), 0);
            for (Off x = off; x < off + static_cast<Off>(ins.bytes.size()); ++x) {
                if (!ob.fix.contains(x)) continue;
                for (std::size_t k = 0; k < 4 && static_cast<std::size_t>(x - off) + k < c.mask.size(); ++k) c.mask[static_cast<std::size_t>(x - off) + k] = 1;
            }
            for (std::size_t k = 0; k < ins.bytes.size(); ++k) {
                if (c.mask[k] == 0) c.original.push_back(ins.bytes[k]);
            }
            candidates.emplace(Place{i, off}, std::move(c));
        }
    }
    std::set<std::string> near_targets;   // the names used as call or jump targets
    for (const auto& [i, ob] : objects_) {
        for (const auto& [a, f] : ob.fix) {
            if (f.kind == 8) near_targets.insert(label(f.object, f.offset));
        }
    }
    std::set<std::string> names;
    for (const auto& [i, ob] : objects_) {
        for (const Off a : std::vector<Off>(ob.labels.begin(), ob.labels.end())) {
            const std::string n = label(i, a);
            if (n.find('+') == std::string::npos) names.insert(n);
        }
    }
    std::vector<assembly::Candidate> asked;
    for (const auto& [at, c] : candidates) asked.push_back(c);
    const std::set<assembly::Candidate> ok = prober.probe(asked, names, near_targets, outdir, assembly::Mode::Flat32);
    for (const auto& [at, c] : candidates) {
        if (ok.contains(c)) good.insert(at);
    }
    return good;
}

// Instructions written as mnemonics that did not come back as the original's bytes.
Writer::Differences Writer::differences(const le::Module& built, const std::set<Place>& good) const {
    Differences out;
    std::map<std::uint32_t, const le::Object*> have;
    for (const le::Object& o : built.objects()) have[o.index] = &o;
    for (const auto& [i, ob] : objects_) {
        const auto b = have.find(i);
        if (b == have.end()) {
            out.differing.push_back(i);
            continue;
        }
        const Bytes data = built.object_data(*b->second);
        std::set<Off> patched;
        for (const auto& [a, f] : ob.fix) {
            for (Off k = 0; k < 4; ++k) patched.insert(a + k);
        }
        const std::size_t n = std::min<std::size_t>(data.size(), ob.size);
        bool equal = b->second->vsize == ob.size;
        for (std::size_t a = 0; a < n && equal; ++a) equal = patched.contains(static_cast<Off>(a)) || data[a] == ob.data[a];
        if (equal) continue;
        out.differing.push_back(i);
        std::vector<assembly::Piece<Place>> pieces;
        const auto data_piece = [&](Off lo, Off hi) {
            if (hi <= lo) return;
            assembly::Piece<Place> p;
            p.offset = static_cast<std::size_t>(lo);
            p.length = static_cast<std::size_t>(hi - lo);
            Bytes mask;
            for (Off x = lo; x < hi; ++x) mask.push_back(patched.contains(x) ? 1 : 0);
            p.mask = std::move(mask);
            pieces.push_back(std::move(p));
        };
        Off a = 0, start = 0;
        while (a < static_cast<Off>(ob.size)) {
            if (ob.ins.contains(a) && good.contains(Place{i, a})) {
                data_piece(start, a);
                const Instruction& ins = ob.ins.at(a);
                const std::size_t length = ins.bytes.size();
                Bytes mask;
                for (Off x = a; x < a + static_cast<Off>(length); ++x) mask.push_back(patched.contains(x) ? 1 : 0);
                const bool jump = ins.target.has_value();
                if (jump) {   // the distance: right once the rest is
                    const std::size_t width = length == 2 ? 1 : 4;
                    for (std::size_t k = length > width ? length - width : 0; k < length; ++k) mask[k] = 1;
                }
                assembly::Piece<Place> p;
                p.offset = static_cast<std::size_t>(a);
                p.length = length;
                p.mask = std::move(mask);
                p.key = Place{i, a};
                p.inner_jump = jump;
                pieces.push_back(std::move(p));
                a += static_cast<Off>(length);
                start = a;
            } else {
                a += ob.ins.contains(a) ? static_cast<Off>(ob.ins.at(a).bytes.size()) : 1;
            }
        }
        data_piece(start, static_cast<Off>(ob.size));
        const std::set<Place> blamed = assembly::culprits<Place>(ob.data, data, pieces);
        out.bad.insert(blamed.begin(), blamed.end());
    }
    return out;
}

// ---- the whole job -----------------------------------------------------------------------------------------------

namespace {

// Set the built file's LE module-flags field to the original's. The field (header offset 0x10) tells the loader the kind of module; the Win3.x
// VxDs in WIN386.EXE carry a value (0x8020) OpenWatcom's linker does not emit, and it is metadata, not anything the code produces, so a
// faithful rebuild carries the original's.
void match_module_flags(const std::filesystem::path& image, std::uint32_t want) {
    Bytes data = fs::read_file(image);
    if (data.size() < 0x40) return;
    const std::uint32_t lfa = static_cast<std::uint32_t>(data[0x3C]) | (static_cast<std::uint32_t>(data[0x3D]) << 8) |
                              (static_cast<std::uint32_t>(data[0x3E]) << 16) | (static_cast<std::uint32_t>(data[0x3F]) << 24);
    if (static_cast<std::uint64_t>(lfa) + 0x14 > data.size()) return;
    const bool le_header = data[lfa] == 'L' && (data[lfa + 1] == 'E' || data[lfa + 1] == 'X');
    if (!le_header) return;
    std::uint32_t have = 0;
    for (std::size_t k = 0; k < 4; ++k) have |= static_cast<std::uint32_t>(data[lfa + 0x10 + k]) << (8 * k);
    if (have == want) return;
    for (std::size_t k = 0; k < 4; ++k) data[lfa + 0x10 + k] = static_cast<std::uint8_t>(want >> (8 * k));
    fs::write_file(image, data);
}

// Whether the LE has a non-resident name table inside its file.
bool has_nonresident_names(const le::Module& lf) {
    const le::Header& h = lf.header();
    return h.nonresident_names != 0 && h.nonresident_size != 0 && static_cast<std::uint64_t>(h.nonresident_names) + h.nonresident_size <= lf.data().size();
}

}  // namespace

Result reconstruct(const std::filesystem::path& module_path, const std::filesystem::path& outdir_, const Options& options, std::size_t member) {
    std::vector<std::pair<std::string, le::Module>> members = le::open_modules(module_path);
    if (members.empty()) throw FormatError("driver writer", 0, fs::to_text(module_path) + ": holds no driver");
    if (member >= members.size()) throw UsageError(std::format("{} holds {} drivers, not {}", fs::to_text(module_path.filename()), members.size(), member + 1));
    const auto& [member_name, lf] = members[member];
    platforms::DecoderSet decoders;
    Writer w(lf, decoders.get(platforms::Arch::X86, 32, true), options.service_label);
    const std::filesystem::path outdir = std::filesystem::absolute(outdir_);
    std::string device = lf.descriptor() ? std::string(text::trim(lf.descriptor()->name)) : std::string(text::trim(member_name));
    if (device.empty()) device = "DRIVER";
    const std::string stem = text::sanitize_identifier(text::lower(device));
    fs::make_dirs(outdir);
    const std::filesystem::path src = outdir / (stem + ".asm");
    const std::filesystem::path out = outdir / (stem + ".vxd");
    assembly::Tools tools;
    tools.wasm = options.tools.wasm;
    tools.wlink = options.tools.wlink;
    tools.env = options.tools.env;
    assembly::Prober prober(tools, options.probes.empty() ? outdir / "assembly_probes.json" : options.probes);
    std::set<Place> good = w.probe(prober, outdir);
    Result result;
    result.source = src;

    // the driver's .def -- its description and exports, which the linker puts in the non-resident name table -- when the original has one; kept
    // as written once it is there
    std::string definitions;
    const std::filesystem::path def_path = outdir / (stem + ".def");
    if (std::filesystem::is_regular_file(def_path)) {
        definitions = text::replace_all(text::replace_all(text::from_latin1(fs::read_file(def_path)), "\r\n", "\n"), "\r", "\n");
    } else if (has_nonresident_names(lf)) {
        definitions = build::vxdfile::definitions(lf.data(), static_cast<std::uint32_t>(lf.lfa()));
        write_assembler_text(def_path, std::format("; {} -- the definitions it was linked with, from its non-resident name table\nVXD {}\n", device, device) + definitions);
    }
    if (!definitions.empty()) result.definitions = def_path;

    static const Regex error_line(R"(\((\d+)\)\s*:\s*Error)");
    const std::size_t max_rounds = options.max_rounds != 0 ? options.max_rounds : kMaxRoundsDefault;
    for (std::size_t rounds = 1; rounds <= max_rounds; ++rounds) {
        const Writer::Text written = w.source(&good);
        write_assembler_text(src, written.text);
        result.rounds = rounds;
        result.instructions = written.counts.instructions;
        result.raw = written.counts.raw;
        result.data = written.counts.data;

        const assembly::Made object = assembly::assemble(tools, src, outdir);
        if (!object.file) {
            std::set<Place> failed;
            error_line.for_each_match(object.log, [&](const RegexMatch& m) {
                const std::size_t n = text::parse_int<std::size_t>(m[1], 10).value_or(0);
                if (const auto hit = written.line_of.find(n); hit != written.line_of.end()) failed.insert(hit->second);
                return true;
            });
            // an uncommon opcode the assembler rejects (sidt, lgdt, ...) is rejected everywhere it appears, so drop every instance of it at once
            // rather than one per round
            std::set<Place> drop = failed;
            std::set<std::string> uncommon;
            for (const Place& p : failed) {
                const std::string& m = w.objects().at(p.object).ins.at(p.offset).mnemonic;
                if (!is_common_opcode(m)) uncommon.insert(m);
            }
            if (!uncommon.empty()) {
                for (const auto& [i, ob] : w.objects()) {
                    for (const auto& [a, ins] : ob.ins) {
                        if (good.contains(Place{i, a}) && uncommon.contains(ins.mnemonic)) drop.insert(Place{i, a});
                    }
                }
            }
            if (drop.empty()) {
                result.notes = {"assembler: " + last_message_line(object.log, 200)};
                break;
            }
            for (const Place& p : drop) good.erase(p);
            continue;
        }
        const std::vector<le::Export> exports = lf.exports();
        build::driver::LinkOptions link;
        link.dynamic = lf.header().module_flags == le::kModuleDynamic;
        link.classes = w.classes();
        link.named = !exports.empty() && exports.front().name && !exports.front().name->empty();
        link.definitions = definitions;
        const build::driver::ToolResult linked = build::driver::link(options.tools, *object.file, device, out, link);
        if (!linked.file) {
            result.notes = {"linker: " + last_message_line(linked.log, 200)};
            break;
        }
        match_module_flags(*linked.file, lf.header().module_flags);   // a flag the linker will not emit (Win3.x LE)
        const le::Module built = le::Module::open(*linked.file);
        result.image = linked.file;
        result.linked = true;
        const Writer::Differences d = w.differences(built, good);
        if (options.log && !d.differing.empty())
            options.log(std::format("  round {}: objects {} differ; {} instructions go back to bytes", rounds, joined_ids(d.differing), d.bad.size()));
        if (d.differing.empty() || d.bad.empty()) {
            result.built = build::driver::compare(built, lf);
            result.notes = result.built->notes;
            result.differing_objects = d.differing;
            break;
        }
        for (const Place& p : d.bad) good.erase(p);
    }
    // the driver as one L2 tree, as the file was last written: each object's head and each function a MachineFragment
    if (w.assembly()) {
        Arena arena;
        const std::filesystem::path l2 = outdir / "module.l2.json";
        json::save(l2, translate::l2::to_json(*w.assembly()->document(arena)), json::Style::PythonCompatible);
        result.l2 = l2;
    }
    return result;
}

dos_writer::WriteVxd vxd_writer(Options options) {
    return [options = std::move(options)](const std::filesystem::path& program, const std::filesystem::path& outdir, const dos_writer::Log& log) {
        Options o = options;
        if (log) o.log = log;
        const Result r = reconstruct(program, outdir, o, 0);
        dos_writer::VxdPart part;
        part.source = r.source;
        part.image = r.image;
        part.linked = r.linked;
        part.same = r.same();
        part.rounds = r.rounds;
        part.instructions = r.instructions;
        part.raw = r.raw;
        part.data = r.data;
        part.notes = r.notes;
        return part;
    };
}

// ---- the command -------------------------------------------------------------------------------------------------

std::string_view usage() {
    return R"(driver_writer -- a virtual device driver as assembly source that builds back into it.

    driver-source <driver> [--out DIR]

Write a driver's assembly source from its image, build it and check the build against the original. A container of drivers (VMM32.VXD,
WIN386.EXE) has every member written.
)";
}

namespace {

// One line saying how a driver came back, then what is worth knowing of it.
bool report_driver(std::string_view name, const Result& r, const std::function<void(std::string_view)>& say) {
    const std::size_t n = std::max<std::size_t>(1, r.instructions);
    say(std::format("{}: {} instructions, {} ({}%) written as bytes; {} bytes of data; {} round{}", name, r.instructions, r.raw, 100 * r.raw / n, r.data, r.rounds,
                    r.rounds == 1 ? "" : "s"));
    if (r.built) {
        const build::driver::Checks& c = r.built->checks;
        const std::pair<std::string_view, bool> all[] = {{"objects", c.objects}, {"descriptor", c.descriptor}, {"services", c.services}, {"exports", c.exports}, {"module", c.module}};
        std::string line = "  built: ";
        for (std::size_t i = 0; i < std::size(all); ++i) line += std::format("{}{} {}", i ? ", " : "", all[i].first, all[i].second ? "same" : "DIFFERENT");
        say(line);
    }
    for (const std::string& note : r.notes) say("  " + note);
    say("  source: " + fs::to_text(r.source));
    return r.same();
}

}  // namespace

int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say) {
    std::vector<std::string> drivers;
    std::optional<std::string> out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--out") {
            if (i + 1 >= args.size()) throw UsageError("--out needs a folder");
            out = args[++i];
        } else if (!args[i].starts_with("--")) {
            drivers.push_back(args[i]);
        }
    }
    if (drivers.empty()) {
        say(usage());
        return 2;
    }
    std::filesystem::path path = fs::from_text(drivers.front());
    if (!std::filesystem::is_regular_file(path)) {
        const auto found = env.retail ? env.retail(drivers.front()) : std::nullopt;
        if (!found) throw NotFoundError(path, 0, "no driver " + drivers.front());
        path = *found;
    }
    const std::filesystem::path base = out ? fs::from_text(*out) : env.state / "driver_source" / text::lower(fs::to_text(path.stem()));
    const std::vector<std::pair<std::string, le::Module>> members = le::open_modules(path);
    Options options = env.options;
    options.log = [&](const std::string& line) { say(line); };
    bool ok = true;
    for (std::size_t i = 0; i < members.size(); ++i) {
        const std::string& member = members[i].first;
        const std::filesystem::path outdir = members.size() == 1 ? base : base / text::sanitize_identifier(text::lower(member));
        const Result r = reconstruct(path, outdir, options, i);
        ok = report_driver(members.size() > 1 ? member : fs::to_text(path.filename()), r, say) && ok;
    }
    return ok ? 0 : 1;
}

}  // namespace cb::source::driver_writer

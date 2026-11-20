// cb-unity: alone (helpers named for what they do here)
#include "source/dos_image.hpp"

#include <algorithm>
#include <format>

#include "core/regex.hpp"
#include "core/text.hpp"

namespace cb::source::dos_image {

namespace dos = cb::formats::dos;

namespace {

std::uint16_t word_at(ByteView image, std::size_t at) { return static_cast<std::uint16_t>(image[at] | (image[at + 1] << 8)); }

bool is_prefix_byte(std::uint8_t b) {
    return b == 0x26 || b == 0x2E || b == 0x36 || b == 0x3E || b == 0x64 || b == 0x65 || b == 0x66 || b == 0x67 || b == 0xF0 ||
           b == 0xF2 || b == 0xF3;
}

// The width of the displacement a near jump or call carries, 0 for any other instruction: the one-operand relative forms
// (call, jmp, the conditional jumps, loop and jcxz). A 66 prefix makes the call and jmp displacements and the long conditional
// ones 32 bits wide.
std::size_t displacement_width(ByteView code) {
    std::size_t k = 0;
    bool wide = false;
    for (; k < code.size() && is_prefix_byte(code[k]); ++k) {
        if (code[k] == 0x66) wide = true;   // (every prefix is stepped over, whether or not one before it was a 66)
    }
    if (k >= code.size()) return 0;
    const std::uint8_t op = code[k];
    if (op == 0xE8 || op == 0xE9) return wide ? 4 : 2;
    if (op == 0xEB || (op >= 0x70 && op <= 0x7F) || (op >= 0xE0 && op <= 0xE3)) return 1;
    if (op == 0x0F && k + 1 < code.size() && code[k + 1] >= 0x80 && code[k + 1] <= 0x8F) return wide ? 4 : 2;
    return 0;
}

// The displacement as capstone resolved it in 16-bit code: the low 16 bits, signed.
std::int64_t displacement(ByteView instruction, std::size_t width) {
    std::uint32_t raw = 0;
    for (std::size_t i = 0; i < width; ++i) raw |= static_cast<std::uint32_t>(instruction[instruction.size() - width + i]) << (8 * i);
    if (width == 1) return static_cast<std::int8_t>(raw & 0xFF);
    return static_cast<std::int16_t>(raw & 0xFFFF);
}

// `[0x1234]` alone in brackets is an address in the data segment, which the assembler reads as a number unless the segment is said.
std::string say_data_segment(std::string_view operands) {
    static const Regex bare_bracket(R"((?<![a-z]:)\[(0x[0-9a-f]+)\])");
    return bare_bracket.replace_all(operands, "ds:[$1]");
}

std::string left(std::string_view s, std::size_t width) { return std::format("{:<{}}", s, width); }

}  // namespace

DosImage::DosImage(const dos::Program& program, std::string name, const symbols::dos_interrupts::ServiceTable& services,
                   const platforms::Decoder& decoder)
    : program_(program), name_(std::move(name)), image_(program.image) {
    const std::uint32_t load = dos::kLoadSegment;
    origin_ = program.kind == dos::Kind::Com ? 0x100 : 0;   // a .COM file sits behind its program prefix
    for (const std::uint32_t a : program.relocations) relocated_.insert(a - program.base);
    std::set<std::uint32_t> segs;
    for (const std::uint32_t a : relocated_)
        if (a + 2 <= image_.size()) segs.insert((word_at(image_, a) - load) & 0xFFFF);
    // a program with a header of its own (a driver may be linked as one too), or bytes as they load
    flat_ = !(program.kind != dos::Kind::Com && !program.spans.empty() && program.spans[0].label == "program");
    if (!flat_) {
        segs.insert((program.cs - load) & 0xFFFF);
        if (program.sp != 0 && ((program.ss - load) & 0xFFFF) < (image_.size() + 15) / 16) {
            stack_in_ = (program.ss - load) & 0xFFFF;
            segs.insert(*stack_in_);
        }
    }
    top_ = static_cast<std::uint32_t>((image_.size() + 15) / 16);
    std::set<std::uint32_t> kept{0};
    for (const std::uint32_t s : segs)
        if (s < top_) kept.insert(s);
    segments_.assign(kept.begin(), kept.end());

    analysis_ = analyze::dos_functions::discover(program);
    for (const auto& [address, f] : analysis_.functions) names_[address - program.base] = f.name;
    if (!flat_) entry_ = (((program.cs - load) & 0xFFFF) << 4) + program.ip;
    for (const auto& [at, n] : names_) labels_.insert(at);
    if (entry_) labels_.insert(*entry_);
    for (const analyze::dos_functions::Interrupt& i : analysis_.interrupts) {
        const symbols::dos_interrupts::Service s = services.name(i.number, i.ah ? std::optional<std::uint32_t>(*i.ah) : std::nullopt,
                                                                 i.al ? std::optional<std::uint32_t>(*i.al) : std::nullopt);
        calls_[i.address - program.base] = s.name + (!s.arguments.empty() && s.exact ? "(" + s.arguments + ")" : std::string());
    }
    decode_instructions(decoder);
    divide();
}

// ---- decoding the code the analysis found --------------------------------------------------------------------------------

void DosImage::decode_instructions(const platforms::Decoder& decoder) {
    const std::size_t size = image_.size();
    for (std::size_t at = 0; at < size; ++at) {
        if (at >= analysis_.bytes.size() || analysis_.bytes[at] != analyze::dos_functions::ByteClass::Code) continue;
        Bytes window(image_.begin() + static_cast<std::ptrdiff_t>(at), image_.begin() + static_cast<std::ptrdiff_t>(std::min(at + 16, size)));
        std::optional<std::uint32_t> where;
        for (std::size_t x = at; x < std::min(at + 15, size); ++x) {
            if (!relocated_.contains(static_cast<std::uint32_t>(x))) continue;
            const std::size_t i = x - at;
            window[i] = kSegmentMark & 0xFF;
            if (i + 1 < window.size()) window[i + 1] = kSegmentMark >> 8;
            where = static_cast<std::uint32_t>(x);
            break;
        }
        const std::vector<platforms::Insn> decoded = decoder.disasm(window, at & 0xFFFF, 1);
        if (decoded.empty() || at + decoded[0].size > size) continue;
        const platforms::Insn& insn = decoded[0];
        if (where && !(at <= *where && *where + 2 <= at + insn.size)) where.reset();
        bool several = false;   // more than one relocated word: written as bytes
        for (std::size_t x = at; x < at + insn.size; ++x)
            if ((!where || x != *where) && relocated_.contains(static_cast<std::uint32_t>(x))) several = true;
        if (several) continue;

        std::optional<std::uint32_t> target;
        const std::size_t width = displacement_width(ByteView(window.data(), insn.size));
        if (width != 0 && !where && window[0] != 0x9A && window[0] != 0xEA) {
            const std::int64_t t = static_cast<std::int64_t>(at) + insn.size + displacement(ByteView(window.data(), insn.size), width);
            if (t >= 0 && t < static_cast<std::int64_t>(size) && segment_of(static_cast<std::uint32_t>(t)) == segment_of(static_cast<std::uint32_t>(at))) {
                target = static_cast<std::uint32_t>(t);
                labels_.insert(*target);
            }
        }
        ins_[static_cast<std::uint32_t>(at)] =
            Instruction{Bytes(image_.begin() + static_cast<std::ptrdiff_t>(at), image_.begin() + static_cast<std::ptrdiff_t>(at + insn.size)),
                        insn.mnemonic, insn.op_str, target, where};
    }
    // an instruction something jumps into the middle of is not written as one
    std::vector<std::uint32_t> drop;
    for (const auto& [at, i] : ins_)
        for (std::size_t x = at + 1; x < at + i.bytes.size(); ++x)
            if (labels_.contains(static_cast<std::uint32_t>(x))) {
                drop.push_back(at);
                break;
            }
    for (const std::uint32_t at : drop) ins_.erase(at);
    for (bool again = true; again;) {
        again = false;
        std::set<std::uint32_t> inside;
        for (const auto& [at, i] : ins_)
            for (std::size_t x = at + 1; x < at + i.bytes.size(); ++x) inside.insert(static_cast<std::uint32_t>(x));
        for (auto it = ins_.begin(); it != ins_.end();) {
            if (it->second.target && inside.contains(*it->second.target)) {
                it = ins_.erase(it);
                again = true;
            } else {
                ++it;
            }
        }
    }
}

// A stretch of the program no relocated word divides can be longer than a segment may be (COMMAND.COM's code is one such stretch
// of 75K): the original's segments were set up by the program itself, at run time. For the assembler it is divided where it can be --
// at a paragraph no jump or call reaches across and no instruction lies over -- which changes no byte: everything inside refers to
// its own half by label or by number.
void DosImage::divide() {
    const std::int64_t img_end = static_cast<std::int64_t>(image_.size());
    const auto reach_of = [&] {
        std::vector<std::pair<std::int64_t, std::int64_t>> reach;
        for (const auto& [at, i] : ins_)
            if (i.target) reach.emplace_back(std::min<std::int64_t>(at, *i.target), std::max<std::int64_t>(at, *i.target));
        return reach;
    };
    auto reach = reach_of();
    std::set<std::int64_t> inside;
    for (const auto& [at, i] : ins_)
        for (std::size_t x = at + 1; x < at + i.bytes.size(); ++x) inside.insert(static_cast<std::int64_t>(x));
    for (bool again = true; again;) {
        again = false;
        for (std::size_t k = 0; k < segments_.size(); ++k) {
            const std::int64_t lo = static_cast<std::int64_t>(segments_[k]) * 16;
            const std::int64_t hi = k + 1 < segments_.size() ? static_cast<std::int64_t>(segments_[k + 1]) * 16 : img_end;
            if (hi - lo <= kSegmentSpan) continue;
            std::optional<std::int64_t> cut, fewest;
            for (std::int64_t b = (lo + kSegmentSpan) & ~std::int64_t{15}; b > lo + 0x1000; b -= 16) {
                if (inside.contains(b) || relocated_.contains(static_cast<std::uint32_t>(b)) || relocated_.contains(static_cast<std::uint32_t>(b - 1))) continue;
                std::int64_t n = 0;
                for (const auto& [a, z] : reach)
                    if (a < b && b <= z && lo <= a && z < hi) ++n;
                if (!fewest || n < *fewest) {
                    cut = b;
                    fewest = n;
                }
                if (n == 0) break;
            }
            if (!cut) return;   // it cannot be divided: left as it is
            if (*fewest != 0) {
                // every place is jumped across (the DOS kernel's code runs in a segment of its own at run time): the few jumps
                // across the place chosen are written as bytes
                for (auto& [at, i] : ins_) {
                    if (i.target && std::min<std::int64_t>(at, *i.target) < *cut && *cut <= std::max<std::int64_t>(at, *i.target)) {
                        i.target.reset();
                        crossing_.insert(at);
                    }
                }
                reach = reach_of();
            }
            segments_.push_back(static_cast<std::uint32_t>(*cut / 16));
            std::sort(segments_.begin(), segments_.end());
            segments_.erase(std::unique(segments_.begin(), segments_.end()), segments_.end());
            again = true;
            break;
        }
    }
}

// ---- text ------------------------------------------------------------------------------------------------------------------

std::uint32_t DosImage::segment_of(std::uint32_t at) const {
    std::uint32_t best = 0;
    for (const std::uint32_t s : segments_)
        if (s * 16 <= at) best = s;
    return best;
}

std::string DosImage::label(std::uint32_t at) const {
    const auto it = names_.find(at);
    return it != names_.end() && !it->second.empty() ? it->second : std::format("L_{:05X}", at);
}

std::string DosImage::seg_name(std::uint32_t value) const {
    std::uint32_t base = 0;
    for (const std::uint32_t s : segments_)
        if (s <= value) base = std::max(base, s);
    return std::format("seg S{:04X}{}", base, value != base ? std::format(" + {}", value - base) : std::string());
}

std::string DosImage::segment_at(std::uint32_t at) const { return seg_name((word_at(image_, at) - dos::kLoadSegment) & 0xFFFF); }

std::optional<std::string> DosImage::mnemonic(std::uint32_t at) const {
    const Instruction& i = ins_.at(at);
    if (crossing_.contains(at)) return std::nullopt;
    const std::string probe = i.mnemonic + " ";
    if (const auto op = assembly::string_op(probe)) {
        std::string ops = i.operands;
        if (const std::size_t e = ops.find("es:[di]"); e != std::string::npos) ops.erase(e, 7);
        if (!i.where && ops.find(':') == std::string::npos) {
            const std::string s = std::string(op->prefix) + std::string(op->op) + op->width;
            return std::string(text::trim(s));
        }
    }
    if (i.target) {
        if (i.mnemonic == "loop" || i.mnemonic == "loope" || i.mnemonic == "loopne" || i.mnemonic == "jcxz" || i.mnemonic == "jecxz")
            return left(i.mnemonic, 6) + " " + label(*i.target);
        return left(i.mnemonic, 6) + " " + (i.bytes.size() == 2 ? "short" : "near ptr") + " " + label(*i.target);
    }
    std::string o = i.operands;
    if (i.where) {
        const std::string marker = std::format("0x{:x}", kSegmentMark);
        const std::size_t m = o.find(marker);
        if (m == std::string::npos) return std::nullopt;
        o = text::replace_all(o, marker, segment_at(*i.where));
    }
    o = say_data_segment(o);
    return std::string(text::rtrim(left(assembly::mnemonic(i.mnemonic), 6) + " " + assembly::operands(o)));
}

std::vector<std::string> DosImage::raw_lines(std::uint32_t at) const {
    const Instruction& i = ins_.at(at);
    std::vector<std::string> lines;
    std::vector<std::uint8_t> run;
    const auto flush = [&] {
        if (run.empty()) return;
        std::string line = "        db   ";
        for (std::size_t k = 0; k < run.size(); ++k) line += (k != 0 ? ", " : "") + std::format("0{:02X}h", run[k]);
        lines.push_back(std::move(line));
        run.clear();
    };
    std::size_t x = at;
    while (x < at + i.bytes.size()) {
        if (relocated_.contains(static_cast<std::uint32_t>(x))) {
            flush();
            lines.push_back("        dw   " + segment_at(static_cast<std::uint32_t>(x)));
            x += 2;
        } else {
            run.push_back(i.bytes[x - at]);
            ++x;
        }
    }
    flush();
    const std::string text_form = mnemonic(at).value_or(std::string(text::trim(i.mnemonic + " " + i.operands)));
    if (!lines.empty()) lines[0] = left(lines[0], 44) + " ; " + text_form;
    return lines;
}

Source DosImage::source(const std::set<std::uint32_t>* good) const {
    const std::string& name = name_;
    translate::l2::Assembly out({"; " + name + " -- reconstructed from the program image by CodeBreaker.", ";",
                                 "; One segment for each segment the program names. A word holding a segment is written as",
                                 "; that segment, so the program can be loaded anywhere; offsets inside a segment are the",
                                 "; original's numbers. Labels are L_<position in the load module>. Instructions written as",
                                 "; bytes are ones the assembler would encode differently; their mnemonic is in the comment.", "", ".386p", ""},
                                "masm", name);
    Counts counts;
    const std::size_t size = image_.size();
    std::size_t done = 0;   // how far the segments before have got
    for (std::size_t k = 0; k < segments_.size(); ++k) {
        const std::size_t s = segments_[k];
        const std::size_t lo = s * 16;
        const std::size_t hi = k + 1 < segments_.size() ? static_cast<std::size_t>(segments_[k + 1]) * 16 : size;
        // The original's linker packed segments end to end, so the last instruction of one may run a few bytes into the paragraph
        // the next is named by. That next segment then starts where the one before stopped (byte-aligned), and its offsets count
        // from its paragraph all the same.
        const std::size_t over = std::max(done, lo) - lo;
        out.part(std::format("S{:04X}", s), "segment", std::nullopt, hi - lo);
        const char* align = over != 0 ? "byte" : "para";
        if (stack_in_ && s == *stack_in_ && hi == size) out.append(std::format("S{:04X} segment {} stack use16 'STACK'", s, align));
        else out.append(std::format("S{:04X} segment {} public use16 'C{:04X}'", s, align, s));
        out.append(std::format("        assume cs:S{:04X}, ds:nothing, es:nothing, ss:nothing", s));
        if (origin_ != 0 && s == 0) out.append(std::format("        org  {}", origin_));
        // (the segment by name, for the program's C: `FP_SEG(&codebreaker_seg_18e9)` is this segment wherever a build links
        // it -- the C does not write the original's segment numbers, which move when the C's own code is linked in)
        out.append(std::format("        public _codebreaker_seg_{:04x}", s));
        out.append(std::format("_codebreaker_seg_{:04x} label byte", s));
        std::size_t at = std::max(lo + over, s == 0 ? static_cast<std::size_t>(origin_) : std::size_t{0});   // (DOS supplies what is before a .COM file's origin)
        std::vector<std::uint8_t> run;
        const auto flush = [&] {
            const std::vector<std::string> lines = assembly::data_lines(run);
            out.extend(lines);
            run.clear();
        };
        while (at < hi) {
            const std::uint32_t here = static_cast<std::uint32_t>(at);
            if (labels_.contains(here)) {
                flush();
                out.append(label(here) + ":");
            }
            if (const auto i = ins_.find(here); i != ins_.end()) {
                flush();
                std::optional<std::string> written;
                if (good == nullptr || good->contains(here)) written = mnemonic(here);
                ++counts.instructions;
                if (!written) {
                    ++counts.raw;
                    const std::vector<std::string> lines = raw_lines(here);
                    out.extend(lines);
                } else {
                    const auto note = calls_.find(here);
                    out.append("        " + *written + (note != calls_.end() ? "    ; " + note->second : std::string()));
                }
                at += i->second.bytes.size();
            } else if (relocated_.contains(here)) {
                flush();
                out.append("        dw   " + segment_at(here));
                at += 2;
            } else {
                run.push_back(image_[at]);
                ++counts.data;
                ++at;
            }
        }
        flush();
        done = at;
        if (labels_.contains(static_cast<std::uint32_t>(hi)) && hi == size) out.append(label(static_cast<std::uint32_t>(hi)) + ":");
        // (the stack's room behind the file's bytes is not written: a linker puts it in the file as zeros; it is memory the header
        // asks for -- the stack is set there after linking)
        out.append(std::format("S{:04X} ends", s));
        out.append("");
    }
    out.part("end", "end");
    if (!flat_) {
        const std::uint32_t stack = (program_.ss - dos::kLoadSegment) & 0xFFFF;
        if (stack >= top_) {   // the stack lies behind the load module
            if (stack > top_) {
                out.append("GAP segment para public use16 'GAP'");
                out.append(std::format("        db   {} dup (?)", (stack - top_) * 16));
                out.append("GAP ends");
            }
            out.append("STK segment para stack use16 'STACK'");
            out.append(std::format("        db   {} dup (?)", program_.sp));
            out.append("STK ends");
            out.append("");
        }
        // (an entry point outside the load module -- COMMAND.COM's is in its program prefix -- has no label to name; it is set in
        // the file's header after linking)
        out.append(entry_ && *entry_ < size ? "end " + label(*entry_) : "end");
    } else if (program_.kind == dos::Kind::Com && labels_.contains(origin_)) {
        out.append("end " + label(origin_));   // where it starts: the linker leaves out what is before
    } else {
        out.append("end");
    }
    return {text::join(out.lines(), "\n") + "\n", counts};
}

// ---- what the probing and the culprit search are given --------------------------------------------------------------------------

std::vector<assembly::Piece<std::uint32_t>> DosImage::pieces(const std::set<std::uint32_t>& good) const {
    std::vector<assembly::Piece<std::uint32_t>> out;
    std::size_t at = origin_, start = origin_;
    const std::size_t n = image_.size();
    const auto data = [&](std::size_t end) {
        if (end > start) out.push_back({start - origin_, end - start, std::nullopt, std::nullopt, false});
    };
    while (at < n) {
        const std::uint32_t here = static_cast<std::uint32_t>(at);
        if (const auto i = ins_.find(here); i != ins_.end()) {
            const std::size_t size = i->second.bytes.size();
            data(at);
            std::optional<Bytes> mask;
            if (i->second.where) {
                mask = Bytes(size, 0);
                (*mask)[*i->second.where - at] = 1;
                (*mask)[*i->second.where - at + 1] = 1;
            }
            out.push_back({at - origin_, size, mask, good.contains(here) ? std::optional<std::uint32_t>(here) : std::nullopt, i->second.target.has_value()});
            at += size;
            start = at;
        } else if (relocated_.contains(here)) {
            data(at);
            out.push_back({at - origin_, 2, Bytes{1, 1}, std::nullopt, false});
            at += 2;
            start = at;
        } else {
            ++at;
        }
    }
    data(n);
    return out;
}

std::map<std::uint32_t, assembly::Candidate> DosImage::candidates() const {
    std::map<std::uint32_t, assembly::Candidate> out;
    for (const auto& [at, i] : ins_) {
        if (i.target) continue;
        const std::optional<std::string> text_form = mnemonic(at);
        if (!text_form) continue;
        Bytes mask(i.bytes.size(), 0);
        if (i.where) {
            mask[*i.where - at] = 1;
            mask[*i.where - at + 1] = 1;
        }
        Bytes outside;
        for (std::size_t k = 0; k < i.bytes.size(); ++k)
            if (mask[k] == 0) outside.push_back(i.bytes[k]);
        out[at] = assembly::Candidate{*text_form, std::move(outside), std::move(mask)};
    }
    return out;
}

std::vector<std::string> DosImage::link_lines(std::string_view object) const {
    const std::string file = "file " + std::string(object);
    if (program_.kind == dos::Kind::Com) return {"format dos com", "option quiet, nodefaultlibs", file};
    if (flat_) return {"format raw bin", "option quiet, nodefaultlibs", file};
    // the stack's size is said outright: the linker would give a small one 4096 bytes
    const std::string stack = program_.sp != 0 ? std::format(", stack={}", program_.sp) : std::string();
    return {"format dos", "option quiet, nodefaultlibs, packcode=0, packdata=0" + stack, file};
}

}  // namespace cb::source::dos_image

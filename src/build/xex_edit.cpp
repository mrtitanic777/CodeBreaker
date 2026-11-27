#include "build/xex_edit.hpp"
#include "source/dialect.hpp"
#include "source/risc_source.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::build::xex_edit {

namespace {

constexpr std::string_view kXexOperations = "privileges, nop, bytes, asm, hook, asm-only";

// "pal50" and the bits named so far.
struct NamedBit {
    std::string_view name;
    unsigned bit;
};
constexpr NamedBit kNamedBits[] = {{"pal50", kPal50Incompatible}};

std::string xex_edit_quote(std::string_view s) { return "'" + std::string(s) + "'"; }

// `+bit` or `-bit`: the bit's number (or a name) after a sign.
Change parse_change(std::string_view token) {
    const char sign = token.empty() ? ' ' : token[0];
    if (sign != '+' && sign != '-') throw UsageError("a change is +bit or -bit: " + xex_edit_quote(token));
    const std::string name = text::lower(token.substr(1));
    std::optional<unsigned> bit;
    for (const NamedBit& named : kNamedBits)
        if (named.name == name) bit = named.bit;
    if (!bit) {
        const auto number = text::parse_int<long long>(name, 0);
        if (!number) throw UsageError("not a privilege bit: " + xex_edit_quote(token));
        if (*number < 0 || *number >= 64) throw UsageError("privilege bits are 0-63: " + xex_edit_quote(token));
        bit = static_cast<unsigned>(*number);
    }
    if (*bit >= 64) throw UsageError("privilege bits are 0-63: " + xex_edit_quote(token));
    return Change{*bit, sign == '+'};
}

// The number in an address, count or cave argument.
std::uint32_t parse_number(std::string_view token) {
    const auto number = text::parse_int<std::uint32_t>(token, 0);
    if (!number) throw UsageError("not a number: " + xex_edit_quote(token));
    return *number;
}

// Several asm lines are separated by `;` or newlines.
std::string asm_lines(std::string_view text) { return text::replace_all(text, ";", "\n"); }

Bytes parse_hex_bytes(std::string_view token) {
    const std::string digits = text::replace_all(token, " ", "");
    try {
        return crypto::from_hex(digits);
    } catch (const Error&) {
        throw UsageError("not hexadecimal bytes: " + xex_edit_quote(token));
    }
}

// A displaced instruction that branches relative to its own address cannot move to a cave.
bool relative_branch(std::uint32_t word) {
    const std::uint32_t op = word >> 26;
    return (op == 18 || op == 16) && (word & 2) == 0;
}

std::string bits_text(const std::vector<unsigned>& bits, bool name_them) {
    std::string out;
    for (const unsigned bit : bits) {
        if (!out.empty()) out += ", ";
        out += std::to_string(bit);
        if (name_them && bit == kPal50Incompatible) out += " (pal50)";
    }
    return out.empty() ? "none" : out;
}

}  // namespace

Privileges privileges(ByteView xex_file) {
    if (xex_file.size() < 4 || !std::equal(formats::xex::kMagic.begin(), formats::xex::kMagic.end(), xex_file.begin()))
        throw FormatError("xex_edit", 0, "not an XEX2 module");
    Privileges found;
    std::set<unsigned> bits;
    formats::xex::OptionalHeaders headers(xex_file);
    while (const auto header = headers.next()) {
        for (unsigned half = 0; half < 2; ++half) {
            if (header->key != (half == 0 ? kPrivilegeKeyLow : kPrivilegeKeyHigh)) continue;
            (half == 0 ? found.low_at : found.high_at) = header->offset + 4;
            for (unsigned b = 0; b < 32; ++b)
                if ((header->value >> b) & 1) bits.insert(32 * half + b);
        }
    }
    found.bits.assign(bits.begin(), bits.end());
    return found;
}

Bytes set_privileges(ByteView xex_file, std::span<const Change> changes) {
    Bytes out(xex_file.begin(), xex_file.end());
    const Privileges found = privileges(xex_file);
    for (const Change& change : changes) {
        const unsigned half = change.bit / 32;
        const std::optional<std::size_t>& at = half == 0 ? found.low_at : found.high_at;
        if (half > 1) throw UsageError(std::format("privilege bits are 0-63, not {}", change.bit));
        if (!at)
            throw UsageError(std::format("the header has no privilege entry for bit {} (key {:x})", change.bit,
                                         half == 0 ? kPrivilegeKeyLow : kPrivilegeKeyHigh));
        io::ByteReader r(out, "xex_edit");
        r.seek(*at);
        std::uint32_t value = r.u32_be();
        const std::uint32_t mask = std::uint32_t{1} << (change.bit % 32);
        value = change.on ? (value | mask) : (value & ~mask);
        for (unsigned k = 0; k < 4; ++k) out[*at + k] = static_cast<std::uint8_t>(value >> (24 - 8 * k));
    }
    return out;
}

Image::Image(Bytes file, std::uint32_t base) : file_(std::move(file)), base_(base) {
    io::ByteReader r(file_, "xex_edit");
    r.seek(8);
    pe_offset_ = r.u32_be();
}

Image Image::open(const formats::xex::Tools& tools, const std::filesystem::path& xex) {
    const formats::xex::Header header = formats::xex::read_header(xex);
    if (!header.base) throw FormatError("xex_edit", 0, "the module states no load address");
    const fs::ScopedTempDir dir{"cb-xex-edit"};
    const std::filesystem::path basic = dir.path() / "basic.xex";
    formats::xex::write_basic(tools, xex, basic);
    return Image(fs::read_file(basic), *header.base);
}

std::size_t Image::offset(std::uint32_t address, std::size_t size) const {
    const std::uint64_t at = pe_offset_ + std::uint64_t{address} - base_;
    if (address < base_ || at + size > file_.size())
        throw UsageError(std::format("{:08X}+{:X} is outside the image ({:08X}, {:X} bytes)", address, size, base_,
                                     file_.size() >= pe_offset_ ? file_.size() - pe_offset_ : 0));
    return static_cast<std::size_t>(at);
}

Bytes Image::read(std::uint32_t address, std::size_t size) const {
    const std::size_t at = offset(address, size);
    return Bytes(file_.begin() + static_cast<std::ptrdiff_t>(at), file_.begin() + static_cast<std::ptrdiff_t>(at + size));
}

void Image::write(std::uint32_t address, ByteView bytes) {
    const std::size_t at = offset(address, bytes.size());
    std::copy(bytes.begin(), bytes.end(), file_.begin() + static_cast<std::ptrdiff_t>(at));
}

void Image::save(const std::filesystem::path& out) const { fs::write_file(out, file_); }

namespace {

Bytes word_bytes(std::uint32_t word) {
    return Bytes{static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8),
                 static_cast<std::uint8_t>(word)};
}

// PowerPC source at `base` through the one assembler (the PowerPC description under GNU as's dialect and the directive layer).
Bytes assemble_ppc(std::string_view text, std::uint32_t base) {
    return source::risc_source::assemble("ppc", source::dialect::find("gas-ppc").parse(std::format(".org {:#x}\n", base) + std::string(text)), {});
}

std::uint32_t branch_word(std::uint32_t target, std::uint32_t from) {
    const Bytes word = assemble_ppc(std::format("b {:#x}\n", target), from);
    if (word.size() != 4) throw UsageError("the assembler produced no branch");
    return (std::uint32_t{word[0]} << 24) | (std::uint32_t{word[1]} << 16) | (std::uint32_t{word[2]} << 8) | word[3];
}

}  // namespace

void hook(Image& image, std::uint32_t address, std::uint32_t cave, std::string_view asm_text) {
    const Bytes stolen = image.read(address, 4);
    const std::uint32_t word = (std::uint32_t{stolen[0]} << 24) | (std::uint32_t{stolen[1]} << 16) | (std::uint32_t{stolen[2]} << 8) | stolen[3];
    if (relative_branch(word))
        throw UsageError(std::format("the instruction at {:08X} is a relative branch ({:08X}): it cannot be moved to the cave", address, word));
    Bytes body = assemble_ppc(asm_text, cave);
    const std::uint32_t back = branch_word(address + 4, cave + static_cast<std::uint32_t>(body.size()) + 4);
    append(body, stolen);
    append(body, word_bytes(back));
    image.write(cave, body);
    image.write(address, word_bytes(branch_word(cave, address)));
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "xex-edit";
    c.summary = "change an Xbox 360 executable: its privilege flags, or its code at an address";
    c.area = "Xbox 360";
    c.description =
        "  xex-edit privileges <in.xex>                         the privilege bits that are set\n"
        "  xex-edit privileges <in.xex> <+bit|-bit ...> <out>   set (+) or clear (-) bits; `pal50` names bit 10\n"
        "  xex-edit nop   <in.xex> <address> [count] <out>      count instructions become nop\n"
        "  xex-edit bytes <in.xex> <address> <hex> <out>        raw bytes over the code\n"
        "  xex-edit asm   <in.xex> <address> \"<asm>\" <out>      PowerPC assembled there\n"
        "  xex-edit hook  <in.xex> <address> <cave> \"<asm>\" <out>\n"
        "                                                       `b cave` at address; at the cave the asm, the displaced\n"
        "                                                       instruction, and `b address+4`\n"
        "  xex-edit asm-only \"<asm>\" [address]                  the bytes, nothing written";
    c.params = {cli::Positional{"op", "privileges, nop, bytes, asm, hook or asm-only"},
                cli::Positional{"args", "the operation's arguments (see below)", false, true}};
    c.passthrough = true;
    c.handler = [environment](const cli::Args& a) -> int {
        const std::string op = a.required("op");
        const std::vector<std::string> args = a.values("args");
        // an operation that needs more arguments than it was given
        const auto need = [&](std::size_t count) {
            if (args.size() < count) throw UsageError(std::format("{} needs {} argument(s)", op, count));
        };
        try {
            if (op == "asm-only") {
                need(1);
                const std::uint32_t address = args.size() > 1 ? parse_number(args[1]) : 0;
                out("{}\n", crypto::to_hex(assemble_ppc(asm_lines(args[0]), address)));
                return kExitOk;
            }
            if (op == "privileges") {
                need(1);
                const Bytes file = fs::read_file(fs::from_text(args[0]));
                if (args.size() == 1) {
                    out("privileges set: {}\n", bits_text(privileges(file).bits, true));
                    return kExitOk;
                }
                std::vector<Change> changes;
                for (std::size_t i = 1; i + 1 < args.size(); ++i) changes.push_back(parse_change(args[i]));
                const Bytes edited = set_privileges(file, changes);
                fs::write_file(fs::from_text(args.back()), edited);
                out("privileges now: {} -> {}\n", bits_text(privileges(edited).bits, false), args.back());
                return kExitOk;
            }
            if (op != "nop" && op != "bytes" && op != "asm" && op != "hook") {
                out("unknown operation {} ({})\n", xex_edit_quote(op), kXexOperations);
                return kExitUsage;
            }
            need(op == "nop" ? 3 : op == "hook" ? 5 : 4);
            Image image = Image::open(environment.tools, fs::from_text(args[0]));
            const std::uint32_t address = parse_number(args[1]);
            const std::string& target = args.back();
            if (op == "nop") {
                const std::uint32_t count = args.size() > 3 ? parse_number(args[2]) : 1;
                Bytes nops;
                for (std::uint32_t i = 0; i < count; ++i) append(nops, word_bytes(kNop));
                image.write(address, nops);
            } else if (op == "bytes") {
                image.write(address, parse_hex_bytes(args[2]));
            } else if (op == "asm") {
                image.write(address, assemble_ppc(asm_lines(args[2]), address));
            } else {
                hook(image, address, parse_number(args[2]), asm_lines(args[3]));
            }
            image.save(fs::from_text(target));
            out("{} at {:08X} -> {}\n", op, address, target);
            return kExitOk;
        } catch (const AsmError& problem) {
            out("xex-edit: {}\n", problem.what());
            return kExitFailed;
        } catch (const UsageError& problem) {
            out("xex-edit: {}\n", problem.what());
            return kExitFailed;
        } catch (const FormatError& problem) {
            out("xex-edit: {}\n", problem.what());
            return kExitFailed;
        }
    };
    return c;
}

}  // namespace cb::build::xex_edit

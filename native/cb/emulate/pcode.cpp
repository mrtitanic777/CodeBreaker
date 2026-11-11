// cb/emulate/pcode.cpp -- see pcode.hpp.
#include "cb/emulate/pcode.hpp"

#include <array>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::emulate::pcode {

std::string_view mode_name(Mode m) {
    switch (m) {
        case Mode::X86Real16: return "x86-16-real";
        case Mode::X86Flat32: return "x86-32-flat";
        case Mode::PpcBe64: return "ppc-be-64";
    }
    return "x86-32-flat";
}

std::string_view sla_file(Mode m) {
    return m == Mode::PpcBe64 ? "ppc_64_xenon_be.sla" : "x86.sla";
}

// ---- Lifted -----------------------------------------------------------------------------------------------

Lifted::Lifted(std::uint32_t length_, std::vector<Op> ops_) : length(length_), ops(std::move(ops_)) {
    static const std::array<std::string_view, 7> kFollowed = {"LOAD",   "STORE",    "CALLOTHER", "CBRANCH",
                                                               "BRANCHIND", "CALLIND", "RETURN"};
    for (const Op& op : ops) {
        for (std::string_view f : kFollowed) {
            if (op.opcode == f) full = true;
        }
        for (const Input& in : op.inputs) {
            if (const Varnode* v = std::get_if<Varnode>(&in)) {
                if (v->space == Space::Register) {
                    for (std::uint64_t k = v->offset; k < v->offset + v->size; ++k) reads.insert(k);
                } else if (v->space == Space::Ram) {
                    full = true;
                }
            }
        }
        if (op.out) {
            if (op.out->space == Space::Register) {
                for (std::uint64_t k = op.out->offset; k < op.out->offset + op.out->size; ++k) writes.insert(k);
            } else if (op.out->space == Space::Ram) {
                full = true;
            }
        }
    }
}

namespace {

[[noreturn]] void bad_line(std::string_view line, std::string_view why) {
    throw ToolError("pcodecheck", 0, line.substr(0, 80), std::string(why));
}

// "-" none, "@name" a user operation's name, else space:offset:size (offset in hex, size in decimal).
Input parse_input(std::string_view text, bool window, std::string_view line) {
    if (text == "-") return std::monostate{};
    if (!text.empty() && text[0] == '@') return std::string(text.substr(1));
    const auto parts = text::split(text, ':');
    if (parts.size() != 3 || parts[0].size() != 1) bad_line(line, "a varnode that is not space:offset:size");
    Varnode v;
    switch (parts[0][0]) {
        case 'r': v.space = Space::Register; break;
        case 'u': v.space = Space::Unique; break;
        case 'c': v.space = Space::Constant; break;
        case 'm': v.space = Space::Ram; break;
        default: bad_line(line, "an unknown space");
    }
    const auto offset = text::parse_int<std::uint64_t>(parts[1], 16);
    const auto size = text::parse_int<std::uint32_t>(parts[2], 10);
    if (!offset || !size) bad_line(line, "a varnode with a bad number");
    v.offset = (v.space == Space::Ram && window) ? (*offset & 0xFFFF) : *offset;   // (a 16-bit window: the offset only)
    v.size = *size;
    return v;
}

// One answer line of pcodecheck: "<length> <op>;<op>;..." with each op "OPCODE out in in ...".
std::optional<Lifted> parse_lifted(std::string_view line, bool window) {
    if (line.empty() || line.starts_with("err")) return std::nullopt;
    const std::size_t sp = line.find(' ');
    const auto length = text::parse_int<std::uint32_t>(sp == std::string_view::npos ? line : line.substr(0, sp), 10);
    if (!length) bad_line(line, "a length that is not a number");
    std::vector<Op> ops;
    if (sp != std::string_view::npos && sp + 1 < line.size()) {
        for (std::string_view text : text::split(line.substr(sp + 1), ';')) {
            const auto parts = text::split(text, ' ');
            if (parts.size() < 2) bad_line(line, "an op with no output field");
            Op op;
            op.opcode = std::string(parts[0]);
            Input out = parse_input(parts[1], window, line);
            if (const Varnode* v = std::get_if<Varnode>(&out)) op.out = *v;   // (a name is not a varnode: no output)
            for (std::size_t k = 2; k < parts.size(); ++k) op.inputs.push_back(parse_input(parts[k], window, line));
            ops.push_back(std::move(op));
        }
    }
    return Lifted(*length, std::move(ops));
}

std::string hex_of(ByteView code) {
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string out;
    out.reserve(code.size() * 2);
    for (std::uint8_t b : code) {
        out.push_back(kDigits[b >> 4]);
        out.push_back(kDigits[b & 15]);
    }
    return out;
}

}  // namespace

// ---- Lifter -----------------------------------------------------------------------------------------------

Lifter::Lifter(const std::filesystem::path& exe, const std::filesystem::path& sla, Mode mode) : mode_(mode) {
    process::ProcessSpec spec;
    spec.argv = {exe.string(), sla.string(), std::string(mode_name(mode)) + "-lift"};
    spec.err = process::Capture::Discard;
    child_ = std::make_unique<process::Process>(process::start(spec));
    const std::string head = read_line();
    const auto fields = text::split_ws(head);
    if (fields.empty() || fields[0] != "regs") {
        child_->kill();
        throw ToolError("pcodecheck", 0, head, "not a lifting pcodecheck (no register table)");
    }
    for (std::size_t i = 1; i < fields.size(); ++i) {
        const auto parts = text::split(fields[i], ':');
        const auto offset = parts.size() == 3 ? text::parse_int<std::uint64_t>(parts[1], 16) : std::nullopt;
        const auto size = parts.size() == 3 ? text::parse_int<std::uint32_t>(parts[2], 10) : std::nullopt;
        if (!offset || !size) {
            child_->kill();
            throw ToolError("pcodecheck", 0, head, std::format("a register entry that is not name:offset:size: '{}'", fields[i]));
        }
        regs_[std::string(parts[0])] = RegisterSlot{*offset, *size};
    }
}

std::unique_ptr<Lifter> Lifter::try_start(const std::filesystem::path& exe, const std::filesystem::path& sla,
                                          Mode mode) {
    try {
        return std::make_unique<Lifter>(exe, sla, mode);
    } catch (const ToolError&) {
        return nullptr;   // no program that lifts: the answer the caller wanted
    }
}

// The next line the child wrote, without its newline ("\r\n" or "\n"); empty at the end of its output.
std::string Lifter::read_line() {
    while (true) {
        const std::size_t nl = pending_.find('\n');
        if (nl != std::string::npos) {
            std::string line = pending_.substr(0, nl);
            pending_.erase(0, nl + 1);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            return line;
        }
        std::uint8_t buf[4096];
        const std::size_t got = child_->read(process::Process::Stream::Out, buf);
        if (got == 0) {
            std::string rest = std::move(pending_);
            pending_.clear();
            if (!rest.empty() && rest.back() == '\r') rest.pop_back();
            return rest;
        }
        pending_.append(reinterpret_cast<const char*>(buf), got);
    }
}

std::string Lifter::line_locked(ByteView code, std::uint64_t offset) {
    const std::string request = std::format("{} {:x}\n", hex_of(code), offset);
    child_->write(as_bytes(request));
    return read_line();
}

std::string Lifter::line(ByteView code, std::uint64_t offset) {
    std::lock_guard<std::mutex> hold(lock_);
    return line_locked(code, offset);
}

const Lifted* Lifter::lift(ByteView code, std::uint64_t offset) {
    const std::string key = std::format("{} {:x}", hex_of(code), offset);
    std::lock_guard<std::mutex> hold(lock_);
    auto it = memo_.find(key);
    if (it == memo_.end()) {
        const std::string line = line_locked(code, offset);
        it = memo_.emplace(key, parse_lifted(line, mode_ == Mode::X86Real16)).first;
    }
    return it->second ? &*it->second : nullptr;
}

// ---- configuration ------------------------------------------------------------------------------------------

PcodeConfig PcodeConfig::from_environment(const std::filesystem::path& decompiler_bin,
                                          const std::filesystem::path& languages) {
    PcodeConfig c;
    const std::optional<std::string> env = fs::get_env("CB_PCODECHECK");
    c.exe = (env && !env->empty()) ? std::filesystem::path(*env) : decompiler_bin / "pcodecheck.exe";
    c.languages = languages;
    return c;
}

Lifter* LifterSet::get(Mode mode) {
    std::lock_guard<std::mutex> hold(lock_);
    for (auto& [m, lifter] : started_) {
        if (m == mode) return lifter.get();
    }
    const std::filesystem::path sla = config_.sla(mode);
    std::unique_ptr<Lifter> made;
    if (std::filesystem::exists(config_.exe) && std::filesystem::exists(sla)) {
        made = Lifter::try_start(config_.exe, sla, mode);
    }
    started_.emplace_back(mode, std::move(made));
    return started_.back().second.get();
}

}  // namespace cb::emulate::pcode

// The old harness of the Python module (state/work/typed-port/decompile.m68k: 130,000 calls, 30 MB of recorded results),
// replayed against the C++ when CB_M68K_CORPUS names its folder (the file expected.txt in it). Not part of the normal run:
// the corpus is generated state, not source. The intended differences are the DIVERGES list of m68k_test.cpp.
#include <algorithm>
#include <format>
#include <map>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/m68k.hpp"

namespace m = cb::decompile::m68k;
using std::string;

namespace {

string hx(std::string_view s) {
    string out;
    for (unsigned char c : s) out += std::format("{:02x}", c);
    return out;
}

string unhex(std::string_view h) {
    string out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) out += static_cast<char>(std::stoi(string(h.substr(i, 2)), nullptr, 16));
    return out;
}

cb::Bytes bytes_of(std::string_view h) {
    cb::Bytes b;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) b.push_back(static_cast<std::uint8_t>(std::stoi(string(h.substr(i, 2)), nullptr, 16)));
    return b;
}

// The old harness's encoding of an operand, with what Python kept in `xsize` for kinds that do not use it left out.
string enc_op(const m::Op& o) {
    using K = m::OpKind;
    string xr = "N", xs = "-", sz = "N";
    if (o.kind == K::Index || o.kind == K::PcIndex) {
        xr = std::format("{}{}", o.index.address ? 'A' : 'D', static_cast<int>(o.index.number));
        xs = hx(m::size_name(o.index.size));
    }
    if (o.kind == K::Regs) xs = o.predecrement ? "pre" : "-";
    if (o.kind == K::Imm) sz = "s" + hx(m::size_name(o.size));
    if (o.kind == K::Label8) sz = std::format("i{}", o.target);
    return std::format("{}/{}/{}/{}/{}/{}", hx(m::kind_name(o.kind)), static_cast<int>(o.reg), o.value, xr, xs, sz);
}

string enc_insn(const m::Insn& i) {
    string ops;
    for (const m::Op& o : i.operands()) ops += (ops.empty() ? "" : ";") + enc_op(o);
    const auto flow = cb::decompile::z80::flow_name(i.flow);
    string raw;
    for (std::uint8_t c : i.bytes()) raw += std::format("{:02x}", c);
    return std::format("I|{}|{}|{}|{}|{}|{}|{}|{}|{}", hx(i.mnemonic), hx(m::size_name(i.size)), static_cast<int>(i.length),
                       i.target ? std::to_string(*i.target) : "N", hx(flow), i.pc, raw, i.word, ops.empty() ? "-" : ops);
}

// The same normalisation of the recorded operand text.
string norm_expected(const string& e) {
    if (e == "N") return e;
    auto parts = cb::text::split(e, '|');
    string out;
    for (std::size_t k = 0; k < parts.size(); ++k) {
        if (k + 1 < parts.size()) {
            out += string(parts[k]) + "|";
            continue;
        }
        string ops;
        if (parts[k] == "-") {
            out += "-";
            break;
        }
        for (std::string_view op : cb::text::split(parts[k], ';')) {
            auto f = cb::text::split(op, '/');
            const string kind = unhex(f[0]);
            string xs = "-";
            if (kind == "index" || kind == "pcindex") xs = string(f[4]);
            if (kind == "regs") xs = f[4] == hx("pre") ? "pre" : "-";
            ops += (ops.empty() ? "" : ";") + string(f[0]) + "/" + string(f[1]) + "/" + string(f[2]) + "/" + string(f[3]) + "/" + xs + "/" + string(f[5]);
        }
        out += ops;
    }
    return out;
}

std::vector<std::int64_t> ints_of(const m::Insn& i) {
    std::vector<std::int64_t> v;
    for (const m::Op& o : i.operands()) {
        v.push_back(o.value);
        if (o.kind == m::OpKind::Label8) v.push_back(o.target);
    }
    if (i.target) v.push_back(*i.target);
    return v;
}

cb::decompile::AddressLabels text_labels(const string& mode, const std::vector<std::int64_t>& ints) {
    cb::decompile::AddressLabels l;
    if (mode == "G") {
        for (std::int64_t v : ints) {
            if (v >= 0 && v <= 0xFFFFFFFFll) l[static_cast<std::uint32_t>(v)] = std::format("lab_{:x}", v);
        }
    }
    return l;
}

cb::decompile::NameLabels asm_labels(const string& mode, const std::vector<std::int64_t>& ints) {
    cb::decompile::NameLabels l;
    if (mode == "L") {
        l = {{"loop", 0x1002}, {"sub", 0x2000}, {"VDP_CTRL", 0xC00004}, {"far", 0x1000000}, {"neg", -4}, {"zero", 0}, {"near", 0x1010},
             {"ram", 0xFF8000}, {"b", 0x7F}, {"WORD", 0x1234}, {"high", 0xFFFF8000ll}, {".local", 7}, {"@v", 9}, {"X\xc3\xa9", 11}};
    } else if (mode == "G") {
        for (std::int64_t v : ints) {
            if (v >= 0) l[std::format("lab_{:x}", v)] = v;
        }
    }
    return l;
}

// "B:hex" or "E:<kind>"
string asm_result(const string& line, std::uint32_t pc, const cb::decompile::NameLabels& labels) {
    try {
        return "B:" + [&] {
            string h;
            for (std::uint8_t c : m::assemble(line, pc, labels)) h += std::format("{:02x}", c);
            return h;
        }();
    } catch (const cb::decompile::z80::UnknownLabel&) {
        return "E:KeyError";
    } catch (const cb::Error&) {
        return "E:ValueError";
    }
}

}  // namespace

CB_TEST("decompile.m68k.python_corpus", [] {
    const auto dir = cb::fs::get_env("CB_M68K_CORPUS");
    if (!dir) return;
    const string all = cb::fs::read_text(std::filesystem::path(*dir) / "expected.txt");
    std::map<string, std::size_t> checked, differ;
    std::vector<string> shown;
    const auto report = [&](const string& kind, const string& line, const string& got) {
        ++differ[kind];
        if (shown.size() < 25) shown.push_back(std::format("{}: {}\n      mine: {}", kind, line.substr(0, 220), got.substr(0, 220)));
    };
    for (std::string_view raw : cb::text::lines(all)) {
        const string line(raw);
        const std::size_t arrow = line.find(" -> ");
        if (arrow == string::npos) continue;
        const auto head = cb::text::split(std::string_view(line).substr(0, arrow), ' ');
        const string want = line.substr(arrow + 4);
        const string cmd(head[0]);
        if (cmd == "decode") {
            const std::int64_t at = std::stoll(string(head[2]));
            if (at < 0) continue;
            ++checked[cmd];
            const auto insn = m::decode(bytes_of(head[1]), static_cast<std::size_t>(at), static_cast<std::uint32_t>(std::stoll(string(head[3]))));
            const string got = insn ? enc_insn(*insn) : "N";
            if (got != norm_expected(want)) report(cmd, line, got);
        } else if (cmd == "rt") {
            ++checked[cmd];
            const std::uint32_t pc = static_cast<std::uint32_t>(std::stoll(string(head[2])));
            const auto insn = m::decode(bytes_of(head[1]), 0, pc);
            string got = "N";
            if (insn) {
                const auto ints = ints_of(*insn);
                const string t = m::text(*insn, text_labels(string(head[3]), ints));
                got = "T:" + hx(t) + " " + asm_result(t, pc, asm_labels(string(head[3]), ints));
            }
            // an error of the Python carries its message for four kinds; the category is what is compared
            string w = want;
            if (w.find(" E:ValueError:") != string::npos) w = w.substr(0, w.find(" E:ValueError:") + 14);
            if (got.find(" E:ValueError") != string::npos) got = got.substr(0, got.find(" E:ValueError") + 14);
            if (w.find("E:ValueError") != string::npos) w = w.substr(0, w.find("E:ValueError") + 12);
            if (got.find("E:ValueError") != string::npos) got = got.substr(0, got.find("E:ValueError") + 12);
            if (got != w) report(cmd, line, got);
        } else if (cmd == "asm") {
            ++checked[cmd];
            const string src = unhex(head[1]);
            const std::uint32_t pc = static_cast<std::uint32_t>(std::stoll(string(head[2])));
            const string got = asm_result(src, pc, asm_labels(string(head[3]), {}));
            string w = want;
            if (w.rfind("E:ValueError", 0) == 0) w = "E:ValueError";
            const bool python_other_error = w.rfind("E:", 0) == 0 && w != "E:ValueError" && w != "E:KeyError";
            const bool equal = got == w || (python_other_error && got.rfind("E:", 0) == 0);
            if (!equal) report(cmd, std::format("{} | {}", src, line), got);
        }
    }
    for (const auto& [k, n] : checked) {
        cb::testing::record_failure(__FILE__, __LINE__, std::format("corpus {}: {} checked, {} differ", k, n, differ[k]));
    }
    for (const string& s : shown) cb::testing::record_failure(__FILE__, __LINE__, s);
});

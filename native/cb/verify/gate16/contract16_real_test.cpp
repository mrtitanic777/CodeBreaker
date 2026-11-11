#include "cb/verify/gate16/contract16.hpp"

#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

// Real code, and the Python's answers on it. testdata/contract16/<module>.calls holds the relocated code segments of a
// Windows 95 16-bit module (as describe16 gives them) and, for each function, what the Python's contract16._calls
// answered with the callee-name and removed-bytes formulas below (state/work/w0b29/dump_calls.py wrote it with Capstone
// 5.0.7, the engine this tree uses). 474 calls over four modules: near and far, with pushes, cleanups by `add sp`, by
// `pop`, by LEAVE, and joins. Each line is `segment <n> <hex image>` or `function <segment> <lo> <hi> <rows>`; a row is
// `callee,taken off,pushed,leave` and rows are joined by ';'.

using namespace cb;
using namespace cb::verify::gate16::contract16;

namespace {

std::uint64_t place_hash(const CalleeKey& key) {
    if (const auto* far = std::get_if<FarCall>(&key))
        return far->address;
    const auto& n = std::get<NearCall>(key);
    return std::uint64_t{n.segment} * 65536 + n.offset;
}

std::optional<std::string> formula_name(const CalleeKey& key) {
    const std::uint64_t h = place_hash(key);
    if (h % 5 == 0)
        return std::nullopt;
    if (std::holds_alternative<FarCall>(key))
        return std::format("F{:x}", h);
    const auto& n = std::get<NearCall>(key);
    return std::format("N{}_{:x}", n.segment, n.offset);
}

std::optional<std::int64_t> formula_removes(const CalleeKey& key) {
    const std::uint64_t r = place_hash(key) % 7;
    return r <= 1 ? std::nullopt : std::optional<std::int64_t>(2 * (static_cast<std::int64_t>(r) - 1));
}

std::string row_text(const std::vector<Call>& found) {
    std::string out;
    for (const Call& c : found)
        out += std::format("{}{},{},{},{}", out.empty() ? "" : ";", c.callee, c.taken_off.text(),
                           c.pushed ? std::to_string(*c.pushed) : "null", c.at_leave ? "leave" : "-");
    return out;
}

}  // namespace

CB_TEST("verify.gate16.contract16.real_modules_agree_with_the_python", [] {
    platforms::DecoderSet decoders;
    const platforms::Decoder& cs = decoders.get(platforms::Arch::X86, 16, true);
    std::size_t functions = 0, with_calls = 0, rows = 0, disagreements = 0;
    for (const char* module : {"lzexpand.dll", "keyboard.drv", "ver.dll", "toolhelp.dll"}) {
        const std::string data =
            fs::read_text(testing::data_path(std::format("verify/gate16/testdata/contract16/{}.calls", module)));
        std::map<unsigned, Bytes> images;
        for (const std::string_view line : text::lines(data)) {
            const std::vector<std::string_view> p = text::split_n(line, ' ', 2);
            if (p.size() == 3 && p[0] == "segment")
                images[*text::parse_int<unsigned>(p[1])] = crypto::from_hex(p[2]);
        }
        for (const std::string_view line : text::lines(data)) {
            const std::vector<std::string_view> p = text::split_n(line, ' ', 4);
            if (p[0] != "function")
                continue;
            const unsigned segment = *text::parse_int<unsigned>(p[1]);
            const std::size_t lo = *text::parse_int<std::size_t>(p[2], 16);
            const std::size_t hi = *text::parse_int<std::size_t>(p[3], 16);
            const std::string expected = p.size() > 4 ? std::string(p[4]) : std::string();
            const std::vector<Call> got = calls(cs, images.at(segment), segment, lo, hi, formula_name, formula_removes);
            ++functions;
            with_calls += !expected.empty();
            rows += got.size();
            if (row_text(got) != expected) {
                ++disagreements;
                testing::record_failure(__FILE__, __LINE__,
                                        std::format("{} {}:{:x}: got [{}], the Python said [{}]", module, segment, lo,
                                                    row_text(got), expected));
            }
        }
    }
    CB_CHECK(functions > 200);
    CB_CHECK(with_calls > 100);
    CB_CHECK_EQ(rows, 474u);
    CB_CHECK_EQ(disagreements, 0u);
});

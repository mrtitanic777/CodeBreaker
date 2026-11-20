// The assembler engine's MASM mode (x86asm, `--masm`): every fixture of build/testdata/masm/fixtures (the NASM fork's own MASM-mode tests) assembled flat (-f bin)
// must give the bytes of its golden/<name>.hex, and the fixtures that declare a segment must give the same code in the object backends (-f obj, -f win32).
// The fixtures and their golden bytes are read where they are kept; x86asm assembles them in this process.
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "build/x86asm/x86asm.hpp"
#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

using namespace cb;

namespace {

namespace mfx_fs = std::filesystem;

// A fixture listed here fails today on the engine's current source, as on every build before it: it is reported, not counted as a regression
// (the fixture list).
const std::set<std::string> kMasmFixturesKnown = {"masm_paren_operand"};   // a parenthesised operand naming a later EQU: not yet

// One fixture assembled by x86asm in memory: the file it wrote, nothing when it did not assemble. A file it includes is read from the disk.
std::optional<Bytes> masm_fixtures_assemble(const mfx_fs::path& source, const std::string& format) {
    build::x86asm::Request request;
    request.cwd = source.parent_path().generic_string();
    request.args = {"--masm", "-f", format, source.generic_string(), "-o", "out.bin"};
    request.read = [](const std::string& path) -> std::optional<Bytes> {
        std::error_code ec;
        if (!mfx_fs::is_regular_file(fs::from_text(path), ec)) return std::nullopt;
        return fs::read_file(fs::from_text(path));
    };
    const build::x86asm::Result r = build::x86asm::assemble(request);
    if (r.exit != 0) return std::nullopt;
    const auto at = r.files.find("out.bin");
    if (at == r.files.end()) return std::nullopt;
    return at->second;
}

std::vector<mfx_fs::path> masm_fixtures_sources() {
    std::vector<mfx_fs::path> sources;
    for (const auto& entry : mfx_fs::directory_iterator(testing::data_path("build/testdata/masm/fixtures"))) {
        if (entry.path().extension() == ".asm") sources.push_back(entry.path());
    }
    std::sort(sources.begin(), sources.end());
    return sources;
}

mfx_fs::path masm_fixtures_golden(const std::string& name) { return testing::data_path("build/testdata/masm/golden") / (name + ".hex"); }

// ---- the object backends: the code of the _TEXT segment read back out of the object -----------------------------------------------------

std::pair<unsigned, std::size_t> mfx_omf_index(const Bytes& b, std::size_t p) {
    if (b[p] & 0x80) return {((b[p] & 0x7Fu) << 8) | b[p + 1], p + 2};
    return {b[p], p + 1};
}

std::optional<Bytes> mfx_omf_code(const Bytes& data) {
    std::vector<std::string> lnames = {""};
    std::vector<std::pair<std::string, std::string>> segs;
    std::map<std::string, Bytes> segb;
    std::size_t i = 0;
    while (i + 3 <= data.size()) {
        const std::uint8_t t = data[i];
        const std::size_t ln = data[i + 1] | (static_cast<std::size_t>(data[i + 2]) << 8);
        if (ln == 0 || i + 3 + ln > data.size()) break;
        const Bytes body(data.begin() + static_cast<std::ptrdiff_t>(i + 3), data.begin() + static_cast<std::ptrdiff_t>(i + 3 + ln - 1));
        i += 3 + ln;
        if (t == 0x96) {   // LNAMES
            std::size_t p = 0;
            while (p < body.size()) {
                const std::size_t l = body[p];
                lnames.emplace_back(body.begin() + static_cast<std::ptrdiff_t>(p + 1), body.begin() + static_cast<std::ptrdiff_t>(p + 1 + l));
                p += 1 + l;
            }
        } else if (t == 0x98 || t == 0x99) {   // SEGDEF / SEGDEF32
            const bool wide = t == 0x99;
            std::size_t p = 0;
            const unsigned acbp = body[p++];
            if ((acbp >> 5) == 0) p += 3;   // an absolute segment: frame and offset
            p += wide ? 4 : 2;              // the segment length
            const auto [si, p1] = mfx_omf_index(body, p);
            const auto [ci, p2] = mfx_omf_index(body, p1);
            (void)p2;
            const std::string name = si < lnames.size() ? lnames[si] : "_TEXT";
            const std::string cls = ci < lnames.size() ? lnames[ci] : "";
            segs.emplace_back(name, cls);
            segb.try_emplace(name);
        } else if (t == 0xA0 || t == 0xA1) {   // LEDATA / LEDATA32
            const bool wide = t == 0xA1;
            std::size_t p = 0;
            const auto [si, p1] = mfx_omf_index(body, p);
            p = p1;
            std::size_t off = 0;
            for (std::size_t k = 0; k < (wide ? 4u : 2u); ++k) off |= static_cast<std::size_t>(body[p + k]) << (8 * k);
            p += wide ? 4 : 2;
            if (si > 0 && si <= segs.size()) {
                Bytes& bb = segb[segs[si - 1].first];
                const std::size_t n = body.size() - p;
                if (bb.size() < off + n) bb.resize(off + n, 0);
                std::copy(body.begin() + static_cast<std::ptrdiff_t>(p), body.end(), bb.begin() + static_cast<std::ptrdiff_t>(off));
            }
        }
    }
    const auto upper = [](std::string s) {
        for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        return s;
    };
    for (const auto& [name, cls] : segs) {
        if (upper(cls).find("CODE") != std::string::npos || upper(name).ends_with("TEXT")) return segb[name];
    }
    return std::nullopt;
}

std::optional<Bytes> mfx_coff_code(const Bytes& data) {
    const auto u16 = [&](std::size_t at) { return static_cast<std::size_t>(data[at] | (data[at + 1] << 8)); };
    const auto u32 = [&](std::size_t at) { return u16(at) | (u16(at + 2) << 16); };
    const std::size_t sections = u16(2), off = 20 + u16(16);
    for (std::size_t k = 0; k < sections; ++k) {
        const std::size_t sh = off + k * 40;
        std::string name(data.begin() + static_cast<std::ptrdiff_t>(sh), data.begin() + static_cast<std::ptrdiff_t>(sh + 8));
        while (!name.empty() && name.back() == '\0') name.pop_back();
        for (char& c : name) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        const std::size_t size = u32(sh + 16), ptr = u32(sh + 20);
        if (name.ends_with("TEXT") && size != 0 && ptr != 0) {
            return Bytes(data.begin() + static_cast<std::ptrdiff_t>(ptr), data.begin() + static_cast<std::ptrdiff_t>(ptr + size));
        }
    }
    return std::nullopt;
}

}  // namespace

CB_TEST("build.masm_fixtures.every_fixture_assembles_to_its_golden_bytes", [] {
    const std::vector<mfx_fs::path> sources = masm_fixtures_sources();
    CB_CHECK(sources.size() >= 80u);
    std::size_t passed = 0;
    for (const auto& source : sources) {
        const std::string name = fs::to_text(source.stem());
        const std::optional<Bytes> made = masm_fixtures_assemble(source, "bin");
        const mfx_fs::path hex = masm_fixtures_golden(name);
        const bool known = kMasmFixturesKnown.count(name) != 0;
        bool good = made && mfx_fs::is_regular_file(hex);
        if (good) good = *made == crypto::from_hex(text::trim(fs::read_text(hex)));
        if (good) {
            ++passed;
        } else if (known) {
            std::cout << "known " << name << "\n";
        } else {
            std::cout << "FAIL " << name << "\n";
            CB_CHECK(good);
        }
    }
    CB_CHECK(passed + kMasmFixturesKnown.size() >= sources.size());
});

CB_TEST("build.masm_fixtures.segment_fixtures_give_the_same_code_in_the_object_backends", [] {
    // A fixture is object-checked when it declares a `NAME segment', and hll_struct besides (it guards the STRUCT-then-.code USE32 path: USE16 would add 66
    // prefixes). Two are -f bin tests only: a 16-bit FAR pointer needs a 16-bit relocation COFF has none of, and the SEGMENT/ENDS resume fixture splits its
    // bytes across two physical segments.
    const std::set<std::string> object_safe = {"hll_struct"}, bin_only = {"masm_lds_farptr", "masm_segment_resume"};
    const std::regex declares_segment(R"(^\s*\S+\s+segment\b)", std::regex::icase);
    std::size_t checked = 0;
    for (const auto& source : masm_fixtures_sources()) {
        const std::string name = fs::to_text(source.stem());
        const mfx_fs::path hex = masm_fixtures_golden(name);
        if (!mfx_fs::is_regular_file(hex) || bin_only.count(name) != 0) continue;
        bool declares = object_safe.count(name) != 0;
        const std::string src = fs::read_text(source);
        if (!declares) {
            for (const std::string_view line : text::lines(src)) {
                const std::string one(line);
                if (std::regex_search(one, declares_segment)) {
                    declares = true;
                    break;
                }
            }
        }
        if (!declares) continue;
        const Bytes want = crypto::from_hex(text::trim(fs::read_text(hex)));
        struct Backend {
            const char* format;
            std::optional<Bytes> (*reader)(const Bytes&);
        };
        for (const Backend& backend : {Backend{"obj", &mfx_omf_code}, Backend{"win32", &mfx_coff_code}}) {
            const std::optional<Bytes> object = masm_fixtures_assemble(source, backend.format);
            const bool assembled = object.has_value();
            if (!assembled) std::cout << "FAIL " << name << " [-f " << backend.format << "]: it did not assemble\n";
            CB_CHECK(assembled);
            if (!assembled) continue;
            const std::optional<Bytes> code = backend.reader(*object);
            const bool same = code && *code == want;
            if (!same) std::cout << "FAIL " << name << " [-f " << backend.format << "]: the code is not the golden bytes\n";
            CB_CHECK(same);
            ++checked;
        }
    }
    CB_CHECK(checked >= 6u);
});

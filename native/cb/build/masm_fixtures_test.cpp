// The assembler engine's MASM mode (native/nasm, `nasm --masm`): every fixture of native/nasm/test/masm/fixtures assembled flat (-f bin) must give the
// bytes of its golden/<name>.hex, and the fixtures that declare a segment must give the same code in the object backends (-f obj, -f win32). The fixtures and their golden bytes are read
// where they are kept. Skipped when the engine is not built (native/nasm/build/bin/nasm.exe of this checkout or the main one, or CB_NASM).
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;

namespace {

namespace mfx_fs = std::filesystem;

// A fixture listed here fails today on the engine's current source, as on every build before it: it is reported, not counted as a regression
// (the fixture list).
const std::set<std::string> kMasmFixturesKnown = {"masm_paren_operand"};   // a parenthesised operand naming a later EQU: not yet

std::optional<mfx_fs::path> masm_fixtures_nasm() {
    std::vector<mfx_fs::path> candidates;
    if (const auto given = fs::get_env("CB_NASM")) candidates.push_back(fs::from_text(*given));
    candidates.push_back(testing::data_path("../nasm/build/bin/nasm.exe"));
    try {
        candidates.push_back(config::current().main_checkout() / "native" / "nasm" / "build" / "bin" / "nasm.exe");
    } catch (const Error&) {
    }
    std::error_code ec;
    for (const auto& c : candidates) {
        if (mfx_fs::is_regular_file(c, ec)) return c;
    }
    return std::nullopt;
}

std::vector<mfx_fs::path> masm_fixtures_sources() {
    std::vector<mfx_fs::path> sources;
    for (const auto& entry : mfx_fs::directory_iterator(testing::data_path("../nasm/test/masm/fixtures"))) {
        if (entry.path().extension() == ".asm") sources.push_back(entry.path());
    }
    std::sort(sources.begin(), sources.end());
    return sources;
}

mfx_fs::path masm_fixtures_golden(const std::string& name) { return testing::data_path("../nasm/test/masm/golden") / (name + ".hex"); }

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
    const auto nasm = masm_fixtures_nasm();
    if (!nasm) {
        std::cout << "SKIP the assembler engine is not built (native/nasm/CMakeLists.txt)\n";
        return;
    }
    const std::vector<mfx_fs::path> sources = masm_fixtures_sources();
    CB_CHECK(sources.size() >= 80u);
    fs::ScopedTempDir dir("cb-masm-fixtures");
    std::size_t passed = 0;
    for (const auto& source : sources) {
        const std::string name = fs::to_text(source.stem());
        const mfx_fs::path out = dir.path() / (name + ".bin");
        const process::ProcessResult r = process::run({.argv = {fs::to_text(*nasm), "--masm", "-f", "bin", fs::to_text(source), "-o", fs::to_text(out)}});
        const mfx_fs::path hex = masm_fixtures_golden(name);
        const bool known = kMasmFixturesKnown.count(name) != 0;
        bool good = r.ok() && mfx_fs::is_regular_file(out) && mfx_fs::is_regular_file(hex);
        if (good) good = fs::read_file(out) == crypto::from_hex(text::trim(fs::read_text(hex)));
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
    const auto nasm = masm_fixtures_nasm();
    if (!nasm) {
        std::cout << "SKIP the assembler engine is not built (native/nasm/CMakeLists.txt)\n";
        return;
    }
    // A fixture is object-checked when it declares a `NAME segment', and hll_struct besides (it guards the STRUCT-then-.code USE32 path: USE16 would add 66
    // prefixes). Two are -f bin tests only: a 16-bit FAR pointer needs a 16-bit relocation COFF has none of, and the SEGMENT/ENDS resume fixture splits its
    // bytes across two physical segments.
    const std::set<std::string> object_safe = {"hll_struct"}, bin_only = {"masm_lds_farptr", "masm_segment_resume"};
    const std::regex declares_segment(R"(^\s*\S+\s+segment\b)", std::regex::icase);
    fs::ScopedTempDir dir("cb-masm-objects");
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
            const mfx_fs::path out = dir.path() / (name + "." + backend.format);
            const process::ProcessResult r =
                process::run({.argv = {fs::to_text(*nasm), "--masm", "-f", backend.format, fs::to_text(source), "-o", fs::to_text(out)}});
            const bool assembled = r.ok() && mfx_fs::is_regular_file(out);
            if (!assembled) std::cout << "FAIL " << name << " [-f " << backend.format << "]: it did not assemble\n";
            CB_CHECK(assembled);
            if (!assembled) continue;
            const std::optional<Bytes> code = backend.reader(fs::read_file(out));
            const bool same = code && *code == want;
            if (!same) std::cout << "FAIL " << name << " [-f " << backend.format << "]: the code is not the golden bytes\n";
            CB_CHECK(same);
            ++checked;
        }
    }
    CB_CHECK(checked >= 6u);
});

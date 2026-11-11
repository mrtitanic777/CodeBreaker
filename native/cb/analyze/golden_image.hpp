// The golden inputs of the analyses: a module of the Windows 95 corpus as the live Python and cbanalyze.dll were handed
// it (testdata/<module>.image: spans, function starts, imports, relocations; <module>.names: the names after discovery).
// Included by the *_test.cpp files that run an analysis over those modules.
#pragma once

#include <string>

#include "cb/analyze/native.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

namespace cb::analyze::test {

inline constexpr const char* kGoldenModules[] = {"lz32.dll",   "mosaf.dll",   "password.cpl", "dpwsock.dll", "sccview.dll",
                                                 "vswks.dll",  "iosclass.dll", "infrared.dll", "cheyprop.dll", "pa3dmxd.drv",
                                                 "mcicda.drv", "pointer.exe", "drwatson.exe", "emm386.exe",   "bigmem.drv"};

inline ImageSpec read_golden_image(const std::string& module) {
    const auto blob = cb::fs::read_file(cb::testing::data_path("analyze/testdata/" + module + ".image"));
    const auto names = cb::fs::read_file(cb::testing::data_path("analyze/testdata/" + module + ".names"));
    cb::io::ByteReader r(blob, "image blob");
    ImageSpec spec;
    const char kind = static_cast<char>(r.u8());
    spec.kind = kind == 'p' ? ImageKind::Pe : kind == 'n' ? ImageKind::Ne : ImageKind::Le;
    spec.bits = r.u8();
    spec.flat = r.u8() != 0;
    spec.has_pointers = r.u8() != 0;
    const std::uint32_t n_spans = r.u32();
    for (std::uint32_t i = 0; i < n_spans; ++i) {
        Span s;
        s.va = r.i64();
        const std::uint32_t length = r.u32();
        s.executable = r.u8() != 0;
        s.writable = r.u8() != 0;
        s.initialised = r.i64();
        const std::uint32_t name_length = r.u32();
        s.name = std::string(cb::as_text(r.bytes(name_length)));
        s.data = r.bytes_copy(length);
        spec.spans.push_back(std::move(s));
    }
    const std::uint32_t n_functions = r.u32();
    for (std::uint32_t i = 0; i < n_functions; ++i) {
        const Va start = r.i64();
        const Va limit = r.i64();
        spec.functions.emplace_back(start, limit);
    }
    const std::uint32_t n_imports = r.u32();
    for (std::uint32_t i = 0; i < n_imports; ++i) spec.imports[r.i64()] = "";
    const std::uint32_t n_pointers = r.u32();
    for (std::uint32_t i = 0; i < n_pointers; ++i) {
        const Va site = r.i64();
        Pointer p;
        p.kind = static_cast<PointerKind>(r.u8());
        const bool has_target = r.u8() != 0;
        const Va target = r.i64();
        if (has_target) p.target = target;
        spec.pointers.emplace_back(site, p);
    }
    spec.data_segment = r.i64();
    const std::uint32_t n_roots = r.u32();
    for (std::uint32_t i = 0; i < n_roots; ++i) spec.roots.push_back(r.i64());
    // the names: the functions', then the imports'
    cb::io::ByteReader nr(names, "names blob");
    for (auto* table : {&spec.function_names, &spec.imports}) {
        const std::uint32_t n = nr.u32();
        for (std::uint32_t i = 0; i < n; ++i) {
            const Va va = nr.i64();
            const std::uint32_t length = nr.u32();
            (*table)[va] = std::string(cb::as_text(nr.bytes(length)));
        }
    }
    return spec;
}

}  // namespace cb::analyze::test

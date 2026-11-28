// sdk/bench_main.cpp -- cb_sdk_bench <checkout> <state folder>: each SDK call timed against the internal call it wraps (docs/SDK_DESIGN.md,
// "the SDK may not add measurable cost"). Every pair runs the same work; a time is the fastest of several rounds, per call.
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <exception>
#include <functional>
#include <string>
#include <vector>

#include "cb/binary.h"
#include "cb/cb.h"
#include "cb/decompile.h"
#include "cb/disasm.h"
#include "core/fs.hpp"
#include "decompile/decompiler.hpp"
#include "engine/lane_kit.hpp"
#include "formats/inspect.hpp"
#include "sdk/runtime.hpp"
#include "source/processors.hpp"

namespace {

// The fastest round of `rounds` of each of two bodies, as microseconds per call; each round is `calls` calls, and the two take turns,
// so a machine that is busier for a while slows both alike.
std::pair<double, double> bench_pair(int rounds, int calls, const std::function<void()>& sdk, const std::function<void()>& internal) {
    double best_sdk = 1e300, best_internal = 1e300;
    auto round = [&](const std::function<void()>& body) {
        const auto t0 = std::chrono::steady_clock::now();
        for (int i = 0; i < calls; ++i) body();
        return std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - t0).count() / calls;
    };
    for (int r = 0; r < rounds; ++r) {
        best_sdk = std::min(best_sdk, round(sdk));
        best_internal = std::min(best_internal, round(internal));
    }
    return {best_sdk, best_internal};
}

void bench_row(const char* what, double sdk, double internal) {
    std::printf("%-44s %12.2f us %12.2f us %+8.1f%%\n", what, sdk, internal, (sdk - internal) / internal * 100.0);
}

void bench_ok(cb_status s, const char* what) {
    if (s != CB_OK) {
        std::fprintf(stderr, "%s: %s\n", what, cb_last_error());
        std::exit(1);
    }
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 3) {
        std::fprintf(stderr, "usage: cb_sdk_bench <checkout> <state folder>\n");
        return 2;
    }
    try {
        const std::string checkout = argv[1], state = argv[2];
        cb_init_options o = CB_INIT_OPTIONS_INIT;
        o.data_root = checkout.c_str();
        o.state_dir = state.c_str();
        bench_ok(cb_init(&o), "cb_init");
        const std::filesystem::path testdata = cb::fs::from_text(checkout) / "src" / "formats" / "testdata";
        std::printf("%-44s %15s %15s %9s\n", "call (per call, fastest of rounds)", "SDK", "internal", "cost");

        for (const char* name : {"lz32.dll", "lzexpand.dll"}) {
            const std::string path = cb::fs::to_text(testdata / name);
            const std::filesystem::path file = cb::fs::from_text(path);   // (the internal call is given the path the SDK makes of the text)
            const auto [sdk, internal] = bench_pair(7, 200, [&] {
                cb_binary* b = nullptr;
                bench_ok(cb_binary_open(path.c_str(), &b), "cb_binary_open");
                cb_binary_close(b);
            }, [&] { (void)cb::formats::inspect::describe(file); });
            bench_row((std::string("cb_binary_open + close: ") + name + " / describe").c_str(), sdk, internal);
        }

        // 4 KB of lz32.dll's code, as x86-32
        cb_binary* lz = nullptr;
        bench_ok(cb_binary_open(cb::fs::to_text(testdata / "lz32.dll").c_str(), &lz), "cb_binary_open");
        std::size_t n = 0;
        const cb_section* sections = cb_binary_sections(lz, &n);
        const cb::Bytes file = cb::fs::read_file(testdata / "lz32.dll");
        const cb_section* text = std::find_if(sections, sections + n, [](const cb_section& s) { return s.code != 0; });
        const std::size_t size = std::min<std::size_t>(4096, static_cast<std::size_t>(text->file_size));
        const std::uint8_t* code = file.data() + text->file_offset;
        const cb::source::processors::Processor& x86 = cb::source::processors::find("x86-32");
        const auto [sdk_dis, int_dis] = bench_pair(7, 50, [&] {
            cb_instructions* list = nullptr;
            bench_ok(cb_disasm("x86-32", code, size, text->address.offset, 0, &list), "cb_disasm");
            cb_instructions_free(list);
        }, [&] { (void)cb::source::processors::read_instructions(x86, cb::ByteView(code, size), text->address.offset, 0); });
        bench_row("cb_disasm: 4 KB of x86-32 / read_instructions", sdk_dis, int_dis);

        // decompiling, warm (the first call fills the cache; every timed call after it reads it, as both sides do)
        const std::filesystem::path lz_file = cb::fs::from_text(cb_binary_path(lz));
        const cb_function* f = nullptr;
        bench_ok(cb_binary_function_find(lz, "LZCopy", &f), "cb_binary_function_find");
        cb_string c{nullptr, 0};
        bench_ok(cb_function_decompile(f, nullptr, &c), "cb_function_decompile");
        cb_string_free(&c);
        cb::decompile::decompiler::Decompiler& d = cb::sdk_impl::kit().decompiler();
        const std::vector<cb::decompile::decompiler::Va> starts{static_cast<cb::decompile::decompiler::Va>(cb_function_address(f).offset)};
        const auto [sdk_fn, int_fn] = bench_pair(5, 2, [&] {
            cb_string s{nullptr, 0};
            bench_ok(cb_function_decompile(f, nullptr, &s), "cb_function_decompile");
            cb_string_free(&s);
        }, [&] { (void)d.compilable32(lz_file, starts); });
        bench_row("cb_function_decompile: LZCopy / compilable32", sdk_fn, int_fn);
        (void)d.as_c(lz_file);
        const auto [sdk_mod, int_mod] = bench_pair(5, 1, [&] {
            cb_binary* b = nullptr;
            bench_ok(cb_binary_open(cb_binary_path(lz), &b), "cb_binary_open");
            cb_string s{nullptr, 0};
            bench_ok(cb_module_decompile(b, nullptr, &s, nullptr), "cb_module_decompile");
            cb_string_free(&s);
            cb_binary_close(b);
        }, [&] {
            (void)cb::formats::inspect::describe(lz_file);
            (void)d.as_c(lz_file);
        });
        bench_row("open + cb_module_decompile: lz32 / describe + as_c", sdk_mod, int_mod);
        cb_binary_close(lz);
        cb_shutdown();
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "cb_sdk_bench: %s\n", e.what());
        return 1;
    }
}

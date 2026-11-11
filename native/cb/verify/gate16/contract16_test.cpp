#include "cb/verify/gate16/contract16.hpp"

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from verify/gate16/contract16.py:
//   * the description is handed in (`Described`), not computed with describe16 and kept by a pickle cache: the describer
//     belongs to decompile/describe and keeps what it describes;
//   * a callee key is a NearCall or a FarCall; the bytes taken off after a call are a TakenOff (a count, or an operand
//     read from a register, or "leave"), not an int or a string;
//   * the map file is read from the text the caller gives, and a build with no m.map is no mismatch (the Python catches
//     OSError and answers {});
//   * the Python's `low` table of at-least counts was filled and never read; it is gone;
//   * the "why" text lists the byte counts in numeric order (the Python sorted them as text, "12" before "2").

using namespace cb;
using namespace cb::verify::gate16::contract16;

namespace {

Bytes hex(std::string_view h) { return crypto::from_hex(h); }

// the names and removed bytes the tests/test_verification.py case and the harness used for far callees
std::optional<std::string> far_name(const CalleeKey& key) {
    static const std::map<std::uint32_t, std::string> names = {
        {0xE0000000, "GlobalSize"}, {0xE0000010, "GlobalAlloc"}, {0xE0000020, "none"}, {0xE0000030, "two"}};
    if (const auto* far = std::get_if<FarCall>(&key)) {
        const auto it = names.find(far->address);
        return it != names.end() ? it->second : std::format("N{:x}", far->address);
    }
    return std::format("near{:x}", std::get<NearCall>(key).offset);
}
std::optional<std::int64_t> far_removes(const CalleeKey& key) {
    static const std::map<std::uint32_t, std::int64_t> rem = {
        {0xE0000000, 2}, {0xE0000010, 6}, {0xE0000020, 0}, {0xE0000030, 4}};
    if (const auto* far = std::get_if<FarCall>(&key)) {
        const auto it = rem.find(far->address);
        return it != rem.end() ? it->second : 0;
    }
    return std::nullopt;
}

std::string show(const std::vector<Call>& calls) {
    std::string out;
    for (const Call& c : calls)
        out += std::format("[{} {} {} {}]", c.callee, c.taken_off.text(),
                           c.pushed ? std::to_string(*c.pushed) : "null", c.at_leave ? "leave" : "-");
    return out;
}

std::string calls_of(std::string_view code_hex, bool with_removes) {
    platforms::DecoderSet decoders;
    const Bytes code = hex(code_hex);
    return show(calls(decoders.get(platforms::Arch::X86, 16, true), code, 1, 0, code.size(), far_name,
                      with_removes ? Removes(far_removes) : Removes{}));
}

}  // namespace

// The cases of the Python's tests/test_verification.py and of the old harness (expected.py), whose outputs were read
// and checked by hand: each row is [callee, bytes taken off after the call, argument bytes pushed (or null), LEAVE].
CB_TEST("verify.gate16.contract16.a_calls_arguments_are_what_its_callee_removes_of_what_was_pushed", [] {
    // `GlobalAlloc(0x2042, GlobalSize(h))` pushes the flags, then GlobalSize's handle: six bytes are GlobalAlloc's, two
    // GlobalSize's
    CB_CHECK_EQ(calls_of("684220" "57" "9a000000e0" "52" "50" "9a100000e0" "cb", true),
                "[GlobalSize 0 2 -][GlobalAlloc 0 6 -]");
    // a push before a call that takes nothing is the next call's (USER's sub_14_041A)
    CB_CHECK_EQ(calls_of("bb0100" "56" "9a200000e0" "50" "9a300000e0" "cb", true), "[none 0 null -][two 0 4 -]");
    // fewer pushed than the callee removes, from the function's start: a count that differs
    CB_CHECK_EQ(calls_of("bb0100" "50" "9a100000e0" "cb", true), "[GlobalAlloc 0 2 -]");
});

CB_TEST("verify.gate16.contract16.without_what_callees_remove_no_count_is_given", [] {
    CB_CHECK_EQ(calls_of("684220" "57" "9a000000e0" "52" "50" "9a100000e0" "cb", false),
                "[GlobalSize 0 null -][GlobalAlloc 0 null -]");
    CB_CHECK_EQ(calls_of("bb0100" "56" "9a200000e0" "50" "9a300000e0" "cb", false), "[none 0 null -][two 0 null -]");
    CB_CHECK_EQ(calls_of("bb0100" "50" "9a100000e0" "cb", false), "[GlobalAlloc 0 null -]");
});

CB_TEST("verify.gate16.contract16.what_the_caller_takes_off", [] {
    for (bool with : {true, false}) {
        // `add sp, 4` after a near call: four bytes, two words pushed after the prologue
        CB_CHECK_EQ(calls_of("55" "8bec" "6a01" "6a02" "e8" "1000" "83c404" "5d" "cb", with), "[near1a 4 4 -]");
        // at /Os, `pop bx`: two bytes, taken off by popping them into a register no one reads
        CB_CHECK_EQ(calls_of("55" "8bec" "50" "e8" "0400" "5b" "5d" "cb", with), "[nearb 2 2 -]");
        // the frame's LEAVE follows: SP is set back from BP whatever was taken off
        CB_CHECK_EQ(calls_of("55" "8bec" "6a01" "9a" "10" "00" "00" "e0" "c9" "cb", with), "[GlobalAlloc 0 2 leave]");
        // a join: a jump lands after the pushes of two paths, and a path's pushes are not all known
        CB_CHECK_EQ(calls_of("55" "8bec" "6a01" "74" "03" "6a02" "50" "9a" "00" "00" "00" "e0" "83c406" "cb", with),
                    "[GlobalSize 6 null -]");
        // a call through a pointer has no callee here
        CB_CHECK_EQ(calls_of("55" "8bec" "6a01" "ff1e" "0010" "83c402" "cb", with), "");
        // room for locals (`sub sp, 16`) is not an argument; nothing pushed for the call
        CB_CHECK_EQ(calls_of("55" "8bec" "83ec10" "6a01" "e8" "0000" "cb", with), "[nearb 0 null -]");
    }
});

CB_TEST("verify.gate16.contract16.calls_clamp_the_range_and_survive_an_empty_one", [] {
    platforms::DecoderSet decoders;
    const platforms::Decoder& cs = decoders.get(platforms::Arch::X86, 16, true);
    const Bytes code = hex("55" "8bec" "6a01" "e8" "0400" "83c402" "5d" "cb");
    // a function that runs "to the segment's end" past the image is the same as one that ends at it
    CB_CHECK_EQ(show(calls(cs, code, 1, 0, 0x10000, far_name)), show(calls(cs, code, 1, 0, code.size(), far_name)));
    CB_CHECK_EQ(show(calls(cs, code, 1, 0x100, 0x200, far_name)), "");   // wholly past the end
    CB_CHECK_EQ(show(calls(cs, code, 1, 5, 3, far_name)), "");           // empty
    CB_CHECK_EQ(show(calls(cs, {}, 1, 0, 10, far_name)), "");
    // undecodable bytes end the walk, as they do the decoder
    CB_CHECK_EQ(show(calls(cs, hex("ffff" "e80000"), 1, 0, 5, far_name)), "");
});

CB_TEST("verify.gate16.contract16.a_callee_with_no_name_is_not_listed", [] {
    platforms::DecoderSet decoders;
    const Bytes code = hex("6a01" "9a000000e0" "cb");
    const auto nameless = [](const CalleeKey&) -> std::optional<std::string> { return std::nullopt; };
    CB_CHECK(calls(decoders.get(platforms::Arch::X86, 16, true), code, 1, 0, code.size(), nameless).empty());
});

CB_TEST("verify.gate16.contract16.a_count_read_from_an_operand", [] {
    // `add sp, bx` after a call: the count is read from a register
    platforms::DecoderSet decoders;
    const Bytes code = hex("55" "8bec" "6a01" "e8" "0400" "03e3" "5d" "cb");   // add sp, bx
    const std::vector<Call> found = calls(decoders.get(platforms::Arch::X86, 16, true), code, 1, 0, code.size(), far_name);
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK_EQ(found[0].taken_off.text(), "?bx");
    CB_CHECK(!found[0].pushed.has_value());
});

CB_TEST("verify.gate16.contract16.place_and_address", [] {
    CB_CHECK_EQ(address_of(1, 0x1234), 0x10001234u);
    CB_CHECK_EQ(address_of(2, 0), 0x10080000u);
    const Place p = place_of(address_of(24, 0x09B8));
    CB_CHECK_EQ(p.segment, 24);
    CB_CHECK_EQ(p.offset, 0x09B8u);
    CB_CHECK_EQ(place_of(0xE0000010).segment, (0xE000 - 0x1000) / 8 + 1);   // an import's stand-in is in no segment
    CB_CHECK(place_of(0x00010000).segment <= 0);                              // below the first selector: no segment
});

CB_TEST("verify.gate16.contract16.what_the_description_says", [] {
    const DescribedFacts f = parse_description(
        "M\tx\ty\tz\n"
        "F\t10000040\tcallee\t2\n"
        "F\t10000080\tfar_one\t-1\n"
        "X\te0000000\tDOS3CALL\n"
        "X\te0000010\tGlobalAlloc\t6\n"
        "F\t10000100\tnot_a_count\tabc\n"
        "F\tzz\tbad_address\t4\n"
        "F\t10000200\n");
    CB_CHECK_EQ(f.names.size(), 5u);
    CB_CHECK_EQ(f.names.at(0x10000040), "callee");
    CB_CHECK_EQ(f.names.at(0xE0000000), "DOS3CALL");
    CB_CHECK_EQ(f.pops.at(0x10000040).value(), 2);
    CB_CHECK(!f.pops.at(0x10000080).has_value());   // a negative count: said, and not known
    CB_CHECK_EQ(f.pops.at(0xE0000010).value(), 6);
    CB_CHECK(!f.pops.contains(0x10000100));         // not a number
    CB_CHECK(!f.pops.contains(0xE0000000));         // no count at all
    CB_CHECK(!f.names.contains(0x10000200));        // too short
});

namespace {

// Segment 1: `caller` at 0 pushes `pushes` words and calls `callee` (a near function at 0x40, `ret`), taking off `takes`.
Bytes module_image(std::initializer_list<std::uint8_t> body) {
    Bytes image(0x41, 0x90);
    std::copy(body.begin(), body.end(), image.begin());
    image[0x40] = 0xC3;   // callee: ret
    return image;
}

Described described(Bytes image, std::string extra_description = {}) {
    Described d;
    d.images[1] = std::move(image);
    d.names[address_of(1, 0)] = "caller";
    d.names[address_of(1, 0x40)] = "callee";
    d.description = "F\t10000000\tcaller\nF\t10000040\tcallee\n" + extra_description;
    // the nearest return of the function at `offset`: this module's functions end in `ret`
    d.returns = [](ByteView code, std::uint32_t offset, bool far) -> std::optional<std::int64_t> {
        if (far || offset >= code.size() || code[offset] != 0xC3)
            return std::nullopt;
        return 0;
    };
    return d;
}

const std::string kMap = "\r\nAddress         Publics by Value\r\n\r\n0001:0000       caller\r\n0001:0040       _callee@2\r\n";

// push 1; call callee; add sp, <takes>
Bytes one_push(std::uint8_t takes) { return module_image({0x55, 0x8B, 0xEC, 0x6A, 0x01, 0xE8, 0x38, 0x00, 0x83, 0xC4, takes, 0x5D, 0xC3}); }
// push 1; push 2; call callee; add sp, 4
Bytes two_pushes() { return module_image({0x55, 0x8B, 0xEC, 0x6A, 0x01, 0x6A, 0x02, 0xE8, 0x36, 0x00, 0x83, 0xC4, 0x04, 0x5D, 0xC3}); }

const std::vector<std::string> kNames = {"caller", "callee", "not_in_the_original"};

}  // namespace

CB_TEST("verify.gate16.contract16.a_module_against_itself_has_no_mismatch", [] {
    const Described o = described(one_push(2));
    CB_CHECK(mismatches(o, o, kMap, kNames).empty());
});

CB_TEST("verify.gate16.contract16.the_caller_takes_off_other_bytes", [] {
    const auto found = mismatches(described(one_push(2)), described(one_push(4)), kMap, kNames);
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK_EQ(found.at("caller"), "after calling callee the original takes off [2] bytes, ours [4]");
    // a function that is not asked about is not reported
    const std::vector<std::string> only_callee = {"callee"};
    CB_CHECK(mismatches(described(one_push(2)), described(one_push(4)), kMap, only_callee).empty());
});

CB_TEST("verify.gate16.contract16.the_build_pushes_fewer_arguments", [] {
    // the same bytes taken off (four), but the build pushed one word where the original pushed two
    const auto found = mismatches(described(two_pushes()), described(one_push(4)), kMap, kNames);
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK_EQ(found.at("caller"), "it pushes [2] argument bytes for callee, the original [4]");
}) ;

CB_TEST("verify.gate16.contract16.a_callee_that_takes_its_arguments_in_registers", [] {
    // both call far through the import DOS3CALL; the build's map does not know it, its description does
    const Bytes code = [] {
        Bytes b = one_push(2);
        const Bytes far_call = hex("9a000000e0");   // lcall E000:0000 -> 0xE0000000
        std::copy(far_call.begin(), far_call.end(), b.begin() + 5);   // (replaces call + add: a far call to the import)
        return b;
    }();
    const std::string x = "X\te0000000\tDOS3CALL\n";
    const auto found = mismatches(described(code, x), described(code, x), kMap, kNames);
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK_EQ(found.at("caller"), "it calls DOS3CALL, which takes its arguments in registers");
});

CB_TEST("verify.gate16.contract16.the_link_map_decoration_is_removed_and_junk_lines_are_skipped", [] {
    // "_callee@2" is "callee"; lines that are not symbols (header, blank, a bad address) say nothing
    const std::string map = "Publics\r\n\r\n0001:zzzz       broken\r\nnot a line\r\n0001:0000       caller\r\n0001:0040+      _callee@2\r\n"
                            "0001:0050       two words\r\n";
    const auto found = mismatches(described(one_push(2)), described(one_push(4)), map, kNames);
    CB_CHECK_EQ(found.size(), 1u);
    // no symbols: nothing is compared
    CB_CHECK(mismatches(described(one_push(2)), described(one_push(4)), "", kNames).empty());
    CB_CHECK(mismatches(described(one_push(2)), described(one_push(4)), "garbage", kNames).empty());
});

CB_TEST("verify.gate16.contract16.mismatches_of_two_paths", [] {
    fs::ScopedTempDir tmp("cb-contract16");
    const Describer describe = [](const std::filesystem::path& p) {
        return described(p.filename() == "orig.dll" ? one_push(2) : one_push(4));
    };
    const std::vector<std::string> names = {"caller"};
    // no map in the folder: the build did not link; nothing to say
    CB_CHECK(mismatches(describe, "orig.dll", tmp.path(), "built.dll", names).empty());
    fs::write_text(tmp.path() / "m.map", kMap);
    const auto found = mismatches(describe, "orig.dll", tmp.path(), "built.dll", names);
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK(found.contains("caller"));
    CB_CHECK(mismatches(describe, "orig.dll", tmp.path(), "orig.dll", names).empty());   // the module against itself
});

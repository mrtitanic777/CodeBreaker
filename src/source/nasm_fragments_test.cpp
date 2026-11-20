// source/nasm_fragments_test.cpp -- the NASM writers of the NE, PE and LE images give each function a MachineFragment of its own (its origin, its length to
// the next) and the file's text is unchanged by it: the fragments joined are the file
// (on lzexpand.dll, lz32.dll and splitter.vxd).
//
// Names here are file-local to this unity unit: every one starts with `nf_`.
#include <string>
#include <vector>

#include "core/arena.hpp"
#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/le_writer.hpp"
#include "source/ne_writer.hpp"
#include "source/pe_writer.hpp"
#include "translate/l2.hpp"

namespace {

// The units of the writer's L2 tree: how many are functions, whether each function has an origin in the module and a length, and the fragments' text
// joined by a line break.
struct NfTree {
    std::size_t functions = 0;
    bool all_placed = true;   // every function: a length above zero and the module's name for its origin
    std::string joined;
};

NfTree nf_read(const cb::translate::l2::Assembly& assembly, const std::string& module) {
    cb::Arena arena;
    const auto doc = cb::translate::l2::to_json(*assembly.document(arena));
    NfTree tree;
    bool first = true;
    for (const auto& unit : doc.at("units")) {
        if (!first) tree.joined += '\n';
        first = false;
        tree.joined += unit.value("asm", std::string());
        if (unit.value("reason", std::string()) != "function") continue;
        ++tree.functions;
        if (unit.value("length", 0) <= 0 || unit.at("origin").value("module", std::string()) != module) tree.all_placed = false;
    }
    return tree;
}

std::filesystem::path nf_data(std::string_view name) { return cb::testing::data_path("formats/testdata/" + std::string(name)); }

}  // namespace

CB_TEST("source.nasm_fragments.an_ne_program_is_one_fragment_a_function", [] {
    cb::source::ne_writer::Image image = cb::source::ne_writer::Image::open(nf_data("lzexpand.dll"));
    const std::string text = image.source({}, true, false).text;
    const NfTree tree = nf_read(image.assembly(), "lzexpand.dll");
    CB_CHECK(tree.functions > 5);
    CB_CHECK(tree.all_placed);
    CB_CHECK_EQ(tree.joined, text);
});

CB_TEST("source.nasm_fragments.a_pe_program_is_one_fragment_a_function", [] {
    cb::source::pe_writer::Image image = cb::source::pe_writer::Image::open(nf_data("lz32.dll"));
    const std::string text = image.source({}, true).text;
    const NfTree tree = nf_read(image.assembly(), "lz32.dll");
    CB_CHECK(tree.functions > 5);
    CB_CHECK(tree.all_placed);
    CB_CHECK_EQ(tree.joined, text);
});

CB_TEST("source.nasm_fragments.an_le_driver_is_one_fragment_a_function", [] {
    cb::source::le_writer::Image image = cb::source::le_writer::Image::open(nf_data("splitter.vxd"));
    const std::string text = image.source({}, true, true).text;
    const NfTree tree = nf_read(image.assembly(), "splitter.vxd");
    CB_CHECK(tree.functions > 5);
    CB_CHECK(tree.all_placed);
    CB_CHECK_EQ(tree.joined, text);
});

// cb-unity: alone (file-local names that other files in a batch also use)
// The console-lane source writers' speed and sameness, on programs of any size (docs/SPEED.md). Nothing runs unless CB_WRITER_SPEED_FILES names files (separated
// by ';'): each is written as source and built again in CB_WRITER_SPEED_OUT (a folder), and one line says its writer's wall seconds, whether it rebuilt as the
// original, and the SHA-256 of the source and of the rebuilt file, so that a run before a change and a run after can be compared line by line. With CB_PROFILE set
// the writers' stages are printed when the process ends.
//
//     cb_tests_source --filter source.writers_speed       (CB_WRITER_SPEED_FILES=a.nro;b.3dsx CB_WRITER_SPEED_OUT=out)
#include <chrono>
#include <iostream>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/source/gba_writer.hpp"
#include "cb/source/gekko_writer.hpp"
#include "cb/source/n3ds_writer.hpp"
#include "cb/source/n64_writer.hpp"
#include "cb/source/nds_writer.hpp"
#include "cb/source/ps3_writer.hpp"
#include "cb/source/psp_writer.hpp"
#include "cb/source/vita_writer.hpp"
#include "cb/source/switch_writer.hpp"

namespace cb::source {
namespace {

std::string digest_of(const std::filesystem::path& file) {
    if (!std::filesystem::is_regular_file(file)) return "-";
    const Bytes bytes = fs::read_file(file);
    return crypto::to_hex(crypto::sha256(bytes));
}

CB_TEST("source.writers_speed.files", [] {
    const std::optional<std::string> list = fs::get_env("CB_WRITER_SPEED_FILES");
    if (!list || list->empty()) return;   // (nothing asked for)
    const std::optional<std::string> out = fs::get_env("CB_WRITER_SPEED_OUT");
    CB_CHECK(out && !out->empty());
    if (!out || out->empty()) return;
    std::size_t at = 0;
    while (at <= list->size()) {
        std::size_t end = list->find(';', at);
        if (end == std::string::npos) end = list->size();
        const std::filesystem::path file = fs::from_text(list->substr(at, end - at));
        at = end + 1;
        if (file.empty()) continue;
        const std::filesystem::path folder = fs::from_text(*out) / file.filename();
        const std::string ext = text::lower(fs::to_text(file.extension()));
        const auto start = std::chrono::steady_clock::now();
        image_writer::Written w;
        if (ext == ".pbp" || ext == ".prx") w = psp_writer::reconstruct(file, folder);
        else if (ext == ".3dsx") w = n3ds_writer::reconstruct(file, folder);
        else if (ext == ".nro" || ext == ".nso") w = switch_writer::reconstruct(file, folder);
        else if (ext == ".dol" || ext == ".rpx" || ext == ".rpl") w = gekko_writer::reconstruct(file, folder);
        else if (ext == ".sprx" || file.filename() == "EBOOT.BIN") w = ps3_writer::reconstruct(file, folder);
        else if (ext == ".elf" && fs::to_text(file).find("ps3") != std::string::npos) w = ps3_writer::reconstruct(file, folder);
        else if (ext == ".elf" && fs::to_text(file).find("3ds") != std::string::npos) w = n3ds_writer::reconstruct(file, folder);
        else if (ext == ".elf") w = gekko_writer::reconstruct(file, folder);
        else if (ext == ".bin" && fs::to_text(file).find("vita") != std::string::npos) w = vita_writer::reconstruct(file, folder);
        else if (ext == ".nds") w = nds_writer::reconstruct(file, folder);
        else if (ext == ".gba") w = gba_writer::reconstruct(file, folder);
        else if (ext == ".n64" || ext == ".z64") w = n64_writer::reconstruct(file, folder);
        else {
            std::cout << file.filename().string() << ": no writer for this extension\n";
            CB_CHECK(false);
            continue;
        }
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        std::cout << file.filename().string() << ": " << w.functions.size() << " functions, " << (w.same ? "identical" : "DIFFERENT") << ", " << seconds << " s, source "
                  << digest_of(w.source) << ", built " << (w.built ? digest_of(*w.built) : std::string("-")) << std::endl;
        CB_CHECK(w.same);
    }
});

}  // namespace
}  // namespace cb::source

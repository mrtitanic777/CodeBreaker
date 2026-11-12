// cb/java/projects/minecraft/applet_ping.hpp -- neutralise the applet document-base "level count" / host
// ANTI-PIRACY PING.
//
// Early Minecraft (Indev's Minecraft.setLevel) opens a URL to the APPLET's document base to ping a level count and
// verify the game is running on minecraft.net, gating the level load on it:
//
//     try {
//         BufferedReader var2 = new BufferedReader(new InputStreamReader(
//             new URL(getDocumentBase() + "?n=" + user + "&i=" + sess).openStream()));
//         Integer.parseInt(var2.readLine());
//         var2.close();
//         if (getDocumentBase().toString().startsWith("http://www.minecraft.net/") || ...) {
//             theWorld = var1;
//         }
//     } catch (Throwable var3) {
//     }
//
// On the console there is NO applet -- URL.openStream() is a NULL-returning stub, so readLine() dereferences a NULL
// stream. That is an ACCESS VIOLATION, not a Java exception, so the try/catch does NOT catch it -> a hard crash the
// instant setLevel runs. And setLevel runs on the VERY FIRST line of generateLevel (`setLevel((World*)NULL)`), so
// pressing "Create" in the new-level menu dies immediately.
//
// The proven OG Indev hand-port DROPS the whole ping and sets the level unconditionally ("applet host check n/a on
// 360"). This pass migrates that into the generator so every edition's setLevel loses the ping. Keyed on the STRUCTURE
// (a try that opens getDocumentBase()->openStream() and guards a `theWorld = <var>` assignment on the host check), not
// on names.
//
// The conversion applies it on the IR before the back end writes (patch(), by tree shape), then checks the output for a
// surviving trigger (stranded()). rewrite()/run() are the same fix on C++ text, for `codebreaker java applet-ping
// <dir>` on a tree converted elsewhere; that text form runs over the converted tree (xbox360), BEFORE overlays.
// Idempotent (after the rewrite there is no getDocumentBase()->openStream() try left) and fail-loud through the
// survivor check.
#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "cb/translate/ir.hpp"

namespace cb::java::projects::minecraft::applet_ping {

// How many ping sites one file (or one class) had rewritten.
struct FileSites {
    std::string file;
    std::size_t sites = 0;
};

struct Report {
    std::vector<FileSites> files;
    std::size_t sites = 0;
    // Files where a getDocumentBase()->openStream() try SURVIVED the rewrite (the structure drifted: fail loud).
    std::vector<std::string> stranded;
};

struct Rewritten {
    std::string text;
    std::size_t sites = 0;
};

// Rewrite the setLevel URL ping in one .cpp's text.
Rewritten rewrite(std::string_view text);

// Neutralise the ping across the *.cpp files directly in a converted tree (files are rewritten in place).
Report run(const std::filesystem::path& out_dir);

// The same fix on the lowered program (`units` are Unit nodes of one arena): a try that calls getDocumentBase() and
// openStream(), whose only effect is an `if (host check) { this.f = v; }`, with empty catches, becomes `this.f = v;`.
// The report has one entry per class, named <class>.cpp; `stranded` is empty.
Report patch(const std::vector<translate::ir::Node*>& units);

// The *.cpp files of a converted tree where a getDocumentBase()->openStream() try survived.
std::vector<std::string> stranded(const std::filesystem::path& out_dir);

}  // namespace cb::java::projects::minecraft::applet_ping

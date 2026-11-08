// codebreaker/java/projects/minecraft/applet_ping.hpp -- C++ port of codebreaker.java.projects.minecraft.applet_ping.
//
// applet_ping.py -- neutralize the applet document-base "level count" / host ANTI-PIRACY PING.
//
// Early Minecraft (Indev's Minecraft.setLevel) opens a URL to the APPLET's document base to ping a level
// count and verify the game is running on minecraft.net, gating the level load on it:
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
// On the console there is NO applet -- URL.openStream() is a NULL-returning stub, so readLine() dereferences
// a NULL stream. That is an ACCESS VIOLATION, not a Java exception, so the try/catch does NOT catch it -> a
// hard crash the instant setLevel runs. And setLevel runs on the VERY FIRST line of generateLevel
// (`setLevel((World*)NULL)`), so pressing "Create" in the new-level menu dies immediately.
//
// The proven OG Indev hand-port (C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftIndev) DROPS the
// whole ping and sets the level unconditionally ("applet host check n/a on 360"). This pass migrates that into
// the generator so every edition's setLevel loses the ping. Keyed on the STRUCTURE (a try that opens
// getDocumentBase()->openStream() and guards a `theWorld = <var>` assignment on the host check), not on names.
//
// convert applies it on the IR before the back end writes (patch(), by tree shape), then checks the output
// for a surviving trigger (stranded()). run()/process() are the same fix on C++ text, for `codebreaker java <fix> <dir>` on
// a tree converted elsewhere; that text form runs over the converted tree (xbox360), BEFORE overlays. Idempotent (after the rewrite
// there is no getDocumentBase()->openStream() try left) and fail-loud via run()'s survivor check. Also exposed
// as `codebreaker java applet-ping <dir>`.
//
// Typing notes:
//   * the Python's result dicts {'files': [{'file','sites'}...], 'sites': N, 'stranded': [files]} are the
//     structs Touched and Result (run() and patch() return the same record);
//   * patch() takes the units (Unit nodes) of the lowered program.
#pragma once

#include <cstdint>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"
#include "codebreaker/translate/ir.hpp"

namespace codebreaker::java::projects::minecraft::applet_ping {

// The setLevel ping: a try that (1) opens getDocumentBase()->...->openStream(), (2) guards a
// `this-><field> = <var>;` assignment inside an if on the host check, (3) is followed by an empty catch.
// `[^{}]*?` keeps each span inside the brace-free prefix so we don't cross into the if/catch bodies wrongly.
extern const py::re::Pattern _PING;

extern const py::str _NOTE;

// fail-loud: an openStream() on getDocumentBase() still guarded by a try that survived the pass.
extern const py::re::Pattern _SURVIVOR;

// One file the pass changed: {'file': name, 'sites': n}.
struct Touched {
    py::str file;
    int64_t sites = 0;
};

// {'files': [...], 'sites': N, 'stranded': [files]}
struct Result {
    py::list<Touched> files;
    int64_t sites = 0;
    py::list<py::str> stranded;
};

// Rewrite the setLevel URL ping in one .cpp. Returns (new_text, n_sites).
std::tuple<py::str, int64_t> process(const py::str& text);

// Neutralize the applet ping across the converted tree. Returns
// {'files': [{'file','sites'}...], 'sites': N, 'stranded': [files]}. `stranded` = files where a
// getDocumentBase()->openStream() try SURVIVED the rewrite (structure drifted -> fail loud).
Result run(const py::str& out_dir);

// ---- the same fix on the IR, before the back end writes (what convert runs) ------------------------------
// The ping on the lowered program: a try that calls getDocumentBase() and openStream(), whose only
// effect is an `if (host check) { this.f = v; }`, with empty catches -> just `this.f = v;`. Returns
// {'files': [{'file','sites'}], 'sites': N, 'stranded': []} like run().
Result patch(const py::list<codebreaker::translate::ir::NodeP>& units);

// The .cpp files where a getDocumentBase()->openStream() try survived.
py::list<py::str> stranded(const py::str& out_dir);

}  // namespace codebreaker::java::projects::minecraft::applet_ping

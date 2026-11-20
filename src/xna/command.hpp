// xna/command.hpp -- `codebreaker xna`, complete.
//
//     xna <package, folder or game .exe> [--name N] [--no-content]     an XNA game as a Converted project (run.hpp)
//     xna port <project> [--overlay DIR] [--run SECONDS] [--check-content]     its C# built and run on the PC (port.hpp)
//
//     table.set("xna", xna::command::runner({.config = config}));
//
// The two modules take what they work with as values; this makes them from the configuration: the Converted folder (setting converted), ILSpy and
// de4dot (settings ilspycmd, de4dot), the checkout's runtime for the port (codebreaker/xna/runtime, apistubs), the overlays folder (xna_overlays),
// MonoGame's effect compiler and ffmpeg (settings mgfxc, ffmpeg; else on PATH), and the converters of the formats the games ship: .xnb content
// (xna/xnb), XACT wave and sound banks (xna/xact).
#pragma once

#include <functional>
#include <span>
#include <string>

#include "base/config.hpp"
#include "xna/port.hpp"
#include "xna/run.hpp"

namespace cb::xna::command {

struct Environment {
    cb::config::Config config;
};

// What `convert` is given for the content: the .xnb converters (the record each makes as JSON text).
run::Hooks run_hooks(const Environment& environment);
// What the port is given: the readers renamed as the port reads them, XACT banks decoded.
port::Hooks port_hooks(const Environment& environment);
run::Environment run_environment(const Environment& environment);
port::Tools port_tools(const Environment& environment);

int run(std::span<const std::string> args, const Environment& environment);
std::function<int(std::span<const std::string>)> runner(const Environment& environment);

}  // namespace cb::xna::command

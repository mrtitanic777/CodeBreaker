// ui/report.hpp -- `codebreaker report MODULE [--out FILE]`: one page about a program, to read or to share. What it is, what made it, how much of
// its code came back as source and how faithful that source is (the store's verdicts), the functions still to look at, and where its source lives.
// The same reading as the MCP server's `triage` tool, written as one self-contained HTML file (no scripts, no outside files, light and dark).
//
//     codebreaker report lz32.dll                      writes <converted>/lz32.dll-report.html and prints where
//     codebreaker report C:\Windows\notepad.exe --out notepad.html
#pragma once

#include <memory>
#include <string>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/cli.hpp"
#include "core/json.hpp"

namespace cb::ui::report {

// The page for what the triage tool answered about a module (its JSON). No operating-system API, nothing read: tested on any machine.
std::string html(const Json& triage);

struct Environment {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;   // the configuration's own when null
};
cli::Command command(const Environment& environment);

}  // namespace cb::ui::report

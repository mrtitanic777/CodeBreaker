// formats/xbox/summary.hpp -- what `info` adds to the format line of an Xbox or Xbox 360 file.
//
//     inspect::DescribeHooks hooks;  hooks.xbox_summary = xbox::summary;
//
// A container (a disc image, an STFS package, a FATX drive ...) is described by its volumes: how many, and for each its kind, label, file
// count and bytes, and the boot-sector fields it shows. An asset is described by its own header: an XBE by its title, id, base, sections,
// region and media; an XDBF by its version, entry count per namespace and achievements; an XPR bundle by its format and size; an XACT wave
// bank by its version, byte order, entry count and name; a XUR scene by its version, tool version and sections; a Bink video by its header;
// a CHD by its sizes and codecs. A file whose format has nothing to add gives no facts.
//
// Errors: a damaged file is a cb::Error (FormatError ...); `info` catches it and keeps the format line.
#pragma once

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include "formats/detect.hpp"

namespace cb::formats::xbox {

// "label: value" pairs, as cb::formats::inspect::Facts.
using SummaryFacts = std::vector<std::pair<std::string, std::string>>;

// `match` is the first of detect::identify(path).
SummaryFacts summary(const std::filesystem::path& path, const detect::Detected& match);

}  // namespace cb::formats::xbox

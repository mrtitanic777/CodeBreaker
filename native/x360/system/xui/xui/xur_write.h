#pragma once

// Writing XUR scenes (version 8, the format xur.h reads) and compiling XUI's XML scene source (the XDK's .xui files,
// XuiTool's format) into them: our own xui2bin. The XDK 21256's xui2bin.exe writes version 5 (pool indices for
// floats are not used there, and XAM 17559 reads only 8), so the reference for what is written is XAM's own scenes:
// every scene of the dashboard and XAM packages must read back identically after a write (CB_XUI_TEST=1).
//
// The XML: an element is a tag named after its XUI class with a <Properties> block (one tag per property; indexed
// properties carry index="n"; property objects such as Fill nest their own <Properties>; a figure's Points are
// "count,x,y,c1x,c1y,c2x,c2y,type,..."), its child elements, and <Timelines>: <NamedFrames> (<NamedFrame> Name, Time,
// Command play|stop|goto|gotoandplay|gotoandstop, CommandParams) and <Timeline> per animated child (Id, one
// <TimelineProp [index=]>Fill.FillColor</TimelineProp> per track, <KeyFrame> Time, Interpolation, EaseIn, EaseOut,
// EaseScale, and one <Prop> per track).

#include "xui/xur.h"

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace xui
{
    // The figure path a kCustom value refers to (null if there is none).
    using FigureLookup = std::function<const FigurePath*(uint32_t custom)>;

    // Writes `root` as a version 8 scene. Returns false with a reason in `error`.
    bool WriteScene(const Node& root, const FigureLookup& figure, std::vector<uint8_t>& out, std::string& error);

    // Compiles XUI XML (UTF-8) into a version 8 scene.
    bool CompileScene(std::string_view xml, std::vector<uint8_t>& out, std::string& error);
}

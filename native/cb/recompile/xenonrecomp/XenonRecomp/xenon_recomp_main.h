#pragma once

// The XenonRecomp program as a function (LibXenonRecomp): `xenon_recomp_main({"XenonRecomp", config.toml, ppc_context.h})`
// recompiles a title, or the instruction tests when the first argument is a folder. Returns the program's exit code;
// its messages go to xenon_log (stdout unless the caller collects them, xenon_log::Capture).
int xenon_recomp_main(int argc, char* argv[]);

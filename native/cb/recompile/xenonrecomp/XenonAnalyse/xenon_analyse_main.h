#pragma once

// The XenonAnalyse program as a function (LibXenonAnalyseMain): `xenon_analyse_main({"XenonAnalyse", title.xex,
// switch_tables.toml})` writes the switch tables it finds. Returns the program's exit code; its messages go to xenon_log.
int xenon_analyse_main(int argc, char** argv);

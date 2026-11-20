// command.h - the xex engine's command line, run in-process by `codebreaker xex` (cb::formats::xex::run).
#pragma once

// argv[1] is the command (info, list, extract, resources, xml, idc, compress, ..., selftest); returns the exit code.
// Writes its report to stdout and its errors to stderr, as the xex program did.
int xex_command(int argc, char** argv);

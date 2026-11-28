# The CodeBreaker SDK reference

Made from the comments of `include/cb` by `tools\sdk\reference.cmd` (src/sdk/reference.cpp); do not edit these pages, edit the headers. The design is [docs/SDK_DESIGN.md](../../SDK_DESIGN.md); the samples are in [samples/](../../../samples/).

## Headers

- [binary.h](binary_h.md): open any file CodeBreaker reads: its format, processor, sections, imports, exports and functions.
- [cb.h](cb_h.md): the CodeBreaker SDK: the version, starting and stopping the library, errors, strings and the handle types.
- [decompile.h](decompile_h.md): a function or a whole module as C.
- [disasm.h](disasm_h.md): the instructions of any processor CodeBreaker has a description of.
- [version.h](version_h.md): the CodeBreaker SDK's version, as macros only (the resource compiler reads this file too).
- [cb.hpp](cb_hpp.md): the CodeBreaker SDK for C++: the C interface (cb.h, binary.h, disasm.h, decompile.h) with RAII handles, std::string_view and std::span, and errors as exceptions. Header only, C++20; it adds nothing to a call but the check of its status.

## Everything, by name

| Name | Kind | Header | Since | Summary |
|---|---|---|---|---|
| [CB_DECOMPILE_OPTIONS_INIT](decompile_h.md#cb_decompile_options_init) | macro | decompile.h | 0.1 | The defaults of cb_decompile_options. |
| [CB_INIT_OPTIONS_INIT](cb_h.md#cb_init_options_init) | macro | cb.h | 0.1 | The defaults of cb_init_options. |
| [CB_SDK_MAKE_VERSION](version_h.md#cb_sdk_make_version) | macro | version.h | 0.1 | A version as one number, (major << 16) \| (minor << 8) \| patch, which compares as versions do. |
| [CB_SDK_VERSION](version_h.md#cb_sdk_version) | macro | version.h | 0.1 | The version of the headers as one number; compare it with cb_version(), the library's. |
| [CB_SDK_VERSION_MAJOR](version_h.md#cb_sdk_version_major) | macro | version.h | 0.1 | The major version of the headers this program is compiled against. |
| [CB_SDK_VERSION_MINOR](version_h.md#cb_sdk_version_minor) | macro | version.h | 0.1 | The minor version of the headers. |
| [CB_SDK_VERSION_PATCH](version_h.md#cb_sdk_version_patch) | macro | version.h | 0.1 | The patch version of the headers. |
| [CB_SDK_VERSION_STRING](version_h.md#cb_sdk_version_string) | macro | version.h | 0.1 | The version of the headers as text, "major.minor.patch". |
| [cb_address](binary_h.md#cb_address) | type | binary.h | 0.1 | A place in a binary's address space. |
| [cb_address_format](functions/cb_address_format.md) | function | binary.h | 0.1 | An address as text, into a caller-owned buffer: "0x10001000", "3:01a4", "1234:0010", "1:00001000", "" for none. |
| [cb_address_style](binary_h.md#cb_address_style) | enum | binary.h | 0.1 | How a cb_address is to be read. |
| [cb_arch](disasm_h.md#cb_arch) | type | disasm.h | 0.1 | One processor CodeBreaker knows. |
| [cb_arch_find](functions/cb_arch_find.md) | function | disasm.h | 0.1 | The processor a name or one of its aliases names, compared without case ("I386" is "x86-32"). |
| [cb_arch_list](functions/cb_arch_list.md) | function | disasm.h | 0.1 | Every processor CodeBreaker knows, in a fixed order. |
| [cb_binary](cb_h.md#cb_binary) | type | cb.h | 0.1 | An open file of any format CodeBreaker reads (binary.h). Opaque; cb_binary_open makes one, cb_binary_close frees it. |
| [cb_binary_arch](functions/cb_binary_arch.md) | function | binary.h | 0.1 | The processor the file's code is for, by the name cb_disasm takes ("x86-32", "x86-16", "ppc", "a64" ...). |
| [cb_binary_close](functions/cb_binary_close.md) | function | binary.h | 0.1 | Free a binary, its functions and every record and string it gave. |
| [cb_binary_entry](functions/cb_binary_entry.md) | function | binary.h | 0.1 | Where the program starts. |
| [cb_binary_exports](functions/cb_binary_exports.md) | function | binary.h | 0.1 | Everything the binary exports, in the order of its export table. |
| [cb_binary_format](functions/cb_binary_format.md) | function | binary.h | 0.1 | What the file is: "PE", "NE", "LE", "LX", "W3", "MZ" for an executable, else the format line of what it was |
| [cb_binary_function](functions/cb_binary_function.md) | function | binary.h | 0.1 | One of the binary's functions, in address order. |
| [cb_binary_function_at](functions/cb_binary_function_at.md) | function | binary.h | 0.1 | The function that starts at an address: one the binary names, else a new one named sub_<address>, added to |
| [cb_binary_function_count](functions/cb_binary_function_count.md) | function | binary.h | 0.1 | How many functions the binary names: its entry point and the exports that are code, by address. |
| [cb_binary_function_find](functions/cb_binary_function_find.md) | function | binary.h | 0.1 | The function with this name (an export's name, "entry"), compared exactly. |
| [cb_binary_image_base](functions/cb_binary_image_base.md) | function | binary.h | 0.1 | Where the executable asks to be loaded (a PE's image base); 0 for a format without one. |
| [cb_binary_imports](functions/cb_binary_imports.md) | function | binary.h | 0.1 | Every function the binary imports, module by module, in the order the file lists them. |
| [cb_binary_kind](functions/cb_binary_kind.md) | function | binary.h | 0.1 | The lane that reads the file ("pe", "ne", "le", "dos"), or the family of a file that is not an executable. |
| [cb_binary_open](functions/cb_binary_open.md) | function | binary.h | 0.1 | Open a file and read what it is. |
| [cb_binary_path](functions/cb_binary_path.md) | function | binary.h | 0.1 | The path the binary was opened from. |
| [cb_binary_sections](functions/cb_binary_sections.md) | function | binary.h | 0.1 | The binary's sections, in the order the file lists them. |
| [cb_binary_size](functions/cb_binary_size.md) | function | binary.h | 0.1 | The file's size in bytes. |
| [cb_decompile_options](decompile_h.md#cb_decompile_options) | type | decompile.h | 0.1 | How a decompile call works. Make one with CB_DECOMPILE_OPTIONS_INIT. |
| [cb_decompiler_check](functions/cb_decompiler_check.md) | function | decompile.h | 0.1 | Whether the decompiler can run: its program and its processor descriptions are where the data root says. |
| [cb_disasm](functions/cb_disasm.md) | function | disasm.h | 0.1 | Read instructions from bytes. |
| [cb_export](binary_h.md#cb_export) | type | binary.h | 0.1 | One function or value the binary exports. |
| [cb_flow](disasm_h.md#cb_flow) | enum | disasm.h | 0.1 | How control leaves an instruction, as its description says. |
| [cb_function](cb_h.md#cb_function) | type | cb.h | 0.1 | One function of an open binary (binary.h, decompile.h). Opaque; owned by its binary, valid until it is closed. |
| [cb_function_address](functions/cb_function_address.md) | function | binary.h | 0.1 | Where the function starts. |
| [cb_function_binary](functions/cb_function_binary.md) | function | binary.h | 0.1 | The binary the function belongs to. |
| [cb_function_decompile](functions/cb_function_decompile.md) | function | decompile.h | 0.1 | One function as C. |
| [cb_function_name](functions/cb_function_name.md) | function | binary.h | 0.1 | The function's name ("LZCopy", "entry", "sub_10001000"). |
| [cb_import](binary_h.md#cb_import) | type | binary.h | 0.1 | One function the binary imports. |
| [cb_init](functions/cb_init.md) | function | cb.h | 0.1 | Set the library up. Call it once before any other call that does work; each cb_init is matched by a cb_shutdown. |
| [cb_init_options](cb_h.md#cb_init_options) | type | cb.h | 0.1 | How cb_init sets the library up. Make one with CB_INIT_OPTIONS_INIT and change what you need. |
| [cb_instruction](disasm_h.md#cb_instruction) | type | disasm.h | 0.1 | One instruction. |
| [cb_instructions](cb_h.md#cb_instructions) | type | cb.h | 0.1 | The instructions one cb_disasm call read (disasm.h). Opaque; cb_instructions_free frees it. |
| [cb_instructions_data](functions/cb_instructions_data.md) | function | disasm.h | 0.1 | The instructions one cb_disasm call read, in address order. |
| [cb_instructions_free](functions/cb_instructions_free.md) | function | disasm.h | 0.1 | Free a list of instructions and every string in it. |
| [cb_last_error](functions/cb_last_error.md) | function | cb.h | 0.1 | What the last call on this thread that failed said: what failed, on which input, and what to do. |
| [cb_module_decompile](functions/cb_module_decompile.md) | function | decompile.h | 0.1 | Every function of a binary as the text of one .c file. |
| [cb_module_info](decompile_h.md#cb_module_info) | type | decompile.h | 0.1 | What cb_module_decompile made. |
| [cb_section](binary_h.md#cb_section) | type | binary.h | 0.1 | A section of a PE, a segment of an NE, an object of an LE, the program of a DOS file. |
| [cb_shutdown](functions/cb_shutdown.md) | function | cb.h | 0.1 | Undo one cb_init. The last one frees what the library made (the decompiler and its caches). |
| [cb_status](cb_h.md#cb_status) | enum | cb.h | 0.1 | What a call did. CB_OK is 0; every other value is a failure, and cb_last_error() says what failed and what to do. |
| [cb_status_name](functions/cb_status_name.md) | function | cb.h | 0.1 | The name of a status ("CB_ERROR_NOT_FOUND"), for logs. |
| [cb_string](cb_h.md#cb_string) | type | cb.h | 0.1 | Text the library made for the caller: data is NUL-terminated UTF-8, size its length without the NUL. |
| [cb_string_free](functions/cb_string_free.md) | function | cb.h | 0.1 | Free a cb_string the library made, and set it to {NULL, 0}. |
| [cb_version](functions/cb_version.md) | function | cb.h | 0.1 | The version of the library that is loaded, as CB_SDK_MAKE_VERSION makes it. |
| [cb_version_string](functions/cb_version_string.md) | function | cb.h | 0.1 | The version of the library that is loaded, as text ("0.1.0"). |

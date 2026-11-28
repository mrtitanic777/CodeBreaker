/* cb/binary.h -- open any file CodeBreaker reads: its format, processor, sections, imports, exports and functions.
 *
 *     cb_binary* b;
 *     cb_binary_open("lz32.dll", &b);
 *     printf("%s on %s\n", cb_binary_format(b), cb_binary_arch(b));              PE on x86-32
 *     size_t n;
 *     const cb_section* s = cb_binary_sections(b, &n);                          the records point into the binary
 *     for (size_t i = 0; i < n; ++i) printf("%s\n", s[i].name);
 *     cb_binary_close(b);
 *
 * Executables of the MZ family (PE, NE, LE/LX and their W3 bundles, DOS programs) are read whole. Any other file CodeBreaker knows
 * (console ROMs and discs, Xbox containers, archives, images ...) is identified: its format is named, and the parts it does not have as
 * an executable are empty. A part of a file that is damaged is left empty; the rest is still read.
 *
 * Records (cb_section, cb_import, cb_export) and strings point into the binary: they are valid until cb_binary_close, and are never
 * freed by the caller. A binary may be read from several threads at once; cb_binary_function_at and the decompile calls change it,
 * so call those from one thread at a time per binary.
 */
#ifndef CB_BINARY_H
#define CB_BINARY_H

#include "cb/cb.h"

#ifdef __cplusplus
extern "C" {
#endif

/** cb_address_style: how a cb_address is to be read.
 * @since 0.1
 */
typedef enum cb_address_style {
    CB_ADDRESS_NONE = 0,       /* no address (an import, a part with no place in memory) */
    CB_ADDRESS_LINEAR = 1,     /* offset is a linear address (a PE's virtual address, an LE object's base); segment is 0 */
    CB_ADDRESS_NE_SEGMENT = 2, /* segment is a 1-based NE segment number, offset a 16-bit offset in it: "3:01a4" */
    CB_ADDRESS_REAL_MODE = 3,  /* segment:offset of real mode (a DOS program): "1234:0010" */
    CB_ADDRESS_LE_OBJECT = 4   /* segment is an LE object number, offset a 32-bit offset in it: "1:00001000" */
} cb_address_style;

/** cb_address: a place in a binary's address space.
 * @field offset the linear address, or the offset in the segment.
 * @field segment the segment, object or paragraph; 0 for a linear address.
 * @field style a cb_address_style.
 * @since 0.1
 */
typedef struct cb_address {
    uint64_t offset;
    uint32_t segment;
    uint32_t style;
} cb_address;

/** cb_address_format: an address as text, into a caller-owned buffer: "0x10001000", "3:01a4", "1234:0010", "1:00001000", "" for none.
 * @param address the address.
 * @param buffer where the text goes, NUL-terminated; may be NULL when size is 0.
 * @param size the buffer's size in bytes.
 * @param needed NULL, or receives the bytes the text needs with its NUL (also when the buffer is too small).
 * @return CB_OK; CB_ERROR_INVALID_ARGUMENT for a NULL address; CB_ERROR_BUFFER_TOO_SMALL when the text and its NUL do not fit (24 bytes
 *   always do).
 * @threads Any thread, at any time (cb_init is not needed).
 * @since 0.1
 */
CB_API cb_status cb_address_format(const cb_address* address, char* buffer, size_t size, size_t* needed);

/** cb_binary_open: open a file and read what it is.
 * @param path the file, UTF-8.
 * @param out receives the binary; close it with cb_binary_close. Left NULL when the call fails.
 * @return CB_OK, also for a file CodeBreaker does not recognise (its format is then ""); CB_ERROR_NOT_FOUND when there is no such file;
 *   CB_ERROR_INVALID_ARGUMENT for a NULL path or out, or a folder; CB_ERROR_NOT_INITIALIZED before cb_init.
 * @ownership The caller owns the binary.
 * @threads Any thread.
 * @since 0.1
 */
CB_API cb_status cb_binary_open(const char* path, cb_binary** out);

/** cb_binary_close: free a binary, its functions and every record and string it gave.
 * @param binary NULL (nothing happens), or an open binary.
 * @threads Any thread, when no other call is using the binary.
 * @since 0.1
 */
CB_API void cb_binary_close(cb_binary* binary);

/** cb_binary_path: the path the binary was opened from.
 * @return a string the binary owns; "" for a NULL binary.
 * @since 0.1
 */
CB_API const char* cb_binary_path(const cb_binary* binary);

/** cb_binary_format: what the file is: "PE", "NE", "LE", "LX", "W3", "MZ" for an executable, else the format line of what it was
 * identified as ("NES ROM", "Xbox 360 XEX" ...).
 * @return a string the binary owns; "" when the file is not one CodeBreaker knows, or for a NULL binary.
 * @since 0.1
 */
CB_API const char* cb_binary_format(const cb_binary* binary);

/** cb_binary_kind: the lane that reads the file ("pe", "ne", "le", "dos"), or the family of a file that is not an executable.
 * @return a string the binary owns; "" when not known.
 * @since 0.1
 */
CB_API const char* cb_binary_kind(const cb_binary* binary);

/** cb_binary_arch: the processor the file's code is for, by the name cb_disasm takes ("x86-32", "x86-16", "ppc", "a64" ...).
 * A processor CodeBreaker has no description of yet (an x64 PE: "x86-64") is named all the same; cb_disasm says it is not supported.
 * @return a string the binary owns; "" when the file says no processor.
 * @since 0.1
 */
CB_API const char* cb_binary_arch(const cb_binary* binary);

/** cb_binary_size: the file's size in bytes.
 * @return the size; 0 for a NULL binary.
 * @since 0.1
 */
CB_API uint64_t cb_binary_size(const cb_binary* binary);

/** cb_binary_image_base: where the executable asks to be loaded (a PE's image base); 0 for a format without one.
 * @return the base address.
 * @since 0.1
 */
CB_API uint64_t cb_binary_image_base(const cb_binary* binary);

/** cb_binary_entry: where the program starts.
 * @return the entry point; style CB_ADDRESS_NONE when it has none (a resource-only DLL, a file that is not a program).
 * @since 0.1
 */
CB_API cb_address cb_binary_entry(const cb_binary* binary);

/** cb_section: a section of a PE, a segment of an NE, an object of an LE, the program of a DOS file.
 * @field name its name (".text", "seg 1", "object 1" ...).
 * @field address where it is loaded.
 * @field size its size in memory.
 * @field file_offset where its bytes are in the file.
 * @field file_size how many of its bytes the file holds.
 * @field code 1 when it holds code.
 * @since 0.1
 */
typedef struct cb_section {
    const char* name;
    cb_address address;
    uint64_t size;
    uint64_t file_offset;
    uint64_t file_size;
    uint32_t code;
} cb_section;

/** cb_binary_sections: the binary's sections, in the order the file lists them.
 * @param binary the binary.
 * @param count receives how many there are (0 for a NULL binary).
 * @return the first of count records the binary owns; NULL when there are none.
 * @since 0.1
 */
CB_API const cb_section* cb_binary_sections(const cb_binary* binary, size_t* count);

/** cb_import: one function the binary imports.
 * @field module the module it comes from ("KERNEL32.dll").
 * @field name its name; NULL when it is imported by ordinal only.
 * @field ordinal its ordinal, when has_ordinal is 1.
 * @field has_ordinal 1 when it is imported by ordinal.
 * @field delay 1 when it comes from a PE's delay-load directory.
 * @since 0.1
 */
typedef struct cb_import {
    const char* module;
    const char* name;
    uint32_t ordinal;
    uint32_t has_ordinal;
    uint32_t delay;
} cb_import;

/** cb_binary_imports: every function the binary imports, module by module, in the order the file lists them.
 * @param binary the binary.
 * @param count receives how many there are.
 * @return the first of count records the binary owns; NULL when there are none.
 * @since 0.1
 */
CB_API const cb_import* cb_binary_imports(const cb_binary* binary, size_t* count);

/** cb_export: one function or value the binary exports.
 * @field name its name; NULL when it is exported by ordinal only.
 * @field ordinal its ordinal.
 * @field address where it is; style CB_ADDRESS_NONE for a forwarded export.
 * @field forward "OTHER.Function" for a PE export forwarded to another module; else NULL.
 * @since 0.1
 */
typedef struct cb_export {
    const char* name;
    uint64_t ordinal;
    cb_address address;
    const char* forward;
} cb_export;

/** cb_binary_exports: everything the binary exports, in the order of its export table.
 * @param binary the binary.
 * @param count receives how many there are.
 * @return the first of count records the binary owns; NULL when there are none.
 * @since 0.1
 */
CB_API const cb_export* cb_binary_exports(const cb_binary* binary, size_t* count);

/** cb_binary_function_count: how many functions the binary names: its entry point and the exports that are code, by address.
 * (Functions found by analysis join this list in a later version.)
 * @return the count; 0 for a NULL binary.
 * @since 0.1
 */
CB_API size_t cb_binary_function_count(const cb_binary* binary);

/** cb_binary_function: one of the binary's functions, in address order.
 * @param binary the binary.
 * @param index 0 to cb_binary_function_count() - 1.
 * @return the function (owned by the binary); NULL for an index out of range or a NULL binary.
 * @since 0.1
 */
CB_API const cb_function* cb_binary_function(const cb_binary* binary, size_t index);

/** cb_binary_function_find: the function with this name (an export's name, "entry"), compared exactly.
 * @param binary the binary.
 * @param name the name.
 * @param out receives the function (owned by the binary).
 * @return CB_OK; CB_ERROR_NOT_FOUND when the binary names no such function; CB_ERROR_INVALID_ARGUMENT for a NULL argument.
 * @since 0.1
 */
CB_API cb_status cb_binary_function_find(const cb_binary* binary, const char* name, const cb_function** out);

/** cb_binary_function_at: the function that starts at an address: one the binary names, else a new one named sub_<address>, added to
 * the binary (it is not counted by cb_binary_function_count).
 * @param binary the binary.
 * @param address where the function starts.
 * @param out receives the function (owned by the binary).
 * @return CB_OK; CB_ERROR_INVALID_ARGUMENT for a NULL argument or an address of style CB_ADDRESS_NONE.
 * @threads One thread at a time per binary.
 * @since 0.1
 */
CB_API cb_status cb_binary_function_at(cb_binary* binary, cb_address address, const cb_function** out);

/** cb_function_name: the function's name ("LZCopy", "entry", "sub_10001000").
 * @return a string the binary owns; "" for a NULL function.
 * @since 0.1
 */
CB_API const char* cb_function_name(const cb_function* function);

/** cb_function_address: where the function starts.
 * @return the address; style CB_ADDRESS_NONE for a NULL function.
 * @since 0.1
 */
CB_API cb_address cb_function_address(const cb_function* function);

/** cb_function_binary: the binary the function belongs to.
 * @return the binary; NULL for a NULL function.
 * @since 0.1
 */
CB_API cb_binary* cb_function_binary(const cb_function* function);

#ifdef __cplusplus
}
#endif

#endif

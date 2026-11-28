# binary.h

[The reference](README.md)

```
cb/binary.h -- open any file CodeBreaker reads: its format, processor, sections, imports, exports and functions.

    cb_binary* b;
    cb_binary_open("lz32.dll", &b);
    printf("%s on %s\n", cb_binary_format(b), cb_binary_arch(b));              PE on x86-32
    size_t n;
    const cb_section* s = cb_binary_sections(b, &n);                          the records point into the binary
    for (size_t i = 0; i < n; ++i) printf("%s\n", s[i].name);
    cb_binary_close(b);

Executables of the MZ family (PE, NE, LE/LX and their W3 bundles, DOS programs) are read whole. Any other file CodeBreaker knows
(console ROMs and discs, Xbox containers, archives, images ...) is identified: its format is named, and the parts it does not have as
an executable are empty. A part of a file that is damaged is left empty; the rest is still read.

Records (cb_section, cb_import, cb_export) and strings point into the binary: they are valid until cb_binary_close, and are never
freed by the caller. A binary may be read from several threads at once; cb_binary_function_at and the decompile calls change it,
so call those from one thread at a time per binary.
```

## cb_address_style

`binary.h` · enum · since 0.1

How a cb_address is to be read.

```c
typedef enum cb_address_style {
    CB_ADDRESS_NONE = 0,       /* no address (an import, a part with no place in memory) */
    CB_ADDRESS_LINEAR = 1,     /* offset is a linear address (a PE's virtual address, an LE object's base); segment is 0 */
    CB_ADDRESS_NE_SEGMENT = 2, /* segment is a 1-based NE segment number, offset a 16-bit offset in it: "3:01a4" */
    CB_ADDRESS_REAL_MODE = 3,  /* segment:offset of real mode (a DOS program): "1234:0010" */
    CB_ADDRESS_LE_OBJECT = 4   /* segment is an LE object number, offset a 32-bit offset in it: "1:00001000" */
} cb_address_style;
```

## cb_address

`binary.h` · type · since 0.1

A place in a binary's address space.

```c
typedef struct cb_address {
    uint64_t offset;
    uint32_t segment;
    uint32_t style;
} cb_address;
```

| Field | |
|---|---|
| `offset` | the linear address, or the offset in the segment. |
| `segment` | the segment, object or paragraph; 0 for a linear address. |
| `style` | a cb_address_style. |

## cb_address_format

`binary.h` · function · since 0.1

An address as text, into a caller-owned buffer: "0x10001000", "3:01a4", "1234:0010", "1:00001000", "" for none.

```c
cb_status cb_address_format(const cb_address* address, char* buffer, size_t size, size_t* needed);
```

| Parameter | |
|---|---|
| `address` | the address. |
| `buffer` | where the text goes, NUL-terminated; may be NULL when size is 0. |
| `size` | the buffer's size in bytes. |
| `needed` | NULL, or receives the bytes the text needs with its NUL (also when the buffer is too small). |

**Returns.** CB_OK; CB_ERROR_INVALID_ARGUMENT for a NULL address; CB_ERROR_BUFFER_TOO_SMALL when the text and its NUL do not fit (24 bytes always do).

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Samples.** [samples/HelloDecompile/main.cpp](../../../samples/HelloDecompile/main.cpp), [samples/framework/framework.c](../../../samples/framework/framework.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_address_format](functions/cb_address_format.md)

## cb_binary_open

`binary.h` · function · since 0.1

Open a file and read what it is.

```c
cb_status cb_binary_open(const char* path, cb_binary** out);
```

| Parameter | |
|---|---|
| `path` | the file, UTF-8. |
| `out` | receives the binary; close it with cb_binary_close. Left NULL when the call fails. |

**Returns.** CB_OK, also for a file CodeBreaker does not recognise (its format is then ""); CB_ERROR_NOT_FOUND when there is no such file; CB_ERROR_INVALID_ARGUMENT for a NULL path or out, or a folder; CB_ERROR_NOT_INITIALIZED before cb_init.

**Ownership.** The caller owns the binary.

**Thread safety.** Any thread.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_open](functions/cb_binary_open.md)

## cb_binary_close

`binary.h` · function · since 0.1

Free a binary, its functions and every record and string it gave.

```c
void cb_binary_close(cb_binary* binary);
```

| Parameter | |
|---|---|
| `binary` | NULL (nothing happens), or an open binary. |

**Thread safety.** Any thread, when no other call is using the binary.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_close](functions/cb_binary_close.md)

## cb_binary_path

`binary.h` · function · since 0.1

The path the binary was opened from.

```c
const char* cb_binary_path(const cb_binary* binary);
```

**Returns.** a string the binary owns; "" for a NULL binary.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_path](functions/cb_binary_path.md)

## cb_binary_format

`binary.h` · function · since 0.1

What the file is: "PE", "NE", "LE", "LX", "W3", "MZ" for an executable, else the format line of what it was

```c
const char* cb_binary_format(const cb_binary* binary);
```

identified as ("NES ROM", "Xbox 360 XEX" ...).

**Returns.** a string the binary owns; "" when the file is not one CodeBreaker knows, or for a NULL binary.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_format](functions/cb_binary_format.md)

## cb_binary_kind

`binary.h` · function · since 0.1

The lane that reads the file ("pe", "ne", "le", "dos"), or the family of a file that is not an executable.

```c
const char* cb_binary_kind(const cb_binary* binary);
```

**Returns.** a string the binary owns; "" when not known.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_kind](functions/cb_binary_kind.md)

## cb_binary_arch

`binary.h` · function · since 0.1

The processor the file's code is for, by the name cb_disasm takes ("x86-32", "x86-16", "ppc", "a64" ...).

```c
const char* cb_binary_arch(const cb_binary* binary);
```

A processor CodeBreaker has no description of yet (an x64 PE: "x86-64") is named all the same; cb_disasm says it is not supported.

**Returns.** a string the binary owns; "" when the file says no processor.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_arch](functions/cb_binary_arch.md)

## cb_binary_size

`binary.h` · function · since 0.1

The file's size in bytes.

```c
uint64_t cb_binary_size(const cb_binary* binary);
```

**Returns.** the size; 0 for a NULL binary.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_size](functions/cb_binary_size.md)

## cb_binary_image_base

`binary.h` · function · since 0.1

Where the executable asks to be loaded (a PE's image base); 0 for a format without one.

```c
uint64_t cb_binary_image_base(const cb_binary* binary);
```

**Returns.** the base address.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_image_base](functions/cb_binary_image_base.md)

## cb_binary_entry

`binary.h` · function · since 0.1

Where the program starts.

```c
cb_address cb_binary_entry(const cb_binary* binary);
```

**Returns.** the entry point; style CB_ADDRESS_NONE when it has none (a resource-only DLL, a file that is not a program).

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_entry](functions/cb_binary_entry.md)

## cb_section

`binary.h` · type · since 0.1

A section of a PE, a segment of an NE, an object of an LE, the program of a DOS file.

```c
typedef struct cb_section {
    const char* name;
    cb_address address;
    uint64_t size;
    uint64_t file_offset;
    uint64_t file_size;
    uint32_t code;
} cb_section;
```

| Field | |
|---|---|
| `name` | its name (".text", "seg 1", "object 1" ...). |
| `address` | where it is loaded. |
| `size` | its size in memory. |
| `file_offset` | where its bytes are in the file. |
| `file_size` | how many of its bytes the file holds. |
| `code` | 1 when it holds code. |

## cb_binary_sections

`binary.h` · function · since 0.1

The binary's sections, in the order the file lists them.

```c
const cb_section* cb_binary_sections(const cb_binary* binary, size_t* count);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `count` | receives how many there are (0 for a NULL binary). |

**Returns.** the first of count records the binary owns; NULL when there are none.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c), [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_sections](functions/cb_binary_sections.md)

## cb_import

`binary.h` · type · since 0.1

One function the binary imports.

```c
typedef struct cb_import {
    const char* module;
    const char* name;
    uint32_t ordinal;
    uint32_t has_ordinal;
    uint32_t delay;
} cb_import;
```

| Field | |
|---|---|
| `module` | the module it comes from ("KERNEL32.dll"). |
| `name` | its name; NULL when it is imported by ordinal only. |
| `ordinal` | its ordinal, when has_ordinal is 1. |
| `has_ordinal` | 1 when it is imported by ordinal. |
| `delay` | 1 when it comes from a PE's delay-load directory. |

## cb_binary_imports

`binary.h` · function · since 0.1

Every function the binary imports, module by module, in the order the file lists them.

```c
const cb_import* cb_binary_imports(const cb_binary* binary, size_t* count);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `count` | receives how many there are. |

**Returns.** the first of count records the binary owns; NULL when there are none.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_imports](functions/cb_binary_imports.md)

## cb_export

`binary.h` · type · since 0.1

One function or value the binary exports.

```c
typedef struct cb_export {
    const char* name;
    uint64_t ordinal;
    cb_address address;
    const char* forward;
} cb_export;
```

| Field | |
|---|---|
| `name` | its name; NULL when it is exported by ordinal only. |
| `ordinal` | its ordinal. |
| `address` | where it is; style CB_ADDRESS_NONE for a forwarded export. |
| `forward` | "OTHER.Function" for a PE export forwarded to another module; else NULL. |

## cb_binary_exports

`binary.h` · function · since 0.1

Everything the binary exports, in the order of its export table.

```c
const cb_export* cb_binary_exports(const cb_binary* binary, size_t* count);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `count` | receives how many there are. |

**Returns.** the first of count records the binary owns; NULL when there are none.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_exports](functions/cb_binary_exports.md)

## cb_binary_function_count

`binary.h` · function · since 0.1

How many functions the binary names: its entry point and the exports that are code, by address.

```c
size_t cb_binary_function_count(const cb_binary* binary);
```

(Functions found by analysis join this list in a later version.)

**Returns.** the count; 0 for a NULL binary.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_function_count](functions/cb_binary_function_count.md)

## cb_binary_function

`binary.h` · function · since 0.1

One of the binary's functions, in address order.

```c
const cb_function* cb_binary_function(const cb_binary* binary, size_t index);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `index` | 0 to cb_binary_function_count() - 1. |

**Returns.** the function (owned by the binary); NULL for an index out of range or a NULL binary.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_function](functions/cb_binary_function.md)

## cb_binary_function_find

`binary.h` · function · since 0.1

The function with this name (an export's name, "entry"), compared exactly.

```c
cb_status cb_binary_function_find(const cb_binary* binary, const char* name, const cb_function** out);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `name` | the name. |
| `out` | receives the function (owned by the binary). |

**Returns.** CB_OK; CB_ERROR_NOT_FOUND when the binary names no such function; CB_ERROR_INVALID_ARGUMENT for a NULL argument.

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_function_find](functions/cb_binary_function_find.md)

## cb_binary_function_at

`binary.h` · function · since 0.1

The function that starts at an address: one the binary names, else a new one named sub_<address>, added to

```c
cb_status cb_binary_function_at(cb_binary* binary, cb_address address, const cb_function** out);
```

the binary (it is not counted by cb_binary_function_count).

| Parameter | |
|---|---|
| `binary` | the binary. |
| `address` | where the function starts. |
| `out` | receives the function (owned by the binary). |

**Returns.** CB_OK; CB_ERROR_INVALID_ARGUMENT for a NULL argument or an address of style CB_ADDRESS_NONE.

**Thread safety.** One thread at a time per binary.

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_binary_function_at](functions/cb_binary_function_at.md)

## cb_function_name

`binary.h` · function · since 0.1

The function's name ("LZCopy", "entry", "sub_10001000").

```c
const char* cb_function_name(const cb_function* function);
```

**Returns.** a string the binary owns; "" for a NULL function.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_function_name](functions/cb_function_name.md)

## cb_function_address

`binary.h` · function · since 0.1

Where the function starts.

```c
cb_address cb_function_address(const cb_function* function);
```

**Returns.** the address; style CB_ADDRESS_NONE for a NULL function.

**Samples.** [samples/HelloBinary/main.c](../../../samples/HelloBinary/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_function_address](functions/cb_function_address.md)

## cb_function_binary

`binary.h` · function · since 0.1

The binary the function belongs to.

```c
cb_binary* cb_function_binary(const cb_function* function);
```

**Returns.** the binary; NULL for a NULL function.

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_function_binary](functions/cb_function_binary.md)


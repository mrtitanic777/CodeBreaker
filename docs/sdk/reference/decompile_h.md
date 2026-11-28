# decompile.h

[The reference](README.md)

```
cb/decompile.h -- a function or a whole module as C.

    const cb_function* f;
    cb_binary_function_find(binary, "LZCopy", &f);
    cb_string c = {0};
    if (cb_function_decompile(f, NULL, &c) == CB_OK) puts(c.data);
    cb_string_free(&c);

One decompiler for every format: the readers take the file apart, the native decompiler (decompiler.exe, beside cbsdk.dll) lifts
each function from its processor's SLEIGH description, and the C is written from what it found. What it makes is kept in the state
folder (cb_init), keyed on everything it depends on, so asking again is fast.

A 32-bit x86 function is decompiled by itself. A function of any other kind of module is cut out of its whole module's C, which is
made once per binary and kept with it (cb_module_decompile gives all of it).

The decompile calls are serialized: one runs at a time in the process (the native decompiler already uses every core).
```

## cb_decompile_options

`decompile.h` · type · since 0.1

How a decompile call works. Make one with CB_DECOMPILE_OPTIONS_INIT.

```c
typedef struct cb_decompile_options {
    uint32_t struct_size;
    const char* language;
} cb_decompile_options;
```

| Field | |
|---|---|
| `struct_size` | sizeof(cb_decompile_options). |
| `language` | the language to write: NULL or "c" (the only one in this version). |

## CB_DECOMPILE_OPTIONS_INIT

`decompile.h` · macro · since 0.1

The defaults of cb_decompile_options.

```c
#define CB_DECOMPILE_OPTIONS_INIT {sizeof(cb_decompile_options), NULL}
```

## cb_module_info

`decompile.h` · type · since 0.1

What cb_module_decompile made.

```c
typedef struct cb_module_info {
    size_t decompiled;
    size_t found;
    size_t library;
} cb_module_info;
```

| Field | |
|---|---|
| `decompiled` | the functions written as C. |
| `found` | the functions found. |
| `library` | of found, those identified as the compiler's library and left out of the text. |

## cb_decompiler_check

`decompile.h` · function · since 0.1

Whether the decompiler can run: its program and its processor descriptions are where the data root says.

```c
cb_status cb_decompiler_check(void);
```

**Returns.** CB_OK; CB_ERROR_NOT_FOUND naming what is missing; CB_ERROR_NOT_INITIALIZED before cb_init.

**Thread safety.** Any thread.

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_decompiler_check](functions/cb_decompiler_check.md)

## cb_function_decompile

`decompile.h` · function · since 0.1

One function as C.

```c
cb_status cb_function_decompile(const cb_function* function, const cb_decompile_options* options, cb_string* out);
```

| Parameter | |
|---|---|
| `function` | the function (cb_binary_function, cb_binary_function_find, cb_binary_function_at). |
| `options` | NULL for the defaults. |
| `out` | receives the C: the function with the comment lines above it. Free it with cb_string_free. |

**Returns.** CB_OK; CB_ERROR_NOT_FOUND when the decompiler made no function at that address, or cannot run (cb_decompiler_check); CB_ERROR_UNSUPPORTED for a file that is not an executable this version decompiles, or a language other than "c"; CB_ERROR_TOOL when the native decompiler failed; CB_ERROR_INVALID_ARGUMENT for a NULL argument; CB_ERROR_NOT_INITIALIZED before cb_init.

**Ownership.** The caller owns the string.

**Thread safety.** One decompile call runs at a time in the process; others wait.

**Samples.** [samples/HelloDecompile/main.cpp](../../../samples/HelloDecompile/main.cpp)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_function_decompile](functions/cb_function_decompile.md)

## cb_module_decompile

`decompile.h` · function · since 0.1

Every function of a binary as the text of one .c file.

```c
cb_status cb_module_decompile(cb_binary* binary, const cb_decompile_options* options, cb_string* out, cb_module_info* info);
```

| Parameter | |
|---|---|
| `binary` | the binary. |
| `options` | NULL for the defaults. |
| `out` | receives the C. Free it with cb_string_free. |
| `info` | NULL, or receives how many functions were found and decompiled. |

**Returns.** CB_OK; CB_ERROR_UNSUPPORTED for a file that is not an executable this version decompiles, or a language other than "c"; CB_ERROR_NOT_FOUND when the decompiler cannot run (cb_decompiler_check); CB_ERROR_TOOL when the native decompiler failed; CB_ERROR_INVALID_ARGUMENT for a NULL argument; CB_ERROR_NOT_INITIALIZED before cb_init.

**Ownership.** The caller owns the string. The binary keeps its own copy for cb_function_decompile.

**Thread safety.** One decompile call runs at a time in the process; others wait.

**Samples.** [samples/HelloDecompile/main.cpp](../../../samples/HelloDecompile/main.cpp)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_module_decompile](functions/cb_module_decompile.md)


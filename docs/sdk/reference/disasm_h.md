# disasm.h

[The reference](README.md)

```
cb/disasm.h -- the instructions of any processor CodeBreaker has a description of.

    const uint8_t code[] = {0x3E, 0x12, 0xC9};
    cb_instructions* list;
    cb_disasm("z80", code, sizeof code, 0x0000, 0, &list);
    size_t n;
    const cb_instruction* ins = cb_instructions_data(list, &n);
    for (size_t i = 0; i < n; ++i) printf("%04llx  %s %s\n", ins[i].address, ins[i].mnemonic, ins[i].operands);   LD A,0x12 / RET
    cb_instructions_free(list);

One disassembler for every processor: a processor is a SLEIGH description (the same one the decompiler lifts and the emulator runs),
so every processor reads the same way and says the same things of each instruction: its text, its length, how control leaves it and
where it goes. cb_arch_list names them all.
```

## cb_arch

`disasm.h` · type · since 0.1

One processor CodeBreaker knows.

```c
typedef struct cb_arch {
    const char* name;
    const char* aliases;
    const char* summary;
    uint32_t big_endian;
    uint32_t described;
} cb_arch;
```

| Field | |
|---|---|
| `name` | the name cb_disasm takes ("z80", "x86-32", "m68k" ...). |
| `aliases` | the other names it is known by, separated by spaces ("gbz80 ..."); "" when none. |
| `summary` | what it is and where it runs. |
| `big_endian` | 1 when its words are big-endian. |
| `described` | 1 when it has a SLEIGH description, so cb_disasm reads it; 0 when CodeBreaker knows it only by other means. |

## cb_arch_list

`disasm.h` · function · since 0.1

Every processor CodeBreaker knows, in a fixed order.

```c
const cb_arch* cb_arch_list(size_t* count);
```

| Parameter | |
|---|---|
| `count` | receives how many there are. |

**Returns.** the first of count records the library owns, valid while it is loaded.

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_arch_list](functions/cb_arch_list.md)

## cb_arch_find

`disasm.h` · function · since 0.1

The processor a name or one of its aliases names, compared without case ("I386" is "x86-32").

```c
cb_status cb_arch_find(const char* name, const cb_arch** out);
```

| Parameter | |
|---|---|
| `name` | the name. |
| `out` | receives the processor's record (owned by the library). |

**Returns.** CB_OK; CB_ERROR_NOT_FOUND when no processor has that name (the message lists the names); CB_ERROR_INVALID_ARGUMENT for a NULL argument.

**Thread safety.** Any thread, at any time (cb_init is not needed).

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_arch_find](functions/cb_arch_find.md)

## cb_flow

`disasm.h` · enum · since 0.1

How control leaves an instruction, as its description says.

```c
typedef enum cb_flow {
    CB_FLOW_NEXT = 0,          /* falls through */
    CB_FLOW_BRANCH = 1,        /* may go to target, or fall through */
    CB_FLOW_JUMP = 2,          /* goes to target only */
    CB_FLOW_CALL = 3,          /* calls target, then falls through */
    CB_FLOW_CALL_INDIRECT = 4, /* calls through a register or memory, then falls through */
    CB_FLOW_RETURN = 5,        /* returns */
    CB_FLOW_INDIRECT = 6,      /* goes through a register or memory */
    CB_FLOW_INVALID = 7        /* the bytes are no instruction of the processor's */
} cb_flow;
```

## cb_instruction

`disasm.h` · type · since 0.1

One instruction.

```c
typedef struct cb_instruction {
    uint64_t address;
    uint32_t length;
    uint32_t flow;
    const char* mnemonic;
    const char* operands;
    uint64_t target;
    uint32_t has_target;
} cb_instruction;
```

| Field | |
|---|---|
| `address` | its address (a word address for a word-addressed processor: the CP1610). |
| `length` | how many bytes it takes; for CB_FLOW_INVALID, how many bytes were stepped over (the processor's alignment). |
| `flow` | a cb_flow. |
| `mnemonic` | its mnemonic as the description writes it ("LD", "mov"); "" for CB_FLOW_INVALID. |
| `operands` | its operands ("A,0x12"); "" when it has none. |
| `target` | where a branch, jump or call goes, when has_target is 1. |
| `has_target` | 1 when the instruction goes to a constant address. |

## cb_disasm

`disasm.h` · function · since 0.1

Read instructions from bytes.

```c
cb_status cb_disasm(const char* arch, const void* code, size_t size, uint64_t address, size_t count, cb_instructions** out);
```

| Parameter | |
|---|---|
| `arch` | a processor's name or alias (cb_arch_list). |
| `code` | the bytes; may be NULL when size is 0. |
| `size` | how many bytes there are. |
| `address` | the address of the first byte. |
| `count` | how many instructions to read at most; 0: until the bytes end. |
| `out` | receives the instructions; free them with cb_instructions_free. Left NULL when the call fails. |

**Returns.** CB_OK; CB_ERROR_NOT_FOUND for a name no processor has; CB_ERROR_UNSUPPORTED for a processor with no description; CB_ERROR_IO when its description cannot be read (the data root is not complete); CB_ERROR_INVALID_ARGUMENT for a NULL argument; CB_ERROR_NOT_INITIALIZED before cb_init.

**Ownership.** The caller owns the instructions; they do not point into code, which may be freed at once.

**Thread safety.** Any thread; calls for the same processor from several threads run side by side.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_disasm](functions/cb_disasm.md)

## cb_instructions_data

`disasm.h` · function · since 0.1

The instructions one cb_disasm call read, in address order.

```c
const cb_instruction* cb_instructions_data(const cb_instructions* instructions, size_t* count);
```

| Parameter | |
|---|---|
| `instructions` | the list. |
| `count` | receives how many there are. |

**Returns.** the first of count records the list owns, valid until cb_instructions_free; NULL when there are none.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_instructions_data](functions/cb_instructions_data.md)

## cb_instructions_free

`disasm.h` · function · since 0.1

Free a list of instructions and every string in it.

```c
void cb_instructions_free(cb_instructions* instructions);
```

| Parameter | |
|---|---|
| `instructions` | NULL (nothing happens), or a list cb_disasm made. |

**Thread safety.** Any thread.

**Samples.** [samples/DisasmAnyCpu/main.c](../../../samples/DisasmAnyCpu/main.c)

**Tests.** [src/sdk/sdk_test.cpp](../../../src/sdk/sdk_test.cpp)

[The page of cb_instructions_free](functions/cb_instructions_free.md)


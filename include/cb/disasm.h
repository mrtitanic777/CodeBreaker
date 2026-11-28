/* cb/disasm.h -- the instructions of any processor CodeBreaker has a description of.
 *
 *     const uint8_t code[] = {0x3E, 0x12, 0xC9};
 *     cb_instructions* list;
 *     cb_disasm("z80", code, sizeof code, 0x0000, 0, &list);
 *     size_t n;
 *     const cb_instruction* ins = cb_instructions_data(list, &n);
 *     for (size_t i = 0; i < n; ++i) printf("%04llx  %s %s\n", ins[i].address, ins[i].mnemonic, ins[i].operands);   LD A,0x12 / RET
 *     cb_instructions_free(list);
 *
 * One disassembler for every processor: a processor is a SLEIGH description (the same one the decompiler lifts and the emulator runs),
 * so every processor reads the same way and says the same things of each instruction: its text, its length, how control leaves it and
 * where it goes. cb_arch_list names them all.
 */
#ifndef CB_DISASM_H
#define CB_DISASM_H

#include "cb/cb.h"

#ifdef __cplusplus
extern "C" {
#endif

/** cb_arch: one processor CodeBreaker knows.
 * @field name the name cb_disasm takes ("z80", "x86-32", "m68k" ...).
 * @field aliases the other names it is known by, separated by spaces ("gbz80 ..."); "" when none.
 * @field summary what it is and where it runs.
 * @field big_endian 1 when its words are big-endian.
 * @field described 1 when it has a SLEIGH description, so cb_disasm reads it; 0 when CodeBreaker knows it only by other means.
 * @since 0.1
 */
typedef struct cb_arch {
    const char* name;
    const char* aliases;
    const char* summary;
    uint32_t big_endian;
    uint32_t described;
} cb_arch;

/** cb_arch_list: every processor CodeBreaker knows, in a fixed order.
 * @param count receives how many there are.
 * @return the first of count records the library owns, valid while it is loaded.
 * @threads Any thread, at any time (cb_init is not needed).
 * @since 0.1
 */
CB_API const cb_arch* cb_arch_list(size_t* count);

/** cb_arch_find: the processor a name or one of its aliases names, compared without case ("I386" is "x86-32").
 * @param name the name.
 * @param out receives the processor's record (owned by the library).
 * @return CB_OK; CB_ERROR_NOT_FOUND when no processor has that name (the message lists the names); CB_ERROR_INVALID_ARGUMENT for a NULL
 *   argument.
 * @threads Any thread, at any time (cb_init is not needed).
 * @since 0.1
 */
CB_API cb_status cb_arch_find(const char* name, const cb_arch** out);

/** cb_flow: how control leaves an instruction, as its description says.
 * @since 0.1
 */
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

/** cb_instruction: one instruction.
 * @field address its address (a word address for a word-addressed processor: the CP1610).
 * @field length how many bytes it takes; for CB_FLOW_INVALID, how many bytes were stepped over (the processor's alignment).
 * @field flow a cb_flow.
 * @field mnemonic its mnemonic as the description writes it ("LD", "mov"); "" for CB_FLOW_INVALID.
 * @field operands its operands ("A,0x12"); "" when it has none.
 * @field target where a branch, jump or call goes, when has_target is 1.
 * @field has_target 1 when the instruction goes to a constant address.
 * @since 0.1
 */
typedef struct cb_instruction {
    uint64_t address;
    uint32_t length;
    uint32_t flow;
    const char* mnemonic;
    const char* operands;
    uint64_t target;
    uint32_t has_target;
} cb_instruction;

/** cb_disasm: read instructions from bytes.
 * @param arch a processor's name or alias (cb_arch_list).
 * @param code the bytes; may be NULL when size is 0.
 * @param size how many bytes there are.
 * @param address the address of the first byte.
 * @param count how many instructions to read at most; 0: until the bytes end.
 * @param out receives the instructions; free them with cb_instructions_free. Left NULL when the call fails.
 * @return CB_OK; CB_ERROR_NOT_FOUND for a name no processor has; CB_ERROR_UNSUPPORTED for a processor with no description; CB_ERROR_IO
 *   when its description cannot be read (the data root is not complete); CB_ERROR_INVALID_ARGUMENT for a NULL argument;
 *   CB_ERROR_NOT_INITIALIZED before cb_init.
 * @ownership The caller owns the instructions; they do not point into code, which may be freed at once.
 * @threads Any thread; calls for the same processor from several threads run side by side.
 * @since 0.1
 */
CB_API cb_status cb_disasm(const char* arch, const void* code, size_t size, uint64_t address, size_t count, cb_instructions** out);

/** cb_instructions_data: the instructions one cb_disasm call read, in address order.
 * @param instructions the list.
 * @param count receives how many there are.
 * @return the first of count records the list owns, valid until cb_instructions_free; NULL when there are none.
 * @since 0.1
 */
CB_API const cb_instruction* cb_instructions_data(const cb_instructions* instructions, size_t* count);

/** cb_instructions_free: free a list of instructions and every string in it.
 * @param instructions NULL (nothing happens), or a list cb_disasm made.
 * @threads Any thread.
 * @since 0.1
 */
CB_API void cb_instructions_free(cb_instructions* instructions);

#ifdef __cplusplus
}
#endif

#endif

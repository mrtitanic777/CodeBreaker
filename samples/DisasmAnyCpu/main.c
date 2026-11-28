/* DisasmAnyCpu -- one disassembler for every processor: the same call reads Z80, 6502, x86, 68000, ARM, PowerPC and MIPS code, each from
 * its SLEIGH description, and says the same things of every instruction (text, length, how control leaves it, where it goes).
 * Plain C, on disasm.h (and binary.h for --binary).
 *
 *     DisasmAnyCpu                          a tour: a few instructions of each of several processors
 *     DisasmAnyCpu z80 3E12CD3412C9 0x100   bytes in hex for a processor, at an address
 *     DisasmAnyCpu --binary lz32.dll 12     a file's entry point, in the processor the file is for
 *     DisasmAnyCpu --list                   every processor CodeBreaker knows
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cb/binary.h"
#include "cb/cb.h"
#include "cb/disasm.h"
#include "framework.h"

static const char* const flow_names[] = {"", "branch", "jump", "call", "call indirect", "return", "indirect", "not an instruction"};

static void print_instructions(const char* arch, const unsigned char* code, size_t size, unsigned long long address, size_t count) {
    cb_instructions* list = NULL;
    const cb_instruction* ins;
    size_t n, i, k;
    double t = fw_seconds();
    fw_check(cb_disasm(arch, code, size, address, count, &list), "cb_disasm");
    fw_timed("cb_disasm", t);
    ins = cb_instructions_data(list, &n);
    for (i = 0; i < n; ++i) {
        char bytes[3 * 8 + 1] = "", line[256];
        size_t end;
        const size_t at = (size_t)(ins[i].address - address);
        for (k = 0; k < ins[i].length && k < 8 && at + k < size; ++k) snprintf(bytes + 3 * k, 4, "%02X ", code[at + k]);
        end = (size_t)snprintf(line, sizeof line, "  %08llx  %-24s %-8s %-28s %s", (unsigned long long)ins[i].address, bytes, ins[i].mnemonic,
                               ins[i].operands, flow_names[ins[i].flow]);
        if (end >= sizeof line) end = sizeof line - 1;
        while (end > 0 && line[end - 1] == ' ') line[--end] = '\0';   /* (no trailing blanks: the gate compares the text) */
        printf("%s", line);
        if (ins[i].has_target) printf(" -> %llx", (unsigned long long)ins[i].target);
        printf("\n");
    }
    cb_instructions_free(list);
}

/* "3E 12 c9" -> bytes; the count, or 0 when the text is not hex. */
static size_t parse_hex(const char* text, unsigned char* out, size_t room) {
    size_t n = 0;
    int high = -1;
    for (; *text != '\0'; ++text) {
        const char c = *text;
        int d;
        if (c == ' ' || c == ',') continue;
        d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
        if (d < 0) return 0;
        if (high < 0) high = d;
        else {
            if (n == room) return 0;
            out[n++] = (unsigned char)(high * 16 + d);
            high = -1;
        }
    }
    return high < 0 ? n : 0;
}

static void tour(void) {
    static const struct {
        const char* arch;
        const char* hex;
        unsigned long long address;
        const char* what;
    } steps[] = {
        {"z80", "3E12 CD3412 C9", 0x100, "Z80: LD, CALL, RET"},
        {"6502", "A912 8D0002 60", 0x8000, "6502: LDA, STA, RTS"},
        {"x86-16", "B8004C CD21", 0x100, "x86, 16-bit: DOS exit"},
        {"x86-32", "55 8BEC 83EC08 E805000000 C9 C3", 0x401000, "x86, 32-bit: a frame, a call, return"},
        {"m68k", "7001 4EB900002000 4E75", 0x1000, "68000: MOVEQ, JSR, RTS"},
        {"arm", "0100A0E3 010000EB 1EFF2FE1", 0x8000, "ARM: MOV, BL, BX LR"},
        {"ppc", "38600001 48000011 4E800020", 0x82000000, "PowerPC: LI, BL, BLR"},
        {"mips", "01000224 0800E003 00000000", 0x80000000, "MIPS: ADDIU, JR RA and its delay slot"},
    };
    size_t i;
    for (i = 0; i < sizeof steps / sizeof steps[0]; ++i) {
        unsigned char code[64];
        const size_t n = parse_hex(steps[i].hex, code, sizeof code);
        printf("%s (%s)\n", steps[i].what, steps[i].arch);
        print_instructions(steps[i].arch, code, n, steps[i].address, 0);
        printf("\n");
    }
}

static void list_processors(void) {
    size_t n, i;
    const cb_arch* archs = cb_arch_list(&n);
    for (i = 0; i < n; ++i) printf("%-10s %s  %s\n", archs[i].name, archs[i].described ? "described" : "         ", archs[i].summary);
}

/* The bytes at a file's entry point, read through the section that holds it, disassembled for the file's processor. */
static void entry_of(const char* path, size_t count) {
    cb_binary* binary = NULL;
    const cb_section* sections;
    size_t n, i;
    cb_address entry;
    char text[32];
    fw_check(cb_binary_open(path, &binary), "cb_binary_open");
    entry = cb_binary_entry(binary);
    sections = cb_binary_sections(binary, &n);
    for (i = 0; i < n; ++i) {
        const cb_section* s = &sections[i];
        if (s->address.style != entry.style || s->address.segment != entry.segment) continue;
        if (entry.offset < s->address.offset || entry.offset >= s->address.offset + s->file_size) continue;
        {
            unsigned char code[256];
            const unsigned long long skip = entry.offset - s->address.offset;
            const size_t want = (size_t)(s->file_size - skip < sizeof code ? s->file_size - skip : sizeof code);
            size_t got;
            FILE* f = fopen(path, "rb");
            if (f == NULL || fseek(f, (long)(s->file_offset + skip), SEEK_SET) != 0) {
                fprintf(stderr, "DisasmAnyCpu: %s cannot be read\n", path);
                exit(1);
            }
            got = fread(code, 1, want, f);
            fclose(f);
            printf("%s: %s, %s, entry %s in %s\n", fw_file_name(path), cb_binary_format(binary), cb_binary_arch(binary), fw_address(entry, text), s->name);
            print_instructions(cb_binary_arch(binary), code, got, entry.offset, count);
        }
        cb_binary_close(binary);
        return;
    }
    fprintf(stderr, "DisasmAnyCpu: %s has no entry point in a section of the file\n", path);
    cb_binary_close(binary);
    exit(1);
}

int main(int argc, char** argv) {
    static const char* const switches[] = {"--list", "--binary", NULL};
    fw_args args;
    fw_parse(argc, argv, "DisasmAnyCpu [<arch> <hex bytes> [address] | --binary <file> [count] | --list]", 0, switches, &args);
    fw_start(&args);
    if (args.flags[0]) list_processors();
    else if (args.flags[1]) {
        if (args.count < 1) {
            fprintf(stderr, "DisasmAnyCpu: --binary takes a file\n");
            return 2;
        }
        entry_of(args.values[0], args.count > 1 ? (size_t)strtoul(args.values[1], NULL, 0) : 16);
    } else if (args.count >= 2) {
        unsigned char code[256];
        const size_t n = parse_hex(args.values[1], code, sizeof code);
        if (n == 0) {
            fprintf(stderr, "DisasmAnyCpu: %s is not bytes in hex\n", args.values[1]);
            return 2;
        }
        print_instructions(args.values[0], code, n, args.count > 2 ? strtoull(args.values[2], NULL, 0) : 0, 0);
    } else tour();
    fw_stop();
    return 0;
}

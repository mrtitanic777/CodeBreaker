/* HelloBinary -- open a file with the CodeBreaker SDK and print what it is: its format, processor, sections, imports, exports and the
 * functions it names. Plain C, on the C interface (binary.h).
 *
 *     HelloBinary lz32.dll
 */
#include <stdio.h>
#include <string.h>

#include "cb/binary.h"
#include "cb/cb.h"
#include "framework.h"

int main(int argc, char** argv) {
    fw_args args;
    cb_binary* binary = NULL;
    const cb_section* sections;
    const cb_import* imports;
    const cb_export* exports;
    size_t count, i, modules = 0;
    char address[32];
    double t;

    fw_parse(argc, argv, "HelloBinary <file>", 1, NULL, &args);
    fw_start(&args);

    t = fw_seconds();
    fw_check(cb_binary_open(args.values[0], &binary), "cb_binary_open");
    fw_timed("cb_binary_open", t);

    printf("file      %s\n", fw_file_name(cb_binary_path(binary)));
    printf("format    %s (%s)\n", cb_binary_format(binary), cb_binary_kind(binary));
    printf("arch      %s\n", cb_binary_arch(binary)[0] ? cb_binary_arch(binary) : "(none)");
    printf("size      %llu bytes\n", (unsigned long long)cb_binary_size(binary));
    printf("base      0x%llx\n", (unsigned long long)cb_binary_image_base(binary));
    printf("entry     %s\n", cb_binary_entry(binary).style != CB_ADDRESS_NONE ? fw_address(cb_binary_entry(binary), address) : "(none)");

    sections = cb_binary_sections(binary, &count);
    printf("\nsections (%zu)\n", count);
    for (i = 0; i < count; ++i)
        printf("  %-10s %-14s size %8llu  file %8llu at 0x%06llx%s\n", sections[i].name, fw_address(sections[i].address, address),
               (unsigned long long)sections[i].size, (unsigned long long)sections[i].file_size, (unsigned long long)sections[i].file_offset,
               sections[i].code ? "  code" : "");

    imports = cb_binary_imports(binary, &count);
    for (i = 0; i < count; ++i) modules += i == 0 || strcmp(imports[i].module, imports[i - 1].module) != 0;
    printf("\nimports (%zu from %zu modules)\n", count, modules);
    for (i = 0; i < count; ++i) {
        if (imports[i].name != NULL) printf("  %-14s %s\n", imports[i].module, imports[i].name);
        else printf("  %-14s #%u\n", imports[i].module, imports[i].ordinal);
    }

    exports = cb_binary_exports(binary, &count);
    printf("\nexports (%zu)\n", count);
    for (i = 0; i < count; ++i)
        printf("  %4llu  %-14s %s\n", (unsigned long long)exports[i].ordinal, exports[i].forward ? exports[i].forward : fw_address(exports[i].address, address),
               exports[i].name ? exports[i].name : "(by ordinal)");

    count = cb_binary_function_count(binary);
    printf("\nfunctions (%zu)\n", count);
    for (i = 0; i < count; ++i) {
        const cb_function* f = cb_binary_function(binary, i);
        printf("  %-14s %s\n", fw_address(cb_function_address(f), address), cb_function_name(f));
    }

    cb_binary_close(binary);
    fw_stop();
    return 0;
}

/* x64_runtime.c -- the source of x64_runtime.obj, a two-function x64 object the runtime-library tests learn from.
   Built with Visual C++'s x64 compiler: cl /c /O1 /GS- /Zl x64_runtime.c */

extern long long cb_table[16];
extern long long cb_hook(long long v);

/* a global read RIP-relative and a call: two REL32 fields the linker fills in */
long long __cb_scaled_lookup(int i, long long scale)
{
    long long v = cb_table[i & 15] * scale;
    if (v > 1000)
        v = cb_hook(v) - 7;
    return v ^ (scale << 3);
}

/* no relocations: the same bytes wherever it is linked */
unsigned cb_mix(unsigned a, unsigned b)
{
    unsigned h = a * 0x9E3779B1u;
    h ^= b + 0x7F4A7C15u + (h << 6) + (h >> 2);
    return h * 0x85EBCA77u;
}

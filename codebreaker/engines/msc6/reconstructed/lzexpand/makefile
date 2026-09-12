#
# makefile for LZEXPAND.DLL   (16-bit Windows NE, multi-segment)
#
# Reconstructed the way the retail DLL was built: one hand-written .c per code
# segment.  Medium model (/AM) gives each module its own <module>_TEXT segment;
# /NOPACKCODE keeps the linker from folding them back into one, so the NE carries
# the same 11 code segments + 1 data segment as the shipped binary.
#
# Toolchain: Microsoft C 8.00c (Visual C++ 1.52c) -- cl / LINK 5.60 / rc, with
# the medium-model DLL C runtime (MDLLCEW.LIB) and the Windows imports (LIBW.LIB).
# The object list is passed through the response file lz.lnk because the Phar Lap
# hosted linker limits the command-line length.
#
CC     = cl
LINK   = link
RC     = rc

CFLAGS = /c /AM /G2 /Gsw /Ox

MODULES = lzdata.c lzinit.c lzio.c lzhdr.c lzlzss.c lzdecode.c \
          lzname.c lzctx.c lzcopy.c lzfile.c lzwep.c

all: lzexpand.dll

# each module -> its own <module>_TEXT code segment
objs:
    $(CC) $(CFLAGS) $(MODULES)

lzexpand.dll: objs lz.lnk lzexpand.def
    $(LINK) @lz.lnk
    $(RC) lzexpand.dll

clean:
    -del *.obj
    -del lzexpand.dll
    -del lzexpand.map

Real resources for the tests of cb/formats/resources (cb::formats::resources).

*.bin             one resource each, cut out of Windows system files by a script over `pefile`:
                  comctl32.dll, shell32.dll, wmsui32.dll (32-bit: UTF-16 text, aligned fields). The dialogs, menus
                  (plain and extended), string blocks, accelerators and version blocks are the smallest of their kind in
                  those modules (some in Korean / Chinese for the non-ASCII text).
golden.json       what the Python reader (codebreaker/formats/resources.py, the behaviour this module replaces) decoded
                  each one to: the `decoded` value is the old dict/list shape (`class`, `controls`, `font` as a list ...).
                  The tests render the new typed values into the same shape and compare. Reviewed by reading: the
                  version blocks carry the modules' real names ("Microsoft Corporation", ...), the dialog controls their
                  real classes.
SETUP.EXE         the Windows 95 Setup stub, a 5184-byte 16-bit (NE) module: one icon group, its icon, a version block.
SETUP.golden.json what the old reader gave for it (read / decode / the icon file's size and CRC).
comctl32_bitmap_133.bin, comctl32_groupcursor_102.bin, comctl32_cursor_1.bin, media.golden.json
                  a bitmap and a cursor group with its member, and the size and CRC-32 of the .bmp / .cur file the old
                  reader made of them.
stdole2_typelib.bin / stdole32_typelib.bin
                  the TYPELIB resources of stdole2.tlb (MSFT layout) and stdole32.tlb (the older SLTG layout), with the
                  old reader's description of each (*.golden.json).

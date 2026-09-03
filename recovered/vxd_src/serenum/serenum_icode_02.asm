; serenum_icode_02.asm -- SERENUM object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        sub    esp, 40h
        mov    edi, esp
        push   0
        push   edi
        db  0CDh, 020h, 06Fh, 000h, 001h, 000h
        add    esp, 8
        cmp    dword ptr [edi + 0ch], 2eeh
        lea    esp, [esp + 40h]
        ret
        db  000h, 000h, 000h
_ITEXT ends
        end

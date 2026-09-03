; snapshot_icode_03.asm -- SNAPSHOT object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

SNAPVXD_DEVICE_INIT:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 58h
        mov    byte ptr [ebp + 1ch], 0d4h
        mov    word ptr [ebp + 18h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 2
        je     SHORT loc_3_0040
        db  0C6h, 005h, 0E3h, 000h, 000h, 000h, 001h
loc_3_0040:
        mov    word ptr [ebp + 1ch], 58d2h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 054h, 000h, 028h, 000h
        clc
        ret
        db  0F8h, 0C3h
_ITEXT ends
        end

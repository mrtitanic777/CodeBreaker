; ebios_pcode_05.asm -- EBIOS object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

EBIOS_Get_Version:
        mov    eax, 200h
        db  08Bh, 015h, 054h, 000h, 000h, 000h
        movzx  edx, dx
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        clc
        ret
EBIOS_Get_Unused_Mem:
        xor    eax, eax
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        ret
_PTEXT ends
        end

; vdmad_pcode_04.asm -- VDMAD object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VDMAD_Set_IO_Address:
        db  0BFh, 02Ch, 005h, 000h, 000h
        db  0CDh, 020h, 095h, 000h, 001h, 000h
        xor    esi, esi
        db  0BFh, 029h, 007h
_PTEXT ends
        end

; vpicd_pcode_08.asm -- VPICD object 8 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_8_0000:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_8_002E
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        shl    eax, 8
        db  003h, 005h, 0D4h, 025h, 000h, 000h
        db  0A3h, 0D4h, 025h, 000h, 000h
        shl    eax, 4
        db  0A3h, 0D8h, 025h, 000h, 000h
        add    eax, 3fch
        db  0A3h, 0DCh, 025h, 000h, 000h
loc_8_002E:
        clc
        ret
_PTEXT ends
        end

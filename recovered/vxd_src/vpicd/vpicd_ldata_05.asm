; vpicd_ldata_05.asm -- VPICD object 5 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  053h, 06Ah, 000h, 068h, 0C0h, 000h, 000h, 000h, 0CDh, 020h, 0A7h, 000h, 001h, 000h, 083h, 0C4h
        db  008h, 085h, 0C0h, 075h, 006h, 0CDh, 020h, 0BFh, 080h, 001h, 000h, 0A3h, 0A8h, 025h, 000h, 000h
        db  0CDh, 020h, 0FFh, 000h, 001h, 000h, 085h, 0C0h, 075h, 002h, 0EBh, 0E9h, 0A3h, 0B0h, 025h, 000h
        db  000h, 0B8h, 005h, 000h, 000h, 000h
sub_5_0036:
        xor    ecx, ecx
        db  0CDh, 020h, 05Ch, 001h, 001h, 000h
        jae    SHORT loc_5_0048
        db  0A1h, 0A8h, 025h, 000h, 000h
        lea    eax, [eax + 1fh]
loc_5_0048:
        db  0A3h, 0ACh, 025h, 000h, 000h
        mov    al, 0ah
        out    20h, al
        jmp    SHORT loc_5_0053
loc_5_0053:
        out    0a0h, al
        db  0EBh, 000h
        db  0E4h
_LDATA ends
        end

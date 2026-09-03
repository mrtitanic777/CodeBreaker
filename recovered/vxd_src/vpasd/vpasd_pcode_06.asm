; vpasd_pcode_06.asm -- VPASD object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_6_0008:
        db  0CDh, 020h, 04Eh, 000h, 001h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_0025
        cmp    eax, 0eh
        jae    SHORT loc_6_0025
        db  0FFh, 014h, 085h, 000h, 000h, 000h, 000h
loc_6_0025:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
        ret
_PTEXT ends
        end

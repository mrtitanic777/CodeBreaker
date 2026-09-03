; nscl_icode_02.asm -- NSCL object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        db  0CDh, 020h, 000h, 000h, 087h, 004h
        jb     SHORT loc_2_0036
        xor    edx, edx
        db  0E8h, 044h, 030h, 0FEh, 0FFh
        or     eax, eax
        jne    SHORT loc_2_002D
loc_2_0013:
        db  0CDh, 020h, 002h, 000h, 087h, 004h
        or     eax, eax
        je     SHORT loc_2_0027
        db  081h, 00Dh, 020h, 039h, 000h, 000h, 000h, 010h, 000h, 000h
loc_2_0027:
        db  0E8h, 04Ch, 02Eh, 0FEh, 0FFh
        ret
loc_2_002D:
        db  083h, 00Dh, 020h, 039h, 000h, 000h, 010h
        jmp    SHORT loc_2_0013
loc_2_0036:
        ret
        db  000h, 04Eh, 065h, 074h, 077h, 06Fh, 072h, 06Bh, 000h, 057h, 06Fh, 072h, 06Bh, 067h, 072h, 06Fh
        db  075h, 070h, 000h, 044h, 069h, 072h, 065h, 063h, 074h, 065h, 064h, 042h, 072h, 06Fh, 061h, 064h
        db  063h, 061h, 073h, 074h, 000h, 04Dh, 041h, 053h, 054h, 045h, 052h, 05Fh, 043h, 052h, 045h, 044h
        db  045h, 04Eh, 054h, 049h, 041h, 04Ch, 000h, 000h, 000h
_ITEXT ends
        end

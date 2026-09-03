; reboot_pcode_05.asm -- REBOOT object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        clc
loc_5_0001:
        ret
        db  08Bh, 0C4h, 06Ah, 000h, 050h, 06Ah, 001h, 068h, 0FFh, 000h, 000h, 000h, 0CDh, 020h, 061h, 000h
        db  001h, 000h, 083h, 0C4h, 010h, 058h, 0C1h, 0E8h, 00Ch, 03Dh, 0FFh, 000h, 000h, 000h, 075h, 043h
        db  0A1h, 0ECh, 002h, 000h, 000h, 085h, 0C0h, 075h, 035h, 068h, 000h, 001h, 000h, 000h, 050h, 050h
        db  050h, 050h, 050h, 06Ah, 001h, 06Ah, 001h, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h
        db  085h, 0C0h, 074h, 01Fh, 0A3h, 0ECh, 002h, 000h, 000h, 0BEh, 000h, 0F0h, 00Fh, 000h, 08Bh, 0F8h
        db  0FCh, 0B9h, 000h, 004h, 000h, 000h, 0F3h, 0A5h, 066h, 0C7h, 047h, 0F0h, 0CDh, 019h, 0E8h, 002h
        db  000h, 000h, 000h, 0F8h, 0C3h
loc_5_0067:
        push   0
        push   0
        push   1
        push   0ffh
        push   ebx
        db  0FFh, 035h, 0ECh, 002h, 000h, 000h
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        push   0
        push   -1
        push   0
        push   -3
        push   1
        push   0ffh
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
        ret
_PTEXT ends
        end

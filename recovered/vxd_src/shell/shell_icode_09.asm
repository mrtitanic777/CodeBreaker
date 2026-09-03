; shell_icode_09.asm -- SHELL object 9 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  000h, 000h, 000h, 000h, 088h, 000h, 000h, 000h, 088h, 000h, 000h, 000h, 088h, 000h, 000h, 000h
        db  01Ch, 000h, 000h, 000h, 085h, 000h, 000h, 000h, 0CAh, 000h, 000h, 000h, 00Dh, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 020h, 008h, 000h, 000h, 094h, 004h, 000h, 000h, 022h, 000h
_ITEXT ends
        end

; reboot_pcode_06.asm -- REBOOT object 6 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_6_0000:
        ret
        db  0E8h, 031h, 000h, 000h, 000h, 08Bh, 0C3h, 0CDh, 020h, 03Bh, 000h, 001h, 000h, 0CDh, 020h, 004h
        db  000h, 001h, 000h, 075h, 01Fh, 0CDh, 020h, 03Bh, 000h, 001h, 000h, 033h, 0C3h, 075h, 015h, 087h
        db  005h, 0ECh, 002h, 000h, 000h, 085h, 0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0CDh, 020h, 055h, 000h
        db  001h, 000h, 059h, 059h, 05Bh, 0C3h
sub_6_0037:
        xor    eax, eax
        push   eax
        push   1
        mov    al, 0ffh
        push   eax
        push   ebx
        push   eax
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
        ret
_PTEXT ends
        end

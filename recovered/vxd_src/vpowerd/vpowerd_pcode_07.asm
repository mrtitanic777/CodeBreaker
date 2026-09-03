; vpowerd_pcode_07.asm -- VPOWERD object 7 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 010h, 0E0h
        db  000h, 0F0h, 054h, 04Fh, 053h, 048h, 049h, 042h, 041h, 000h, 000h, 053h, 079h, 073h, 074h, 065h
        db  06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch
        db  053h, 065h, 074h, 05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h, 073h, 05Ch, 056h, 078h, 044h
        db  05Ch, 056h, 050h, 04Fh, 057h, 045h, 052h, 044h, 000h, 046h, 06Ch, 061h, 067h, 073h, 000h, 0F7h
        db  0C3h, 001h, 000h, 00Fh, 085h, 095h, 000h, 066h, 089h, 00Eh, 000h, 000h, 0E8h, 0C2h, 000h, 0F6h
        db  006h, 007h, 000h, 001h, 00Fh, 085h, 089h, 000h, 0B8h, 000h, 053h, 033h, 0DBh, 0CDh, 015h, 00Fh
        db  082h, 089h, 000h, 081h, 0FBh, 04Dh, 050h, 00Fh, 085h, 081h, 000h, 0F7h, 0C1h, 002h, 000h, 074h
        db  07Bh, 0F6h, 006h, 007h, 000h, 002h, 074h, 003h, 0B8h, 000h, 001h, 0A3h, 004h, 000h, 0B8h, 000h
        db  054h, 033h, 0DBh, 0CDh, 02Fh, 03Dh, 000h, 054h, 074h, 019h, 081h, 0FBh, 04Dh, 050h, 075h, 013h
        db  0B8h, 001h, 054h, 0BBh, 000h, 001h, 0CDh, 02Fh, 072h, 052h, 088h, 03Eh, 006h, 000h, 080h, 00Eh
        db  007h, 000h, 080h, 0B8h, 003h, 053h, 033h, 0DBh, 0CDh, 015h, 072h, 040h, 0B8h, 004h, 053h, 033h
        db  0DBh, 0CDh, 015h, 072h, 037h, 0E8h, 0C4h, 000h, 075h, 005h, 080h, 00Eh, 007h, 000h, 040h, 08Dh
        db  036h, 004h, 000h, 0B8h, 006h, 000h, 0B9h, 008h, 000h, 0BAh, 002h, 000h, 0FFh, 01Eh, 000h, 000h
        db  072h, 01Ah, 066h, 08Bh, 0D0h, 033h, 0C0h, 033h, 0DBh, 033h, 0F6h, 0C3h, 0B8h, 001h, 080h, 0EBh
        db  0F6h, 0BAh, 001h, 001h, 0B9h, 020h, 000h, 0B8h, 017h, 04Ah, 0CDh, 02Fh, 0B8h, 003h, 080h, 0EBh
        db  0E6h, 041h, 050h, 04Dh, 020h, 064h, 069h, 073h, 061h, 062h, 06Ch, 065h, 064h, 020h, 062h, 079h
        db  020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 04Dh, 061h, 06Eh, 061h, 067h, 065h, 072h, 00Dh
        db  00Ah
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        lea    eax, [esi - 4]
        push   ss
        push   eax
        push   ds
        push   0b866001bh
        add    al, byte ptr [eax]
        add    byte ptr [eax + 0b85066h], al
        add    edi, edi
        push   ds
        add    byte ptr [eax], al
        add    esp, 0ch
        or     eax, edx
        jne    SHORT loc_7_0188
        lea    eax, [esi - 0ch]
        push   ss
        push   eax
        lea    eax, [esi - 8]
        push   ss
        push   eax
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   ds
        push   0ff660049h
        db  076h, 0FCh
        mov    eax, 1eff0109h
        add    byte ptr [eax], al
        add    esp, 18h
        or     eax, edx
        db  075h, 00Fh
        cmp    word ptr [esi - 0ch], 4
        db  075h, 008h
        mov    al, byte ptr [esi - 8]
        and    al, 3fh
        mov    byte ptr [0ff660007h], al
        db  076h, 0FCh
        mov    eax, 1eff0102h
        add    byte ptr [eax], al
        pop    ax
loc_7_0188:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 01Eh, 00Ch, 000h, 0E8h, 00Fh, 000h, 075h, 00Ah, 0B8h, 0F0h, 044h, 0CDh, 015h, 072h, 003h
        db  032h, 0C0h, 0C3h, 00Ch, 001h, 0C3h, 006h, 056h, 057h, 0FCh, 08Bh, 00Fh, 00Bh, 0C9h, 074h, 011h
        db  0C4h, 07Fh, 002h, 08Dh, 077h, 006h, 0F3h, 0A6h, 075h, 007h, 003h, 01Fh, 08Dh, 05Fh, 006h, 0EBh
        db  0E9h, 05Fh, 05Eh, 007h, 0C3h, 000h
_PTEXT ends
        end

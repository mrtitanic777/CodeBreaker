; perf_icode_03.asm -- PERF object 3 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_3_0000:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        clc
        ret
loc_3_0016:
        db  068h, 008h, 000h, 000h, 000h
        db  068h, 020h, 000h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 04Bh, 001h, 001h, 000h
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 1
        cmc
        jb     SHORT loc_3_003F
        call   sub_3_0040
loc_3_003F:
        ret
sub_3_0040:
        push   esi
        push   edi
        push   ebx
        push   ecx
        push   edx
        db  00Fh, 0B7h, 01Dh, 00Ch, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Ah, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 048h, 000h
        or     eax, eax
        je     SHORT loc_3_007E
        db  068h, 074h, 00Eh, 000h, 000h
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0CDh, 020h, 001h, 000h, 048h, 000h
        add    esp, 18h
loc_3_007E:
        cmp    eax, 0
        je     NEAR PTR loc_3_0164
        db  0A3h, 048h, 001h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 00Eh, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 014h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 0D4h, 00Eh, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0
        push   esp
        push   eax
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 010h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 016h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  0A1h, 048h, 001h, 000h, 000h
        db  068h, 0F0h, 00Eh, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0
        push   esp
        push   eax
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  0A1h, 048h, 001h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 012h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 020h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 018h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 00Ch, 00Fh, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0
        push   esp
        push   eax
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  0A3h, 04Ch, 001h, 000h, 000h
loc_3_0164:
        pop    edx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        ret
        db  000h, 000h
_ITEXT ends
        end

; spooler_icode_02.asm -- SPOOLER object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

SPOOLER_DEVICE_INIT:
        db  0CDh, 020h, 000h, 000h, 040h, 000h
        jb     SHORT loc_2_001F
        db  068h, 06Dh, 001h, 000h, 000h
        push   22h
        db  068h, 080h, 000h, 000h, 000h
        db  0CDh, 020h, 062h, 000h, 040h, 000h
        add    esp, 0ch
        jae    SHORT loc_2_0021
loc_2_001F:
        stc
        ret
loc_2_0021:
        clc
        ret
SPOOLER_INIT_COMPLETE:
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 004h, 000h, 000h, 000h
        push   80000002h
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, 0
        jne    SHORT loc_2_0096
        db  068h, 045h, 000h, 000h, 000h
        db  068h, 049h, 000h, 000h, 000h
        db  068h, 041h, 000h, 000h, 000h
        push   0
        db  068h, 034h, 000h, 000h, 000h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        pop    ecx
        db  0CDh, 020h, 000h, 000h, 021h, 000h
        jb     SHORT loc_2_0096
        cmp    eax, 400h
        jne    SHORT loc_2_0096
        cmp    bl, 3
        jne    SHORT loc_2_0096
        call   sub_2_01E9
        or     eax, eax
        jne    SHORT loc_2_0096
        db  0C6h, 005h, 0DBh, 002h, 000h, 000h, 001h
loc_2_0096:
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_00A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 68h
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        sub    esi, esi
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_2_01E2
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 5ch], ecx
        mov    dword ptr [ebp - 58h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 54h], eax
        push   dword ptr [ebp - 5ch]
        push   dword ptr [ebp - 58h]
        push   dword ptr [ebp - 54h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, esi
        jne    NEAR PTR loc_2_01E2
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 50h], eax
        db  0C7h, 045h, 0B4h, 030h, 003h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 48h], eax
        push   dword ptr [ebp - 50h]
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, esi
        jne    NEAR PTR loc_2_01CA
        mov    dword ptr [ebp - 8], 0ah
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 44h], eax
        lea    eax, [ebp - 68h]
        mov    dword ptr [ebp - 40h], eax
        mov    dword ptr [ebp - 3ch], esi
        mov    dword ptr [ebp - 38h], esi
        db  0C7h, 045h, 0CCh, 028h, 003h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 44h]
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        push   dword ptr [ebp - 34h]
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        cmp    eax, esi
        jne    SHORT loc_2_01B2
        mov    dword ptr [ebp - 8], 0ah
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 2ch], eax
        lea    eax, [ebp - 68h]
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 24h], esi
        mov    dword ptr [ebp - 20h], esi
        db  0C7h, 045h, 0E4h, 09Ch, 003h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        cmp    eax, esi
        jne    SHORT loc_2_01B2
        mov    esi, 1
loc_2_01B2:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        push   dword ptr [ebp - 14h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_2_01CA:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 10h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_2_01E2:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_01E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 88h
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 3ch
        sub    esi, esi
        lea    eax, [ebp - 28h]
        mov    dword ptr [ebp - 4ch], eax
        db  0C7h, 045h, 0B8h, 040h, 003h, 000h, 000h
        mov    dword ptr [ebp - 44h], 80000002h
        push   dword ptr [ebp - 4ch]
        push   dword ptr [ebp - 48h]
        push   dword ptr [ebp - 44h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, esi
        jne    NEAR PTR loc_2_0320
        lea    eax, [ebp - 2ch]
        mov    dword ptr [ebp - 40h], eax
        db  0C7h, 045h, 0C4h, 078h, 003h, 000h, 000h
        mov    dword ptr [ebp - 38h], 80000002h
        push   dword ptr [ebp - 40h]
        push   dword ptr [ebp - 3ch]
        push   dword ptr [ebp - 38h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        cmp    eax, esi
        jne    NEAR PTR loc_2_0308
        sub    edi, edi
loc_2_0267:
        mov    dword ptr [ebp - 4], 3ch
        mov    ecx, edi
        inc    edi
        mov    dword ptr [ebp - 24h], 0
        mov    dword ptr [ebp - 20h], 0
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [ebp - 18h], 0
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [ebp - 88h]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 0ch], ecx
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 24h]
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 050h, 001h, 001h, 000h
        add    esp, 20h
        or     eax, eax
        jne    SHORT loc_2_02F0
        lea    eax, [ebp - 88h]
        push   eax
        push   dword ptr [ebp - 2ch]
        call   sub_2_00A0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_02EB
        jmp    NEAR PTR loc_2_0267
loc_2_02EB:
        mov    esi, 1
loc_2_02F0:
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 34h], eax
        push   dword ptr [ebp - 34h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_2_0308:
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 30h], eax
        push   dword ptr [ebp - 30h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
loc_2_0320:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_0328:
        dec    esi
        db  065h, 074h, 049h
        inc    esp
        add    byte ptr [eax], al
        db  000h
loc_2_0330:
        dec    esi
        db  065h, 074h, 077h
        outsd
        db  072h, 06Bh
        push   eax
        db  072h, 06Fh
        db  076h, 069h
        jb     NEAR PTR loc_2_0340
loc_2_0340:
        push   ebx
        db  079h, 073h
        je     SHORT loc_2_03AA
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        je     SHORT loc_2_0392
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        inc    ebx
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        pop    esp
        dec    esi
        db  065h, 074h, 077h
        outsd
        db  072h, 06Bh
        push   eax
        db  072h, 06Fh
        db  076h, 069h
        db  064h, 065h, 072h, 05Ch
        dec    edi
        db  072h, 064h
        db  065h, 072h, 000h
        db  000h
loc_2_0378:
        push   ebx
        db  079h, 073h
        db  074h, 065h
        insd
        pop    esp
        inc    ebx
        db  075h, 072h
        db  072h, 065h
        outsb
        db  074h, 043h
        outsd
        outsb
        db  074h, 072h
        outsd
        insb
        push   ebx
        db  065h, 074h, 05Ch
        push   ebx
loc_2_0392:
        db  065h, 072h, 076h
        imul   esp, dword ptr [ebx + 65h], 73h
loc_2_039C:
        push   edi
        imul   ebp, dword ptr [esi + 64h], 3373776fh
        xor    dword ptr [esp + ecx*2 + 4ch], eax
        add    byte ptr [eax], al
loc_2_03AA:
        add    byte ptr [eax], al
_ITEXT ends
        end

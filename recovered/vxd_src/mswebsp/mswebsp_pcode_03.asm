; mswebsp_pcode_03.asm -- MSWEBSP object 3 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  053h, 056h, 057h, 055h, 08Bh, 05Ch, 024h, 01Ch, 08Bh, 054h, 024h, 018h, 08Bh, 02Bh, 03Bh, 0D5h
        db  072h, 00Eh, 0B8h, 002h, 000h, 000h, 000h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 064h, 024h, 000h
        db  08Bh, 074h, 024h, 014h, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h
        db  0D1h, 049h, 02Bh, 0E9h, 04Dh, 089h, 02Bh, 03Bh, 0D5h, 072h, 00Ah, 0B8h, 002h, 000h, 000h, 000h
        db  05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh
        db  0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h, 08Bh, 0F7h, 08Bh, 0FDh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh
        db  0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 003h, 089h, 002h, 0FFh, 00Bh, 02Bh, 0C0h, 05Dh, 05Fh
        db  05Eh, 05Bh, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_01:
        sub    esp, 154h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 174h]
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    ebx, dword ptr [esp + 170h]
        add    eax, ebx
        mov    dword ptr [esp + 20h], eax
        lea    ebp, [ebx + 4]
        lea    edi, [esp + 64h]
        mov    ecx, 0bh
        rep movsd
        movsw
        movsb
        mov    edi, dword ptr [esp + 16ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 64h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        db  0BAh, 030h, 000h, 000h, 000h
        lea    edi, [esp + 64h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        mov    eax, dword ptr [edx]
        mov    dword ptr [edi - 1], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 3], eax
        dec    edi
        mov    eax, dword ptr [esp + 16ch]
        mov    dword ptr [ebx], 0
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_0127
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 154h
        ret
loc_3_0127:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 68h]
        push   eax
        push   80000002h
        call   sub_3_08F8
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_3_0151
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 154h
        ret
loc_3_0151:
        mov    dword ptr [esp + 14h], 40h
        mov    dword ptr [esp + 18h], 100h
        lea    eax, [esp + 18h]
        push   eax
        sub    edi, edi
        lea    eax, [esp + 68h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 20h]
        push   edi
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   edi
        push   eax
        call   sub_3_0908
        add    esp, 20h
        cmp    eax, edi
        jne    SHORT loc_3_01DD
        nop
loc_3_0190:
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   ebp
        push   eax
        call   0
        add    esp, 0ch
        mov    esi, eax
        cmp    esi, 2
        je     SHORT loc_3_01DD
        add    ebp, 4
        lea    eax, [esp + 18h]
        push   eax
        inc    dword ptr [ebx]
        lea    eax, [esp + 68h]
        push   eax
        inc    edi
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 20h]
        push   0
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   edi
        push   eax
        call   sub_3_0908
        add    esp, 20h
        or     eax, eax
        je     SHORT loc_3_0190
loc_3_01DD:
        mov    eax, dword ptr [esp + 10h]
        push   eax
        call   sub_3_0900
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 154h
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 083h, 03Dh, 088h, 005h, 000h, 000h, 000h
        db  074h, 015h, 0C7h, 005h, 088h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 0A1h, 094h, 006h, 000h
        db  000h, 050h, 0E8h, 0F2h, 006h, 000h, 000h, 0C3h, 090h
SECPROV_Service_02:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_03:
        mov    eax, 5
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_06:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_04:
        sub    eax, eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
SECPROV_Service_05:
        push   edi
        mov    edx, dword ptr [esp + 8]
        db  0BFh, 07Ch, 001h, 000h, 000h
        mov    dword ptr [edx], 1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    dword ptr [edx + 4], ecx
        db  0C7h, 042h, 008h, 07Ch, 001h, 000h, 000h
        mov    dword ptr [edx + 0ch], 2
        db  0C7h, 042h, 010h, 040h, 001h, 000h, 000h
        sub    eax, eax
        pop    edi
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C7h, 005h, 078h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0C7h, 005h, 044h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 0C3h, 08Dh, 064h
        db  024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_02C0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 4]
        push   ecx
        mov    eax, dword ptr [eax]
        push   eax
        call   SECPROV_Service_02
        add    esp, 8
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_02E0:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 1ch], 4
        je     SHORT loc_3_02F8
        mov    eax, 32h
        pop    esi
        add    esp, 4
        ret
loc_3_02F8:
        lea    eax, [esp + 6]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   eax
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi]
        push   eax
        call   SECPROV_Service_03
        add    esp, 20h
        xor    edx, edx
        mov    ecx, dword ptr [esi + 20h]
        mov    dx, word ptr [esp + 6]
        pop    esi
        mov    dword ptr [ecx], edx
        add    esp, 4
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_0340:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        push   eax
        mov    eax, dword ptr [ecx + 4]
        push   eax
        mov    eax, dword ptr [ecx]
        push   eax
        call   SECPROV_Service_04
        add    esp, 0ch
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0360:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx], 4
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [eax], 1
        sub    eax, eax
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_3_0380:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edx + 4]
        db  08Bh, 00Dh, 048h, 001h, 000h, 000h
        cmp    dword ptr [eax], ecx
        jae    SHORT loc_3_03A0
        mov    eax, 6fh
        pop    edi
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_3_03A0:
        db  0A1h, 048h, 001h, 000h, 000h
        mov    edi, dword ptr [edx]
        db  08Bh, 035h, 044h, 001h, 000h, 000h
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [edx + 4]
        db  0A1h, 048h, 001h, 000h, 000h
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_03D0:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_03E0:
        mov    eax, 1
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_03F0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        sub    eax, eax
        mov    ebx, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    edx, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 1ch], eax
        je     SHORT loc_3_0458
loc_3_0408:
        mov    esi, dword ptr [edx + 4]
        mov    edi, dword ptr [esp + 24h]
        mov    ecx, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        add    dword ptr [edi], ecx
        mov    ebp, dword ptr [edx]
        cmp    ebx, ebp
        jb     SHORT loc_3_0443
        mov    edi, dword ptr [esp + 20h]
        mov    esi, dword ptr [esi]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [edx]
        add    dword ptr [esp + 20h], ecx
        sub    ebx, dword ptr [edx]
        jmp    SHORT loc_3_044F
loc_3_0443:
        mov    dword ptr [edx + 8], 0
        mov    eax, 0eah
loc_3_044F:
        add    edx, 0ch
        dec    dword ptr [esp + 1ch]
        jne    SHORT loc_3_0408
loc_3_0458:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
SECPROV_Service_00:
        mov    eax, 400h
        clc
        ret
        db  0CDh, 020h, 00Fh, 080h, 091h, 004h, 056h, 057h, 0FCh, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 074h, 024h
        db  010h, 0ACh, 0AAh, 00Ah, 0C0h, 075h, 0FAh, 05Fh, 05Eh, 0C3h, 056h, 057h, 0FCh, 08Bh, 07Ch, 024h
        db  00Ch, 08Bh, 074h, 024h, 010h, 08Ah, 007h, 03Ah, 006h, 075h, 00Bh, 047h, 046h, 00Ah, 0C0h, 075h
        db  0F4h, 02Bh, 0C0h, 05Fh, 05Eh, 0C3h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 0EBh, 0F6h, 057h, 08Bh, 07Ch
        db  024h, 008h, 02Bh, 0C0h, 08Bh, 0C8h, 049h, 0F2h, 0AEh, 0F7h, 0D9h, 083h, 0E9h, 002h, 08Bh, 0C1h
        db  05Fh, 0C3h, 08Bh, 044h, 024h, 004h, 086h, 0E0h, 0C3h, 0C8h, 034h, 000h, 000h, 08Dh, 05Dh, 0CCh
        db  0C7h, 003h, 09Ch, 006h, 000h, 000h, 0C7h, 043h, 01Ch, 003h, 05Fh, 000h, 000h, 0C7h, 043h, 010h
        db  000h, 000h, 000h, 000h, 066h, 0C7h, 043h, 014h, 000h, 000h, 053h, 0CDh, 020h, 001h, 000h, 08Eh
        db  004h, 058h, 066h, 08Bh, 043h, 02Ch, 066h, 0A9h, 001h, 000h, 075h, 02Ch, 08Bh, 043h, 01Ch, 0C1h
        db  0E8h, 010h, 0A2h, 025h, 001h, 000h, 000h, 066h, 0C7h, 005h, 026h, 001h, 000h, 000h, 000h, 000h
        db  0C7h, 043h, 01Ch, 003h, 05Fh, 000h, 000h, 066h, 0C7h, 043h, 014h, 003h, 002h, 053h, 0CDh, 020h
        db  001h, 000h, 08Eh, 004h, 058h, 0F8h, 0C9h, 0C3h, 0C6h, 005h, 025h, 001h, 000h, 000h, 000h, 0F9h
        db  0EBh, 0F4h, 0C8h, 034h, 000h, 000h, 08Dh, 05Dh, 0CCh, 0C7h, 003h, 09Ch, 006h, 000h, 000h, 0C7h
        db  043h, 01Ch, 003h, 05Fh, 000h, 000h, 0C7h, 043h, 010h, 000h, 000h, 000h, 000h, 066h, 0C7h, 043h
        db  014h, 001h, 002h, 053h, 0CDh, 020h, 001h, 000h, 08Eh, 004h, 058h, 066h, 08Bh, 043h, 02Ch, 066h
        db  0A9h, 001h, 000h, 075h, 017h, 08Bh, 043h, 01Ch, 0C1h, 0E8h, 010h, 0A2h, 025h, 001h, 000h, 000h
        db  066h, 0C7h, 005h, 026h, 001h, 000h, 000h, 000h, 002h, 0F8h, 0C9h, 0C3h, 0C6h, 005h, 025h, 001h
        db  000h, 000h, 000h, 0F9h, 0EBh, 0F4h
sub_3_057D:
        enter  34h, 0
        push   ebx
        db  0C6h, 005h, 025h, 001h, 000h, 000h, 000h
        lea    ebx, [ebp - 34h]
        db  0C7h, 003h, 09Ch, 006h, 000h, 000h
        mov    dword ptr [ebx + 1ch], 5f03h
        mov    dword ptr [ebx + 10h], 0
        mov    word ptr [ebx + 14h], 203h
        push   ebx
        db  0CDh, 020h, 001h, 000h, 08Eh, 004h
        pop    ebx
        mov    ax, word ptr [ebx + 2ch]
        pop    ebx
        leave
        ret
        db  0C8h, 034h, 000h, 000h, 056h, 057h, 053h, 0A1h, 008h, 001h, 000h, 000h, 0B9h, 005h, 000h, 000h
        db  000h, 0CDh, 020h, 027h, 000h, 001h, 000h, 00Fh, 0B6h, 005h, 025h, 001h, 000h, 000h, 00Bh, 0C0h
        db  074h, 04Fh, 08Dh, 05Dh, 0CCh, 0C1h, 0E0h, 010h, 066h, 0B8h, 0FEh, 0E9h, 089h, 043h, 01Ch, 0A1h
        db  01Ch, 001h, 000h, 000h, 089h, 003h, 053h, 0CDh, 020h, 001h, 000h, 08Eh, 004h, 083h, 0C4h, 004h
        db  08Dh, 05Dh, 0CCh, 066h, 08Bh, 043h, 02Ch, 066h, 0A9h, 001h, 000h, 075h, 024h, 0FFh, 035h, 01Ch
        db  001h, 000h, 000h, 0FFh, 035h, 044h, 001h, 000h, 000h, 0E8h, 06Eh, 0FEh, 0FFh, 0FFh, 083h, 0C4h
        db  008h, 00Bh, 0C0h, 075h, 00Ch, 066h, 083h, 03Dh, 026h, 001h, 000h, 000h, 000h, 074h, 010h, 0EBh
        db  02Fh, 0E8h, 095h, 0FEh, 0FFh, 0FFh, 073h, 007h, 0E8h, 0F7h, 0FEh, 0FFh, 0FFh, 072h, 038h, 0E8h
        db  0B7h, 000h, 000h, 000h, 08Bh, 010h, 00Bh, 0D2h, 075h, 016h, 066h, 083h, 03Dh, 026h, 001h, 000h
        db  000h, 000h, 075h, 007h, 0E8h, 0DBh, 0FEh, 0FFh, 0FFh, 072h, 01Ch, 0E8h, 037h, 001h, 000h, 000h
        db  0A1h, 008h, 001h, 000h, 000h, 0CDh, 020h, 028h, 000h, 001h, 000h, 00Fh, 0B6h, 005h, 025h, 001h
        db  000h, 000h, 05Bh, 05Fh, 05Eh, 0C9h, 0C3h, 0EBh, 0E7h, 0C3h, 057h, 056h, 08Bh, 03Dh, 01Ch, 001h
        db  000h, 000h, 0C6h, 047h, 002h, 03Dh, 083h, 0C7h, 003h, 066h, 0C7h, 007h, 000h, 004h, 08Bh, 00Dh
        db  048h, 001h, 000h, 000h, 049h, 088h, 04Fh, 002h, 083h, 0C7h, 003h, 08Bh, 035h, 044h, 001h, 000h
        db  000h, 0F3h, 0A4h, 0C6h, 007h, 001h, 0B9h, 00Bh, 000h, 000h, 000h, 047h, 088h, 00Fh, 047h, 0BEh
        db  030h, 001h, 000h, 000h, 0F3h, 0A4h, 08Bh, 035h, 01Ch, 001h, 000h, 000h, 02Bh, 0FEh, 066h, 08Bh
        db  0C7h, 066h, 083h, 0E8h, 002h, 086h, 0E0h, 066h, 089h, 006h, 0FFh, 035h, 018h, 001h, 000h, 000h
        db  0FFh, 035h, 020h, 001h, 000h, 000h, 057h, 0FFh, 035h, 01Ch, 001h, 000h, 000h, 0B8h, 017h, 000h
        db  000h, 000h, 050h, 0E8h, 0F2h, 000h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 008h, 0A1h
        db  020h, 001h, 000h, 000h, 05Eh, 05Fh, 0C3h, 02Bh, 0C0h, 0EBh, 0F9h, 0C8h, 034h, 000h, 000h, 08Dh
        db  07Dh, 0CCh, 00Fh, 0B6h, 005h, 025h, 001h, 000h, 000h, 0C1h, 0E0h, 010h, 066h, 0B8h, 0FFh, 0E9h
        db  089h, 047h, 01Ch, 0A1h, 020h, 001h, 000h, 000h, 089h, 007h, 057h, 0CDh, 020h, 001h, 000h, 08Eh
        db  004h, 058h, 066h, 08Bh, 047h, 02Ch, 066h, 083h, 0E0h, 001h, 075h, 044h, 08Bh, 03Dh, 020h, 001h
        db  000h, 000h, 0A1h, 01Ch, 001h, 000h, 000h, 080h, 07Fh, 01Ch, 000h, 075h, 042h, 08Ah, 04Fh, 01Bh
        db  0C6h, 040h, 002h, 016h, 066h, 0C7h, 000h, 000h, 002h, 088h, 048h, 003h, 0BAh, 004h, 000h, 000h
        db  000h, 0FFh, 035h, 018h, 001h, 000h, 000h, 0FFh, 035h, 020h, 001h, 000h, 000h, 052h, 0FFh, 035h
        db  01Ch, 001h, 000h, 000h, 06Ah, 017h, 0E8h, 06Fh, 000h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h
        db  0A1h, 020h, 001h, 000h, 000h, 074h, 006h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C9h, 0C3h, 0C6h
        db  040h, 002h, 01Ch, 066h, 0C7h, 000h, 000h, 005h, 066h, 08Bh, 04Fh, 01Bh, 066h, 089h, 048h, 003h
        db  0BAh, 007h, 000h, 000h, 000h, 0EBh, 0BAh, 0C8h, 034h, 000h, 000h, 053h, 0B8h, 0FCh, 001h, 000h
        db  000h, 08Dh, 055h, 0CCh, 089h, 002h, 0B8h, 03Ch, 001h, 000h, 000h, 089h, 042h, 004h, 066h, 0B8h
        db  003h, 05Fh, 089h, 042h, 01Ch, 0C6h, 042h, 010h, 000h, 066h, 0C7h, 042h, 018h, 001h, 000h, 066h
        db  0C7h, 042h, 014h, 001h, 002h, 052h, 0CDh, 020h, 001h, 000h, 08Eh, 004h, 05Ah, 00Fh, 0B7h, 042h
        db  02Ch, 024h, 001h, 00Ah, 0C0h, 075h, 000h, 05Bh, 0C9h, 0C3h, 0C8h, 034h, 000h, 000h, 053h, 0CDh
        db  020h, 0CFh, 000h, 001h, 000h, 0A3h, 028h, 001h, 000h, 000h, 08Bh, 045h, 00Ch, 08Dh, 05Dh, 0CCh
        db  089h, 043h, 004h, 08Bh, 045h, 014h, 089h, 003h, 08Bh, 045h, 010h, 066h, 089h, 043h, 018h, 08Bh
        db  045h, 018h, 066h, 089h, 043h, 014h, 00Fh, 0B6h, 005h, 025h, 001h, 000h, 000h, 0C1h, 0E0h, 010h
        db  0B4h, 0F2h, 08Ah, 045h, 008h, 089h, 043h, 01Ch, 053h, 0CDh, 020h, 001h, 000h, 08Eh, 004h, 058h
        db  0A1h, 02Ch, 001h, 000h, 000h, 00Bh, 0C0h, 075h, 029h, 066h, 0A1h, 026h, 001h, 000h, 000h, 066h
        db  083h, 0F8h, 000h, 074h, 01Dh, 0B8h, 060h, 0EAh, 000h, 000h, 0BAh, 000h, 000h, 000h, 000h, 056h
        db  0BEh, 04Eh, 008h, 000h, 000h, 0CDh, 020h, 03Ch, 000h, 001h, 000h, 089h, 035h, 02Ch, 001h, 000h
        db  000h, 05Eh, 00Fh, 0B6h, 043h, 01Ch, 05Bh, 0C9h, 0C3h
loc_3_084E:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  02Bh, 005h, 028h, 001h, 000h, 000h
        cmp    eax, 7530h
        ja     SHORT loc_3_087F
        mov    eax, 0ea60h
        mov    edx, 0
        push   esi
        db  0BEh, 04Eh, 008h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 02Ch, 001h, 000h, 000h
        pop    esi
        ret
loc_3_087F:
        db  0C7h, 005h, 02Ch, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 0
        push   eax
        push   eax
        db  068h, 09Fh, 008h, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        ret
loc_3_089F:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  02Bh, 005h, 028h, 001h, 000h, 000h
        cmp    eax, 7530h
        jbe    SHORT loc_3_08B7
        call   sub_3_057D
loc_3_08B7:
        ret
SECPROV_W32_DEVICEIOCONTROL:
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 0
        jne    SHORT loc_3_08C7
        mov    ecx, 400h
        jmp    SHORT loc_3_08CC
loc_3_08C7:
        cmp    ecx, -1
        jne    SHORT loc_3_08D0
loc_3_08CC:
        xor    eax, eax
        clc
        ret
loc_3_08D0:
        cmp    ecx, 64h
        jl     SHORT loc_3_08EF
        cmp    ecx, 6ch
        jge    SHORT loc_3_08EF
        sub    ecx, 64h
        push   dword ptr [esi + 14h]
        push   dword ptr [esi + 10h]
        db  0FFh, 014h, 08Dh, 038h, 000h, 000h, 000h
        add    esp, 8
        clc
        ret
loc_3_08EF:
        mov    eax, 1
        clc
        ret
        db  000h, 000h
sub_3_08F8:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0900:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_3_0908:
        db  0CDh, 020h, 050h, 081h, 001h, 000h
        add    byte ptr [eax], al
        pop    edx
        pop    eax
        db  0CDh, 020h, 028h, 000h, 001h, 000h
        jmp    edx
_PTEXT ends
        end

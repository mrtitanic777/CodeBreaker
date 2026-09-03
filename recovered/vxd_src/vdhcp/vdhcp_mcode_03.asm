; vdhcp_mcode_03.asm -- VDHCP object 3 segment _LMSGTABLE
.386p
_LMSGTABLE segment dword public use32 'MCODE'

        db  0CDh, 020h, 000h, 000h
loc_3_0004:
        mov    byte ptr [edx + esi*2], al
        adc    al, 0cdh
        and    byte ptr [eax], al
        db  000h
loc_3_000C:
        inc    eax
        db  000h
loc_3_000E:
        jb     SHORT loc_3_001C
        db  0CDh, 020h
loc_3_0012:
        or     al, byte ptr [eax]
loc_3_0014:
        mov    byte ptr [ebx], al
        db  0C8h
loc_3_0018:
        sub    byte ptr [eax], al
        add    al, bh
loc_3_001C:
        cmc
        sbb    eax, eax
        ret
        db  0E8h, 0DBh, 0FFh, 0FFh, 0FFh, 00Bh, 0C0h, 00Fh, 084h, 0FFh, 000h, 000h, 000h, 0C7h, 005h, 040h
        db  030h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 048h, 030h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0A1h, 0C8h, 028h, 000h, 000h, 0A3h, 044h, 030h, 000h, 000h, 0E8h, 004h, 000h, 0FEh, 0FFh
        db  0E8h, 07Bh, 006h, 000h, 000h, 00Bh, 0C0h, 075h, 003h, 02Bh, 0C0h, 0C3h, 0E8h, 037h, 002h, 000h
        db  000h, 00Bh, 0C0h, 075h, 003h, 02Bh, 0C0h, 0C3h, 0C7h, 005h, 078h, 02Bh, 000h, 000h, 001h, 000h
        db  000h, 000h, 0C7h, 005h, 064h, 02Bh, 000h, 000h, 060h, 02Bh, 000h, 000h, 0C7h, 005h, 060h, 02Bh
        db  000h, 000h, 060h, 02Bh, 000h, 000h, 0C7h, 005h, 088h, 02Bh, 000h, 000h, 084h, 02Bh, 000h, 000h
        db  0C7h, 005h, 084h, 02Bh, 000h, 000h, 084h, 02Bh, 000h, 000h, 0C7h, 005h, 050h, 030h, 000h, 000h
        db  04Ch, 030h, 000h, 000h, 0C7h, 005h, 04Ch, 030h, 000h, 000h, 04Ch, 030h, 000h, 000h, 0C7h, 005h
        db  074h, 02Bh, 000h, 000h, 070h, 02Bh, 000h, 000h, 0C7h, 005h, 070h, 02Bh, 000h, 000h, 070h, 02Bh
        db  000h, 000h, 0E8h, 011h, 006h, 000h, 000h, 00Bh, 0C0h, 074h, 005h, 02Bh, 0C0h, 0C3h, 08Bh, 0FFh
        db  06Ah, 000h, 0E8h, 019h, 04Ch, 0FEh, 0FFh, 083h, 0C4h, 004h, 06Ah, 001h, 068h, 0D8h, 007h, 000h
        db  000h, 0E8h, 0C4h, 021h, 0FEh, 0FFh, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 003h, 02Bh, 0C0h, 0C3h
        db  068h, 00Ch, 007h, 000h, 000h, 0E8h, 0B6h, 0FFh, 0FDh, 0FFh, 083h, 0C4h, 004h, 068h, 0B4h, 007h
        db  000h, 000h, 0E8h, 093h, 0FFh, 0FDh, 0FFh, 083h, 0C4h, 004h, 06Ah, 000h, 06Ah, 000h, 0E8h, 005h
        db  00Ah, 0FEh, 0FFh, 083h, 0C4h, 008h, 0E8h, 039h, 0FFh, 0FDh, 0FFh, 0C7h, 005h, 0CCh, 028h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0B8h, 001h, 000h, 000h, 000h, 0C3h, 090h, 02Bh, 0C0h, 0C3h, 090h
        db  056h, 057h, 06Ah, 000h, 0FFh, 074h, 024h, 010h, 0E8h, 057h, 004h, 000h, 000h, 083h, 0C4h, 008h
        db  08Bh, 0F0h, 00Bh, 0F6h, 074h, 052h, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h
        db  0AEh, 0F7h, 0D1h, 051h, 0E8h, 047h, 00Ch, 0FEh, 0FFh, 083h, 0C4h, 004h, 08Bh, 04Ch, 024h, 010h
        db  089h, 001h, 08Bh, 0D0h, 00Bh, 0D2h, 074h, 028h, 08Bh, 0FEh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh
        db  0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h, 08Bh, 0F7h, 08Bh, 0FAh, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 02Bh, 0C0h, 05Fh, 05Eh, 0C3h, 08Bh, 0FFh
        db  05Fh, 0B8h, 09Ah, 000h, 000h, 0C0h, 05Eh, 0C3h, 05Fh, 0B8h, 001h, 000h, 000h, 0C0h, 05Eh, 0C3h
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 30h], eax
        db  0C7h, 045h, 0D4h, 0ECh, 028h, 000h, 000h
        mov    dword ptr [ebp - 28h], 80000002h
        push   dword ptr [ebp - 30h]
        push   dword ptr [ebp - 2ch]
        push   dword ptr [ebp - 28h]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        jne    NEAR PTR loc_3_0290
        mov    dword ptr [ebp - 24h], 10h
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 44h]
        mov    dword ptr [ebp - 1ch], eax
        lea    eax, [ebp - 34h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 14h], 0
        db  0C7h, 045h, 0F0h, 028h, 029h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   dword ptr [ebp - 20h]
        push   dword ptr [ebp - 1ch]
        push   dword ptr [ebp - 18h]
        push   dword ptr [ebp - 14h]
        push   dword ptr [ebp - 10h]
        push   dword ptr [ebp - 0ch]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        add    esp, 18h
        mov    esi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        add    esp, 4
        or     esi, esi
        jne    SHORT loc_3_0290
        lea    edi, [ebp - 44h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  0E8h, 042h, 00Bh, 0FEh, 0FFh
        add    esp, 4
        db  0A3h, 058h, 030h, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_0290
        lea    edi, [ebp - 44h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        db  08Bh, 03Dh, 058h, 030h, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_0290:
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  08Bh, 0FFh, 053h, 056h, 057h, 02Bh, 0DBh, 039h, 01Dh, 040h, 029h, 000h, 000h, 074h, 03Dh, 0BEh
        db  044h, 029h, 000h, 000h, 0FFh, 076h, 0FCh, 0E8h, 054h, 01Eh, 0FEh, 0FFh, 083h, 0C4h, 004h, 089h
        db  006h, 08Bh, 0F8h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 049h, 03Bh
        db  0CBh, 072h, 010h, 08Bh, 03Eh, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h
        db  08Dh, 059h, 0FFh, 083h, 0C6h, 008h, 083h, 07Eh, 0FCh, 000h, 075h, 0C8h, 083h, 0C3h, 04Eh, 053h
        db  0E8h, 0B5h, 00Ah, 0FEh, 0FFh, 083h, 0C4h, 004h, 0A3h, 078h, 029h, 000h, 000h, 00Bh, 0C0h, 075h
        db  009h, 02Bh, 0C0h, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 049h, 000h, 068h, 052h, 004h, 000h, 000h, 0E8h
        db  0FCh, 01Dh, 0FEh, 0FFh, 083h, 0C4h
_LMSGTABLE ends
        end

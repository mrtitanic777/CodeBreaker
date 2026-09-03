; vxdldr_ldata_05.asm -- VXDLDR object 5 segment _LDATA
.386p
_LDATA segment dword public use32 'LDATA'

        db  08Ch, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 010h, 08Bh, 0D8h, 083h, 07Ch, 024h, 010h, 000h, 00Fh, 084h
        db  095h, 000h, 000h, 000h, 08Bh, 0EBh, 083h, 0C3h, 008h, 08Bh, 045h, 004h, 029h, 044h, 024h, 010h
        db  08Dh, 070h, 0F8h, 0D1h, 0EEh, 08Bh, 0C6h, 04Eh, 00Bh, 0C0h, 074h, 072h, 08Bh, 045h, 000h, 050h
        db  08Bh, 044h, 024h, 01Ch, 050h, 0E8h, 096h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 066h, 08Bh, 013h
        db  066h, 08Bh, 0CAh, 081h, 0E1h, 0FFh, 00Fh, 000h, 000h, 066h, 0C1h, 0EAh, 00Ch, 003h, 0C8h, 08Bh
        db  0C2h, 025h, 0FFh, 0FFh, 000h, 000h, 083h, 0F8h, 005h, 077h, 058h, 0FFh, 024h, 085h, 0E0h, 010h
        db  000h, 000h, 033h, 0C0h, 066h, 08Bh, 001h, 0C1h, 0E0h, 010h, 003h, 0C7h, 0EBh, 020h, 08Bh, 0FFh
        db  001h, 039h, 0EBh, 020h, 033h, 0C0h, 066h, 08Bh, 001h, 0C1h, 0E0h, 010h, 083h, 0C3h, 002h, 033h
        db  0D2h, 066h, 08Bh, 013h, 04Eh, 003h, 0C2h, 08Dh, 084h, 007h, 000h, 080h, 000h, 000h, 0C1h, 0F8h
        db  010h, 066h, 089h, 001h, 083h, 0C3h, 002h, 08Bh, 0C6h, 04Eh, 00Bh, 0C0h, 075h, 08Eh, 083h, 07Ch
        db  024h, 010h, 000h, 00Fh, 085h, 06Bh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h
        db  0C4h, 004h, 0C3h, 0B8h, 0F9h, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h
        db  0B8h, 0F8h, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h, 08Dh, 049h, 000h
        db  0B8h, 0F8h, 0FFh, 0FFh
sub_5_00D4:
        call   ptr [ebp + 5fh]
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_5_00E0:
        xchg   esp, eax
        adc    byte ptr [eax], al
        db  000h, 062h, 010h
        add    byte ptr [eax], al
loc_5_00E8:
        rcl    byte ptr [eax], 0
        db  000h, 070h, 010h
        add    byte ptr [eax], al
loc_5_00F0:
        db  074h, 010h
        add    byte ptr [eax], al
        rcl    byte ptr [eax]
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 14h]
        push   eax
        mov    eax, dword ptr [esp + 28h]
        push   0
        mov    esi, dword ptr [esp + 28h]
        push   eax
        push   esi
        db  0E8h, 073h, 0FEh, 0FFh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_0132
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_0132:
        mov    ecx, dword ptr [esp + 28h]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_5_0160
        push   24h
        push   0
        db  0E8h, 0CAh, 008h, 000h, 000h
        mov    ecx, dword ptr [esp + 30h]
        add    esp, 8
        mov    dword ptr [ecx], eax
        or     eax, eax
        jne    SHORT loc_5_0160
        mov    eax, 0fffffffeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_5_0160:
        mov    ecx, dword ptr [esp + 28h]
        mov    eax, dword ptr [ecx]
        push   eax
        db  0E8h, 07Bh, 008h, 000h, 000h
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_0182
        mov    eax, 0fffffffeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_0182:
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        push   esi
        db  0E8h, 039h, 0FEh, 0FFh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [edi + 10h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 20h]
        sub    ebx, ebx
        push   eax
        push   esi
        db  0E8h, 005h, 0FEh, 0FFh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 18h], eax
        mov    ebp, eax
        cmp    dword ptr [edi + 10h], ebx
        jbe    SHORT loc_5_01F1
loc_5_01D8:
        mov    eax, dword ptr [ebp]
        push   eax
        push   esi
        db  0E8h, 0EEh, 0FDh, 0FFh, 0FFh
        add    esp, 8
        mov    dword ptr [ebp], eax
        inc    ebx
        add    ebp, 4
        cmp    dword ptr [edi + 10h], ebx
        ja     SHORT loc_5_01D8
loc_5_01F1:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        push   esi
        db  0E8h, 0D1h, 0FDh, 0FFh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 1ch], eax
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 1ch]
loc_5_020C:
        push   eax
        sub    ebx, ebx
        push   esi
        db  0E8h, 0BBh, 0FDh, 0FFh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 20h], eax
        mov    ebp, eax
        cmp    dword ptr [edi + 0ch], ebx
        db  076h, 019h
        mov    eax, dword ptr [ebp]
        push   eax
        push   esi
        db  0E8h, 0A4h, 0FDh, 0FFh, 0FFh
loc_5_022C:
        add    esp, 8
        mov    dword ptr [ebp], eax
        inc    ebx
        db  083h
_LDATA ends
        end

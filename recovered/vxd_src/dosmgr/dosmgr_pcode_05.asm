; dosmgr_pcode_05.asm -- DOSMGR object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_0000:
        mov    word ptr [ebp + 1ch], 4800h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        test   byte ptr [esi + 4], 8
        je     SHORT loc_5_0066
        movzx  eax, word ptr [esi]
        movzx  edx, word ptr [esi + 2]
        test   byte ptr [esi + 4], 2
        je     SHORT loc_5_0025
        dec    edx
loc_5_0025:
        mov    word ptr [ebp + 38h], ax
        mov    word ptr [ebp + 1ch], 4a00h
        mov    word ptr [ebp + 10h], dx
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4800h
        mov    word ptr [ebp + 10h], 0ffffh
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 4800h
        mov    eax, 21h
loc_5_0060:
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_5_0066:
        mov    dword ptr [esi + 8], 0
        test   byte ptr [esi + 4], 2
        je     SHORT loc_5_00A7
        push   0
        push   10h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_5_0172
        mov    dword ptr [esi + 8], eax
        mov    edi, eax
        push   esi
        movzx  ecx, word ptr [esi]
        movzx  esi, word ptr [esi + 2]
        add    esi, ecx
        dec    esi
        shl    esi, 4
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 4
        cld
        rep movsd
        pop    esi
loc_5_00A7:
        movzx  eax, word ptr [esi]
        movzx  ecx, word ptr [esi + 2]
        shr    eax, 8
        shr    ecx, 8
        push   ecx
        push   eax
        db  0CDh, 020h, 000h, 000h, 006h, 000h
        cmp    eax, 30ah
        pop    ecx
        pop    edx
        push   ebx
        push   ecx
loc_5_00C5:
        push   edx
        jb     SHORT loc_5_00F7
        xor    ebx, ebx
loc_5_00CA:
        db  0CDh, 020h, 010h, 000h, 006h, 000h
        test   ah, 10h
        je     SHORT loc_5_00F7
        test   ax, 280h
        jne    NEAR PTR loc_5_0178
        test   ah, 1
        je     NEAR PTR loc_5_0178
        inc    ecx
        dec    edx
loc_5_00EA:
        jne    SHORT loc_5_00CA
        pop    edx
        pop    ecx
        pop    ebx
        or     word ptr [esi + 4], 200h
        jmp    SHORT loc_5_0114
loc_5_00F7:
        cmp    edx, dword ptr [esp]
        jne    SHORT loc_5_0178
        pop    edx
        pop    ecx
        pop    ebx
        push   edx
        push   0
        push   0
        push   edx
        push   ecx
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        pop    edx
        or     eax, eax
        je     SHORT loc_5_017B
loc_5_0114:
        and    byte ptr [esi + 4], 0feh
        movzx  eax, word ptr [esi]
        dec    eax
        shl    eax, 4
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        add    eax, dword ptr [ebx + 4]
        pop    ebx
        cmp    byte ptr [eax], 5ah
        jne    SHORT loc_5_0133
        or     byte ptr [esi + 4], 1
loc_5_0133:
        pop    ecx
        add    esi, 10h
        dec    ecx
        db  00Fh, 085h, 07Fh, 0FEh, 0FFh, 0FFh
        db  0BEh, 014h, 002h, 000h, 000h
        db  08Bh, 00Dh, 014h, 003h, 000h, 000h
        or     ecx, ecx
        db  074h, 071h
loc_5_014D:
        test   byte ptr [esi + 5], 1
        jne    SHORT loc_5_016B
        mov    dx, word ptr [esi]
        mov    word ptr [ebp + 38h], dx
        mov    word ptr [ebp + 1ch], 4900h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
loc_5_016B:
        add    esi, 10h
        loop   loc_5_014D
        db  0EBh, 04Ch
loc_5_0172:
        or     byte ptr [esi + 5], 1
        jmp    SHORT loc_5_0133
loc_5_0178:
        pop    edx
        pop    ecx
        pop    ebx
loc_5_017B:
        pop    ecx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        jmp    SHORT loc_5_020B
        db  051h, 00Fh, 0B7h, 006h, 00Fh, 0B7h, 04Eh, 002h, 0C1h, 0E8h, 008h, 0C1h, 0E9h, 008h, 051h, 050h
        db  0CDh, 020h, 000h, 000h, 006h, 000h, 03Dh, 00Ah, 003h, 000h, 000h, 058h, 059h, 072h, 006h, 0CDh
        db  020h, 013h, 000h, 006h, 000h, 083h, 0C6h, 010h, 059h, 0E2h
DOSMGR_MMGR_PSP_Change_Notifier:
        aad    0f6h
        db  005h, 010h, 002h, 000h, 000h
loc_5_01C4:
        add    al, 74h
        db  02Dh, 0F6h, 005h, 010h, 002h
        add    byte ptr [eax], al
        add    dh, byte ptr [ebp + ecx - 75h]
        db  015h, 000h, 000h, 000h, 000h
        or     byte ptr [edx + ebx + 1], 4
        jmp    SHORT loc_5_01F4
        db  066h, 0C7h, 045h, 01Ch, 003h, 058h, 066h, 0C7h, 045h, 010h, 000h, 000h, 0B8h, 021h, 000h, 000h
        db  000h, 0CDh, 020h, 084h, 000h, 001h, 000h
loc_5_01F4:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        clc
        ret
loc_5_020B:
        db  0CDh, 020h, 0BFh, 000h, 001h, 000h
        add    byte ptr [eax], al
        db  000h, 0A1h, 0D0h, 001h, 000h, 000h
        shr    eax, 10h
        mov    eax, dword ptr [eax + 22h]
loc_5_021F:
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        mov    edx, eax
        mov    eax, dword ptr [eax]
        cmp    ax, -1
        jne    SHORT loc_5_021F
        db  089h, 015h, 07Ch, 001h, 000h, 000h
        db  0A3h, 080h, 001h, 000h, 000h
        ret
        db  053h, 057h, 056h, 0CDh, 020h, 082h, 000h, 001h, 000h, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh
        db  06Bh, 008h, 066h, 0C7h, 045h, 01Ch, 006h, 033h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h
        db  000h, 001h, 000h, 08Ah, 045h, 015h, 0A8h, 010h, 074h, 013h, 0CDh, 020h, 000h, 000h, 006h, 000h
        db  03Dh, 00Ah, 003h, 000h, 000h, 072h, 006h, 0CDh, 020h, 011h, 000h, 006h, 000h, 066h, 0C7h, 045h
        db  01Ch, 000h, 010h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 08Bh, 045h
        db  01Ch, 0A2h, 09Ch, 001h, 000h, 000h, 0CDh, 020h, 0BBh, 000h, 001h, 000h, 080h, 0FCh, 007h, 00Fh
        db  082h, 057h, 003h, 000h, 000h, 066h, 0C7h, 045h, 01Ch, 000h, 034h, 0B8h, 021h, 000h, 000h, 000h
        db  0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h, 045h, 038h, 0C1h, 0E0h, 004h, 00Fh, 0B7h, 05Dh
        db  010h, 04Bh, 003h, 0C3h, 0A3h, 000h, 002h, 000h, 000h, 066h, 0C7h, 045h, 01Ch, 000h, 052h, 0B8h
        db  021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h, 045h, 038h, 0A3h, 090h
        db  001h, 000h, 000h, 0C1h, 0E0h, 004h, 00Fh, 0B7h, 05Dh, 010h, 0A3h, 08Ch, 001h, 000h, 000h, 003h
        db  0C3h, 0A3h, 004h, 002h, 000h, 000h, 066h, 0C7h, 045h, 01Ch, 007h, 016h, 066h, 0C7h, 045h, 010h
        db  015h, 000h, 033h, 0C0h, 066h, 089h, 045h, 014h, 066h, 0C7h, 045h, 018h, 000h, 000h, 066h, 089h
        db  045h, 038h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h, 045h
        db  014h, 00Bh, 0C0h, 075h, 005h, 0B8h, 070h, 000h, 000h, 000h, 066h, 0A3h, 02Ah, 021h, 000h, 000h
        db  00Fh, 0B7h, 075h, 038h, 0C1h, 0E6h, 004h, 00Fh, 0B7h, 045h, 010h, 003h, 0F0h, 0CDh, 020h, 001h
        db  000h, 001h, 000h, 003h, 073h, 004h, 0BFh, 074h, 01Fh, 000h, 000h, 0B9h, 00Eh, 000h, 000h, 000h
        db  0FCh, 0F3h, 0A4h, 066h, 0C7h, 045h, 01Ch, 007h, 016h, 066h, 0C7h, 045h, 010h, 015h, 000h, 066h
        db  0C7h, 045h, 018h, 004h, 000h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h
        db  066h, 081h, 07Dh, 01Ch, 07Ch, 0B9h, 075h, 011h, 066h, 081h, 07Dh, 014h, 0ABh, 0A2h, 075h, 009h
        db  00Fh, 0B7h, 045h, 010h, 0A3h, 084h, 01Fh, 000h, 000h, 066h, 0C7h, 045h, 01Ch, 007h, 016h, 066h
        db  0C7h, 045h, 010h, 015h, 000h, 066h, 0C7h, 045h, 018h, 001h, 000h, 00Fh, 0B7h, 005h, 082h, 01Fh
        db  000h, 000h, 089h, 045h, 014h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h
        db  066h, 081h, 07Dh, 01Ch, 07Ch, 0B9h, 075h, 017h, 066h, 081h, 07Dh, 014h, 0ABh, 0A2h, 075h, 00Fh
        db  00Fh, 0B7h, 045h, 010h, 0A3h, 09Dh, 001h, 000h, 000h, 0FEh, 005h, 0A1h, 001h, 000h, 000h, 066h
        db  083h, 03Dh, 02Ah, 021h, 000h, 000h, 000h, 00Fh, 084h, 01Fh, 002h, 000h, 000h, 066h, 0C7h, 045h
        db  01Ch, 000h, 051h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 00Fh, 0B7h
        db  05Dh, 010h, 0C1h, 0E3h, 004h, 089h, 01Dh, 070h, 01Dh, 000h, 000h, 0CDh, 020h, 001h, 000h, 001h
        db  000h, 08Bh, 05Bh, 004h, 003h, 01Dh, 004h, 002h, 000h, 000h, 08Bh, 043h, 016h, 00Fh, 0B7h, 0C8h
        db  0C1h, 0E8h, 010h, 0C1h, 0E0h, 004h, 003h, 0C1h, 0A3h, 020h, 000h, 000h, 000h, 00Fh, 0B6h, 043h
        db  021h, 0A3h, 028h, 000h, 000h, 000h, 08Bh, 043h, 022h, 0A3h, 02Ch, 021h
DOSMGR_Get_IndosPtr:
        add    byte ptr [eax], al
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        db  0A3h, 060h, 01Dh, 000h, 000h
        mov    eax, dword ptr [ebx + 4]
        movzx  ebx, word ptr [ebx + 6]
        shl    ebx, 4
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        db  0A3h, 088h, 001h, 000h, 000h
        db  0F6h, 005h, 084h, 01Fh, 000h, 000h, 002h
        jne    SHORT loc_5_04C8
        cmp    word ptr [eax + 2], -1
        jne    SHORT loc_5_0483
        db  0A3h, 03Ch, 020h, 000h, 000h
        db  08Bh, 01Dh, 004h, 002h, 000h, 000h
        mov    eax, dword ptr [ebx + 1ah]
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        jmp    SHORT loc_5_04C3
loc_5_0483:
        mov    esi, eax
        db  039h, 01Dh, 08Ch, 001h, 000h, 000h
        jne    SHORT loc_5_04A3
        movzx  ebx, word ptr [eax + 2]
        mov    eax, dword ptr [eax]
        shl    ebx, 4
        movzx  ecx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, ecx
        jmp    SHORT loc_5_0483
loc_5_04A3:
        cmp    word ptr [esi], -1
        je     SHORT loc_5_04BD
        mov    esi, dword ptr [esi]
        mov    ecx, esi
        db  081h, 0E1h, 0FFh, 0FFh, 000h
sub_5_04B2:
        add    cl, al
        out    dx, al
        adc    cl, al
        out    4, al
        add    esi, ecx
        jmp    SHORT loc_5_04A3
loc_5_04BD:
        db  089h, 035h, 03Ch, 020h, 000h, 000h
loc_5_04C3:
        db  0A3h, 064h, 01Dh, 000h, 000h
loc_5_04C8:
        db  0F6h, 005h, 084h, 01Fh, 000h, 000h, 008h
        jne    SHORT loc_5_0508
        mov    word ptr [ebp + 1ch], 5d06h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 3ch]
        shl    eax, 4
        movzx  ebx, word ptr [ebp + 4]
        add    eax, ebx
        movzx  edx, word ptr [ebp + 14h]
        movzx  ecx, word ptr [ebp + 18h]
        db  0A3h, 000h, 020h, 000h, 000h
        db  089h, 015h, 004h, 020h, 000h, 000h
        db  089h, 00Dh, 05Ch, 01Dh, 000h, 000h
loc_5_0508:
        db  0F6h, 005h, 084h, 01Fh, 000h, 000h, 008h
        jne    SHORT loc_5_0561
        push   0
        db  068h, 0F8h, 01Fh, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 0E2h, 000h, 000h, 000h
        db  0A1h, 000h, 020h, 000h, 000h
        db  003h, 005h, 004h, 020h, 000h, 000h
        db  0A3h, 014h, 020h, 000h, 000h
        db  0A1h, 05Ch, 01Dh, 000h, 000h
        db  02Bh, 005h, 004h, 020h, 000h, 000h
loc_5_0544:
        db  0A3h, 018h, 020h, 000h, 000h
        push   0
loc_5_054B:
        db  068h, 00Ch, 020h, 000h, 000h
        db  0CDh, 020h, 0A6h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        db  00Fh, 084h, 0AAh, 000h, 000h, 000h
loc_5_0561:
        db  0E8h, 02Fh, 001h, 000h, 000h
        db  00Fh, 082h, 085h, 000h, 000h, 000h
        mov    eax, 58h
        db  0A3h, 024h, 000h, 000h, 000h
        db  0F7h, 025h, 028h, 000h, 000h, 000h
        db  0A3h, 02Ch, 000h, 000h, 000h
        db  0F6h, 005h, 084h, 01Fh, 000h, 000h, 001h
        db  075h, 023h
        db  0A3h, 02Ch, 020h, 000h, 000h
        db  0A1h, 020h, 000h, 000h, 000h
        db  0A3h, 028h, 020h, 000h, 000h
        push   0
        db  068h, 020h
_PTEXT ends
        end

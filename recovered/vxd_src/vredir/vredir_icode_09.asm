; vredir_icode_09.asm -- VREDIR object 9 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

        db  000h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
sub_9_0010:
        db  0B8h, 000h, 000h, 000h, 000h
        mov    ecx, 10h
loc_9_001A:
        mov    dword ptr [eax + 4], eax
        mov    dword ptr [eax], eax
        add    eax, 8
        dec    ecx
        jne    SHORT loc_9_001A
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_9_0030:
        push   ebx
        push   esi
        push   edi
        db  0CDh, 020h, 000h, 000h, 08Bh, 004h
        jb     SHORT loc_9_0059
        cmp    eax, 100h
        jne    SHORT loc_9_0059
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 0
        db  0CDh, 020h, 001h, 000h, 08Bh, 004h
        jb     SHORT loc_9_0059
        db  0A2h, 000h, 000h, 000h, 000h
loc_9_0059:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  0CCh, 0CCh, 0CCh
sub_9_0060:
        xor    cx, cx
        push   ebp
        mov    ebp, esp
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        mov    si, 1
loc_9_0070:
        add    cx, cx
        inc    cx
        movzx  edx, cx
        add    si, si
        cmp    edx, eax
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        jbe    SHORT loc_9_0070
        mov    ax, 0ffffh
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        sub    ax, si
        test   ecx, ecx
        db  066h, 089h, 035h, 000h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        je     SHORT loc_9_00DD
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 0
        db  089h, 015h, 000h, 000h, 000h, 000h
        jbe    SHORT loc_9_00D1
loc_9_00B4:
        lea    eax, [esi + 1]
        db  089h, 014h, 0B5h, 000h, 000h, 000h, 000h
        mov    word ptr [edx], ax
        inc    esi
        lea    eax, [edx + 4bch]
        cmp    esi, ecx
        mov    dword ptr [edx + 38h], eax
        mov    edx, eax
        jb     SHORT loc_9_00B4
loc_9_00D1:
        mov    dword ptr [edx - 484h], 0
        jmp    SHORT loc_9_00E7
loc_9_00DD:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_9_00E7:
        xor    eax, eax
        push   28h
        db  066h, 0A1h, 022h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [eax + eax*4]
        lea    eax, [ecx + ecx*4]
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0106:
        add    esp, 8
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h
sub_9_0110:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   0
        push   100h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h
        add    esp, 8
        mov    edi, eax
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        movsd
        movsd
        movsd
        movsd
        shr    eax, 4
        mov    word ptr [ebp + 3ch], ax
        mov    word ptr [ebp + 14h], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0153:
        mov    word ptr [ebp + 18h], ax
        mov    word ptr [ebp + 1ch], 5e01h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        push   ebx
        db  0CDh, 020h, 000h, 000h, 032h, 000h
        jb     SHORT loc_9_019B
        mov    ax, 3fh
        db  0BEh, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 032h, 000h
loc_9_019B:
        pop    ebx
        ret
VREDIR_SYS_CRITICAL_INIT:
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        test   edx, edx
        jns    SHORT loc_9_01AE
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
loc_9_01AE:
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        cmp    ah, 4
        jb     SHORT loc_9_01CD
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h
loc_9_01CD:
        db  0CDh, 020h, 000h, 000h, 080h, 004h
        jb     SHORT loc_9_0210
        cmp    eax, 86h
        jne    SHORT loc_9_0212
        db  0CDh, 020h, 000h, 000h, 014h, 000h
        jb     SHORT loc_9_0224
        cmp    eax, 30ah
        jb     SHORT loc_9_0239
        db  0CDh, 020h, 000h, 000h, 087h, 004h
        jb     SHORT loc_9_01F9
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_9_01F9:
        db  0CDh, 020h, 000h, 000h, 09Bh, 004h
        jb     SHORT loc_9_020E
        cmp    eax, 120h
        jb     SHORT loc_9_020E
        db  0FFh, 005h, 000h, 000h, 000h, 000h
loc_9_020E:
        clc
        ret
loc_9_0210:
        stc
        ret
loc_9_0212:
        push   eax
        push   22h
        push   17dch
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_021F:
        add    esp, 0ch
        jmp    SHORT loc_9_024C
loc_9_0224:
        push   0
        push   0
        push   0
        push   17ddh
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0234:
        add    esp, 10h
        jmp    SHORT loc_9_024C
loc_9_0239:
        push   eax
        push   30ah
        push   17deh
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0249:
        add    esp, 0ch
loc_9_024C:
        stc
        ret
        db  056h, 052h, 065h, 064h, 069h, 072h, 000h
sub_9_0255:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 58h
        mov    byte ptr [ebp + 1ch], 0d4h
        mov    word ptr [ebp + 18h], 0
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        sub    ebx, ebx
        movzx  eax, byte ptr [ebp + 1ch]
        movzx  edx, word ptr [ebp + 14h]
        movzx  ecx, word ptr [ebp + 18h]
        cmp    cx, 3fcfh
        jne    SHORT loc_9_02E6
        or     eax, eax
        je     SHORT loc_9_02E6
        cmp    edx, 2
        jne    SHORT loc_9_02E6
        movzx  eax, word ptr [ebp + 10h]
        test   eax, 1
        je     SHORT loc_9_02E6
        movzx  eax, byte ptr [ebp + 1dh]
        cmp    eax, 0
        jne    SHORT loc_9_02DB
        mov    byte ptr [ebp + 1dh], 58h
        mov    byte ptr [ebp + 1ch], 0d3h
        mov    eax, 2fh
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  ecx, word ptr [ebp + 38h]
        shl    ecx, 4
        movzx  ebx, word ptr [ebp + 10h]
        add    ebx, ecx
        movzx  edx, word ptr [ebp + 14h]
        add    edx, ecx
loc_9_02DB:
        db  0B8h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 082h, 004h
loc_9_02E6:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        or     ebx, ebx
        je     SHORT loc_9_030B
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
loc_9_030B:
        ret
VREDIR_DEVICE_INIT:
        db  0CDh, 020h, 001h, 000h, 080h, 004h
        jb     NEAR PTR loc_9_05B6
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0322:
        db  0CDh, 020h, 003h, 000h, 014h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 007h, 000h, 014h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_9_0255
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_0348:
        test   ax, ax
        jne    NEAR PTR loc_9_05C9
        call   sub_9_0110
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, 400024h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        push   ebx
        push   ecx
        push   edx
        db  00Fh, 0B7h, 01Dh, 00Ch, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 048h, 000h
        or     eax, eax
        je     SHORT loc_9_03A8
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0CDh, 020h, 001h, 000h, 048h, 000h
        add    esp, 18h
loc_9_03A8:
        or     eax, eax
        je     NEAR PTR loc_9_05AD
        db  0A3h, 000h, 000h, 000h, 000h
        db  00Fh, 0B7h, 01Dh, 010h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 012h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 014h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 016h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 018h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 01Ah, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 01Ch, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 01Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 020h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   2
        push   0
        push   esp
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 022h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 024h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 026h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 028h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 02Ah, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 02Ch, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 02Eh, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 030h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 032h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
        db  00Fh, 0B7h, 01Dh, 034h, 000h, 000h, 000h
        db  003h, 01Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 036h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 015h, 038h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   0
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 003h, 000h, 048h, 000h
        add    esp, 24h
loc_9_05AD:
        pop    edx
        pop    ecx
        pop    ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_05B5:
        ret
loc_9_05B6:
        push   0
        push   0
        push   0
        push   17d4h
        db  0E8h, 000h, 000h, 000h, 000h
loc_9_05C6:
        add    esp, 10h
loc_9_05C9:
        stc
        ret
        db  090h
loc_9_05CC:
        xor    esi, esi
        ret
        db  090h, 053h, 056h, 050h, 052h, 083h, 03Dh, 000h, 000h, 000h, 000h, 000h, 074h, 025h, 0BAh, 005h
        db  052h, 000h, 000h, 0BBh, 00Eh, 000h, 000h, 000h, 033h, 0C0h, 0BEh, 000h, 000h, 000h, 000h, 0CDh
        db  020h, 001h, 000h, 087h, 004h, 0FCh, 023h, 0C0h, 075h, 00Fh, 052h, 0E8h, 000h, 000h, 000h, 000h
        db  083h, 0C4h, 004h, 0F8h, 05Ah, 058h, 05Eh, 05Bh, 0C3h, 03Dh, 0A0h, 000h, 000h, 000h, 075h, 0EAh
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0EBh, 0DEh
sub_9_061B:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 032h, 000h
        pop    ebx
        sbb    eax, eax
        not    eax
        ret
_ITEXT ends
        end

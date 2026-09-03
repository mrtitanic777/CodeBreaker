; vmm_pcode_18.asm -- VMM object 18 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_18_0000:
        clc
        pushfd
        pushad
        mov    ebx, dword ptr [edi + 14h]
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_18_0023
        test   byte ptr [edi + 1], 4
        jne    SHORT loc_18_0017
        dec    dword ptr [ebx - 6ch]
loc_18_0017:
        mov    ebp, dword ptr [edi + 10h]
        test   byte ptr [ebp + 2dh], 2
        jne    SHORT loc_18_0023
        dec    byte ptr [ebx - 68h]
loc_18_0023:
        call   sub_18_00D8
        call   sub_18_004C
loc_18_002D:
        call   sub_18_0070
loc_18_0032:
        db  0E8h, 00Ah, 081h, 0EEh, 0FFh
loc_18_0037:
        xor    eax, eax
        mov    dword ptr [edi + 0ch], eax
        sub    edi, 5ch
        push   eax
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_18_0049:
        popad
        popfd
        ret
sub_18_004C:
        lea    edx, [edi + 6ch]
        jmp    SHORT sub_18_0059
loc_18_0051:
        mov    esi, eax
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
sub_18_0059:
        cli
        mov    esi, dword ptr [edx]
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        jne    SHORT loc_18_0051
        mov    dword ptr [edx], eax
        sti
        db  0CDh, 020h, 09Ch, 080h, 001h, 000h
        add    byte ptr [eax], al
        db  000h
sub_18_0070:
        pushad
        push   100h
        push   0
        push   1
        push   dword ptr [edi + 50h]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        popad
        call   sub_18_008D
        ret
sub_18_008D:
        pushad
        mov    esi, dword ptr [edi + 48h]
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        push   0
        push   dword ptr [edi + 50h]
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        movzx  eax, word ptr [edi - 2eh]
        push   0
        push   eax
        db  0CDh, 020h, 077h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        movzx  eax, word ptr [edi - 30h]
        cmp    eax, 0
        je     SHORT loc_18_00D5
        push   ebx
        mov    ebx, dword ptr [edi + 14h]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 079h, 000h, 001h, 000h
        add    esp, 0ch
        xor    ax, ax
        mov    word ptr [edi - 30h], ax
        pop    ebx
loc_18_00D5:
        popad
        ret
        db  000h
sub_18_00D8:
        pushad
        pushfd
        cli
        lea    edx, [edi - 48h]
loc_18_00DE:
        mov    esi, dword ptr [edx]
        cmp    esi, edx
        je     SHORT loc_18_00EC
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        jmp    SHORT loc_18_00DE
loc_18_00EC:
        popfd
        popad
        ret
        db  000h
loc_18_00F0:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_18_0105:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
        db  08Bh, 044h, 024h, 004h, 0CDh, 020h, 026h, 000h, 001h, 000h, 0C2h, 004h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
_PageFree:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        mov    edi, dword ptr [esp + 24h]
        test   edi, 0fffh
        jne    NEAR PTR loc_18_025B
        shr    edi, 0ch
        push   edi
        db  0E8h, 05Eh, 037h, 0EEh, 0FFh
        mov    dword ptr [esp + 14h], eax
        lea    eax, [esp + 10h]
        push   eax
        push   1
        push   edi
        db  0E8h, 08Dh, 037h, 0EEh, 0FFh
        mov    ebx, eax
        cmp    ebx, esi
        je     NEAR PTR loc_18_0246
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, eax
        shr    ecx, 0ch
        cmp    ecx, edi
        jne    NEAR PTR loc_18_0246
        test   al, 10h
        je     SHORT loc_18_018C
        test   byte ptr [esp + 28h], 10h
        je     NEAR PTR loc_18_0246
loc_18_018C:
        mov    esi, dword ptr [ebx]
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        push   edi
        db  0E8h, 054h, 020h, 0EEh, 0FFh
        mov    dword ptr [esp + 18h], eax
        lea    ebp, [esi*4]
        add    ebp, dword ptr [esp + 10h]
        cmp    ebp, dword ptr [esp + 10h]
        jbe    SHORT loc_18_01C5
loc_18_01AF:
        push   0
        push   dword ptr [esp + 14h]
        db  0E8h, 076h, 039h, 0EEh, 0FFh
        add    dword ptr [esp + 10h], 4
        cmp    ebp, dword ptr [esp + 10h]
        ja     SHORT loc_18_01AF
loc_18_01C5:
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        db  0E8h, 08Ah, 020h, 0EEh, 0FFh
        push   ebx
        lea    ebx, [edi*4]
        push   dword ptr [esp + 18h]
        call   sub_18_0265
        sub    ebx, 800000h
        shr    ebx, 0ch
        add    esi, edi
        lea    edx, [esi*4]
        sub    edx, 800004h
        shr    edx, 0ch
        sub    edx, ebx
        inc    edx
        mov    ecx, dword ptr [eax + 4]
        shr    ecx, 0ch
        and    edi, 0fffffc00h
        cmp    edi, ecx
        jae    SHORT loc_18_0213
        inc    ebx
        dec    edx
loc_18_0213:
        mov    eax, dword ptr [eax]
        add    eax, ecx
        add    esi, 3ffh
        and    esi, 0fffffc00h
        cmp    eax, esi
        jae    SHORT loc_18_0228
        dec    edx
loc_18_0228:
        or     edx, edx
        jle    SHORT loc_18_023C
        push   20000000h
        push   edx
        push   ebx
        db  0FFh, 015h, 0A4h, 0D7h, 000h, 000h
        add    esp, 0ch
loc_18_023C:
        mov    esi, 1
        db  0E8h, 0C8h, 034h, 0EEh, 0FFh
loc_18_0246:
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        db  0E8h, 077h, 01Dh, 0EEh, 0FFh
loc_18_025B:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_18_0265:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    edi, dword ptr [eax + 4]
        and    edi, 0fffff000h
        mov    dword ptr [eax + 4], edi
        mov    bx, word ptr [eax + 8]
        mov    cx, word ptr [eax + 0ah]
        movzx  esi, bx
        imul   esi, esi, 0ch
        db  08Bh, 015h, 0ECh, 0FAh, 000h, 000h
        mov    word ptr [esi + edx + 0ah], cx
        movzx  ecx, cx
        imul   ecx, ecx, 0ch
        mov    word ptr [edx + ecx + 8], bx
        mov    ecx, dword ptr [esp + 14h]
        shr    edi, 0ch
        mov    ebx, dword ptr [ecx + 8]
loc_18_02A7:
        mov    esi, ebx
        mov    ebx, dword ptr [ebx + 8]
        mov    ecx, dword ptr [ebx + 4]
        shr    ecx, 0ch
        cmp    ecx, edi
        jb     SHORT loc_18_02A7
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [eax + 8], ebx
        mov    ecx, dword ptr [eax]
        mov    ebp, dword ptr [ebx + 4]
        shr    ebp, 0ch
        sub    ebp, ecx
        cmp    ebp, edi
        jne    SHORT loc_18_02DE
        mov    ebp, dword ptr [ebx]
        add    ebp, ecx
        mov    dword ptr [eax], ebp
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [edx - 4]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [edx - 4], ebx
loc_18_02DE:
        mov    ebx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        shr    ecx, 0ch
        add    ecx, ebx
        cmp    ecx, edi
        jne    SHORT loc_18_0302
        mov    ecx, dword ptr [eax]
        add    ecx, ebx
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esi + 8], ecx
        mov    ecx, dword ptr [edx - 4]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx - 4], eax
        mov    eax, esi
loc_18_0302:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_18_0309:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 1
        mov    esi, dword ptr [esp + 1ch]
        or     esi, esi
        jne    SHORT loc_18_0399
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [eax + 0ch]
        push   5
        movzx  eax, word ptr [eax + 8]
        imul   eax, eax, 0ch
        db  08Bh, 03Dh, 0ECh, 0FAh, 000h, 000h
        lea    esi, [eax + edi]
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 8
        mov    ebx, dword ptr [eax + 0ch]
        cmp    esi, ebx
        je     SHORT loc_18_0384
loc_18_035C:
        movzx  eax, word ptr [esi + 8]
        imul   eax, eax, 0ch
        lea    ebp, [edi + eax]
        push   10h
        mov    eax, dword ptr [esi + 4]
        and    eax, 0fffff000h
        push   eax
        db  0FFh, 015h, 07Ch, 0D4h, 000h, 000h
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        mov    esi, ebp
        cmp    ebp, ebx
        jne    SHORT loc_18_035C
loc_18_0384:
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        jmp    NEAR PTR loc_18_04A7
loc_18_0399:
        cmp    dword ptr [esi + 10h], 44434443h
        jne    NEAR PTR loc_18_049F
        db  039h, 035h, 03Ch, 0F8h, 000h, 000h
        je     NEAR PTR loc_18_049F
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0F0h, 0F8h, 000h, 000h
        add    esp, 8
        mov    ebx, dword ptr [esi + 8]
        mov    dword ptr [esi + 8], 0fffffbadh
        db  08Bh, 03Dh, 0ECh, 0FAh, 000h, 000h
        mov    eax, dword ptr [ebx + 8]
        mov    ecx, dword ptr [edi - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebx + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [edi - 4], eax
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi - 4], eax
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi - 4], eax
        db  0F6h, 005h, 030h, 0F9h, 000h, 000h, 008h
        jne    SHORT loc_18_0412
        push   dword ptr [ebx + 10h]
        db  0FFh, 015h, 030h, 0D7h, 000h, 000h
        add    esp, 4
loc_18_0412:
        push   ebx
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 032h, 010h, 0EEh, 0FFh
        db  03Bh, 035h, 060h, 0F8h, 000h, 000h
        jne    SHORT loc_18_0438
        mov    eax, dword ptr [esi + 0ch]
        db  0A3h, 060h, 0F8h, 000h, 000h
        db  0C7h, 005h, 08Ch, 0EEh, 000h, 000h, 001h, 000h, 000h, 000h
loc_18_0438:
        db  0B8h, 004h, 0FBh, 000h, 000h
loc_18_043D:
        cmp    dword ptr [eax], esi
        jne    SHORT loc_18_0447
        mov    dword ptr [eax], 0
loc_18_0447:
        add    eax, 8
        db  03Dh, 004h, 0FCh, 000h, 000h
        jb     SHORT loc_18_043D
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        cmp    dword ptr [eax + 0ch], esi
        je     SHORT loc_18_0463
loc_18_045B:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], esi
        jne    SHORT loc_18_045B
loc_18_0463:
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_18_047B
        push   eax
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 0D5h, 00Fh, 0EEh, 0FFh
loc_18_047B:
        mov    dword ptr [esi + 10h], 0fffffffdh
        push   esi
        db  0FFh, 035h, 0E8h, 0F7h, 000h, 000h
        db  0E8h, 0C2h, 00Fh, 0EEh, 0FFh
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        jmp    SHORT loc_18_04A7
loc_18_049F:
        mov    dword ptr [esp + 10h], 0
loc_18_04A7:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  000h
sub_18_04B4:
        xor    eax, eax
        db  039h, 005h, 0A8h, 0F8h, 000h, 000h
        jne    SHORT loc_18_052E
        test   byte ptr [edi + 3], 20h
        je     SHORT loc_18_04DD
        mov    eax, dword ptr [edi - 5ch]
        mov    byte ptr [eax + 3], 0
        db  08Bh, 035h, 038h, 0F7h, 000h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        db  0FFh, 005h, 034h, 0F7h, 000h, 000h
loc_18_04DD:
        db  0A1h, 030h, 0F7h, 000h, 000h
        db  03Bh, 005h, 02Ch, 0F7h, 000h, 000h
        jbe    SHORT loc_18_04F3
        db  083h, 03Dh, 034h, 0F7h, 000h, 000h, 004h
        ja     SHORT loc_18_0541
loc_18_04F3:
        push   ecx
        shr    ecx, 0ch
        lea    eax, [ecx*4 - 800000h]
        shr    eax, 0ch
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        db  0F7h, 005h, 016h, 005h, 000h, 000h, 000h, 000h, 040h, 000h
        je     SHORT loc_18_0522
        sub    ecx, 1000h
loc_18_0522:
        push   10h
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_18_052D:
        ret
loc_18_052E:
        shr    ecx, 0ch
        inc    ecx
        push   eax
        push   1
        push   ecx
        db  0CDh, 020h, 01Fh, 001h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_18_052D
loc_18_0541:
        db  0A0h, 0B7h, 005h, 000h, 000h
        db  08Bh, 035h, 038h, 0F7h, 000h, 000h
        db  08Bh, 015h, 098h, 0F6h, 000h, 000h
        mov    eax, dword ptr [edx - 5ch]
        bts    dword ptr [edx], 1dh
        jb     SHORT loc_18_0570
        db  08Bh, 035h, 038h, 0F7h, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        inc    byte ptr [eax + 3]
        db  0FFh, 00Dh, 034h, 0F7h, 000h, 000h
loc_18_0570:
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     NEAR PTR loc_18_04F3
        inc    byte ptr [eax + 3]
        push   eax
        push   ecx
        mov    ecx, dword ptr [eax + 4]
        shr    ecx, 0ch
        push   20000000h
        push   1
        push   ecx
        db  0CDh, 020h, 01Fh, 001h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        db  0A0h, 0B7h, 005h, 000h, 000h
        db  0FFh, 00Dh, 034h, 0F7h, 000h, 000h
        db  0FFh, 00Dh, 030h, 0F7h, 000h, 000h
        pop    eax
        db  08Bh, 035h, 03Ch, 0F7h, 000h, 000h
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
loc_18_05B7:
        jmp    NEAR PTR loc_18_04F3
loc_18_05BC:
        db  0BEh, 024h, 0F7h, 000h, 000h
        xor    eax, eax
        mov    dword ptr [esi], eax
        push   esi
        mov    edi, edx
        push   edi
        db  0E8h, 0DDh, 006h, 0EEh, 0FFh
        mov    eax, 20h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    edi
        mov    eax, 21h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    esi
        dec    dword ptr [esi + 4]
        push   esi
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        pop    ecx
loc_18_05F1:
        ret
loc_18_05F2:
        db  0CDh, 020h, 00Ch, 001h, 001h, 000h
        jb     SHORT loc_18_05F1
        db  0CDh, 020h, 00Eh, 001h, 001h, 000h
        je     SHORT loc_18_05F1
        db  0CDh, 020h, 009h, 001h, 001h, 000h
        jne    SHORT loc_18_05F1
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        push   edi
        mov    eax, 1fh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        pop    edx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        mov    esi, esp
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        add    esp, 6ch
        db  0E8h, 06Bh, 073h, 0EEh, 0FFh
        db  0BEh, 024h, 0F7h, 000h, 000h
        inc    dword ptr [esi + 4]
loc_18_064A:
        cmp    dword ptr [esi], 0
        je     SHORT loc_18_065C
        push   0
        push   esi
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    SHORT loc_18_064A
loc_18_065C:
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        mov    eax, 1000h
        mov    ecx, 2
        db  0BEh, 0BCh, 005h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  089h, 035h, 024h, 0F7h, 000h, 000h
        db  0E8h, 036h, 087h, 0EEh, 0FFh
        db  0E8h, 0A6h, 083h, 0EEh, 0FFh
        ret
_PTEXT ends
        end

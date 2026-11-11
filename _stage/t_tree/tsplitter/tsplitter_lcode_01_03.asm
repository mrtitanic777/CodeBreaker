; tsplitter_lcode_01_03.asm -- TSPLITTER object 1 segment _LTEXT  (part 3/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0111:
        pushfd
        cli
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        popfd
        push   esi
        mov    esi, dword ptr [eax + 24h]
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        mov    esi, dword ptr [eax + 1ch]
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
        pop    esi
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        ret
Splitter_Service_00:
        mov    eax, 1
        clc
        ret
Splitter_Service_01:
        push   esi
        push   edi
        call   sub_1_00D1
        or     eax, eax
        jne    NEAR PTR loc_1_01E8
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_1_01CC
        mov    cl, byte ptr [ebx + 3]
        mov    byte ptr [eax + 1], cl
        mov    cl, byte ptr [ebx + 30h]
        mov    byte ptr [eax], cl
        sub    ecx, ecx
        mov    dword ptr [eax + 28h], ecx
        mov    dword ptr [eax + 20h], ecx
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 14h], 1
        cmp    byte ptr [ebx + 1], 0dh
        jne    SHORT loc_1_0186
        or     dword ptr [eax + 14h], 2
loc_1_0186:
        cld
        lea    edi, [eax + 2]
        lea    esi, [ebx + 1ah]
        mov    ecx, 10h
        rep movsb
        push   eax
        mov    eax, 5
        mov    ecx, 4
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        pop    eax
        jb     SHORT loc_1_01D8
        mov    dword ptr [eax + 24h], esi
        push   eax
        mov    eax, 5
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        pop    eax
        jb     SHORT loc_1_01CF
        mov    dword ptr [eax + 1ch], esi
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        pushfd
        cli
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        popfd
loc_1_01CC:
        pop    edi
        pop    esi
        ret
loc_1_01CF:
        mov    esi, dword ptr [eax + 24h]
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_1_01D8:
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        xor    eax, eax
        jmp    SHORT loc_1_01CC
loc_1_01E8:
        or     dword ptr [eax + 14h], 1
        jmp    SHORT loc_1_01CC
Splitter_Service_02:
        push   esi
        push   edi
        call   sub_1_00D1
        or     eax, eax
        je     SHORT loc_1_0247
        mov    edi, eax
        btr    dword ptr [edi + 14h], 0
        jae    SHORT loc_1_0237
        mov    esi, dword ptr [edi + 24h]
loc_1_0205:
        pushfd
        cli
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        popfd
        or     eax, eax
        je     SHORT loc_1_0237
        dec    dword ptr [edi + 28h]
        pushad
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 17h
        mov    byte ptr [ebx + 31h], 17h
        cmp    dword ptr [ebx + 2ch], 0
        je     SHORT loc_1_022E
        mov    eax, 17h
        call   dword ptr [ebx + 2ch]
loc_1_022E:
        popad
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        jmp    SHORT loc_1_0205
loc_1_0237:
        mov    eax, edi
        test   dword ptr [eax + 14h], 2
        jne    SHORT loc_1_024D
        call   sub_1_0111
loc_1_0247:
        db  0CDh, 020h, 002h, 000h, 014h, 000h
loc_1_024D:
        xor    eax, eax
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 31h], al
        pop    edi
        pop    esi
        ret
Splitter_Service_03:
        push   esi
        db  0A3h, 0F4h, 005h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_027C
        mov    eax, 140002h
        db  0BEh, 09Fh, 002h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        jb     SHORT loc_1_0295
        db  089h, 035h, 0FCh, 005h, 000h, 000h
        jmp    SHORT loc_1_0295
loc_1_027C:
        db  083h, 03Dh, 0FCh, 005h, 000h, 000h, 000h
        je     SHORT loc_1_0295
        mov    eax, 140002h
        db  0BEh, 09Fh, 002h, 000h, 000h
        db  0CDh, 020h, 01Ch, 001h, 001h, 000h
loc_1_0295:
        pop    esi
        ret
        db  0EBh, 006h, 0FFh, 025h, 000h, 006h, 000h, 000h
Splitter_Service_04:
        push   ecx
        push   edx
        push   esi
        push   edi
        push   eax
        cld
        sub    eax, eax
        mov    al, byte ptr [ebx]
        and    eax, 0ffffff7fh
        cmp    eax, 36h
        je     SHORT loc_1_02F1
        cmp    eax, 21h
        je     NEAR PTR loc_1_0372
        cmp    eax, 35h
        je     NEAR PTR loc_1_0466
        cmp    eax, 31h
        je     SHORT loc_1_0310
loc_1_02CA:
        clc
        pop    eax
loc_1_02CC:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        cld
        jb     SHORT loc_1_02E2
        db  083h, 03Dh, 0FCh, 005h, 000h, 000h, 000h
        je     SHORT loc_1_02E2
        db  0FFh, 025h, 0FCh, 005h, 000h, 000h
loc_1_02E2:
        ret
loc_1_02E3:
        sub    eax, eax
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 31h], al
        add    esp, 4
        stc
        jmp    SHORT loc_1_02CC
loc_1_02F1:
        db  0A1h, 0F4h, 005h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_02CA
        cmp    dword ptr [ebx + 2ch], eax
        je     SHORT loc_1_02CA
        push   ebx
        call   sub_1_00D1
        pop    ebx
        or     eax, eax
        je     SHORT loc_1_02CA
        or     dword ptr [eax + 14h], 2
        jmp    SHORT loc_1_02CA
loc_1_0310:
        push   ebx
        call   sub_1_00D1
        pop    ebx
        or     eax, eax
        je     SHORT loc_1_02CA
        mov    edi, eax
        btr    dword ptr [edi + 14h], 1
        jae    SHORT loc_1_0359
        mov    esi, dword ptr [edi + 1ch]
loc_1_0327:
        pushfd
        cli
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        popfd
        or     eax, eax
        je     SHORT loc_1_0359
        pushad
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 17h
        mov    byte ptr [ebx + 31h], 17h
        cmp    dword ptr [ebx + 2ch], 0
        je     SHORT loc_1_034D
        mov    eax, 17h
        call   dword ptr [ebx + 2ch]
loc_1_034D:
        popad
        dec    dword ptr [edi + 20h]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        jmp    SHORT loc_1_0327
loc_1_0359:
        mov    eax, edi
        test   dword ptr [eax + 14h], 1
        jne    NEAR PTR loc_1_02E3
        call   sub_1_0111
        jmp    NEAR PTR loc_1_02CA
loc_1_0372:
        push   ebx
        call   sub_1_00A5
        pop    ebx
        or     eax, eax
        je     NEAR PTR loc_1_02CA
        mov    edi, eax
        sub    edx, edx
        db  0A1h, 0F4h, 005h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0396
        cmp    dword ptr [ebx + 2ch], eax
        jne    SHORT loc_1_0396
        inc    edx
        jmp    SHORT loc_1_03B5
loc_1_0396:
        test   dword ptr [edi + 14h], 1
        jne    SHORT loc_1_03B5
        cmp    dword ptr [edi + 18h], 0
        jne    NEAR PTR loc_1_02CA
        mov    eax, edi
        call   sub_1_0111
        jmp    NEAR PTR loc_1_02CA
loc_1_03B5:
        mov    esi, dword ptr [edi + edx*8 + 1ch]
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_1_02CA
        mov    dword ptr [eax], ebx
        pushfd
        cli
        db  0CDh, 020h, 09Fh, 000h, 001h, 000h
        inc    dword ptr [edi + edx*8 + 20h]
        mov    ecx, dword ptr [edi + 18h]
        cmp    ecx, dword ptr [edi + edx*8 + 20h]
        jb     SHORT loc_1_03F0
        popfd
        mov    byte ptr [ebx + 1], 0ffh
        mov    byte ptr [ebx + 31h], 0ffh
        sub    eax, eax
        add    esp, 4
        stc
        jmp    NEAR PTR loc_1_02CC
loc_1_03F0:
        inc    dword ptr [edi + 18h]
        popfd
        mov    esi, eax
        push   edx
        push   ebx
        mov    ecx, 40h
        add    ecx, 5adh
        push   ecx
        push   0
        call   0
        add    esp, 8
        pop    ebx
        pop    edx
        or     eax, eax
        jne    SHORT loc_1_042E
        mov    eax, esi
        mov    esi, dword ptr [edi + edx*8 + 1ch]
        pushfd
        cli
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        popfd
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_02CA
loc_1_042E:
        db  0BFh, 0E6h, 004h, 000h, 000h
        mov    dword ptr [eax + 2ch], edi
        mov    cl, byte ptr [ebx]
        mov    byte ptr [eax], cl
        or     byte ptr [eax], 80h
        mov    word ptr [eax + 8], 5adh
        mov    edi, eax
        add    edi, 40h
        mov    dword ptr [eax + 4], edi
        mov    cl, byte ptr [ebx + 30h]
        mov    byte ptr [eax + 30h], cl
        mov    cl, byte ptr [ebx + 3]
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [ebx + 1], 0ffh
        mov    byte ptr [ebx + 31h], 0ffh
        mov    ebx, eax
        jmp    NEAR PTR loc_1_02CA
loc_1_0466:
        sub    edx, edx
        db  0A1h, 0F4h, 005h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0477
        cmp    dword ptr [ebx + 2ch], eax
        jne    SHORT loc_1_0477
        inc    edx
loc_1_0477:
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     NEAR PTR loc_1_02CA
loc_1_0489:
        push   eax
        mov    esi, dword ptr [eax + edx*8 + 1ch]
        pushfd
        cli
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        je     SHORT loc_1_04D1
loc_1_0498:
        cmp    dword ptr [eax], ebx
        jne    SHORT loc_1_04C9
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        popfd
        pushad
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 0bh
        mov    byte ptr [ebx + 31h], 0bh
        cmp    dword ptr [ebx + 2ch], 0
        je     SHORT loc_1_04BC
        mov    eax, 0bh
        call   dword ptr [ebx + 2ch]
loc_1_04BC:
        popad
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        pop    eax
        jmp    NEAR PTR loc_1_02E3
loc_1_04C9:
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_0498
loc_1_04D1:
        popfd
        pop    eax
        db  08Bh, 035h, 0F0h, 005h, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        jne    SHORT loc_1_0489
        jmp    NEAR PTR loc_1_02CA
loc_1_04E6:
        pushad
        cld
        push   ebx
        call   sub_1_00A5
        pop    ebx
        or     eax, eax
        je     NEAR PTR loc_1_0579
        mov    edi, eax
        mov    edx, 1
loc_1_04FE:
        mov    esi, dword ptr [edi + edx*8 + 1ch]
        db  0CDh, 020h, 0A5h, 000h, 001h, 000h
        je     SHORT loc_1_0576
        dec    dword ptr [edi + edx*8 + 20h]
        push   eax
        push   edx
        push   esi
        push   edi
        mov    eax, dword ptr [eax]
        sub    ecx, ecx
        mov    cx, word ptr [eax + 8]
        cmp    cx, word ptr [ebx + 8]
        jge    SHORT loc_1_0532
        cmp    byte ptr [ebx + 1], 0
        jne    SHORT loc_1_052D
        mov    edx, 6
        jmp    SHORT loc_1_053D
loc_1_052D:
        mov    dl, byte ptr [ebx + 1]
        jmp    SHORT loc_1_053D
loc_1_0532:
        mov    dl, byte ptr [ebx + 1]
        mov    cx, word ptr [ebx + 8]
        mov    word ptr [eax + 8], cx
loc_1_053D:
        mov    edi, dword ptr [eax + 4]
        mov    esi, dword ptr [ebx + 4]
        cld
        rep movsb
        mov    ecx, 10h
        lea    edi, [eax + 0ah]
        lea    esi, [ebx + 0ah]
        cld
        rep movsb
        mov    byte ptr [eax + 1], dl
        mov    byte ptr [eax + 31h], dl
        pushad
        mov    ebx, eax
        sub    eax, eax
        cmp    dword ptr [ebx + 2ch], eax
        je     SHORT loc_1_056B
        mov    al, byte ptr [ebx + 1]
        call   dword ptr [ebx + 2ch]
        cld
loc_1_056B:
        popad
        pop    edi
        pop    esi
        pop    edx
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
loc_1_0576:
        dec    edx
        je     SHORT loc_1_04FE
loc_1_0579:
        dec    dword ptr [edi + 18h]
        push   ebx
        call   sub_1_0034
        add    esp, 4
        popad
        ret
        db  000h, 03Bh, 001h, 000h, 000h, 042h, 001h, 000h, 000h, 0EEh, 001h, 000h, 000h, 058h, 002h, 000h
        db  000h, 09Fh, 002h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 098h, 004h, 003h, 00Ah, 000h, 000h, 053h, 070h, 06Ch, 069h
        db  074h, 074h, 065h, 072h, 000h, 000h, 000h, 080h, 099h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  088h, 005h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_05FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0600:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end

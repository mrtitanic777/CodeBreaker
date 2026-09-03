; vtcp_lcode_01_01.asm -- VTCP object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        db  0CDh, 020h, 002h, 080h, 089h, 004h
sub_1_0006:
        db  0CDh, 020h, 007h, 080h, 088h, 004h
sub_1_000C:
        db  0CDh, 020h, 006h, 080h, 089h, 004h
MSTCP_Service_00:
        mov    ax, 100h
        clc
        ret
MSTCP_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0DFh, 0FFh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0D6h, 0FFh, 0FEh, 0FFh
        clc
        ret
MSTCP_V86_API:
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        push   eax
        cmp    word ptr [eax], 0
        jne    SHORT loc_1_0044
        call   sub_1_009C
        jmp    SHORT loc_1_0055
loc_1_0044:
        cmp    word ptr [eax], 1
        jne    SHORT loc_1_0051
        call   sub_1_0110
        jmp    SHORT loc_1_0055
loc_1_0051:
        mov    ax, 0ffffh
loc_1_0055:
        pop    edx
        mov    word ptr [ebp + 1ch], ax
        ret
sub_1_005B:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        jecxz  loc_1_007E
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_007F
        movzx  edx, ax
        shr    eax, 10h
        shl    eax, 4
        add    eax, edx
        add    eax, dword ptr [ebx + 4]
        pop    ebx
loc_1_007E:
        ret
loc_1_007F:
        shr    eax, 10h
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    ebx
        mov    edx, dword ptr [esp + 4]
        movzx  edx, dx
        add    eax, edx
        ret
        db  000h, 000h
sub_1_009C:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        lea    edi, [esp + 0ch]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    esi, dword ptr [esp + 34h]
        push   dword ptr [esi + 2]
        call   sub_1_005B
        add    esp, 4
        mov    edi, eax
        push   dword ptr [esi + 0ah]
        call   sub_1_005B
        add    esp, 4
        mov    ebx, eax
        push   dword ptr [esi + 6]
        call   sub_1_005B
        add    esp, 4
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 1ch], 0
        push   dword ptr [esi + 0eh]
        call   sub_1_005B
        add    esp, 4
        push   eax
        push   ebx
        lea    eax, [esp + 24h]
        push   eax
        push   edi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_74F0
        add    esp, 14h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Bh, 0FFh
sub_1_0110:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        lea    edi, [esp + 0ch]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    esi, dword ptr [esp + 20h]
        push   dword ptr [esi + 2]
        call   sub_1_005B
        add    esp, 4
        mov    edi, eax
        push   dword ptr [esi + 0ah]
        call   sub_1_005B
        add    esp, 4
        mov    ebx, eax
        push   dword ptr [esi + 6]
        call   sub_1_005B
        add    esp, 4
        push   dword ptr [ebx]
        push   eax
        push   edi
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_7878
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_0164:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_01A8
        db  0A1h, 048h, 096h, 000h, 000h
        mov    dword ptr [eax], 400h
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 401h
        mov    dword ptr [eax + 0ch], 0
        db  0C7h, 005h, 03Ch, 097h, 000h, 000h, 002h, 000h, 000h, 000h
        db  068h, 03Ch, 097h, 000h, 000h
        push   eax
        db  0FFh, 015h, 0D0h, 095h, 000h, 000h
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_01A8:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_01D1
        push   0
        push   0
        lea    eax, [esp + 0ch]
        push   eax
        db  068h, 094h, 05Ah, 000h, 000h
        call   sub_1_5AB0
        add    esp, 10h
        push   dword ptr [esp + 4]
        call   sub_1_05CC
        add    esp, 4
loc_1_01D1:
        ret
        db  08Bh, 0FFh
sub_1_01D4:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, edx
        and    ecx, 0ffffh
        db  039h, 00Dh, 0F8h, 095h, 000h, 000h
        jbe    SHORT loc_1_01FF
        db  0A1h, 020h, 096h, 000h, 000h
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     SHORT loc_1_01FF
        movzx  ecx, word ptr [eax + 3ah]
        shr    edx, 10h
        cmp    ecx, edx
        je     SHORT loc_1_0201
loc_1_01FF:
        sub    eax, eax
loc_1_0201:
        ret
        db  08Bh, 0FFh
sub_1_0204:
        push   ebx
        push   esi
        push   edi
loc_1_0207:
        db  08Bh, 015h, 020h, 096h, 000h, 000h
        db  08Bh, 01Dh, 0F8h, 095h, 000h, 000h
        db  08Bh, 00Dh, 084h, 096h, 000h, 000h
        sub    eax, eax
        cmp    ebx, eax
        je     SHORT loc_1_0234
        nop
loc_1_0220:
        cmp    ebx, ecx
        jne    SHORT loc_1_0226
        sub    ecx, ecx
loc_1_0226:
        cmp    dword ptr [edx + ecx*4], 0
        je     SHORT loc_1_0232
        inc    ecx
        inc    eax
        cmp    ebx, eax
        ja     SHORT loc_1_0220
loc_1_0232:
        cmp    ebx, eax
loc_1_0234:
        ja     SHORT loc_1_0298
        cmp    ebx, 100h
        je     NEAR PTR loc_1_02DC
        sub    ebx, 0f0h
        sbb    eax, eax
        and    ebx, eax
        add    ebx, 100h
        lea    eax, [ebx*4]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        db  08Bh, 015h, 020h, 096h, 000h, 000h
        db  08Bh, 00Dh, 0F8h, 095h, 000h, 000h
        je     SHORT loc_1_02D0
        shl    ecx, 2
        mov    edi, eax
        mov    esi, edx
        shr    ecx, 2
        rep movsd
        db  0A3h, 020h, 096h, 000h, 000h
        db  089h, 01Dh, 0F8h, 095h, 000h, 000h
        push   edx
        call   sub_1_90FC
        add    esp, 4
        jmp    NEAR PTR loc_1_0207
        db  090h
loc_1_0298:
        db  066h, 08Bh, 035h, 058h, 096h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 3ah], si
        mov    byte ptr [eax + 35h], cl
        mov    dword ptr [edx + ecx*4], eax
        lea    eax, [ecx + 1]
        db  0A3h, 084h, 096h, 000h, 000h
        movzx  eax, si
        shl    eax, 10h
        and    ecx, 0ffffh
        or     eax, ecx
        inc    si
        db  066h, 089h, 035h, 058h, 096h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_02D0:
        pop    edi
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_02DC:
        pop    edi
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_02E8:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [ecx + 2ch]
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [edx], eax
        movzx  eax, word ptr [ecx + 30h]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_1_0318
        cmp    byte ptr [eax + 34h], 11h
        jne    SHORT loc_1_0318
        mov    dword ptr [esi + 4], eax
        mov    eax, 1
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_0318:
        mov    ecx, dword ptr [esi]
loc_1_031A:
        or     eax, eax
        je     SHORT loc_1_032E
loc_1_031E:
        cmp    byte ptr [eax + 34h], 11h
        je     SHORT loc_1_032A
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_031E
loc_1_032A:
        or     eax, eax
        jne    SHORT loc_1_0356
loc_1_032E:
        db  08Dh, 014h, 08Dh, 0F8h, 096h, 000h, 000h
loc_1_0335:
        inc    ecx
        add    edx, 4
        db  081h, 0FAh, 038h, 097h, 000h, 000h
        jae    SHORT loc_1_034D
        cmp    dword ptr [edx], 0
        je     SHORT loc_1_0335
        db  08Bh, 004h, 08Dh, 0F8h, 096h, 000h, 000h
loc_1_034D:
        cmp    ecx, 10h
        jne    SHORT loc_1_031A
        or     eax, eax
        je     SHORT loc_1_0364
loc_1_0356:
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], eax
        mov    eax, 1
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_0364:
        sub    eax, eax
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 4], 0
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_0378:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    SHORT loc_1_03D4
        or     eax, eax
        jne    SHORT loc_1_03D4
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 1
        inc    eax
        db  08Dh, 004h, 0B5h, 0F8h, 096h, 000h, 000h
loc_1_039C:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_03B2
loc_1_03A2:
        cmp    byte ptr [ecx + 34h], 11h
        je     SHORT loc_1_03AE
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_03A2
loc_1_03AE:
        or     ecx, ecx
        jne    SHORT loc_1_03C1
loc_1_03B2:
        inc    esi
        add    eax, 4
        db  03Dh, 038h, 097h, 000h, 000h
        jb     SHORT loc_1_039C
        or     ecx, ecx
        je     SHORT loc_1_03D0
loc_1_03C1:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_03D0:
        sub    eax, eax
        pop    esi
        ret
loc_1_03D4:
        cmp    esi, 10h
        jae    SHORT loc_1_03EE
        db  08Bh, 00Ch, 0B5h, 0F8h, 096h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_03EE
loc_1_03E4:
        cmp    ecx, eax
        je     SHORT loc_1_03FC
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_03E4
loc_1_03EE:
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        ret
loc_1_03FC:
        cmp    byte ptr [ecx + 34h], 11h
        jne    SHORT loc_1_03EE
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 1
        mov    eax, 1
        pop    esi
        ret
        db  090h
sub_1_0414:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    si, word ptr [ebp + 0ch]
        movzx  ebx, si
        mov    dl, byte ptr [ebp + 10h]
        inc    edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_0428:
        mov    eax, dword ptr [ebp + 8]
        add    eax, ebx
        and    eax, 0fh
        db  08Bh, 004h, 085h, 0F8h, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0459
        nop
loc_1_043C:
        cmp    eax, edi
        jbe    SHORT loc_1_0453
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 2ch], ecx
        jne    SHORT loc_1_0453
        cmp    word ptr [eax + 30h], si
        jne    SHORT loc_1_0453
        cmp    byte ptr [eax + 34h], dl
        je     SHORT loc_1_046E
loc_1_0453:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_043C
loc_1_0459:
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_1_046C
        mov    dword ptr [ebp + 8], 0
        sub    edi, edi
        jmp    SHORT loc_1_0428
        db  08Bh, 0FFh
loc_1_046C:
        sub    eax, eax
loc_1_046E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
sub_1_0474:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi]
        mov    al, byte ptr [esi + 0ah]
        push   eax
        mov    ax, word ptr [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        call   sub_1_0414
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_049B
        mov    dword ptr [esi], eax
        mov    ecx, dword ptr [eax + 2ch]
        mov    dword ptr [esi + 4], ecx
loc_1_049B:
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_04A0:
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ax, word ptr [esp + 8]
        mov    word ptr [ecx + 8], ax
        mov    al, byte ptr [esp + 0ch]
        mov    byte ptr [ecx + 0ah], al
        push   ecx
        call   sub_1_0474
        add    esp, 4
        ret
        db  090h
sub_1_04CC:
        mov    edx, dword ptr [esp + 4]
        movzx  eax, word ptr [edx + 30h]
        add    eax, dword ptr [edx + 2ch]
        and    eax, 0fh
        db  08Dh, 004h, 085h, 0F8h, 096h, 000h, 000h
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_04F4
loc_1_04E8:
        cmp    ecx, edx
        ja     SHORT loc_1_04F4
        mov    eax, ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_04E8
loc_1_04F4:
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax], edx
        cmp    byte ptr [edx + 34h], 11h
        jne    SHORT loc_1_0504
        db  0FFh, 005h, 08Ch, 095h, 000h, 000h
loc_1_0504:
        ret
        db  08Dh, 049h, 000h
sub_1_0508:
        mov    eax, dword ptr [esp + 4]
        movzx  ecx, word ptr [eax + 30h]
        add    ecx, dword ptr [eax + 2ch]
        and    ecx, 0fh
        db  08Dh, 00Ch, 08Dh, 0F8h, 096h, 000h, 000h
        mov    edx, dword ptr [ecx]
        or     edx, edx
        je     SHORT loc_1_0544
loc_1_0524:
        cmp    edx, eax
        je     SHORT loc_1_0534
        mov    ecx, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_0524
        ret
        db  08Dh, 049h, 000h
loc_1_0534:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        cmp    byte ptr [edx + 34h], 11h
        jne    SHORT loc_1_0544
        db  0FFh, 00Dh, 08Ch, 095h, 000h, 000h
loc_1_0544:
        ret
        db  08Dh, 049h, 000h
sub_1_0548:
        push   ebx
        inc    ecx
        db  0B9h, 0F8h, 096h, 000h, 000h
        mov    bl, byte ptr [esp + 0ch]
        mov    dx, word ptr [esp + 8]
loc_1_0558:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_1_056F
loc_1_055E:
        cmp    word ptr [eax + 30h], dx
        jne    SHORT loc_1_0569
        cmp    byte ptr [eax + 34h], bl
        je     SHORT loc_1_057C
loc_1_0569:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_055E
loc_1_056F:
        add    ecx, 4
        db  081h, 0F9h, 038h, 097h, 000h, 000h
        jb     SHORT loc_1_0558
        sub    eax, eax
loc_1_057C:
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_0580:
        push   esi
        mov    eax, dword ptr [esp + 8]
        lea    esi, [eax + 4]
        sub    edx, edx
        mov    eax, dword ptr [eax]
        cmp    eax, edx
        jle    SHORT loc_1_05A4
loc_1_0590:
        cmp    word ptr [esi + 2], 2
        je     SHORT loc_1_05A8
        movzx  ecx, word ptr [esi]
        add    ecx, 4
        add    esi, ecx
        inc    edx
        cmp    edx, eax
        jl     SHORT loc_1_0590
loc_1_05A4:
        sub    al, al
        pop    esi
        ret
loc_1_05A8:
        cmp    word ptr [esi], 0eh
        jb     SHORT loc_1_05C8
        add    esi, 4
        mov    cx, word ptr [esi]
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax], cx
        mov    ecx, dword ptr [esi + 2]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        mov    al, 1
        pop    esi
        ret
loc_1_05C8:
        sub    al, al
        pop    esi
        ret
sub_1_05CC:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 14h]
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 1ch]
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 20h], eax
        sub    edi, edi
        db  0C7h, 044h, 024h, 010h, 0F8h, 096h, 000h, 000h
        mov    esi, dword ptr [esp + 28h]
loc_1_05F9:
        mov    eax, dword ptr [esp + 10h]
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        je     NEAR PTR loc_1_06EA
loc_1_0608:
        cmp    dword ptr [ebx + 2ch], esi
        jne    NEAR PTR loc_1_06E0
        mov    ax, word ptr [ebx + 32h]
        test   al, 80h
        je     NEAR PTR loc_1_06E0
        and    ax, 0ff7fh
        mov    word ptr [ebx + 32h], ax
        lea    eax, [ebx + 20h]
        push   eax
        db  0FFh, 015h, 0C4h, 095h, 000h, 000h
        add    esp, 4
        mov    ecx, dword ptr [ebx + 4]
        or     ecx, ecx
        je     SHORT loc_1_064E
        lea    eax, [ebx + 4]
loc_1_063C:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_063C
        mov    dword ptr [eax], edi
        mov    edi, ecx
        mov    dword ptr [ebx + 4], 0
loc_1_064E:
        lea    ecx, [ebx + 8]
        mov    eax, dword ptr [ecx]
        sub    eax, ebx
        cmp    eax, 8
        je     SHORT loc_1_0683
loc_1_065A:
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [esp + 18h], edx
        lea    eax, [esp + 14h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx]
        sub    eax, ebx
        cmp    eax, 8
        jne    SHORT loc_1_065A
loc_1_0683:
        lea    ecx, [ebx + 18h]
        mov    eax, dword ptr [ecx]
        sub    eax, ebx
        cmp    eax, 18h
        je     SHORT loc_1_06B9
loc_1_0690:
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [esp + 20h], edx
        lea    eax, [esp + 1ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx]
        sub    eax, ebx
        cmp    eax, 18h
        jne    SHORT loc_1_0690
loc_1_06B9:
        mov    ebp, dword ptr [ebx + 94h]
        or     ebp, ebp
        je     SHORT loc_1_06D6
        nop
loc_1_06C4:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        push   eax
        call   sub_1_90FC
        add    esp, 4
        or     ebp, ebp
        jne    SHORT loc_1_06C4
loc_1_06D6:
        mov    dword ptr [ebx + 94h], 0
loc_1_06E0:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    NEAR PTR loc_1_0608
loc_1_06EA:
        add    dword ptr [esp + 10h], 4
        db  081h, 07Ch, 024h, 010h, 038h, 097h, 000h, 000h
        jb     NEAR PTR loc_1_05F9
        or     edi, edi
        je     SHORT loc_1_071F
loc_1_0701:
        mov    esi, edi
        mov    edi, dword ptr [edi]
        push   0
        push   6
        push   dword ptr [esi + 14h]
        call   dword ptr [esi + 10h]
        add    esp, 0ch
        push   esi
        call   sub_1_0BC4
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_1_0701
loc_1_071F:
        lea    eax, [esp + 1ch]
        cmp    dword ptr [esp + 1ch], eax
        je     SHORT loc_1_075E
loc_1_0729:
        mov    esi, dword ptr [esp + 1ch]
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 1ch]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   6
        push   dword ptr [esi + 18h]
        call   dword ptr [esi + 14h]
        add    esp, 0ch
        push   esi
        call   sub_1_144C
        add    esp, 4
        lea    eax, [esp + 1ch]
        cmp    dword ptr [esp + 1ch], eax
        jne    SHORT loc_1_0729
loc_1_075E:
        lea    eax, [esp + 14h]
        cmp    dword ptr [esp + 14h], eax
        je     SHORT loc_1_07AD
loc_1_0768:
        mov    esi, dword ptr [esp + 14h]
        mov    eax, esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 14h], eax
        lea    eax, [esp + 14h]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   6
        push   dword ptr [esi + 18h]
        call   dword ptr [esi + 14h]
        add    esp, 0ch
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        je     SHORT loc_1_079A
        push   eax
        call   sub_1_13A8
        add    esp, 4
loc_1_079A:
        push   esi
        call   sub_1_1418
        add    esp, 4
        lea    eax, [esp + 14h]
        cmp    dword ptr [esp + 14h], eax
        jne    SHORT loc_1_0768
loc_1_07AD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_07B8:
        push   dword ptr [esp + 8]
        call   sub_1_1120
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_07C8:
        push   ebx
        mov    eax, dword ptr [esp + 0ch]
        mov    byte ptr [eax], 0
        mov    ecx, dword ptr [esp + 10h]
        mov    byte ptr [ecx], 0
        mov    edx, dword ptr [esp + 8]
        or     edx, edx
        je     SHORT loc_1_07FE
        cmp    byte ptr [edx], 0
        je     SHORT loc_1_07FE
loc_1_07E4:
        mov    bl, byte ptr [edx]
        cmp    bl, 1
        jne    SHORT loc_1_07F0
        mov    byte ptr [eax], 1
        jmp    SHORT loc_1_07F8
loc_1_07F0:
        cmp    bl, 2
        jne    SHORT loc_1_07F8
        mov    byte ptr [ecx], 1
loc_1_07F8:
        inc    edx
        cmp    byte ptr [edx], 0
        jne    SHORT loc_1_07E4
loc_1_07FE:
        pop    ebx
        ret
loc_1_0800:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_1_0580
        add    esp, 0ch
        or     al, al
        jne    SHORT loc_1_082C
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
loc_1_082C:
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [ebp + 14h]
        call   sub_1_07C8
        add    esp, 0ch
        push   98h
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_087C
        push   dword ptr [ebp - 8]
        db  0FFh, 015h, 0A8h, 095h, 000h, 000h
        add    esp, 4
        or     al, al
        je     SHORT loc_1_087C
        or     esi, esi
        je     SHORT loc_1_0871
        push   esi
        call   sub_1_90FC
        add    esp, 4
loc_1_0871:
        pop    edi
        mov    eax, 3
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_087C:
        cmp    word ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0914
        db  066h, 0A1h, 0F4h, 092h, 000h, 000h
        mov    word ptr [ebp - 4], ax
        sub    edi, edi
        mov    ebx, dword ptr [ebp + 10h]
loc_1_0896:
        cmp    word ptr [ebp - 4], 1388h
        jbe    SHORT loc_1_08A4
        mov    word ptr [ebp - 4], 401h
loc_1_08A4:
        mov    ah, byte ptr [ebp - 4]
        mov    al, byte ptr [ebp - 3]
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_08BC
        push   ebx
        push   eax
        call   sub_1_0548
        add    esp, 8
        jmp    SHORT loc_1_08CB
loc_1_08BC:
        push   0
        push   ebx
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_1_0414
        add    esp, 10h
loc_1_08CB:
        or     eax, eax
        je     SHORT loc_1_08DC
        inc    edi
        inc    word ptr [ebp - 4]
        cmp    edi, 0f88h
        jl     SHORT loc_1_0896
loc_1_08DC:
        cmp    edi, 0f88h
        jne    SHORT loc_1_08FC
        or     esi, esi
        je     SHORT loc_1_08F1
        push   esi
        call   sub_1_90FC
        add    esp, 4
loc_1_08F1:
        pop    edi
        mov    eax, 4
        pop    esi
        pop    ebx
        leave
        ret
        db  090h
loc_1_08FC:
        mov    ax, word ptr [ebp - 4]
        inc    ax
        db  066h, 0A3h, 0F4h, 092h, 000h, 000h
        mov    ah, byte ptr [ebp - 4]
        mov    al, byte ptr [ebp - 3]
        mov    word ptr [ebp - 4], ax
        jmp    SHORT loc_1_096F
loc_1_0914:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_096C
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_0934
        mov    ebx, dword ptr [ebp + 10h]
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_1_0548
        add    esp, 8
        jmp    SHORT loc_1_0948
        db  08Dh, 049h, 000h
loc_1_0934:
        push   0
        mov    ebx, dword ptr [ebp + 10h]
        push   ebx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        call   sub_1_0414
        add    esp, 10h
loc_1_0948:
        or     eax, eax
        je     SHORT loc_1_096F
        cmp    byte ptr [ebp - 2], 0
        jne    SHORT loc_1_096F
        or     esi, esi
        je     SHORT loc_1_095F
        push   esi
        call   sub_1_90FC
        add    esp, 4
loc_1_095F:
        pop    edi
        mov    eax, 2
        pop    esi
        pop    ebx
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_096C:
        mov    ebx, dword ptr [ebp + 10h]
loc_1_096F:
        or     esi, esi
        je     NEAR PTR loc_1_0A2C
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 26h
        rep stosd
        lea    eax, [esi + 20h]
        push   eax
        db  0FFh, 015h, 0B8h, 095h, 000h, 000h
        add    esp, 4
        lea    eax, [esi + 3ch]
        push   eax
        db  0FFh, 015h, 0B8h, 095h, 000h, 000h
        add    esp, 4
        mov    byte ptr [esi + 45h], 1
        mov    dword ptr [esi + 48h], 0
        mov    dword ptr [esi + 5ch], 0
        db  0C7h, 046h, 054h, 0B8h, 007h, 000h, 000h
        db  0A1h, 0B4h, 094h, 000h, 000h
        mov    dword ptr [esi + 60h], eax
        lea    eax, [esi + 8]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0ch], eax
        lea    eax, [esi + 18h]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 1ch], eax
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi + 30h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 2ch], eax
        mov    byte ptr [esi + 34h], bl
        mov    word ptr [esi + 32h], 80h
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_09F0
        mov    word ptr [esi + 32h], 180h
loc_1_09F0:
        push   esi
        or     word ptr [esi + 32h], 8
        call   sub_1_0204
        add    esp, 4
        mov    edi, eax
        cmp    edi, -1
        push   esi
        jne    SHORT loc_1_0A18
        call   sub_1_90FC
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0A18:
        call   sub_1_04CC
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], edi
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_0A2C:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        leave
        ret
        db  08Bh, 0FFh
sub_1_0A38:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 18h]
        test   byte ptr [esi + 32h], 20h
        je     SHORT loc_1_0A56
        mov    ecx, dword ptr [esi + 10h]
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
loc_1_0A56:
        push   esi
        movzx  ecx, byte ptr [esi + 35h]
        db  0A1h, 020h, 096h, 000h, 000h
        mov    dword ptr [eax + ecx*4], 0
        call   sub_1_0508
        add    esp, 4
        mov    ecx, dword ptr [esi + 4ch]
        or     ecx, ecx
        je     SHORT loc_1_0AC1
loc_1_0A76:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_1_0AAD
        cmp    byte ptr [eax + 84h], 0
        je     SHORT loc_1_0A99
        test   byte ptr [eax + 86h], 1
        jne    SHORT loc_1_0A99
        inc    dword ptr [eax + 74h]
        mov    dword ptr [eax + 0c8h], edi
        mov    edi, eax
loc_1_0A99:
        mov    dword ptr [eax + 0cch], 0
        mov    dword ptr [eax + 0dch], 0
loc_1_0AAD:
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    SHORT loc_1_0A76
loc_1_0AC1:
        lea    ebx, [esi + 18h]
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 18h
        je     SHORT loc_1_0AF6
loc_1_0ACD:
        mov    ebp, dword ptr [ebx]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        push   0
        push   7
        push   dword ptr [ebp + 18h]
        call   dword ptr [ebp + 14h]
        add    esp, 0ch
        push   ebp
        call   sub_1_144C
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 18h
        jne    SHORT loc_1_0ACD
loc_1_0AF6:
        lea    ebx, [esi + 8]
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 8
        je     SHORT loc_1_0B3B
loc_1_0B02:
        mov    ebp, dword ptr [ebx]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        push   0
        push   7
        push   dword ptr [ebp + 18h]
        call   dword ptr [ebp + 14h]
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_1_0B29
        push   eax
        call   sub_1_13A8
        add    esp, 4
loc_1_0B29:
        push   ebp
        call   sub_1_1418
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, esi
        cmp    eax, 8
        jne    SHORT loc_1_0B02
loc_1_0B3B:
        lea    eax, [esi + 20h]
        push   eax
        db  0FFh, 015h, 0C4h, 095h, 000h, 000h
        add    esp, 4
        mov    ebx, dword ptr [esi + 94h]
        or     ebx, ebx
        je     SHORT loc_1_0B74
loc_1_0B52:
        push   0
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        db  0FFh, 015h, 0D4h, 095h, 000h, 000h
        add    esp, 0ch
        mov    eax, ebx
        mov    ebx, dword ptr [ebx]
        push   eax
        call   sub_1_90FC
        add    esp, 4
        or     ebx, ebx
        jne    SHORT loc_1_0B52
loc_1_0B74:
        push   esi
        call   sub_1_90FC
        add    esp, 4
        or     edi, edi
        je     SHORT loc_1_0BA7
        mov    ebx, dword ptr [esp + 10h]
loc_1_0B85:
        dec    dword ptr [edi + 74h]
        or     dword ptr [edi + 58h], 40000000h
        mov    esi, dword ptr [edi + 0c8h]
        push   ebx
        push   40h
        push   edi
        call   sub_1_6398
        add    esp, 0ch
        mov    edi, esi
        or     esi, esi
        jne    SHORT loc_1_0B85
loc_1_0BA7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_1_0BB0:
        db  0A1h, 09Ch, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0BC2
        mov    ecx, dword ptr [eax + 10h]
        db  089h, 00Dh, 09Ch, 096h, 000h, 000h
loc_1_0BC2:
        ret
        db  090h
sub_1_0BC4:
        db  08Bh, 00Dh, 09Ch, 096h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 10h], ecx
        db  0A3h, 09Ch, 096h, 000h, 000h
        ret
        db  090h
loc_1_0BD8:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        push   dword ptr [ebx]
        call   sub_1_01D4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_0BF8
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0BF8:
        mov    ax, word ptr [esi + 32h]
        test   al, 40h
        jne    SHORT loc_1_0C28
        cmp    word ptr [esi + 38h], 0
        jne    SHORT loc_1_0C28
        or     ax, 40h
        mov    word ptr [esi + 32h], ax
        and    ax, 0ff7fh
        mov    word ptr [esi + 32h], ax
        push   esi
        call   sub_1_0A38
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_0C28:
        test   al, 1
        jne    SHORT loc_1_0CA4
        mov    edi, dword ptr [esi + 4]
        call   sub_1_0BB0
        or     eax, eax
        je     SHORT loc_1_0C98
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 14h], ecx
        mov    cx, word ptr [esi + 32h]
        and    cx, 0fffdh
        mov    word ptr [esi + 32h], cx
        or     cx, 1
        mov    word ptr [esi + 32h], cx
        and    cx, 0ff7fh
        mov    word ptr [esi + 32h], cx
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [eax], 0
        or     edi, edi
        je     SHORT loc_1_0C8E
        nop
loc_1_0C70:
        push   0
        push   7
        push   dword ptr [edi + 14h]
        call   dword ptr [edi + 10h]
        add    esp, 0ch
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_0BC4
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_1_0C70
loc_1_0C8E:
        pop    edi
        mov    eax, 0ffh
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_0C98:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_0CA4:
        pop    edi
        mov    eax, 6
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_0CB0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        lea    esi, [eax + 94h]
        mov    eax, dword ptr [eax + 94h]
        or     eax, eax
        je     SHORT loc_1_0CDF
        mov    ecx, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
loc_1_0CCD:
        cmp    dword ptr [eax + 4], ecx
        jne    SHORT loc_1_0CD7
        cmp    dword ptr [eax + 8], edx
        je     SHORT loc_1_0CDF
loc_1_0CD7:
        mov    esi, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_0CCD
loc_1_0CDF:
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], esi
        pop    esi
        ret
        db  090h
sub_1_0CE8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 1ch], 5
        jne    SHORT loc_1_0D4C
        mov    ebp, dword ptr [esp + 18h]
        add    ebp, 20h
        push   ebp
        db  0FFh, 015h, 0C4h, 095h, 000h, 000h
        add    esp, 4
        push   ebp
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        db  0FFh, 015h, 0C0h, 095h, 000h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_0D28
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_0D28:
        cmp    eax, 2afeh
        jne    SHORT loc_1_0D3C
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0D3C:
        pop    ebp
        mov    eax, 0bh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_0D4C:
        mov    ecx, dword ptr [esp + 20h]
        or     ecx, ecx
        jne    SHORT loc_1_0D64
        mov    eax, 0bh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_0D64:
        sub    esi, esi
        mov    eax, dword ptr [esp + 1ch]
        dec    eax
        cmp    eax, 6
        ja     SHORT loc_1_0D78
        db  0FFh, 024h, 085h, 054h, 00Fh, 000h, 000h
        db  090h
loc_1_0D78:
        pop    ebp
        mov    esi, 0bh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_0D88:
        mov    ebx, dword ptr [esp + 24h]
        mov    al, byte ptr [ebx]
        mov    ebp, dword ptr [esp + 18h]
        mov    byte ptr [ebp + 29h], al
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_0DA0:
        mov    ebx, dword ptr [esp + 24h]
        mov    al, byte ptr [ebx]
        mov    ebp, dword ptr [esp + 18h]
        mov    byte ptr [ebp + 45h], al
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_0DB8:
        cmp    ecx, 4
        jb     SHORT loc_1_0DF8
        mov    ebx, dword ptr [esp + 24h]
        mov    edi, dword ptr [ebx]
        or     edi, edi
        je     SHORT loc_1_0DE4
        push   edi
        db  0FFh, 015h, 0A8h, 095h, 000h, 000h
        add    esp, 4
        or     al, al
        je     SHORT loc_1_0DE4
        mov    eax, 0bh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_0DE4:
        mov    eax, esi
        mov    ebp, dword ptr [esp + 18h]
        mov    dword ptr [ebp + 48h], edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_0DF8:
        pop    ebp
        mov    esi, 0bh
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_0E08:
        mov    ebx, dword ptr [esp + 24h]
        cmp    byte ptr [ebx], 0
        mov    ebp, dword ptr [esp + 18h]
        je     SHORT loc_1_0E24
        or     word ptr [ebp + 32h], 8
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0E24:
        mov    eax, esi
        and    word ptr [ebp + 32h], 0fff7h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0E34:
        cmp    ecx, 8
        jb     NEAR PTR loc_1_0F44
        mov    ebx, dword ptr [esp + 24h]
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        mov    ebp, dword ptr [esp + 24h]
        push   ebp
        call   sub_1_0CB0
        add    esp, 10h
        cmp    dword ptr [esp + 1ch], 6
        jne    SHORT loc_1_0EB0
        or     eax, eax
        je     SHORT loc_1_0E74
        mov    esi, 0bh
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_0E74:
        push   0ch
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0E94
        mov    esi, 1
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_0E94:
        mov    ecx, dword ptr [ebp + 94h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp + 94h], eax
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 8], ecx
        jmp    SHORT loc_1_0ED5
        db  090h
loc_1_0EB0:
        or     eax, eax
        jne    SHORT loc_1_0EC4
        mov    esi, 0bh
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_0EC4:
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [edx], ecx
        push   eax
        call   sub_1_90FC
        add    esp, 4
loc_1_0ED5:
        mov    eax, 0
        cmp    dword ptr [esp + 1ch], 6
        sete   al
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        db  0FFh, 015h, 0D4h, 095h, 000h, 000h
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_0F49
        cmp    dword ptr [esp + 1ch], 6
        jne    SHORT loc_1_0F26
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx]
        push   ebp
        call   sub_1_0CB0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_0F26
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [edx], ecx
        push   eax
        call   sub_1_90FC
        add    esp, 4
loc_1_0F26:
        sub    edi, 2afeh
        cmp    edi, 1
        sbb    esi, esi
        and    esi, 0fffffff6h
        add    esi, 0bh
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_0F44:
        mov    esi, 0bh
loc_1_0F49:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h, 088h, 00Dh, 000h, 000h, 0A0h, 00Dh, 000h, 000h, 0B8h, 00Dh, 000h, 000h, 008h, 00Eh, 000h
        db  000h, 078h, 00Dh, 000h, 000h, 034h, 00Eh, 000h, 000h, 034h, 00Eh, 000h, 000h
sub_1_0F70:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   dword ptr [edi]
        call   sub_1_01D4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_0F90
        mov    eax, 6
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_0F90:
        mov    ax, word ptr [esi + 32h]
        test   al, 80h
        je     NEAR PTR loc_1_1040
        test   al, 40h
        jne    SHORT loc_1_0FEC
        cmp    word ptr [esi + 38h], 0
        jne    SHORT loc_1_0FEC
        or     ax, 40h
        push   dword ptr [esp + 18h]
        mov    word ptr [esi + 32h], ax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_1_0CE8
        add    esp, 10h
        mov    edi, eax
        mov    ax, word ptr [esi + 32h]
        test   al, 7
        jne    SHORT loc_1_0FDC
        and    ax, 0ffbfh
        mov    word ptr [esi + 32h], ax
        mov    eax, edi
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_0FDC:
        push   esi
        call   sub_1_1120
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_0FEC:
        call   sub_1_0BB0
        or     eax, eax
        je     SHORT loc_1_1038
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [eax + 14h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 0ch], ecx
        or     word ptr [esi + 32h], 2
        add    esi, 4
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_102B
loc_1_1024:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_1024
loc_1_102B:
        mov    dword ptr [esi], eax
        mov    eax, 0ffh
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_1038:
        pop    edi
        mov    eax, 1
        pop    esi
        ret
loc_1_1040:
        pop    edi
        mov    eax, 6
        pop    esi
        ret
loc_1_1048:
        push   dword ptr [esp + 4]
        call   sub_1_01D4
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_1100
        test   byte ptr [eax + 32h], 80h
        je     NEAR PTR loc_1_1100
        sub    edx, edx
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, 5
        ja     SHORT loc_1_1078
        db  0FFh, 024h, 08Dh, 008h, 011h, 000h, 000h
loc_1_1078:
        mov    edx, 0ah
        mov    eax, edx
        ret
loc_1_1080:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 64h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 68h], ecx
        mov    eax, edx
        ret
        db  08Dh, 049h, 000h
loc_1_1094:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 6ch], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 70h], ecx
        mov    eax, edx
        ret
        db  08Dh, 049h, 000h
loc_1_10A8:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 74h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 78h], ecx
        mov    eax, edx
        ret
        db  08Dh, 049h, 000h
loc_1_10BC:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 7ch], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 80h], ecx
        mov    eax, edx
        ret
loc_1_10D0:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 84h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 88h], ecx
        mov    eax, edx
        ret
        db  090h
loc_1_10E8:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 8ch], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 90h], ecx
        mov    eax, edx
        ret
        db  090h
loc_1_1100:
        mov    eax, 6
        ret
        db  08Bh, 0FFh, 080h, 010h, 000h, 000h, 094h, 010h, 000h, 000h, 0A8h, 010h, 000h, 000h, 0BCh, 010h
        db  000h, 000h, 0D0h, 010h, 000h, 000h, 0E8h, 010h, 000h, 000h
sub_1_1120:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        test   byte ptr [esi + 32h], 7
        je     NEAR PTR loc_1_11DF
loc_1_1130:
        mov    edi, dword ptr [esi + 4]
        mov    ax, word ptr [esi + 32h]
        test   al, 1
        jne    NEAR PTR loc_1_11E8
        test   al, 2
        je     SHORT loc_1_1183
        nop
loc_1_1144:
        mov    edi, dword ptr [esi + 4]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 4], eax
        or     eax, eax
        jne    SHORT loc_1_1156
        and    word ptr [esi + 32h], 0fffdh
loc_1_1156:
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_1_0CE8
        add    esp, 10h
        push   0
        push   eax
        push   dword ptr [edi + 14h]
        call   dword ptr [edi + 10h]
        add    esp, 0ch
        push   edi
        call   sub_1_0BC4
        add    esp, 4
        test   byte ptr [esi + 32h], 2
        jne    SHORT loc_1_1144
loc_1_1183:
        mov    dx, word ptr [esi + 32h]
        test   dl, 4
        je     SHORT loc_1_11D5
        lea    ecx, [esi + 8]
        mov    edi, dword ptr [ecx]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 8
        je     SHORT loc_1_11CC
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        inc    word ptr [esi + 38h]
        and    dx, 0ffbfh
        mov    word ptr [esi + 32h], dx
        push   edi
        push   esi
        call   sub_1_1590
        add    esp, 8
        mov    ax, word ptr [esi + 38h]
        dec    ax
        mov    word ptr [esi + 38h], ax
        jne    SHORT loc_1_11DF
        or     word ptr [esi + 32h], 40h
        jmp    SHORT loc_1_11D5
        db  090h
loc_1_11CC:
        and    dx, 0fffbh
        mov    word ptr [esi + 32h], dx
loc_1_11D5:
        test   byte ptr [esi + 32h], 7
        jne    NEAR PTR loc_1_1130
loc_1_11DF:
        pop    edi
        and    word ptr [esi + 32h], 0ffbfh
        pop    esi
        ret
loc_1_11E8:
        push   esi
        call   sub_1_0A38
        add    esp, 4
        push   0
        push   0
        push   dword ptr [edi + 14h]
        call   dword ptr [edi + 10h]
        add    esp, 0ch
        push   edi
        call   sub_1_0BC4
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_120C:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 38h]
        dec    ax
        mov    word ptr [ecx + 38h], ax
        jne    SHORT loc_1_123D
        mov    ax, word ptr [ecx + 32h]
        test   al, 40h
        jne    SHORT loc_1_123D
        test   al, 7
        je     SHORT loc_1_123D
        or     ax, 40h
        mov    word ptr [ecx + 32h], ax
        push   ecx
        add    ecx, 50h
        push   ecx
        call   sub_1_90EA
        add    esp, 8
loc_1_123D:
        ret
        db  08Bh, 0FFh
sub_1_1240:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 38h]
        dec    ax
        mov    word ptr [ecx + 38h], ax
        jne    SHORT loc_1_126D
        mov    ax, word ptr [ecx + 32h]
        test   al, 40h
        jne    SHORT loc_1_126D
        test   al, 7
        je     SHORT loc_1_126D
        or     ax, 40h
        mov    word ptr [ecx + 32h], ax
        push   ecx
        call   sub_1_1120
        add    esp, 4
loc_1_126D:
        ret
        db  08Bh, 0FFh
sub_1_1270:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    esi, esi
        db  0A1h, 0F8h, 092h, 000h, 000h
        db  03Bh, 005h, 0FCh, 092h, 000h, 000h
        jae    NEAR PTR loc_1_139C
        push   0ch
        call   sub_1_90F6
        add    esp, 4
        mov    edi, eax
        cmp    edi, esi
        jne    SHORT loc_1_12A4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_12A4:
        push   5
        lea    eax, [edi + 4]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_91DE
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_12D0
        push   edi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_12D0:
        db  08Bh, 01Dh, 094h, 095h, 000h, 000h
        add    ebx, 8
        lea    eax, [ebx + ebx*4]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    dword ptr [esp + 1ch], eax
        or     eax, eax
        jne    SHORT loc_1_1308
        push   dword ptr [edi + 4]
        call   sub_1_91E4
        push   edi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1308:
        sub    ebp, ebp
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esp + 18h], eax
loc_1_1315:
        push   ebx
        push   dword ptr [esp + 1ch]
        push   dword ptr [edi + 4]
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_91EA
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_1374
        or     ebp, ebp
        je     SHORT loc_1_1348
        push   dword ptr [esp + 10h]
        call   sub_1_13A8
        add    esp, 4
        jmp    SHORT loc_1_134C
        db  08Dh, 049h, 000h
loc_1_1348:
        mov    esi, dword ptr [esp + 10h]
loc_1_134C:
        inc    ebp
        add    dword ptr [esp + 18h], ebx
        cmp    ebp, 5
        jb     SHORT loc_1_1315
        db  083h, 005h, 0F8h, 092h, 000h, 000h, 005h
        db  0A1h, 000h, 093h, 000h, 000h
        mov    dword ptr [edi], eax
        db  089h, 03Dh, 000h, 093h, 000h, 000h
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1374:
        push   dword ptr [edi + 4]
        call   sub_1_91E4
        push   edi
        call   sub_1_90FC
        add    esp, 4
        push   dword ptr [esp + 1ch]
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_139C:
        sub    esi, esi
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_13A8:
        db  0A1h, 00Ch, 096h, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 00Ch, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_13BC:
        mov    edx, dword ptr [esp + 4]
        mov    ax, word ptr [edx + 32h]
        test   al, 20h
        jne    SHORT loc_1_13F6
        and    ax, 0fffbh
        mov    word ptr [edx + 32h], ax
        or     ax, 20h
        mov    word ptr [edx + 32h], ax
        lea    ecx, [edx + 10h]
        db  0A1h, 060h, 096h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A1h, 060h, 096h, 000h, 000h
        mov    dword ptr [edx + 14h], eax
        db  089h, 00Dh, 060h, 096h, 000h, 000h
        db  0C7h, 001h, 05Ch, 096h, 000h, 000h
loc_1_13F6:
        ret
        db  090h
sub_1_13F8:
        db  0A1h, 094h, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_140C
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 094h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
loc_1_140C:
        push   20h
        call   sub_1_90F6
        add    esp, 4
        ret
        db  090h
sub_1_1418:
        db  08Bh, 00Dh, 094h, 096h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        db  0A3h, 094h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_142C:
        db  0A1h, 044h, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_1440
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 044h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
loc_1_1440:
        push   20h
        call   sub_1_90F6
        add    esp, 4
        ret
        db  090h
sub_1_144C:
        db  08Bh, 00Dh, 044h, 096h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        db  0A3h, 044h, 096h, 000h, 000h
        ret
        db  08Bh, 0FFh
loc_1_1460:
        push   esi
        db  081h, 03Dh, 0ECh, 095h, 000h, 000h, 0ECh, 095h, 000h, 000h
        je     SHORT loc_1_14DC
loc_1_146D:
        db  08Bh, 035h, 0ECh, 095h, 000h, 000h
        mov    eax, esi
        mov    eax, dword ptr [eax]
        db  0A3h, 0ECh, 095h, 000h, 000h
        mov    eax, dword ptr [esi]
        db  0C7h, 040h, 004h, 0ECh, 095h, 000h, 000h
        sub    esi, 10h
        mov    ax, word ptr [esi + 32h]
        and    ax, 0ffdfh
        mov    word ptr [esi + 32h], ax
        test   al, 40h
        jne    SHORT loc_1_14C8
        lea    edx, [esi + 8]
        mov    ecx, dword ptr [edx]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 8
        je     SHORT loc_1_14D0
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [eax + 4], edx
        inc    word ptr [esi + 38h]
        push   ecx
        push   esi
        call   sub_1_1590
        add    esp, 8
        push   esi
        call   sub_1_1240
        add    esp, 4
        jmp    SHORT loc_1_14D0
        db  08Bh, 0FFh
loc_1_14C8:
        or     ax, 4
        mov    word ptr [esi + 32h], ax
loc_1_14D0:
        db  081h, 03Dh, 0ECh, 095h, 000h, 000h, 0ECh, 095h, 000h, 000h
        jne    SHORT loc_1_146D
loc_1_14DC:
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_14E0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        mov    edi, dword ptr [ebp + 14h]
        mov    ebx, dword ptr [ebp + 18h]
        db  081h, 03Dh, 05Ch, 096h, 000h, 000h, 05Ch, 096h, 000h, 000h
        mov    si, word ptr [ebp + 1ch]
        jne    SHORT loc_1_150C
        push   dword ptr [esp + 18h]
        call   sub_1_13A8
        add    esp, 4
        jmp    SHORT loc_1_1572
loc_1_150C:
        db  08Bh, 00Dh, 05Ch, 096h, 000h, 000h
        mov    eax, ecx
        mov    eax, dword ptr [eax]
        db  0A3h, 05Ch, 096h, 000h, 000h
        mov    eax, dword ptr [ecx]
        db  0C7h, 040h, 004h, 05Ch, 096h, 000h, 000h
        sub    ecx, 10h
        mov    eax, dword ptr [ecx + 8]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 8
        je     SHORT loc_1_156C
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 10h], edx
        lea    eax, [ecx + 10h]
        db  08Bh, 015h, 0F0h, 095h, 000h, 000h
        mov    dword ptr [edx], eax
        db  08Bh, 015h, 0F0h, 095h, 000h, 000h
        mov    dword ptr [ecx + 14h], edx
        db  0A3h, 0F0h, 095h, 000h, 000h
        db  0C7h, 000h, 0ECh, 095h, 000h, 000h
        push   0
        db  068h, 04Ch, 097h, 000h, 000h
        call   sub_1_90EA
        add    esp, 8
        jmp    SHORT loc_1_1572
        db  08Bh, 0FFh
loc_1_156C:
        and    word ptr [ecx + 32h], 0ffdfh
loc_1_1572:
        push   ebp
        call   sub_1_1418
        add    esp, 4
        or     edi, edi
        je     SHORT loc_1_158B
        movzx  eax, si
        push   eax
        push   0
        push   ebx
        call   edi
        add    esp, 0ch
loc_1_158B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1590:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 28h]
        inc    esi
        mov    esi, dword ptr [esp + 2ch]
loc_1_15A0:
        mov    ebx, dword ptr [esi + 10h]
        or     ebx, ebx
        jne    SHORT loc_1_15CE
        db  0A1h, 00Ch, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_15BC
        mov    ecx, eax
        mov    ecx, dword ptr [ecx]
        db  089h, 00Dh, 00Ch, 096h, 000h, 000h
        jmp    SHORT loc_1_15C1
loc_1_15BC:
        call   sub_1_1270
loc_1_15C1:
        mov    ebx, eax
        or     eax, eax
        je     NEAR PTR loc_1_1754
        mov    dword ptr [esi + 10h], eax
loc_1_15CE:
        mov    al, byte ptr [esi + 8]
        and    al, 0f0h
        cmp    al, 0e0h
        je     SHORT loc_1_15E0
        mov    edi, dword ptr [ebp + 2ch]
        lea    eax, [ebp + 20h]
        jmp    SHORT loc_1_15E6
        db  090h
loc_1_15E0:
        mov    edi, dword ptr [ebp + 48h]
        lea    eax, [ebp + 3ch]
loc_1_15E6:
        mov    dword ptr [esp + 20h], eax
        test   byte ptr [ebp + 33h], 1
        jne    SHORT loc_1_161C
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 1ah]
        push   eax
        lea    eax, [esp + 1bh]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   edi
        push   dword ptr [esi + 8]
        db  0FFh, 015h, 0A0h, 095h, 000h, 000h
        add    esp, 18h
        mov    dword ptr [esp + 18h], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_1_1631
loc_1_161C:
        mov    eax, 1
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 0
loc_1_1631:
        or     eax, eax
        je     NEAR PTR loc_1_170C
        or     edi, edi
        je     SHORT loc_1_1641
        mov    dword ptr [esp + 18h], edi
loc_1_1641:
        mov    edi, dword ptr [ebx + 4]
        db  003h, 03Dh, 094h, 095h, 000h, 000h
        mov    dword ptr [ebx + 0ch], 8
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [ebx], eax
        mov    ax, word ptr [ebp + 30h]
        mov    word ptr [edi], ax
        mov    ax, word ptr [esi + 1eh]
        mov    word ptr [edi + 2], ax
        mov    ax, word ptr [esi + 1ch]
        add    ax, 8
        mov    word ptr [esp + 14h], ax
        mov    ah, al
        mov    al, byte ptr [esp + 15h]
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 6], 0
        test   byte ptr [ebp + 32h], 8
        je     SHORT loc_1_16C4
        push   ebx
        movzx  ecx, word ptr [esi + 0ah]
        movzx  edx, word ptr [esi + 8]
        add    ecx, edx
        movzx  edx, word ptr [esp + 1eh]
        add    ecx, edx
        movzx  edx, word ptr [esp + 1ch]
        add    ecx, edx
        and    eax, 0ffffh
        add    ecx, eax
        add    ecx, 1100h
        push   ecx
        call   sub_1_1D1C
        add    esp, 8
        cmp    ax, 0ffffh
        je     SHORT loc_1_16C0
        not    ax
loc_1_16C0:
        mov    word ptr [edi + 6], ax
loc_1_16C4:
        db  0FFh, 005h, 088h, 095h, 000h, 000h
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esi + 8]
        movzx  eax, word ptr [esp + 24h]
        push   eax
        push   ebx
        push   esi
        db  0FFh, 035h, 0E4h, 095h, 000h, 000h
        db  0FFh, 015h, 098h, 095h, 000h, 000h
        add    esp, 20h
        mov    edi, eax
        push   dword ptr [esp + 1ch]
        db  0FFh, 015h, 0A4h, 095h, 000h, 000h
        add    esp, 4
        cmp    edi, 2bf7h
        je     SHORT loc_1_173A
        jmp    SHORT loc_1_1730
        db  08Dh, 049h, 000h
loc_1_170C:
        mov    eax, 3
        cmp    byte ptr [esp + 13h], 0ffh
        je     SHORT loc_1_171D
        mov    eax, 1ch
loc_1_171D:
        push   0
        push   eax
        push   dword ptr [esi + 18h]
        call   dword ptr [esi + 14h]
        add    esp, 0ch
        mov    dword ptr [esi + 14h], 0
loc_1_1730:
        push   ebx
        push   esi
        call   sub_1_14E0
        add    esp, 8
loc_1_173A:
        lea    ecx, [ebp + 8]
        mov    esi, dword ptr [ecx]
        mov    eax, esi
        sub    eax, ebp
        cmp    eax, 8
        je     SHORT loc_1_1774
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        jmp    NEAR PTR loc_1_15A0
loc_1_1754:
        lea    ecx, [ebp + 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi], eax
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [ecx], esi
        push   ebp
        call   sub_1_13BC
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_1774:
        and    word ptr [ebp + 32h], 0fffbh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_1784:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        cmp    ebx, 0fff7h
        ja     NEAR PTR loc_1_18C0
        call   sub_1_13F8
        mov    edi, eax
        mov    ebp, dword ptr [esp + 14h]
        push   dword ptr [ebp]
        call   sub_1_01D4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_18A8
        test   byte ptr [esi + 32h], 80h
        je     NEAR PTR loc_1_18A8
        or     edi, edi
        je     NEAR PTR loc_1_189C
        lea    eax, [edi + 1eh]
        push   eax
        lea    eax, [edi + 8]
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 14h]
        call   sub_1_0580
        add    esp, 0ch
        or     al, al
        je     NEAR PTR loc_1_1888
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [edi + 1ch], bx
        mov    ax, word ptr [esi + 32h]
        test   al, 20h
        jne    SHORT loc_1_1864
        test   al, 40h
        jne    SHORT loc_1_185C
        db  0A1h, 00Ch, 096h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_1820
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 00Ch, 096h, 000h, 000h
        jmp    SHORT loc_1_1825
        db  090h
loc_1_1820:
        call   sub_1_1270
loc_1_1825:
        or     eax, eax
        mov    dword ptr [edi + 10h], eax
        je     SHORT loc_1_1850
        inc    word ptr [esi + 38h]
        push   edi
        push   esi
        call   sub_1_1590
        add    esp, 8
        push   esi
        call   sub_1_1240
        add    esp, 4
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_1850:
        push   esi
        call   sub_1_13BC
        add    esp, 4
        jmp    SHORT loc_1_1864
        db  090h
loc_1_185C:
        or     ax, 4
        mov    word ptr [esi + 32h], ax
loc_1_1864:
        add    esi, 8
        mov    dword ptr [edi + 10h], 0
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [edi], esi
        mov    eax, 0ffh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_1888:
        push   edi
        call   sub_1_1418
        add    esp, 4
        mov    eax, 3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_189C:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_18A8:
        or     edi, edi
        je     SHORT loc_1_18B5
        push   edi
        call   sub_1_1418
        add    esp, 4
loc_1_18B5:
        pop    ebp
        mov    eax, 6
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_18C0:
        pop    ebp
        mov    eax, 23h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_18CC:
        push   ebx
        push   esi
        push   edi
        call   sub_1_142C
        mov    edi, eax
        mov    ebx, dword ptr [esp + 10h]
        push   dword ptr [ebx]
        call   sub_1_01D4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_1994
        test   byte ptr [esi + 32h], 80h
        je     NEAR PTR loc_1_1994
        or     edi, edi
        je     NEAR PTR loc_1_1988
        mov    ecx, dword ptr [esp + 14h]
        or     ecx, ecx
        je     SHORT loc_1_1924
        cmp    dword ptr [ecx + 10h], 0
        je     SHORT loc_1_1924
        lea    eax, [edi + 1eh]
        push   eax
        lea    eax, [edi + 8]
        push   eax
        push   dword ptr [ecx + 14h]
        call   sub_1_0580
        add    esp, 0ch
        jmp    SHORT loc_1_1933
        db  090h
loc_1_1924:
        mov    al, 1
        mov    dword ptr [edi + 8], 0
        mov    word ptr [edi + 1eh], 0
loc_1_1933:
        or     al, al
        je     SHORT loc_1_1974
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 10h], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [edi + 18h], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [edi + 1ch], ax
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 1ch], edi
        add    esi, 18h
        mov    dword ptr [edi], esi
        mov    eax, 0ffh
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_1974:
        push   edi
        call   sub_1_144C
        add    esp, 4
        mov    eax, 3
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
loc_1_1988:
        pop    edi
        mov    eax, 1
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_1994:
        or     edi, edi
        je     SHORT loc_1_19A1
        push   edi
        call   sub_1_144C
        add    esp, 4
loc_1_19A1:
        pop    edi
        mov    eax, 6
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_19AC:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 10h], 0
        mov    esi, dword ptr [esp + 38h]
        test   byte ptr [esi + 32h], 80h
        je     NEAR PTR loc_1_1B30
        mov    edi, dword ptr [esi + 18h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -18h
        mov    ebx, dword ptr [esp + 3ch]
        mov    bp, word ptr [esp + 40h]
        je     SHORT loc_1_1A64
loc_1_19E6:
        mov    eax, dword ptr [edi + 8]
        or     eax, eax
        je     SHORT loc_1_19F1
        cmp    ebx, eax
        jne    SHORT loc_1_19FF
loc_1_19F1:
        mov    ax, word ptr [edi + 1eh]
        or     ax, ax
        je     SHORT loc_1_1A0C
        cmp    bp, ax
        je     SHORT loc_1_1A0C
loc_1_19FF:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -18h
        jne    SHORT loc_1_19E6
        jmp    SHORT loc_1_1A64
loc_1_1A0C:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
        push   8
        movzx  eax, word ptr [edi + 1ch]
        push   eax
        push   dword ptr [edi + 0ch]
        push   dword ptr [esp + 50h]
        call   sub_1_1DD0
        add    esp, 10h
        mov    esi, eax
        push   ebp
        push   ebx
        push   dword ptr [esp + 54h]
        push   dword ptr [edi + 10h]
        call   sub_1_1F9C
        add    esp, 10h
        db  0FFh, 005h, 07Ch, 095h, 000h, 000h
        push   esi
        push   eax
        push   dword ptr [edi + 18h]
        call   dword ptr [edi + 14h]
        add    esp, 0ch
        push   edi
        call   sub_1_144C
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
loc_1_1A64:
        mov    edi, dword ptr [esi + 84h]
        or     edi, edi
        je     NEAR PTR loc_1_1B20
        mov    eax, dword ptr [esi + 88h]
        mov    dword ptr [esp + 18h], eax
        inc    word ptr [esi + 38h]
        push   ebp
        push   ebx
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_1F70
        add    esp, 0ch
        db  0FFh, 005h, 07Ch, 095h, 000h, 000h
        lea    eax, [esp + 10h]
        push   eax
        mov    eax, dword ptr [esp + 48h]
        mov    eax, dword ptr [eax + 8]
        add    eax, 8
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 54h]
        sub    eax, 8
        push   eax
        mov    eax, dword ptr [esp + 54h]
        mov    eax, dword ptr [eax + 0ch]
        sub    eax, 8
        push   eax
        push   200h
        mov    eax, dword ptr [esp + 64h]
        push   dword ptr [eax]
        movzx  eax, byte ptr [eax + 8]
        push   eax
        lea    eax, [esp + 3ch]
        push   eax
        push   16h
        push   dword ptr [esp + 40h]
        call   edi
        add    esp, 2ch
        cmp    eax, 18h
        jne    SHORT loc_1_1B0F
        mov    eax, dword ptr [esp + 14h]
        add    eax, 8
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   dword ptr [esp + 50h]
        call   sub_1_1DD0
        add    esp, 10h
        push   eax
        push   0
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax + 0ch]
        call   dword ptr [eax + 8]
        add    esp, 0ch
loc_1_1B0F:
        push   esi
        call   sub_1_120C
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_1_1B20:
        pop    ebp
        db  0FFh, 005h, 084h, 095h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Bh, 0FFh
loc_1_1B30:
        pop    ebp
        db  0FFh, 005h, 084h, 095h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Bh, 0FFh
loc_1_1B40:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 2ch]
        db  0FFh, 015h, 0A8h, 095h, 000h, 000h
        add    esp, 4
        test   al, 1
        je     SHORT loc_1_1B78
        cmp    dword ptr [esp + 2ch], 0
        jne    SHORT loc_1_1B66
        cmp    byte ptr [esp + 3ch], 0
        jne    SHORT loc_1_1B78
loc_1_1B66:
        sub    eax, eax
        db  0FFh, 005h, 084h, 095h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_1B78:
        mov    esi, dword ptr [esp + 34h]
        mov    edi, dword ptr [esi + 8]
        mov    ax, word ptr [edi + 4]
        mov    bx, ax
        and    ebx, 0ffh
        shl    ebx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ebx, eax
        cmp    ebx, 8
        jae    SHORT loc_1_1BB0
        db  0FFh, 005h, 084h, 095h, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_1BB0:
        cmp    dword ptr [esp + 38h], ebx
        je     SHORT loc_1_1BD6
        jbe    SHORT loc_1_1C28
        mov    ecx, esi
        mov    edx, ebx
        or     esi, esi
        je     SHORT loc_1_1BD6
loc_1_1BC0:
        mov    eax, dword ptr [ecx + 0ch]
        sub    eax, edx
        sbb    ebp, ebp
        and    eax, ebp
        add    eax, edx
        mov    dword ptr [ecx + 0ch], eax
        sub    edx, eax
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_1BC0
loc_1_1BD6:
        cmp    word ptr [edi + 6], 0
        je     SHORT loc_1_1C38
        push   esi
        mov    ah, bl
        mov    al, bh
        movzx  ecx, ax
        movzx  eax, word ptr [esp + 32h]
        add    ecx, eax
        movzx  eax, word ptr [esp + 30h]
        add    ecx, eax
        movzx  eax, word ptr [esp + 2eh]
        add    ecx, eax
        movzx  eax, word ptr [esp + 2ch]
        add    ecx, eax
        add    ecx, 1100h
        push   ecx
        call   sub_1_1D9C
        add    esp, 8
        cmp    ax, 0ffffh
        je     SHORT loc_1_1C38
        db  0FFh, 005h, 084h, 095h, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_1C28:
        sub    eax, eax
        db  0FFh, 005h, 084h, 095h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1C38:
        cmp    byte ptr [esp + 3ch], 0
        jne    SHORT loc_1_1C94
        push   0
        push   11h
        mov    ax, word ptr [edi + 2]
        push   eax
        push   dword ptr [esp + 34h]
        call   sub_1_0414
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1C80
        push   dword ptr [esp + 10h]
        mov    ebp, dword ptr [esp + 48h]
        push   ebp
        push   ebx
        push   esi
        mov    cx, word ptr [edi]
        push   ecx
        push   dword ptr [esp + 40h]
        push   eax
        call   sub_1_19AC
        add    esp, 1ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_1C80:
        pop    ebp
        db  0FFh, 005h, 080h, 095h, 000h, 000h
        mov    eax, 2b2ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_1C94:
        lea    eax, [esp + 14h]
        push   eax
        push   11h
        mov    ax, word ptr [edi + 2]
        push   eax
        push   dword ptr [esp + 3ch]
        call   sub_1_04A0
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_1CC0
        db  0FFh, 005h, 080h, 095h, 000h, 000h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_1CC0:
        mov    ebp, dword ptr [esp + 44h]
loc_1_1CC4:
        mov    cx, word ptr [edi]
        push   dword ptr [esp + 10h]
        push   ebp
        push   ebx
        push   esi
        push   ecx
        push   dword ptr [esp + 40h]
        push   eax
        call   sub_1_19AC
        add    esp, 1ch
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_0474
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1CC4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_1CF8:
        cmp    byte ptr [esp + 4], 1
        jne    SHORT loc_1_1D1B
        cmp    dword ptr [esp + 8], 2b0eh
        jne    SHORT loc_1_1D1B
        push   0
        push   0
        push   0
        push   0
        push   11h
        call   0
        add    esp, 14h
loc_1_1D1B:
        ret
sub_1_1D1C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 01Dh, 094h, 095h, 000h, 000h
        sub    ebp, ebp
        sub    esi, esi
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, ecx
        shl    eax, 10h
        mov    edx, ecx
        shr    edx, 10h
        or     eax, edx
        add    eax, ecx
        shr    eax, 10h
        inc    edi
        mov    edi, dword ptr [esp + 18h]
loc_1_1D44:
        test   ebp, 1
        je     SHORT loc_1_1D61
        mov    ecx, eax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        mov    eax, ecx
        xor    esi, 1
loc_1_1D61:
        mov    ecx, dword ptr [edi + 4]
        add    ecx, ebx
        push   dword ptr [edi + 0ch]
        push   ecx
        push   eax
        call   sub_1_8F49
        add    esp, 0ch
        sub    ebx, ebx
        mov    ebp, dword ptr [edi + 0ch]
        mov    edi, dword ptr [edi]
        cmp    edi, ebx
        jne    SHORT loc_1_1D44
        cmp    esi, ebx
        je     SHORT loc_1_1D94
        mov    ecx, eax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        mov    eax, ecx
loc_1_1D94:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_1D9C:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, ecx
        shl    eax, 10h
        mov    edx, ecx
        shr    edx, 10h
        or     eax, edx
        add    eax, ecx
        shr    eax, 10h
        mov    esi, dword ptr [esp + 0ch]
loc_1_1DB6:
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   eax
        call   sub_1_8F49
        add    esp, 0ch
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_1DB6
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_1DD0:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ebx, dword ptr [esp + 24h]
        cmp    ebx, eax
        je     NEAR PTR loc_1_1E8A
        mov    dword ptr [esp + 10h], eax
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [esp + 14h], ecx
        mov    edx, dword ptr [esp + 20h]
        inc    ecx
        mov    ecx, dword ptr [edx + 0ch]
        sub    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 18h], ecx
loc_1_1E00:
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 14h]
        sub    edi, ecx
        sbb    esi, esi
        and    edi, esi
        add    edi, ecx
        mov    ecx, dword ptr [esp + 28h]
        sub    edi, ecx
        sbb    esi, esi
        and    edi, esi
        lea    ebp, [edi + ecx]
        mov    esi, dword ptr [edx + 8]
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [esp + 10h]
        mov    ecx, ebp
        shr    ecx, 2
        add    esi, dword ptr [esp + 2ch]
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    eax, ebp
        sub    dword ptr [esp + 18h], ebp
        jne    SHORT loc_1_1E58
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     SHORT loc_1_1E8A
        mov    dword ptr [esp + 2ch], 0
        mov    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [esp + 18h], ecx
        jmp    SHORT loc_1_1E5C
loc_1_1E58:
        add    dword ptr [esp + 2ch], ebp
loc_1_1E5C:
        sub    dword ptr [esp + 14h], ebp
        jne    SHORT loc_1_1E7C
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_1_1E8A
        mov    dword ptr [esp + 10h], 0
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [esp + 14h], ecx
        jmp    SHORT loc_1_1E80
        db  08Dh, 049h, 000h
loc_1_1E7C:
        add    dword ptr [esp + 10h], ebp
loc_1_1E80:
        sub    dword ptr [esp + 28h], ebp
        jne    NEAR PTR loc_1_1E00
loc_1_1E8A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
sub_1_1E94:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [esp + 20h]
        cmp    dword ptr [ebx + 0ch], ebp
        ja     SHORT loc_1_1EAF
loc_1_1EA5:
        sub    ebp, dword ptr [ebx + 0ch]
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx + 0ch], ebp
        jbe    SHORT loc_1_1EA5
loc_1_1EAF:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_1EF2
loc_1_1EB6:
        mov    edx, dword ptr [ebx + 0ch]
        sub    edx, ebp
        mov    eax, edx
        mov    ecx, dword ptr [esp + 1ch]
        sub    eax, ecx
        sbb    esi, esi
        and    eax, esi
        add    eax, ecx
        mov    esi, dword ptr [ebx + 8]
        mov    edi, dword ptr [esp + 14h]
        add    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    edx, eax
        jne    SHORT loc_1_1EE8
        mov    ebx, dword ptr [ebx]
        sub    ebp, ebp
loc_1_1EE8:
        add    dword ptr [esp + 14h], eax
        sub    dword ptr [esp + 1ch], eax
        jne    SHORT loc_1_1EB6
loc_1_1EF2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_1EF8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 24h]
        mov    esi, dword ptr [ecx]
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [eax + 4]
        add    edx, esi
        mov    ecx, dword ptr [eax + 0ch]
        sub    ecx, esi
        mov    dword ptr [esp + 10h], ecx
        inc    ebx
        mov    ebx, dword ptr [esp + 20h]
loc_1_1F1C:
        mov    edi, edx
        mov    ecx, dword ptr [esp + 10h]
        sub    ecx, ebx
        sbb    esi, esi
        and    ecx, esi
        lea    ebp, [ecx + ebx]
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    edx, ebp
        add    dword ptr [esp + 1ch], ebp
        sub    ebx, ebp
        je     SHORT loc_1_1F5C
        sub    dword ptr [esp + 10h], ebp
        jne    SHORT loc_1_1F1C
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 10h], ecx
        jmp    SHORT loc_1_1F1C
        db  090h
loc_1_1F5C:
        sub    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_1F70:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], 1
        add    ecx, 4
        mov    word ptr [ecx + 2], 2
        mov    word ptr [ecx], 0eh
        mov    ax, word ptr [esp + 0ch]
        mov    word ptr [ecx + 4], ax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 6], eax
        ret
        db  08Dh, 049h, 000h
sub_1_1F9C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    edx, dword ptr [esp + 14h]
        cmp    edx, ebx
        je     SHORT loc_1_201C
        mov    dword ptr [edx], ebx
        mov    ebp, dword ptr [edx + 8]
        cmp    ebp, ebx
        je     SHORT loc_1_1FE0
        mov    ecx, dword ptr [esp + 18h]
        movzx  eax, byte ptr [ecx + 8]
        cmp    eax, ebp
        jle    SHORT loc_1_1FC8
        mov    ebx, 9
        jmp    SHORT loc_1_1FCA
        db  08Bh, 0FFh
loc_1_1FC8:
        mov    ebp, eax
loc_1_1FCA:
        mov    esi, dword ptr [ecx]
        mov    edi, dword ptr [edx + 0ch]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    dword ptr [edx + 8], ebp
loc_1_1FE0:
        mov    eax, dword ptr [edx + 10h]
        or     eax, eax
        je     SHORT loc_1_201C
        cmp    eax, 16h
        jb     SHORT loc_1_2010
        mov    dword ptr [edx + 10h], 16h
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   dword ptr [edx + 14h]
        call   sub_1_1F70
        add    esp, 0ch
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_2010:
        mov    ebx, 1ah
        mov    dword ptr [edx + 10h], 0
loc_1_201C:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_2024:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        test   byte ptr [esi + 58h], 1
        jne    NEAR PTR loc_1_20F5
        lea    eax, [esp + 0eh]
        push   eax
        push   dword ptr [esi + 68h]
        db  0FFh, 015h, 0ACh, 095h, 000h, 000h
        add    esp, 8
        or     al, al
        je     NEAR PTR loc_1_20F5
        sub    word ptr [esp + 0eh], 14h
        mov    ax, word ptr [esi + 80h]
        mov    cx, word ptr [esp + 0eh]
        sub    ax, cx
        sbb    dx, dx
        and    ax, dx
        add    ax, cx
        mov    word ptr [esp + 0eh], ax
        mov    ebx, dword ptr [esi + 94h]
        movzx  edi, ax
        mov    eax, ebx
        sub    edx, edx
        div    edi
        cmp    eax, 4
        jae    SHORT loc_1_20D8
        cmp    word ptr [esp + 0eh], 3fffh
        ja     SHORT loc_1_20A0
        lea    eax, [edi*4]
        mov    dword ptr [esi + 94h], eax
        jmp    SHORT loc_1_20C6
loc_1_20A0:
        sub    edx, edx
        mov    eax, 0ffffh
        div    edi
        cmp    eax, 1
        je     SHORT loc_1_20BC
        imul   eax, edi
        mov    dword ptr [esi + 94h], eax
        jmp    SHORT loc_1_20C6
        db  08Dh, 049h, 000h
loc_1_20BC:
        mov    dword ptr [esi + 94h], 0ffffh
loc_1_20C6:
        pop    edi
        mov    eax, dword ptr [esi + 94h]
        mov    dword ptr [esi + 44h], eax
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_20D8:
        mov    ecx, edi
        imul   ecx, eax
        cmp    ecx, ebx
        je     SHORT loc_1_20F5
        inc    eax
        imul   eax, edi
        cmp    eax, 0ffffh
        ja     SHORT loc_1_20F5
        mov    dword ptr [esi + 94h], eax
        mov    dword ptr [esi + 44h], eax
loc_1_20F5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_20FC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        test   byte ptr [esi + 86h], 1
        jne    NEAR PTR loc_1_22C1
        mov    eax, dword ptr [esi + 58h]
        test   al, 40h
        jne    NEAR PTR loc_1_22C1
        cmp    dword ptr [esi + 0a0h], 0
        je     NEAR PTR loc_1_22C1
        or     eax, 40h
        mov    dword ptr [esi + 58h], eax
loc_1_2131:
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 0a0h]
loc_1_2139:
        test   byte ptr [edi + 17h], 80h
        je     SHORT loc_1_2190
        mov    eax, dword ptr [edi]
        cmp    dword ptr [esi + 50h], edi
        jne    SHORT loc_1_2149
        mov    dword ptr [esi + 50h], eax
loc_1_2149:
        or     eax, eax
        mov    dword ptr [esi + 0a0h], eax
        jne    SHORT loc_1_215A
        db  0C7h, 046h, 060h, 0D4h, 042h, 000h, 000h
loc_1_215A:
        mov    eax, dword ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_1_2166
        mov    word ptr [eax], 420h
loc_1_2166:
        push   dword ptr [edi + 0ch]
        push   0
        push   dword ptr [edi + 8]
        call   dword ptr [edi + 4]
        add    esp, 0ch
        or     ebx, ebx
        je     SHORT loc_1_2184
        push   edi
        call   sub_1_408C
        add    esp, 4
        jmp    SHORT loc_1_2186
        db  090h
loc_1_2184:
        mov    ebx, edi
loc_1_2186:
        mov    edi, dword ptr [esi + 0a0h]
        or     edi, edi
        jne    SHORT loc_1_2139
loc_1_2190:
        or     edi, edi
        jne    NEAR PTR loc_1_22B0
        mov    byte ptr [esi + 83h], 0
        cmp    dword ptr [esi + 0ach], 0
        je     NEAR PTR loc_1_222C
        cmp    dword ptr [esi + 0dch], 0
        je     NEAR PTR loc_1_229C
        cmp    dword ptr [esi + 54h], 0
        jne    NEAR PTR loc_1_229C
        push   ebx
        push   esi
        call   sub_1_487C
        add    esp, 8
        push   esi
        call   sub_1_80C8
        add    esp, 4
        cmp    dword ptr [esi + 0a0h], 0
        jne    NEAR PTR loc_1_2131
        cmp    dword ptr [esi + 0ach], 0
        jne    NEAR PTR loc_1_22BD
        test   dword ptr [esi + 58h], 1100000h
        je     NEAR PTR loc_1_22BD
loc_1_21FD:
        mov    eax, dword ptr [esi + 58h]
        test   eax, 1000000h
        je     SHORT loc_1_2274
        and    eax, 0ffffffbfh
        mov    dword ptr [esi + 58h], eax
        inc    dword ptr [esi + 74h]
        push   dword ptr [esp + 0ch]
        push   1
        and    eax, 200000h
        push   eax
        push   esi
        call   sub_1_70A4
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_222C:
        push   ebx
        call   sub_1_408C
        add    esp, 4
        test   dword ptr [esi + 58h], 1100000h
        jne    SHORT loc_1_21FD
        cmp    dword ptr [esi + 0dch], 0
        je     SHORT loc_1_2260
        cmp    dword ptr [esi + 54h], 0
        jne    SHORT loc_1_2260
        db  0C7h, 046h, 060h, 020h, 046h, 000h, 000h
        and    dword ptr [esi + 58h], 0ffffffbfh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_2260:
        pop    edi
        db  0C7h, 046h, 060h, 0D4h, 042h, 000h, 000h
        and    dword ptr [esi + 58h], 0ffffffbfh
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_2274:
        test   eax, 100000h
        je     SHORT loc_1_22C1
        and    eax, 0ffefffffh
        mov    dword ptr [esi + 58h], eax
        push   16h
        push   0
        push   esi
        call   sub_1_6FB8
        add    esp, 0ch
        and    dword ptr [esi + 58h], 0ffffffbfh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
loc_1_229C:
        push   ebx
        call   sub_1_408C
        add    esp, 4
        and    dword ptr [esi + 58h], 0ffffffbfh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_22B0:
        or     ebx, ebx
        je     SHORT loc_1_22BD
        push   ebx
        call   sub_1_408C
        add    esp, 4
loc_1_22BD:
        and    dword ptr [esi + 58h], 0ffffffbfh
loc_1_22C1:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_22C8:
        sub    esp, 4
        push   esi
        push   edi
        db  081h, 03Dh, 0DCh, 095h, 000h, 000h, 0DCh, 095h, 000h, 000h
        je     NEAR PTR loc_1_2382
        mov    edi, dword ptr [esp + 8]
loc_1_22E1:
        db  08Bh, 035h, 0DCh, 095h, 000h, 000h
        mov    eax, esi
        mov    eax, dword ptr [eax]
        db  0A3h, 0DCh, 095h, 000h, 000h
        mov    eax, dword ptr [esi]
        db  0C7h, 040h, 004h, 0DCh, 095h, 000h, 000h
        sub    esi, 0d0h
        test   byte ptr [esi + 86h], 1
        jne    SHORT loc_1_2364
loc_1_2308:
        mov    eax, dword ptr [esi + 58h]
        test   ah, 7
        je     SHORT loc_1_2364
        test   ah, 1
        je     SHORT loc_1_2326
        and    eax, 0fffffeffh
        mov    dword ptr [esi + 58h], eax
        push   esi
        call   sub_1_20FC
        add    esp, 4
loc_1_2326:
        mov    eax, dword ptr [esi + 58h]
        test   ah, 4
        je     SHORT loc_1_2342
        and    eax, 0fffffbffh
        mov    dword ptr [esi + 58h], eax
        inc    dword ptr [esi + 74h]
        push   esi
        call   sub_1_8824
        add    esp, 4
loc_1_2342:
        mov    eax, dword ptr [esi + 58h]
        test   ah, 2
        je     SHORT loc_1_235B
        and    eax, 0fffffdffh
        mov    dword ptr [esi + 58h], eax
        push   esi
        call   sub_1_80C8
        add    esp, 4
loc_1_235B:
        test   byte ptr [esi + 86h], 1
        je     SHORT loc_1_2308
loc_1_2364:
        push   edi
        and    dword ptr [esi + 58h], 0ffffffdfh
        push   esi
        call   sub_1_63D8
        add    esp, 8
        db  081h, 03Dh, 0DCh, 095h, 000h, 000h, 0DCh, 095h, 000h, 000h
        jne    NEAR PTR loc_1_22E1
loc_1_2382:
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_1_2388:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [edx + 58h]
        or     eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 58h], eax
        test   al, 20h
        jne    SHORT loc_1_23C7
        or     eax, 20h
        mov    dword ptr [edx + 58h], eax
        inc    dword ptr [edx + 74h]
        lea    ecx, [edx + 0d0h]
        db  0A1h, 0E0h, 095h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A1h, 0E0h, 095h, 000h, 000h
        mov    dword ptr [edx + 0d4h], eax
        db  089h, 00Dh, 0E0h, 095h, 000h, 000h
        db  0C7h, 001h, 0DCh, 095h, 000h, 000h
loc_1_23C7:
        ret
sub_1_23C8:
        push   esi
        db  0F6h, 005h, 050h, 096h, 000h, 000h, 003h
        je     NEAR PTR loc_1_24A7
        db  0F6h, 005h, 050h, 096h, 000h, 000h, 010h
        jne    NEAR PTR loc_1_24A7
        db  083h, 00Dh, 050h, 096h, 000h, 000h, 010h
loc_1_23EA:
        db  0F6h, 005h, 050h, 096h, 000h, 000h, 001h
        je     SHORT loc_1_2437
        db  081h, 03Dh, 044h, 097h, 000h, 000h, 044h, 097h, 000h, 000h
        je     SHORT loc_1_2430
        db  08Bh, 035h, 044h, 097h, 000h, 000h
        mov    eax, esi
        mov    eax, dword ptr [eax]
        db  0A3h, 044h, 097h, 000h, 000h
        mov    eax, dword ptr [esi]
        db  0C7h, 040h, 004h, 044h, 097h, 000h, 000h
        push   0
        push   dword ptr [esi + 10h]
        push   dword ptr [esi + 0ch]
        call   dword ptr [esi + 8]
        add    esp, 0ch
        push   esi
        call   sub_1_5EE8
        add    esp, 4
        jmp    SHORT loc_1_2437
loc_1_2430:
        db  083h, 025h, 050h, 096h, 000h, 000h, 0FEh
loc_1_2437:
        db  0F6h, 005h, 050h, 096h, 000h, 000h, 002h
        je     SHORT loc_1_2493
        db  081h, 03Dh, 010h, 096h, 000h, 000h, 010h, 096h, 000h, 000h
        je     SHORT loc_1_248C
        db  08Bh, 035h, 010h, 096h, 000h, 000h
        mov    eax, esi
        mov    eax, dword ptr [eax]
        db  0A3h, 010h, 096h, 000h, 000h
        mov    eax, dword ptr [esi]
        db  0C7h, 040h, 004h, 010h, 096h, 000h, 000h
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        mov    ecx, 0
        jne    SHORT loc_1_2473
        mov    ecx, dword ptr [esi + 14h]
loc_1_2473:
        push   ecx
        push   eax
        push   dword ptr [esi + 0ch]
        call   dword ptr [esi + 8]
        add    esp, 0ch
        push   esi
        call   sub_1_7CD4
        add    esp, 4
        jmp    SHORT loc_1_2493
        db  08Dh, 049h, 000h
loc_1_248C:
        db  083h, 025h, 050h, 096h, 000h, 000h, 0FDh
loc_1_2493:
        db  0F6h, 005h, 050h, 096h, 000h, 000h, 003h
        jne    NEAR PTR loc_1_23EA
        db  083h, 025h, 050h, 096h, 000h, 000h, 0EFh
loc_1_24A7:
        call   sub_1_22C8
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_24B0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0b4h]
        or     esi, esi
        je     SHORT loc_1_2509
        mov    dword ptr [eax + 0b4h], 0
        mov    cx, word ptr [eax + 6ch]
        push   ecx
        push   dword ptr [eax + 64h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esi + 14h]
        call   sub_1_1F9C
        add    esp, 10h
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 10h], eax
        db  083h, 00Dh, 050h, 096h, 000h, 000h, 001h
        db  0A1h, 048h, 097h, 000h, 000h
        mov    dword ptr [eax], esi
        db  0A1h, 048h, 097h, 000h, 000h
        mov    dword ptr [esi + 4], eax
        db  089h, 035h, 048h, 097h, 000h, 000h
        db  0C7h, 006h, 044h, 097h, 000h, 000h
loc_1_2509:
        pop    esi
        ret
        db  090h
sub_1_250C:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 4ch]
        test   byte ptr [edi + 32h], 80h
        jne    SHORT loc_1_252C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 049h, 000h
loc_1_252C:
        mov    edx, dword ptr [edi + 4ch]
        or     edx, edx
        mov    si, word ptr [esp + 58h]
        je     SHORT loc_1_257C
loc_1_2538:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_256B
        cmp    byte ptr [eax + 84h], 1
        jne    SHORT loc_1_256B
        test   byte ptr [eax + 86h], 1
        jne    SHORT loc_1_256B
        mov    ecx, dword ptr [eax + 64h]
        or     ecx, ecx
        je     SHORT loc_1_255D
        cmp    dword ptr [esp + 54h], ecx
        jne    SHORT loc_1_256B
loc_1_255D:
        mov    cx, word ptr [eax + 6ch]
        or     cx, cx
        je     SHORT loc_1_2574
        cmp    si, cx
        je     SHORT loc_1_2574
loc_1_256B:
        mov    edx, dword ptr [edx + 8]
        or     edx, edx
        jne    SHORT loc_1_2538
        jmp    SHORT loc_1_2580
loc_1_2574:
        mov    ebx, 1
        jmp    SHORT loc_1_2580
        db  090h
loc_1_257C:
        mov    eax, dword ptr [esp + 1ch]
loc_1_2580:
        or     ebx, ebx
        je     SHORT loc_1_25A4
        inc    dword ptr [eax + 74h]
        mov    ecx, dword ptr [eax + 0b4h]
        test   byte ptr [ecx + 18h], 1
        jne    NEAR PTR loc_1_2710
        or     dword ptr [eax + 58h], 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  090h
loc_1_25A4:
        cmp    dword ptr [edi + 64h], 0
        je     NEAR PTR loc_1_270E
        call   sub_1_5D70
        mov    ebx, eax
        call   sub_1_5EFC
        mov    ebp, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_26F4
        or     ebp, ebp
        je     NEAR PTR loc_1_2701
        mov    eax, dword ptr [edi + 64h]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [edi + 68h]
        mov    dword ptr [esp + 18h], eax
        push   esi
        push   dword ptr [esp + 58h]
        lea    eax, [esp + 38h]
        push   eax
        call   sub_1_1F70
        add    esp, 0ch
        inc    word ptr [edi + 38h]
        mov    byte ptr [ebx + 84h], 1
        mov    dword ptr [ebx + 0b4h], ebp
        or     dword ptr [ebx + 58h], 4
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        mov    esi, dword ptr [esp + 64h]
        push   dword ptr [esi]
        movzx  eax, byte ptr [esi + 8]
        push   eax
        push   0
        push   0
        lea    eax, [esp + 48h]
        push   eax
        push   16h
        push   dword ptr [esp + 38h]
        call   dword ptr [esp + 38h]
        add    esp, 24h
        cmp    eax, 18h
        jne    NEAR PTR loc_1_26CC
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebp + 14h], eax
        mov    ebp, dword ptr [edi + 4ch]
        mov    eax, 0fh
        or     ebp, ebp
        je     SHORT loc_1_26A1
loc_1_2656:
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [ebp + 10h], ecx
        jne    SHORT loc_1_2665
        test   byte ptr [ebp + 0dh], 3
        je     SHORT loc_1_2670
loc_1_2665:
        mov    ebp, dword ptr [ebp + 8]
        or     ebp, ebp
        jne    SHORT loc_1_2656
        jmp    SHORT loc_1_269D
        db  08Bh, 0FFh
loc_1_2670:
        mov    dword ptr [ebx + 74h], 1
        push   dword ptr [esp + 28h]
        push   ebx
        push   ebp
        call   sub_1_66C8
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_2690
        sub    ebp, ebp
        jmp    SHORT loc_1_269D
        db  08Dh, 049h, 000h
loc_1_2690:
        mov    dword ptr [ebp], ebx
        mov    dword ptr [ebx + 0cch], ebp
        inc    word ptr [ebp + 0eh]
loc_1_269D:
        or     ebp, ebp
        jne    SHORT loc_1_26B7
loc_1_26A1:
        push   eax
        push   esi
        push   ebx
        call   sub_1_24B0
        add    esp, 0ch
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        sub    ebx, ebx
loc_1_26B7:
        push   edi
        call   sub_1_120C
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Bh, 0FFh
loc_1_26CC:
        push   ebp
        call   sub_1_5EE8
        add    esp, 4
        push   ebx
        call   sub_1_5E04
        add    esp, 4
        push   edi
        call   sub_1_120C
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
        db  08Dh, 049h, 000h
loc_1_26F4:
        or     ebp, ebp
        je     SHORT loc_1_2701
        push   ebp
        call   sub_1_5EE8
        add    esp, 4
loc_1_2701:
        or     ebx, ebx
        je     SHORT loc_1_270E
        push   ebx
        call   sub_1_5E04
        add    esp, 4
loc_1_270E:
        sub    eax, eax
loc_1_2710:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
sub_1_2718:
        mov    edx, dword ptr [esp + 4]
        mov    al, byte ptr [edx + 0ch]
        and    al, 0f3h
        shr    al, 2
        movzx  ecx, al
        sub    ecx, 14h
        add    edx, 14h
        or     ecx, ecx
        je     SHORT loc_1_275C
loc_1_2731:
        mov    al, byte ptr [edx]
        or     al, al
        je     SHORT loc_1_275C
        cmp    al, 1
        jne    SHORT loc_1_2740
        inc    edx
        dec    ecx
        jmp    SHORT loc_1_2758
        db  090h
loc_1_2740:
        cmp    al, 2
        je     SHORT loc_1_2764
        mov    al, byte ptr [edx + 1]
        or     al, al
        je     SHORT loc_1_275C
        and    eax, 0ffh
        cmp    ecx, eax
        jb     SHORT loc_1_275C
        sub    ecx, eax
        add    edx, eax
loc_1_2758:
        or     ecx, ecx
        jne    SHORT loc_1_2731
loc_1_275C:
        mov    ax, 218h
        ret
        db  08Dh, 049h, 000h
loc_1_2764:
        cmp    byte ptr [edx + 1], 4
        jne    SHORT loc_1_275C
        mov    cx, word ptr [edx + 2]
        or     cx, cx
        je     SHORT loc_1_275C
        mov    ah, cl
        mov    al, ch
        ret
sub_1_2778:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        test   byte ptr [eax + 11h], 4
        mov    esi, dword ptr [esp + 10h]
        jne    SHORT loc_1_27A5
        cmp    byte ptr [esi + 84h], 0ah
        jne    SHORT loc_1_279C
        mov    word ptr [esi + 90h], 960h
loc_1_279C:
        push   esi
        call   sub_1_80C8
        add    esp, 4
loc_1_27A5:
        push   dword ptr [esp + 4]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        pop    esi
        add    esp, 4
        ret
        db  090h
sub_1_27B8:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    ebx, dword ptr [esp + 28h]
        sub    ebx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esp + 14h], eax
        cmp    ebx, eax
        jne    NEAR PTR loc_1_28A4
        cmp    byte ptr [esi + 84h], 4
        jne    NEAR PTR loc_1_28A4
        lea    edx, [esi + 38h]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [edx], edx
        mov    dword ptr [esi + 3ch], edx
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 28h], 0
        mov    dword ptr [esi + 2ch], 0
        mov    dword ptr [esi + 30h], 0
        mov    dword ptr [esi + 34h], 0
        mov    dword ptr [esi + 14h], 0
loc_1_281D:
        mov    edi, ecx
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edi + 10h], 0
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        or     eax, eax
        jg     SHORT loc_1_2891
        mov    eax, dword ptr [edi + 2ch]
        or     eax, eax
        je     SHORT loc_1_2847
        mov    dword ptr [eax], 0
        mov    dword ptr [edi + 2ch], 0
loc_1_2847:
        db  0A1h, 06Ch, 096h, 000h, 000h
        sub    eax, dword ptr [edi + 30h]
        add    dword ptr [esi + 0c4h], eax
        mov    ebx, dword ptr [esi + 0bch]
        mov    eax, dword ptr [edi + 14h]
        add    eax, ebx
        mov    dword ptr [esi + 0bch], eax
        cmp    ebx, eax
        mov    eax, 0
        seta   al
        add    dword ptr [esi + 0c0h], eax
        db  0A1h, 014h, 096h, 000h, 000h
        mov    dword ptr [eax], edi
        db  0A1h, 014h, 096h, 000h, 000h
        mov    dword ptr [edi + 4], eax
        db  089h, 03Dh, 014h, 096h, 000h, 000h
        db  0C7h, 007h, 010h, 096h, 000h, 000h
loc_1_2891:
        cmp    ecx, edx
        jne    SHORT loc_1_281D
        db  083h, 00Dh, 050h, 096h, 000h, 000h, 002h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_28A4:
        mov    ecx, dword ptr [esi + 38h]
        mov    dword ptr [esp + 18h], ebx
        mov    edx, ecx
        mov    dword ptr [esp + 10h], 0
        or     ebx, ebx
        mov    edi, dword ptr [esp + 1ch]
        jle    SHORT loc_1_290C
loc_1_28BD:
        lea    eax, [esi + 38h]
        cmp    edx, eax
        je     SHORT loc_1_290C
        mov    edi, edx
        mov    eax, dword ptr [edx + 20h]
        cmp    ebx, eax
        jl     SHORT loc_1_28DC
        sub    ebx, eax
        mov    dword ptr [esp + 10h], edx
        mov    edx, dword ptr [edx]
        or     ebx, ebx
        jg     SHORT loc_1_28BD
        jmp    SHORT loc_1_290C
        db  090h
loc_1_28DC:
        mov    ebp, dword ptr [edi + 28h]
        sub    dword ptr [edi + 20h], ebx
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [edi + 24h]
        cmp    ebx, eax
        jge    SHORT loc_1_28F8
        add    ebx, dword ptr [edi + 24h]
        mov    dword ptr [edi + 24h], ebx
        sub    ebx, ebx
        jmp    SHORT loc_1_290C
        db  08Bh, 0FFh
loc_1_28F8:
        sub    ebx, eax
        mov    ebp, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    ebx, eax
        jge    SHORT loc_1_28F8
        mov    dword ptr [edi + 24h], ebx
        mov    dword ptr [edi + 28h], ebp
        sub    ebx, ebx
loc_1_290C:
        mov    eax, dword ptr [esp + 28h]
        sub    eax, dword ptr [esi + 8]
        or     eax, eax
        jle    SHORT loc_1_295B
        lea    eax, [esi + 38h]
        cmp    edx, eax
        je     SHORT loc_1_2938
        mov    edi, edx
        mov    dword ptr [esi + 28h], edx
        mov    eax, dword ptr [edx + 28h]
        mov    dword ptr [esi + 2ch], eax
        mov    eax, dword ptr [edx + 24h]
        mov    dword ptr [esi + 30h], eax
        mov    eax, dword ptr [edx + 20h]
        mov    dword ptr [esi + 34h], eax
        jmp    SHORT loc_1_2954
        db  090h
loc_1_2938:
        mov    dword ptr [esi + 28h], 0
        mov    dword ptr [esi + 2ch], 0
        mov    dword ptr [esi + 30h], 0
        mov    dword ptr [esi + 34h], 0
loc_1_2954:
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esi + 8], eax
loc_1_295B:
        sub    ebx, dword ptr [esp + 18h]
        add    ebx, dword ptr [esp + 14h]
        mov    dword ptr [esi + 14h], ebx
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esi + 4], eax
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_2A14
        lea    eax, [esi + 38h]
        cmp    edx, eax
        jne    SHORT loc_1_2988
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 3ch], eax
        jmp    SHORT loc_1_2991
        db  08Bh, 0FFh
loc_1_2988:
        mov    dword ptr [esi + 38h], edx
        lea    eax, [esi + 38h]
        mov    dword ptr [edx + 4], eax
loc_1_2991:
        cmp    edx, ecx
        je     SHORT loc_1_2A0D
loc_1_2995:
        mov    ebx, dword ptr [ecx]
        mov    dword ptr [ecx + 10h], 0
        mov    eax, dword ptr [ecx + 18h]
        dec    eax
        mov    dword ptr [ecx + 18h], eax
        or     eax, eax
        jg     SHORT loc_1_2A07
        mov    eax, dword ptr [ecx + 2ch]
        or     eax, eax
        je     SHORT loc_1_29BD
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx + 2ch], 0
loc_1_29BD:
        db  0A1h, 06Ch, 096h, 000h, 000h
        sub    eax, dword ptr [edi + 30h]
        add    dword ptr [esi + 0c4h], eax
        mov    ebp, dword ptr [esi + 0bch]
        mov    eax, dword ptr [edi + 14h]
        add    eax, ebp
        mov    dword ptr [esi + 0bch], eax
        cmp    ebp, eax
        mov    eax, 0
        seta   al
        add    dword ptr [esi + 0c0h], eax
        db  0A1h, 014h, 096h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A1h, 014h, 096h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  089h, 00Dh, 014h, 096h, 000h, 000h
        db  0C7h, 001h, 010h, 096h, 000h, 000h
loc_1_2A07:
        mov    ecx, ebx
        cmp    ebx, edx
        jne    SHORT loc_1_2995
loc_1_2A0D:
        db  083h, 00Dh, 050h, 096h, 000h, 000h, 002h
loc_1_2A14:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_2A1C:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        mov    eax, dword ptr [esp + 0ch]
        je     SHORT loc_1_2A54
loc_1_2A2A:
        mov    ecx, edi
        mov    esi, dword ptr [eax + 0ch]
        sub    ecx, esi
        sbb    edx, edx
        and    ecx, edx
        add    ecx, esi
        sub    edi, ecx
        add    dword ptr [eax + 8], ecx
        sub    esi, ecx
        mov    dword ptr [eax + 0ch], esi
        jne    SHORT loc_1_2A4B
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_2A54
        mov    eax, ecx
loc_1_2A4B:
        or     edi, edi
        jne    SHORT loc_1_2A2A
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_2A54:
        pop    edi
        pop    esi
        ret
        db  090h
sub_1_2A58:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     SHORT loc_1_2A82
loc_1_2A61:
        cmp    dword ptr [eax + 4], 3
        jne    SHORT loc_1_2A7C
        mov    esi, dword ptr [eax]
        push   eax
        call   sub_1_90FC
        add    esp, 4
        mov    eax, esi
        or     eax, eax
        jne    SHORT loc_1_2A61
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_2A7C:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_2A61
loc_1_2A82:
        pop    esi
        ret
sub_1_2A84:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esi + 9ch]
        mov    ebx, dword ptr [esi + 40h]
loc_1_2A95:
        or     edi, edi
        je     SHORT loc_1_2AF5
        mov    ecx, dword ptr [edi + 4]
        mov    eax, ebx
        sub    eax, ecx
        js     SHORT loc_1_2AFC
        mov    eax, dword ptr [edi + 0ch]
        and    eax, 100h
        or     eax, eax
        mov    eax, 0
        setne  al
        add    eax, dword ptr [edi + 8]
        add    eax, ecx
        mov    ecx, ebx
        sub    ecx, eax
        js     SHORT loc_1_2B04
        push   dword ptr [edi + 14h]
        call   sub_1_2A58
        add    esp, 4
        mov    ebp, dword ptr [edi]
        push   edi
        call   sub_1_90FC
        add    esp, 4
        mov    edi, ebp
        mov    dword ptr [esi + 9ch], ebp
        or     ebp, ebp
        jne    SHORT loc_1_2A95
        mov    al, byte ptr [esi + 82h]
        dec    al
        mov    byte ptr [esi + 82h], al
        jne    SHORT loc_1_2AF5
        and    dword ptr [esi + 5ch], 0fffffffeh
loc_1_2AF5:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_2AFC:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_2B04:
        mov    ebp, ebx
        sub    ebp, dword ptr [edi + 4]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx], ebx
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [ecx + 0ch], eax
        cmp    dword ptr [edi + 8], ebp
        je     SHORT loc_1_2B3C
        push   ebp
        push   dword ptr [edi + 14h]
        call   sub_1_40F0
        add    esp, 8
        mov    ebx, eax
        mov    ecx, dword ptr [edi + 8]
        sub    ecx, ebp
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_2B56
        db  090h
loc_1_2B3C:
        push   dword ptr [edi + 14h]
        call   sub_1_2A58
        add    esp, 4
        db  0BBh, 074h, 096h, 000h, 000h
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], 0
loc_1_2B56:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 9ch], eax
        or     eax, eax
        jne    SHORT loc_1_2B76
        mov    al, byte ptr [esi + 82h]
        dec    al
        mov    byte ptr [esi + 82h], al
        jne    SHORT loc_1_2B76
        and    dword ptr [esi + 5ch], 0fffffffeh
loc_1_2B76:
        push   edi
        call   sub_1_90FC
        add    esp, 4
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Bh, 0FFh
sub_1_2B88:
        push   ebx
        push   esi
        push   edi
        push   1ch
        call   sub_1_90F6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_2BA4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_2BA4:
        mov    ebx, dword ptr [esp + 1ch]
        lea    eax, [ebx + 10h]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_2BCC
        push   esi
        call   sub_1_90FC
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_2BCC:
        mov    dword ptr [edi + 0ch], ebx
        mov    dword ptr [edi + 4], 3
        mov    dword ptr [edi], 0
        lea    eax, [edi + 10h]
        mov    dword ptr [edi + 8], eax
        or     ebx, ebx
        je     SHORT loc_1_2BF6
        push   0
        push   ebx
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_1E94
        add    esp, 10h
loc_1_2BF6:
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 8], ebx
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esi + 10h], eax
        mov    dword ptr [esi + 14h], edi
        mov    dword ptr [esi + 18h], edi
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi], ecx
        mov    dword ptr [eax], esi
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_2C28:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax + 10h]
        and    eax, 100h
        or     eax, eax
        mov    ecx, 0
        setne  cl
        mov    eax, dword ptr [esp + 28h]
        add    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 30h]
        add    eax, ecx
        mov    dword ptr [esp + 1ch], eax
        mov    ebx, dword ptr [esp + 24h]
        add    ebx, 9ch
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     NEAR PTR loc_1_2DC8
loc_1_2C69:
        mov    edx, dword ptr [esi + 0ch]
        and    edx, 100h
        lea    ecx, [esi + 4]
        mov    dword ptr [esp + 14h], ecx
        mov    ebp, dword ptr [ecx]
        lea    eax, [esi + 8]
        mov    dword ptr [esp + 18h], eax
        cmp    edx, 1
        sbb    ecx, ecx
        inc    ecx
        add    ecx, dword ptr [eax]
        lea    edi, [ecx + ebp]
        mov    eax, dword ptr [esp + 28h]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        sub    eax, edi
        or     eax, eax
        jg     NEAR PTR loc_1_2DB8
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, ebp
        js     NEAR PTR loc_1_2DC8
        cmp    dword ptr [esp + 30h], 0
        mov    eax, dword ptr [esp + 28h]
        je     NEAR PTR loc_1_2EA0
        mov    eax, dword ptr [eax + 10h]
        test   ah, 1
        je     SHORT loc_1_2CD0
        mov    edx, dword ptr [esp + 1ch]
        sub    edx, edi
        or     edx, edx
        jle    NEAR PTR loc_1_2DFD
loc_1_2CD0:
        sub    ebp, ecx
        mov    dword ptr [esp + 10h], ebp
        or     ebp, ebp
        jle    NEAR PTR loc_1_2D60
        test   ah, 20h
        jne    SHORT loc_1_2D10
        test   byte ptr [esi + 0dh], 20h
        je     SHORT loc_1_2D10
        push   ebp
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   ebx
        call   sub_1_2B88
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_2D03
        mov    eax, dword ptr [ebx]
        mov    esi, dword ptr [eax]
loc_1_2D03:
        mov    dword ptr [esp + 10h], 0
        jmp    SHORT loc_1_2D60
        db  08Dh, 049h, 000h
loc_1_2D10:
        lea    eax, [ebp + 10h]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_2DFD
        mov    dword ptr [ebx + 4], 3
        mov    dword ptr [ebx + 0ch], ebp
        lea    eax, [ebx + 10h]
        mov    dword ptr [ebx + 8], eax
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   eax
        call   sub_1_1E94
        add    esp, 10h
        mov    eax, dword ptr [esp + 18h]
        add    dword ptr [eax], ebp
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [ebx], eax
        mov    dword ptr [esi + 14h], ebx
        mov    eax, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx], eax
loc_1_2D60:
        mov    edx, dword ptr [esi + 0ch]
        test   dh, 1
        jne    NEAR PTR loc_1_2E08
        mov    eax, dword ptr [esp + 28h]
        mov    ecx, dword ptr [eax]
        mov    eax, ecx
        sub    eax, edi
        mov    ebp, dword ptr [esp + 30h]
        add    ebp, eax
        or     ebp, ebp
        jle    NEAR PTR loc_1_2E54
        mov    eax, dword ptr [esp + 28h]
        test   byte ptr [eax + 11h], 20h
        je     SHORT loc_1_2E0A
        test   dh, 20h
        jne    SHORT loc_1_2E0A
        cmp    dword ptr [esp + 10h], 0
        jg     SHORT loc_1_2E0A
        sub    edi, ecx
        push   edi
        push   dword ptr [esp + 30h]
        call   sub_1_40F0
        add    esp, 8
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [esp + 28h]
        add    dword ptr [eax], edi
        sub    dword ptr [eax + 0ch], edi
        jmp    SHORT loc_1_2DBC
loc_1_2DB8:
        or     edx, edx
        jne    SHORT loc_1_2DFD
loc_1_2DBC:
        mov    ebx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    NEAR PTR loc_1_2C69
loc_1_2DC8:
        mov    eax, dword ptr [esp + 24h]
        mov    esi, dword ptr [eax + 9ch]
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   ebx
        call   sub_1_2B88
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_2DFD
        or     esi, esi
        jne    SHORT loc_1_2DFD
        mov    eax, dword ptr [esp + 24h]
        inc    byte ptr [eax + 82h]
        or     dword ptr [eax + 5ch], 1
loc_1_2DFD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_2E08:
        sub    ebp, ebp
loc_1_2E0A:
        or     ebp, ebp
        jle    SHORT loc_1_2E54
        lea    eax, [ebp + 10h]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_2E54
        mov    dword ptr [ebx + 4], 3
        mov    dword ptr [ebx + 0ch], ebp
        lea    ecx, [ebx + 10h]
        mov    dword ptr [ebx + 8], ecx
        mov    eax, dword ptr [esp + 28h]
        sub    edi, dword ptr [eax]
        push   edi
        push   ebp
        push   dword ptr [esp + 34h]
        push   ecx
        call   sub_1_1E94
        add    esp, 10h
        add    dword ptr [esi + 8], ebp
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [eax], ebx
        mov    dword ptr [esi + 18h], ebx
loc_1_2E54:
        mov    eax, dword ptr [esp + 28h]
        test   byte ptr [eax + 11h], 20h
        je     SHORT loc_1_2DFD
        mov    edx, dword ptr [eax + 0ch]
        add    edx, dword ptr [eax]
        mov    eax, dword ptr [esi + 0ch]
        test   ah, 20h
        je     SHORT loc_1_2E88
        mov    eax, dword ptr [esi + 4]
        add    eax, dword ptr [esi + 10h]
        mov    ecx, edx
        sub    ecx, eax
        jns    SHORT loc_1_2E90
        mov    edx, eax
        sub    edx, dword ptr [esi + 4]
        mov    dword ptr [esi + 10h], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_2E88:
        or     eax, 2000h
        mov    dword ptr [esi + 0ch], eax
loc_1_2E90:
        pop    ebp
        sub    edx, dword ptr [esi + 4]
        mov    dword ptr [esi + 10h], edx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_2EA0:
        test   byte ptr [eax + 11h], 1
        je     NEAR PTR loc_1_2DFD
        mov    ecx, dword ptr [esi + 0ch]
        test   ch, 1
        jne    NEAR PTR loc_1_2DFD
        cmp    dword ptr [eax], edi
        jne    NEAR PTR loc_1_2DFD
        or     ecx, 100h
        mov    dword ptr [esi + 0ch], ecx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_2ED0:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0FFh, 005h, 064h, 095h, 000h, 000h
        mov    ebx, dword ptr [esp + 40h]
        mov    edi, dword ptr [ebx + 8]
        cmp    byte ptr [esp + 48h], 0
        jne    NEAR PTR loc_1_407C
        cmp    dword ptr [esp + 44h], 14h
        jb     NEAR PTR loc_1_406C
        mov    ax, word ptr [esp + 44h]
        push   ebx
        mov    ch, al
        mov    cl, ah
        and    ecx, 0ffffh
        movzx  eax, word ptr [esp + 3eh]
        add    ecx, eax
        movzx  eax, word ptr [esp + 3ch]
        add    ecx, eax
        movzx  eax, word ptr [esp + 3ah]
        add    ecx, eax
        movzx  eax, word ptr [esp + 38h]
        add    ecx, eax
        add    ecx, 600h
        push   ecx
        call   sub_1_1D9C
        add    esp, 8
        cmp    ax, 0ffffh
        jne    NEAR PTR loc_1_406C
        mov    ecx, dword ptr [edi + 4]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [esp + 18h], edx
        mov    ecx, dword ptr [edi + 8]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [esp + 1ch], edx
        mov    ax, word ptr [edi + 0eh]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    dword ptr [esp + 20h], ecx
        mov    ax, word ptr [edi + 12h]
        mov    cx, ax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    dword ptr [esp + 24h], ecx
        movzx  eax, word ptr [edi + 0ch]
        mov    dword ptr [esp + 28h], eax
        mov    al, byte ptr [edi + 0ch]
        and    al, 0f3h
        shr    al, 2
        and    eax, 0ffh
        cmp    eax, dword ptr [esp + 44h]
        ja     NEAR PTR loc_1_405C
        sub    dword ptr [esp + 44h], eax
        sub    dword ptr [ebx + 0ch], eax
        add    dword ptr [ebx + 8], eax
        mov    ax, word ptr [edi + 2]
        push   eax
        mov    ax, word ptr [edi]
        push   eax
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        call   sub_1_5B50
        add    esp, 10h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_3E8C
        db  0A1h, 06Ch, 096h, 000h, 000h
        mov    dword ptr [esi + 98h], eax
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 40h], eax
        jne    NEAR PTR loc_1_3274
        mov    eax, dword ptr [esp + 28h]
        and    eax, 3700h
        cmp    eax, dword ptr [esi + 5ch]
        jne    NEAR PTR loc_1_3274
        sub    edi, edi
        inc    dword ptr [esi + 74h]
        mov    ecx, dword ptr [esi + 4]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 1ch]
        jns    NEAR PTR loc_1_3150
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [esi + 0ch]
        cmp    eax, edi
        jg     NEAR PTR loc_1_3150
        push   dword ptr [esp + 1ch]
        push   esi
        call   sub_1_27B8
        add    esp, 8
        mov    eax, dword ptr [esi + 8ch]
        cmp    eax, edi
        je     SHORT loc_1_30F8
        mov    ecx, dword ptr [esp + 1ch]
        sub    ecx, dword ptr [esi + 78h]
        cmp    ecx, edi
        jle    SHORT loc_1_30F8
        db  08Bh, 015h, 06Ch, 096h, 000h, 000h
        sub    dx, ax
        mov    dword ptr [esi + 8ch], edi
        mov    ax, word ptr [esi + 7ch]
        mov    cx, ax
        shr    cx, 3
        sub    dx, cx
        lea    edi, [eax + edx]
        mov    word ptr [esi + 7ch], di
        movsx  eax, dx
        cdq
        xor    eax, edx
        sub    eax, edx
        mov    cx, word ptr [esi + 7eh]
        mov    dx, cx
        shr    dx, 3
        sub    ax, dx
        add    ax, cx
        mov    word ptr [esi + 7eh], ax
        shr    di, 2
        movzx  ecx, di
        and    eax, 0ffffh
        add    ecx, eax
        sar    ecx, 1
        cmp    ecx, 3
        jg     SHORT loc_1_30E7
        mov    ecx, 3
loc_1_30E7:
        cmp    ecx, 960h
        jl     SHORT loc_1_30F4
        mov    ecx, 960h
loc_1_30F4:
        mov    word ptr [esi + 72h], cx
loc_1_30F8:
        mov    edi, dword ptr [esi + 1ch]
        movzx  eax, word ptr [esi + 70h]
        mov    ecx, dword ptr [esi + 18h]
        cmp    edi, ecx
        jae    SHORT loc_1_311C
        cmp    dword ptr [esi + 20h], edi
        jbe    SHORT loc_1_3110
        jmp    SHORT loc_1_3117
        db  08Dh, 049h, 000h
loc_1_3110:
        imul   eax, eax
        sub    edx, edx
        div    edi
loc_1_3117:
        add    edi, eax
        mov    dword ptr [esi + 1ch], edi
loc_1_311C:
        mov    byte ptr [esi + 85h], 0
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [esi + 0ch], eax
        jne    SHORT loc_1_3138
        mov    word ptr [esi + 90h], 0
        jmp    SHORT loc_1_3143
        db  090h
loc_1_3138:
        mov    ax, word ptr [esi + 72h]
        mov    word ptr [esi + 90h], ax
loc_1_3143:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esp + 20h]
        jmp    SHORT loc_1_31A4
loc_1_3150:
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [esi + 0ch]
        or     eax, eax
        jle    SHORT loc_1_3174
        push   esi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_3174:
        cmp    dword ptr [esp + 1ch], ecx
        jne    SHORT loc_1_31CC
        mov    eax, dword ptr [esi + 48h]
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 18h]
        js     SHORT loc_1_3196
        cmp    dword ptr [esp + 18h], eax
        jne    SHORT loc_1_31CC
        mov    eax, dword ptr [esi + 4ch]
        sub    eax, dword ptr [esp + 1ch]
        or     eax, eax
        jg     SHORT loc_1_31CC
loc_1_3196:
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 20h]
loc_1_31A4:
        sub    eax, ecx
        sbb    edx, edx
        not    edx
        and    eax, edx
        add    eax, ecx
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 48h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 4ch], eax
        cmp    dword ptr [esi + 14h], 1
        cmc
        sbb    edi, edi
        and    edi, 400h
loc_1_31CC:
        mov    ebp, dword ptr [esi + 44h]
        mov    eax, dword ptr [esp + 44h]
        cmp    ebp, eax
        jl     SHORT loc_1_31D9
        mov    ebp, eax
loc_1_31D9:
        or     ebp, ebp
        je     SHORT loc_1_3240
        or     dword ptr [esi + 5ch], 2
        push   ebp
        push   ebx
        push   dword ptr [esp + 30h]
        push   esi
        call   dword ptr [esi + 60h]
        add    esp, 10h
        add    dword ptr [esi + 40h], eax
        sub    dword ptr [esi + 44h], eax
        and    dword ptr [esi + 5ch], 0fffffffdh
        mov    edx, dword ptr [esi + 58h]
        mov    ecx, edx
        and    ecx, 800000h
        cmp    ecx, 1
        cmc
        sbb    ecx, ecx
        and    ecx, 400h
        or     edi, ecx
        and    edx, 0ff7fffffh
        mov    dword ptr [esi + 58h], edx
        test   dh, 8
        jne    SHORT loc_1_3238
        cmp    ebp, eax
        jne    SHORT loc_1_3238
        or     edx, 800h
        mov    dword ptr [esi + 58h], edx
        mov    word ptr [esi + 92h], 2
        jmp    SHORT loc_1_324F
        db  090h
loc_1_3238:
        or     edi, 200h
        jmp    SHORT loc_1_324F
loc_1_3240:
        cmp    dword ptr [esp + 44h], 1
        cmc
        sbb    eax, eax
        and    eax, 200h
        or     edi, eax
loc_1_324F:
        or     edi, edi
        je     SHORT loc_1_325D
        push   edi
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_325D:
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3274:
        mov    al, byte ptr [esi + 84h]
        cmp    al, 3
        jne    SHORT loc_1_3284
        test   byte ptr [esi + 58h], 4
        je     SHORT loc_1_328E
loc_1_3284:
        cmp    al, 4
        je     SHORT loc_1_3298
        test   byte ptr [esi + 5ch], 2
        je     SHORT loc_1_3298
loc_1_328E:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3298:
        or     al, al
        je     NEAR PTR loc_1_3E68
        test   byte ptr [esi + 86h], 1
        jne    NEAR PTR loc_1_3E68
        mov    ecx, dword ptr [esi + 58h]
        and    ecx, 1200000h
        cmp    ecx, 1000000h
        je     NEAR PTR loc_1_3E68
        cmp    al, 2
        jne    NEAR PTR loc_1_34E0
        test   byte ptr [esp + 29h], 10h
        je     SHORT loc_1_3300
        mov    eax, dword ptr [esp + 1ch]
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_3300
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 50h]
        push   edi
        call   sub_1_82D8
        add    esp, 14h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_3300:
        test   byte ptr [esp + 29h], 4
        je     SHORT loc_1_3354
        movzx  eax, byte ptr [esi + 85h]
        db  03Bh, 005h, 04Ch, 096h, 000h, 000h
        jne    SHORT loc_1_3340
        push   0eh
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_24B0
        add    esp, 0ch
        push   dword ptr [esp + 10h]
        push   10h
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_3340:
        sub    eax, eax
        mov    word ptr [esi + 90h], 5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_3354:
        test   byte ptr [esp + 29h], 2
        je     NEAR PTR loc_1_34D4
        inc    dword ptr [esi + 74h]
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 40h], eax
        test   byte ptr [esp + 29h], 20h
        je     SHORT loc_1_338C
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_3384
        dec    dword ptr [esp + 24h]
        jmp    SHORT loc_1_338C
        db  08Dh, 049h, 000h
loc_1_3384:
        and    dword ptr [esp + 28h], 0ffffdfffh
loc_1_338C:
        push   edi
        call   sub_1_2718
        add    esp, 4
        mov    word ptr [esi + 80h], ax
        mov    eax, dword ptr [esp + 50h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_33E0
        test   byte ptr [esi + 58h], 2
        jne    SHORT loc_1_33FB
        push   0
        push   dword ptr [esp + 3ch]
        lea    eax, [esi + 0e4h]
        push   eax
        push   dword ptr [esp + 5ch]
        db  0FFh, 015h, 0BCh, 095h, 000h, 000h
        add    esp, 10h
        push   dword ptr [esi + 0f0h]
        db  0FFh, 015h, 0A4h, 095h, 000h, 000h
        add    esp, 4
        push   esi
        call   sub_1_6BE4
        add    esp, 4
        jmp    SHORT loc_1_33FB
        db  090h
loc_1_33E0:
        mov    ax, word ptr [esi + 70h]
        mov    cx, word ptr [esi + 80h]
        sub    ax, cx
        sbb    dx, dx
        and    ax, dx
        add    ax, cx
        mov    word ptr [esi + 70h], ax
loc_1_33FB:
        push   esi
        mov    byte ptr [esi + 85h], 0
        mov    word ptr [esi + 90h], 0
        call   sub_1_2024
        add    esp, 4
        test   byte ptr [esp + 29h], 10h
        je     NEAR PTR loc_1_34A0
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 48h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 4ch], eax
        push   esi
        call   sub_1_845C
        add    esp, 4
        push   0
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_24B0
        add    esp, 0ch
        push   esi
        call   sub_1_80C8
        add    esp, 4
        test   byte ptr [esi + 86h], 1
        jne    SHORT loc_1_3487
        and    dword ptr [esp + 28h], 0fffffdffh
        mov    eax, dword ptr [esp + 28h]
        and    ah, 37h
        cmp    ah, 10h
        jne    SHORT loc_1_34EA
        cmp    dword ptr [esp + 44h], 0
        jne    SHORT loc_1_34EA
loc_1_3487:
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_34A0:
        mov    eax, dword ptr [esi + 4]
        mov    byte ptr [esi + 84h], 3
        mov    dword ptr [esi + 8], eax
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_7E10
        add    esp, 8
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_34D4:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_34E0:
        lea    eax, [esi + 74h]
        mov    dword ptr [esp + 14h], eax
        inc    dword ptr [esi + 74h]
loc_1_34EA:
        cmp    dword ptr [esi + 44h], 0
        je     NEAR PTR loc_1_3664
        mov    eax, dword ptr [esp + 18h]
        sub    eax, dword ptr [esi + 40h]
        jns    NEAR PTR loc_1_35AB
        test   byte ptr [esp + 29h], 2
        je     SHORT loc_1_3518
        and    dword ptr [esp + 28h], 0fffffdffh
        inc    dword ptr [esp + 18h]
        dec    dword ptr [esp + 24h]
loc_1_3518:
        mov    ecx, dword ptr [esi + 40h]
        sub    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 28h]
        and    eax, 100h
        or     eax, eax
        mov    eax, 0
        setne  al
        mov    edx, dword ptr [esp + 44h]
        add    edx, eax
        cmp    edx, ecx
        jg     SHORT loc_1_357C
        test   byte ptr [esp + 29h], 10h
        je     NEAR PTR loc_1_3630
        mov    eax, dword ptr [esi + 4]
        sub    eax, dword ptr [esp + 1ch]
        jns    NEAR PTR loc_1_3630
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [esi + 0ch]
        or     eax, eax
        jg     NEAR PTR loc_1_3630
        mov    dword ptr [esp + 44h], 0
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 40h]
        mov    dword ptr [esp + 18h], eax
        and    dword ptr [esp + 28h], 0ffffd8ffh
loc_1_357C:
        sub    dword ptr [esp + 44h], ecx
        add    dword ptr [esp + 18h], ecx
        sub    dword ptr [esp + 24h], ecx
        push   ecx
        push   ebx
        call   sub_1_2A1C
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [esp + 24h], 0
        jge    SHORT loc_1_35AB
        mov    dword ptr [esp + 24h], 0
        and    dword ptr [esp + 28h], 0ffffdfffh
loc_1_35AB:
        mov    edx, dword ptr [esp + 28h]
        and    edx, 100h
        mov    eax, dword ptr [esp + 28h]
        and    eax, 200h
        or     eax, eax
        mov    eax, 0
        setne  al
        cmp    edx, 1
        sbb    ecx, ecx
        inc    ecx
        add    eax, ecx
        add    eax, dword ptr [esp + 44h]
        je     SHORT loc_1_35D7
        dec    eax
loc_1_35D7:
        mov    ecx, dword ptr [esi + 40h]
        mov    ebp, dword ptr [esi + 44h]
        lea    ecx, [ecx + ebp - 1]
        mov    ebp, eax
        sub    ebp, ecx
        add    ebp, dword ptr [esp + 18h]
        or     ebp, ebp
        jle    NEAR PTR loc_1_36B6
        mov    ebp, dword ptr [esp + 18h]
        sub    ebp, ecx
        or     ebp, ebp
        jle    SHORT loc_1_3648
        sub    ecx, dword ptr [esp + 18h]
        cmp    ecx, -1
        jne    SHORT loc_1_3616
        cmp    dword ptr [esp + 44h], 0
        jne    SHORT loc_1_3616
        test   byte ptr [esp + 29h], 3
        je     NEAR PTR loc_1_36B6
loc_1_3616:
        push   esi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_3630:
        push   esi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3648:
        or     edx, edx
        je     SHORT loc_1_3655
        and    dword ptr [esp + 28h], 0fffffeffh
        dec    eax
loc_1_3655:
        sub    eax, ecx
        add    eax, dword ptr [esp + 18h]
        sub    dword ptr [esp + 44h], eax
        jmp    SHORT loc_1_36B6
        db  08Dh, 049h, 000h
loc_1_3664:
        mov    eax, dword ptr [esi + 40h]
        cmp    dword ptr [esp + 18h], eax
        je     SHORT loc_1_3683
        test   byte ptr [esp + 29h], 4
        je     SHORT loc_1_36D0
        mov    ecx, dword ptr [esp + 18h]
        sub    ecx, eax
        cmp    ecx, 1
        jne    SHORT loc_1_36D0
        mov    dword ptr [esp + 18h], eax
loc_1_3683:
        cmp    dword ptr [esp + 44h], 0
        jne    SHORT loc_1_3691
        test   byte ptr [esp + 29h], 3
        je     SHORT loc_1_36B6
loc_1_3691:
        and    dword ptr [esp + 28h], 0fffffcffh
        mov    dword ptr [esp + 44h], 0
        test   byte ptr [esp + 29h], 4
        jne    SHORT loc_1_36BD
        push   200h
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_36B6:
        test   byte ptr [esp + 29h], 4
        je     SHORT loc_1_3730
loc_1_36BD:
        cmp    byte ptr [esi + 84h], 3
        jne    SHORT loc_1_36E8
        test   byte ptr [esi + 58h], 8
        je     SHORT loc_1_36E8
        mov    al, 10h
        jmp    SHORT loc_1_36EA
loc_1_36D0:
        push   esi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_36E8:
        mov    al, 80h
loc_1_36EA:
        push   dword ptr [esp + 10h]
        push   eax
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        cmp    byte ptr [esi + 84h], 0ah
        je     SHORT loc_1_3719
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   14h
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_6FB8
        add    esp, 0ch
loc_1_3719:
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3730:
        test   byte ptr [esp + 29h], 2
        je     SHORT loc_1_3798
        push   dword ptr [esp + 10h]
        push   40h
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 50h]
        push   edi
        call   sub_1_82D8
        add    esp, 14h
        cmp    byte ptr [esi + 84h], 0ah
        je     SHORT loc_1_3780
        push   esi
        call   sub_1_60F4
        add    esp, 4
        push   14h
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_6FB8
        add    esp, 0ch
loc_1_3780:
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_3798:
        test   byte ptr [esp + 29h], 10h
        je     NEAR PTR loc_1_3E50
        mov    al, byte ptr [esi + 84h]
        cmp    al, 3
        jne    NEAR PTR loc_1_3844
        mov    ecx, dword ptr [esi + 4]
        mov    eax, ecx
        sub    eax, dword ptr [esp + 1ch]
        jns    SHORT loc_1_3814
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [esi + 0ch]
        or     eax, eax
        jg     SHORT loc_1_3814
        mov    byte ptr [esi + 85h], 0
        mov    word ptr [esi + 90h], 0
        inc    ecx
        mov    dword ptr [esi + 4], ecx
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 48h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 4ch], eax
        push   esi
        call   sub_1_845C
        add    esp, 4
        push   0
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_24B0
        add    esp, 0ch
        jmp    NEAR PTR loc_1_3B67
loc_1_3814:
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 50h]
        push   edi
        call   sub_1_82D8
        add    esp, 14h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3844:
        mov    edx, dword ptr [esi + 4]
        mov    ecx, edx
        sub    ecx, dword ptr [esp + 1ch]
        jns    NEAR PTR loc_1_3A38
        mov    ecx, dword ptr [esp + 1ch]
        sub    ecx, dword ptr [esi + 0ch]
        or     ecx, ecx
        jg     NEAR PTR loc_1_3A38
        push   dword ptr [esp + 1ch]
        push   esi
        call   sub_1_27B8
        add    esp, 8
        mov    eax, dword ptr [esi + 8ch]
        or     eax, eax
        je     SHORT loc_1_38F4
        mov    ecx, dword ptr [esp + 1ch]
        sub    ecx, dword ptr [esi + 78h]
        or     ecx, ecx
        jle    SHORT loc_1_38F4
        db  08Bh, 015h, 06Ch, 096h, 000h, 000h
        sub    dx, ax
        mov    dword ptr [esi + 8ch], 0
        mov    ax, word ptr [esi + 7ch]
        mov    cx, ax
        shr    cx, 3
        sub    dx, cx
        lea    edi, [eax + edx]
        mov    word ptr [esi + 7ch], di
        movsx  eax, dx
        cdq
        xor    eax, edx
        sub    eax, edx
        mov    cx, word ptr [esi + 7eh]
        mov    dx, cx
        shr    dx, 3
        sub    ax, dx
        add    ax, cx
        mov    word ptr [esi + 7eh], ax
        shr    di, 2
        movzx  ecx, di
        and    eax, 0ffffh
        add    ecx, eax
        sar    ecx, 1
        cmp    ecx, 3
        jg     SHORT loc_1_38E3
        mov    ecx, 3
loc_1_38E3:
        cmp    ecx, 960h
        jl     SHORT loc_1_38F0
        mov    ecx, 960h
loc_1_38F0:
        mov    word ptr [esi + 72h], cx
loc_1_38F4:
        mov    eax, dword ptr [esi + 58h]
        test   ah, 10h
        je     SHORT loc_1_3944
        and    eax, 0ffffefffh
        mov    dword ptr [esi + 58h], eax
        mov    byte ptr [esi + 0d9h], 0
        mov    ecx, 218h
        movzx  eax, byte ptr [esi + 0ech]
        sub    ecx, eax
        movzx  eax, word ptr [esi + 80h]
        cmp    ecx, eax
        jl     SHORT loc_1_3926
        mov    ecx, eax
loc_1_3926:
        movzx  eax, cx
        mov    word ptr [esi + 70h], cx
        mov    dword ptr [esi + 1ch], eax
        mov    al, byte ptr [esi + 82h]
        dec    al
        mov    byte ptr [esi + 82h], al
        jne    SHORT loc_1_3944
        and    dword ptr [esi + 5ch], 0fffffffeh
loc_1_3944:
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        cmp    edi, ecx
        jae    SHORT loc_1_3970
        cmp    dword ptr [esi + 20h], edi
        movzx  eax, word ptr [esi + 70h]
        jbe    SHORT loc_1_395C
        jmp    SHORT loc_1_3963
        db  08Dh, 049h, 000h
loc_1_395C:
        imul   eax, eax
        sub    edx, edx
        div    edi
loc_1_3963:
        add    edi, eax
        sub    edi, ecx
        sbb    eax, eax
        and    edi, eax
        add    edi, ecx
        mov    dword ptr [esi + 1ch], edi
loc_1_3970:
        mov    ecx, dword ptr [esi + 0ch]
        mov    byte ptr [esi + 85h], 0
        cmp    dword ptr [esp + 1ch], ecx
        lea    edi, [esi + 90h]
        je     SHORT loc_1_3990
        mov    ax, word ptr [esi + 72h]
        mov    word ptr [edi], ax
        jmp    SHORT loc_1_3995
        db  090h
loc_1_3990:
        mov    word ptr [edi], 0
loc_1_3995:
        mov    eax, dword ptr [esi + 58h]
        test   eax, 20000000h
        je     SHORT loc_1_39C3
        cmp    dword ptr [esp + 1ch], ecx
        jne    SHORT loc_1_39C3
        and    eax, 0dffeffffh
        mov    dword ptr [esi + 58h], eax
        movzx  eax, byte ptr [esi + 84h]
        cmp    eax, 5
        je     SHORT loc_1_39D0
        cmp    eax, 8
        je     SHORT loc_1_3A00
        cmp    eax, 9
        je     SHORT loc_1_3A1C
loc_1_39C3:
        mov    eax, 1
        jmp    NEAR PTR loc_1_3A96
        db  08Dh, 049h, 000h
loc_1_39D0:
        push   0
        mov    byte ptr [esi + 84h], 6
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_24B0
        add    esp, 0ch
        db  0A0h, 054h, 096h, 000h, 000h
        mov    byte ptr [esi + 85h], al
        mov    word ptr [edi], 960h
        mov    eax, 1
        jmp    NEAR PTR loc_1_3A96
loc_1_3A00:
        push   dword ptr [esp + 10h]
        push   0
        push   1
        push   esi
        call   sub_1_70A4
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_3A1C:
        push   dword ptr [esp + 10h]
        push   0
        push   0
        push   esi
        call   sub_1_70A4
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_3A38:
        cmp    al, 6
        jne    SHORT loc_1_3A45
        mov    word ptr [esi + 90h], 960h
loc_1_3A45:
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [esi + 0ch]
        or     eax, eax
        jle    SHORT loc_1_3A68
        push   esi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3A68:
        cmp    dword ptr [esp + 1ch], edx
        jne    SHORT loc_1_3A94
        mov    eax, dword ptr [esi + 48h]
        mov    ecx, eax
        sub    ecx, dword ptr [esp + 18h]
        js     SHORT loc_1_3A8A
        cmp    dword ptr [esp + 18h], eax
        jne    SHORT loc_1_3A94
        mov    eax, dword ptr [esi + 4ch]
        sub    eax, dword ptr [esp + 1ch]
        or     eax, eax
        jg     SHORT loc_1_3A94
loc_1_3A8A:
        mov    eax, 1
        jmp    SHORT loc_1_3A96
        db  08Dh, 049h, 000h
loc_1_3A94:
        sub    eax, eax
loc_1_3A96:
        or     eax, eax
        je     NEAR PTR loc_1_3B67
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    ecx, dword ptr [esp + 20h]
        sub    eax, ecx
        sbb    edx, edx
        not    edx
        and    eax, edx
        add    eax, ecx
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 48h], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esi + 4ch], eax
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_1_3B14
        mov    eax, dword ptr [esi + 38h]
        sub    eax, esi
        cmp    eax, 38h
        je     NEAR PTR loc_1_3B67
        mov    eax, dword ptr [esi + 58h]
        and    eax, 0fffffbffh
        mov    dword ptr [esi + 58h], eax
        mov    byte ptr [esi + 85h], 0
        mov    cx, word ptr [esi + 72h]
        mov    word ptr [esi + 90h], cx
        test   eax, 80000h
        jne    SHORT loc_1_3B67
        or     eax, 80000h
        mov    dword ptr [esi + 58h], eax
        inc    byte ptr [esi + 82h]
        or     dword ptr [esi + 5ch], 1
        jmp    SHORT loc_1_3B67
loc_1_3B14:
        mov    eax, dword ptr [esi + 58h]
        test   eax, 80000h
        je     SHORT loc_1_3B4D
        mov    byte ptr [esi + 85h], 0
        and    eax, 0ffb7ffffh
        mov    dword ptr [esi + 58h], eax
        push   dword ptr [esi + 4]
        push   esi
        call   sub_1_5534
        add    esp, 8
        mov    al, byte ptr [esi + 82h]
        dec    al
        mov    byte ptr [esi + 82h], al
        jne    SHORT loc_1_3B4D
        and    dword ptr [esi + 5ch], 0fffffffeh
loc_1_3B4D:
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_3B59
        test   byte ptr [esi + 5ah], 1
        je     SHORT loc_1_3B67
loc_1_3B59:
        push   400h
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_3B67:
        cmp    dword ptr [esp + 44h], 0
        ja     SHORT loc_1_3B8C
        test   byte ptr [esp + 29h], 1
        jne    SHORT loc_1_3B8C
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3B8C:
        mov    cl, byte ptr [esi + 84h]
        cmp    cl, 4
        jb     NEAR PTR loc_1_3E38
        cmp    cl, 6
        ja     NEAR PTR loc_1_3E38
        mov    eax, dword ptr [esi + 58h]
        and    eax, 1000000h
        jne    NEAR PTR loc_1_3E38
        mov    edx, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 40h], edx
        jne    NEAR PTR loc_1_3DDC
        mov    eax, dword ptr [esi + 5ch]
        test   al, 2
        je     SHORT loc_1_3BE0
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_3BE0:
        or     eax, 2
        mov    dword ptr [esi + 5ch], eax
loc_1_3BE6:
        test   byte ptr [esp + 29h], 20h
        je     SHORT loc_1_3C0E
        lea    eax, [esp + 44h]
        push   eax
        push   ebx
        lea    eax, [esp + 20h]
        push   eax
        push   esi
        call   sub_1_4EF8
        add    esp, 10h
        test   byte ptr [esi + 86h], 1
        jne    NEAR PTR loc_1_3DA0
loc_1_3C0E:
        cmp    dword ptr [esp + 44h], 0
        jbe    SHORT loc_1_3C70
        push   dword ptr [esp + 44h]
        push   ebx
        push   dword ptr [esp + 30h]
        push   esi
        call   dword ptr [esi + 60h]
        add    esp, 10h
        mov    edi, eax
        add    dword ptr [esi + 40h], edi
        sub    dword ptr [esi + 44h], edi
        mov    eax, dword ptr [esi + 58h]
        test   ah, 8
        je     SHORT loc_1_3C48
        push   200h
        push   esi
        call   sub_1_2388
        add    esp, 8
        jmp    SHORT loc_1_3C59
        db  08Dh, 049h, 000h
loc_1_3C48:
        or     eax, 800h
        mov    dword ptr [esi + 58h], eax
        mov    word ptr [esi + 92h], 2
loc_1_3C59:
        cmp    edi, dword ptr [esp + 44h]
        jne    NEAR PTR loc_1_3CF8
        test   byte ptr [esi + 86h], 1
        jne    NEAR PTR loc_1_3DA0
loc_1_3C70:
        mov    edx, dword ptr [esi + 58h]
        test   dh, 80h
        je     SHORT loc_1_3CCC
        test   edx, 4000000h
        jne    SHORT loc_1_3C9C
        mov    eax, dword ptr [esi + 40h]
        cmp    dword ptr [esi + 108h], eax
        jne    SHORT loc_1_3C95
        mov    eax, dword ptr [esi + 10ch]
        inc    eax
        mov    dword ptr [esi + 40h], eax
loc_1_3C95:
        sub    eax, eax
        jmp    SHORT loc_1_3CA2
        db  08Dh, 049h, 000h
loc_1_3C9C:
        mov    eax, dword ptr [esi + 94h]
loc_1_3CA2:
        mov    ecx, dword ptr [esi + 40h]
        sub    ecx, dword ptr [esi + 10ch]
        sub    ecx, eax
        je     SHORT loc_1_3CCC
        and    edx, 0ffff7fffh
        mov    dword ptr [esi + 58h], edx
        mov    al, byte ptr [esi + 82h]
        dec    al
        mov    byte ptr [esi + 82h], al
        jne    SHORT loc_1_3CCC
        and    dword ptr [esi + 5ch], 0fffffffeh
loc_1_3CCC:
        test   byte ptr [esp + 29h], 1
        jne    SHORT loc_1_3D0C
        lea    eax, [esp + 44h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   esi
        call   sub_1_2A84
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_1_3BE6
        jmp    NEAR PTR loc_1_3DA0
        db  08Dh, 049h, 000h
loc_1_3CF8:
        push   200h
        push   esi
        call   sub_1_2388
        add    esp, 8
        jmp    NEAR PTR loc_1_3DA0
        db  090h
loc_1_3D0C:
        sub    edi, edi
        inc    dword ptr [esi + 40h]
        push   200h
        push   esi
        call   sub_1_2388
        add    esp, 8
        push   esi
        call   sub_1_4DD8
        add    esp, 4
        movzx  eax, byte ptr [esi + 84h]
        cmp    eax, 3
        jl     SHORT loc_1_3D8D
        cmp    eax, 4
        jle    SHORT loc_1_3D48
        cmp    eax, 5
        je     SHORT loc_1_3D60
        cmp    eax, 6
        je     SHORT loc_1_3D70
        jmp    SHORT loc_1_3D8D
        db  08Dh, 049h, 000h
loc_1_3D48:
        inc    byte ptr [esi + 82h]
        mov    byte ptr [esi + 84h], 7
        or     dword ptr [esi + 5ch], 1
        mov    edi, 1
        jmp    SHORT loc_1_3D8D
loc_1_3D60:
        mov    edi, 1
        mov    byte ptr [esi + 84h], 8
        jmp    SHORT loc_1_3D8D
        db  08Bh, 0FFh
loc_1_3D70:
        inc    dword ptr [esi + 74h]
        mov    word ptr [esi + 90h], 0
        push   dword ptr [esp + 10h]
        push   1
        push   1
        push   esi
        call   sub_1_70A4
        add    esp, 10h
loc_1_3D8D:
        or     edi, edi
        je     SHORT loc_1_3DA0
        push   16h
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_6FB8
        add    esp, 0ch
loc_1_3DA0:
        mov    eax, dword ptr [esi + 58h]
        and    dword ptr [esi + 5ch], 0fffffffdh
        test   eax, 800000h
        je     SHORT loc_1_3DC4
        and    eax, 0ff7fffffh
        mov    dword ptr [esi + 58h], eax
        push   400h
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_3DC4:
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_3DDC:
        cmp    cl, 4
        jb     SHORT loc_1_3E20
        cmp    cl, 6
        ja     SHORT loc_1_3E20
        or     eax, eax
        jne    SHORT loc_1_3E20
        push   dword ptr [esp + 44h]
        push   ebx
        lea    eax, [esp + 20h]
        push   eax
        push   esi
        call   sub_1_2C28
        add    esp, 10h
        push   esi
        call   sub_1_80C8
        add    esp, 4
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_3E20:
        push   esi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3E38:
        push   esi
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2778
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_3E50:
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_3E68:
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 50h]
        push   edi
        call   sub_1_82D8
        add    esp, 14h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_3E8C:
        test   byte ptr [esp + 29h], 4
        je     SHORT loc_1_3EA0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_3EA0:
        push   dword ptr [esp + 38h]
        db  0FFh, 015h, 0A8h, 095h, 000h, 000h
        add    esp, 4
        test   al, 1
        jne    NEAR PTR loc_1_4050
        cmp    al, 0ffh
        je     NEAR PTR loc_1_4050
        mov    eax, dword ptr [esp + 28h]
        and    ah, 12h
        cmp    ah, 2
        jne    NEAR PTR loc_1_402C
        push   0
        push   6
        mov    ax, word ptr [edi + 2]
        push   eax
        push   dword ptr [esp + 40h]
        call   sub_1_0414
        add    esp, 10h
        or     eax, eax
        je     NEAR PTR loc_1_402C
        push   dword ptr [esp + 50h]
        mov    cx, word ptr [edi]
        push   ecx
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_250C
        add    esp, 14h
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_402C
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [esi + 64h], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esi + 68h], eax
        mov    ax, word ptr [edi]
        mov    word ptr [esi + 6ch], ax
        mov    ax, word ptr [edi + 2]
        mov    word ptr [esi + 6eh], ax
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 40h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [esi + 10h], eax
        push   edi
        call   sub_1_2718
        add    esp, 4
        mov    word ptr [esi + 80h], ax
        mov    byte ptr [esi + 84h], 3
        db  0FFh, 005h, 054h, 095h, 000h, 000h
        or     dword ptr [esi + 5ch], 2
        push   esi
        call   sub_1_5BE8
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_3F98
        push   13h
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_24B0
        add    esp, 0ch
        dec    dword ptr [esi + 74h]
        push   dword ptr [esp + 10h]
        push   40h
        push   esi
        call   sub_1_6398
        add    esp, 0ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Dh, 049h, 000h
loc_1_3F98:
        mov    eax, dword ptr [esi + 58h]
        and    dword ptr [esi + 5ch], 0fffffffdh
        test   eax, 800000h
        je     SHORT loc_1_3FBC
        and    eax, 0ff7fffffh
        mov    dword ptr [esi + 58h], eax
        push   400h
        push   esi
        call   sub_1_2388
        add    esp, 8
loc_1_3FBC:
        mov    eax, dword ptr [esp + 50h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_3FE5
        test   byte ptr [esi + 58h], 2
        jne    SHORT loc_1_3FE5
        push   0
        push   dword ptr [esp + 3ch]
        lea    eax, [esi + 0e4h]
        push   eax
        push   dword ptr [esp + 5ch]
        db  0FFh, 015h, 0BCh, 095h, 000h, 000h
        add    esp, 10h
loc_1_3FE5:
        test   byte ptr [esi + 58h], 4
        je     SHORT loc_1_4004
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_6C9C
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_4004:
        push   0
        push   dword ptr [esp + 54h]
        push   esi
        call   sub_1_24B0
        add    esp, 0ch
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_63D8
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_402C:
        push   dword ptr [esp + 50h]
        push   dword ptr [esp + 38h]
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 50h]
        push   edi
        call   sub_1_82D8
        add    esp, 14h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  090h
loc_1_4050:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
        db  08Bh, 0FFh
loc_1_405C:
        sub    eax, eax
        db  0FFh, 005h, 070h, 095h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_406C:
        sub    eax, eax
        db  0FFh, 005h, 070h, 095h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_1_407C:
        sub    eax, eax
        db  0FFh, 005h, 070h, 095h, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
sub_1_408C:
        db  0A1h, 004h, 093h, 000h, 000h
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 004h, 093h, 000h, 000h
        ret
        db  08Bh, 0FFh
sub_1_40A0:
        db  0A1h, 004h, 093h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_40B4
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 004h, 093h, 000h, 000h
        ret
        db  08Bh, 0FFh
loc_1_40B4:
        db  0A1h, 008h, 093h, 000h, 000h
        db  039h, 005h, 00Ch, 093h, 000h, 000h
        mov    eax, 0
        jbe    SHORT loc_1_40D0
        push   24h
        call   sub_1_90F6
        add    esp, 4
loc_1_40D0:
        or     eax, eax
        je     SHORT loc_1_40DA
        db  0FFh, 005h, 008h, 093h, 000h, 000h
loc_1_40DA:
        ret
        db  090h
sub_1_40DC:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_40EC
loc_1_40E5:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_40E5
loc_1_40EC:
        ret
        db  08Dh, 049h, 000h
sub_1_40F0:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        mov    esi, dword ptr [esp + 0ch]
        je     SHORT loc_1_4130
loc_1_40FE:
        mov    eax, dword ptr [esi + 0ch]
        cmp    edi, eax
        jb     SHORT loc_1_4124
        sub    edi, eax
        mov    eax, esi
        mov    esi, dword ptr [esi]
        cmp    dword ptr [eax + 4], 3
        jne    SHORT loc_1_411A
        push   eax
        call   sub_1_90FC
        add    esp, 4
loc_1_411A:
        or     edi, edi
        jne    SHORT loc_1_40FE
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_4124:
        mov    eax, esi
        sub    dword ptr [esi + 0ch], edi
        add    dword ptr [esi + 8], edi
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_4130:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_4138:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 20h]
        cmp    dword ptr [ebp + 4], 3
        je     NEAR PTR loc_1_4268
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esp + 28h]
        sub    eax, ebx
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, ebx
        mov    dword ptr [esp + 18h], eax
        add    eax, 10h
        push   eax
        call   sub_1_90F6
        add    esp, 4
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     NEAR PTR loc_1_425C
        mov    edx, eax
        mov    dword ptr [esp + 10h], edx
        mov    ecx, eax
        mov    dword ptr [ecx], 0
        mov    ecx, eax
        mov    dword ptr [ecx + 4], 3
        mov    ecx, eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 0ch], eax
        mov    edi, dword ptr [esp + 14h]
        add    edi, 10h
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 8], edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    ebx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [esp + 20h], ecx
        or     ecx, ecx
        je     SHORT loc_1_4235
loc_1_41C6:
        mov    ecx, dword ptr [esp + 20h]
        cmp    dword ptr [ecx + 4], 3
        je     SHORT loc_1_4235
        or     ebx, ebx
        je     SHORT loc_1_4235
        mov    eax, dword ptr [ecx + 0ch]
        sub    eax, ebx
        sbb    ecx, ecx
        and    eax, ecx
        lea    ebp, [eax + ebx]
        lea    eax, [ebp + 10h]
        push   eax
        call   sub_1_90F6
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_4244
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 3
        mov    dword ptr [eax + 0ch], ebp
        lea    edi, [eax + 10h]
        mov    dword ptr [eax + 8], edi
        mov    ecx, dword ptr [esp + 20h]
        mov    esi, dword ptr [ecx + 8]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [edx], eax
        mov    dword ptr [esp + 10h], eax
        sub    ebx, ebp
        mov    ecx, dword ptr [esp + 20h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [esp + 20h], ecx
        or     ecx, ecx
        jne    SHORT loc_1_41C6
loc_1_4235:
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_1_4282
        db  08Dh, 049h, 000h
loc_1_4244:
        push   dword ptr [esp + 14h]
        call   sub_1_2A58
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_425C:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_4268:
        mov    dword ptr [esp + 14h], ebp
        mov    dword ptr [esp + 10h], ebp
        add    ebp, 0ch
        mov    ebx, dword ptr [esp + 28h]
        cmp    dword ptr [ebp], ebx
        jbe    SHORT loc_1_427F
        mov    dword ptr [ebp], ebx
loc_1_427F:
        sub    ebx, dword ptr [ebp]
loc_1_4282:
        or     ebx, ebx
        je     SHORT loc_1_42A1
loc_1_4286:
        mov    edx, dword ptr [esp + 10h]
        mov    edx, dword ptr [edx]
        mov    dword ptr [esp + 10h], edx
        cmp    dword ptr [edx + 0ch], ebx
        jbe    SHORT loc_1_4298
        mov    dword ptr [edx + 0ch], ebx
loc_1_4298:
        mov    edx, dword ptr [esp + 10h]
        sub    ebx, dword ptr [edx + 0ch]
        jne    SHORT loc_1_4286
loc_1_42A1:
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_42BE
        push   eax
        call   sub_1_2A58
        add    esp, 4
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [edx], 0
loc_1_42BE:
        mov    eax, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_42D4:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        push   esi
        lea    eax, [esp + 0ch]
        push   eax
        mov    edi, dword ptr [esp + 20h]
        push   edi
        call   sub_1_4138
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_4340
        mov    edx, dword ptr [esp + 10h]
        cmp    dword ptr [edx + 0ach], 0
        jne    SHORT loc_1_4320
        mov    dword ptr [edx + 0ach], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0b0h], eax
        add    dword ptr [edx + 0a8h], esi
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  090h
loc_1_4320:
        mov    ecx, dword ptr [edx + 0b0h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0b0h], eax
        add    dword ptr [edx + 0a8h], esi
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_1_4340:
        push   edi
        call   sub_1_2A58
        add    esp, 4
        sub    esi, esi
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  090h
sub_1_4354:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    ecx, dword ptr [esp + 3ch]
        mov    ebx, dword ptr [ecx + 50h]
        mov    ebp, dword ptr [ebx + 20h]
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [ebx + 1ch]
        sub    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        sub    ecx, edx
        sub    eax, ecx
        sbb    esi, esi
        and    eax, esi
        add    eax, ecx
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebp + 4]
        add    eax, edx
        mov    dword ptr [esp + 1ch], eax
        mov    ecx, dword ptr [esp + 44h]
        mov    dword ptr [esp + 18h], ecx
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [esp + 40h]
        and    eax, 800h
        cmp    eax, 1
        cmc
        sbb    eax, eax
        and    eax, 80000000h
        mov    dword ptr [esp + 2ch], eax
        mov    dword ptr [esp + 30h], eax
        or     dword ptr [ebx + 14h], eax
loc_1_43C5:
        mov    edx, dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 24h]
        sub    edx, eax
        sbb    ecx, ecx
        and    edx, ecx
        add    edx, eax
        mov    eax, dword ptr [esp + 48h]
        sub    eax, dword ptr [esp + 14h]
        sub    edx, eax
        sbb    ecx, ecx
        and    edx, ecx
        add    eax, edx
        mov    dword ptr [esp + 10h], eax
        mov    edi, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 14h], eax
        add    dword ptr [ebx + 0ch], eax
        sub    dword ptr [esp + 24h], eax
        jne    SHORT loc_1_4448
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, edx
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [esp + 18h], ecx
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, dword ptr [esp + 48h]
        je     SHORT loc_1_4434
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 28h], eax
loc_1_4434:
        cmp    dword ptr [edx + 4], 3
        jne    SHORT loc_1_4450
        push   edx
        call   sub_1_90FC
        add    esp, 4
        jmp    SHORT loc_1_4450
        db  08Dh, 049h, 000h
loc_1_4448:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 28h], eax
loc_1_4450:
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [esp + 20h], eax
        jne    NEAR PTR loc_1_44E8
        mov    eax, dword ptr [ebx + 1ch]
        sub    eax, dword ptr [ebx + 0ch]
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     SHORT loc_1_4488
        or     eax, eax
        je     SHORT loc_1_4488
        mov    ecx, dword ptr [ebp + 0ch]
        sub    ecx, eax
        sbb    edx, edx
        and    ecx, edx
        add    eax, ecx
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_1_44F0
        db  090h
loc_1_4488:
        mov    eax, dword ptr [ebx + 14h]
        or     eax, 80000000h
        mov    dword ptr [ebx + 14h], eax
        test   ah, 1
        je     SHORT loc_1_44A9
        push   200h
        push   dword ptr [esp + 40h]
        call   sub_1_2388
        add    esp, 8
loc_1_44A9:
        mov    ebx, dword ptr [ebx]
        mov    dword ptr [esp + 30h], 1
        or     ebx, ebx
        je     SHORT loc_1_44FE
        mov    ebp, dword ptr [ebx + 20h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebx + 1ch]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, dword ptr [esp + 48h]
        je     SHORT loc_1_44F0
        mov    eax, dword ptr [esp + 2ch]
        or     dword ptr [ebx + 14h], eax
        jmp    SHORT loc_1_44F0
        db  08Bh, 0FFh
loc_1_44E8:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 1ch], eax
loc_1_44F0:
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, dword ptr [esp + 48h]
        jne    NEAR PTR loc_1_43C5
loc_1_44FE:
        or     ebx, ebx
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [ecx + 50h], ebx
        je     SHORT loc_1_4520
        mov    dword ptr [ebx + 20h], ebp
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx + 10h], eax
        db  0C7h, 041h, 060h, 054h, 043h, 000h, 000h
        jmp    SHORT loc_1_452B
        db  090h
loc_1_4520:
        mov    ecx, dword ptr [esp + 3ch]
        db  0C7h, 041h, 060h, 0D4h, 042h, 000h, 000h
loc_1_452B:
        mov    ecx, dword ptr [esp + 3ch]
        mov    eax, dword ptr [ecx + 54h]
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, eax
        sub    ecx, edx
        sbb    esi, esi
        and    ecx, esi
        add    ecx, edx
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [ecx + 54h], eax
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, dword ptr [esp + 48h]
        je     NEAR PTR loc_1_45D8
        mov    eax, dword ptr [esp + 28h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 48h]
        sub    eax, dword ptr [esp + 14h]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_4138
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_45DF
        mov    ecx, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [ecx + 0ach]
        or     ecx, ecx
        jne    SHORT loc_1_45AC
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [ecx + 0ach], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [ecx + 0b0h], eax
        jmp    SHORT loc_1_45BC
        db  090h
loc_1_45AC:
        mov    edx, dword ptr [esp + 34h]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [ecx + 0ach], eax
loc_1_45BC:
        mov    eax, dword ptr [esp + 48h]
        sub    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 3ch]
        add    dword ptr [ecx + 0a8h], eax
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [esp + 14h], eax
        jmp    SHORT loc_1_45EB
loc_1_45D8:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_45EB
loc_1_45DF:
        push   dword ptr [esp + 18h]
        call   sub_1_2A58
        add    esp, 4
loc_1_45EB:
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_1_4608
        push   100h
        push   dword ptr [esp + 40h]
        call   sub_1_2388
        add    esp, 8
        jmp    SHORT loc_1_4613
        db  08Dh, 049h, 000h
loc_1_4608:
        mov    ecx, dword ptr [esp + 3ch]
        mov    byte ptr [ecx + 83h], 5
loc_1_4613:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
        db  090h
loc_1_4620:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        call   sub_1_40A0
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_4864
        mov    esi, dword ptr [esp + 34h]
        mov    edi, dword ptr [esp + 40h]
        mov    dword ptr [esi + 54h], edi
        or     dword ptr [esi + 58h], 80h
        mov    ecx, dword ptr [esp + 38h]
        and    ecx, 800h
        or     ecx, 1440h
        shr    ecx, 1
        lea    eax, [esp + 1ch]
        push   eax
        mov    ebx, dword ptr [esp + 40h]
        push   dword ptr [ebx + 8]
        lea    eax, [esp + 18h]
        push   eax
        push   edi
        push   dword ptr [ebx + 0ch]
        push   ecx
        push   dword ptr [esi + 0b8h]
        push   dword ptr [esi + 0e0h]
        call   dword ptr [esi + 0dch]
        add    esp, 20h
        mov    dword ptr [esp + 18h], eax
        and    dword ptr [esi + 58h], 0ffffff7fh
        cmp    eax, 18h
        jne    NEAR PTR loc_1_4760
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ebp + 20h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 1ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebp + 18h], eax
        mov    eax, dword ptr [esp + 2ch]
        movzx  eax, word ptr [eax]
        mov    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp + 10h], 0
        mov    dword ptr [ebp + 0ch], 0
        mov    eax, dword ptr [esi + 0a0h]
        or     eax, eax
        jne    SHORT loc_1_46FC
        mov    dword ptr [esi + 0a0h], ebp
        mov    dword ptr [esi + 0a4h], ebp
        mov    dword ptr [ebp], 0
        jmp    SHORT loc_1_4705
        db  090h
loc_1_46FC:
        mov    dword ptr [ebp], eax
        mov    dword ptr [esi + 0a0h], ebp
loc_1_4705:
        mov    dword ptr [esi + 50h], ebp
        db  0C7h, 046h, 060h, 054h, 043h, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [esi + 54h], eax
        sub    edi, dword ptr [esp + 10h]
        je     SHORT loc_1_4748
        push   dword ptr [esp + 10h]
        push   ebx
        call   sub_1_40F0
        add    esp, 8
        push   edi
        push   eax
        push   dword ptr [esp + 40h]
        push   esi
        call   sub_1_4354
        add    esp, 10h
        add    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_4748:
        push   ebx
        call   sub_1_2A58
        add    esp, 4
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_4760:
        push   ebp
        call   sub_1_408C
        add    esp, 4
        cmp    dword ptr [esp + 18h], 17h
        jne    SHORT loc_1_4778
        mov    dword ptr [esp + 10h], 0
loc_1_4778:
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [esi + 54h], eax
        cmp    dword ptr [esi + 0a0h], 0
        je     SHORT loc_1_478F
        db  0C7h, 046h, 060h, 054h, 043h, 000h, 000h
loc_1_478F:
        cmp    edi, dword ptr [esp + 10h]
        jne    SHORT loc_1_47AC
        push   ebx
        call   sub_1_2A58
        add    esp, 4
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 0FFh
loc_1_47AC:
        push   dword ptr [esp + 10h]
        push   ebx
        call   sub_1_40F0
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [esi + 0a0h], 0
        jne    SHORT loc_1_4840
        db  0C7h, 046h, 060h, 0D4h, 042h, 000h, 000h
        mov    eax, edi
        sub    eax, dword ptr [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ebx
        call   sub_1_4138
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_4828
        mov    ecx, dword ptr [esi + 0ach]
        or     ecx, ecx
        jne    SHORT loc_1_4800
        mov    dword ptr [esi + 0ach], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 0b0h], eax
        jmp    SHORT loc_1_480C
loc_1_4800:
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [edx], ecx
        mov    dword ptr [esi + 0ach], eax
loc_1_480C:
        mov    eax, edi
        sub    eax, dword ptr [esp + 10h]
        add    dword ptr [esi + 0a8h], eax
        mov    dword ptr [esp + 10h], edi
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_1_4828:
        push   ebx
        call   sub_1_2A58
        add    esp, 4
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Dh, 049h, 000h
loc_1_4840:
        sub    edi, dword ptr [esp + 10h]
        push   edi
        push   ebx
        push   dword ptr [esp + 40h]
        push   esi
        call   sub_1_4354
        add    esp, 10h
        add    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  090h
loc_1_4864:
        push   dword ptr [esp + 3ch]
        call   sub_1_2A58
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 0FFh
_LTEXT ends
        end

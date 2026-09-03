; vip_lcode_01_01.asm -- VIP object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

VIP_Service_00:
        mov    ax, 100h
        clc
        ret
        db  0CDh, 020h, 000h, 000h, 09Ah, 004h, 072h, 006h, 0CDh, 020h, 003h, 080h, 09Ah, 004h, 0C3h
VIP_Service_06:
        jmp    NEAR PTR loc_1_101C
VIP_Service_07:
        jmp    NEAR PTR loc_1_1090
        db  056h, 02Bh, 0F6h, 087h, 035h, 0F3h, 0BBh, 000h, 000h, 0CDh, 020h, 05Bh, 001h, 001h, 000h, 05Eh
        db  0CDh, 020h, 009h, 080h, 088h, 004h
loc_1_0035:
        db  0C7h, 005h, 0F3h, 0BBh, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 011h, 080h, 040h, 000h
sub_1_0045:
        push   esi
        push   ebx
        sub    ebx, ebx
        db  039h, 01Dh, 0F3h, 0BBh, 000h, 000h
        jne    SHORT loc_1_0069
        db  0BEh, 035h, 000h, 000h, 000h
        mov    ecx, 800h
        sub    eax, eax
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  089h, 035h, 0F3h, 0BBh, 000h, 000h
loc_1_0069:
        pop    ebx
        pop    esi
        ret
VIP_Service_01:
        jmp    NEAR PTR loc_1_0AA8
VIP_Service_02:
        jmp    NEAR PTR sub_1_0348
VIP_Service_03:
        sub    eax, eax
VIP_Service_04:
        jmp    NEAR PTR sub_1_0BF4
VIP_Service_08:
        jmp    NEAR PTR loc_1_02D8
sub_1_0082:
        jecxz  loc_1_0097
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_0098
        movzx  edx, cx
        shr    ecx, 10h
        shl    ecx, 4
        add    ecx, edx
        add    ecx, dword ptr [ebx + 4]
loc_1_0097:
        ret
loc_1_0098:
        pushad
        shr    ecx, 10h
        push   0
        push   ecx
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        mov    edx, dword ptr [esp + 18h]
        movzx  edx, dx
        add    ecx, edx
        mov    dword ptr [esp + 18h], ecx
        popad
        ret
loc_1_00B8:
        mov    edx, esp
        push   ebx
        push   edi
        push   esi
        cld
        mov    eax, dword ptr [edx + 4]
        mov    edi, dword ptr [eax + 1ch]
        mov    ebx, dword ptr [eax + 20h]
        movzx  ecx, word ptr [ebx]
        cmp    ecx, dword ptr [edx + 10h]
        jbe    SHORT loc_1_00D2
        mov    ecx, dword ptr [edx + 10h]
loc_1_00D2:
        mov    word ptr [ebx], cx
        mov    ebx, dword ptr [edx + 0ch]
        push   ecx
loc_1_00D9:
        mov    ecx, dword ptr [ebx + 0ch]
        cmp    ecx, dword ptr [esp]
        jbe    SHORT loc_1_00E4
        mov    ecx, dword ptr [esp]
loc_1_00E4:
        sub    dword ptr [esp], ecx
        mov    esi, dword ptr [ebx + 8]
        rep movsb
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [esp], 0
        jne    SHORT loc_1_00D9
        pop    ecx
        mov    ecx, dword ptr [edx + 8]
        test   ecx, ecx
        jne    SHORT loc_1_012B
        mov    ebx, dword ptr [edx + 14h]
        mov    edi, dword ptr [eax + 30h]
        mov    cl, byte ptr [ebx + 9]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [ebx + 0ah]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [ebx + 0bh]
        mov    byte ptr [edi + 2], cl
        mov    esi, dword ptr [ebx]
        movzx  ecx, byte ptr [ebx + 8]
        cmp    cl, byte ptr [edi + 3]
        jbe    SHORT loc_1_0121
        mov    cl, byte ptr [edi + 3]
loc_1_0121:
        mov    byte ptr [edi + 3], cl
        mov    edi, dword ptr [eax + 34h]
        rep movsb
        sub    ecx, ecx
loc_1_012B:
        mov    dword ptr [eax + 3ch], ecx
        mov    ebx, dword ptr [eax + 38h]
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        pop    esi
        pop    edi
        pop    ebx
        ret
VIP_V86_API:
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    word ptr [eax], 0
        jne    NEAR PTR loc_1_021E
        sub    esp, 40h
        mov    edi, esp
        mov    dword ptr [edi + 38h], ebx
        mov    ecx, dword ptr [eax + 10h]
        call   sub_1_0082
        mov    dword ptr [edi + 1ch], ecx
        mov    ecx, dword ptr [eax + 14h]
        call   sub_1_0082
        mov    dword ptr [edi + 20h], ecx
        mov    ecx, dword ptr [eax + 18h]
        call   sub_1_0082
        mov    dl, byte ptr [ecx + 3]
        mov    byte ptr [edi + 2ch], dl
        mov    dl, byte ptr [ecx]
        mov    byte ptr [edi + 2dh], dl
        mov    dl, byte ptr [ecx + 1]
        mov    byte ptr [edi + 2eh], dl
        mov    dl, byte ptr [ecx + 2]
        mov    byte ptr [edi + 2fh], dl
        mov    ecx, dword ptr [ecx + 4]
        call   sub_1_0082
        mov    dword ptr [edi + 24h], ecx
        mov    ecx, dword ptr [eax + 1ch]
        call   sub_1_0082
        mov    dword ptr [edi + 30h], ecx
        mov    ecx, dword ptr [ecx + 4]
        call   sub_1_0082
        mov    dword ptr [edi + 34h], ecx
        mov    ecx, dword ptr [eax + 0ah]
        call   sub_1_0082
        lea    edx, [edi + 24h]
        push   edx
        push   dword ptr [eax + 2]
        db  068h, 0B8h, 000h, 000h, 000h
        movzx  edx, word ptr [eax + 0eh]
        push   edx
        push   ecx
        push   dword ptr [eax + 6]
        push   edi
        mov    dword ptr [edi + 3ch], 2bf7h
        call   sub_1_9B58
        add    esp, 1ch
        cmp    eax, 2bf7h
        jne    SHORT loc_1_0214
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_1_01E7:
        mov    eax, dword ptr [edi + 3ch]
        cmp    eax, 2bf7h
        jne    SHORT loc_1_01FF
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        jmp    SHORT loc_1_01E7
loc_1_01FF:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
loc_1_0205:
        mov    word ptr [ebp + 1ch], ax
        shr    eax, 10h
        mov    word ptr [ebp + 14h], ax
        add    esp, 40h
        ret
loc_1_0214:
        mov    ecx, dword ptr [edi + 20h]
        mov    word ptr [ecx], 0
        jmp    SHORT loc_1_0205
loc_1_021E:
        mov    word ptr [ebp + 1ch], 2b03h
        mov    word ptr [ebp + 14h], 0
        ret
VIP_Service_05:
        jmp    NEAR PTR loc_1_9FF0
        db  0C3h
loc_1_0231:
        mov    eax, dword ptr [esp + 8]
        push   0
        push   eax
        db  068h, 074h, 02Eh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 017h, 000h
        add    esp, 0ch
        ret
sub_1_0247:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        test   edx, edx
        je     SHORT loc_1_0269
        db  00Fh, 0B7h, 015h, 00Eh, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_0276
loc_1_0269:
        db  00Fh, 0B7h, 015h, 010h, 000h, 000h, 000h
        db  003h, 015h, 004h, 000h, 000h, 000h
loc_1_0276:
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   edx
        push   ecx
        push   eax
        push   140001h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        mov    ecx, eax
        mov    eax, 10001030h
        sub    edi, edi
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_02A6:
        db  0CDh, 020h, 01Ah, 080h, 001h, 000h
loc_1_02AC:
        db  0C7h, 005h, 0F7h, 0BBh, 000h, 000h, 001h, 000h, 000h, 000h
        clc
        ret
loc_1_02B8:
        stc
        ret
VIP_Control:
        cmp    eax, 1
        db  00Fh, 084h, 03Dh, 0FDh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 034h, 0FDh, 0FEh, 0FFh
        cmp    eax, 1ch
        je     SHORT loc_1_02B8
        cmp    eax, 4
        je     SHORT loc_1_02AC
        clc
        ret
loc_1_02D8:
        mov    edx, dword ptr [esp + 4]
        cmp    edx, 0ffffh
        ja     SHORT loc_1_0318
        db  0A1h, 090h, 0C0h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_0311
        inc    ecx
        db  08Bh, 00Dh, 050h, 0C0h, 000h, 000h
loc_1_02F4:
        cmp    ecx, eax
        je     SHORT loc_1_02FE
        cmp    word ptr [eax + 16h], dx
        je     SHORT loc_1_030C
loc_1_02FE:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_02F4
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
        db  090h
loc_1_030C:
        db  0A3h, 004h, 0BCh, 000h, 000h
loc_1_0311:
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        ret
loc_1_0318:
        mov    eax, 1
        db  0C7h, 005h, 004h, 0BCh, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_0328:
        push   esi
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_0346
        nop
loc_1_0334:
        mov    eax, dword ptr [esi + 0ch]
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 10h]
        add    esp, 4
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_0334
loc_1_0346:
        pop    esi
        ret
sub_1_0348:
        sub    esp, 4
        push   esi
        push   edi
        sub    eax, eax
        sub    esi, esi
        db  08Bh, 015h, 068h, 0C0h, 000h, 000h
        cmp    edx, eax
        jle    SHORT loc_1_038C
        db  0BFh, 00Ch, 0C0h, 000h, 000h
        mov    cl, byte ptr [esp + 10h]
loc_1_0364:
        cmp    byte ptr [edi], cl
        je     SHORT loc_1_0374
        add    edi, 14h
        inc    esi
        cmp    edx, esi
        jg     SHORT loc_1_0364
        jmp    SHORT loc_1_0390
        db  08Bh, 0FFh
loc_1_0374:
        lea    eax, [esi*4]
        db  08Dh, 084h, 080h, 0FCh, 0BFh, 000h, 000h
        mov    dword ptr [esp + 8], 0
        jmp    SHORT loc_1_0390
loc_1_038C:
        mov    cl, byte ptr [esp + 10h]
loc_1_0390:
        or     eax, eax
        jne    SHORT loc_1_03AF
        cmp    edx, 4
        jge    SHORT loc_1_03D7
        lea    eax, [edx*4]
        db  08Dh, 084h, 080h, 0FCh, 0BFh, 000h, 000h
        mov    dword ptr [esp + 8], 1
loc_1_03AF:
        mov    byte ptr [eax + 10h], cl
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 0ch], ecx
        add    edx, dword ptr [esp + 8]
        db  089h, 015h, 068h, 0C0h, 000h, 000h
loc_1_03D7:
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_03E0:
        mov    edx, dword ptr [esp + 8]
        cmp    dl, 7fh
        jne    SHORT loc_1_03F0
        mov    eax, 2b03h
        ret
        db  090h
loc_1_03F0:
        db  08Bh, 00Dh, 090h, 0C0h, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_041D
        inc    eax
        db  0A1h, 050h, 0C0h, 000h, 000h
loc_1_0400:
        cmp    ecx, eax
        je     SHORT loc_1_0413
        test   byte ptr [ecx + 14h], 1
        je     SHORT loc_1_0413
        or     edx, edx
        je     SHORT loc_1_0419
        cmp    dword ptr [ecx + 4], edx
        je     SHORT loc_1_0419
loc_1_0413:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_0400
loc_1_0419:
        or     ecx, ecx
        jne    SHORT loc_1_0424
loc_1_041D:
        mov    eax, 2b03h
        ret
        db  090h
loc_1_0424:
        cmp    dword ptr [esp + 0ch], 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   dword ptr [esp + 8]
        push   ecx
        call   sub_1_A7D8
        add    esp, 0ch
        ret
loc_1_043C:
        push   dword ptr [esp + 4]
        call   sub_1_7044
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_044C:
        db  0A1h, 090h, 0C0h, 000h, 000h
        or     eax, eax
        mov    ecx, dword ptr [esp + 4]
        je     SHORT loc_1_047C
loc_1_0459:
        cmp    dword ptr [eax + 4], ecx
        jne    SHORT loc_1_0464
        test   byte ptr [eax + 14h], 1
        jne    SHORT loc_1_046C
loc_1_0464:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_0459
        jmp    SHORT loc_1_047C
loc_1_046C:
        mov    ax, word ptr [eax + 3ch]
        mov    ecx, dword ptr [esp + 8]
        mov    word ptr [ecx], ax
        mov    al, 1
        ret
        db  08Bh, 0FFh
loc_1_047C:
        cmp    cl, 7fh
        jne    SHORT loc_1_0494
        db  0A1h, 050h, 0C0h, 000h, 000h
        mov    ax, word ptr [eax + 3ch]
        mov    ecx, dword ptr [esp + 8]
        mov    word ptr [ecx], ax
        mov    al, 1
        ret
loc_1_0494:
        sub    al, al
        ret
        db  090h
sub_1_0498:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    al, byte ptr [esi + 8]
        mov    byte ptr [esp + 13h], al
        movzx  ebx, al
        push   ebx
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_1_04CC
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_04CC:
        mov    esi, dword ptr [esi]
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    byte ptr [esp + 1ch], 28h
        mov    byte ptr [esp + 1dh], 28h
        mov    byte ptr [esp + 1eh], 28h
        mov    esi, dword ptr [esp + 28h]
        and    byte ptr [esi + 0bh], 7fh
        mov    byte ptr [esp + 12h], 0
        cmp    byte ptr [esp + 13h], 0
        je     NEAR PTR loc_1_069E
loc_1_0508:
        mov    edx, dword ptr [esp + 18h]
        movzx  ecx, byte ptr [esp + 12h]
        add    edx, ecx
        mov    bl, byte ptr [edx]
        or     bl, bl
        je     NEAR PTR loc_1_069E
        cmp    bl, 1
        jne    SHORT loc_1_052C
        inc    byte ptr [esp + 12h]
        jmp    NEAR PTR loc_1_0690
        db  090h
loc_1_052C:
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esp + 11h], al
        movzx  eax, bl
        cmp    eax, 7
        je     SHORT loc_1_055C
        cmp    eax, 44h
        je     SHORT loc_1_0590
        cmp    eax, 83h
        je     NEAR PTR loc_1_0618
        cmp    eax, 89h
        je     NEAR PTR loc_1_0610
        jmp    NEAR PTR loc_1_0688
        db  090h
loc_1_055C:
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [esp + 1dh], al
        cmp    dword ptr [esp + 30h], 0
        jne    NEAR PTR loc_1_0688
        lea    edi, [edx + 3]
        movzx  esi, byte ptr [esp + 11h]
        sub    esi, 3
        sub    eax, eax
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        jmp    NEAR PTR loc_1_0684
        db  090h
loc_1_0590:
        mov    al, byte ptr [esp + 12h]
        mov    byte ptr [esp + 1eh], al
        cmp    dword ptr [esp + 30h], 0
        jne    NEAR PTR loc_1_0688
        mov    byte ptr [edx + 2], 5
        mov    al, byte ptr [edx + 3]
        and    al, 0fh
        mov    byte ptr [edx + 3], al
        and    eax, 0ffh
        or     eax, eax
        jl     NEAR PTR loc_1_0688
        cmp    eax, 1
        jle    SHORT loc_1_05CC
        cmp    eax, 3
        je     SHORT loc_1_05EC
        jmp    NEAR PTR loc_1_0688
        db  090h
loc_1_05CC:
        sub    eax, eax
        movzx  esi, byte ptr [esp + 11h]
        sub    esi, 4
        lea    edi, [edx + 4]
        mov    ecx, esi
        shr    ecx, 2
        rep stosd
        mov    ecx, esi
        and    ecx, 3
        rep stosb
        jmp    NEAR PTR loc_1_0688
loc_1_05EC:
        lea    edi, [edx + 8]
        cmp    byte ptr [esp + 11h], 0ch
        jb     NEAR PTR loc_1_0688
        mov    cl, byte ptr [esp + 11h]
        sub    cl, 8
        shr    cl, 2
        sub    eax, eax
        and    ecx, 0ffh
        rep stosd
        jmp    SHORT loc_1_0688
loc_1_0610:
        mov    esi, dword ptr [esp + 28h]
        or     byte ptr [esi + 0bh], 80h
loc_1_0618:
        cmp    byte ptr [esp + 11h], 3
        je     SHORT loc_1_0684
        mov    al, byte ptr [edx + 2]
        sub    al, 5
        and    eax, 0ffh
        add    eax, dword ptr [esp + 18h]
        lea    esi, [eax + ecx]
        mov    dword ptr [esp + 14h], esi
        mov    eax, esi
        lea    edi, [edx + 3]
        mov    ebx, edi
        mov    ebp, dword ptr [esi]
        mov    esi, dword ptr [esp + 28h]
        mov    dword ptr [esi + 4], ebp
        cmp    edi, dword ptr [esp + 14h]
        jae    SHORT loc_1_065C
loc_1_064A:
        mov    esi, dword ptr [eax]
        mov    ebp, dword ptr [ebx]
        mov    dword ptr [eax], ebp
        sub    eax, 4
        mov    dword ptr [ebx], esi
        add    ebx, 4
        cmp    ebx, eax
        jb     SHORT loc_1_064A
loc_1_065C:
        mov    eax, dword ptr [esp + 18h]
        movzx  ebx, byte ptr [esp + 11h]
        sub    ebx, 7
        lea    esi, [ecx + eax + 7]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 2ch]
        mov    esi, dword ptr [esp + 14h]
        mov    dword ptr [esi], eax
loc_1_0684:
        mov    byte ptr [edx + 2], 4
loc_1_0688:
        mov    al, byte ptr [esp + 11h]
        add    byte ptr [esp + 12h], al
loc_1_0690:
        mov    al, byte ptr [esp + 12h]
        cmp    byte ptr [esp + 13h], al
        ja     NEAR PTR loc_1_0508
loc_1_069E:
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_1_06BA
        push   dword ptr [esp + 30h]
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_6264
        add    esp, 0ch
loc_1_06BA:
        mov    al, byte ptr [esp + 13h]
        mov    esi, dword ptr [esp + 28h]
        mov    byte ptr [esi + 8], al
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_06D8:
        mov    ecx, dword ptr [esp + 4]
        movzx  edx, byte ptr [ecx + 1]
        mov    eax, dword ptr [esp + 8]
        shl    eax, 2
        add    eax, 3
        cmp    eax, edx
        ja     SHORT loc_1_0708
        cmp    dword ptr [esp + 0ch], edx
        jb     SHORT loc_1_0708
        inc    edx
        test   dl, 3
        jne    SHORT loc_1_0708
        cmp    byte ptr [ecx + 2], 4
        je     SHORT loc_1_0704
        sub    al, al
        ret
        db  090h
loc_1_0704:
        mov    al, 1
        ret
        db  090h
loc_1_0708:
        sub    al, al
        ret
        db  090h
sub_1_070C:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 4], 0
        db  08Ah, 00Dh, 058h, 0C1h, 000h, 000h
        mov    byte ptr [eax + 9], cl
        db  08Ah, 00Dh, 034h, 0C2h, 000h, 000h
        mov    byte ptr [eax + 0ah], cl
        mov    byte ptr [eax + 0bh], 0
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 8], 0
        ret
sub_1_0738:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 13h], 0
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [edx + 4], 0
        and    byte ptr [edx + 0bh], 7fh
        cmp    dword ptr [esp + 24h], 0
        jne    SHORT loc_1_0770
        mov    dword ptr [edx], 0
        mov    byte ptr [edx + 8], 0
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Bh, 0FFh
loc_1_0770:
        mov    esi, dword ptr [esp + 24h]
        and    esi, 3
        mov    eax, dword ptr [esp + 24h]
        je     SHORT loc_1_0783
        and    eax, 0fffffffch
        add    eax, 4
loc_1_0783:
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_1_07A4
        mov    eax, 2afeh
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_07A4:
        or     esi, esi
        mov    edx, dword ptr [esp + 24h]
        je     SHORT loc_1_07B2
        and    edx, 0fffffffch
        add    edx, 4
loc_1_07B2:
        sub    eax, eax
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        sub    ebx, ebx
        mov    dword ptr [esp + 14h], 2bf7h
        cmp    dword ptr [esp + 24h], eax
        je     NEAR PTR loc_1_0A03
        mov    ebp, dword ptr [esp + 20h]
loc_1_07DE:
        cmp    dword ptr [esp + 14h], 2bf7h
        jne    NEAR PTR loc_1_0A15
        mov    cl, byte ptr [ebp]
        movzx  eax, cl
        cmp    eax, 44h
        jg     SHORT loc_1_0810
        je     NEAR PTR loc_1_08BC
        or     eax, eax
        je     SHORT loc_1_0834
        cmp    eax, 1
        je     SHORT loc_1_0844
        cmp    eax, 7
        je     SHORT loc_1_0858
        jmp    SHORT loc_1_0826
        db  08Dh, 049h, 000h
loc_1_0810:
        cmp    eax, 83h
        je     NEAR PTR loc_1_0969
        cmp    eax, 89h
        je     NEAR PTR loc_1_0948
loc_1_0826:
        mov    dword ptr [esp + 14h], 2affh
        jmp    NEAR PTR loc_1_09F8
        db  090h
loc_1_0834:
        mov    dword ptr [esp + 14h], 0
        jmp    NEAR PTR loc_1_09F8
        db  08Dh, 049h, 000h
loc_1_0844:
        inc    ebp
        mov    esi, dword ptr [esp + 18h]
        mov    byte ptr [esi + ebx], cl
        inc    ebx
        dec    dword ptr [esp + 24h]
        jmp    NEAR PTR loc_1_09F8
        db  08Bh, 0FFh
loc_1_0858:
        test   byte ptr [esp + 13h], 4
        je     SHORT loc_1_086C
        mov    dword ptr [esp + 14h], 2affh
        jmp    NEAR PTR loc_1_09F8
loc_1_086C:
        push   dword ptr [esp + 24h]
        or     byte ptr [esp + 17h], 4
        push   1
        push   ebp
        call   sub_1_06D8
        add    esp, 0ch
        or     al, al
        jne    SHORT loc_1_0894
        mov    dword ptr [esp + 14h], 2affh
        jmp    NEAR PTR loc_1_09F8
        db  08Dh, 049h, 000h
loc_1_0894:
        mov    esi, ebp
        movzx  eax, byte ptr [ebp + 1]
        mov    edi, dword ptr [esp + 18h]
        add    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebx, eax
        add    ebp, eax
        sub    dword ptr [esp + 24h], eax
        jmp    NEAR PTR loc_1_09F8
        db  090h
loc_1_08BC:
        test   byte ptr [esp + 13h], 8
        je     SHORT loc_1_08D0
        mov    dword ptr [esp + 14h], 2affh
        jmp    NEAR PTR loc_1_09F8
loc_1_08D0:
        mov    cl, byte ptr [ebp + 3]
        or     byte ptr [esp + 13h], 8
        mov    al, cl
        and    al, 0fh
        shr    cl, 4
        or     cl, cl
        jne    SHORT loc_1_0938
        or     al, al
        je     SHORT loc_1_08EF
        cmp    al, 1
        je     SHORT loc_1_08EF
        cmp    al, 3
        jne    SHORT loc_1_0938
loc_1_08EF:
        movzx  eax, byte ptr [ebp + 1]
        cmp    eax, dword ptr [esp + 24h]
        ja     SHORT loc_1_0928
        cmp    eax, 8
        jb     SHORT loc_1_0928
        cmp    byte ptr [ebp + 2], 5
        jne    SHORT loc_1_0928
        mov    edi, dword ptr [esp + 18h]
        add    edi, ebx
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    ebx, eax
        add    ebp, eax
        sub    dword ptr [esp + 24h], eax
        jmp    NEAR PTR loc_1_09F8
        db  090h
loc_1_0928:
        mov    dword ptr [esp + 14h], 2affh
        jmp    NEAR PTR loc_1_09F8
        db  08Dh, 049h, 000h
loc_1_0938:
        mov    dword ptr [esp + 14h], 2affh
        jmp    NEAR PTR loc_1_09F8
        db  08Dh, 049h, 000h
loc_1_0948:
        test   byte ptr [esp + 13h], 3
        je     SHORT loc_1_095C
        mov    dword ptr [esp + 14h], 2affh
        jmp    NEAR PTR loc_1_09F8
loc_1_095C:
        mov    edx, dword ptr [esp + 28h]
        or     byte ptr [edx + 0bh], 80h
        or     byte ptr [esp + 13h], 1
loc_1_0969:
        cmp    cl, 83h
        jne    SHORT loc_1_098A
        test   byte ptr [esp + 13h], 3
        je     SHORT loc_1_0980
        mov    dword ptr [esp + 14h], 2affh
        jmp    SHORT loc_1_09F8
        db  090h
loc_1_0980:
        cmp    cl, 83h
        jne    SHORT loc_1_098A
        or     byte ptr [esp + 13h], 2
loc_1_098A:
        push   dword ptr [esp + 24h]
        push   2
        push   ebp
        call   sub_1_06D8
        add    esp, 0ch
        or     al, al
        jne    SHORT loc_1_09A8
        mov    dword ptr [esp + 14h], 2affh
        jmp    SHORT loc_1_09F8
        db  090h
loc_1_09A8:
        mov    al, byte ptr [ebp]
        mov    esi, dword ptr [esp + 18h]
        mov    byte ptr [esi + ebx], al
        inc    ebp
        inc    ebx
        movzx  eax, byte ptr [ebp]
        inc    ebp
        sub    dword ptr [esp + 24h], eax
        sub    eax, 4
        mov    byte ptr [esi + ebx], al
        inc    ebx
        mov    cl, byte ptr [ebp]
        mov    byte ptr [esi + ebx], cl
        inc    ebp
        inc    ebx
        mov    ecx, dword ptr [ebp]
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [edx + 4], ecx
        add    ebp, 4
        lea    edx, [eax - 3]
        mov    edi, esi
        add    edi, ebx
        mov    esi, ebp
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    ebx, [eax + ebx - 3]
        lea    ebp, [eax + ebp - 3]
loc_1_09F8:
        cmp    dword ptr [esp + 24h], 0
        jne    NEAR PTR loc_1_07DE
loc_1_0A03:
        cmp    dword ptr [esp + 14h], 2bf7h
        jne    SHORT loc_1_0A15
        mov    dword ptr [esp + 14h], 0
loc_1_0A15:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_0A34
        push   dword ptr [esp + 18h]
        call   sub_1_B944
        add    esp, 4
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_0A34:
        test   bl, 3
        je     SHORT loc_1_0A3F
        and    ebx, 0fffffffch
        add    ebx, 4
loc_1_0A3F:
        cmp    ebx, 28h
        jbe    SHORT loc_1_0A60
        push   dword ptr [esp + 18h]
        call   sub_1_B944
        add    esp, 4
        mov    eax, 2b09h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
loc_1_0A60:
        sub    eax, eax
        mov    edx, dword ptr [esp + 28h]
        mov    esi, dword ptr [esp + 18h]
        mov    dword ptr [edx], esi
        mov    byte ptr [edx + 8], bl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
sub_1_0A78:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_0AA1
        mov    dword ptr [esi], 0
        push   eax
        call   sub_1_B944
        add    esp, 4
        mov    byte ptr [esi + 8], 0
        mov    dword ptr [esi + 4], 0
        and    byte ptr [esi + 0bh], 7fh
loc_1_0AA1:
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_0AA8:
        cmp    dword ptr [esp + 8], 48h
        jae    SHORT loc_1_0AB8
        mov    eax, 2af9h
        ret
        db  08Dh, 049h, 000h
loc_1_0AB8:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 1
        mov    dword ptr [eax + 4], 14h
        db  0C7h, 040h, 008h, 048h, 068h, 000h, 000h
        db  0C7h, 040h, 00Ch, 048h, 003h, 000h, 000h
        db  0C7h, 040h, 010h, 05Ch, 07Ah, 000h, 000h
        db  0C7h, 040h, 014h, 0ECh, 07Bh, 000h, 000h
        db  0C7h, 040h, 018h, 03Ch, 004h, 000h, 000h
        db  0C7h, 040h, 01Ch, 04Ch, 004h, 000h, 000h
        db  0C7h, 040h, 020h, 0BCh, 078h, 000h, 000h
        db  0C7h, 040h, 024h, 0FCh, 078h, 000h, 000h
        db  0C7h, 040h, 028h, 00Ch, 007h, 000h, 000h
        db  0C7h, 040h, 02Ch, 098h, 004h, 000h, 000h
        db  0C7h, 040h, 030h, 038h, 007h, 000h, 000h
        db  0C7h, 040h, 034h, 078h, 00Ah, 000h, 000h
        db  0C7h, 040h, 038h, 07Ch, 0ABh, 000h, 000h
        db  0C7h, 040h, 03Ch, 0CCh, 0AFh, 000h, 000h
        db  0C7h, 040h, 040h, 0ACh, 0B2h, 000h, 000h
        db  0C7h, 040h, 044h, 0E0h, 003h, 000h, 000h
        sub    eax, eax
        ret
loc_1_0B3C:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        sub    esi, 24h
        sub    edi, edi
        push   esi
        call   sub_1_96CC
        add    esp, 4
        push   esi
        call   sub_1_AA10
        add    esp, 4
        cmp    dword ptr [esp + 14h], edi
        je     SHORT loc_1_0BD8
        lea    eax, [esi + 1ch]
        mov    edx, dword ptr [eax]
        cmp    edx, edi
        je     SHORT loc_1_0B8C
loc_1_0B69:
        mov    cl, byte ptr [edx + 0fh]
        dec    cl
        mov    byte ptr [edx + 0fh], cl
        jne    SHORT loc_1_0B84
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], edi
        mov    edi, edx
        db  0FFh, 005h, 018h, 0C2h, 000h, 000h
        jmp    SHORT loc_1_0B86
        db  090h
loc_1_0B84:
        mov    eax, edx
loc_1_0B86:
        mov    edx, dword ptr [eax]
        or     edx, edx
        jne    SHORT loc_1_0B69
loc_1_0B8C:
        or     edi, edi
        je     SHORT loc_1_0BBC
loc_1_0B90:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [ebx + 18h], 0
        je     SHORT loc_1_0BAF
        push   0
        push   1
        push   0bh
        lea    eax, [ebx + 1ch]
        push   eax
        push   dword ptr [esi + 4]
        call   sub_1_9610
        add    esp, 14h
loc_1_0BAF:
        push   ebx
        call   sub_1_4E1C
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_1_0B90
loc_1_0BBC:
        push   0
        db  068h, 03Ch, 00Bh, 000h, 000h
        push   1f4h
        add    esi, 24h
        push   esi
        call   sub_1_B920
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0BD8:
        push   esi
        db  068h, 03Ch, 00Bh, 000h, 000h
        push   1f4h
        add    esi, 24h
        push   esi
        call   sub_1_B920
        add    esp, 10h
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_0BF4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 01Dh, 090h, 0C0h, 000h, 000h
        or     ebx, ebx
        je     NEAR PTR loc_1_0D58
        mov    ax, word ptr [esp + 14h]
        nop
loc_1_0C0C:
        cmp    word ptr [ebx + 16h], ax
        je     SHORT loc_1_0C18
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    SHORT loc_1_0C0C
loc_1_0C18:
        or     ebx, ebx
        je     NEAR PTR loc_1_0D58
        db  03Bh, 01Dh, 050h, 0C0h, 000h, 000h
        je     NEAR PTR loc_1_0D58
        mov    edi, dword ptr [ebx + 0ch]
        db  0FFh, 005h, 0FCh, 0BBh, 000h, 000h
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        mov    ax, word ptr [ebx + 14h]
        jne    NEAR PTR loc_1_0CC8
        test   al, 1
        je     SHORT loc_1_0CB8
        and    ax, 0fffeh
        mov    word ptr [ebx + 14h], ax
        mov    eax, dword ptr [edi + 34h]
        dec    eax
        mov    dword ptr [edi + 34h], eax
        db  0BEh, 074h, 077h, 000h, 000h
        je     SHORT loc_1_0C64
        db  0BEh, 094h, 077h, 000h, 000h
loc_1_0C64:
        push   ebx
        call   sub_1_AAC4
        add    esp, 4
        push   0
        mov    ax, word ptr [ebx + 16h]
        push   eax
        push   dword ptr [ebx + 18h]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_0FE0
        add    esp, 14h
        push   0
        push   edi
        push   esi
        call   sub_1_77EC
        add    esp, 0ch
        db  0A1h, 050h, 0C0h, 000h, 000h
        push   dword ptr [eax + 0ch]
        push   -1
        push   -1
        push   dword ptr [ebx + 4]
        call   sub_1_7F8C
        add    esp, 10h
        push   0
        push   dword ptr [ebx + 4]
        push   0
        push   dword ptr [edi + 4]
        call   dword ptr [edi + 1ch]
        add    esp, 10h
loc_1_0CB8:
        pop    ebp
        db  0FFh, 00Dh, 0FCh, 0BBh, 000h, 000h
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0CC8:
        test   al, 1
        jne    SHORT loc_1_0D48
        mov    dword ptr [ebx + 4], esi
        mov    ebp, dword ptr [esp + 1ch]
        mov    dword ptr [ebx + 8], ebp
        or     ax, 1
        mov    word ptr [ebx + 14h], ax
        inc    dword ptr [edi + 34h]
        push   ebx
        call   sub_1_8EF4
        add    esp, 4
        or     eax, eax
        mov    edi, 1
        jne    SHORT loc_1_0CF5
        sub    edi, edi
loc_1_0CF5:
        or     edi, edi
        je     SHORT loc_1_0D0F
        mov    eax, dword ptr [ebx + 0ch]
        push   ebp
        push   esi
        push   0
        push   dword ptr [eax + 4]
        call   dword ptr [eax + 18h]
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_0D24
loc_1_0D0F:
        push   0
        push   0
        mov    ax, word ptr [ebx + 16h]
        push   eax
        call   sub_1_0BF4
        add    esp, 0ch
        jmp    SHORT loc_1_0D4A
        db  08Bh, 0FFh
loc_1_0D24:
        push   ebx
        call   sub_1_AA74
        add    esp, 4
        push   1
        mov    ax, word ptr [ebx + 16h]
        push   eax
        push   dword ptr [ebx + 18h]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_0FE0
        add    esp, 14h
        jmp    SHORT loc_1_0D4A
        db  090h
loc_1_0D48:
        sub    edi, edi
loc_1_0D4A:
        mov    eax, edi
        db  0FFh, 00Dh, 0FCh, 0BBh, 000h, 000h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_0D58:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
sub_1_0D60:
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [edx + 1ch], 0
        mov    dword ptr [edx + 20h], 0
        db  066h, 0A1h, 0BDh, 0C1h, 000h, 000h
        mov    word ptr [edx + 16h], ax
        db  066h, 0FFh, 005h, 0BDh, 0C1h, 000h, 000h
        mov    eax, dword ptr [edx + 0ch]
        lea    ecx, [eax + 28h]
        cmp    dword ptr [ecx + 10h], 0
        je     SHORT loc_1_0D99
        nop
loc_1_0D90:
        mov    ecx, dword ptr [ecx + 10h]
        cmp    dword ptr [ecx + 10h], 0
        jne    SHORT loc_1_0D90
loc_1_0D99:
        mov    dword ptr [ecx + 10h], edx
        mov    dword ptr [edx + 10h], 0
        test   byte ptr [edx + 14h], 1
        je     SHORT loc_1_0DAC
        inc    dword ptr [eax + 34h]
loc_1_0DAC:
        push   0
        mov    dword ptr [edx + 30h], 0
        mov    dword ptr [edx + 38h], 0
        db  0A1h, 06Ch, 0BFh, 000h, 000h
        mov    dword ptr [edx + 34h], eax
        db  068h, 03Ch, 00Bh, 000h, 000h
        push   1f4h
        add    edx, 24h
        push   edx
        call   sub_1_B920
        add    esp, 10h
        mov    eax, 1
        ret
sub_1_0DE0:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 28h]
        mov    ebx, dword ptr [esp + 2ch]
        movzx  eax, word ptr [esi + 3ch]
        mov    ecx, dword ptr [ebx + 4]
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        sub    ax, 14h
        mov    word ptr [esi + 3ch], ax
        call   sub_1_B94A
        mov    eax, dword ptr [ebx + 4]
        sub    eax, 14h
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_0E2C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_0E2C:
        push   5ch
        push   1
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0E5C
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_0E5C:
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_BA0E
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0E94
        push   dword ptr [esp + 14h]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Bh, 0FFh
loc_1_0E94:
        sub    eax, eax
        mov    edi, dword ptr [esp + 18h]
        add    edi, 38h
        mov    ecx, 17h
        rep stosd
        push   1
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0ED8
        push   dword ptr [esp + 14h]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
loc_1_0ED8:
        push   dword ptr [ebx + 4]
        push   ebp
        push   dword ptr [esp + 24h]
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_0F1C
        push   dword ptr [esp + 1ch]
        call   sub_1_BA2C
        push   dword ptr [esp + 14h]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_0F1C:
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 1ch]
        call   sub_1_BA50
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 40h], ebp
        test   byte ptr [esi + 14h], 1
        je     SHORT loc_1_0F70
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   0
        push   dword ptr [ebx]
        call   dword ptr [ebx + 1ch]
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0F70
        push   dword ptr [esp + 1ch]
        call   sub_1_BA2C
        push   dword ptr [esp + 14h]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  090h
loc_1_0F70:
        test   byte ptr [esi + 14h], 4
        je     SHORT loc_1_0FB4
        push   0
        mov    eax, dword ptr [esi + 0ch]
        push   dword ptr [eax + 3ch]
        push   2
        push   dword ptr [ebx]
        call   dword ptr [ebx + 1ch]
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_1_0FB4
        push   dword ptr [esp + 1ch]
        call   sub_1_BA2C
        push   dword ptr [esp + 14h]
        call   sub_1_BA08
        push   ebp
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
        db  08Dh, 049h, 000h
loc_1_0FB4:
        test   byte ptr [ebx + 38h], 1
        je     SHORT loc_1_0FBF
        or     word ptr [esi + 14h], 2
loc_1_0FBF:
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [edx + 2ch]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 38h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edx + 2ch], eax
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_1_0FE0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0BDh, 06Ch, 0C0h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 1ch]
loc_1_0FF5:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     SHORT loc_1_100C
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   ebx
        push   edi
        push   esi
        call   eax
        add    esp, 14h
loc_1_100C:
        add    ebp, 4
        db  081h, 0FDh, 08Ch, 0C0h, 000h, 000h
        jb     SHORT loc_1_0FF5
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_101C:
        push   esi
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_102C
        mov    eax, dword ptr [esp + 8]
        sub    ecx, ecx
        jmp    SHORT loc_1_1032
loc_1_102C:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 8]
loc_1_1032:
        sub    esi, esi
        db  0BAh, 06Ch, 0C0h, 000h, 000h
loc_1_1039:
        cmp    dword ptr [edx], ecx
        je     SHORT loc_1_1050
        add    edx, 4
        inc    esi
        db  081h, 0FAh, 08Ch, 0C0h, 000h, 000h
        jb     SHORT loc_1_1039
        sub    eax, eax
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_1050:
        db  089h, 004h, 0B5h, 06Ch, 0C0h, 000h, 000h
        mov    eax, 1
        pop    esi
        ret
        db  08Bh, 0FFh
sub_1_1060:
        push   ebx
        push   esi
        push   edi
        db  0BBh, 0CCh, 0C2h, 000h, 000h
        mov    si, word ptr [esp + 10h]
        mov    edi, dword ptr [esp + 14h]
loc_1_1071:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     SHORT loc_1_107E
        push   edi
        push   esi
        call   eax
        add    esp, 8
loc_1_107E:
        add    ebx, 4
        db  081h, 0FBh, 0ECh, 0C2h, 000h, 000h
        jb     SHORT loc_1_1071
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
loc_1_1090:
        push   esi
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_10A0
        mov    edx, dword ptr [esp + 8]
        sub    esi, esi
        jmp    SHORT loc_1_10A6
loc_1_10A0:
        sub    edx, edx
        mov    esi, dword ptr [esp + 8]
loc_1_10A6:
        sub    ecx, ecx
        db  0B8h, 0CCh, 0C2h, 000h, 000h
loc_1_10AD:
        cmp    dword ptr [eax], esi
        je     SHORT loc_1_10C0
        add    eax, 4
        inc    ecx
        db  03Dh, 0ECh, 0C2h, 000h, 000h
        jb     SHORT loc_1_10AD
        sub    eax, eax
        pop    esi
        ret
loc_1_10C0:
        pop    esi
        db  089h, 014h, 08Dh, 0CCh, 0C2h, 000h, 000h
        mov    eax, 1
        ret
        db  08Bh, 0FFh
sub_1_10D0:
        sub    esp, 48h
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B94A
        mov    dword ptr [esp + 24h], 0
        lea    eax, [esp + 30h]
        push   eax
        push   dword ptr [esp + 60h]
        call   sub_1_B520
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1108
        mov    eax, 2b2ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  08Bh, 0FFh
loc_1_1108:
        push   dword ptr [esp + 30h]
        mov    dword ptr [esp + 2ch], 0
        lea    eax, [esp + 3ch]
        push   eax
        call   sub_1_B5FC
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_11FF
        db  0FFh, 035h, 03Ch, 0C1h, 000h, 000h
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_11FF
        db  08Bh, 015h, 03Ch, 0C1h, 000h, 000h
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        cmp    dword ptr [esp + 38h], eax
        mov    dword ptr [ebx + 3ch], eax
        jne    SHORT loc_1_1169
        mov    dword ptr [ebx + 3ch], 0ffffffffh
loc_1_1169:
        mov    edi, dword ptr [esp + 6ch]
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 0ch], eax
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ebx + 10h], eax
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [ebx + 18h], eax
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [edi + 2ch]
        mov    dword ptr [ebx + 20h], eax
        mov    eax, dword ptr [edi + 30h]
        mov    dword ptr [ebx + 24h], eax
        mov    eax, dword ptr [edi + 34h]
        mov    dword ptr [ebx + 28h], eax
        mov    dword ptr [ebx + 2ch], 0
        mov    eax, dword ptr [edi + 4]
        sub    eax, 14h
        mov    dword ptr [ebx + 40h], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 44h], eax
        mov    eax, dword ptr [edi + 38h]
        and    eax, 2
        or     eax, eax
        mov    eax, 0
        setne  al
        mov    dword ptr [ebx + 48h], eax
        mov    eax, dword ptr [edi + 3ch]
        mov    dword ptr [ebx + 4ch], eax
        mov    eax, dword ptr [edi + 40h]
        mov    dword ptr [ebx + 50h], eax
        push   dword ptr [esp + 30h]
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_B728
        add    esp, 8
        mov    dword ptr [esp + 28h], eax
        or     eax, eax
        jne    SHORT loc_1_122C
        push   ebx
        call   sub_1_B944
        add    esp, 4
loc_1_11FF:
        push   dword ptr [esp + 30h]
        call   sub_1_B8F4
        add    esp, 4
loc_1_120B:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_1_121E
        push   dword ptr [esp + 28h]
        call   sub_1_B944
        add    esp, 4
loc_1_121E:
        pop    ebp
        mov    eax, 2b2ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
        db  090h
loc_1_122C:
        push   dword ptr [esp + 30h]
        call   sub_1_B8F4
        add    esp, 4
        db  0A1h, 0B4h, 0C0h, 000h, 000h
        mov    dword ptr [ebx], eax
        db  089h, 01Dh, 0B4h, 0C0h, 000h, 000h
        db  083h, 03Dh, 0C8h, 0C1h, 000h, 000h, 000h
        jne    SHORT loc_1_1254
        db  089h, 01Dh, 0C8h, 0C1h, 000h, 000h
loc_1_1254:
        sub    eax, eax
        db  0FFh, 005h, 054h, 0C0h, 000h, 000h
        mov    ecx, 1
        inc    esi
        db  08Bh, 035h, 058h, 0C0h, 000h, 000h
loc_1_1268:
        test   esi, ecx
        je     SHORT loc_1_1278
        add    ecx, ecx
        inc    eax
        cmp    eax, 20h
        jb     SHORT loc_1_1268
        jmp    SHORT loc_1_1287
        db  08Bh, 0FFh
loc_1_1278:
        or     esi, ecx
        lea    edx, [eax + 1]
        mov    dword ptr [esp + 2ch], edx
        db  089h, 035h, 058h, 0C0h, 000h, 000h
loc_1_1287:
        cmp    eax, 20h
        je     NEAR PTR loc_1_120B
        mov    edx, dword ptr [esp + 2ch]
        mov    dword ptr [ebx + 30h], edx
        mov    dword ptr [esp + 18h], 0
        cmp    dword ptr [esp + 20h], 0
        jbe    NEAR PTR loc_1_146C
        mov    ebp, dword ptr [esp + 28h]
loc_1_12AE:
        mov    ecx, dword ptr [ebp]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [esp + 14h], edx
        mov    al, dl
        and    al, 0f0h
        cmp    al, 0e0h
        je     NEAR PTR loc_1_120B
        cmp    al, 0f0h
        jne    SHORT loc_1_12F1
        cmp    edx, -1
        jne    NEAR PTR loc_1_120B
loc_1_12F1:
        db  0B8h, 090h, 0C0h, 000h, 000h
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1324
loc_1_1300:
        test   byte ptr [esi + 14h], 8
        je     SHORT loc_1_1310
        mov    eax, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_1300
        jmp    SHORT loc_1_1320
loc_1_1310:
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi], 0
        db  0FFh, 00Dh, 054h, 0C1h, 000h, 000h
loc_1_1320:
        or     esi, esi
        jne    SHORT loc_1_1338
loc_1_1324:
        push   44h
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_120B
loc_1_1338:
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [esi + 4], ecx
        mov    eax, dword ptr [ebp + 4]
        mov    edx, eax
        and    edx, 0ff00h
        mov    dword ptr [esp + 1ch], eax
        shl    dword ptr [esp + 1ch], 10h
        or     edx, dword ptr [esp + 1ch]
        shl    edx, 8
        mov    dword ptr [esp + 10h], eax
        and    dword ptr [esp + 10h], 0ff0000h
        shr    eax, 10h
        or     dword ptr [esp + 10h], eax
        shr    dword ptr [esp + 10h], 8
        or     edx, dword ptr [esp + 10h]
        mov    dword ptr [esi + 8], edx
        mov    eax, dword ptr [esp + 3ch]
        sub    eax, 44h
        sbb    edx, edx
        not    edx
        and    eax, edx
        add    eax, 44h
        mov    word ptr [esi + 3ch], ax
        mov    eax, dword ptr [esp + 60h]
        mov    dword ptr [esi + 18h], eax
        mov    dword ptr [esi + 0ch], ebx
        cmp    ecx, 1
        sbb    eax, eax
        inc    ax
        mov    word ptr [esi + 14h], ax
        or     ax, 8
        cmp    dword ptr [esp + 18h], 0
        mov    word ptr [esi + 14h], ax
        jne    SHORT loc_1_13F2
        or     ax, 4
        push   dword ptr [esp + 2ch]
        mov    word ptr [esi + 14h], ax
        mov    dword ptr [esp + 28h], esi
        push   edi
        db  068h, 0CCh, 04Bh, 000h, 000h
        db  068h, 040h, 058h, 000h, 000h
        db  068h, 0F4h, 06Dh, 000h, 000h
        db  068h, 0D8h, 05Bh, 000h, 000h
        db  068h, 014h, 055h, 000h, 000h
        push   esi
        push   dword ptr [esp + 7ch]
        call   dword ptr [esp + 8ch]
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_1_120B
loc_1_13F2:
        push   edi
        db  0A1h, 090h, 0C0h, 000h, 000h
        mov    dword ptr [esi], eax
        db  089h, 035h, 090h, 0C0h, 000h, 000h
        db  0FFh, 005h, 054h, 0C1h, 000h, 000h
        push   esi
        call   sub_1_0DE0
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_120B
        push   esi
        call   sub_1_0D60
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_120B
        lea    eax, [esp + 44h]
        push   eax
        push   dword ptr [esp + 44h]
        push   esi
        call   sub_1_8FF8
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1454
        mov    ax, word ptr [esi + 14h]
        test   al, 1
        je     SHORT loc_1_1454
        and    ax, 0fffeh
        mov    word ptr [esi + 14h], ax
        mov    eax, dword ptr [esi + 0ch]
        dec    dword ptr [eax + 34h]
loc_1_1454:
        add    ebp, 8
        inc    dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, dword ptr [esp + 20h]
        jb     NEAR PTR loc_1_12AE
        jmp    SHORT loc_1_1470
        db  090h
loc_1_146C:
        mov    esi, dword ptr [esp + 34h]
loc_1_1470:
        push   dword ptr [edi]
        call   dword ptr [edi + 28h]
        add    esp, 4
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_1_1492
        push   1
        mov    eax, dword ptr [esp + 28h]
        mov    ax, word ptr [eax + 16h]
        push   eax
        call   sub_1_1060
        add    esp, 8
loc_1_1492:
        sub    edi, edi
        cmp    dword ptr [esp + 20h], edi
        jbe    SHORT loc_1_14D6
loc_1_149A:
        push   1
        mov    ax, word ptr [esi + 16h]
        push   eax
        push   dword ptr [esi + 18h]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   sub_1_0FE0
        add    esp, 14h
        cmp    dword ptr [esi + 4], 0
        jne    SHORT loc_1_14C4
        mov    ax, word ptr [esi + 16h]
        push   eax
        call   6
        jmp    SHORT loc_1_14CA
loc_1_14C4:
        push   esi
        call   sub_1_AA74
loc_1_14CA:
        inc    edi
        add    esp, 4
        mov    esi, dword ptr [esi]
        cmp    edi, dword ptr [esp + 20h]
        jb     SHORT loc_1_149A
loc_1_14D6:
        push   dword ptr [esp + 28h]
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 48h
        ret
sub_1_14EC:
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [eax + 0ch]
        db  08Bh, 01Dh, 090h, 0C0h, 000h, 000h
        cmp    ebx, edi
        je     NEAR PTR loc_1_15F5
loc_1_1506:
        cmp    dword ptr [ebx + 0ch], esi
        jne    NEAR PTR loc_1_15EB
        or     edi, edi
        jne    SHORT loc_1_1515
        mov    edi, ebx
loc_1_1515:
        mov    ax, word ptr [ebx + 14h]
        test   al, 1
        je     SHORT loc_1_1570
        and    ax, 0fffeh
        mov    word ptr [ebx + 14h], ax
        push   ebx
        call   sub_1_AAC4
        add    esp, 4
        push   0
        mov    ax, word ptr [ebx + 16h]
        push   eax
        push   dword ptr [ebx + 18h]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_1_0FE0
        add    esp, 14h
        push   0
        push   esi
        db  068h, 074h, 077h, 000h, 000h
        call   sub_1_77EC
        add    esp, 0ch
        db  0A1h, 050h, 0C0h, 000h, 000h
        push   dword ptr [eax + 0ch]
        push   -1
        push   -1
        push   dword ptr [ebx + 4]
        call   sub_1_7F8C
        add    esp, 10h
        jmp    SHORT loc_1_1586
        db  090h
loc_1_1570:
        push   0
        mov    ax, word ptr [ebx + 16h]
        push   eax
        push   dword ptr [ebx + 18h]
        push   0
        push   0
        call   sub_1_0FE0
        add    esp, 14h
loc_1_1586:
        db  039h, 01Dh, 004h, 0BCh, 000h, 000h
        jne    SHORT loc_1_1598
        db  0C7h, 005h, 004h, 0BCh, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1598:
        lea    eax, [ebx + 24h]
        and    word ptr [ebx + 14h], 0fff7h
        push   eax
        call   sub_1_B926
        add    esp, 4
        mov    eax, dword ptr [ebx + 1ch]
        or     eax, eax
        je     SHORT loc_1_15C6
loc_1_15B1:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx + 1ch], ecx
        push   eax
        call   sub_1_4E1C
        add    esp, 4
        mov    eax, dword ptr [ebx + 1ch]
        or     eax, eax
        jne    SHORT loc_1_15B1
loc_1_15C6:
        mov    eax, dword ptr [ebx + 20h]
        or     eax, eax
        je     SHORT loc_1_15EB
loc_1_15CD:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx + 20h], ecx
        push   0
        push   0
        push   0
        push   2b0bh
        push   eax
        call   dword ptr [eax + 8]
        add    esp, 14h
        mov    eax, dword ptr [ebx + 20h]
        or     eax, eax
        jne    SHORT loc_1_15CD
loc_1_15EB:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    NEAR PTR loc_1_1506
loc_1_15F5:
        mov    eax, 1
        mov    ecx, dword ptr [esi + 30h]
        dec    cl
        shl    eax, cl
        not    eax
        db  021h, 005h, 058h, 0C0h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_161C
        push   0
        mov    ax, word ptr [edi + 16h]
        push   eax
        call   sub_1_1060
        add    esp, 8
loc_1_161C:
        mov    eax, dword ptr [esi + 2ch]
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [edi + 4]
        push   dword ptr [eax + 10h]
        call   sub_1_BA08
        push   dword ptr [edi + 8]
        call   sub_1_BA2C
        push   ebx
        call   sub_1_B944
        add    esp, 4
        db  03Bh, 035h, 0C8h, 0C1h, 000h, 000h
        jne    SHORT loc_1_1682
        db  0FFh, 035h, 0C8h, 0C1h, 000h, 000h
        push   -1
        push   -1
        push   -1
        call   sub_1_7F8C
        add    esp, 10h
        db  0FFh, 035h, 0C8h, 0C1h, 000h, 000h
        push   -1
        push   -1
        push   0
        call   sub_1_7F8C
        add    esp, 10h
        db  0C7h, 005h, 0C8h, 0C1h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 06Ch, 0BDh, 000h, 000h, 0FFh, 0FFh, 000h, 000h
loc_1_1682:
        db  0B9h, 0B4h, 0C0h, 000h, 000h
        db  039h, 035h, 0B4h, 0C0h, 000h, 000h
        je     SHORT loc_1_169C
        nop
loc_1_1690:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_1_169C
        mov    ecx, eax
        cmp    dword ptr [ecx], esi
        jne    SHORT loc_1_1690
loc_1_169C:
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_16B4
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        db  0FFh, 00Dh, 054h, 0C0h, 000h, 000h
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_16B4:
        db  083h, 03Dh, 0C8h, 0C1h, 000h, 000h, 000h
        jne    SHORT loc_1_1724
        db  083h, 03Dh, 054h, 0C0h, 000h, 000h, 001h
        je     SHORT loc_1_1724
        db  0A1h, 0B4h, 0C0h, 000h, 000h
        db  0A3h, 0C8h, 0C1h, 000h, 000h
        db  08Bh, 035h, 090h, 0C0h, 000h, 000h
        or     esi, esi
        je     SHORT loc_1_1724
loc_1_16DA:
        test   byte ptr [esi + 14h], 1
        je     SHORT loc_1_171E
        db  03Bh, 035h, 050h, 0C0h, 000h, 000h
        je     SHORT loc_1_171E
        movzx  eax, word ptr [esi + 3ch]
        db  08Bh, 00Dh, 06Ch, 0BDh, 000h, 000h
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        db  0A3h, 06Ch, 0BDh, 000h, 000h
        push   1
        push   2
        push   1
        push   eax
        db  0FFh, 035h, 0C8h, 0C1h, 000h, 000h
        push   -1
        push   -1
        mov    eax, dword ptr [esi + 0ch]
        push   dword ptr [eax + 3ch]
        call   sub_1_7F60
        add    esp, 20h
loc_1_171E:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_16DA
loc_1_1724:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1728:
        sub    esp, 2ch
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 1ch], eax
        cmp    eax, 1
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [esp + 24h], eax
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [esp + 28h], eax
        jne    SHORT loc_1_1752
loc_1_1752:
        mov    esi, dword ptr [esp + 38h]
        lea    edi, [esi + 88h]
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 8ch], 0
        mov    dword ptr [esi + 90h], 0
        call   sub_1_02A6
        lea    eax, [esp + 0ch]
        push   eax
        push   dword ptr [esi + 4]
        lea    eax, [esp + 10h]
        push   eax
        call   sub_1_B9AE
        cmp    dword ptr [esp + 8], 103h
        jne    SHORT loc_1_17A4
        push   edi
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 8], eax
loc_1_17A4:
        mov    eax, dword ptr [esp + 4ch]
        or     eax, eax
        je     SHORT loc_1_17B2
        mov    ecx, dword ptr [esp + 30h]
        mov    dword ptr [eax], ecx
loc_1_17B2:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 2ch
        ret
sub_1_17BC:
        push   esi
        mov    edx, dword ptr [esp + 8]
        movzx  eax, byte ptr [edx + 5ah]
        mov    esi, dword ptr [esp + 0ch]
        cmp    eax, dword ptr [esi + 0ch]
        jb     SHORT loc_1_17D8
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [esi], eax
        mov    dword ptr [edx + 14h], esi
        pop    esi
        ret
loc_1_17D8:
        push   esi
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [edx + 18h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx + 18h], eax
        call   sub_1_BA3E
        pop    esi
        ret
        db  090h
sub_1_17EC:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    eax, dword ptr [esi + 0d4h]
        cmp    dword ptr [esi + 0d8h], eax
        jbe    SHORT loc_1_1835
        push   0ch
        call   sub_1_B93E
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_1835
        push   20h
        lea    eax, [edi + 4]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_1_1840
        push   edi
        call   sub_1_B944
        add    esp, 4
loc_1_1835:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_1_1840:
        movzx  eax, byte ptr [esi + 5ah]
        shl    eax, 5
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_1_1874
        push   dword ptr [edi + 4]
        call   sub_1_BA2C
        push   edi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  090h
loc_1_1874:
        sub    ebx, ebx
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0dch]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi + 0dch], edi
        add    dword ptr [esi + 0d4h], 20h
        sub    ebp, ebp
loc_1_1894:
        mov    ecx, ebp
        movzx  eax, byte ptr [esi + 5ah]
        push   eax
        imul   ecx, eax
        add    ecx, dword ptr [esp + 1ch]
        push   ecx
        push   dword ptr [edi + 4]
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_18DA
        or     ebp, ebp
        je     SHORT loc_1_18D0
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_17BC
        add    esp, 8
        jmp    SHORT loc_1_18D4
        db  090h
loc_1_18D0:
        mov    ebx, dword ptr [esp + 10h]
loc_1_18D4:
        inc    ebp
        cmp    ebp, 20h
        jb     SHORT loc_1_1894
loc_1_18DA:
        mov    eax, ebx
        sub    ebp, 20h
        add    dword ptr [esi + 0d4h], ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
sub_1_18F0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 18h]
        mov    bl, byte ptr [esp + 20h]
        cmp    byte ptr [edi + 5ah], bl
        jb     SHORT loc_1_197C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esp + 0ch], eax
        or     eax, eax
        je     SHORT loc_1_1940
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi + 14h], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], 0
        movzx  eax, bl
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp + 0ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_1940:
        push   edi
        call   sub_1_17EC
        add    esp, 4
        mov    dword ptr [esp + 0ch], eax
        or     eax, eax
        je     SHORT loc_1_196E
        mov    dword ptr [eax], 0
        movzx  eax, bl
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esi], eax
loc_1_196E:
        mov    eax, dword ptr [esp + 0ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_197C:
        mov    ecx, dword ptr [edi + 18h]
        mov    esi, dword ptr [esp + 1ch]
        mov    dword ptr [esi], ecx
        or     ecx, ecx
        je     SHORT loc_1_19D0
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi + 18h], eax
        movzx  eax, bl
        push   eax
        push   ecx
        push   dword ptr [edi + 0ch]
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA32
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_19B8
        mov    eax, dword ptr [esp + 0ch]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_19B8:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [edi + 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 18h], eax
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_19D0:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h, 053h, 056h, 08Bh, 074h, 024h, 014h, 00Bh, 0F6h, 074h, 02Bh, 08Bh, 044h, 024h
        db  00Ch, 08Bh, 04Ch, 024h, 010h, 08Ah, 019h, 08Ah, 0D3h, 0C0h, 0EAh, 004h, 081h, 0E2h, 0FFh, 000h
        db  000h, 000h, 08Ah, 092h, 068h, 0BCh, 000h, 000h, 083h, 0E3h, 00Fh, 00Ah, 093h, 078h, 0BCh, 000h
        db  000h, 088h, 010h, 040h, 041h, 04Eh, 075h, 0DDh, 05Eh, 05Bh, 0C3h
sub_1_1A14:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ax, word ptr [esp + 3ch]
        mov    edx, dword ptr [esp + 24h]
        mov    word ptr [edx], ax
        mov    word ptr [edx + 2], 8
        mov    ebx, dword ptr [esp + 1ch]
        mov    cl, byte ptr [ebx + 5ch]
        mov    byte ptr [edx + 4], cl
        mov    byte ptr [edx + 5], 4
        mov    ax, word ptr [esp + 28h]
        mov    word ptr [edx + 6], ax
        add    edx, 8
        mov    esi, dword ptr [esp + 30h]
        or     esi, esi
        jne    SHORT loc_1_1A52
        lea    esi, [ebx + 54h]
loc_1_1A52:
        mov    edi, edx
        movzx  ebp, cl
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    edx, ebp
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [edx], eax
        add    edx, 4
        mov    esi, dword ptr [esp + 2ch]
        or     esi, esi
        mov    edi, edx
        je     SHORT loc_1_1A8C
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_1_1A9C
        db  08Bh, 0FFh
loc_1_1A8C:
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
loc_1_1A9C:
        sub    edi, edi
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [edx + ebp], eax
        cmp    dword ptr [esp + 40h], edi
        je     SHORT loc_1_1AC4
        cmp    byte ptr [ebx + 5bh], 0
        je     SHORT loc_1_1AC4
        mov    edi, 1
        mov    dword ptr [esp + 10h], 0c0010011h
        mov    esi, dword ptr [esp + 20h]
        jmp    SHORT loc_1_1B16
loc_1_1AC4:
        inc    dword ptr [ebx + 50h]
        mov    esi, dword ptr [esp + 20h]
        push   esi
        push   dword ptr [ebx + 4]
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_B99C
        cmp    dword ptr [esp + 10h], 103h
        je     SHORT loc_1_1B16
        mov    edi, 1
        dec    dword ptr [ebx + 50h]
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_1AFC
        mov    eax, dword ptr [esi + 4]
        add    dword ptr [ebx + 44h], eax
        jmp    SHORT loc_1_1B16
        db  08Bh, 0FFh
loc_1_1AFC:
        cmp    dword ptr [esp + 10h], 0c000009ah
        jne    SHORT loc_1_1B10
        inc    dword ptr [ebx + 0ach]
        jmp    SHORT loc_1_1B16
        db  08Bh, 0FFh
loc_1_1B10:
        inc    dword ptr [ebx + 0b0h]
loc_1_1B16:
        or     edi, edi
        je     SHORT loc_1_1B38
        lea    eax, [esp + 14h]
        push   eax
        push   esi
        call   sub_1_BA5C
        push   dword ptr [esp + 14h]
        push   ebx
        call   sub_1_17BC
        add    esp, 8
        push   esi
        call   sub_1_BA1A
loc_1_1B38:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_1B44:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 15h], 0
        mov    byte ptr [esp + 14h], 0
        mov    ebx, dword ptr [esp + 38h]
        lea    edx, [ebx + 20h]
        sub    ebp, ebp
loc_1_1B5E:
        mov    eax, dword ptr [edx + 8]
        or     eax, eax
        je     SHORT loc_1_1B7C
        mov    ecx, dword ptr [edx + 0ch]
        mov    esi, dword ptr [esp + 3ch]
        and    esi, ecx
        mov    edi, eax
        and    edi, ecx
        cmp    esi, edi
        je     SHORT loc_1_1B84
        or     ebp, ebp
        jne    SHORT loc_1_1B7C
        mov    ebp, eax
loc_1_1B7C:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_1B5E
        jmp    SHORT loc_1_1B87
loc_1_1B84:
        mov    ebp, dword ptr [edx + 8]
loc_1_1B87:
        or     ebp, ebp
        jne    SHORT loc_1_1B98
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
loc_1_1B98:
        push   dword ptr [ebx + 10h]
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        call   sub_1_BA0E
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_1_1BC4
        inc    dword ptr [ebx + 0ach]
        mov    eax, dword ptr [esp + 30h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  090h
loc_1_1BC4:
        mov    eax, dword ptr [esp + 24h]
        mov    byte ptr [eax + 3ch], 0
        inc    dword ptr [ebx + 4ch]
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     SHORT loc_1_1C00
        cmp    eax, 1
        je     SHORT loc_1_1C48
        cmp    eax, 2
        je     NEAR PTR loc_1_1C84
        cmp    eax, 7
        je     NEAR PTR loc_1_1CAC
        inc    dword ptr [ebx + 0b0h]
        mov    eax, 0c0010019h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_1_1C00:
        db  0BEh, 00Ch, 0BCh, 000h, 000h
        mov    byte ptr [esp + 13h], 0eh
        cmp    byte ptr [ebx + 61h], 0
        jne    SHORT loc_1_1C24
        mov    dword ptr [esp + 1ch], 0
        mov    word ptr [esp + 16h], 100h
        jmp    SHORT loc_1_1C38
        db  08Dh, 049h, 000h
loc_1_1C24:
        mov    byte ptr [esp + 14h], 8
        db  0C7h, 044h, 024h, 01Ch, 054h, 0BCh, 000h, 000h
        mov    word ptr [esp + 16h], 600h
loc_1_1C38:
        mov    dword ptr [esp + 20h], 6
        jmp    NEAR PTR loc_1_1CCD
        db  08Dh, 049h, 000h
loc_1_1C48:
        db  0BEh, 01Ch, 0BCh, 000h, 000h
        mov    dword ptr [esp + 20h], 8
        mov    byte ptr [esp + 14h], 8
        db  0C7h, 044h, 024h, 01Ch, 054h, 0BCh, 000h, 000h
        mov    byte ptr [esp + 13h], 0eh
        cmp    byte ptr [esp + 40h], 1
        mov    word ptr [esp + 16h], 600h
        jne    SHORT loc_1_1CCD
        mov    byte ptr [esp + 13h], 10h
        mov    byte ptr [esp + 15h], 80h
        jmp    SHORT loc_1_1CCD
        db  08Dh, 049h, 000h
loc_1_1C84:
        db  0BEh, 030h, 0BCh, 000h, 000h
        mov    byte ptr [esp + 13h], 0dh
        db  0C7h, 044h, 024h, 01Ch, 054h, 0BCh, 000h, 000h
        mov    byte ptr [esp + 14h], 8
        mov    dword ptr [esp + 20h], 7
        mov    word ptr [esp + 16h], 100h
        jmp    SHORT loc_1_1CCD
loc_1_1CAC:
        db  0BEh, 040h, 0BCh, 000h, 000h
        mov    byte ptr [esp + 13h], 3
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 0
        mov    word ptr [esp + 16h], 700h
loc_1_1CCD:
        mov    al, byte ptr [esp + 14h]
        add    al, byte ptr [esp + 13h]
        add    al, 1ch
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ebx
        call   sub_1_18F0
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1D08
        push   dword ptr [esp + 24h]
        call   sub_1_BA1A
        inc    dword ptr [ebx + 0ach]
        mov    eax, 0c000009ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Bh, 0FFh
loc_1_1D08:
        cmp    dword ptr [ebx + 8], 7
        jne    SHORT loc_1_1D12
        sub    dword ptr [eax + 0ch], 0ah
loc_1_1D12:
        mov    edi, dword ptr [esp + 18h]
        movzx  ecx, byte ptr [esp + 13h]
        mov    dword ptr [esp + 28h], ecx
        mov    edx, ecx
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        cmp    dword ptr [esp + 44h], 0
        jne    SHORT loc_1_1D3D
        lea    ecx, [ebx + 54h]
        mov    dword ptr [esp + 44h], ecx
loc_1_1D3D:
        cmp    dword ptr [ebx + 8], 0
        jne    SHORT loc_1_1D53
        cmp    byte ptr [ebx + 61h], 0
        je     SHORT loc_1_1D53
        mov    ecx, dword ptr [esp + 18h]
        mov    word ptr [ecx + 0ch], 2400h
loc_1_1D53:
        movzx  edx, byte ptr [ebx + 5ch]
        mov    edi, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 44h]
        mov    ecx, edx
        shr    ecx, 2
        add    edi, dword ptr [esp + 20h]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [esp + 18h]
        mov    cl, byte ptr [esp + 15h]
        mov    edx, dword ptr [esp + 20h]
        or     byte ptr [esi + edx], cl
        movzx  ecx, byte ptr [esp + 14h]
        mov    dword ptr [esp + 2ch], ecx
        mov    edx, ecx
        mov    edi, dword ptr [esp + 28h]
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, edx
        shr    ecx, 2
        add    edi, dword ptr [esp + 18h]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        push   eax
        push   dword ptr [esp + 28h]
        call   sub_1_BA50
        push   dword ptr [esp + 48h]
        push   dword ptr [esp + 1ah]
        push   ebp
        push   dword ptr [esp + 48h]
        push   dword ptr [esp + 54h]
        push   0
        push   100h
        mov    eax, dword ptr [esp + 34h]
        add    eax, dword ptr [esp + 48h]
        add    eax, dword ptr [esp + 44h]
        push   eax
        push   dword ptr [esp + 44h]
        push   ebx
        call   sub_1_1A14
        add    esp, 28h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Dh, 049h, 000h
sub_1_1DEC:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 24h]
        push   dword ptr [ebp + 10h]
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_BA0E
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_1E20
        inc    dword ptr [ebp + 0ach]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_1E20:
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [eax + 3ch], 0
        inc    dword ptr [ebp + 48h]
        mov    al, byte ptr [ebp + 60h]
        mov    byte ptr [esp + 13h], al
        cmp    dword ptr [esp + 3ch], 0
        je     SHORT loc_1_1E40
        add    al, byte ptr [ebp + 61h]
        mov    byte ptr [esp + 13h], al
loc_1_1E40:
        cmp    dword ptr [ebp + 8], 1
        mov    ebx, dword ptr [esp + 38h]
        jne    SHORT loc_1_1E4E
        add    byte ptr [esp + 13h], bl
loc_1_1E4E:
        mov    al, byte ptr [esp + 13h]
        add    al, 1ch
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ebp
        call   sub_1_18F0
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1E80
        inc    dword ptr [ebp + 0ach]
        push   dword ptr [esp + 18h]
        call   sub_1_BA1A
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_1E80:
        mov    ecx, dword ptr [ebp + 8]
        or     ecx, ecx
        je     SHORT loc_1_1EC4
        cmp    ecx, 1
        je     NEAR PTR loc_1_1F10
        cmp    ecx, 2
        je     NEAR PTR loc_1_1F78
        cmp    ecx, 7
        je     NEAR PTR loc_1_1FA8
        inc    dword ptr [ebp + 0b0h]
        push   eax
        push   ebp
        call   sub_1_17BC
        add    esp, 8
        push   dword ptr [esp + 18h]
        call   sub_1_BA1A
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_1EC4:
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, ecx
        mov    esi, dword ptr [esp + 30h]
        movsd
        movsw
        lea    esi, [ebp + 54h]
        lea    edi, [ecx + 6]
        movsd
        movsw
        cmp    dword ptr [esp + 3ch], 0
        jne    SHORT loc_1_1EF0
        mov    word ptr [ecx + 0ch], 608h
        mov    bx, 100h
        jmp    NEAR PTR loc_1_1FC8
loc_1_1EF0:
        db  0BEh, 054h, 0BCh, 000h, 000h
        mov    word ptr [ecx + 0ch], 2400h
        mov    bx, 600h
        mov    edi, dword ptr [esp + 14h]
        add    edi, 0eh
        movsd
        movsd
        jmp    NEAR PTR loc_1_1FC8
        db  08Dh, 049h, 000h
loc_1_1F10:
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, edx
        mov    byte ptr [ecx], 10h
        mov    byte ptr [edx + 1], 40h
        lea    edi, [edx + 2]
        mov    esi, dword ptr [esp + 30h]
        movsd
        movsw
        lea    esi, [ebp + 54h]
        lea    edi, [edx + 8]
        movsd
        movsw
        or     ebx, ebx
        je     SHORT loc_1_1F61
        mov    edi, dword ptr [esp + 14h]
        add    edi, 0eh
        mov    esi, dword ptr [esp + 34h]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 14h]
        and    byte ptr [ecx + 0eh], 1fh
        mov    ecx, dword ptr [esp + 14h]
        xor    byte ptr [ecx + 0fh], 80h
        or     byte ptr [edx + 8], 80h
loc_1_1F61:
        db  0BEh, 054h, 0BCh, 000h, 000h
        mov    ecx, dword ptr [esp + 14h]
        lea    edi, [ecx + ebx + 0eh]
        movsd
        movsd
        mov    bx, 600h
        jmp    SHORT loc_1_1FC8
        db  08Bh, 0FFh
loc_1_1F78:
        mov    edx, dword ptr [esp + 14h]
        mov    ecx, edx
        mov    byte ptr [ecx], 57h
        lea    edi, [edx + 1]
        mov    esi, dword ptr [esp + 30h]
        movsd
        movsw
        lea    esi, [ebp + 54h]
        lea    edi, [edx + 7]
        movsd
        movsw
        db  0BEh, 054h, 0BCh, 000h, 000h
        mov    edi, dword ptr [esp + 14h]
        add    edi, 0dh
        movsd
        movsd
        mov    bx, 100h
        jmp    SHORT loc_1_1FC8
loc_1_1FA8:
        mov    dl, byte ptr [ebp + 54h]
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, esi
        mov    byte ptr [ecx], dl
        mov    ecx, dword ptr [esp + 30h]
        mov    cl, byte ptr [ecx]
        mov    byte ptr [esi + 1], cl
        mov    byte ptr [esi + 2], 0d5h
        sub    dword ptr [eax + 0ch], 0ah
        mov    bx, 700h
loc_1_1FC8:
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_BA50
        push   1
        push   ebx
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 34h]
        push   0
        push   dword ptr [esp + 44h]
        push   200h
        movzx  eax, byte ptr [esp + 2fh]
        add    eax, dword ptr [esp + 30h]
        push   eax
        push   dword ptr [esp + 38h]
        push   ebp
        call   sub_1_1A14
        add    esp, 28h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
sub_1_2008:
        mov    edx, dword ptr [esp + 4]
        add    edx, 10h
        cmp    dword ptr [edx], 0
        je     SHORT loc_1_2036
        mov    ecx, dword ptr [esp + 8]
loc_1_2018:
        mov    eax, dword ptr [edx]
        cmp    ecx, eax
        je     SHORT loc_1_2028
        lea    edx, [eax + 18h]
        cmp    dword ptr [edx], 0
        jne    SHORT loc_1_2018
        ret
        db  090h
loc_1_2028:
        add    ecx, 18h
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    dword ptr [ecx + 4], 0
loc_1_2036:
        ret
        db  090h
sub_1_2038:
        mov    cl, byte ptr [esp + 0bh]
        and    ecx, 1fh
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 1ch]
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     SHORT loc_1_205C
loc_1_204D:
        mov    ecx, dword ptr [esp + 8]
        cmp    dword ptr [eax + 8], ecx
        je     SHORT loc_1_205E
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_204D
loc_1_205C:
        sub    eax, eax
loc_1_205E:
        ret
        db  090h
sub_1_2060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 34h]
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_1_20EC
        mov    al, byte ptr [ebp + 0ch]
        and    al, 0f0h
        cmp    al, 0e0h
        je     SHORT loc_1_20EC
        add    esi, 20h
        nop
loc_1_2080:
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_20C5
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, eax
        not    ecx
        and    ecx, edx
        mov    edi, dword ptr [ebp - 4]
        and    edi, eax
        or     ecx, edi
        cmp    ecx, dword ptr [ebp + 0ch]
        je     SHORT loc_1_20D4
        mov    al, byte ptr [ebp - 4]
        shr    al, 2
        and    eax, 0ffh
        and    eax, 0fffffffch
        db  08Bh, 080h, 000h, 0BDh, 000h, 000h
        mov    edi, eax
        not    edi
        and    edi, edx
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, eax
        or     edi, ecx
        cmp    edi, dword ptr [ebp + 0ch]
        je     SHORT loc_1_20E0
loc_1_20C5:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_2080
        sub    eax, eax
        pop    edi
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_20D4:
        pop    edi
        mov    eax, 1
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_20E0:
        pop    edi
        mov    eax, 1
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_20EC:
        pop    edi
        mov    eax, 1
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
sub_1_20F8:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 28h]
        mov    ebx, dword ptr [esp + 28h]
        push   ebx
        call   sub_1_2060
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_1_2388
        cmp    byte ptr [ebx + 5bh], 0
        jne    NEAR PTR loc_1_2370
        mov    al, byte ptr [ebx + 60h]
        add    al, byte ptr [ebx + 61h]
        cmp    dword ptr [ebx + 8], 1
        jne    SHORT loc_1_2130
        add    al, 2
loc_1_2130:
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   ebx
        call   sub_1_18F0
        add    esp, 0ch
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     NEAR PTR loc_1_2358
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     SHORT loc_1_2190
        cmp    eax, 1
        je     NEAR PTR loc_1_2220
        cmp    eax, 2
        je     NEAR PTR loc_1_2258
        cmp    eax, 7
        je     NEAR PTR loc_1_22B0
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [eax], 0c0010019h
        push   dword ptr [esp + 14h]
        push   ebx
        call   sub_1_17BC
        add    esp, 8
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
loc_1_2190:
        mov    ebp, dword ptr [esp + 10h]
        mov    al, byte ptr [esp + 28h]
        and    al, 0f0h
        cmp    al, 0e0h
        je     SHORT loc_1_21AC
        db  0BEh, 00Ch, 0BCh, 000h, 000h
        mov    edi, ebp
        movsd
        movsw
        jmp    SHORT loc_1_21C4
        db  08Bh, 0FFh
loc_1_21AC:
        db  0BEh, 044h, 0BCh, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        movsd
        movsw
        mov    eax, dword ptr [esp + 28h]
        and    eax, 0ffff7f00h
        or     dword ptr [ebp + 2], eax
loc_1_21C4:
        lea    esi, [ebx + 54h]
        lea    edi, [ebp + 6]
        movsd
        movsw
        cmp    byte ptr [ebx + 61h], 0
        jne    SHORT loc_1_21E0
        mov    word ptr [ebp + 0ch], 8
        jmp    NEAR PTR loc_1_22C3
        db  08Bh, 0FFh
loc_1_21E0:
        lea    eax, [esp + 1ch]
        push   eax
        push   0
        push   0
        push   0
        push   dword ptr [esp + 3ch]
        call   sub_1_BA20
        mov    eax, dword ptr [esp + 1ch]
        add    ax, 8
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebp + 0ch], cx
        mov    eax, dword ptr [esp + 10h]
        add    eax, 0eh
        db  0BEh, 054h, 0BCh, 000h, 000h
        mov    edi, eax
        movsd
        movsd
        mov    word ptr [eax + 6], 8
        jmp    NEAR PTR loc_1_22C3
        db  090h
loc_1_2220:
        db  0BEh, 01Ch, 0BCh, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        mov    edi, eax
        movsd
        movsd
        lea    esi, [ebx + 54h]
        lea    edi, [eax + 8]
        movsd
        movsw
        or     byte ptr [eax + 8], 80h
        add    eax, 0eh
        mov    byte ptr [eax], 0c2h
        mov    byte ptr [eax + 1], 70h
        add    eax, 2
        db  0BEh, 054h, 0BCh, 000h, 000h
        mov    edi, eax
        movsd
        movsd
        mov    word ptr [eax + 6], 8
        jmp    SHORT loc_1_22C3
loc_1_2258:
        mov    ecx, dword ptr [esp + 10h]
        mov    al, byte ptr [esp + 28h]
        and    al, 0f0h
        cmp    al, 0e0h
        je     SHORT loc_1_2274
        db  0BEh, 030h, 0BCh, 000h, 000h
        mov    edi, ecx
        movsd
        movsw
        movsb
        jmp    SHORT loc_1_228D
        db  090h
loc_1_2274:
        db  0BEh, 04Ch, 0BCh, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        movsd
        movsw
        movsb
        mov    eax, dword ptr [esp + 28h]
        and    eax, 0ffff7f00h
        or     dword ptr [ecx + 3], eax
loc_1_228D:
        lea    esi, [ebx + 54h]
        lea    edi, [ecx + 7]
        movsd
        movsw
        mov    eax, dword ptr [esp + 10h]
        add    eax, 0dh
        db  0BEh, 054h, 0BCh, 000h, 000h
        mov    edi, eax
        movsd
        movsd
        mov    word ptr [eax + 6], 8
        jmp    SHORT loc_1_22C3
        db  08Bh, 0FFh
loc_1_22B0:
        mov    dl, byte ptr [ebx + 54h]
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, eax
        mov    byte ptr [ecx], dl
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [eax + 2], 0d4h
loc_1_22C3:
        inc    dword ptr [ebx + 4ch]
        inc    dword ptr [ebx + 50h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 30h]
        call   sub_1_BA50
        push   dword ptr [esp + 2ch]
        push   dword ptr [ebx + 4]
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_B99C
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esp + 18h], 103h
        je     SHORT loc_1_237A
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_2310
        mov    eax, dword ptr [esp + 2ch]
        mov    eax, dword ptr [eax + 4]
        add    dword ptr [ebx + 44h], eax
        jmp    SHORT loc_1_232A
        db  08Bh, 0FFh
loc_1_2310:
        cmp    dword ptr [esp + 18h], 0c000009ah
        jne    SHORT loc_1_2324
        inc    dword ptr [ebx + 0ach]
        jmp    SHORT loc_1_232A
        db  08Bh, 0FFh
loc_1_2324:
        inc    dword ptr [ebx + 0b0h]
loc_1_232A:
        dec    dword ptr [ebx + 50h]
        lea    eax, [esp + 14h]
        push   eax
        push   dword ptr [esp + 30h]
        call   sub_1_BA5C
        push   dword ptr [esp + 14h]
        push   ebx
        call   sub_1_17BC
        add    esp, 8
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_2358:
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [eax], 0c000009ah
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_2370:
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [eax], 0c0010011h
loc_1_237A:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_2388:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Bh, 0FFh
sub_1_2394:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 24h]
        cmp    byte ptr [ebx + 5bh], 0
        jne    NEAR PTR loc_1_2530
        mov    ebp, dword ptr [esp + 2ch]
        mov    al, byte ptr [ebp + 19h]
        cmp    al, 2
        jne    NEAR PTR loc_1_24C4
        mov    dword ptr [ebp + 14h], 78h
        mov    al, byte ptr [ebp + 18h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   ebx
        call   sub_1_18F0
        add    esp, 0ch
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     NEAR PTR loc_1_24B0
        inc    dword ptr [ebx + 48h]
        inc    dword ptr [ebx + 50h]
        movzx  eax, byte ptr [ebp + 18h]
        lea    esi, [ebp + 1ah]
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    dword ptr [ebx + 8], 0
        jne    SHORT loc_1_2434
        cmp    byte ptr [ebp + 18h], 0eh
        je     SHORT loc_1_2434
        lea    eax, [esp + 1ch]
        push   eax
        push   0
        push   0
        push   0
        mov    esi, dword ptr [esp + 38h]
        push   esi
        call   sub_1_BA20
        mov    eax, dword ptr [esp + 1ch]
        add    ax, 8
        mov    ch, al
        mov    cl, ah
        mov    eax, dword ptr [esp + 18h]
        mov    word ptr [eax + 0ch], cx
        jmp    SHORT loc_1_2438
        db  090h
loc_1_2434:
        mov    esi, dword ptr [esp + 28h]
loc_1_2438:
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_1_BA50
        push   esi
        push   dword ptr [ebx + 4]
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_B99C
        cmp    dword ptr [esp + 10h], 103h
        je     SHORT loc_1_24A1
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_246C
        mov    eax, dword ptr [esi + 4]
        add    dword ptr [ebx + 44h], eax
        jmp    SHORT loc_1_2486
        db  08Dh, 049h, 000h
loc_1_246C:
        cmp    dword ptr [esp + 10h], 0c000009ah
        jne    SHORT loc_1_2480
        inc    dword ptr [ebx + 0ach]
        jmp    SHORT loc_1_2486
        db  08Bh, 0FFh
loc_1_2480:
        inc    dword ptr [ebx + 0b0h]
loc_1_2486:
        dec    dword ptr [ebx + 50h]
        lea    eax, [esp + 14h]
        push   eax
        push   esi
        call   sub_1_BA5C
        push   dword ptr [esp + 14h]
        push   ebx
        call   sub_1_17BC
        add    esp, 8
loc_1_24A1:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_24B0:
        pop    ebp
        inc    dword ptr [ebx + 0ach]
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_24C4:
        cmp    al, 1
        ja     SHORT loc_1_251C
        mov    edi, dword ptr [ebp + 0ch]
        call   sub_1_B92C
        mov    esi, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 0ch], esi
        mov    ecx, eax
        sub    ecx, dword ptr [ebp + 4]
        cmp    ecx, 3e8h
        jbe    SHORT loc_1_24FD
        mov    dword ptr [ebp + 4], eax
        mov    byte ptr [ebp + 19h], 1
        push   1
        push   0
        push   1
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_1_1B44
        add    esp, 14h
loc_1_24FD:
        or     edi, edi
        je     SHORT loc_1_250E
        push   0
        push   edi
        push   dword ptr [ebx]
        call   sub_1_5BD8
        add    esp, 0ch
loc_1_250E:
        pop    ebp
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_251C:
        pop    ebp
        inc    dword ptr [ebx + 0b0h]
        mov    eax, 0c001000fh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_2530:
        pop    ebp
        mov    eax, 0c0010011h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
sub_1_2540:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    al, byte ptr [esp + 23h]
        and    eax, 1fh
        mov    dword ptr [esp + 14h], eax
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        mov    esi, dword ptr [esp + 24h]
        push   esi
        call   sub_1_2038
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_2580
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_2580:
        mov    ebp, 28h
        cmp    dword ptr [esi + 8], 1
        je     SHORT loc_1_2596
        movzx  eax, byte ptr [esi + 60h]
        movzx  ecx, byte ptr [esi + 61h]
        lea    ebp, [eax + ecx]
loc_1_2596:
        add    ebp, 1ah
        push   ebp
        call   sub_1_B93E
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_25B4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_25B4:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, ebp
        shr    ecx, 2
        rep stosd
        mov    ecx, ebp
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebx + 8], eax
        cmp    dword ptr [esi + 8], 1
        jne    SHORT loc_1_25E0
        db  083h, 03Dh, 0C4h, 0C1h, 000h, 000h, 000h
        mov    byte ptr [ebx + 19h], 0
        je     SHORT loc_1_25E4
loc_1_25E0:
        mov    byte ptr [ebx + 19h], 1
loc_1_25E4:
        mov    dword ptr [ebx + 10h], 0
        call   sub_1_B92C
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 14h], 78h
        mov    ecx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esp + 14h]
        lea    eax, [ecx + eax*4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [eax], ebx
        inc    dword ptr [esi + 68h]
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [ecx], edx
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_2624:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 30h]
        or     esi, esi
        je     SHORT loc_1_2670
        lea    ebp, [esi + 18h]
        mov    eax, dword ptr [ebp + 4]
        or     eax, eax
        mov    edi, dword ptr [esp + 2ch]
        je     SHORT loc_1_2678
        cmp    dword ptr [eax + 8], edi
        jne    SHORT loc_1_2664
        push   dword ptr [esp + 18h]
        push   eax
        push   dword ptr [esp + 30h]
        push   dword ptr [esp + 30h]
        call   sub_1_2394
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h
loc_1_2664:
        push   esi
        push   eax
        call   sub_1_2008
        add    esp, 8
        jmp    SHORT loc_1_2678
loc_1_2670:
        mov    edi, dword ptr [esp + 2ch]
        mov    ebp, dword ptr [esp + 1ch]
loc_1_2678:
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        mov    ebx, dword ptr [esp + 2ch]
        push   ebx
        call   sub_1_2038
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_26BC
        or     esi, esi
        je     SHORT loc_1_269F
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [ebp], ecx
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [ebp + 4], eax
loc_1_269F:
        push   dword ptr [esp + 18h]
        push   eax
        push   dword ptr [esp + 30h]
        push   ebx
        call   sub_1_2394
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  08Dh, 049h, 000h
loc_1_26BC:
        lea    eax, [esp + 14h]
        push   eax
        mov    esi, dword ptr [esp + 2ch]
        push   esi
        push   edi
        push   ebx
        call   sub_1_20F8
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_26E0
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_26E0:
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        push   ebx
        call   sub_1_2540
        add    esp, 0ch
        db  00Bh
sub_1_26F0:
        sal    byte ptr [ecx + esi*2 - 76h], 48h
        sbb    dword ptr [eax + 3d7701f9h], eax
        cmp    dword ptr [eax + 0ch], 0
        jne    SHORT loc_1_2720
        mov    dword ptr [eax + 0ch], esi
        push   1
        push   0
        push   ecx
        push   edi
        push   ebx
        call   sub_1_1B44
        add    esp, 14h
        mov    eax, 103h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_1_2720:
        push   dword ptr [esp + 10h]
        push   eax
        push   esi
        push   ebx
        call   sub_1_2394
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  090h, 080h, 0F9h, 002h, 075h, 017h, 0FFh, 074h, 024h, 010h, 050h, 056h, 053h, 0E8h, 04Bh, 0FCh
        db  0FFh, 0FFh, 083h, 0C4h, 010h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 010h, 0C3h, 05Dh, 0B8h, 09Ah
        db  000h, 000h, 0C0h, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 010h, 0C3h, 08Dh, 049h, 000h, 05Dh, 0B8h, 09Ah
        db  000h, 000h, 0C0h, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 010h, 0C3h, 08Dh, 049h, 000h
sub_1_2774:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax + 10h]
        or     ecx, ecx
        je     SHORT loc_1_2790
        nop
loc_1_2780:
        add    ecx, 18h
        mov    dword ptr [ecx + 4], 0
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_2780
loc_1_2790:
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        ret
        db  08Dh, 049h, 000h
loc_1_279C:
        sub    esp, 4
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 1ch]
        add    eax, dword ptr [esp + 20h]
        push   eax
        push   dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [eax + 4]
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_B9A2
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_27D4:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    byte ptr [eax + 97h], 2
        mov    byte ptr [eax + 5bh], 2
        lea    esi, [eax + 88h]
        mov    dword ptr [esi], 0
        mov    dword ptr [eax + 8ch], 0
        mov    dword ptr [eax + 90h], 0
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     SHORT loc_1_2830
        push   eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_B996
        cmp    dword ptr [esp + 4], 103h
        jne    SHORT loc_1_2830
        push   esi
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 4], eax
loc_1_2830:
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_2838:
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        lea    edi, [eax + 18h]
        mov    ecx, dword ptr [edi + 4]
        or     ecx, ecx
        je     SHORT loc_1_2855
        push   eax
        push   ecx
        call   sub_1_2008
        add    esp, 8
        sub    eax, eax
        stosd
        stosd
loc_1_2855:
        pop    edi
        ret
        db  090h
sub_1_2858:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ebx + 0cch]
        lea    eax, [eax + eax*2]
        add    eax, eax
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        mov    ebp, 0
        je     SHORT loc_1_2886
        push   eax
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
loc_1_2886:
        or     ebp, ebp
        jne    SHORT loc_1_28A0
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_28A0
        mov    esi, 0c000009ah
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_28A0:
        mov    eax, ebp
        mov    edx, dword ptr [ebx + 0c8h]
        sub    ecx, ecx
        cmp    dword ptr [ebx + 0cch], ecx
        jbe    SHORT loc_1_28E3
loc_1_28B2:
        mov    esi, dword ptr [ebx + 8]
        or     esi, esi
        jne    SHORT loc_1_28C0
        db  0BEh, 044h, 0BCh, 000h, 000h
        jmp    SHORT loc_1_28CA
loc_1_28C0:
        cmp    esi, 2
        jne    SHORT loc_1_28D5
        db  0BEh, 04Dh, 0BCh, 000h, 000h
loc_1_28CA:
        mov    edi, eax
        movsd
        movsw
        mov    esi, dword ptr [edx + 4]
        or     dword ptr [eax + 2], esi
loc_1_28D5:
        mov    edx, dword ptr [edx]
        add    eax, 6
        inc    ecx
        cmp    dword ptr [ebx + 0cch], ecx
        ja     SHORT loc_1_28B2
loc_1_28E3:
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        jne    SHORT loc_1_2908
        push   0
        push   dword ptr [esp + 14h]
        push   ebp
        push   1010103h
        push   1
        push   ebx
        call   sub_1_1728
        add    esp, 18h
        mov    esi, eax
        jmp    SHORT loc_1_292C
        db  08Dh, 049h, 000h
loc_1_2908:
        cmp    eax, 2
        jne    SHORT loc_1_2928
        push   0
        push   dword ptr [esp + 14h]
        push   ebp
        push   3010103h
        push   1
        push   ebx
        call   sub_1_1728
        add    esp, 18h
        mov    esi, eax
        jmp    SHORT loc_1_292C
loc_1_2928:
        mov    esi, dword ptr [esp + 14h]
loc_1_292C:
        or     ebp, ebp
        je     SHORT loc_1_2939
        push   ebp
        call   sub_1_B944
        add    esp, 4
loc_1_2939:
        cmp    esi, 0c0010009h
        jne    SHORT loc_1_295E
        lea    eax, [ebx + 64h]
        or     dword ptr [eax], 4
        push   0
        push   4
        push   eax
        push   1010eh
        push   1
        push   ebx
        call   sub_1_1728
        add    esp, 18h
        mov    esi, eax
loc_1_295E:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_2968:
        mov    edx, dword ptr [esp + 4]
        add    edx, 0c8h
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_2989
        mov    ecx, dword ptr [esp + 8]
loc_1_297C:
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_1_2989
        mov    edx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_297C
loc_1_2989:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], edx
        ret
sub_1_2990:
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_1_29AA
        cmp    eax, 2
        jne    SHORT loc_1_29F6
loc_1_29AA:
        lea    eax, [esp + 8]
        push   eax
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_1_2968
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_29F4
        mov    ecx, dword ptr [eax + 8]
        dec    ecx
        mov    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_29F6
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [edx], ecx
        dec    dword ptr [esi + 0cch]
        push   eax
        call   sub_1_B944
        add    esp, 4
        push   esi
        call   sub_1_2858
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_29F4:
        sub    edi, edi
loc_1_29F6:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_1_2A00:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [esp + 14h]
        cmp    byte ptr [esi + 5bh], 0
        je     SHORT loc_1_2A20
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_2A20:
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     SHORT loc_1_2A2C
        cmp    eax, 2
        jne    SHORT loc_1_2AAA
loc_1_2A2C:
        mov    ebx, dword ptr [esp + 18h]
        and    ebx, 0ffff7f00h
        lea    eax, [esp + 0ch]
        push   eax
        push   ebx
        push   esi
        call   sub_1_2968
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_2A58
        inc    dword ptr [eax + 8]
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_2A58:
        push   0ch
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2AA8
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], 1
        mov    ecx, dword ptr [esi + 0c8h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 0c8h], eax
        inc    dword ptr [esi + 0cch]
        push   esi
        call   sub_1_2858
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2AAA
        push   ebx
        push   esi
        call   sub_1_2990
        add    esp, 8
        sub    edi, edi
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_2AA8:
        sub    edi, edi
loc_1_2AAA:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  090h
sub_1_2AB4:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        je     SHORT loc_1_2B00
        cmp    eax, 4
        je     NEAR PTR loc_1_2B7C
        cmp    eax, 2
        jne    SHORT loc_1_2AE0
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [esi + 34h], ecx
        mov    eax, 1
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_2AE0:
        cmp    eax, 1
        jne    SHORT loc_1_2AF8
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 10h]
        call   sub_1_2A00
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_1_2AF8:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_2B00:
        cmp    eax, 4
        je     SHORT loc_1_2B81
        sub    edi, edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 28h], edi
        jne    SHORT loc_1_2B28
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [esi + 28h], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi + 2ch], ecx
        mov    dword ptr [esi + 24h], 3
        jmp    SHORT loc_1_2B53
        db  090h
loc_1_2B28:
        push   10h
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2B58
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [eax + 4], 3
        mov    ecx, dword ptr [esi + 20h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 20h], eax
loc_1_2B53:
        mov    edi, 1
loc_1_2B58:
        cmp    edi, 1
        jne    SHORT loc_1_2B76
        cmp    byte ptr [esi + 5bh], 0
        jne    SHORT loc_1_2B76
        push   1
        push   0
        push   1
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_1B44
        add    esp, 14h
loc_1_2B76:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
        db  090h
loc_1_2B7C:
        cmp    eax, 4
        jne    SHORT loc_1_2BB6
loc_1_2B81:
        push   0ch
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_2BB4
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 8], ecx
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esi + 30h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 30h], eax
        inc    dword ptr [esi + 6ch]
        mov    eax, 1
        pop    edi
        pop    esi
        ret
loc_1_2BB4:
        sub    eax, eax
loc_1_2BB6:
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
sub_1_2BBC:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        jne    SHORT loc_1_2C14
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [edi + 28h], eax
        jne    SHORT loc_1_2BE4
        mov    dword ptr [edi + 28h], 0
        mov    eax, 1
        pop    edi
        pop    esi
        ret
        db  08Bh, 0FFh
loc_1_2BE4:
        add    edi, 20h
        mov    esi, dword ptr [edi]
        or     esi, esi
        je     SHORT loc_1_2C0B
loc_1_2BED:
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_2BFA
        mov    edi, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_2BED
loc_1_2BFA:
        or     esi, esi
        je     SHORT loc_1_2C0B
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_2C0B:
        pop    edi
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
        pop    esi
        ret
loc_1_2C14:
        cmp    eax, 4
        jne    SHORT loc_1_2C60
        mov    edi, dword ptr [esp + 0ch]
        lea    edx, [edi + 30h]
        mov    esi, dword ptr [edx]
        or     esi, esi
        je     SHORT loc_1_2C54
        mov    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 18h]
loc_1_2C2E:
        cmp    dword ptr [esi + 4], eax
        jne    SHORT loc_1_2C38
        cmp    dword ptr [esi + 8], ecx
        je     SHORT loc_1_2C40
loc_1_2C38:
        mov    edx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_2C2E
loc_1_2C40:
        or     esi, esi
        je     SHORT loc_1_2C54
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        dec    dword ptr [edi + 6ch]
        push   esi
        call   sub_1_B944
        add    esp, 4
loc_1_2C54:
        pop    edi
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_2C60:
        cmp    eax, 1
        jne    SHORT loc_1_2C78
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 10h]
        call   sub_1_2990
        add    esp, 8
        pop    edi
        pop    esi
        ret
loc_1_2C78:
        sub    eax, eax
        pop    edi
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_2C80:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_B92C
        mov    dword ptr [esp + 14h], eax
        sub    ebp, ebp
        mov    eax, dword ptr [esp + 24h]
        add    eax, 20h
loc_1_2C99:
        mov    ecx, dword ptr [eax + 4]
        or     ecx, ecx
        je     SHORT loc_1_2CA4
        dec    ecx
        mov    dword ptr [eax + 4], ecx
loc_1_2CA4:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_2C99
        mov    eax, dword ptr [esp + 24h]
        mov    ebx, dword ptr [eax + 1ch]
        mov    dword ptr [esp + 18h], 20h
loc_1_2CB9:
        mov    edi, ebx
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     NEAR PTR loc_1_2D70
loc_1_2CC5:
        mov    al, byte ptr [esi + 19h]
        mov    byte ptr [esp + 13h], 0
        cmp    al, 2
        mov    ecx, 927c0h
        je     SHORT loc_1_2CDB
        mov    ecx, 3e8h
loc_1_2CDB:
        cmp    dword ptr [esi + 4], -1
        je     SHORT loc_1_2D1F
        mov    edx, dword ptr [esp + 14h]
        sub    edx, dword ptr [esi + 4]
        cmp    edx, ecx
        ja     SHORT loc_1_2CF9
        cmp    al, 2
        jne    SHORT loc_1_2D1F
        mov    ecx, dword ptr [esi + 14h]
        dec    ecx
        mov    dword ptr [esi + 14h], ecx
        jne    SHORT loc_1_2D1F
loc_1_2CF9:
        or     al, al
        je     SHORT loc_1_2D38
        mov    eax, dword ptr [esi + 0ch]
        or     eax, eax
        je     SHORT loc_1_2D09
        mov    dword ptr [eax + 38h], ebp
        mov    ebp, eax
loc_1_2D09:
        push   esi
        push   edi
        call   sub_1_2774
        add    esp, 8
        mov    eax, dword ptr [esp + 24h]
        dec    dword ptr [eax + 68h]
        mov    byte ptr [esp + 13h], 1
loc_1_2D1F:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_1_2D64
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   eax
        call   sub_1_B944
        add    esp, 4
        jmp    SHORT loc_1_2D68
        db  08Dh, 049h, 000h
loc_1_2D38:
        mov    edi, dword ptr [esi + 8]
        call   sub_1_B92C
        mov    dword ptr [esp + 14h], eax
        mov    dword ptr [esi + 4], eax
        mov    byte ptr [esi + 19h], 1
        push   1
        push   0
        push   1
        push   edi
        push   dword ptr [esp + 34h]
        call   sub_1_1B44
        add    esp, 14h
        mov    edi, ebx
        mov    esi, dword ptr [ebx]
        jmp    SHORT loc_1_2D68
loc_1_2D64:
        mov    edi, esi
        mov    esi, dword ptr [esi]
loc_1_2D68:
        or     esi, esi
        jne    NEAR PTR loc_1_2CC5
loc_1_2D70:
        add    ebx, 4
        dec    dword ptr [esp + 18h]
        jne    NEAR PTR loc_1_2CB9
        or     ebp, ebp
        je     SHORT loc_1_2D9B
loc_1_2D81:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp + 38h]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   dword ptr [eax]
        call   sub_1_5BD8
        add    esp, 0ch
        or     ebp, ebp
        jne    SHORT loc_1_2D81
loc_1_2D9B:
        push   dword ptr [esp + 24h]
        db  068h, 080h, 02Ch, 000h, 000h
        push   3e8h
        mov    eax, dword ptr [esp + 30h]
        add    eax, 70h
        push   eax
        call   sub_1_B920
        add    esp, 10h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  08Dh, 049h, 000h
sub_1_2DC4:
        mov    edx, dword ptr [esp + 4]
        add    edx, 20h
        mov    eax, 4
        mov    ecx, dword ptr [esp + 8]
loc_1_2DD4:
        cmp    dword ptr [edx + 8], ecx
        je     SHORT loc_1_2DE0
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_2DD4
        ret
loc_1_2DE0:
        mov    eax, dword ptr [edx + 4]
        ret
sub_1_2DE4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_2DC4
        add    esp, 8
        cmp    eax, 4
        je     SHORT loc_1_2E04
        mov    al, 1
        pop    esi
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_2E04:
        mov    edx, dword ptr [esi + 30h]
        or     edx, edx
        je     SHORT loc_1_2E5F
        nop
loc_1_2E0C:
        mov    esi, dword ptr [edx + 8]
        mov    eax, esi
        and    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [edx + 4], eax
        jne    SHORT loc_1_2E59
        cmp    esi, -1
        je     SHORT loc_1_2E64
        cmp    eax, dword ptr [ebp + 0ch]
        je     SHORT loc_1_2E59
        mov    ecx, esi
        not    ecx
        or     ecx, eax
        cmp    ecx, dword ptr [ebp + 0ch]
        je     SHORT loc_1_2E59
        mov    al, byte ptr [ebp + 0ch]
        shr    al, 2
        and    eax, 0ffh
        and    eax, 0fffffffch
        db  08Bh, 080h, 000h, 0BDh, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_1_2E6C
        mov    ecx, eax
        and    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, dword ptr [ebp + 0ch]
        je     SHORT loc_1_2E59
        not    eax
        or     eax, ecx
        cmp    eax, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_2E6C
loc_1_2E59:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    SHORT loc_1_2E0C
loc_1_2E5F:
        sub    al, al
        pop    esi
        leave
        ret
loc_1_2E64:
        pop    esi
        mov    al, 1
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_2E6C:
        pop    esi
        mov    al, 1
        leave
        ret
        db  08Dh, 049h, 000h
loc_1_2E74:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        sub    edi, edi
        inc    esi
        mov    esi, dword ptr [esp + 38h]
loc_1_2E84:
        mov    ecx, dword ptr [esi + 18h]
        and    ecx, 0ffh
        cmp    ecx, 63h
        jbe    SHORT loc_1_2EB0
        mov    ebp, 64h
        mov    eax, ecx
        sub    edx, edx
        div    ebp
        add    al, 30h
        mov    byte ptr [esp + ebx + 10h], al
        inc    ebx
        mov    eax, ecx
        sub    edx, edx
        div    ebp
        mov    ecx, edx
        jmp    SHORT loc_1_2EB5
        db  08Bh, 0FFh
loc_1_2EB0:
        cmp    ecx, 9
        jbe    SHORT loc_1_2ECF
loc_1_2EB5:
        mov    eax, ecx
        mov    ebp, 0ah
        sub    edx, edx
        div    ebp
        add    al, 30h
        mov    byte ptr [esp + ebx + 10h], al
        inc    ebx
        mov    eax, ecx
        sub    edx, edx
        div    ebp
        mov    ecx, edx
loc_1_2ECF:
        inc    ebx
        add    cl, 30h
        mov    byte ptr [esp + ebx + 0fh], cl
        cmp    edi, 3
        je     SHORT loc_1_2EE2
        mov    byte ptr [esp + ebx + 10h], 2eh
        inc    ebx
loc_1_2EE2:
        inc    edi
        shr    dword ptr [esi + 18h], 8
        cmp    edi, 4
        jb     SHORT loc_1_2E84
        mov    byte ptr [esp + ebx + 10h], 0
        sub    ecx, ecx
        cmp    dword ptr [esi + 1ch], ecx
        jbe    SHORT loc_1_2F45
        lea    eax, [esp + 20h]
loc_1_2EFC:
        mov    dl, byte ptr [esi + ecx + 20h]
        shr    dl, 4
        cmp    dl, 0ah
        jb     SHORT loc_1_2F10
        add    dl, 37h
        jmp    SHORT loc_1_2F13
        db  08Dh, 049h, 000h
loc_1_2F10:
        add    dl, 30h
loc_1_2F13:
        mov    byte ptr [eax], dl
        mov    dl, byte ptr [esi + ecx + 20h]
        and    dl, 0fh
        cmp    dl, 0ah
        jb     SHORT loc_1_2F28
        add    dl, 37h
        jmp    SHORT loc_1_2F2B
        db  08Bh, 0FFh
loc_1_2F28:
        add    dl, 30h
loc_1_2F2B:
        mov    byte ptr [eax + 1], dl
        mov    edx, dword ptr [esi + 1ch]
        sub    edx, ecx
        cmp    edx, 1
        je     SHORT loc_1_2F3C
        mov    byte ptr [eax + 2], 3ah
loc_1_2F3C:
        inc    ecx
        add    eax, 3
        cmp    dword ptr [esi + 1ch], ecx
        ja     SHORT loc_1_2EFC
loc_1_2F45:
        mov    eax, dword ptr [esi + 1ch]
        lea    eax, [eax + eax*2]
        mov    byte ptr [esp + eax + 1fh], 0
        push   dword ptr [esi + 14h]
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_0247
        add    esp, 0ch
        push   esi
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  08Bh, 0FFh
sub_1_2F78:
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 0
        call   sub_1_B92C
        mov    dword ptr [esp + 24h], eax
        mov    ebx, dword ptr [esp + 50h]
        mov    ecx, dword ptr [ebx + 8]
        cmp    ecx, 7
        je     NEAR PTR loc_1_3048
        cmp    dword ptr [esp + 60h], 1ch
        jae    SHORT loc_1_2FBC
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
loc_1_2FBC:
        mov    ebp, dword ptr [esp + 5ch]
        mov    ax, word ptr [ebp]
        cmp    ax, 100h
        je     SHORT loc_1_2FE0
        cmp    ax, 600h
        je     SHORT loc_1_2FE0
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  08Dh, 049h, 000h
loc_1_2FE0:
        cmp    byte ptr [ebp + 4], 6
        je     SHORT loc_1_2FF4
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  090h
loc_1_2FF4:
        or     ecx, ecx
        jne    SHORT loc_1_3008
        cmp    byte ptr [ebx + 61h], 0
        jne    SHORT loc_1_3008
        mov    dword ptr [esp + 2ch], 0
        jmp    SHORT loc_1_3014
loc_1_3008:
        cmp    dword ptr [esp + 64h], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 2ch], eax
loc_1_3014:
        cmp    ecx, 1
        jne    SHORT loc_1_3034
        mov    eax, dword ptr [esp + 58h]
        sub    eax, 0eh
        cmp    eax, 2
        jbe    SHORT loc_1_3034
        mov    dword ptr [esp + 20h], eax
        mov    ecx, dword ptr [esp + 54h]
        add    ecx, 0eh
        mov    dword ptr [esp + 1ch], ecx
loc_1_3034:
        lea    ecx, [ebp + 8]
        mov    dword ptr [esp + 14h], ecx
        lea    ecx, [ebp + 0eh]
        mov    dword ptr [esp + 18h], ecx
        lea    eax, [ebp + 18h]
        jmp    SHORT loc_1_30A5
        db  090h
loc_1_3048:
        cmp    dword ptr [esp + 60h], 12h
        jae    SHORT loc_1_305C
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
loc_1_305C:
        mov    ebp, dword ptr [esp + 5ch]
        cmp    word ptr [ebp], 700h
        je     SHORT loc_1_3078
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  08Dh, 049h, 000h
loc_1_3078:
        cmp    byte ptr [ebp + 4], 1
        je     SHORT loc_1_308C
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  090h
loc_1_308C:
        lea    ecx, [ebp + 8]
        mov    dword ptr [esp + 2ch], 0
        mov    dword ptr [esp + 14h], ecx
        inc    ecx
        mov    dword ptr [esp + 18h], ecx
        mov    eax, ecx
        add    eax, 5
loc_1_30A5:
        mov    dword ptr [esp + 28h], eax
        cmp    word ptr [ebp + 2], 8
        je     SHORT loc_1_30C0
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  08Dh, 049h, 000h
loc_1_30C0:
        cmp    byte ptr [ebp + 5], 4
        je     SHORT loc_1_30D4
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  090h
loc_1_30D4:
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ecx]
        or     eax, eax
        jne    SHORT loc_1_30EC
        mov    eax, 10001h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  090h
loc_1_30EC:
        push   eax
        push   ebx
        call   sub_1_2DC4
        add    esp, 8
        cmp    eax, 4
        je     NEAR PTR loc_1_320C
        mov    cl, byte ptr [ebx + 5ch]
        cmp    byte ptr [ebp + 4], cl
        jne    SHORT loc_1_3127
        lea    edi, [ebx + 54h]
        movzx  edx, cl
        mov    ecx, edx
        shr    ecx, 2
        mov    esi, dword ptr [esp + 14h]
        repe cmpsd
        jne    SHORT loc_1_3121
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
loc_1_3121:
        je     NEAR PTR loc_1_31FC
loc_1_3127:
        or     eax, eax
        je     SHORT loc_1_315C
        cmp    word ptr [ebp + 6], 200h
        jne    SHORT loc_1_315C
        mov    byte ptr [ebx + 5bh], 2
        mov    byte ptr [ebx + 96h], 2
        push   0
        push   dword ptr [esp + 18h]
        push   1
        mov    ecx, dword ptr [esp + 24h]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_1B44
        add    esp, 14h
        mov    esi, 1
        jmp    SHORT loc_1_319F
loc_1_315C:
        cmp    word ptr [ebp + 6], 100h
        jne    SHORT loc_1_319D
        mov    eax, dword ptr [esp + 28h]
        push   dword ptr [eax]
        push   ebx
        call   sub_1_2DC4
        add    esp, 8
        cmp    eax, 4
        je     SHORT loc_1_319D
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 38h]
        push   dword ptr [eax]
        mov    ecx, dword ptr [esp + 2ch]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_1DEC
        add    esp, 1ch
loc_1_319D:
        sub    esi, esi
loc_1_319F:
        movzx  eax, byte ptr [ebp + 4]
        add    eax, 20h
        push   eax
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_31FC
        mov    ecx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax + 18h], ecx
        mov    dword ptr [eax + 14h], esi
        movzx  edx, byte ptr [ebp + 4]
        mov    dword ptr [eax + 1ch], edx
        lea    edi, [eax + 20h]
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    dword ptr [eax + 0ch], 0
        db  0C7h, 040h, 004h, 031h, 002h, 000h, 000h
        db  08Bh, 00Dh, 06Ch, 0BFh, 000h, 000h
        mov    dword ptr [eax + 10h], ecx
        push   eax
        push   eax
        call   sub_1_B932
        add    esp, 8
loc_1_31FC:
        pop    ebp
        mov    eax, 10001h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  08Dh, 049h, 000h
loc_1_320C:
        mov    cx, word ptr [ebx + 94h]
        mov    word ptr [esp + 12h], cx
        mov    eax, dword ptr [esp + 28h]
        push   dword ptr [eax]
        push   ebx
        call   sub_1_2DE4
        add    esp, 8
        mov    byte ptr [esp + 10h], al
        lea    eax, [esp + 34h]
        push   eax
        mov    ecx, dword ptr [esp + 1ch]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_2038
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_3270
        cmp    byte ptr [esp + 10h], 0
        je     SHORT loc_1_3260
        lea    eax, [esp + 34h]
        push   eax
        mov    ecx, dword ptr [esp + 1ch]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_2540
        add    esp, 0ch
        jmp    SHORT loc_1_3278
loc_1_3260:
        pop    ebp
        mov    eax, 10001h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  08Dh, 049h, 000h
loc_1_3270:
        mov    ecx, dword ptr [esp + 40h]
        mov    dword ptr [esp + 34h], ecx
loc_1_3278:
        or     eax, eax
        je     NEAR PTR loc_1_3465
        mov    edx, dword ptr [eax + 4]
        cmp    edx, -1
        jne    SHORT loc_1_3290
        mov    dword ptr [esp + 24h], 0ffffffffh
loc_1_3290:
        mov    ecx, dword ptr [ebx + 8]
        or     ecx, ecx
        je     SHORT loc_1_32B4
        cmp    ecx, 1
        je     SHORT loc_1_32FC
        cmp    ecx, 2
        je     NEAR PTR loc_1_33E0
        cmp    ecx, 7
        je     NEAR PTR loc_1_3410
        jmp    NEAR PTR loc_1_3434
        db  090h
loc_1_32B4:
        lea    ecx, [eax + 1ah]
        mov    edi, ecx
        mov    esi, dword ptr [esp + 14h]
        movsd
        movsw
        lea    esi, [ebx + 54h]
        lea    edi, [ecx + 6]
        movsd
        movsw
        cmp    dword ptr [esp + 2ch], 0
        mov    word ptr [ecx + 0ch], 8
        je     SHORT loc_1_32F0
        lea    edi, [eax + 28h]
        db  0BEh, 054h, 0BCh, 000h, 000h
        movsd
        movsd
        mov    byte ptr [eax + 18h], 16h
        mov    word ptr [eax + 2eh], 8
        jmp    NEAR PTR loc_1_3429
        db  090h
loc_1_32F0:
        mov    byte ptr [eax + 18h], 0eh
        jmp    NEAR PTR loc_1_3429
        db  08Dh, 049h, 000h
loc_1_32FC:
        cmp    byte ptr [eax + 19h], 2
        jne    SHORT loc_1_3314
        mov    ecx, dword ptr [esp + 24h]
        sub    ecx, edx
        cmp    ecx, 3e8h
        jbe    NEAR PTR loc_1_3434
loc_1_3314:
        lea    edx, [eax + 1ah]
        mov    byte ptr [edx], 10h
        mov    byte ptr [edx + 1], 40h
        lea    edi, [edx + 2]
        mov    esi, dword ptr [esp + 14h]
        movsd
        movsw
        lea    esi, [ebx + 54h]
        lea    edi, [edx + 8]
        movsd
        movsw
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_33AB
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [esp + 3ch], ecx
        lea    edi, [eax + 28h]
        mov    esi, dword ptr [esp + 1ch]
        shr    ecx, 2
        rep movsd
        mov    ecx, dword ptr [esp + 3ch]
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 1ch]
        mov    cl, byte ptr [ecx + 1]
        and    cl, 70h
        shr    cl, 4
        mov    byte ptr [esp + 30h], cl
        sub    byte ptr [esp + 30h], 4
        mov    cl, byte ptr [esp + 38h]
        sbb    byte ptr [esp + 38h], cl
        mov    cl, byte ptr [esp + 30h]
        and    cl, byte ptr [esp + 38h]
        mov    byte ptr [esp + 30h], cl
        add    cl, 4
        mov    byte ptr [esp + 11h], cl
        and    ecx, 0ffh
        db  066h, 08Bh, 00Ch, 04Dh, 088h, 0BCh, 000h, 000h
        or     byte ptr [edx + 8], 80h
        mov    word ptr [esp + 12h], cx
        xor    byte ptr [eax + 29h], 80h
        sub    word ptr [esp + 12h], 8
        and    byte ptr [eax + 28h], 1fh
loc_1_33AB:
        db  0BEh, 054h, 0BCh, 000h, 000h
        mov    ecx, dword ptr [esp + 20h]
        lea    edi, [ecx + eax + 28h]
        movsd
        movsd
        mov    byte ptr [eax + 19h], 2
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 20h]
        add    cl, 16h
        mov    byte ptr [eax + 18h], cl
        and    ecx, 0ffh
        mov    word ptr [ecx + eax + 18h], 8
        jmp    SHORT loc_1_3434
        db  08Bh, 0FFh
loc_1_33E0:
        lea    ecx, [eax + 1ah]
        mov    byte ptr [ecx], 57h
        lea    edi, [ecx + 1]
        mov    esi, dword ptr [esp + 14h]
        movsd
        movsw
        lea    esi, [ebx + 54h]
        lea    edi, [ecx + 7]
        movsd
        movsw
        db  0BEh, 054h, 0BCh, 000h, 000h
        lea    edi, [eax + 27h]
        movsd
        movsd
        mov    byte ptr [eax + 18h], 15h
        mov    word ptr [eax + 2dh], 8
        jmp    SHORT loc_1_3429
        db  090h
loc_1_3410:
        lea    edx, [eax + 1ah]
        mov    cl, byte ptr [ebx + 54h]
        mov    byte ptr [edx], cl
        mov    ecx, dword ptr [esp + 14h]
        mov    cl, byte ptr [ecx]
        mov    byte ptr [edx + 1], cl
        mov    byte ptr [edx + 2], 0d4h
        mov    byte ptr [eax + 18h], 3
loc_1_3429:
        mov    byte ptr [eax + 19h], 2
        mov    ecx, dword ptr [esp + 24h]
        mov    dword ptr [eax + 4], ecx
loc_1_3434:
        mov    esi, dword ptr [eax + 0ch]
        or     esi, esi
        je     SHORT loc_1_3465
        mov    dword ptr [eax + 0ch], 0
        push   dword ptr [esp + 34h]
        push   eax
        push   esi
        push   ebx
        call   sub_1_2394
        add    esp, 10h
        cmp    eax, 103h
        je     SHORT loc_1_3465
        push   0
        push   esi
        push   dword ptr [ebx]
        call   sub_1_5BD8
        add    esp, 0ch
loc_1_3465:
        mov    cx, word ptr [esp + 12h]
        cmp    word ptr [ebx + 94h], cx
        jbe    SHORT loc_1_3497
        movzx  eax, cx
        mov    dword ptr [esp + 44h], eax
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esp + 48h], eax
        push   8
        lea    eax, [esp + 48h]
        push   eax
        push   3
        push   dword ptr [ebx]
        call   sub_1_6DF4
        add    esp, 10h
loc_1_3497:
        cmp    byte ptr [esp + 10h], 0
        je     SHORT loc_1_34CB
        cmp    word ptr [ebp + 6], 100h
        jne    SHORT loc_1_34CB
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 20h]
        mov    eax, dword ptr [esp + 38h]
        push   dword ptr [eax]
        mov    ecx, dword ptr [esp + 2ch]
        push   dword ptr [ecx]
        push   ebx
        call   sub_1_1DEC
        add    esp, 1ch
loc_1_34CB:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 3ch
        ret
        db  08Dh, 049h, 000h
sub_1_34D8:
        push   ebx
        push   esi
        push   edi
        push   0
        push   4
        db  068h, 008h, 0BCh, 000h, 000h
        push   1010fh
        push   1
        mov    edi, dword ptr [esp + 24h]
        push   edi
        call   sub_1_1728
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_1_3504
        mov    byte ptr [edi + 5bh], 2
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3504:
        push   0
        push   4
        lea    eax, [edi + 64h]
        push   eax
        push   1010eh
        push   1
        push   edi
        call   sub_1_1728
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_1_3578
        mov    byte ptr [edi + 96h], 1
        mov    byte ptr [edi + 97h], 1
        mov    byte ptr [edi + 5bh], 0
        sub    ebx, ebx
        lea    esi, [edi + 20h]
        mov    ecx, esi
loc_1_3539:
        mov    eax, dword ptr [ecx + 8]
        or     eax, eax
        je     SHORT loc_1_3559
        mov    dword ptr [ecx + 4], 3
        mov    ebx, ecx
        push   1
        push   0
        push   1
        push   eax
        push   edi
        call   sub_1_1B44
        add    esp, 14h
loc_1_3559:
        mov    ecx, esi
        cmp    ebx, esi
        je     SHORT loc_1_356A
        nop
loc_1_3560:
        or     ecx, ecx
        je     SHORT loc_1_3570
        mov    ecx, dword ptr [ecx]
        cmp    ecx, ebx
        jne    SHORT loc_1_3560
loc_1_356A:
        or     ecx, ecx
        je     SHORT loc_1_3574
        mov    ecx, dword ptr [ecx]
loc_1_3570:
        or     ecx, ecx
        jne    SHORT loc_1_3539
loc_1_3574:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3578:
        mov    byte ptr [edi + 5bh], 2
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_3580:
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        add    eax, 88h
        push   eax
        call   sub_1_B95C
        add    esp, 8
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_359C:
        push   dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        add    eax, 88h
        push   eax
        call   sub_1_B95C
        add    esp, 8
        ret    8
        db  08Dh, 049h, 000h
loc_1_35B8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        lea    ebp, [edi + 38h]
        mov    esi, dword ptr [esp + 18h]
        dec    dword ptr [esi + 50h]
        mov    ebx, dword ptr [esp + 20h]
        or     ebx, ebx
        jne    SHORT loc_1_35E0
        mov    eax, dword ptr [edi + 4]
        add    dword ptr [esi + 44h], eax
        jmp    SHORT loc_1_35F6
        db  08Dh, 049h, 000h
loc_1_35E0:
        cmp    ebx, 0c000009ah
        jne    SHORT loc_1_35F0
        inc    dword ptr [esi + 0ach]
        jmp    SHORT loc_1_35F6
loc_1_35F0:
        inc    dword ptr [esi + 0b0h]
loc_1_35F6:
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        call   sub_1_BA5C
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_1_17BC
        add    esp, 8
        cmp    byte ptr [ebp + 4], 0
        je     SHORT loc_1_362C
        push   ebx
        push   edi
        push   dword ptr [esi]
        call   sub_1_5BD8
        add    esp, 0ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
        db  08Bh, 0FFh
loc_1_362C:
        push   edi
        call   sub_1_BA1A
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
loc_1_363C:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax]
        call   sub_1_5840
        add    esp, 10h
        ret    10h
        db  08Dh, 049h, 000h
loc_1_365C:
        ret    8
        db  090h
loc_1_3660:
        push   dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 8]
        add    eax, 88h
        push   eax
        call   sub_1_B95C
        add    esp, 8
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_367C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 5bh], 0
        jne    NEAR PTR loc_1_37B8
        movzx  ecx, byte ptr [eax + 60h]
        cmp    ecx, dword ptr [esp + 20h]
        ja     NEAR PTR loc_1_37B8
        mov    esi, dword ptr [esp + 2ch]
        add    dword ptr [eax + 38h], esi
        mov    ecx, dword ptr [eax + 8]
        cmp    ecx, 7
        je     SHORT loc_1_3708
        or     ecx, ecx
        mov    ebp, dword ptr [esp + 1ch]
        jne    SHORT loc_1_36C8
        mov    cx, word ptr [ebp + 0ch]
        mov    bh, cl
        mov    bl, ch
        cmp    bx, 600h
        jb     SHORT loc_1_36C8
        sub    edi, edi
        jmp    SHORT loc_1_372B
        db  08Bh, 0FFh
loc_1_36C8:
        cmp    dword ptr [esp + 28h], 8
        jb     SHORT loc_1_36F4
        mov    ecx, dword ptr [esp + 24h]
        cmp    byte ptr [ecx], 0aah
        jne    SHORT loc_1_36F4
        cmp    byte ptr [ecx + 1], 0aah
        jne    SHORT loc_1_36F4
        cmp    byte ptr [ecx + 2], 3
        jne    SHORT loc_1_36F4
        mov    dx, word ptr [ecx + 6]
        mov    bh, dl
        mov    bl, dh
        mov    edi, 8
        jmp    SHORT loc_1_372F
        db  090h
loc_1_36F4:
        pop    ebp
        inc    dword ptr [eax + 0a8h]
        mov    eax, 10001h
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
        db  08Bh, 0FFh
loc_1_3708:
        sub    edi, edi
        mov    ebp, dword ptr [esp + 1ch]
        mov    cl, byte ptr [ebp + 2]
        cmp    cl, 0d4h
        jne    SHORT loc_1_371C
        mov    bx, 800h
        jmp    SHORT loc_1_372B
loc_1_371C:
        cmp    cl, 0d5h
        jne    SHORT loc_1_3728
        mov    bx, 806h
        jmp    SHORT loc_1_372B
        db  090h
loc_1_3728:
        sub    bx, bx
loc_1_372B:
        mov    ecx, dword ptr [esp + 24h]
loc_1_372F:
        movzx  edx, byte ptr [eax + 5fh]
        mov    dl, byte ptr [edx + ebp]
        and    dl, byte ptr [eax + 5dh]
        cmp    dl, byte ptr [eax + 5eh]
        mov    edx, 0
        sete   dl
        cmp    bx, 800h
        jne    SHORT loc_1_3778
        inc    dword ptr [eax + edx*4 + 3ch]
        push   edx
        push   edi
        push   dword ptr [esp + 20h]
        sub    esi, edi
        push   esi
        mov    edx, dword ptr [esp + 38h]
        sub    edx, edi
        push   edx
        add    ecx, edi
        push   ecx
        push   dword ptr [eax]
        call   sub_1_5514
        add    esp, 1ch
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
        db  08Dh, 049h, 000h
loc_1_3778:
        cmp    bx, 806h
        jne    SHORT loc_1_37A4
        inc    dword ptr [eax + edx*4 + 3ch]
        push   edi
        mov    edx, dword ptr [esp + 2ch]
        sub    edx, edi
        push   edx
        add    ecx, edi
        push   ecx
        push   dword ptr [esp + 2ch]
        push   ebp
        push   eax
        call   sub_1_2F78
        add    esp, 18h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
        db  090h
loc_1_37A4:
        pop    ebp
        inc    dword ptr [eax + 0a8h]
        mov    eax, 10001h
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
        db  08Bh, 0FFh
loc_1_37B8:
        pop    ebp
        mov    eax, 10001h
        pop    edi
        pop    esi
        pop    ebx
        ret    1ch
loc_1_37C4:
        call   sub_1_4BCC
        ret    4
loc_1_37CC:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax]
        call   sub_1_6DF4
        add    esp, 10h
        ret    10h
        db  08Dh, 049h, 000h
loc_1_37EC:
        ret    4
        db  090h
sub_1_37F0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ecx + 1ch]
        mov    dword ptr [esp + 10h], eax
        mov    edx, dword ptr [esp + 18h]
        mov    ebp, dword ptr [edx + 4]
        mov    eax, dword ptr [ecx + 0b8h]
        mov    ebx, dword ptr [esp + 20h]
        mov    dword ptr [ebx], eax
        movzx  eax, byte ptr [ecx + 5ch]
        mov    dword ptr [ebx + 4], eax
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        je     SHORT loc_1_3838
        cmp    ecx, 1
        je     SHORT loc_1_383C
        cmp    ecx, 2
        je     SHORT loc_1_3844
        cmp    ecx, 7
        je     SHORT loc_1_3844
        sub    ecx, ecx
        jmp    SHORT loc_1_3849
        db  08Bh, 0FFh
loc_1_3838:
        sub    ecx, ecx
        jmp    SHORT loc_1_3849
loc_1_383C:
        mov    ecx, 2
        jmp    SHORT loc_1_3849
        db  090h
loc_1_3844:
        mov    ecx, 1
loc_1_3849:
        lea    edi, [ebx + 8]
        lea    esi, [ebp + ecx + 1ah]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 10h], eax
        cmp    byte ptr [ebp + 19h], 2
        jne    SHORT loc_1_3880
        mov    eax, 0
        cmp    dword ptr [ebp + 4], -1
        sete   al
        add    eax, 3
        mov    dword ptr [ebx + 14h], eax
        jmp    SHORT loc_1_3887
        db  08Bh, 0FFh
loc_1_3880:
        mov    dword ptr [ebx + 14h], 2
loc_1_3887:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     SHORT loc_1_38A0
        mov    dword ptr [edx + 4], eax
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Bh, 0FFh
loc_1_38A0:
        mov    ecx, dword ptr [edx]
        inc    ecx
        cmp    ecx, 20h
        jae    SHORT loc_1_38BE
        mov    eax, dword ptr [esp + 10h]
        lea    eax, [eax + ecx*4]
loc_1_38B0:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_38D8
        add    eax, 4
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_1_38B0
loc_1_38BE:
        sub    eax, eax
        mov    dword ptr [edx], 0
        mov    dword ptr [edx + 4], 0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_38D8:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + ecx*4]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx], ecx
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
sub_1_38F4:
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [eax + 1ch]
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        or     esi, esi
        jne    SHORT loc_1_3940
        or     eax, eax
        jne    SHORT loc_1_3940
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 1
        lea    eax, [ecx + esi*4]
loc_1_391A:
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        jne    SHORT loc_1_392D
        add    eax, 4
        inc    esi
        cmp    esi, 20h
        jb     SHORT loc_1_391A
        or     ecx, ecx
        je     SHORT loc_1_393C
loc_1_392D:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], ecx
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_393C:
        sub    eax, eax
        pop    esi
        ret
loc_1_3940:
        cmp    esi, 20h
        jae    SHORT loc_1_3956
        mov    ecx, dword ptr [ecx + esi*4]
        or     ecx, ecx
        je     SHORT loc_1_3956
loc_1_394C:
        cmp    ecx, eax
        je     SHORT loc_1_3964
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_394C
loc_1_3956:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
        sub    eax, eax
        pop    esi
        ret
loc_1_3964:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 1
        mov    eax, 1
        pop    esi
        ret
        db  08Dh, 049h, 000h
loc_1_3978:
        sub    esp, 68h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        mov    ecx, dword ptr [esp + 88h]
        mov    ebp, dword ptr [ecx]
        sub    edi, edi
        mov    esi, dword ptr [esp + 80h]
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        cmp    edx, 280h
        mov    ebx, dword ptr [esp + 7ch]
        jne    SHORT loc_1_39B2
        cmp    dword ptr [ebx + 0bch], eax
        je     SHORT loc_1_39CA
loc_1_39B2:
        cmp    edx, 200h
        jne    NEAR PTR loc_1_3E48
        cmp    dword ptr [ebx + 0c0h], eax
        jne    NEAR PTR loc_1_3E48
loc_1_39CA:
        mov    ecx, dword ptr [esp + 88h]
        mov    dword ptr [ecx], 0
        cmp    dword ptr [esi + 0ch], 100h
        je     SHORT loc_1_39F0
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_39F0:
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 100h
        jne    SHORT loc_1_3A64
        cmp    dword ptr [esi + 10h], 1
        jne    SHORT loc_1_3A54
        cmp    ebp, 4
        jb     SHORT loc_1_3A44
        sub    edx, 280h
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 7eh
        add    eax, 202h
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 10h]
        push   eax
        push   4
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 90h]
        call   sub_1_AAE0
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3A44:
        pop    ebp
        mov    eax, 21h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3A54:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3A64:
        cmp    edx, 280h
        jne    NEAR PTR loc_1_3C88
        cmp    eax, 300h
        mov    eax, dword ptr [esi + 10h]
        jne    NEAR PTR loc_1_3B78
        cmp    eax, 1
        jne    SHORT loc_1_3ABC
        cmp    ebp, 4
        jb     SHORT loc_1_3AAC
        lea    eax, [esp + 10h]
        push   eax
        push   4
        add    ebx, 6ch
        push   ebx
        push   dword ptr [esp + 90h]
        call   sub_1_AAE0
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
loc_1_3AAC:
        pop    ebp
        mov    eax, 21h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3ABC:
        cmp    eax, 101h
        je     SHORT loc_1_3AD0
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
loc_1_3AD0:
        mov    eax, dword ptr [esp + 8ch]
        mov    esi, dword ptr [eax]
        or     esi, esi
        je     SHORT loc_1_3B00
        mov    eax, dword ptr [ebx + 30h]
        or     eax, eax
        je     SHORT loc_1_3AF2
loc_1_3AE4:
        cmp    eax, esi
        je     SHORT loc_1_3AEE
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_3AE4
loc_1_3AEE:
        or     eax, eax
        jne    SHORT loc_1_3B03
loc_1_3AF2:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  090h
loc_1_3B00:
        mov    esi, dword ptr [ebx + 30h]
loc_1_3B03:
        or     esi, esi
        je     SHORT loc_1_3B52
        nop
loc_1_3B08:
        mov    eax, ebp
        sub    eax, edi
        cmp    eax, 0ch
        jl     SHORT loc_1_3B52
        mov    dword ptr [esp + 18h], 1
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 20h], eax
        add    edi, 0ch
        lea    eax, [esp + 10h]
        push   eax
        push   0ch
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 90h]
        call   sub_1_AAE0
        add    esp, 10h
        mov    dword ptr [esp + 84h], eax
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_3B08
loc_1_3B52:
        cmp    esi, 1
        mov    ecx, dword ptr [esp + 88h]
        mov    dword ptr [ecx], edi
        mov    eax, dword ptr [esp + 8ch]
        mov    dword ptr [eax], esi
        cmc
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3B78:
        cmp    eax, 1
        jne    SHORT loc_1_3BD8
        cmp    ebp, 8
        jb     SHORT loc_1_3BC8
        mov    ecx, dword ptr [esp + 88h]
        mov    dword ptr [ecx], 8
        mov    eax, dword ptr [ebx + 68h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [ebx + 0b8h]
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 10h]
        push   eax
        push   8
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 90h]
        call   sub_1_AAE0
        add    esp, 10h
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3BC8:
        pop    ebp
        mov    eax, 21h
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3BD8:
        cmp    eax, 101h
        jne    NEAR PTR loc_1_3C78
        lea    eax, [esp + 14h]
        push   eax
        push   ebx
        push   dword ptr [esp + 94h]
        call   sub_1_38F4
        add    esp, 0ch
        mov    esi, eax
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_3C10
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Bh, 0FFh
loc_1_3C10:
        or     esi, esi
        je     SHORT loc_1_3C5D
loc_1_3C14:
        mov    eax, ebp
        sub    eax, edi
        cmp    eax, 18h
        jl     SHORT loc_1_3C5D
        lea    eax, [esp + 18h]
        push   eax
        push   ebx
        push   dword ptr [esp + 94h]
        call   sub_1_37F0
        add    esp, 0ch
        mov    esi, eax
        add    edi, 18h
        lea    eax, [esp + 10h]
        push   eax
        push   18h
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 90h]
        call   sub_1_AAE0
        add    esp, 10h
        mov    dword ptr [esp + 84h], eax
        or     esi, esi
        jne    SHORT loc_1_3C14
loc_1_3C5D:
        cmp    esi, 1
        mov    ecx, dword ptr [esp + 88h]
        mov    dword ptr [ecx], edi
        cmc
        sbb    eax, eax
        and    eax, 9
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  090h
loc_1_3C78:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3C88:
        cmp    eax, 200h
        je     SHORT loc_1_3C9C
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
loc_1_3C9C:
        cmp    dword ptr [esi + 10h], 1
        jne    NEAR PTR loc_1_3E38
        cmp    ebp, 5ch
        jae    SHORT loc_1_3CB8
        mov    eax, 21h
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
loc_1_3CB8:
        mov    eax, dword ptr [ebx + 0b8h]
        mov    dword ptr [esp + 18h], eax
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     SHORT loc_1_3CE0
        cmp    eax, 1
        je     SHORT loc_1_3CEC
        cmp    eax, 2
        je     SHORT loc_1_3CF8
        mov    dword ptr [esp + 1ch], 1
        jmp    SHORT loc_1_3D00
        db  08Dh, 049h, 000h
loc_1_3CE0:
        mov    dword ptr [esp + 1ch], 6
        jmp    SHORT loc_1_3D00
        db  08Bh, 0FFh
loc_1_3CEC:
        mov    dword ptr [esp + 1ch], 9
        jmp    SHORT loc_1_3D00
        db  08Bh, 0FFh
loc_1_3CF8:
        mov    dword ptr [esp + 1ch], 0fh
loc_1_3D00:
        movzx  eax, word ptr [ebx + 94h]
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebx + 98h]
        mov    dword ptr [esp + 24h], eax
        movzx  eax, byte ptr [ebx + 5ch]
        mov    dword ptr [esp + 28h], eax
        lea    esi, [ebx + 54h]
        lea    edi, [esp + 2ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [ebx + 96h]
        mov    dword ptr [esp + 34h], eax
        movzx  eax, byte ptr [ebx + 97h]
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [ebx + 9ch]
        mov    dword ptr [esp + 3ch], eax
        mov    eax, dword ptr [ebx + 38h]
        mov    dword ptr [esp + 40h], eax
        mov    eax, dword ptr [ebx + 3ch]
        mov    dword ptr [esp + 44h], eax
        mov    eax, dword ptr [ebx + 40h]
        mov    dword ptr [esp + 48h], eax
        mov    eax, dword ptr [ebx + 0a0h]
        mov    dword ptr [esp + 4ch], eax
        mov    eax, dword ptr [ebx + 0a4h]
        mov    dword ptr [esp + 50h], eax
        mov    eax, dword ptr [ebx + 0a8h]
        mov    dword ptr [esp + 54h], eax
        mov    eax, dword ptr [ebx + 44h]
        mov    dword ptr [esp + 58h], eax
        mov    eax, dword ptr [ebx + 48h]
        mov    dword ptr [esp + 5ch], eax
        mov    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [esp + 60h], eax
        mov    eax, dword ptr [ebx + 0ach]
        mov    dword ptr [esp + 64h], eax
        mov    eax, dword ptr [ebx + 0b0h]
        mov    dword ptr [esp + 68h], eax
        mov    eax, dword ptr [ebx + 50h]
        mov    dword ptr [esp + 6ch], eax
        mov    eax, dword ptr [ebx + 0b4h]
        mov    dword ptr [esp + 70h], eax
        lea    eax, [esp + 10h]
        push   eax
        push   5ch
        lea    eax, [esp + 20h]
        push   eax
        push   dword ptr [esp + 90h]
        call   sub_1_AAE0
        add    esp, 10h
        mov    edx, dword ptr [ebx + 0b4h]
        lea    ecx, [edx + 5ch]
        cmp    ecx, ebp
        ja     SHORT loc_1_3E1C
        or     edx, edx
        je     SHORT loc_1_3E00
        lea    ecx, [esp + 10h]
        push   ecx
        push   edx
        push   dword ptr [ebx + 0c4h]
        push   eax
        call   sub_1_AAE0
        add    esp, 10h
loc_1_3E00:
        mov    eax, dword ptr [ebx + 0b4h]
        add    eax, 5ch
        mov    ecx, dword ptr [esp + 88h]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
loc_1_3E1C:
        mov    eax, 9
        mov    ecx, dword ptr [esp + 88h]
        mov    dword ptr [ecx], 5ch
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Bh, 0FFh
loc_1_3E38:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3E48:
        pop    ebp
        mov    eax, 0feh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 68h
        ret
        db  08Dh, 049h, 000h
loc_1_3E58:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 20h]
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [edx + 4]
        cmp    eax, 280h
        mov    ebx, dword ptr [esp + 1ch]
        jne    SHORT loc_1_3E7B
        cmp    dword ptr [ebx + 0bch], ecx
        je     SHORT loc_1_3E92
loc_1_3E7B:
        cmp    eax, 200h
        jne    NEAR PTR loc_1_4274
        cmp    dword ptr [ebx + 0c0h], ecx
        jne    NEAR PTR loc_1_4274
loc_1_3E92:
        cmp    dword ptr [edx + 0ch], 100h
        je     SHORT loc_1_3EA8
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_3EA8:
        cmp    eax, 200h
        jne    NEAR PTR loc_1_3F60
        cmp    dword ptr [edx + 8], 200h
        je     SHORT loc_1_3ECC
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_3ECC:
        cmp    dword ptr [edx + 10h], 1
        jne    SHORT loc_1_3F50
        cmp    dword ptr [esp + 28h], 5ch
        jb     SHORT loc_1_3F40
        mov    edx, dword ptr [esp + 24h]
        mov    eax, dword ptr [edx + 1ch]
        cmp    eax, 1
        je     SHORT loc_1_3EFC
        cmp    eax, 2
        je     SHORT loc_1_3F1C
        cmp    eax, 3
        je     SHORT loc_1_3F34
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_3EFC:
        mov    byte ptr [ebx + 96h], 1
        cmp    byte ptr [ebx + 97h], 1
        jne    SHORT loc_1_3F10
        mov    byte ptr [ebx + 5bh], 0
loc_1_3F10:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_3F1C:
        sub    eax, eax
        mov    byte ptr [ebx + 96h], 2
        mov    byte ptr [ebx + 5bh], 2
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_3F34:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_3F40:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_3F50:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_3F60:
        mov    eax, dword ptr [edx + 8]
        cmp    eax, 300h
        jne    NEAR PTR loc_1_409C
        cmp    dword ptr [edx + 10h], 101h
        jne    NEAR PTR loc_1_408C
        cmp    dword ptr [esp + 28h], 0ch
        jb     NEAR PTR loc_1_408C
        mov    edx, dword ptr [esp + 24h]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esp + 10h], eax
        mov    esi, dword ptr [edx + 8]
        mov    eax, dword ptr [edx]
        cmp    eax, 1
        jne    NEAR PTR loc_1_4058
        mov    eax, esi
        and    eax, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 10h], eax
        jne    NEAR PTR loc_1_4048
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_4048
        cmp    dword ptr [esp + 10h], -1
        je     NEAR PTR loc_1_4048
        mov    cl, byte ptr [esp + 10h]
        and    cl, 0f0h
        cmp    cl, 0e0h
        je     SHORT loc_1_4048
        push   eax
        push   ebx
        call   sub_1_2060
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_3FEC
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_3FEC:
        inc    eax
        lea    eax, [ebx + 20h]
loc_1_3FF0:
        mov    ecx, dword ptr [eax + 8]
        or     ecx, ecx
        je     SHORT loc_1_4001
        mov    edx, esi
        and    edx, ecx
        cmp    edx, dword ptr [esp + 10h]
        je     SHORT loc_1_4007
loc_1_4001:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_3FF0
loc_1_4007:
        or     eax, eax
        je     SHORT loc_1_4018
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_4018:
        push   esi
        push   dword ptr [esp + 14h]
        push   4
        push   ebx
        call   sub_1_2AB4
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_4038
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_4038:
        pop    ebp
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_4048:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_4058:
        cmp    eax, 2
        jne    SHORT loc_1_407C
        push   esi
        push   dword ptr [esp + 14h]
        push   4
        push   ebx
        call   sub_1_2BBC
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_407C
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_407C:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_408C:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_409C:
        cmp    eax, 200h
        je     SHORT loc_1_40B0
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_40B0:
        cmp    dword ptr [edx + 10h], 101h
        jne    NEAR PTR loc_1_4264
        cmp    dword ptr [esp + 28h], 18h
        jb     NEAR PTR loc_1_4264
        mov    edx, dword ptr [esp + 24h]
        mov    eax, dword ptr [edx + 14h]
        cmp    eax, 2
        jne    SHORT loc_1_4140
        mov    cl, byte ptr [edx + 13h]
        and    ecx, 1fh
        mov    eax, dword ptr [ebx + 1ch]
        lea    ecx, [eax + ecx*4]
        mov    esi, dword ptr [ecx]
        or     esi, esi
        je     SHORT loc_1_4130
        mov    eax, dword ptr [edx + 10h]
loc_1_40E9:
        cmp    dword ptr [esi + 8], eax
        je     SHORT loc_1_40F6
        mov    ecx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_40E9
loc_1_40F6:
        or     esi, esi
        je     SHORT loc_1_4130
        push   esi
        push   ecx
        call   sub_1_2774
        add    esp, 8
        dec    dword ptr [ebx + 68h]
        mov    eax, dword ptr [esi + 0ch]
        or     eax, eax
        je     SHORT loc_1_411B
        push   0
        push   eax
        push   dword ptr [ebx]
        call   sub_1_5BD8
        add    esp, 0ch
loc_1_411B:
        push   esi
        call   sub_1_B944
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_4130:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_4140:
        cmp    eax, 3
        je     SHORT loc_1_4158
        cmp    eax, 4
        je     SHORT loc_1_4158
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  090h
loc_1_4158:
        movzx  eax, byte ptr [ebx + 5ch]
        mov    edx, dword ptr [esp + 24h]
        cmp    eax, dword ptr [edx + 4]
        je     SHORT loc_1_4174
        mov    eax, 1ah
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_4174:
        lea    eax, [esp + 14h]
        push   eax
        mov    edx, dword ptr [esp + 28h]
        push   dword ptr [edx + 10h]
        push   ebx
        call   sub_1_2540
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_419C
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_419C:
        lea    ecx, [ebp + 1ah]
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     SHORT loc_1_41B8
        cmp    eax, 1
        je     SHORT loc_1_41C0
        cmp    eax, 2
        je     SHORT loc_1_41DC
        cmp    eax, 7
        je     SHORT loc_1_41F0
        jmp    SHORT loc_1_4207
        db  090h
loc_1_41B8:
        mov    byte ptr [ebp + 18h], 0eh
        jmp    SHORT loc_1_4207
        db  08Bh, 0FFh
loc_1_41C0:
        mov    byte ptr [ecx], 10h
        mov    byte ptr [ecx + 1], 40h
        db  0BEh, 054h, 0BCh, 000h, 000h
        lea    edi, [ecx + 0eh]
        movsd
        movsd
        add    ecx, 2
        mov    byte ptr [ebp + 18h], 16h
        jmp    SHORT loc_1_4207
        db  08Bh, 0FFh
loc_1_41DC:
        mov    byte ptr [ecx], 57h
        db  0BEh, 054h, 0BCh, 000h, 000h
        lea    edi, [ecx + 0dh]
        movsd
        movsd
        inc    ecx
        mov    byte ptr [ebp + 18h], 15h
        jmp    SHORT loc_1_4207
loc_1_41F0:
        mov    al, byte ptr [ebx + 54h]
        mov    byte ptr [ecx], al
        mov    edx, dword ptr [esp + 24h]
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ecx + 1], al
        mov    byte ptr [ecx + 2], 0d4h
        mov    byte ptr [ebp + 18h], 3
loc_1_4207:
        cmp    dword ptr [ebx + 8], 7
        je     SHORT loc_1_422D
        mov    esi, dword ptr [esp + 24h]
        add    esi, 8
        mov    edi, ecx
        movsd
        movsw
        lea    esi, [ebx + 54h]
        lea    edi, [ecx + 6]
        movsd
        movsw
        movzx  eax, byte ptr [ebp + 18h]
        mov    word ptr [eax + ebp + 18h], 8
loc_1_422D:
        mov    edx, dword ptr [esp + 24h]
        cmp    dword ptr [edx + 14h], 4
        jne    SHORT loc_1_424C
        mov    dword ptr [ebp + 4], 0ffffffffh
        mov    byte ptr [ebp + 19h], 2
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_424C:
        call   sub_1_B92C
        mov    dword ptr [ebp + 4], eax
        mov    byte ptr [ebp + 19h], 2
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_4264:
        pop    ebp
        mov    eax, 1ah
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
loc_1_4274:
        pop    ebp
        mov    eax, 0feh
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Dh, 049h, 000h
sub_1_4284:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        lea    eax, [esi + 70h]
        push   eax
        call   sub_1_B926
        add    esp, 4
        lea    edi, [esi + 88h]
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 8ch], 0
        mov    dword ptr [esi + 90h], 0
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_42E4
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_B996
        cmp    dword ptr [esp + 14h], 103h
        jne    SHORT loc_1_42E4
        push   edi
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 14h], eax
loc_1_42E4:
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esp + 10h], eax
        or     eax, eax
        je     SHORT loc_1_432C
        sub    ebx, ebx
        inc    edi
        mov    edi, eax
loc_1_42F4:
        cmp    ebx, 20h
        jae    SHORT loc_1_4320
        cmp    dword ptr [edi], 0
        je     SHORT loc_1_4318
loc_1_42FE:
        mov    ebp, dword ptr [edi]
        push   ebp
        push   edi
        call   sub_1_2774
        add    esp, 8
        push   ebp
        call   sub_1_B944
        add    esp, 4
        cmp    dword ptr [edi], 0
        jne    SHORT loc_1_42FE
loc_1_4318:
        inc    ebx
        add    edi, 4
        jmp    SHORT loc_1_42F4
        db  08Bh, 0FFh
loc_1_4320:
        push   dword ptr [esp + 10h]
        call   sub_1_B944
        add    esp, 4
loc_1_432C:
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esi + 1ch], 0
        or     eax, eax
        je     SHORT loc_1_4340
        push   eax
        call   sub_1_BA08
loc_1_4340:
        mov    eax, dword ptr [esi + 0ch]
        or     eax, eax
        je     SHORT loc_1_434D
        push   eax
        call   sub_1_BA2C
loc_1_434D:
        mov    edi, dword ptr [esi + 0dch]
        or     edi, edi
        je     SHORT loc_1_4386
        nop
loc_1_4358:
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0dch], eax
        push   dword ptr [edi + 4]
        call   sub_1_BA2C
        push   dword ptr [edi + 8]
        call   sub_1_B944
        add    esp, 4
        push   edi
        call   sub_1_B944
        add    esp, 4
        mov    edi, dword ptr [esi + 0dch]
        or     edi, edi
        jne    SHORT loc_1_4358
loc_1_4386:
        mov    eax, dword ptr [esi + 0d0h]
        or     eax, eax
        je     SHORT loc_1_4399
        push   eax
        call   sub_1_B944
        add    esp, 4
loc_1_4399:
        push   esi
        call   sub_1_B944
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  08Bh, 0FFh
loc_1_43AC:
        push   dword ptr [esp + 4]
        call   sub_1_34D8
        add    esp, 4
        ret
        db  08Dh, 049h, 000h
loc_1_43BC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 1ch]
        mov    esi, dword ptr [eax]
        sub    edx, edx
        sub    edi, edi
        cmp    esi, edx
        je     SHORT loc_1_4410
        mov    ebx, esi
        mov    ecx, dword ptr [esp + 18h]
loc_1_43D4:
        mov    eax, dword ptr [ecx]
        cmp    eax, 280h
        jne    SHORT loc_1_43F0
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        sub    eax, edx
        sbb    ebp, ebp
        not    ebp
        and    eax, ebp
        add    edx, eax
        jmp    SHORT loc_1_4405
        db  08Dh, 049h, 000h
loc_1_43F0:
        cmp    eax, 200h
        jne    SHORT loc_1_4405
        mov    eax, dword ptr [ecx + 4]
        inc    eax
        sub    eax, edi
        sbb    ebp, ebp
        not    ebp
        and    eax, ebp
        add    edi, eax
loc_1_4405:
        add    ecx, 8
        dec    ebx
        jne    SHORT loc_1_43D4
        jmp    SHORT loc_1_4414
        db  08Dh, 049h, 000h
loc_1_4410:
        mov    ecx, dword ptr [esp + 18h]
loc_1_4414:
        add    esi, 2
        cmp    esi, 20h
        jbe    SHORT loc_1_4424
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  090h
loc_1_4424:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 0bch], edx
        mov    dword ptr [eax + 0c0h], edi
        mov    dword ptr [ecx], 280h
        mov    dword ptr [ecx + 4], edx
        add    ecx, 8
        mov    dword ptr [ecx], 200h
        mov    dword ptr [ecx + 4], edi
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [eax], esi
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  08Dh, 049h, 000h
sub_1_445C:
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 1ch], 0
        push   0e0h
        call   sub_1_B93E
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_4488
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_1_4488:
        mov    edi, esi
        mov    eax, dword ptr [esp + 50h]
        mov    dword ptr [eax], esi
        sub    eax, eax
        mov    ecx, 38h
        rep stosd
        mov    dword ptr [esi + 7ch], eax
        mov    dword ptr [esi + 84h], eax
        db  0A1h, 06Ch, 0BFh, 000h, 000h
        mov    dword ptr [esi + 80h], eax
        mov    dword ptr [esp + 34h], 0
        mov    dword ptr [esp + 38h], 1
        mov    dword ptr [esp + 3ch], 2
        mov    dword ptr [esp + 40h], 7
        mov    byte ptr [esi + 5bh], 1
        mov    byte ptr [esi + 96h], 2
        mov    byte ptr [esi + 97h], 2
        mov    dword ptr [esi + 34h], 0ffffffffh
        mov    dword ptr [esi + 0d8h], 64h
        call   sub_1_B918
        db  0A3h, 0C4h, 0C1h, 000h, 000h
        push   8
        db  0FFh, 035h, 0D8h, 0BCh, 000h, 000h
        lea    eax, [esi + 10h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        call   sub_1_BA02
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_452C
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_452C:
        db  0FFh, 035h, 0DCh, 0BCh, 000h, 000h
        lea    eax, [esi + 0ch]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_BA26
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_455C
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_455C:
        push   80h
        call   sub_1_B93E
        add    esp, 4
        mov    dword ptr [esi + 1ch], eax
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_4588
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Dh, 049h, 000h
loc_1_4588:
        sub    eax, eax
        mov    ecx, 20h
        rep stosd
        lea    ebx, [esi + 88h]
        mov    dword ptr [ebx], eax
        mov    dword ptr [esi + 8ch], eax
        mov    dword ptr [esi + 90h], eax
        lea    edi, [esi + 4]
        push   eax
        push   eax
        mov    ebp, dword ptr [esp + 50h]
        push   ebp
        push   esi
        db  0FFh, 035h, 0E4h, 0BFh, 000h, 000h
        push   4
        lea    eax, [esp + 4ch]
        push   eax
        lea    eax, [esp + 38h]
        push   eax
        push   edi
        lea    eax, [esp + 54h]
        push   eax
        lea    eax, [esp + 40h]
        push   eax
        call   sub_1_B990
        cmp    dword ptr [esp + 18h], 103h
        jne    SHORT loc_1_45E9
        push   ebx
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 18h], eax
loc_1_45E9:
        mov    eax, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + eax*4 + 34h]
        mov    dword ptr [esi + 8], eax
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_4614
        mov    dword ptr [edi], 0
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_1_4614:
        or     eax, eax
        je     SHORT loc_1_4644
        cmp    eax, 1
        je     SHORT loc_1_4688
        cmp    eax, 2
        je     NEAR PTR loc_1_46AC
        cmp    eax, 7
        je     NEAR PTR loc_1_46D8
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_4644:
        mov    byte ptr [esp + 13h], 6
        mov    byte ptr [esp + 17h], 1
        mov    byte ptr [esp + 16h], 1
        mov    byte ptr [esp + 15h], 0
        mov    edi, 1010102h
        mov    bl, 0eh
        mov    byte ptr [esp + 14h], bl
        push   ebp
        call   sub_1_B910
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_467C
        sub    cl, cl
        mov    edx, 0bh
        jmp    NEAR PTR loc_1_46FE
loc_1_467C:
        mov    cl, 8
        mov    bl, 16h
        mov    edx, 0bh
        jmp    SHORT loc_1_46FE
        db  090h
loc_1_4688:
        mov    byte ptr [esp + 13h], 6
        mov    byte ptr [esp + 17h], 80h
        mov    byte ptr [esp + 16h], 80h
        mov    byte ptr [esp + 15h], 2
        mov    edi, 2010102h
        mov    bl, 28h
        mov    byte ptr [esp + 14h], 0eh
        mov    cl, 8
        jmp    SHORT loc_1_46F9
loc_1_46AC:
        mov    byte ptr [esp + 13h], 6
        mov    byte ptr [esp + 17h], 1
        mov    byte ptr [esp + 16h], 1
        mov    byte ptr [esp + 15h], 1
        mov    edi, 3010102h
        mov    bl, 15h
        mov    byte ptr [esp + 14h], 0dh
        mov    cl, 8
        mov    edx, 0bh
        jmp    SHORT loc_1_46FE
        db  08Dh, 049h, 000h
loc_1_46D8:
        mov    byte ptr [esp + 13h], 1
        mov    byte ptr [esp + 17h], 0ffh
        mov    byte ptr [esp + 16h], 0
        mov    byte ptr [esp + 15h], 1
        mov    edi, 6010102h
        mov    bl, 3
        mov    byte ptr [esp + 14h], bl
        sub    cl, cl
loc_1_46F9:
        mov    edx, 9
loc_1_46FE:
        mov    al, byte ptr [esp + 17h]
        mov    byte ptr [esi + 5dh], al
        mov    al, byte ptr [esp + 16h]
        mov    byte ptr [esi + 5eh], al
        mov    al, byte ptr [esp + 15h]
        mov    byte ptr [esi + 5fh], al
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [esi + 5ch], al
        mov    al, byte ptr [esp + 14h]
        mov    byte ptr [esi + 60h], al
        mov    byte ptr [esi + 61h], cl
        mov    dword ptr [esi + 64h], edx
        push   0
        movzx  eax, byte ptr [esp + 17h]
        push   eax
        lea    eax, [esi + 54h]
        push   eax
        push   edi
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 18h
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        je     SHORT loc_1_475C
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_475C:
        push   0
        push   4
        lea    eax, [esp + 28h]
        push   eax
        push   10106h
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 18h
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        je     SHORT loc_1_4790
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
loc_1_4790:
        movzx  eax, byte ptr [esi + 61h]
        sub    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 1
        jne    SHORT loc_1_47A8
        sub    dword ptr [esp + 20h], 12h
        jmp    SHORT loc_1_47C3
        db  090h
loc_1_47A8:
        cmp    eax, 2
        jne    SHORT loc_1_47C3
        mov    eax, dword ptr [esp + 20h]
        sub    eax, 1100h
        sbb    ecx, ecx
        and    eax, ecx
        add    eax, 1100h
        mov    dword ptr [esp + 20h], eax
loc_1_47C3:
        push   0
        mov    eax, dword ptr [esp + 24h]
        mov    word ptr [esi + 94h], ax
        push   4
        lea    eax, [esp + 34h]
        push   eax
        push   10107h
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 18h
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_1_4802
        mov    eax, dword ptr [esp + 2ch]
        shl    eax, 2
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        mov    dword ptr [esi + 98h], eax
loc_1_4802:
        push   0
        push   4
        lea    eax, [esp + 30h]
        push   eax
        push   10113h
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 18h
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        je     SHORT loc_1_4830
        mov    eax, dword ptr [esp + 4ch]
        mov    dword ptr [eax], 0
        jmp    SHORT loc_1_4847
        db  090h
loc_1_4830:
        mov    ecx, 0
        mov    eax, dword ptr [esp + 28h]
        and    eax, 1
        or     eax, eax
        setne  cl
        mov    eax, dword ptr [esp + 4ch]
        mov    dword ptr [eax], ecx
loc_1_4847:
        lea    edi, [esi + 0c4h]
        lea    eax, [esp + 24h]
        push   eax
        push   0
        push   edi
        push   1010dh
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 18h
        mov    dword ptr [esp + 18h], eax
        cmp    eax, 0c0010014h
        je     SHORT loc_1_4877
        cmp    eax, 0c0010016h
        jne    SHORT loc_1_48B4
loc_1_4877:
        push   dword ptr [esp + 24h]
        call   sub_1_B93E
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_48B4
        push   0
        push   dword ptr [esp + 28h]
        push   ebp
        push   1010dh
        push   0
        push   esi
        call   sub_1_1728
        add    esp, 18h
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_1_48B4
        mov    dword ptr [edi], ebp
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [esi + 0b4h], eax
loc_1_48B4:
        test   bl, 3
        je     SHORT loc_1_48BF
        and    bl, 0fch
        add    bl, 4
loc_1_48BF:
        push   esi
        mov    byte ptr [esi + 5ah], bl
        call   sub_1_17EC
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_48D9
        push   eax
        push   esi
        call   sub_1_17BC
        add    esp, 8
loc_1_48D9:
        movzx  edi, bl
        lea    eax, [edi + 1ch]
        db  00Fh, 0AFh, 005h, 0D8h, 0BCh, 000h, 000h
        push   eax
        call   sub_1_B93E
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_4908
        push   esi
        call   sub_1_4284
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_4908:
        mov    dword ptr [esi + 0d0h], eax
        mov    dword ptr [esi + 18h], 0
        mov    dword ptr [esp + 1ch], 0
        db  08Bh, 00Dh, 0D8h, 0BCh, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_1_493E
        nop
loc_1_4928:
        mov    edx, dword ptr [esi + 18h]
        mov    dword ptr [eax], edx
        mov    dword ptr [esi + 18h], eax
        lea    eax, [edi + eax + 1ch]
        inc    dword ptr [esp + 1ch]
        cmp    dword ptr [esp + 1ch], ecx
        jb     SHORT loc_1_4928
loc_1_493E:
        push   esi
        db  068h, 080h, 02Ch, 000h, 000h
        push   3e8h
        add    esi, 70h
        push   esi
        call   sub_1_B920
        add    esp, 10h
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 34h
        ret
        db  08Bh, 0FFh
loc_1_4964:
        mov    eax, dword ptr [esp + 20h]
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ecx + 0b8h], eax
        mov    eax, 1
        ret
loc_1_4980:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        call   sub_1_B94A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_1_445C
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_49B0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
        leave
        ret    14h
        db  090h
loc_1_49B0:
        call   sub_1_B94A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4ch], eax
        db  0C7h, 045h, 0C4h, 024h, 026h, 000h, 000h
        db  0C7h, 045h, 0C8h, 09Ch, 027h, 000h, 000h
        db  0C7h, 045h, 0CCh, 0D4h, 027h, 000h, 000h
        db  0C7h, 045h, 0D0h, 0B4h, 02Ah, 000h, 000h
        db  0C7h, 045h, 0D4h, 0BCh, 02Bh, 000h, 000h
        db  0C7h, 045h, 0D8h, 038h, 028h, 000h, 000h
        db  0C7h, 045h, 0DCh, 0ACh, 043h, 000h, 000h
        db  0C7h, 045h, 0E0h, 078h, 039h, 000h, 000h
        db  0C7h, 045h, 0E4h, 058h, 03Eh, 000h, 000h
        db  0C7h, 045h, 0E8h, 0BCh, 043h, 000h, 000h
        movzx  eax, word ptr [eax + 94h]
        mov    dword ptr [ebp - 48h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 98h]
        mov    dword ptr [ebp - 44h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + 5ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 54h
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 4ch]
        push   eax
        db  068h, 064h, 049h, 000h, 000h
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        call   sub_1_10D0
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_4A68
        push   dword ptr [ebp - 4]
        call   sub_1_4284
        add    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0c0000001h
        leave
        ret    14h
        db  08Bh, 0FFh
loc_1_4A68:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        leave
        ret    14h
        db  08Dh, 049h, 000h
loc_1_4A78:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        lea    eax, [esi + 64h]
        mov    dword ptr [eax], 0
        push   0
        push   4
        push   eax
        push   1010eh
        push   1
        push   esi
        call   sub_1_1728
        add    esp, 18h
        mov    byte ptr [esi + 5bh], 2
        mov    byte ptr [esi + 96h], 2
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_4ABF
        push   eax
        call   sub_1_14EC
        add    esp, 4
        mov    dword ptr [esi], 0
loc_1_4ABF:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_4B0C
        lea    edi, [esi + 88h]
        mov    dword ptr [edi], 0
        mov    dword ptr [esi + 8ch], 0
        mov    dword ptr [esi + 90h], 0
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_1_B996
        cmp    dword ptr [esp + 8], 103h
        jne    SHORT loc_1_4B14
        push   edi
        call   sub_1_B950
        add    esp, 4
        mov    dword ptr [esp + 8], eax
        jmp    SHORT loc_1_4B14
        db  08Bh, 0FFh
loc_1_4B0C:
        mov    dword ptr [esp + 8], 0
loc_1_4B14:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_1_4B35
        mov    dword ptr [esi + 4], 0
        push   esi
        call   sub_1_4284
        add    esp, 4
loc_1_4B35:
        pop    edi
        pop    esi
        add    esp, 4
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_4B40:
        sub    esp, 4
        call   sub_1_6DB8
        db  083h, 03Dh, 0F7h, 0BBh, 000h, 000h, 000h
        je     SHORT loc_1_4B6B
        db  0FFh, 035h, 0E4h, 0BFh, 000h, 000h
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_B98A
        push   0
        call   sub_1_B962
        add    esp, 4
loc_1_4B6B:
        add    esp, 4
        ret
        db  090h
_LTEXT ends
        end

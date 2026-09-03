; int13_lcode_01.asm -- INT13 object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0014:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_001C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        nop
        nop
INT13_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0C1h, 0FFh, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 009h, 000h, 0FFh, 0FFh
        cmp    eax, 2
        db  00Fh, 084h, 002h, 000h, 0FFh, 0FFh
        clc
        ret
Int13_Get_Version:
        mov    eax, 30ah
        clc
        ret
Int13_Hooking_BIOS_Int:
        ret
Int13_Unhooking_BIOS_Int:
        ret
sub_1_005C:
        pushad
        db  0A1h, 01Ch, 000h, 000h, 000h
        and    byte ptr [eax + 2ah], 0dfh
        mov    ecx, dword ptr [78h]
        db  089h, 00Dh, 02Bh, 00Ah, 000h, 000h
        mov    eax, ecx
        and    ecx, 0ffffh
        and    eax, 0ffff0000h
        shr    eax, 0ch
        or     eax, ecx
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        movzx  eax, byte ptr [eax + 3]
        db  0A2h, 02Fh, 00Ah, 000h, 000h
        popad
        ret
        db  000h, 000h, 000h
Int13_Translate_VM_Int:
        xor    edx, edx
        cmp    byte ptr [ebp + 1dh], dl
        jne    SHORT loc_1_00B0
        cmp    byte ptr [ebp + 14h], 80h
        jb     SHORT loc_1_00B0
        mov    byte ptr [ebp + 14h], dl
loc_1_00B0:
        movzx  edx, byte ptr [ebp + 14h]
        test   dl, 78h
        jne    NEAR PTR loc_1_0160
        rol    dl, 1
        db  08Bh, 03Ch, 095h, 0EAh, 009h, 000h, 000h
        ror    dl, 1
        or     edi, edi
        je     NEAR PTR loc_1_0160
        test   dword ptr [edi + 20h], 2000h
        jne    NEAR PTR loc_1_0160
        cmp    byte ptr [ebp + 1dh], 18h
        je     SHORT loc_1_00E9
        db  0CDh, 020h, 047h, 000h, 001h, 000h
loc_1_00E9:
        call   sub_1_0708
        db  0C7h, 046h, 00Ch, 078h, 006h, 000h, 000h
        db  08Bh, 015h, 01Ch, 000h, 000h, 000h
        test   byte ptr [edx + 28h], 1
        je     SHORT loc_1_0107
        cmp    byte ptr [edi + 40h], 0ah
        je     SHORT loc_1_0162
loc_1_0107:
        movzx  edx, byte ptr [ebp + 14h]
        movzx  ecx, byte ptr [ebp + 1dh]
        cmp    cl, 1ah
        ja     SHORT loc_1_011B
        db  0FFh, 024h, 08Dh, 030h, 00Ah, 000h, 000h
loc_1_011B:
        sub    cl, 41h
        cmp    cl, 8
        ja     SHORT loc_1_012A
        db  0FFh, 024h, 08Dh, 09Ch, 00Ah, 000h, 000h
loc_1_012A:
        mov    byte ptr [ebp + 1dh], 1
        jmp    SHORT loc_1_0154
loc_1_0130:
        mov    byte ptr [ebp + 1dh], 0
        and    byte ptr [ebp + 2ch], 0feh
loc_1_0138:
        movzx  edx, byte ptr [edi + 15h]
        rol    dl, 1
        mov    al, byte ptr [ebp + 1dh]
        db  088h, 082h, 0D0h, 009h, 000h, 000h
loc_1_0147:
        call   sub_1_076A
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        clc
        ret
loc_1_0154:
        or     byte ptr [ebp + 2ch], 1
        jmp    SHORT loc_1_0138
loc_1_015A:
        and    byte ptr [ebp + 2ch], 0feh
        jmp    SHORT loc_1_0147
loc_1_0160:
        stc
        ret
loc_1_0162:
        db  0A1h, 01Ch, 000h, 000h, 000h
        test   byte ptr [eax + 2ah], 20h
        jne    SHORT loc_1_0107
        db  08Bh, 015h, 02Bh, 00Ah, 000h, 000h
        cmp    edx, dword ptr [78h]
        jne    NEAR PTR loc_1_039A
        pushad
        mov    eax, edx
        and    edx, 0ffffh
        and    eax, 0ffff0000h
        shr    eax, 0ch
        or     eax, edx
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        movzx  edx, byte ptr [eax + 3]
        db  03Ah, 015h, 02Fh, 00Ah, 000h, 000h
        popad
        jne    NEAR PTR loc_1_039A
        jmp    NEAR PTR loc_1_0107
loc_1_01B6:
        or     dword ptr [esi + 8], 100h
        mov    word ptr [esi + 4], 11h
        or     word ptr [esi + 8], 4008h
        mov    byte ptr [esi + 4dh], 0
        add    edi, 0c3h
        push   esi
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        pop    esi
        sub    edi, 0c3h
        jmp    NEAR PTR loc_1_0130
loc_1_01E6:
        movzx  edx, byte ptr [ebp + 14h]
        rol    dl, 1
        db  08Ah, 082h, 0D0h, 009h, 000h, 000h
        mov    byte ptr [ebp + 1ch], al
        jmp    NEAR PTR loc_1_0130
loc_1_01FA:
        mov    word ptr [esi + 4], 1
loc_1_0200:
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
        call   sub_1_0611
        cmp    byte ptr [edi + 40h], 0ah
        je     NEAR PTR loc_1_0339
loc_1_0215:
        movzx  eax, word ptr [ebp + 38h]
        movzx  edx, word ptr [ebp + 10h]
loc_1_021D:
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        add    eax, edx
        mov    dword ptr [esi + 1ch], eax
        shl    ecx, 9
        dec    ecx
        mov    edx, eax
        and    edx, 0fffh
        add    ecx, edx
        shr    ecx, 0ch
        inc    ecx
        shr    eax, 0ch
        xor    edx, edx
        push   edx
        cmp    word ptr [esi + 4], 1
        je     SHORT loc_1_024C
        mov    edx, 80000000h
loc_1_024C:
        push   ecx
        push   eax
        push   edx
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        push   eax
        mov    eax, esi
        sub    eax, 64h
        push   eax
        db  0FFh, 015h, 014h, 000h, 000h, 000h
        add    esp, 4
        xchg   dword ptr [esp], eax
        test   eax, eax
        je     SHORT loc_1_0286
        pop    eax
        or     eax, eax
        jne    SHORT loc_1_028B
        call   sub_1_064B
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_1_029D
loc_1_0286:
        add    esp, 10h
        jmp    SHORT loc_1_0294
loc_1_028B:
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
loc_1_0294:
        or     dword ptr [esi + 8], 40h
        call   sub_1_064B
loc_1_029D:
        xor    al, al
        mov    cx, word ptr [esi + 6]
        cmp    cx, 2
        jne    SHORT loc_1_02AD
        mov    al, 11h
        jmp    SHORT loc_1_02EE
loc_1_02AD:
        cmp    cx, 10h
        jb     SHORT loc_1_02EE
        je     SHORT loc_1_02EC
        cmp    cx, 18h
        je     SHORT loc_1_02CB
        cmp    cx, 19h
        je     SHORT loc_1_02CF
        cmp    cx, 1fh
        je     SHORT loc_1_02E8
        mov    al, 0ah
        jmp    SHORT loc_1_02EE
loc_1_02CB:
        mov    al, 80h
        jmp    SHORT loc_1_02D1
loc_1_02CF:
        mov    al, 6
loc_1_02D1:
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_1_02EE
        test   byte ptr [edi + 20h], 4
        je     SHORT loc_1_02EE
        push   eax
        push   edx
        call   sub_1_0684
        pop    edx
        pop    eax
        jmp    SHORT loc_1_02EE
loc_1_02E8:
        mov    al, 3
        jmp    SHORT loc_1_02EE
loc_1_02EC:
        mov    al, 4
loc_1_02EE:
        test   al, al
        jne    SHORT loc_1_0322
        test   byte ptr [ebp + 14h], 80h
        je     SHORT loc_1_030C
        cmp    word ptr [esi + 4], 0
        jne    NEAR PTR loc_1_0130
        mov    byte ptr [ebp + 1ch], 50h
        jmp    NEAR PTR loc_1_0130
loc_1_030C:
        cmp    word ptr [esi + 4], 2
        ja     NEAR PTR loc_1_0130
        mov    eax, dword ptr [esi + 18h]
        mov    byte ptr [ebp + 1ch], al
        jmp    NEAR PTR loc_1_0130
loc_1_0322:
        mov    byte ptr [ebp + 1dh], al
        cmp    word ptr [esi + 4], 0
        jne    NEAR PTR loc_1_0154
        mov    byte ptr [ebp + 1ch], 0
        jmp    NEAR PTR loc_1_0154
loc_1_0339:
        db  0A1h, 01Ch, 000h, 000h, 000h
        test   byte ptr [eax + 2ah], 20h
        jne    NEAR PTR loc_1_0215
        db  08Bh, 015h, 02Bh, 00Ah, 000h, 000h
        cmp    edx, dword ptr [78h]
        jne    SHORT loc_1_039A
        pushad
        mov    eax, edx
        and    edx, 0ffffh
        and    eax, 0ffff0000h
        shr    eax, 0ch
        or     eax, edx
        push   0
        push   1
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        movzx  edx, byte ptr [eax + 3]
        db  03Ah, 015h, 02Fh, 00Ah, 000h, 000h
        popad
        jne    SHORT loc_1_039A
        movzx  edx, word ptr [ebp + 18h]
        and    edx, 3fh
        add    edx, ecx
        dec    edx
        cmp    edx, dword ptr [edi + 0e7h]
        jbe    NEAR PTR loc_1_0215
loc_1_039A:
        call   sub_1_076A
        db  08Bh, 03Dh, 0EAh, 009h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_03AD
        or     byte ptr [edi + 24h], 8
loc_1_03AD:
        db  08Bh, 03Dh, 0F2h, 009h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_03BB
        or     byte ptr [edi + 24h], 8
loc_1_03BB:
        xor    eax, eax
        mov    ecx, eax
        inc    al
        db  0CDh, 020h, 001h, 000h, 036h, 000h
        pushfd
        db  08Bh, 03Dh, 0EAh, 009h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_03D6
        and    byte ptr [edi + 24h], 0f7h
loc_1_03D6:
        db  08Bh, 03Dh, 0F2h, 009h, 000h, 000h
        or     edi, edi
        je     SHORT loc_1_03E4
        and    byte ptr [edi + 24h], 0f7h
loc_1_03E4:
        popfd
        jb     SHORT loc_1_03FC
        xor    eax, eax
        db  0BEh, 019h, 004h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        stc
        ret
loc_1_03FC:
        mov    byte ptr [ebp + 1dh], 8
        or     byte ptr [ebp + 2ch], 1
        movzx  edx, byte ptr [ebp + 14h]
        db  08Bh, 03Ch, 095h, 0EAh, 009h, 000h, 000h
        call   sub_1_0708
        jmp    NEAR PTR loc_1_0138
loc_1_0419:
        db  0CDh, 020h, 002h, 000h, 036h, 000h
        ret
loc_1_0420:
        mov    word ptr [esi + 4], 2
        call   sub_1_0611
loc_1_042B:
        call   sub_1_064B
        cmp    word ptr [esi + 6], 16h
        je     NEAR PTR loc_1_0130
        jmp    NEAR PTR loc_1_029D
loc_1_0440:
        movzx  eax, word ptr [ebp + 38h]
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        movzx  edx, word ptr [ebp + 10h]
        add    eax, edx
        mov    dword ptr [esi + 48h], eax
        movzx  eax, byte ptr [ebp + 1ch]
        mov    dword ptr [esi - 4], eax
        mov    al, byte ptr [ebp + 15h]
        mov    byte ptr [esi - 8], al
        movzx  eax, byte ptr [ebp + 19h]
        mov    word ptr [esi - 6], ax
        mov    word ptr [esi + 4], 0eh
        mov    dword ptr [esi + 18h], 0
        call   sub_1_064B
        jmp    NEAR PTR loc_1_029D
loc_1_047F:
        adc    byte ptr [esi], 40h
        or     esp, dword ptr [eax - 2ff69ffbh]
        db  002h
loc_1_0489:
        db  0A0h, 0E8h, 009h, 000h, 000h
        db  08Ah, 025h, 0E9h, 009h, 000h, 000h
        sub    al, ah
        push   esi
        lea    esi, [edi + 0d3h]
        test   dl, 80h
        jne    SHORT loc_1_04F7
        db  08Bh, 00Dh, 01Ch, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 2ch]
        or     ecx, ecx
        je     SHORT loc_1_04D4
loc_1_04AF:
        cmp    byte ptr [ecx], 1
        jne    SHORT loc_1_04D4
        cmp    byte ptr [ecx + 5], dl
        je     SHORT loc_1_04C4
        cmp    dword ptr [ecx + 1], 0
        je     SHORT loc_1_04D4
        add    ecx, dword ptr [ecx + 1]
        jmp    SHORT loc_1_04AF
loc_1_04C4:
        push   ecx
        mov    ecx, dword ptr [ecx + 19h]
        mov    word ptr [ebp], cx
        ror    ecx, 10h
        mov    word ptr [ebp + 38h], cx
        pop    ecx
loc_1_04D4:
        test   byte ptr [edi + 24h], 40h
        jne    SHORT loc_1_0518
        lea    esi, [edi + 54h]
        push   eax
        mov    eax, dword ptr [esi]
        push   edi
        db  08Dh, 03Dh, 07Fh, 004h, 000h, 000h
        mov    ecx, 6
        repne scasw
        pop    edi
        mov    word ptr [ebp + 10h], cx
        pop    eax
        xchg   ah, al
loc_1_04F7:
        mov    ah, byte ptr [esi + 0ch]
        dec    ah
        mov    word ptr [ebp + 14h], ax
        mov    ax, word ptr [esi + 10h]
        dec    ax
        shl    ah, 6
        or     ah, byte ptr [esi + 14h]
loc_1_050C:
        xchg   al, ah
        mov    word ptr [ebp + 18h], ax
        pop    esi
        jmp    NEAR PTR loc_1_0130
loc_1_0518:
        mov    al, byte ptr [ecx + 8]
        dec    al
        xchg   al, ah
        mov    word ptr [ebp + 14h], ax
        movzx  ax, byte ptr [ecx + 0ch]
        mov    word ptr [ebp + 10h], ax
        mov    ax, word ptr [ecx + 6]
        dec    ax
        shl    ah, 6
        or     ah, byte ptr [ecx + 0ah]
        jmp    SHORT loc_1_050C
loc_1_053A:
        cmp    byte ptr [edi + 40h], 0ah
        je     SHORT loc_1_055C
        mov    word ptr [ebp + 1ch], 300h
        mov    edx, dword ptr [edi + 0d3h]
        mov    word ptr [ebp + 14h], dx
        shr    edx, 10h
        mov    word ptr [ebp + 18h], dx
        jmp    NEAR PTR loc_1_015A
loc_1_055C:
        mov    word ptr [ebp + 1ch], 200h
        test   dword ptr [edi + 20h], 10000h
        jne    NEAR PTR loc_1_015A
        mov    word ptr [ebp + 1ch], 100h
        jmp    NEAR PTR loc_1_015A
loc_1_057A:
        cmp    byte ptr [edi + 40h], 0ah
        je     SHORT loc_1_0592
loc_1_0580:
        mov    byte ptr [ebp + 1dh], 0
        jmp    NEAR PTR loc_1_015A
loc_1_0589:
        test   dword ptr [edi + 20h], 4
        je     SHORT loc_1_0580
loc_1_0592:
        or     dword ptr [esi + 8], 100h
        mov    word ptr [esi + 4], 6
        mov    dword ptr [esi + 18h], 0
        mov    al, byte ptr [edi + 15h]
        mov    byte ptr [esi + 4dh], al
        add    edi, 0c3h
        push   esi
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        add    esp, 4
        sub    edi, 0c3h
        cmp    word ptr [esi + 6], 10h
        jb     SHORT loc_1_0580
        mov    byte ptr [ebp + 1dh], 6
        jmp    NEAR PTR loc_1_0154
loc_1_05D2:
        cmp    byte ptr [edi + 40h], 0ah
        jne    SHORT loc_1_05F9
        movzx  eax, byte ptr [ebp + 18h]
        mov    dword ptr [edi + 0e7h], eax
        mov    dl, byte ptr [ebp + 19h]
        inc    dl
        mul    dl
        shl    ax, 1
        mov    dword ptr [edi + 0d3h], eax
        call   sub_1_076A
        stc
        ret
loc_1_05F9:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 1
        jmp    NEAR PTR loc_1_0154
loc_1_0608:
        mov    byte ptr [ebp + 1dh], 1
        jmp    NEAR PTR loc_1_0154
sub_1_0611:
        xor    eax, eax
        mov    dword ptr [esi + 14h], eax
        movzx  eax, word ptr [ebp + 18h]
        mov    ecx, eax
        shr    al, 6
        xchg   ah, al
        mov    edx, dword ptr [edi + 0dfh]
        imul   eax, edx
        movzx  edx, byte ptr [ebp + 15h]
        add    eax, edx
        mov    edx, dword ptr [edi + 0e7h]
        imul   eax, edx
        and    ecx, 3fh
        lea    eax, [ecx + eax - 1]
        mov    dword ptr [esi + 10h], eax
        movzx  ecx, byte ptr [ebp + 1ch]
        mov    dword ptr [esi + 18h], ecx
        ret
sub_1_064B:
        push   ebx
        push   edx
        mov    al, byte ptr [edi + 15h]
        mov    byte ptr [esi + 4dh], al
        add    edi, 0c3h
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        sub    edi, 0c3h
        pop    edx
        pop    ebx
        db  0A1h, 0E0h, 009h, 000h, 000h
        db  08Bh, 00Dh, 0E4h, 009h, 000h, 000h
        db  0CDh, 020h, 027h, 080h, 001h, 000h
loc_1_0678:
        db  0A1h, 0E0h, 009h, 000h, 000h
        db  0CDh, 020h, 031h, 001h, 001h, 000h
        ret
sub_1_0684:
        mov    eax, edi
        or     dword ptr [eax + 20h], 80000h
        test   dword ptr [eax + 20h], 4000h
        jne    SHORT loc_1_0699
        mov    eax, dword ptr [eax + 10h]
loc_1_0699:
        or     eax, eax
        je     SHORT loc_1_06CA
        or     dword ptr [eax + 20h], 100000h
        db  08Bh, 015h, 01Ch, 000h, 000h, 000h
        mov    edx, dword ptr [edx + 40h]
        cli
loc_1_06AE:
        or     edx, edx
        je     SHORT loc_1_06C4
        cmp    dword ptr [edx + 28h], eax
        jne    SHORT loc_1_06BF
        or     dword ptr [edx + 4], 2
        or     dword ptr [edx + 4], 1
loc_1_06BF:
        mov    edx, dword ptr [edx + 24h]
        jmp    SHORT loc_1_06AE
loc_1_06C4:
        sti
        mov    eax, dword ptr [eax + 10h]
        jmp    SHORT loc_1_0699
loc_1_06CA:
        ret
        db  000h
loc_1_06CC:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  esi, word ptr [ebx]
        cmp    esi, 3
        db  00Fh, 084h, 01Eh, 0F9h, 0FEh, 0FFh
        cmp    esi, 4
        db  00Fh, 084h, 073h, 0F9h, 0FEh, 0FFh
        cmp    esi, 2
        je     SHORT loc_1_06FE
        cmp    esi, 16h
        je     NEAR PTR sub_1_005C
        call   sub_1_06FF
loc_1_06FE:
        ret
sub_1_06FF:
        mov    word ptr [ebx + 2], 0ffffh
        ret
        db  000h, 000h
sub_1_0708:
        movzx  ecx, byte ptr [edi + 77h]
        shl    ecx, 3
        mov    eax, dword ptr [edi + 4]
        add    ecx, eax
        add    cx, 0bch
        add    eax, 64h
        sub    esp, 10h
        mov    esi, esp
        mov    word ptr [esi + 4], cx
        mov    dword ptr [esi + 6], eax
        mov    byte ptr [esi + 0eh], 2
        mov    word ptr [esi], 2
        push   esi
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esi + 0ah]
        add    esp, 14h
        mov    ecx, esi
        add    esi, eax
        mov    dword ptr [esi + 48h], esi
        add    dword ptr [esi + 48h], 58h
        mov    dword ptr [esi + 20h], ecx
        mov    eax, esi
        sub    eax, 64h
        mov    dword ptr [eax + 8], edi
        xor    eax, eax
        mov    word ptr [esi + 4], ax
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi + 44h], eax
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 8], 40000401h
        ret
sub_1_076A:
        mov    esi, dword ptr [esi + 20h]
        sub    esp, 8
        mov    ecx, esp
        mov    dword ptr [ecx + 4], esi
        mov    word ptr [ecx], 4
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 0ch
        ret
        db  000h, 000h, 000h
loc_1_0788:
        mov    byte ptr [ebp + 1dh], 1
        mov    byte ptr [ebp + 1ch], 0
        mov    word ptr [ebp + 10h], 0aa55h
        mov    word ptr [ebp + 18h], 3
        and    byte ptr [ebp + 2ch], 0feh
        jmp    NEAR PTR loc_1_0138
loc_1_07A5:
        mov    word ptr [esi + 4], 0
loc_1_07AB:
        call   sub_1_07D7
        jb     NEAR PTR loc_1_0154
        movzx  edx, word ptr [eax + 4]
        movzx  eax, word ptr [eax + 6]
        jmp    NEAR PTR loc_1_021D
loc_1_07C3:
        mov    word ptr [esi + 4], 1
        test   byte ptr [ebp + 1ch], 1
        je     SHORT loc_1_07AB
        mov    word ptr [esi + 4], 4
        jmp    SHORT loc_1_07AB
sub_1_07D7:
        mov    byte ptr [ebp + 1dh], 7
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    byte ptr [eax], 10h
        jb     SHORT loc_1_07FD
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [esi + 18h], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [esi + 10h], edx
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 14h], edx
loc_1_07FD:
        ret
loc_1_07FE:
        mov    word ptr [esi + 4], 2
        call   sub_1_07D7
        jb     NEAR PTR loc_1_0154
        cmp    cl, 6
        jne    NEAR PTR loc_1_042B
        mov    dword ptr [esi + 18h], 1
        jmp    NEAR PTR loc_1_042B
loc_1_0824:
        push   word ptr [ebp + 1ch]
        mov    dword ptr [esi + 34h], esp
        mov    word ptr [esi + 2eh], 440dh
        mov    dword ptr [esi + 30h], 848h
        mov    word ptr [esi + 4], 0dh
        or     dword ptr [esi + 8], 100h
        mov    al, byte ptr [edi + 15h]
        mov    byte ptr [esi + 4dh], al
        push   edx
        push   ebx
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        pop    ebx
        pop    edx
        pop    ax
        mov    byte ptr [ebp + 1ch], ah
        mov    eax, dword ptr [esi + 3ch]
        mov    byte ptr [ebp + 1dh], al
        or     eax, eax
        jne    NEAR PTR loc_1_0154
        jmp    NEAR PTR loc_1_0130
loc_1_086D:
        mov    word ptr [esi + 2eh], 440dh
        mov    dword ptr [esi + 30h], 849h
        mov    word ptr [esi + 4], 0dh
        or     byte ptr [esi + 9], 1
        mov    al, byte ptr [edi + 15h]
        mov    byte ptr [esi + 4dh], al
        push   edx
        push   ebx
        db  0CDh, 020h, 004h, 000h, 010h, 000h
        pop    ebx
        pop    edx
        mov    eax, dword ptr [esi + 3ch]
        mov    byte ptr [ebp + 1dh], al
        or     eax, eax
        jne    NEAR PTR loc_1_0154
        jmp    NEAR PTR loc_1_0130
loc_1_08A7:
        xchg   edx, eax
        mov    ax, 3c04h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edx, eax
        mov    byte ptr [ebp + 1dh], 7
        cmp    word ptr [edx], 1ah
        jb     NEAR PTR loc_1_0154
        call   sub_1_093B
        jb     SHORT loc_1_08DA
        push   edi
        push   esi
        mov    esi, eax
        lea    edi, [edx + 2]
        mov    ecx, 18h
        cld
        rep movsb
        pop    esi
        jmp    SHORT loc_1_0930
loc_1_08DA:
        xor    eax, eax
        mov    al, 2dh
        test   byte ptr [edi + 20h], 4
        jne    SHORT loc_1_08E6
        and    al, 0fbh
loc_1_08E6:
        test   byte ptr [edi + 22h], 3
        je     SHORT loc_1_08EE
        or     al, 10h
loc_1_08EE:
        test   byte ptr [ebx + 1eh], 1
        jne    SHORT loc_1_08F6
        and    al, 0dfh
loc_1_08F6:
        test   byte ptr [edi + 22h], 8
        lea    ecx, [edi + 54h]
        jne    SHORT loc_1_0907
        lea    ecx, [edi + 0d3h]
        or     al, 2
loc_1_0907:
        push   edi
        mov    edi, dword ptr [ecx + 10h]
        mov    dword ptr [edx + 4], edi
        mov    edi, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 8], edi
        mov    edi, dword ptr [ecx + 14h]
        mov    dword ptr [edx + 0ch], edi
        mov    edi, dword ptr [ecx]
        mov    dword ptr [edx + 10h], edi
        mov    edi, dword ptr [ecx + 4]
        mov    dword ptr [edx + 14h], edi
        mov    edi, dword ptr [ecx + 8]
        mov    word ptr [edx + 18h], di
        mov    word ptr [edx + 2], ax
loc_1_0930:
        mov    word ptr [edx], 1ah
        pop    edi
        jmp    NEAR PTR loc_1_0130
sub_1_093B:
        db  08Bh, 00Dh, 01Ch, 000h, 000h, 000h
        mov    ecx, dword ptr [ecx + 2ch]
        or     ecx, ecx
        je     SHORT loc_1_0960
        mov    al, byte ptr [ebp + 14h]
loc_1_094B:
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_1_0955
        cmp    byte ptr [ecx + 5], al
        je     SHORT loc_1_0962
loc_1_0955:
        cmp    dword ptr [ecx + 1], 0
        je     SHORT loc_1_0960
        add    ecx, dword ptr [ecx + 1]
        jmp    SHORT loc_1_094B
loc_1_0960:
        stc
        ret
loc_1_0962:
        test   byte ptr [ecx + 0ch], 1
        je     SHORT loc_1_0960
        lea    eax, [ecx + 0fh]
        ret
        db  053h, 000h, 000h, 000h, 0B3h, 000h, 000h, 000h, 0A0h, 000h, 000h, 000h, 05Ah, 000h, 000h, 000h
        db  05Bh, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 020h, 000h, 003h, 00Ah, 000h, 000h, 049h, 04Eh, 054h, 031h
        db  033h, 020h, 020h, 020h, 000h, 000h, 000h, 042h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  06Ch, 009h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_09E4:
        add    eax, 0
loc_1_09E9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A2B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A2F:
        db  000h, 0B6h, 001h, 000h, 000h, 0E6h
        db  001h, 000h, 000h, 000h, 002h, 000h, 000h, 0FAh, 001h, 000h, 000h, 020h, 004h, 000h, 000h, 040h
        db  004h, 000h, 000h, 008h, 006h, 000h, 000h, 008h, 006h, 000h, 000h, 089h, 004h, 000h, 000h, 008h
        db  006h, 000h, 000h, 008h, 006h, 000h, 000h, 008h, 006h, 000h, 000h, 030h, 001h, 000h, 000h, 030h
        db  001h, 000h, 000h, 008h, 006h, 000h, 000h, 008h, 006h, 000h, 000h, 030h, 001h, 000h, 000h, 030h
        db  001h, 000h, 000h, 008h, 006h, 000h, 000h, 008h, 006h, 000h, 000h, 030h, 001h, 000h, 000h, 03Ah
        db  005h, 000h, 000h, 07Ah, 005h, 000h, 000h, 008h, 006h, 000h, 000h, 0D2h, 005h, 000h, 000h, 030h
        db  001h, 000h, 000h, 008h, 006h, 000h, 000h, 088h, 007h, 000h, 000h, 0A5h, 007h, 000h, 000h, 0C3h
        db  007h, 000h, 000h, 0FEh, 007h, 000h, 000h, 024h, 008h, 000h, 000h, 06Dh, 008h, 000h, 000h, 0FEh
        db  007h, 000h, 000h, 0A7h, 008h, 000h, 000h, 089h, 005h, 000h, 000h, 090h, 090h, 000h, 000h
_LTEXT ends
        end

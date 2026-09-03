; vmm_pcode_15.asm -- VMM object 15 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_15_0000:
        movzx  eax, word ptr [edx + 56h]
        add    eax, 3
        shr    eax, 2
        jne    SHORT loc_15_0021
        db  0A1h, 018h, 0EFh, 000h, 000h
        cmp    eax, 0a00h
        jae    SHORT loc_15_001C
        mov    ax, 0a00h
loc_15_001C:
        sub    eax, 200h
loc_15_0021:
        mov    dword ptr [ebx + 34h], eax
        ret
        db  000h, 000h, 000h
sub_15_0028:
        pushad
        mov    eax, 4
        mov    ecx, 8
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_15_003E
        mov    dword ptr [ebx - 40h], esi
loc_15_003E:
        popad
        ret
loc_15_0040:
        pushad
        mov    eax, 20007h
        xor    edx, edx
        xor    ebx, ebx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        call   sub_15_0880
        or     eax, eax
        db  00Fh, 084h, 0CCh, 000h, 0F1h, 0FFh
        mov    ebx, eax
        db  0E8h, 08Ch, 009h, 0F1h, 0FFh
        db  00Fh, 082h, 0B9h, 000h, 0F1h, 0FFh
        mov    dword ptr [esp], edi
        call   sub_15_01DC
        db  00Fh, 082h, 0A6h, 000h, 0F1h, 0FFh
        call   sub_15_0110
        db  00Fh, 082h, 096h, 000h, 0F1h, 0FFh
        call   sub_15_0028
        db  00Fh, 082h, 086h, 000h, 0F1h, 0FFh
        call   sub_15_00F8
        call   sub_15_0208
        call   sub_15_01E8
        mov    dword ptr [esp + 10h], ebx
        xor    eax, eax
        xor    edx, edx
        xor    ebx, ebx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        popad
        clc
        ret
loc_15_00B6:
        pushad
        call   sub_15_0AAC
        jb     SHORT loc_15_00CE
        xor    eax, eax
        xor    edx, edx
        xor    ebx, ebx
        xor    ecx, ecx
        inc    ecx
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
        clc
loc_15_00CE:
        popad
        ret
loc_15_00D0:
        or     byte ptr [ebx + 1ch], 8
        ret
sub_15_00D5:
        mov    eax, 8
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        mov    eax, 9
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        ret
sub_15_00F8:
        push   eax
        db  066h, 0A1h, 090h, 09Bh, 000h, 000h
        mov    word ptr [ebx - 32h], ax
        db  0A1h, 092h, 09Bh, 000h, 000h
        mov    dword ptr [ebx - 30h], eax
        pop    eax
        ret
        db  000h, 000h, 000h
sub_15_0110:
        pushad
        or     dword ptr [ebx - 4], 0ffffffffh
        xor    eax, eax
        inc    eax
        mov    ecx, 18h
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        jb     SHORT loc_15_015E
        mov    dword ptr [ebx + 20h], esi
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0E8h, 031h, 0FFh, 0F0h, 0FFh
        jb     SHORT loc_15_0155
        mov    edi, ebx
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        cmp    dword ptr [edi + 2ch], ebx
        jne    SHORT loc_15_0152
        mov    eax, 7d0h
        db  0BEh, 0ADh, 023h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
loc_15_0152:
        clc
        popad
        ret
loc_15_0155:
        mov    esi, dword ptr [ebx + 20h]
        db  0CDh, 020h, 09Ch, 000h, 001h, 000h
loc_15_015E:
        stc
        popad
        ret
        db  000h, 000h, 000h
Get_Environment_String:
        push   eax
        push   ecx
        push   esi
        push   edi
        db  08Bh, 03Dh, 054h, 001h, 000h, 000h
        test   edi, edi
        je     SHORT loc_15_0199
        mov    edx, edi
        test   esi, esi
        je     SHORT loc_15_019A
loc_15_0178:
        cmp    byte ptr [edi], 0
        je     SHORT loc_15_0199
        call   sub_15_01A5
        jb     SHORT loc_15_0191
        cmp    byte ptr [ecx + edi], 3dh
        jne    SHORT loc_15_0191
        lea    edx, [ecx + edi + 1]
        clc
        jmp    SHORT loc_15_019A
loc_15_0191:
        xor    eax, eax
        cld
loc_15_0194:
        scasb
        jne    SHORT loc_15_0194
        jmp    SHORT loc_15_0178
loc_15_0199:
        stc
loc_15_019A:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret
Get_PSP_Segment:
        db  0A1h, 050h, 001h, 000h, 000h
        ret
sub_15_01A5:
        push   eax
        push   ebx
        push   esi
        push   edi
        xor    ecx, ecx
        cld
loc_15_01AC:
        lodsb
        test   al, al
        je     SHORT loc_15_01CA
        call   sub_15_01D0
        mov    bl, al
        mov    al, byte ptr [edi]
        inc    edi
        call   sub_15_01D0
        cmp    al, bl
        jne    SHORT loc_15_01C7
        inc    ecx
        jmp    SHORT loc_15_01AC
loc_15_01C7:
        stc
        jmp    SHORT loc_15_01CB
loc_15_01CA:
        clc
loc_15_01CB:
        pop    edi
        pop    esi
        pop    ebx
        pop    eax
        ret
sub_15_01D0:
        cmp    al, 61h
        jb     SHORT loc_15_01DA
        cmp    al, 7ah
        ja     SHORT loc_15_01DA
        sub    al, 20h
loc_15_01DA:
        ret
        db  000h
sub_15_01DC:
        push   edi
        mov    edi, dword ptr [ebx - 20h]
        db  0E8h, 047h, 0FFh, 0F0h, 0FFh
        pop    edi
        ret
        db  000h
sub_15_01E8:
        lea    ecx, [ebx + 38h]
        mov    dword ptr [ecx], ecx
        mov    dword ptr [ecx + 4], ecx
        mov    dword ptr [ecx + 8], 0ffffffffh
        push   edi
        db  0CDh, 020h, 00Dh, 001h, 001h, 000h
        db  0E8h, 0A5h, 0FFh, 0F0h, 0FFh
        pop    edi
        ret
        db  000h, 000h, 000h
sub_15_0208:
        pushad
        db  0BEh, 064h, 0E3h, 000h, 000h
        lea    edi, [ebx + 48h]
        mov    ecx, 1ch
        cld
        rep movsd
        popad
        ret
        db  000h
loc_15_021C:
        cmp    dword ptr [ebx + 14h], 0
        jne    SHORT loc_15_022C
        test   byte ptr [edx + 2], 4
        je     SHORT loc_15_022C
        or     byte ptr [ebx + 27h], 80h
loc_15_022C:
        ret
        db  000h, 000h, 000h
sub_15_0230:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 20h]
        mov    eax, edi
        shr    eax, 0ch
        mov    dword ptr [esp + 14h], eax
        mov    ebx, dword ptr [esp + 28h]
        push   ebx
        push   1
        push   dword ptr [esp + 1ch]
        db  0E8h, 068h, 016h, 0F1h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_15_0307
        test   bh, 1
        jne    SHORT loc_15_02C9
        mov    esi, 74h
        mov    dword ptr [edi + 4], 0
        mov    word ptr [edi + 72h], 4948h
        mov    byte ptr [edi + 70h], bl
        test   bl, 8
        jne    SHORT loc_15_028B
        lea    eax, [edi + 50h]
        mov    dword ptr [edi + 4ch], eax
        push   1
        push   0
        push   eax
        db  0E8h, 0B3h, 009h, 0F1h, 0FFh
loc_15_028B:
        lea    ecx, [edi + 48h]
        lea    eax, [edi + 8]
        db  0BAh, 0B4h, 0F7h, 000h, 000h
        cmp    ecx, eax
        jbe    SHORT loc_15_02BB
loc_15_029A:
        mov    ebp, dword ptr [edx]
        mov    dword ptr [eax], ebp
        mov    dword ptr [eax + 4], 0a0000001h
        lea    ebp, [eax + 14h]
        mov    dword ptr [eax + 8], ebp
        lea    ebp, [eax - 0ch]
        mov    dword ptr [eax + 0ch], ebp
        add    eax, 10h
        add    edx, 4
        cmp    ecx, eax
        ja     SHORT loc_15_029A
loc_15_02BB:
        lea    eax, [edi + 3ch]
        mov    dword ptr [edi + 14h], eax
        lea    eax, [edi + 0ch]
        mov    dword ptr [edi + 40h], eax
        jmp    SHORT loc_15_02CE
loc_15_02C9:
        mov    esi, 8
loc_15_02CE:
        mov    ebp, dword ptr [esp + 24h]
        mov    dword ptr [edi], ebp
        lea    eax, [ebp + edi - 4]
        mov    dword ptr [esp + 10h], eax
        cmp    ebp, 1000h
        jbe    SHORT loc_15_0315
        push   ebx
        push   1
        shr    eax, 0ch
        push   eax
        db  0E8h, 0CCh, 015h, 0F1h, 0FFh
        or     eax, eax
        jne    SHORT loc_15_0315
        push   20000000h
        push   1
        push   dword ptr [esp + 1ch]
        db  0E8h, 0AAh, 01Ah, 0F1h, 0FFh
        add    esp, 0ch
loc_15_0307:
        sub    edi, edi
loc_15_0309:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    10h
loc_15_0315:
        mov    eax, dword ptr [esp + 10h]
        test   bh, 4
        mov    dword ptr [eax], 0a0000000h
        jne    SHORT loc_15_035A
        cmp    ebp, 1000h
        jbe    SHORT loc_15_035A
        sub    ebp, esi
        lea    eax, [esi + ebp - 0ffch]
        add    eax, edi
        sub    ebp, 0ffch
        mov    dword ptr [eax], 0a0000004h
        push   0
        push   0ff4h
        add    eax, 4
        mov    ebx, dword ptr [esp + 24h]
        push   eax
        push   ebx
        db  0E8h, 0B7h, 015h, 0F1h, 0FFh
        jmp    SHORT loc_15_0363
loc_15_035A:
        sub    ebp, esi
        sub    ebp, 4
        mov    ebx, dword ptr [esp + 1ch]
loc_15_0363:
        push   0
        push   ebp
        lea    eax, [edi + esi]
        push   eax
        push   ebx
        db  0E8h, 09Fh, 015h, 0F1h, 0FFh
        jmp    SHORT loc_15_0309
sub_15_0372:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 14h]
        push   0
        push   esi
        db  0E8h, 045h, 016h, 0F1h, 0FFh
        or     eax, eax
        jne    SHORT loc_15_038F
        sub    edi, edi
        jmp    NEAR PTR loc_15_049B
loc_15_038F:
        mov    ebx, dword ptr [esi]
        movzx  eax, byte ptr [esi + 70h]
        push   eax
        mov    edi, dword ptr [esp + 1ch]
        push   ebx
        push   edi
        push   edi
        call   sub_15_0230
        or     eax, eax
        je     NEAR PTR loc_15_0493
        lea    eax, [ebx + esi]
        sub    eax, 0ff8h
        mov    dword ptr [esp + 0ch], eax
        mov    ebx, dword ptr [esi + 14h]
        mov    eax, dword ptr [ebx]
        and    eax, 0ffffffch
        sub    eax, dword ptr [esp + 0ch]
        add    eax, ebx
        cmp    eax, -4
        je     SHORT loc_15_03D2
        mov    ebx, dword ptr [esp + 0ch]
        sub    ebx, 4
loc_15_03D2:
        mov    eax, dword ptr [esp + 0ch]
        test   dword ptr [eax], 0ffffffch
        je     SHORT loc_15_0410
loc_15_03DE:
        mov    eax, dword ptr [esp + 0ch]
        push   1
        mov    eax, dword ptr [eax]
        and    eax, 0ffffffch
        sub    eax, 4
        push   eax
        push   edi
        db  0E8h, 0EBh, 010h, 0F1h, 0FFh
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffffch
        add    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esp + 0ch]
        test   dword ptr [eax], 0ffffffch
        jne    SHORT loc_15_03DE
loc_15_0410:
        lea    eax, [esi + 74h]
        mov    dword ptr [esp + 0ch], eax
        cmp    ebx, eax
        je     SHORT loc_15_044B
loc_15_041B:
        mov    eax, dword ptr [esp + 0ch]
        push   1
        mov    eax, dword ptr [eax]
        and    eax, 0ffffffch
        sub    eax, 4
        push   eax
        push   edi
        db  0E8h, 0AEh, 010h, 0F1h, 0FFh
        or     eax, eax
        je     SHORT loc_15_0493
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffffch
        add    dword ptr [esp + 0ch], eax
        cmp    ebx, dword ptr [esp + 0ch]
        jne    SHORT loc_15_041B
loc_15_044B:
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, esi
        sub    eax, dword ptr [esp + 0ch]
        cmp    eax, -0ch
        je     SHORT loc_15_0495
loc_15_045D:
        mov    eax, dword ptr [esp + 0ch]
        test   dword ptr [eax], 0ffffffch
        je     SHORT loc_15_047B
        cmp    ebx, eax
        je     SHORT loc_15_047B
        sub    eax, esi
        add    eax, edi
        add    eax, 4
        push   eax
        push   edi
        db  0E8h, 0D5h, 00Fh, 0F1h, 0FFh
loc_15_047B:
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, esi
        sub    eax, dword ptr [esp + 0ch]
        cmp    eax, -0ch
        jne    SHORT loc_15_045D
        jmp    SHORT loc_15_0495
loc_15_0493:
        sub    edi, edi
loc_15_0495:
        push   esi
        db  0E8h, 097h, 015h, 0F1h, 0FFh
loc_15_049B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_15_04B0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        lea    esi, [eax*4 - 800000h]
        push   0
        push   1
        mov    eax, esi
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_15_0510
        and    esi, 0fffff3ffh
        shr    esi, 0ah
        db  08Bh, 00Dh, 0E4h, 0F7h, 000h, 000h
        mov    edx, dword ptr [esi - 800000h]
        shr    edx, 0ch
        imul   edx, edx, 0dh
        add    ecx, edx
        cmp    word ptr [ecx + 8], 0f188h
        je     SHORT loc_15_0510
        mov    word ptr [ecx + 8], 0f188h
        mov    ecx, dword ptr [ecx]
        mov    byte ptr [ecx + 2], 4
        db  0FFh, 005h, 088h, 0EEh, 000h, 000h
        db  0FFh, 00Dh, 084h, 0EEh, 000h, 000h
loc_15_0510:
        pop    esi
        ret    4
_GetLastV86Page:
        db  08Bh, 005h, 070h, 0F6h, 000h, 000h
        ret
_GetAppFlatDSAlias:
        db  00Fh, 0B7h, 005h, 06Ah, 0F6h, 000h, 000h
        ret
        db  000h
sub_15_0524:
        enter  0, 0
        push   esi
        push   edi
        push   edx
        push   ecx
        push   eax
        mov    edx, eax
        and    eax, 1fh
        shr    edx, 5
loc_15_0535:
        test   byte ptr [ebp + 8], 4
        je     SHORT loc_15_0541
        bsf    esi, dword ptr [ebx + edx*4]
        je     SHORT loc_15_0583
loc_15_0541:
        xor    esi, esi
        dec    esi
        push   ecx
        sub    ecx, 20h
        jae    SHORT loc_15_054E
        neg    ecx
        shr    esi, cl
loc_15_054E:
        pop    ecx
        xchg   cl, al
        shl    esi, cl
        xchg   cl, al
        test   byte ptr [ebp + 8], 4
        jne    SHORT loc_15_057E
        test   byte ptr [ebp + 8], 2
        jne    SHORT loc_15_0573
        test   byte ptr [ebp + 8], 1
        jne    SHORT loc_15_056E
        not    esi
        and    dword ptr [ebx + edx*4], esi
        jmp    SHORT loc_15_0583
loc_15_056E:
        or     dword ptr [ebx + edx*4], esi
        jmp    SHORT loc_15_0583
loc_15_0573:
        mov    edi, dword ptr [ebx + edx*4]
        and    edi, esi
        cmp    edi, esi
        jne    SHORT loc_15_059B
        jmp    SHORT loc_15_0583
loc_15_057E:
        test   dword ptr [ebx + edx*4], esi
        jne    SHORT loc_15_059B
loc_15_0583:
        sub    eax, 20h
        neg    eax
        sub    ecx, eax
        jbe    SHORT loc_15_0591
        xor    eax, eax
        inc    edx
        jmp    SHORT loc_15_0535
loc_15_0591:
        clc
loc_15_0592:
        pop    eax
        pop    ecx
        pop    edx
        pop    edi
        pop    esi
        leave
        ret    4
loc_15_059B:
        stc
        jmp    SHORT loc_15_0592
_Get_Device_V86_Pages_Array:
        push   edi
        push   esi
        db  0BEh, 0C0h, 0F0h, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        test   eax, eax
        je     SHORT loc_15_05B3
        lea    esi, [eax + 178h]
loc_15_05B3:
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, 9
        cld
        rep movsd
        inc    eax
        pop    esi
        pop    edi
        ret
_Assign_Device_V86_Pages:
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        call   sub_15_06CF
        jb     NEAR PTR loc_15_0670
        je     SHORT loc_15_05FE
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     NEAR PTR loc_15_0670
        push   4
        call   sub_15_0524
        jb     SHORT loc_15_0670
        push   1
        call   sub_15_0524
        jmp    SHORT loc_15_066A
loc_15_05FE:
        db  080h, 03Dh, 068h, 0F6h, 000h, 000h, 000h
        je     SHORT loc_15_0674
        push   4
        call   sub_15_0524
        jb     SHORT loc_15_0670
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
loc_15_0616:
        push   ebx
        lea    ebx, [ebx + 178h]
        push   4
        call   sub_15_0524
        pop    ebx
        jb     SHORT loc_15_0670
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_15_0616
        db  0BBh, 0C0h, 0F0h, 000h, 000h
        push   1
        call   sub_15_0524
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
loc_15_0647:
        push   ebx
        lea    ebx, [ebx + 178h]
        push   1
        call   sub_15_0524
        pop    ebx
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        jne    SHORT loc_15_0647
loc_15_0664:
        or     byte ptr [esi], 10h
        inc    esi
        loop   loc_15_0664
loc_15_066A:
        mov    al, 1
loc_15_066C:
        pop    ebx
        pop    esi
        pop    edi
        ret
loc_15_0670:
        xor    eax, eax
        jmp    SHORT loc_15_066C
loc_15_0674:
        db  03Bh, 03Dh, 06Ch, 0F6h, 000h, 000h
        jbe    SHORT loc_15_0670
        db  03Bh, 03Dh, 070h, 0F6h, 000h, 000h
        ja     SHORT loc_15_06AA
        push   edi
        push   ecx
        push   edx
        push   eax
        add    edi, ecx
        db  03Bh, 03Dh, 070h, 0F6h, 000h, 000h
        ja     SHORT loc_15_06A4
        db  08Bh, 00Dh, 070h, 0F6h, 000h, 000h
        inc    ecx
        sub    ecx, edi
        mov    eax, edi
        push   2
        call   sub_15_0524
loc_15_06A4:
        pop    eax
        pop    edx
        pop    ecx
        pop    edi
        jb     SHORT loc_15_0670
loc_15_06AA:
        push   4
        call   sub_15_0524
        jb     SHORT loc_15_0670
        push   1
        call   sub_15_0524
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        lea    ebx, [ebx + 178h]
        push   1
        call   sub_15_0524
        jmp    SHORT loc_15_0664
sub_15_06CF:
        jecxz  loc_15_0703
        cmp    edi, 10fh
        ja     SHORT loc_15_0703
        mov    eax, ecx
        add    eax, edi
        dec    eax
        cmp    eax, 10fh
        ja     SHORT loc_15_0703
        mov    esi, edi
        db  081h, 0C6h, 0B0h, 0EFh, 000h, 000h
        mov    eax, edi
        mov    edx, edi
        or     ebx, ebx
        je     SHORT loc_15_06FC
        lea    ebx, [ebx + 178h]
loc_15_06FB:
        ret
loc_15_06FC:
        db  0BBh, 0C0h, 0F0h, 000h, 000h
        jmp    SHORT loc_15_06FB
loc_15_0703:
        stc
        jmp    SHORT loc_15_06FB
sub_15_0706:
        push   ebx
        push   edi
        mov    ebx, dword ptr [esp + 0ch]
        or     eax, 0ffffffffh
        xor    ecx, ecx
        db  00Ah, 00Dh, 071h, 003h, 000h, 000h
        je     SHORT loc_15_0766
        xor    eax, eax
        mov    edi, ecx
        push   100h
        push   eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   0
        push   edi
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        test   eax, eax
        je     SHORT loc_15_0766
        xor    ecx, ecx
        db  08Ah, 00Dh, 070h, 003h, 000h, 000h
        xor    edx, edx
        xchg   edi, eax
        push   edx
        push   edx
        push   eax
        push   ecx
        push   ebx
        push   edi
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_15_0766
        push   esi
        db  08Bh, 035h, 068h, 003h, 000h, 000h
        db  08Bh, 00Dh, 06Ch, 003h, 000h, 000h
        cld
        rep movsd
        pop    esi
loc_15_0766:
        pop    edi
        pop    ebx
        ret    4
_Allocate_Device_CB_Area:
        push   5
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0CDh, 020h, 02Eh, 001h, 001h, 000h
        pop    ecx
        pop    ecx
        push   ebx
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        push   1
        push   dword ptr [esp + 0ch]
        push   dword ptr [ebx - 70h]
        db  0E8h, 050h, 00Dh, 0F1h, 0FFh
        or     eax, eax
        je     SHORT loc_15_07E3
        sub    eax, ebx
        push   eax
loc_15_0797:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 010h, 0F7h, 000h, 000h
        je     SHORT loc_15_07E2
        push   1
        push   dword ptr [esp + 10h]
        push   dword ptr [ebx - 70h]
        db  0E8h, 02Dh, 00Dh, 0F1h, 0FFh
        or     eax, eax
        je     SHORT loc_15_07BE
        sub    eax, ebx
        cmp    eax, dword ptr [esp]
        je     SHORT loc_15_0797
loc_15_07BE:
        push   ebx
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
loc_15_07C5:
        mov    eax, dword ptr [esp + 4]
        add    eax, ebx
        push   eax
        push   dword ptr [ebx - 70h]
        db  0E8h, 07Ch, 00Ch, 0F1h, 0FFh
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_15_07C5
        pop    ecx
        xor    eax, eax
loc_15_07E2:
        pop    ecx
loc_15_07E3:
        pop    ebx
        push   eax
        db  0FFh, 035h, 09Ch, 0FDh, 000h, 000h
        db  0CDh, 020h, 02Fh, 001h, 001h, 000h
        pop    ecx
        pop    eax
        ret
sub_15_07F4:
        push   ebx
        push   esi
        push   edi
        db  0FFh, 005h, 078h, 003h, 000h, 000h
        db  083h, 03Dh, 078h, 003h, 000h, 000h, 001h
        ja     SHORT loc_15_082F
        push   0
        db  0FFh, 035h, 054h, 0F6h, 000h, 000h
        db  0CDh, 020h, 0A7h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_15_0836
        db  0A3h, 058h, 0F6h, 000h, 000h
        db  08Bh, 01Dh, 010h, 0F7h, 000h, 000h
        add    eax, ebx
        mov    dword ptr [ebx + 30h], eax
        call   sub_15_0840
loc_15_082F:
        or     eax, 1
loc_15_0832:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_15_0836:
        db  0FFh, 00Dh, 078h, 003h, 000h, 000h
        xor    eax, eax
        jmp    SHORT loc_15_0832
sub_15_0840:
        db  08Bh, 015h, 0E4h, 0F0h, 000h, 000h
        db  00Fh, 0B6h, 01Dh, 0F8h, 0F1h, 000h, 000h
loc_15_084D:
        cmp    dword ptr [edx + 10h], 0
        je     SHORT loc_15_086E
        mov    esi, dword ptr [edx + 14h]
        shl    esi, 0ch
        cmp    dword ptr [edx + 14h], 100h
        jb     SHORT loc_15_0864
        mov    esi, dword ptr [edx]
loc_15_0864:
        mov    edi, dword ptr [edx + 4]
        add    edi, eax
        xor    ecx, ecx
        call   dword ptr [edx + 8]
loc_15_086E:
        add    edx, 18h
        dec    ebx
        jne    SHORT loc_15_084D
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_15_0880:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        call   sub_15_07F4
        cmp    eax, ebx
        je     NEAR PTR loc_15_0A9F
        push   19h
        db  0A1h, 050h, 0F6h, 000h, 000h
        add    eax, 120h
        push   eax
        push   80080000h
        db  0FFh, 015h, 09Ch, 0D7h, 000h, 000h
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_15_0A96
        mov    ebp, eax
        db  08Bh, 00Dh, 010h, 0F7h, 000h, 000h
        shr    ebp, 0ch
        mov    eax, dword ptr [ecx + 4]
        shr    eax, 0ch
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esp + 10h]
        add    eax, 120000h
        push   eax
        push   dword ptr [ecx - 70h]
        call   sub_15_0372
        cmp    eax, ebx
        je     NEAR PTR loc_15_0A87
        mov    ebx, dword ptr [esp + 10h]
        add    ebx, 1200e8h
        lea    ecx, [ebp*4 - 800000h]
        mov    dword ptr [ebx + 10h], 62634d56h
        mov    dword ptr [ebx + 34h], 10000000h
        mov    dword ptr [ebx - 10h], ecx
        mov    dword ptr [ebx - 70h], eax
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 4], ecx
        db  0FFh, 035h, 060h, 0F6h, 000h, 000h
        db  0FFh, 035h, 064h, 0F6h, 000h, 000h
        db  0E8h, 075h, 02Eh, 0F1h, 0FFh
        mov    dword ptr [ebx + 0ch], eax
        or     eax, eax
        je     NEAR PTR loc_15_0A74
        db  08Bh, 03Dh, 058h, 0F6h, 000h, 000h
        add    edi, ebx
        mov    dword ptr [ebx + 30h], edi
        db  0A1h, 054h, 0F6h, 000h, 000h
        db  08Bh, 035h, 05Ch, 0F6h, 000h, 000h
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 0C0h, 0F0h, 000h, 000h
        lea    edi, [ebx + 178h]
        mov    ecx, 8
        rep movsd
        movsw
        push   ebp
        call   sub_15_04B0
        or     eax, eax
        je     NEAR PTR loc_15_0A74
        lea    eax, [ebp*4 - 800000h]
        and    eax, 0fffff3ffh
        shr    eax, 0ah
        mov    eax, dword ptr [eax - 800000h]
        mov    dword ptr [ebx - 0ch], eax
        db  0FFh, 035h, 06Ch, 0F6h, 000h, 000h
        push   ebp
        push   dword ptr [esp + 1ch]
        db  0E8h, 06Ah, 056h, 0F1h, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_15_0A74
        push   0
        db  08Bh, 00Dh, 070h, 0F6h, 000h, 000h
        db  0A1h, 06Ch, 0F6h, 000h, 000h
        sub    ecx, eax
        inc    ecx
        push   ecx
        add    eax, ebp
        push   eax
        db  0E8h, 017h, 056h, 0F1h, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_15_0A74
        mov    eax, 0ffh
        db  08Bh, 00Dh, 070h, 0F6h, 000h, 000h
        sub    eax, ecx
        push   eax
        lea    eax, [ecx + ebp]
        inc    eax
        add    ecx, dword ptr [esp + 18h]
        push   eax
        inc    ecx
        push   ecx
        db  0E8h, 01Dh, 056h, 0F1h, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_15_0A74
        push   10h
        lea    eax, [ebp + 100h]
        push   eax
        push   ebp
        db  0E8h, 007h, 056h, 0F1h, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_15_0A74
        push   10h
        add    ebp, 110h
        mov    eax, dword ptr [esp + 18h]
        push   ebp
        add    eax, 110h
        push   eax
        db  0E8h, 0E8h, 055h, 0F1h, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_15_0A74
        sub    eax, eax
        sub    edx, edx
loc_15_0A30:
        db  038h, 090h, 0E8h, 0F0h, 000h, 000h
        je     SHORT loc_15_0A3F
        mov    ecx, dword ptr [ebx - 10h]
        and    dword ptr [ecx + eax*4], 0fffffffeh
loc_15_0A3F:
        inc    eax
        cmp    eax, 100h
        jl     SHORT loc_15_0A30
        mov    eax, 100h
        sub    edx, edx
        mov    ecx, 0fffffffeh
loc_15_0A53:
        db  038h, 090h, 0E8h, 0F0h, 000h, 000h
        je     SHORT loc_15_0A62
        mov    esi, dword ptr [ebx - 10h]
        and    dword ptr [esi + eax*4 + 40h], ecx
loc_15_0A62:
        inc    eax
        cmp    eax, 110h
        jl     SHORT loc_15_0A53
        push   ebx
        call   sub_15_0706
        or     eax, eax
        jne    SHORT loc_15_0A9F
loc_15_0A74:
        mov    eax, dword ptr [ebx + 0ch]
        or     eax, eax
        je     SHORT loc_15_0A87
        push   eax
        db  0FFh, 035h, 064h, 0F6h, 000h, 000h
        db  0E8h, 064h, 02Dh, 0F1h, 0FFh
loc_15_0A87:
        push   10h
        push   dword ptr [esp + 14h]
        db  0FFh, 015h, 07Ch, 0D4h, 000h, 000h
        add    esp, 8
loc_15_0A96:
        push   0
        db  0E8h, 03Bh, 0F7h, 0F0h, 0FFh
        sub    ebx, ebx
loc_15_0A9F:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  000h, 000h, 000h
sub_15_0AAC:
        and    dword ptr [ebx], 0fffffffbh
        and    dword ptr [edi], 0fffffeffh
        push   eax
        push   0
        push   edi
        db  0E8h, 047h, 084h, 0F1h, 0FFh
        test   eax, eax
        pop    eax
        jne    SHORT loc_15_0AC4
        stc
loc_15_0AC4:
        ret
loc_15_0AC5:
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    dword ptr [ebx - 6ch], 1
        mov    byte ptr [ebx - 68h], 0
        mov    dword ptr [ebp + 2ch], 23000h
        db  0A1h, 018h, 0F7h, 000h, 000h
        mov    word ptr [ebp + 24h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 28h], ax
        db  0A1h, 080h, 0F6h, 000h, 000h
        mov    word ptr [ebp + 30h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 34h], ax
        call   sub_15_00D5
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        db  0E9h, 0E8h, 0F7h, 0F0h, 0FFh
_PTEXT ends
        end
